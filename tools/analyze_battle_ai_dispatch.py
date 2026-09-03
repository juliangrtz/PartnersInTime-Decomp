#!/usr/bin/env python3
"""Catalog PiT's large battle-AI opcode dispatcher from a user-supplied ROM.

The report contains addresses, aliases, direct call edges, and observed command
record offsets. It never copies ROM bytes and is suitable for checking in as
reverse-engineering metadata.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from pathlib import Path

import reassembly


ROOT = Path(__file__).resolve().parents[1]
DISPATCH_ADDRESS = 0x02079950
DISPATCH_SIZE = 0x4AE0
OPCODE_MIN = 0x33
OPCODE_COUNT = 0xB6
TABLE_ADDRESS = 0x02079978

SYMBOL_PATTERN = re.compile(
    r"^([A-Za-z_.$][A-Za-z0-9_.$]*)\s+.*\baddr:0x([0-9A-Fa-f]+)\b"
)
RELOCATION_PATTERN = re.compile(
    r"^from:0x(?P<source>[0-9A-Fa-f]+)\s+"
    r"kind:(?P<kind>[A-Za-z0-9_]+)\s+"
    r"to:0x(?P<target>[0-9A-Fa-f]+)\s+"
)


def decode_arm_branch(address: int, word: int) -> int:
    """Return the target of an ARM B/BL instruction."""
    if word & 0x0E000000 != 0x0A000000:
        raise ValueError(f"0x{word:08X} at 0x{address:08X} is not ARM B/BL")
    displacement = word & 0x00FFFFFF
    if displacement & 0x00800000:
        displacement -= 0x01000000
    return address + 8 + displacement * 4


def extract_dispatch_table(payload: bytes, load_address: int) -> list[int]:
    offset = TABLE_ADDRESS - load_address
    end = offset + OPCODE_COUNT * 4
    if offset < 0 or end > len(payload):
        raise ValueError("battle-AI dispatch table is outside overlay 2")
    return [
        decode_arm_branch(TABLE_ADDRESS + index * 4, word)
        for index, word in enumerate(
            struct.unpack_from(f"<{OPCODE_COUNT}I", payload, offset)
        )
    ]


def read_symbols(path: Path) -> dict[int, list[str]]:
    result: dict[int, list[str]] = {}
    paths = path.rglob("symbols.txt") if path.is_dir() else (path,)
    for symbols_path in paths:
        for line in symbols_path.read_text(encoding="utf-8").splitlines():
            match = SYMBOL_PATTERN.match(line)
            if match:
                result.setdefault(int(match.group(2), 16), []).append(match.group(1))
    return result


def preferred_symbol(names: list[str] | None) -> str | None:
    if not names:
        return None
    return min(
        names,
        key=lambda name: (
            name.startswith("data_"),
            name.startswith("func_"),
            len(name),
            name,
        ),
    )


def read_calls(path: Path) -> list[tuple[int, int]]:
    result = []
    for line in path.read_text(encoding="utf-8").splitlines():
        match = RELOCATION_PATTERN.match(line)
        if match and match.group("kind") == "arm_call":
            result.append(
                (int(match.group("source"), 16), int(match.group("target"), 16))
            )
    return result


def observed_record_offsets(payload: bytes, load_address: int, start: int, end: int) -> list[int]:
    """Find immediate ARM word loads whose base register is command record r5."""
    result = set()
    for address in range(start, end, 4):
        offset = address - load_address
        if offset < 0 or offset + 4 > len(payload):
            continue
        word = struct.unpack_from("<I", payload, offset)[0]
        # ARM single-data-transfer, immediate offset, load, base register r5.
        if (
            word & 0x0E100000 == 0x04100000
            and word & 0x00400000 == 0
            and (word >> 16) & 0xF == 5
        ):
            immediate = word & 0xFFF
            if word & 0x00800000 == 0:
                immediate = -immediate
            if 0 <= immediate <= 0x100:
                result.add(immediate)
    return sorted(result)


def analyze(payload: bytes, load_address: int, symbols_path: Path, relocs_path: Path) -> dict:
    targets = extract_dispatch_table(payload, load_address)
    symbols = read_symbols(symbols_path)
    calls = read_calls(relocs_path)
    aliases: dict[int, list[int]] = {}
    for index, target in enumerate(targets):
        aliases.setdefault(target, []).append(OPCODE_MIN + index)

    boundaries = sorted(set(targets) | {DISPATCH_ADDRESS + DISPATCH_SIZE})
    end_by_target = {
        start: boundaries[index + 1]
        for index, start in enumerate(boundaries[:-1])
    }
    opcodes = []
    for index, target in enumerate(targets):
        opcode = OPCODE_MIN + index
        end = end_by_target[target]
        direct_calls = []
        for site, call_target in calls:
            if target <= site < end:
                direct_calls.append(
                    {
                        "site": f"0x{site:08X}",
                        "target": f"0x{call_target:08X}",
                        "symbol": preferred_symbol(symbols.get(call_target)),
                    }
                )
        opcodes.append(
            {
                "opcode": opcode,
                "opcode_hex": f"0x{opcode:02X}",
                "target": f"0x{target:08X}",
                "block_end": f"0x{end:08X}",
                "aliases": [
                    f"0x{other:02X}" for other in aliases[target] if other != opcode
                ],
                "record_offsets": [
                    f"0x{offset:X}"
                    for offset in observed_record_offsets(
                        payload, load_address, target, end
                    )
                ],
                "direct_calls": direct_calls,
            }
        )
    return {
        "dispatcher": {
            "address": f"0x{DISPATCH_ADDRESS:08X}",
            "size": f"0x{DISPATCH_SIZE:X}",
            "opcode_min": f"0x{OPCODE_MIN:02X}",
            "opcode_max": f"0x{OPCODE_MIN + OPCODE_COUNT - 1:02X}",
            "opcode_count": OPCODE_COUNT,
            "table_address": f"0x{TABLE_ADDRESS:08X}",
            "default_target": "0x0207E3E4",
        },
        "opcodes": opcodes,
    }


def call_summary(entry: dict) -> str:
    seen = []
    for call in entry["direct_calls"]:
        label = call["symbol"] or call["target"]
        if label not in seen:
            seen.append(label)
    return ", ".join(f"`{label}`" for label in seen) or "—"


def render_markdown(report: dict, rom_sha1: str) -> str:
    dispatcher = report["dispatcher"]
    lines = [
        "# Battle-AI opcode catalog (European ARMP build)",
        "",
        "This generated navigation index contains no ROM bytes. It was derived from",
        f"the verified European ROM with SHA-1 `{rom_sha1}` by",
        "`tools/analyze_battle_ai_dispatch.py`.",
        "",
        f"`BattleAI_DispatchOpcode` spans `{dispatcher['size']}` bytes at",
        f"`{dispatcher['address']}`. Its ARM jump table at",
        f"`{dispatcher['table_address']}` covers {dispatcher['opcode_count']} opcodes",
        f"from `{dispatcher['opcode_min']}` through `{dispatcher['opcode_max']}`.",
        "Block ends are the next unique case entry and are navigation boundaries,",
        "not independently proven function boundaries. Record offsets are direct",
        "word loads observed from the command-record register (`r5`).",
        "",
        "| Opcode | Handler | Aliases | Record offsets | Direct calls |",
        "|---:|---:|---|---|---|",
    ]
    for entry in report["opcodes"]:
        aliases = ", ".join(f"`{value}`" for value in entry["aliases"]) or "—"
        offsets = ", ".join(f"`{value}`" for value in entry["record_offsets"]) or "—"
        lines.append(
            f"| `{entry['opcode_hex']}` | `{entry['target']}` | {aliases} | "
            f"{offsets} | {call_summary(entry)} |"
        )
    lines.append("")
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", required=True, type=Path)
    parser.add_argument("--version", choices=sorted(reassembly.VERSIONS), default="eur")
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--format", choices=("json", "markdown"), default="json")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    identity = reassembly.verify_version(rom, args.version)
    module = next(
        module
        for module in reassembly.parse_modules(rom)
        if module.cpu == "arm9" and module.overlay_id == 2
    )
    payload = rom[module.rom_offset : module.rom_offset + module.size]
    config_root = ROOT / "config" / args.version / "arm9"
    overlay_config = config_root / "overlays" / "ov002"
    report = analyze(
        payload,
        module.load_address,
        config_root,
        overlay_config / "relocs.txt",
    )
    report["version"] = args.version
    report["rom_sha1"] = hashlib.sha1(rom).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if args.format == "markdown":
        content = render_markdown(report, str(identity["sha1"]))
    else:
        content = json.dumps(report, indent=2) + "\n"
    args.output.write_text(content, encoding="utf-8")
    print(f"Opcodes: {OPCODE_COUNT}")
    print(f"Output: {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
