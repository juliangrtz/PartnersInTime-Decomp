#!/usr/bin/env python3
"""Relink every PiT ARM9 runtime component into one user-supplied ROM.

The NDS ARM9 payload contains the resident image, aligned ITCM and DTCM
autoload images, and a descriptor table. Overlays live elsewhere in the ROM.
This tool discovers and verifies that layout from the checked-in DSD maps,
relinks all 40 ARM9 components, and preserves the autoload descriptor bytes.
"""

from __future__ import annotations

import argparse
import dataclasses
import hashlib
import json
import struct
import sys
from pathlib import Path

import reassembly
import relink_overlay


ROOT = Path(__file__).resolve().parents[1]
AUTOLOAD_NAMES = ("itcm", "dtcm")
AUTOLOAD_DESCRIPTOR_SIZE = 12
AUTOLOAD_ALIGNMENT = 32


@dataclasses.dataclass(frozen=True)
class Arm9Layout:
    container: reassembly.Module
    resident: reassembly.Module
    autoloads: tuple[reassembly.Module, ...]
    descriptor_offset: int
    descriptor_size: int

    @property
    def components(self) -> tuple[reassembly.Module, ...]:
        return (self.resident, *self.autoloads)


def align_up(value: int, alignment: int) -> int:
    if value < 0 or alignment <= 0 or alignment & (alignment - 1):
        raise reassembly.ReassemblyError("invalid alignment request")
    return (value + alignment - 1) & -alignment


def stored_bounds(
    sections: list[relink_overlay.DsdSection],
) -> tuple[int, int, int]:
    stored = [section for section in sections if section.kind != "bss"]
    if not stored:
        raise reassembly.ReassemblyError("module has no stored DSD sections")
    start = min(section.start for section in stored)
    end = max(section.end for section in stored)
    bss_size = sum(
        section.end - section.start for section in sections if section.kind == "bss"
    )
    return start, end, bss_size


def validate_autoload_descriptors(
    rom_data: bytes,
    descriptor_offset: int,
    autoloads: tuple[reassembly.Module, ...],
) -> None:
    expected_size = len(autoloads) * AUTOLOAD_DESCRIPTOR_SIZE
    table = reassembly.checked_slice(
        rom_data, descriptor_offset, expected_size, "ARM9 autoload descriptors"
    )
    for index, module in enumerate(autoloads):
        destination, code_size, bss_size = struct.unpack_from(
            "<3I", table, index * AUTOLOAD_DESCRIPTOR_SIZE
        )
        expected_bss = module.bss_size or 0
        if (destination, code_size, bss_size) != (
            module.load_address,
            module.size,
            expected_bss,
        ):
            raise reassembly.ReassemblyError(
                f"autoload descriptor {index} does not match {module.name}: "
                f"got (0x{destination:08X}, 0x{code_size:X}, 0x{bss_size:X})"
            )


def discover_arm9_layout(rom_data: bytes, version: str) -> Arm9Layout:
    container = next(
        module for module in reassembly.parse_modules(rom_data) if module.name == "arm9"
    )
    config_root = ROOT / "config" / version / "arm9"
    resident_sections = relink_overlay.read_sections(config_root / "delinks.txt")
    resident_start, resident_end, resident_bss = stored_bounds(resident_sections)
    if resident_start != container.load_address:
        raise reassembly.ReassemblyError(
            "resident ARM9 DSD base does not match the NDS header"
        )
    resident_size = resident_end - resident_start
    resident = dataclasses.replace(
        container,
        size=resident_size,
        bss_size=resident_bss,
    )

    cursor = container.rom_offset + resident_size
    autoloads = []
    for name in AUTOLOAD_NAMES:
        sections = relink_overlay.read_sections(config_root / name / "delinks.txt")
        load_address, logical_end, bss_size = stored_bounds(sections)
        stored_size = align_up(logical_end - load_address, AUTOLOAD_ALIGNMENT)
        autoloads.append(
            reassembly.Module(
                name=name,
                cpu="arm9",
                rom_offset=cursor,
                size=stored_size,
                load_address=load_address,
                bss_size=bss_size,
            )
        )
        cursor += stored_size

    descriptor_size = len(autoloads) * AUTOLOAD_DESCRIPTOR_SIZE
    container_end = container.rom_offset + container.size
    if cursor + descriptor_size != container_end:
        raise reassembly.ReassemblyError(
            "DSD sections and autoload table do not cover the ARM9 payload: "
            f"0x{cursor + descriptor_size:X} != 0x{container_end:X}"
        )
    result = Arm9Layout(
        container=container,
        resident=resident,
        autoloads=tuple(autoloads),
        descriptor_offset=cursor,
        descriptor_size=descriptor_size,
    )
    validate_autoload_descriptors(rom_data, cursor, result.autoloads)
    return result


def component_config(version: str, module: reassembly.Module) -> Path:
    root = ROOT / "config" / version / "arm9"
    if module.name == "arm9":
        return root
    if module.name in AUTOLOAD_NAMES:
        return root / module.name
    if module.overlay_id is not None:
        return root / "overlays" / f"ov{module.overlay_id:03d}"
    raise reassembly.ReassemblyError(f"no DSD configuration for {module.name}")


def relink_all_arm9(
    rom: Path,
    version: str,
    work_root: Path,
    output_directory: Path,
    output_rom: Path,
    require_matching: bool,
) -> list[relink_overlay.RelinkResult]:
    rom_data = rom.read_bytes()
    identity = reassembly.verify_version(rom_data, version)
    layout = discover_arm9_layout(rom_data, version)
    overlays = tuple(
        module
        for module in reassembly.parse_modules(rom_data)
        if module.cpu == "arm9" and module.overlay_id is not None
    )
    modules = (*layout.components, *overlays)
    rebuilt_rom = bytearray(rom_data)
    output_directory.mkdir(parents=True, exist_ok=True)
    results = []
    for module in modules:
        result = relink_overlay.relink_module(
            rom_data,
            identity,
            version,
            module,
            component_config(version, module),
            work_root / module.name,
            output_directory / f"{module.name}.bin",
            require_matching,
            announce=False,
        )
        rebuilt_rom[
            module.rom_offset : module.rom_offset + module.size
        ] = result.payload
        results.append(result)
        print(
            f"[{len(results):02d}/{len(modules):02d}] {module.name}: "
            f"{result.unit_count} units, {result.differing_bytes} differing bytes"
        )

    output_rom.parent.mkdir(parents=True, exist_ok=True)
    output_rom.write_bytes(rebuilt_rom)
    summary = {
        "format": 1,
        "input_rom_sha1": identity["sha1"],
        "output_rom_sha1": hashlib.sha1(rebuilt_rom).hexdigest(),
        "arm9_components": len(results),
        "section_units": sum(result.unit_count for result in results),
        "maintained_units": sum(
            result.maintained_unit_count for result in results
        ),
        "dsd_relocations": sum(result.relocation_count for result in results),
        "differing_bytes": sum(result.differing_bytes for result in results),
        "autoload_descriptors": {
            "rom_offset": f"0x{layout.descriptor_offset:08X}",
            "size": layout.descriptor_size,
            "preserved": True,
        },
    }
    summary_path = work_root / "arm9_summary.json"
    summary_path.parent.mkdir(parents=True, exist_ok=True)
    summary_path.write_text(
        json.dumps(summary, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
        newline="\n",
    )
    print(f"ROM: {output_rom}")
    print(f"ROM SHA-1: {summary['output_rom_sha1']}")
    print(f"ARM9 components: {summary['arm9_components']}")
    print(f"Section units: {summary['section_units']}")
    print(f"DSD relocations inventoried: {summary['dsd_relocations']}")
    print(f"Differing bytes: {summary['differing_bytes']}")
    print(f"Summary: {summary_path}")
    return results


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, required=True)
    parser.add_argument("--version", choices=sorted(reassembly.VERSIONS), default="eur")
    parser.add_argument("--work-dir", type=Path)
    parser.add_argument("--output-dir", type=Path)
    parser.add_argument("--output-rom", type=Path, required=True)
    parser.add_argument("--require-matching", action="store_true")
    args = parser.parse_args()
    root = ROOT / "build" / "reassembly" / args.version / "arm9"
    try:
        relink_all_arm9(
            args.rom,
            args.version,
            args.work_dir or root,
            args.output_dir or root / "bin",
            args.output_rom,
            args.require_matching,
        )
    except (OSError, reassembly.ReassemblyError, json.JSONDecodeError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
