#!/usr/bin/env python3
"""Relink the resident PiT ARM7 image and its discovered autoload images.

The upstream decompilation has no ARM7 DSD project. This tool therefore finds
the NitroSDK static-module parameters in the user-supplied ARM7 payload,
validates its autoload table, and maps each serialized image to its runtime
address before invoking the common fixed-address ELF linker.
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
MODULE_PARAMETER_SIZE = 24
AUTOLOAD_DESCRIPTOR_SIZE = 12
MAX_MODULE_PARAMETER_SEARCH = 0x1000


@dataclasses.dataclass(frozen=True)
class Arm7Layout:
    container: reassembly.Module
    module_parameters_address: int
    resident: reassembly.Module
    autoloads: tuple[reassembly.Module, ...]
    descriptor_offset: int
    descriptor_size: int

    @property
    def components(self) -> tuple[reassembly.Module, ...]:
        return (self.resident, *self.autoloads)


def parse_layout_candidate(
    rom_data: bytes,
    container: reassembly.Module,
    parameter_offset: int,
) -> Arm7Layout | None:
    payload = reassembly.checked_slice(
        rom_data, container.rom_offset, container.size, "ARM7 payload"
    )
    (
        list_start,
        list_end,
        data_start,
        static_bss_start,
        static_bss_end,
        compressed_static_end,
    ) = struct.unpack_from("<6I", payload, parameter_offset)
    image_start = container.load_address
    image_end = image_start + container.size
    if not (
        data_start >= image_start
        and list_start >= data_start
        and list_end == image_end
        and list_end > list_start
        and (list_end - list_start) % AUTOLOAD_DESCRIPTOR_SIZE == 0
        and static_bss_end >= static_bss_start
        and compressed_static_end in (0, list_start)
    ):
        return None
    count = (list_end - list_start) // AUTOLOAD_DESCRIPTOR_SIZE
    if count < 1 or count > 8:
        return None

    descriptor_offset = container.rom_offset + list_start - image_start
    descriptors = reassembly.checked_slice(
        rom_data,
        descriptor_offset,
        count * AUTOLOAD_DESCRIPTOR_SIZE,
        "ARM7 autoload descriptors",
    )
    cursor = container.rom_offset + data_start - image_start
    autoloads = []
    for index in range(count):
        destination, code_size, bss_size = struct.unpack_from(
            "<3I", descriptors, index * AUTOLOAD_DESCRIPTOR_SIZE
        )
        if not code_size or destination & 3 or code_size & 3 or bss_size & 3:
            return None
        autoloads.append(
            reassembly.Module(
                name=f"arm7_autoload_{index:03d}",
                cpu="arm7",
                rom_offset=cursor,
                size=code_size,
                load_address=destination,
                bss_size=bss_size,
            )
        )
        cursor += code_size
    if cursor != descriptor_offset:
        return None

    resident_size = data_start - image_start
    if not resident_size or parameter_offset + MODULE_PARAMETER_SIZE > resident_size:
        return None
    resident = dataclasses.replace(
        container,
        size=resident_size,
        bss_size=static_bss_end - static_bss_start,
    )
    return Arm7Layout(
        container=container,
        module_parameters_address=image_start + parameter_offset,
        resident=resident,
        autoloads=tuple(autoloads),
        descriptor_offset=descriptor_offset,
        descriptor_size=count * AUTOLOAD_DESCRIPTOR_SIZE,
    )


def discover_arm7_layout(rom_data: bytes) -> Arm7Layout:
    container = next(
        module for module in reassembly.parse_modules(rom_data) if module.name == "arm7"
    )
    candidates = []
    search_end = min(container.size - MODULE_PARAMETER_SIZE, MAX_MODULE_PARAMETER_SEARCH)
    for offset in range(0, search_end + 1, 4):
        candidate = parse_layout_candidate(rom_data, container, offset)
        if candidate is not None:
            candidates.append(candidate)
    if len(candidates) != 1:
        addresses = ", ".join(
            f"0x{candidate.module_parameters_address:08X}" for candidate in candidates
        )
        raise reassembly.ReassemblyError(
            f"expected one ARM7 module-parameter block, found {len(candidates)}"
            + (f": {addresses}" if addresses else "")
        )
    return candidates[0]


def component_config(version: str, module: reassembly.Module) -> Path:
    root = ROOT / "config" / version / "arm7"
    if module.name == "arm7":
        return root / "resident"
    prefix = "arm7_autoload_"
    if module.name.startswith(prefix):
        index = int(module.name.removeprefix(prefix))
        return root / "autoloads" / f"a{index:03d}"
    raise reassembly.ReassemblyError(f"no ARM7 configuration for {module.name}")


def sign_extend(value: int, bits: int) -> int:
    sign = 1 << (bits - 1)
    return (value & (sign - 1)) - (value & sign)


def resolve_arm_relocation(word: int, source: int, kind: str) -> int:
    if kind == "arm_call":
        if word & 0x0F000000 != 0x0B000000:
            raise reassembly.ReassemblyError(
                f"0x{source:08X} is not an ARM BL instruction"
            )
        return source + 8 + sign_extend(word & 0x00FFFFFF, 24) * 4
    if kind == "load":
        if word & 0x0F7F0000 != 0x051F0000:
            raise reassembly.ReassemblyError(
                f"0x{source:08X} is not an ARM literal load"
            )
        displacement = word & 0xFFF
        return source + 8 + (displacement if word & 0x00800000 else -displacement)
    raise reassembly.ReassemblyError(f"unsupported ARM7 relocation kind: {kind}")


def validate_known_relocations(
    rom_data: bytes, version: str, layout: Arm7Layout
) -> int:
    count = 0
    for module in layout.components:
        relocations = relink_overlay.read_relocations(
            component_config(version, module) / "relocs.txt"
        )
        payload = reassembly.checked_slice(
            rom_data, module.rom_offset, module.size, module.name
        )
        for relocation in relocations:
            source = relocation["source"]
            source_offset = source - module.load_address
            if source_offset < 0 or source_offset + 4 > module.size:
                raise reassembly.ReassemblyError(
                    f"ARM7 relocation source 0x{source:08X} is outside {module.name}"
                )
            word = struct.unpack_from("<I", payload, source_offset)[0]
            resolved = resolve_arm_relocation(word, source, relocation["kind"])
            if relocation["kind"] == "load":
                literal_offset = resolved - module.load_address
                if literal_offset < 0 or literal_offset + 4 > module.size:
                    raise reassembly.ReassemblyError(
                        f"ARM7 literal for 0x{source:08X} is outside {module.name}"
                    )
                resolved = struct.unpack_from("<I", payload, literal_offset)[0]
            expected = relocation["target"] + relocation["add"]
            if resolved != expected:
                raise reassembly.ReassemblyError(
                    f"ARM7 relocation at 0x{source:08X} resolves to "
                    f"0x{resolved:08X}, expected 0x{expected:08X}"
                )
            count += 1
    return count


def relink_all_arm7(
    rom: Path,
    version: str,
    work_root: Path,
    output_directory: Path,
    output_rom: Path,
    require_matching: bool,
) -> list[relink_overlay.RelinkResult]:
    rom_data = rom.read_bytes()
    identity = reassembly.verify_version(rom_data, version)
    layout = discover_arm7_layout(rom_data)
    validated_relocations = validate_known_relocations(rom_data, version, layout)
    rebuilt_rom = bytearray(rom_data)
    output_directory.mkdir(parents=True, exist_ok=True)
    results = []
    for module in layout.components:
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
            f"[{len(results):02d}/{len(layout.components):02d}] {module.name}: "
            f"{result.unit_count} units, {result.differing_bytes} differing bytes"
        )

    output_rom.parent.mkdir(parents=True, exist_ok=True)
    output_rom.write_bytes(rebuilt_rom)
    summary = {
        "format": 1,
        "input_rom_sha1": identity["sha1"],
        "output_rom_sha1": hashlib.sha1(rebuilt_rom).hexdigest(),
        "arm7_components": len(results),
        "section_units": sum(result.unit_count for result in results),
        "known_relocations": sum(result.relocation_count for result in results),
        "validated_relocations": validated_relocations,
        "differing_bytes": sum(result.differing_bytes for result in results),
        "module_parameters_address": f"0x{layout.module_parameters_address:08X}",
        "autoload_descriptors": {
            "rom_offset": f"0x{layout.descriptor_offset:08X}",
            "size": layout.descriptor_size,
            "preserved": True,
        },
        "components": [
            {
                "name": module.name,
                "rom_offset": f"0x{module.rom_offset:08X}",
                "load_address": f"0x{module.load_address:08X}",
                "stored_size": module.size,
                "bss_size": module.bss_size or 0,
            }
            for module in layout.components
        ],
    }
    summary_path = work_root / "arm7_summary.json"
    summary_path.parent.mkdir(parents=True, exist_ok=True)
    summary_path.write_text(
        json.dumps(summary, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
        newline="\n",
    )
    print(f"ROM: {output_rom}")
    print(f"ROM SHA-1: {summary['output_rom_sha1']}")
    print(f"ARM7 components: {summary['arm7_components']}")
    print(f"Section units: {summary['section_units']}")
    print(f"Known relocations: {summary['known_relocations']}")
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
    root = ROOT / "build" / "reassembly" / args.version / "arm7"
    try:
        relink_all_arm7(
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
