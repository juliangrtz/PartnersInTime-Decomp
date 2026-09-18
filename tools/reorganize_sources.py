#!/usr/bin/env python3
"""Rename and merge matching translation units, keeping the metadata consistent.

A source file in this project is a translation unit that owns exactly one
contiguous native address range per section, declared in the component's
``delinks.txt``.  Renaming or merging such a unit therefore touches several
files at once, and doing it by hand is how stale metadata gets introduced.

This tool applies a *plan* of operations::

    [
      {
        "target": "src/game/window_text_layout.c",
        "members": ["src/game/window_alignment.c", "src/game/window_voice.c"]
      },
      {
        "target": "src/pause_ov007/items.c",
        "members": ["src/overlay007/items.c"]
      }
    ]

For every operation it

* checks that all members belong to the same component,
* checks that their ranges are contiguous in every section they declare, in the
  same order, so the merged unit still owns one unbroken range (DSD rejects a
  unit that declares the same section twice),
* rewrites ``delinks.txt`` so the merged range replaces the member entries,
* rewrites ``linked_sources.txt`` and ``linker_aliases.json``,
* moves the files with ``git mv`` (single member) or concatenates them.

Concatenation order is **descending** native address, because MWCC 1.2 emits
functions in reverse source order: the resulting object must still list its
functions in ascending address order.  The tool only concatenates; deduplicating
the includes and writing the module comment is a follow-up editing step.

Run without ``--apply`` to validate a plan and print what would change.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

SECTION_RE = re.compile(
    r"^(?P<indent>\s+)(?P<section>\.\S+)\s+"
    r"start:(?P<start>0x[0-9a-fA-F]+)\s+end:(?P<end>0x[0-9a-fA-F]+)"
    r"(?P<rest>.*)$"
)
HEADING_RE = re.compile(r"^(?P<name>\S.*):\s*$")


@dataclass
class Block:
    """One ``path:`` heading plus the section lines under it, or free text."""

    name: str | None
    lines: list[str]
    sections: dict[str, tuple[int, int]] = field(default_factory=dict)


@dataclass
class Delinks:
    path: Path
    blocks: list[Block]
    newline: str

    def index_of(self, name: str) -> int:
        for index, block in enumerate(self.blocks):
            if block.name == name:
                return index
        raise KeyError(name)

    def get(self, name: str) -> Block | None:
        for block in self.blocks:
            if block.name == name:
                return block
        return None

    def render(self) -> list[str]:
        lines: list[str] = []
        for block in self.blocks:
            lines.extend(block.lines)
        return lines


def read_lines(path: Path) -> tuple[list[str], str]:
    """Read a text file as lines, remembering the dominant line terminator."""
    with path.open("r", encoding="utf-8-sig", newline="") as handle:
        raw = handle.read()
    newline = "\r\n" if raw.count("\r\n") * 2 >= raw.count("\n") else "\n"
    return raw.splitlines(), newline


def write_lines(path: Path, lines: list[str], newline: str) -> None:
    with path.open("w", encoding="utf-8", newline="") as handle:
        handle.write(newline.join(lines) + newline)


def parse_delinks(lines: list[str], path: Path | None = None, newline: str = "\n") -> Delinks:
    blocks: list[Block] = [Block(None, [])]
    for line in lines:
        heading = HEADING_RE.match(line)
        if heading and not line.lstrip().startswith("#"):
            blocks.append(Block(heading.group("name").strip(), [line]))
            continue
        block = blocks[-1]
        block.lines.append(line)
        section = SECTION_RE.match(line)
        if section and block.name is not None:
            block.sections[section.group("section")] = (
                int(section.group("start"), 16),
                int(section.group("end"), 16),
            )
    return Delinks(path or Path("<memory>"), blocks, newline)


def load_delinks(path: Path) -> Delinks:
    lines, newline = read_lines(path)
    return parse_delinks(lines, path, newline)


def component_delinks(version: str) -> dict[str, Delinks]:
    config = ROOT / "config" / version / "arm9"
    found: dict[str, Delinks] = {}
    candidates = [
        ("arm9", config / "delinks.txt"),
        ("itcm", config / "itcm" / "delinks.txt"),
        ("dtcm", config / "dtcm" / "delinks.txt"),
    ]
    overlays = config / "overlays"
    if overlays.is_dir():
        candidates += [
            (directory.name, directory / "delinks.txt")
            for directory in sorted(overlays.iterdir())
            if directory.is_dir()
        ]
    for key, path in candidates:
        if path.is_file():
            found[key] = load_delinks(path)
    return found


def git(*args: str) -> None:
    subprocess.run(["git", *args], cwd=ROOT, check=True)


class PlanError(Exception):
    pass


def validate(operation: dict, delinks: dict[str, Delinks]):
    """Return (component, members ordered by address, merged section ranges)."""
    target = operation["target"]
    members = operation["members"]
    if not members:
        raise PlanError(f"{target}: no members")

    owners = {}
    for member in members:
        hits = [key for key, value in delinks.items() if value.get(member) is not None]
        if not hits:
            raise PlanError(f"{target}: {member} has no delinks entry")
        if len(hits) > 1:
            raise PlanError(f"{target}: {member} appears in {hits}")
        owners[member] = hits[0]
    components = set(owners.values())
    if len(components) != 1:
        raise PlanError(f"{target}: members span components {sorted(components)}")
    component = components.pop()
    table = delinks[component]

    def first_address(member: str) -> int:
        sections = table.get(member).sections
        if not sections:
            raise PlanError(f"{target}: {member} declares no section")
        return min(start for start, _ in sections.values())

    ordered = sorted(members, key=first_address)
    if ordered != members:
        raise PlanError(
            f"{target}: members must be listed in ascending address order; expected {ordered}"
        )

    section_names: list[str] = []
    for member in ordered:
        for name in table.get(member).sections:
            if name not in section_names:
                section_names.append(name)

    merged: dict[str, tuple[int, int]] = {}
    for name in section_names:
        ranges = [
            table.get(member).sections[name]
            for member in ordered
            if name in table.get(member).sections
        ]
        for previous, following in zip(ranges, ranges[1:]):
            if previous[1] != following[0]:
                raise PlanError(
                    f"{target}: {name} is not contiguous "
                    f"({previous[1]:#x} then {following[0]:#x})"
                )
        merged[name] = (ranges[0][0], ranges[-1][1])

    if len(members) > 1:
        suffixes = {Path(member).suffix for member in members}
        if len(suffixes) > 1:
            raise PlanError(f"{target}: members mix languages {sorted(suffixes)}")
        if Path(target).suffix not in suffixes:
            raise PlanError(f"{target}: target extension differs from its members")
    return component, ordered, merged


def rewrite_delinks(table: Delinks, target: str, members: list[str],
                    merged: dict[str, tuple[int, int]]) -> None:
    """Replace the first member's block with the merged one, drop the others."""
    first = table.get(members[0])
    lines = [f"{target}:"]
    for name, (start, end) in merged.items():
        indent, rest = "    ", ""
        for original in first.lines:
            match = SECTION_RE.match(original)
            if match and match.group("section") == name:
                indent, rest = match.group("indent"), match.group("rest")
                break
        lines.append(f"{indent}{name} start:{start:#010x} end:{end:#010x}{rest}")
    trailing = [line for line in first.lines if not line.strip()]
    first.name = target
    first.lines = lines + trailing
    first.sections = dict(merged)

    for member in members[1:]:
        table.blocks.remove(table.get(member))


def rewrite_manifest(path: Path, target: str, members: list[str]) -> None:
    lines, newline = read_lines(path)
    out: list[str] = []
    replaced = False
    for line in lines:
        value = line.split("#", 1)[0].strip()
        if value in members:
            if not replaced:
                out.append(line.replace(value, target))
                replaced = True
            continue
        out.append(line)
    write_lines(path, out, newline)


def rewrite_aliases(path: Path, target: str, members: list[str]) -> None:
    if not path.is_file():
        return
    data = json.loads(path.read_text(encoding="utf-8"))
    changed = False
    for alias in data:
        if alias.get("source") in members:
            alias["source"] = target
            changed = True
    if changed:
        write_lines(path, (json.dumps(data, indent=2)).splitlines(), "\n")


def apply_operation(target: str, members: list[str]) -> None:
    """Move or concatenate the source files themselves, in Git."""
    target_path = ROOT / target
    if len(members) == 1:
        if members[0] == target:
            return
        target_path.parent.mkdir(parents=True, exist_ok=True)
        git("mv", members[0], target)
        return

    # Descending native address order: MWCC emits functions in reverse source order.
    chunks = [
        (ROOT / member).read_text(encoding="utf-8").replace("\r\n", "\n").rstrip("\n")
        for member in reversed(members)
    ]
    for member in members:
        git("rm", "-q", "-f", member)
    target_path.parent.mkdir(parents=True, exist_ok=True)
    write_lines(target_path, "\n\n".join(chunks).splitlines(), "\n")
    git("add", "--", target)


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("--version", default="eur")
    parser.add_argument("--plan", type=Path, required=True)
    parser.add_argument("--apply", action="store_true",
                        help="perform the moves; otherwise only validate and report")
    args = parser.parse_args()

    plan = json.loads(args.plan.read_text(encoding="utf-8"))
    delinks = component_delinks(args.version)
    config = ROOT / "config" / args.version / "arm9"

    validated = []
    errors: list[str] = []
    seen: set[str] = set()
    for operation in plan:
        try:
            component, members, merged = validate(operation, delinks)
        except PlanError as error:
            errors.append(str(error))
            continue
        target = operation["target"]
        if target in seen:
            errors.append(f"{target}: duplicate target")
            continue
        seen.add(target)
        validated.append((target, component, members, merged))

    # The Metrowerks linker selects objects by basename, so they stay unique.
    basenames: dict[str, str] = {}
    for path in list(ROOT.glob("src/**/*.c")) + list(ROOT.glob("src/**/*.cpp")):
        basenames[path.name] = str(path.relative_to(ROOT)).replace("\\", "/")
    for _, _, members, _ in validated:
        for member in members:
            basenames.pop(Path(member).name, None)
    for target, _, _, _ in validated:
        name = Path(target).name
        if name in basenames:
            errors.append(f"{target}: basename collides with {basenames[name]}")
        basenames[name] = target

    for message in errors:
        print(f"error: {message}", file=sys.stderr)
    if errors:
        return 1

    touched: set[str] = set()
    for target, component, members, merged in validated:
        ranges = " ".join(f"{name}={start:#x}..{end:#x}" for name, (start, end) in merged.items())
        print(f"{component}: {len(members)} -> {target}  {ranges}")
        if not args.apply:
            continue
        apply_operation(target, members)
        rewrite_delinks(delinks[component], target, members, merged)
        rewrite_manifest(config / "linked_sources.txt", target, members)
        rewrite_aliases(config / "linker_aliases.json", target, members)
        touched.add(component)

    if args.apply:
        for component in sorted(touched):
            table = delinks[component]
            write_lines(table.path, table.render(), table.newline)
        print(f"applied {len(validated)} operation(s)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
