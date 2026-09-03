#!/usr/bin/env python3
"""Bootstrap a complete, fixed-layout Nintendo DS code reassembly.

The checked-in tool contains no game bytes.  It reads a user-supplied ROM,
materializes every native module as local assembly source, assembles the source
with LLVM, and writes the rebuilt modules into a copy of the ROM.

The initial source deliberately uses .word/.byte directives.  That makes the
first build lossless despite unknown code/data boundaries.  Later work can
replace those directives with symbolic ARM/Thumb assembly or matching C/C++.
"""

from __future__ import annotations

import argparse
import dataclasses
import hashlib
import json
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[1]

VERSIONS = {
    "eur": {
        "sha1": "ba4ec2f99b4f2e0047601552bccf00aa73e28701",
        "game_code": "ARMP",
    },
    "usa": {
        "sha1": "89c9136db3c3975c451a907e8bd6861ce6b81557",
        "game_code": "ARME",
    },
}

HEADER_MIN_SIZE = 0x160
OVERLAY_ENTRY_SIZE = 0x20
FAT_ENTRY_SIZE = 8
SECURE_AREA_START = 0x4000
SECURE_AREA_END = 0x8000


class ReassemblyError(RuntimeError):
    pass


@dataclasses.dataclass(frozen=True)
class Module:
    name: str
    cpu: str
    rom_offset: int
    size: int
    load_address: int
    overlay_id: int | None = None
    file_id: int | None = None
    ram_size: int | None = None
    bss_size: int | None = None
    static_init_start: int | None = None
    static_init_end: int | None = None
    compressed_size_field: int | None = None

    def as_manifest(self, payload: bytes, source: Path, work: Path) -> dict:
        result = dataclasses.asdict(self)
        result["expected_sha256"] = hashlib.sha256(payload).hexdigest()
        result["source"] = source.relative_to(work).as_posix()
        return result


def read_u32(data: bytes, offset: int) -> int:
    if offset < 0 or offset + 4 > len(data):
        raise ReassemblyError(f"u32 outside ROM at 0x{offset:X}")
    return struct.unpack_from("<I", data, offset)[0]


def checked_slice(data: bytes, offset: int, size: int, label: str) -> bytes:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise ReassemblyError(
            f"{label} range 0x{offset:X}..0x{offset + size:X} is outside ROM"
        )
    return data[offset : offset + size]


def parse_modules(data: bytes) -> list[Module]:
    if len(data) < HEADER_MIN_SIZE:
        raise ReassemblyError("file is too small to be a Nintendo DS ROM")

    fat_offset = read_u32(data, 0x48)
    fat_size = read_u32(data, 0x4C)
    if fat_size % FAT_ENTRY_SIZE:
        raise ReassemblyError(f"FAT size 0x{fat_size:X} is not divisible by 8")
    checked_slice(data, fat_offset, fat_size, "FAT")
    file_count = fat_size // FAT_ENTRY_SIZE

    def fat_range(file_id: int) -> tuple[int, int]:
        if file_id >= file_count:
            raise ReassemblyError(
                f"overlay file id {file_id} exceeds FAT file count {file_count}"
            )
        start, end = struct.unpack_from(
            "<II", data, fat_offset + file_id * FAT_ENTRY_SIZE
        )
        if end < start:
            raise ReassemblyError(f"FAT file {file_id} has a negative size")
        checked_slice(data, start, end - start, f"FAT file {file_id}")
        return start, end

    modules = [
        Module(
            name="arm9",
            cpu="arm9",
            rom_offset=read_u32(data, 0x20),
            size=read_u32(data, 0x2C),
            load_address=read_u32(data, 0x28),
        ),
        Module(
            name="arm7",
            cpu="arm7",
            rom_offset=read_u32(data, 0x30),
            size=read_u32(data, 0x3C),
            load_address=read_u32(data, 0x38),
        ),
    ]

    for module in modules:
        checked_slice(data, module.rom_offset, module.size, module.name)

    for cpu, table_header in (("arm9", 0x50), ("arm7", 0x58)):
        table_offset = read_u32(data, table_header)
        table_size = read_u32(data, table_header + 4)
        if table_size == 0:
            continue
        if table_size % OVERLAY_ENTRY_SIZE:
            raise ReassemblyError(
                f"{cpu} overlay table size 0x{table_size:X} is not divisible by 0x20"
            )
        table = checked_slice(data, table_offset, table_size, f"{cpu} overlay table")
        for index in range(table_size // OVERLAY_ENTRY_SIZE):
            fields = struct.unpack_from("<8I", table, index * OVERLAY_ENTRY_SIZE)
            (
                overlay_id,
                ram_address,
                ram_size,
                bss_size,
                static_init_start,
                static_init_end,
                file_id,
                compressed_size_field,
            ) = fields
            start, end = fat_range(file_id)
            modules.append(
                Module(
                    name=f"{cpu}_ov{overlay_id:03d}",
                    cpu=cpu,
                    rom_offset=start,
                    size=end - start,
                    load_address=ram_address,
                    overlay_id=overlay_id,
                    file_id=file_id,
                    ram_size=ram_size,
                    bss_size=bss_size,
                    static_init_start=static_init_start,
                    static_init_end=static_init_end,
                    compressed_size_field=compressed_size_field,
                )
            )

    return modules


def rom_identity(data: bytes) -> dict[str, str | int]:
    title = bytes(data[0:12]).rstrip(b"\0").decode("ascii", errors="replace")
    game_code = bytes(data[0x0C:0x10]).decode("ascii", errors="replace")
    return {
        "title": title,
        "game_code": game_code,
        "size": len(data),
        "sha1": hashlib.sha1(data).hexdigest(),
        "sha256": hashlib.sha256(data).hexdigest(),
    }


def verify_version(data: bytes, version: str) -> dict[str, str | int]:
    identity = rom_identity(data)
    expected = VERSIONS[version]
    if identity["sha1"] != expected["sha1"]:
        raise ReassemblyError(
            f"{version.upper()} ROM SHA-1 mismatch: got {identity['sha1']}, "
            f"expected {expected['sha1']}"
        )
    if identity["game_code"] != expected["game_code"]:
        raise ReassemblyError(
            f"game code mismatch: got {identity['game_code']!r}, "
            f"expected {expected['game_code']!r}"
        )
    return identity


def default_work(version: str) -> Path:
    return ROOT / "build" / "reassembly" / version


def dsd_symbol_file(version: str, module: Module) -> Path | None:
    root = ROOT / "config" / version / "arm9"
    if module.name == "arm9":
        return root / "symbols.txt"
    if module.cpu == "arm9" and module.overlay_id is not None:
        return root / "overlays" / f"ov{module.overlay_id:03d}" / "symbols.txt"
    return None


def load_dsd_symbols(version: str, module: Module) -> list[tuple[str, int]]:
    path = dsd_symbol_file(version, module)
    if path is None or not path.is_file():
        return []
    symbol_pattern = re.compile(
        r"^([A-Za-z_.$][A-Za-z0-9_.$]*)\s+.*\baddr:0x([0-9A-Fa-f]+)\b"
    )
    result: dict[str, int] = {}
    for line in path.read_text(encoding="utf-8").splitlines():
        match = symbol_pattern.match(line)
        if not match:
            continue
        address = int(match.group(2), 16)
        offset = address - module.load_address
        if 0 <= offset < module.size:
            result.setdefault(match.group(1), offset)
    return sorted(result.items(), key=lambda pair: (pair[1], pair[0]))


def emit_source(
    module: Module,
    payload: bytes,
    source: Path,
    identity: dict,
    symbols: list[tuple[str, int]],
) -> None:
    symbol = f"pit_{module.name}"
    lines = [
        "/* Generated locally from a user-supplied ROM. Do not commit this file. */",
        f"/* ROM SHA-1: {identity['sha1']} */",
        f"/* Module: {module.name}; ROM 0x{module.rom_offset:08X}; "
        f"load 0x{module.load_address:08X}; size 0x{module.size:X}. */",
        "",
        ".syntax unified",
        ".arch armv5te",
        '.section .text, "ax", %progbits',
        ".balign 1",
        f".global {symbol}_start",
        f".global {symbol}_end",
        f".type {symbol}_start, %object",
        f"{symbol}_start:",
    ]

    word_end = len(payload) - (len(payload) % 4)
    for offset in range(0, word_end, 16):
        chunk = payload[offset : min(offset + 16, word_end)]
        words = struct.unpack("<" + "I" * (len(chunk) // 4), chunk)
        values = ", ".join(f"0x{word:08X}" for word in words)
        lines.append(
            f"    /* 0x{module.load_address + offset:08X} */ .word {values}"
        )

    if word_end != len(payload):
        values = ", ".join(f"0x{byte:02X}" for byte in payload[word_end:])
        lines.append(
            f"    /* 0x{module.load_address + word_end:08X} */ .byte {values}"
        )

    lines.extend(
        [
            f"{symbol}_end:",
            f".size {symbol}_start, {symbol}_end - {symbol}_start",
        ]
    )
    if symbols:
        lines.extend(
            [
                "",
                "/* DSD-derived aliases. They emit no bytes and are safe to commit as metadata. */",
            ]
        )
        lines.extend(
            f".set {name}, {symbol}_start + 0x{offset:X}" for name, offset in symbols
        )
    lines.append("")
    source.parent.mkdir(parents=True, exist_ok=True)
    source.write_text("\n".join(lines), encoding="utf-8", newline="\n")


def extract_sources(rom: Path, version: str, work: Path, force: bool) -> Path:
    data = rom.read_bytes()
    identity = verify_version(data, version)
    modules = parse_modules(data)
    source_root = work / "generated"
    manifest_path = work / "manifest.json"

    if manifest_path.exists() and not force:
        raise ReassemblyError(
            f"{manifest_path} already exists; pass --force to regenerate local sources"
        )

    manifest_modules = []
    for module in modules:
        payload = checked_slice(data, module.rom_offset, module.size, module.name)
        source = source_root / f"{module.name}.s"
        symbols = load_dsd_symbols(version, module)
        emit_source(module, payload, source, identity, symbols)
        manifest_modules.append(module.as_manifest(payload, source, work))

    work.mkdir(parents=True, exist_ok=True)
    manifest = {
        "format": 1,
        "version": version,
        "rom": identity,
        "modules": manifest_modules,
    }
    manifest_path.write_text(
        json.dumps(manifest, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
        newline="\n",
    )
    print(
        f"Extracted {len(modules)} native modules into {source_root}\n"
        f"Manifest: {manifest_path}"
    )
    return manifest_path


def resolve_tool(explicit: str | None, names: Iterable[str]) -> str:
    if explicit:
        candidate = Path(explicit)
        if not candidate.is_file():
            raise ReassemblyError(f"tool does not exist: {candidate}")
        return str(candidate)
    for name in names:
        found = shutil.which(name)
        if found:
            return found
    raise ReassemblyError(f"could not find any of: {', '.join(names)}")


def run(command: list[str]) -> None:
    result = subprocess.run(command, text=True, capture_output=True)
    if result.returncode:
        details = "\n".join(part for part in (result.stdout, result.stderr) if part)
        raise ReassemblyError(
            f"command failed ({result.returncode}): {' '.join(command)}\n{details}"
        )


def assemble_source(
    source: Path,
    obj: Path,
    binary: Path,
    llvm_mc: str,
    llvm_objcopy: str,
) -> bytes:
    run(
        [
            llvm_mc,
            "-triple=armv5te-none-eabi",
            "-filetype=obj",
            str(source),
            "-o",
            str(obj),
        ]
    )
    run(
        [
            llvm_objcopy,
            "-O",
            "binary",
            "--only-section=.text",
            str(obj),
            str(binary),
        ]
    )
    return binary.read_bytes()


def module_from_manifest(entry: dict) -> Module:
    fields = {field.name for field in dataclasses.fields(Module)}
    return Module(**{key: value for key, value in entry.items() if key in fields})


def regions_overlap(start_a: int, size_a: int, start_b: int, end_b: int) -> bool:
    return start_a < end_b and start_b < start_a + size_a


def load_maintained_patches(version: str) -> dict[str, list[dict]]:
    patch_file = ROOT / "reasm" / version / "patches.json"
    if not patch_file.is_file():
        return {}
    document = json.loads(patch_file.read_text(encoding="utf-8"))
    if document.get("format") != 1 or not isinstance(document.get("patches"), list):
        raise ReassemblyError(f"invalid maintained-patch file: {patch_file}")
    result: dict[str, list[dict]] = {}
    for index, patch in enumerate(document["patches"]):
        required = {"module", "address", "size", "source", "expected_sha256"}
        missing = required.difference(patch)
        if missing:
            raise ReassemblyError(
                f"patch {index} in {patch_file} lacks: {', '.join(sorted(missing))}"
            )
        result.setdefault(patch["module"], []).append(patch)
    return result


def build_rom(
    rom: Path,
    version: str,
    work: Path,
    output: Path,
    llvm_mc_arg: str | None,
    llvm_objcopy_arg: str | None,
    require_matching: bool,
    allow_secure_area_change: bool,
) -> None:
    base = rom.read_bytes()
    identity = verify_version(base, version)
    manifest_path = work / "manifest.json"
    if not manifest_path.is_file():
        raise ReassemblyError(
            f"missing {manifest_path}; run the extract command first"
        )
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    if manifest.get("format") != 1 or manifest.get("version") != version:
        raise ReassemblyError("manifest format/version mismatch")
    if manifest.get("rom", {}).get("sha1") != identity["sha1"]:
        raise ReassemblyError("manifest was generated from a different ROM")

    llvm_mc = resolve_tool(llvm_mc_arg, ("llvm-mc", "llvm-mc.exe"))
    llvm_objcopy = resolve_tool(
        llvm_objcopy_arg, ("llvm-objcopy", "llvm-objcopy.exe")
    )
    object_root = work / "obj"
    binary_root = work / "bin"
    object_root.mkdir(parents=True, exist_ok=True)
    binary_root.mkdir(parents=True, exist_ok=True)
    maintained_patches = load_maintained_patches(version)

    rebuilt = bytearray(base)
    changed_modules: list[str] = []
    changed_bytes = 0
    written_regions: dict[tuple[int, int], bytes] = {}

    for entry in manifest["modules"]:
        module = module_from_manifest(entry)
        source = work / entry["source"]
        tracked_override = ROOT / "reasm" / version / "modules" / source.name
        if tracked_override.is_file():
            source = tracked_override
        if not source.is_file():
            raise ReassemblyError(f"missing assembly source: {source}")

        obj = object_root / f"{module.name}.o"
        binary = binary_root / f"{module.name}.bin"
        payload = assemble_source(source, obj, binary, llvm_mc, llvm_objcopy)
        if len(payload) != module.size:
            raise ReassemblyError(
                f"{module.name} rebuilt size is 0x{len(payload):X}; "
                f"fixed-layout size must remain 0x{module.size:X}"
            )

        expected = checked_slice(base, module.rom_offset, module.size, module.name)
        if hashlib.sha256(expected).hexdigest() != entry["expected_sha256"]:
            raise ReassemblyError(f"base bytes changed under {module.name}")

        patched_payload = bytearray(payload)
        occupied_patch_ranges: list[tuple[int, int]] = []
        for patch_index, patch in enumerate(maintained_patches.get(module.name, [])):
            address = int(str(patch["address"]), 0)
            patch_size = int(str(patch["size"]), 0)
            patch_offset = address - module.load_address
            if patch_offset < 0 or patch_offset + patch_size > module.size:
                raise ReassemblyError(
                    f"maintained patch {patch['source']} is outside {module.name}"
                )
            for occupied_start, occupied_end in occupied_patch_ranges:
                if patch_offset < occupied_end and occupied_start < patch_offset + patch_size:
                    raise ReassemblyError(
                        f"maintained patch {patch['source']} overlaps another patch"
                    )
            occupied_patch_ranges.append((patch_offset, patch_offset + patch_size))

            original_patch = expected[patch_offset : patch_offset + patch_size]
            if hashlib.sha256(original_patch).hexdigest() != patch["expected_sha256"]:
                raise ReassemblyError(
                    f"expected bytes do not match for maintained patch {patch['source']}"
                )
            patch_source = ROOT / "reasm" / version / patch["source"]
            if not patch_source.is_file():
                raise ReassemblyError(f"missing maintained patch source: {patch_source}")
            patch_obj = object_root / f"{module.name}_patch_{patch_index:03d}.o"
            patch_binary = binary_root / f"{module.name}_patch_{patch_index:03d}.bin"
            patch_payload = assemble_source(
                patch_source,
                patch_obj,
                patch_binary,
                llvm_mc,
                llvm_objcopy,
            )
            if len(patch_payload) != patch_size:
                raise ReassemblyError(
                    f"maintained patch {patch['source']} rebuilt to 0x{len(patch_payload):X}; "
                    f"expected 0x{patch_size:X}"
                )
            patched_payload[patch_offset : patch_offset + patch_size] = patch_payload
        payload = bytes(patched_payload)

        differences = sum(a != b for a, b in zip(expected, payload))
        if differences:
            changed_modules.append(module.name)
            changed_bytes += differences
            if regions_overlap(
                module.rom_offset,
                module.size,
                SECURE_AREA_START,
                SECURE_AREA_END,
            ) and not allow_secure_area_change:
                raise ReassemblyError(
                    f"{module.name} changes bytes in the encrypted DS secure area; "
                    "use dsd with an ARM7 BIOS before enabling such edits"
                )

        key = (module.rom_offset, module.size)
        previous = written_regions.get(key)
        if previous is not None and previous != payload:
            raise ReassemblyError(
                f"multiple overlay records rebuild ROM range 0x{key[0]:X} "
                "with different bytes"
            )
        written_regions[key] = payload
        rebuilt[module.rom_offset : module.rom_offset + module.size] = payload

    if require_matching and rebuilt != base:
        raise ReassemblyError(
            f"baseline is not byte-identical: {changed_bytes} bytes differ in "
            f"{len(changed_modules)} modules"
        )

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(rebuilt)
    output_sha1 = hashlib.sha1(rebuilt).hexdigest()
    print(f"Built: {output}")
    print(f"SHA-1: {output_sha1}")
    print(f"Changed modules: {len(changed_modules)}")
    print(f"Changed bytes: {changed_bytes}")
    if changed_modules:
        print("Modules: " + ", ".join(changed_modules))
    else:
        print("Baseline is byte-identical to the input ROM.")


def doctor(rom: Path, version: str, work: Path) -> None:
    data = rom.read_bytes()
    identity = verify_version(data, version)
    modules = parse_modules(data)
    llvm_mc = resolve_tool(None, ("llvm-mc", "llvm-mc.exe"))
    llvm_objcopy = resolve_tool(None, ("llvm-objcopy", "llvm-objcopy.exe"))
    print(json.dumps(identity, indent=2, sort_keys=True))
    print(f"Native modules: {len(modules)}")
    print(f"LLVM assembler: {llvm_mc}")
    print(f"LLVM objcopy: {llvm_objcopy}")
    print(f"Work directory: {work}")


def add_common(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--rom", type=Path, required=True, help="user-supplied ROM")
    parser.add_argument("--version", choices=sorted(VERSIONS), default="eur")
    parser.add_argument(
        "--work-dir",
        type=Path,
        help="local generated-source directory (default: build/reassembly/VERSION)",
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    extract_parser = subparsers.add_parser(
        "extract", help="materialize local raw assembly sources"
    )
    add_common(extract_parser)
    extract_parser.add_argument("--force", action="store_true")

    build_parser = subparsers.add_parser(
        "build", help="assemble every native module and build a fixed-layout ROM"
    )
    add_common(build_parser)
    build_parser.add_argument("--output", type=Path, required=True)
    build_parser.add_argument("--llvm-mc")
    build_parser.add_argument("--llvm-objcopy")
    build_parser.add_argument(
        "--require-matching",
        action="store_true",
        help="fail unless the complete output ROM is byte-identical",
    )
    build_parser.add_argument(
        "--allow-secure-area-change",
        action="store_true",
        help="unsafe bootstrap escape hatch; does not update KEY1 encryption",
    )

    all_parser = subparsers.add_parser("all", help="extract and build in one command")
    add_common(all_parser)
    all_parser.add_argument("--output", type=Path, required=True)
    all_parser.add_argument("--llvm-mc")
    all_parser.add_argument("--llvm-objcopy")
    all_parser.add_argument("--force", action="store_true")
    all_parser.add_argument("--require-matching", action="store_true")

    doctor_parser = subparsers.add_parser("doctor", help="verify ROM and toolchain")
    add_common(doctor_parser)

    args = parser.parse_args()
    work = args.work_dir or default_work(args.version)
    try:
        if args.command == "extract":
            extract_sources(args.rom, args.version, work, args.force)
        elif args.command == "build":
            build_rom(
                args.rom,
                args.version,
                work,
                args.output,
                args.llvm_mc,
                args.llvm_objcopy,
                args.require_matching,
                args.allow_secure_area_change,
            )
        elif args.command == "all":
            extract_sources(args.rom, args.version, work, args.force)
            build_rom(
                args.rom,
                args.version,
                work,
                args.output,
                args.llvm_mc,
                args.llvm_objcopy,
                args.require_matching,
                False,
            )
        elif args.command == "doctor":
            doctor(args.rom, args.version, work)
        else:
            raise AssertionError(args.command)
    except (OSError, ReassemblyError, json.JSONDecodeError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
