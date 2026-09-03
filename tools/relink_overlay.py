#!/usr/bin/env python3
"""Relink one PiT ARM9 overlay from sectioned assembly translation units.

Raw, ROM-derived section fragments are generated below build/ and never belong
in Git. Maintained symbolic functions come from reasm/<version>/patches.json.
The tool links every unit at its verified runtime address and can place the
result into a copy of the user-supplied ROM.
"""

from __future__ import annotations

import argparse
import dataclasses
import hashlib
import json
import re
import struct
import sys
from pathlib import Path

import reassembly


ROOT = Path(__file__).resolve().parents[1]

SECTION_PATTERN = re.compile(
    r"^\s+\.(?P<name>[A-Za-z0-9_]+)\s+"
    r"start:0x(?P<start>[0-9A-Fa-f]+)\s+"
    r"end:0x(?P<end>[0-9A-Fa-f]+)\s+"
    r"kind:(?P<kind>[A-Za-z0-9_]+)\s+"
    r"align:(?P<align>[0-9]+)\s*$"
)
RELOCATION_PATTERN = re.compile(
    r"^from:0x(?P<source>[0-9A-Fa-f]+)\s+"
    r"kind:(?P<kind>[A-Za-z0-9_]+)\s+"
    r"to:0x(?P<target>[0-9A-Fa-f]+)\s+"
    r"(?:add:(?P<add>[-+]?0x[0-9A-Fa-f]+)\s+)?"
    r"module:(?P<module>.+?)\s*$"
)
SYMBOL_PATTERN = re.compile(
    r"^([A-Za-z_.$][A-Za-z0-9_.$]*)\s+.*\baddr:0x([0-9A-Fa-f]+)\b"
)


@dataclasses.dataclass(frozen=True)
class DsdSection:
    name: str
    start: int
    end: int
    kind: str
    align: int


@dataclasses.dataclass(frozen=True)
class Unit:
    name: str
    kind: str
    start: int
    end: int
    input_section: str
    source: Path
    maintained: bool

    @property
    def size(self) -> int:
        return self.end - self.start


@dataclasses.dataclass(frozen=True)
class RelinkResult:
    module: reassembly.Module
    payload: bytes
    unit_count: int
    maintained_unit_count: int
    relocation_count: int
    differing_bytes: int
    report: Path


def overlay_config(version: str, overlay_id: int) -> Path:
    return ROOT / "config" / version / "arm9" / "overlays" / f"ov{overlay_id:03d}"


def read_sections(path: Path) -> list[DsdSection]:
    if not path.is_file():
        raise reassembly.ReassemblyError(f"missing dsd section map: {path}")
    result = []
    for line in path.read_text(encoding="utf-8").splitlines():
        stripped = line.strip()
        if result and stripped and not stripped.startswith("."):
            break
        match = SECTION_PATTERN.match(line)
        if match:
            start = int(match.group("start"), 16)
            end = int(match.group("end"), 16)
            if end < start:
                raise reassembly.ReassemblyError(f"negative section in {path}: {line}")
            result.append(
                DsdSection(
                    name=match.group("name"),
                    start=start,
                    end=end,
                    kind=match.group("kind"),
                    align=int(match.group("align")),
                )
            )
    if not result:
        raise reassembly.ReassemblyError(f"no sections found in {path}")
    return result


def read_relocations(path: Path) -> list[dict]:
    result = []
    for line_number, line in enumerate(
        path.read_text(encoding="utf-8").splitlines(), start=1
    ):
        if not line.strip():
            continue
        match = RELOCATION_PATTERN.match(line)
        if not match:
            raise reassembly.ReassemblyError(
                f"unrecognized relocation at {path}:{line_number}: {line}"
            )
        result.append(
            {
                "source": int(match.group("source"), 16),
                "kind": match.group("kind"),
                "target": int(match.group("target"), 16),
                "add": int(match.group("add"), 0) if match.group("add") else 0,
                "module": match.group("module"),
            }
        )
    return result


def read_all_symbols(version: str) -> dict[str, int]:
    config_root = ROOT / "config" / version / "arm9"
    result: dict[str, int] = {}
    for path in config_root.rglob("symbols.txt"):
        for line in path.read_text(encoding="utf-8").splitlines():
            match = SYMBOL_PATTERN.match(line)
            if not match:
                continue
            name = match.group(1)
            address = int(match.group(2), 16)
            previous = result.setdefault(name, address)
            if previous != address:
                raise reassembly.ReassemblyError(
                    f"symbol {name} has conflicting addresses in dsd maps"
                )
    return result


def raw_intervals(
    module: reassembly.Module, sections: list[DsdSection]
) -> list[tuple[str, str, int, int]]:
    module_start = module.load_address
    module_end = module_start + module.size
    stored = [
        section
        for section in sections
        if section.kind != "bss"
        and section.end > module_start
        and section.start < module_end
    ]
    stored.sort(key=lambda section: section.start)
    result: list[tuple[str, str, int, int]] = []
    cursor = module_start
    gap_index = 0
    for section in stored:
        start = max(section.start, module_start)
        end = min(section.end, module_end)
        if start < cursor:
            raise reassembly.ReassemblyError(
                f"overlapping dsd sections near 0x{start:08X} in {module.name}"
            )
        if start > cursor:
            result.append((f"gap_{gap_index:03d}", "gap", cursor, start))
            gap_index += 1
        if end > start:
            result.append((section.name, section.kind, start, end))
        cursor = end
    if cursor < module_end:
        result.append((f"gap_{gap_index:03d}", "gap", cursor, module_end))
    if not result or result[0][2] != module_start or result[-1][3] != module_end:
        raise reassembly.ReassemblyError(f"section plan does not cover {module.name}")
    return result


def parse_patch_range(patch: dict) -> tuple[int, int]:
    start = int(str(patch["address"]), 0)
    size = int(str(patch["size"]), 0)
    return start, start + size


def plan_units(
    module: reassembly.Module,
    intervals: list[tuple[str, str, int, int]],
    patches: list[dict],
    generated_root: Path,
    maintained_root: Path,
) -> list[Unit]:
    patch_ranges = [(parse_patch_range(patch), patch) for patch in patches]
    patch_ranges.sort(key=lambda item: item[0][0])
    for index, ((start, end), patch) in enumerate(patch_ranges):
        if start < module.load_address or end > module.load_address + module.size:
            raise reassembly.ReassemblyError(
                f"maintained source {patch['source']} is outside {module.name}"
            )
        if index and patch_ranges[index - 1][0][1] > start:
            raise reassembly.ReassemblyError("maintained source ranges overlap")

    result: list[Unit] = []
    raw_index = 0
    consumed: set[str] = set()
    for interval_name, _kind, interval_start, interval_end in intervals:
        cursor = interval_start
        contained = [
            (bounds, patch)
            for bounds, patch in patch_ranges
            if interval_start <= bounds[0] and bounds[1] <= interval_end
        ]
        for (patch_start, patch_end), patch in contained:
            if patch_start > cursor:
                section = f".pit_raw_{raw_index:03d}"
                result.append(
                    Unit(
                        name=f"raw_{interval_name}_{raw_index:03d}",
                        kind=_kind,
                        start=cursor,
                        end=patch_start,
                        input_section=section,
                        source=generated_root / f"raw_{raw_index:03d}.s",
                        maintained=False,
                    )
                )
                raw_index += 1
            result.append(
                Unit(
                    name=Path(patch["source"]).stem,
                    kind="code",
                    start=patch_start,
                    end=patch_end,
                    input_section=patch["section"],
                    source=maintained_root / patch["source"],
                    maintained=True,
                )
            )
            consumed.add(patch["source"])
            cursor = patch_end
        if cursor < interval_end:
            section = f".pit_raw_{raw_index:03d}"
            result.append(
                Unit(
                    name=f"raw_{interval_name}_{raw_index:03d}",
                    kind=_kind,
                    start=cursor,
                    end=interval_end,
                    input_section=section,
                    source=generated_root / f"raw_{raw_index:03d}.s",
                    maintained=False,
                )
            )
            raw_index += 1
    missing = {patch["source"] for patch in patches}.difference(consumed)
    if missing:
        raise reassembly.ReassemblyError(
            "maintained sources are not contained in a stored dsd section: "
            + ", ".join(sorted(missing))
        )
    return result


def emit_raw_unit(unit: Unit, payload: bytes, cpu: str = "arm9") -> None:
    architecture = "armv4t" if cpu == "arm7" else "armv5te"
    flags = "ax" if unit.kind == "code" else "aw" if unit.kind == "data" else "a"
    lines = [
        "/* Generated from a user-supplied ROM. Do not commit. */",
        ".syntax unified",
        f".arch {architecture}",
        f'.section {unit.input_section}, "{flags}", %progbits',
        ".balign 1",
        f".global pit_{unit.name}_start",
        f"pit_{unit.name}_start:",
    ]
    word_end = len(payload) - len(payload) % 4
    for offset in range(0, word_end, 16):
        chunk = payload[offset : min(offset + 16, word_end)]
        words = struct.unpack("<" + "I" * (len(chunk) // 4), chunk)
        values = ", ".join(f"0x{word:08X}" for word in words)
        lines.append(f"    /* 0x{unit.start + offset:08X} */ .word {values}")
    if word_end != len(payload):
        values = ", ".join(f"0x{byte:02X}" for byte in payload[word_end:])
        lines.append(f"    /* 0x{unit.start + word_end:08X} */ .byte {values}")
    lines.extend([f"pit_{unit.name}_end:", ""])
    unit.source.parent.mkdir(parents=True, exist_ok=True)
    unit.source.write_text("\n".join(lines), encoding="utf-8", newline="\n")


def write_linker_script(
    path: Path,
    units: list[Unit],
    external_definitions: dict[str, int],
) -> None:
    lines = ["OUTPUT_FORMAT(elf32-littlearm)", "OUTPUT_ARCH(arm)", "SECTIONS", "{"]
    assertions = []
    for index, unit in enumerate(units):
        output_section = f".pit_out_{index:03d}"
        lines.append(
            f"  {output_section} 0x{unit.start:X} : "
            f"{{ KEEP(*({unit.input_section})) }}"
        )
        assertions.append(
            f'  ASSERT(SIZEOF({output_section}) == 0x{unit.size:X}, "{unit.name} size mismatch");'
        )
    lines.extend(["  /DISCARD/ : { *(.ARM.attributes) *(.comment) }", "}"])
    for name, value in sorted(external_definitions.items()):
        lines.append(f"PROVIDE({name} = 0x{value:X});")
    lines.extend(assertions)
    lines.append("")
    path.write_text("\n".join(lines), encoding="utf-8", newline="\n")


def relink_module(
    rom_data: bytes,
    identity: dict,
    version: str,
    module: reassembly.Module,
    config: Path,
    work: Path,
    output_binary: Path,
    require_matching: bool,
    announce: bool = True,
) -> RelinkResult:
    module_name = module.name
    sections = read_sections(config / "delinks.txt")
    relocations = read_relocations(config / "relocs.txt")
    known_symbols = read_all_symbols(version)
    patches = reassembly.load_maintained_patches(version).get(module_name, [])
    intervals = raw_intervals(module, sections)
    generated_root = work / "generated"
    units = plan_units(
        module,
        intervals,
        patches,
        generated_root,
        ROOT / "reasm" / version,
    )

    module_payload = reassembly.checked_slice(
        rom_data, module.rom_offset, module.size, module.name
    )
    object_root = work / "obj"
    object_root.mkdir(parents=True, exist_ok=True)
    generated_root.mkdir(parents=True, exist_ok=True)

    llvm_mc = reassembly.resolve_tool(None, ("llvm-mc", "llvm-mc.exe"))
    llvm_objcopy = reassembly.resolve_tool(
        None, ("llvm-objcopy", "llvm-objcopy.exe")
    )
    lld = reassembly.resolve_tool(None, ("ld.lld", "ld.lld.exe"))

    patch_by_section = {patch["section"]: patch for patch in patches}
    external_definitions: dict[str, int] = {}
    objects = []
    unit_report = []
    for index, unit in enumerate(units):
        if not unit.maintained:
            start = unit.start - module.load_address
            payload = module_payload[start : start + unit.size]
            emit_raw_unit(unit, payload, module.cpu)
        if not unit.source.is_file():
            raise reassembly.ReassemblyError(f"missing source unit: {unit.source}")
        obj = object_root / f"unit_{index:03d}.o"
        reassembly.run(
            [
                llvm_mc,
                f"-triple={reassembly.target_triple(module.cpu)}",
                "-filetype=obj",
                str(unit.source),
                "-o",
                str(obj),
            ]
        )
        objects.append(obj)
        if unit.maintained:
            patch = patch_by_section[unit.input_section]
            for name, raw_value in patch.get("externals", {}).items():
                value = int(str(raw_value), 0)
                if known_symbols.get(name) != value:
                    known = known_symbols.get(name)
                    known_text = "missing" if known is None else f"0x{known:08X}"
                    raise reassembly.ReassemblyError(
                        f"external {name} is {known_text} in dsd symbols, not 0x{value:08X}"
                    )
                if not any(
                    unit.start <= relocation["source"] < unit.end
                    and relocation["target"] + relocation["add"] == value
                    for relocation in relocations
                ):
                    raise reassembly.ReassemblyError(
                        f"no dsd relocation in {unit.name} supports external {name}"
                    )
                previous = external_definitions.setdefault(name, value)
                if previous != value:
                    raise reassembly.ReassemblyError(
                        f"conflicting values for external symbol {name}"
                    )
        covered_relocations = [
            relocation
            for relocation in relocations
            if unit.start <= relocation["source"] < unit.end
        ]
        unit_report.append(
            {
                "name": unit.name,
                "kind": unit.kind,
                "start": f"0x{unit.start:08X}",
                "end": f"0x{unit.end:08X}",
                "size": unit.size,
                "input_section": unit.input_section,
                "maintained": unit.maintained,
                "source": str(unit.source.relative_to(ROOT)).replace("\\", "/"),
                "dsd_relocations": len(covered_relocations),
            }
        )

    linker_script = work / "module.ld"
    linked_elf = work / f"{module_name}.elf"
    write_linker_script(linker_script, units, external_definitions)
    reassembly.run(
        [lld, "-m", "armelf", "-T", str(linker_script), *map(str, objects), "-o", str(linked_elf)]
    )
    output_binary.parent.mkdir(parents=True, exist_ok=True)
    reassembly.run([llvm_objcopy, "-O", "binary", str(linked_elf), str(output_binary)])
    rebuilt_payload = output_binary.read_bytes()
    if len(rebuilt_payload) != module.size:
        raise reassembly.ReassemblyError(
            f"linked module size is 0x{len(rebuilt_payload):X}, expected 0x{module.size:X}"
        )
    differing_bytes = sum(a != b for a, b in zip(module_payload, rebuilt_payload))
    if require_matching and differing_bytes:
        raise reassembly.ReassemblyError(
            f"linked module differs in {differing_bytes} bytes"
        )

    report = {
        "format": 1,
        "rom_sha1": identity["sha1"],
        "module": module_name,
        "overlay_id": module.overlay_id,
        "load_address": f"0x{module.load_address:08X}",
        "rom_offset": f"0x{module.rom_offset:08X}",
        "size": module.size,
        "sections": [dataclasses.asdict(section) for section in sections],
        "units": unit_report,
        "dsd_relocations": len(relocations),
        "maintained_units": sum(unit.maintained for unit in units),
        "differing_bytes": differing_bytes,
        "sha256": hashlib.sha256(rebuilt_payload).hexdigest(),
    }
    report_path = work / "report.json"
    report_path.write_text(
        json.dumps(report, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
        newline="\n",
    )

    result = RelinkResult(
        module=module,
        payload=rebuilt_payload,
        unit_count=len(units),
        maintained_unit_count=sum(unit.maintained for unit in units),
        relocation_count=len(relocations),
        differing_bytes=differing_bytes,
        report=report_path,
    )
    if announce:
        print(f"Module: {module_name}")
        print(f"Units: {result.unit_count} ({result.maintained_unit_count} maintained)")
        print(f"DSD relocations inventoried: {result.relocation_count}")
        print(f"Differing bytes: {result.differing_bytes}")
        print(f"Binary: {output_binary}")
        print(f"Report: {result.report}")
    return result


def relink(
    rom: Path,
    version: str,
    overlay_id: int,
    work: Path,
    output_binary: Path,
    output_rom: Path | None,
    require_matching: bool,
    announce: bool = True,
) -> RelinkResult:
    rom_data = rom.read_bytes()
    identity = reassembly.verify_version(rom_data, version)
    module_name = f"arm9_ov{overlay_id:03d}"
    try:
        module = next(
            module
            for module in reassembly.parse_modules(rom_data)
            if module.name == module_name
        )
    except StopIteration as error:
        raise reassembly.ReassemblyError(f"overlay {overlay_id} is not present") from error
    if module.compressed_size_field:
        raise reassembly.ReassemblyError(
            f"{module_name} is compressed; decompression/recompression is not implemented"
        )

    result = relink_module(
        rom_data,
        identity,
        version,
        module,
        overlay_config(version, overlay_id),
        work,
        output_binary,
        require_matching,
        announce,
    )
    if output_rom is not None:
        rebuilt_rom = bytearray(rom_data)
        rebuilt_rom[module.rom_offset : module.rom_offset + module.size] = result.payload
        output_rom.parent.mkdir(parents=True, exist_ok=True)
        output_rom.write_bytes(rebuilt_rom)
        if announce:
            print(f"ROM: {output_rom}")
            print(f"ROM SHA-1: {hashlib.sha1(rebuilt_rom).hexdigest()}")
    return result


def relink_all(
    rom: Path,
    version: str,
    work_root: Path,
    output_directory: Path,
    output_rom: Path | None,
    require_matching: bool,
) -> list[RelinkResult]:
    rom_data = rom.read_bytes()
    identity = reassembly.verify_version(rom_data, version)
    overlays = [
        module
        for module in reassembly.parse_modules(rom_data)
        if module.cpu == "arm9" and module.overlay_id is not None
    ]
    output_directory.mkdir(parents=True, exist_ok=True)
    rebuilt_rom = bytearray(rom_data)
    results = []
    for module in overlays:
        assert module.overlay_id is not None
        result = relink(
            rom,
            version,
            module.overlay_id,
            work_root / module.name,
            output_directory / f"{module.name}.bin",
            None,
            require_matching,
            announce=False,
        )
        rebuilt_rom[
            module.rom_offset : module.rom_offset + module.size
        ] = result.payload
        results.append(result)
        print(
            f"[{module.overlay_id:02d}] {module.name}: "
            f"{result.unit_count} units, {result.differing_bytes} differing bytes"
        )

    if output_rom is not None:
        output_rom.parent.mkdir(parents=True, exist_ok=True)
        output_rom.write_bytes(rebuilt_rom)
        print(f"ROM: {output_rom}")
        print(f"ROM SHA-1: {hashlib.sha1(rebuilt_rom).hexdigest()}")
    print(f"Relinked overlays: {len(results)}")
    print(f"Section units: {sum(result.unit_count for result in results)}")
    print(
        "Maintained units: "
        f"{sum(result.maintained_unit_count for result in results)}"
    )
    print(
        "DSD relocations inventoried: "
        f"{sum(result.relocation_count for result in results)}"
    )
    print(
        "Differing overlay bytes: "
        f"{sum(result.differing_bytes for result in results)}"
    )
    print(f"Input ROM SHA-1: {identity['sha1']}")
    return results


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, required=True)
    parser.add_argument("--version", choices=sorted(reassembly.VERSIONS), default="eur")
    selection = parser.add_mutually_exclusive_group(required=True)
    selection.add_argument("--overlay-id", type=int)
    selection.add_argument("--all-overlays", action="store_true")
    parser.add_argument("--work-dir", type=Path)
    parser.add_argument("--output-bin", type=Path)
    parser.add_argument("--output-dir", type=Path)
    parser.add_argument("--output-rom", type=Path)
    parser.add_argument("--require-matching", action="store_true")
    args = parser.parse_args()
    try:
        default_root = ROOT / "build" / "reassembly" / args.version / "relinked"
        if args.all_overlays:
            if args.output_bin is not None:
                parser.error("--output-bin is only valid with --overlay-id")
            relink_all(
                args.rom,
                args.version,
                args.work_dir or default_root,
                args.output_dir or default_root / "bin",
                args.output_rom,
                args.require_matching,
            )
        else:
            if args.output_dir is not None:
                parser.error("--output-dir is only valid with --all-overlays")
            if args.output_bin is None:
                parser.error("--output-bin is required with --overlay-id")
            assert args.overlay_id is not None
            work = args.work_dir or default_root / f"arm9_ov{args.overlay_id:03d}"
            relink(
                args.rom,
                args.version,
                args.overlay_id,
                work,
                args.output_bin,
                args.output_rom,
                args.require_matching,
            )
    except (OSError, reassembly.ReassemblyError, json.JSONDecodeError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
