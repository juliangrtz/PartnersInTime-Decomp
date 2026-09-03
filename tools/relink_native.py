#!/usr/bin/env python3
"""Relink every PiT ARM9 and ARM7 runtime component into one NDS image."""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path

import reassembly
import relink_arm7
import relink_arm9
import relink_overlay


ROOT = Path(__file__).resolve().parents[1]


def relink_all_native(
    rom: Path,
    version: str,
    work_root: Path,
    output_rom: Path,
    require_matching: bool,
) -> list[relink_overlay.RelinkResult]:
    rom_data = rom.read_bytes()
    identity = reassembly.verify_version(rom_data, version)
    arm9_layout = relink_arm9.discover_arm9_layout(rom_data, version)
    overlays = tuple(
        module
        for module in reassembly.parse_modules(rom_data)
        if module.cpu == "arm9" and module.overlay_id is not None
    )
    arm7_layout = relink_arm7.discover_arm7_layout(rom_data)
    validated_arm7_relocations = relink_arm7.validate_known_relocations(
        rom_data, version, arm7_layout
    )
    modules = (*arm9_layout.components, *overlays, *arm7_layout.components)
    rebuilt_rom = bytearray(rom_data)
    results = []

    for module in modules:
        if module.cpu == "arm7":
            config = relink_arm7.component_config(version, module)
        else:
            config = relink_arm9.component_config(version, module)
        component_root = work_root / module.cpu / module.name
        result = relink_overlay.relink_module(
            rom_data,
            identity,
            version,
            module,
            config,
            component_root,
            component_root / f"{module.name}.bin",
            require_matching,
            announce=False,
        )
        rebuilt_rom[
            module.rom_offset : module.rom_offset + module.size
        ] = result.payload
        results.append(result)
        print(
            f"[{len(results):02d}/{len(modules):02d}] "
            f"{module.name}: {result.unit_count} units, "
            f"{result.differing_bytes} differing bytes"
        )

    output_rom.parent.mkdir(parents=True, exist_ok=True)
    output_rom.write_bytes(rebuilt_rom)
    summary = {
        "format": 1,
        "input_rom_sha1": identity["sha1"],
        "output_rom_sha1": hashlib.sha1(rebuilt_rom).hexdigest(),
        "components": len(results),
        "arm9_components": sum(result.module.cpu == "arm9" for result in results),
        "arm7_components": sum(result.module.cpu == "arm7" for result in results),
        "section_units": sum(result.unit_count for result in results),
        "maintained_units": sum(
            result.maintained_unit_count for result in results
        ),
        "known_relocations": sum(result.relocation_count for result in results),
        "validated_arm7_relocations": validated_arm7_relocations,
        "differing_bytes": sum(result.differing_bytes for result in results),
        "descriptor_tables_preserved": {
            "arm9": f"0x{arm9_layout.descriptor_offset:08X}",
            "arm7": f"0x{arm7_layout.descriptor_offset:08X}",
        },
    }
    summary_path = work_root / "native_summary.json"
    summary_path.parent.mkdir(parents=True, exist_ok=True)
    summary_path.write_text(
        json.dumps(summary, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
        newline="\n",
    )
    print(f"ROM: {output_rom}")
    print(f"ROM SHA-1: {summary['output_rom_sha1']}")
    print(
        f"Components: {summary['components']} "
        f"({summary['arm9_components']} ARM9, {summary['arm7_components']} ARM7)"
    )
    print(f"Section units: {summary['section_units']}")
    print(f"Known relocations: {summary['known_relocations']}")
    print(f"Validated ARM7 relocations: {validated_arm7_relocations}")
    print(f"Differing bytes: {summary['differing_bytes']}")
    print(f"Summary: {summary_path}")
    return results


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, required=True)
    parser.add_argument("--version", choices=sorted(reassembly.VERSIONS), default="eur")
    parser.add_argument("--work-dir", type=Path)
    parser.add_argument("--output-rom", type=Path, required=True)
    parser.add_argument("--require-matching", action="store_true")
    args = parser.parse_args()
    work = args.work_dir or (
        ROOT / "build" / "reassembly" / args.version / "native"
    )
    try:
        relink_all_native(
            args.rom,
            args.version,
            work,
            args.output_rom,
            args.require_matching,
        )
    except (OSError, reassembly.ReassemblyError, json.JSONDecodeError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
