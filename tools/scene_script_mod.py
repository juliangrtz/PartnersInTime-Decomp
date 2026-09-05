#!/usr/bin/env python3
"""Export and fixed-layout rebuild the Overlay-7 MenuAI scene scripts."""

from __future__ import annotations

import argparse
from collections import deque
import re
import struct
from pathlib import Path
from typing import Any

import data_mod
import field_event_mod as vm_codec


ROOT = Path(__file__).resolve().parents[1]
SCHEMA = "pit-scene-scripts-v1"
VM_SCHEMA = "pit-script-vm-descriptors-v1"
OV007_LOAD_ADDRESS = 0x0206AB80
SCENE_VM_DESCRIPTOR_ADDRESS = 0x0208DCA4
SCENE_VM_DESCRIPTOR_COUNT = 0x0D2
SOURCES = (
    "MenuAI/BAI_iwasaki.dat",
    "MenuAI/MAI_fujioka.dat",
    "MenuAI/MAI_uchida.dat",
)

TERMINAL_OPCODES = {0x00, 0x01, 0xAB}
CORE_CODE_REFERENCES = {0x02: 1, 0x04: 4, 0x0C: 3}
SCENE_BRANCH_REFERENCES = {0xB1: 5, 0xB2: 3, 0xB5: 2, 0xB6: 3}
BRANCH_WITH_FALLTHROUGH = {0x04, 0x0C, *SCENE_BRANCH_REFERENCES}
INLINE_SCRIPT_OPCODES = {0xA5: 2}
SPAWN_SCRIPT_OPCODES = {0xA6: 2, 0xA8: 2}
WAIT_AND_SKIP_OPCODES = {0xA7: 1}

SCENE_VARIABLE_NAMES = {
    0x7000: "scene.owner_id",
    0x7001: "scene.owner_type",
    0x7004: "scene.input_0",
    0x7005: "scene.input_1",
    0x7006: "scene.input_3",
    0x7007: "scene.input_0_with_latch",
    0x7008: "scene.input_1_with_latch",
    0x7009: "scene.input_3_with_latch",
}
SCENE_VARIABLE_VALUES = {name: value for value, name in SCENE_VARIABLE_NAMES.items()}
SHARED_RE = re.compile(r"scene\.shared\[(0|[1-9][0-9]*)\]")


def load_vm_schema(version: str) -> tuple[tuple[int, ...], dict[int, str]]:
    if not re.fullmatch(r"[a-z0-9_-]+", version):
        raise data_mod.DataModError(f"invalid version {version!r}")
    path = ROOT / "config" / version / "scene_vm.json"
    document = data_mod.read_json(path)
    if not isinstance(document, dict) or document.get("schema") != VM_SCHEMA:
        raise data_mod.DataModError(f"{path} must use schema {VM_SCHEMA!r}")
    if document.get("overlay_id") != 7 or document.get(
        "descriptor_table_address"
    ) != f"0x{SCENE_VM_DESCRIPTOR_ADDRESS:08X}":
        raise data_mod.DataModError(f"{path} has the wrong scene VM location")
    descriptors = tuple(
        data_mod.parse_integer(value, 7, f"scene VM descriptor {index}")
        for index, value in enumerate(
            data_mod._require_list(document.get("descriptors"), "scene descriptors")
        )
    )
    if len(descriptors) != SCENE_VM_DESCRIPTOR_COUNT:
        raise data_mod.DataModError(
            f"scene VM schema needs {SCENE_VM_DESCRIPTOR_COUNT} descriptors"
        )
    names_value = document.get("known_names")
    if not isinstance(names_value, dict):
        raise data_mod.DataModError("scene VM known_names must be an object")
    names: dict[int, str] = {}
    for raw_opcode, name in names_value.items():
        try:
            opcode = int(raw_opcode, 0)
        except (TypeError, ValueError) as exc:
            raise data_mod.DataModError(
                f"invalid named scene opcode {raw_opcode!r}"
            ) from exc
        if not 0 <= opcode < len(descriptors):
            raise data_mod.DataModError(
                f"named scene opcode 0x{opcode:X} is outside the descriptor table"
            )
        if not isinstance(name, str) or not re.fullmatch(r"[a-z][a-z0-9_]*", name):
            raise data_mod.DataModError(f"invalid scene opcode name {name!r}")
        names[opcode] = name
    if len(set(names.values())) != len(names):
        raise data_mod.DataModError("scene VM opcode names must be unique")
    semantics = document.get("opcode_semantics", {})
    if not isinstance(semantics, dict):
        raise data_mod.DataModError("scene VM opcode_semantics must be an object")
    for raw_opcode, row in semantics.items():
        try:
            opcode = int(raw_opcode, 0)
        except (TypeError, ValueError) as exc:
            raise data_mod.DataModError(
                f"invalid scene semantic opcode {raw_opcode!r}"
            ) from exc
        if opcode not in names or not 0 <= opcode < len(descriptors):
            raise data_mod.DataModError(
                f"scene semantic opcode 0x{opcode:X} must have a known name"
            )
        if not isinstance(row, dict):
            raise data_mod.DataModError(
                f"scene semantic opcode 0x{opcode:X} must be an object"
            )
        arguments = data_mod._require_list(
            row.get("arguments"), f"scene semantic opcode 0x{opcode:X} arguments"
        )
        if len(arguments) != descriptors[opcode] & 0x1F or not all(
            isinstance(argument, str) and argument for argument in arguments
        ):
            raise data_mod.DataModError(
                f"scene semantic opcode 0x{opcode:X} has the wrong argument contract"
            )
        has_result = bool(descriptors[opcode] & 0x20)
        if has_result != (row.get("result") is not None):
            raise data_mod.DataModError(
                f"scene semantic opcode 0x{opcode:X} has the wrong result contract"
            )
        for key in ("control_flow", "yield", "evidence"):
            if not isinstance(row.get(key), str) or not row[key]:
                raise data_mod.DataModError(
                    f"scene semantic opcode 0x{opcode:X} needs {key} evidence"
                )
    return descriptors, names


def verify_vm_descriptors(overlay: bytes, descriptors: tuple[int, ...]) -> None:
    offset = SCENE_VM_DESCRIPTOR_ADDRESS - OV007_LOAD_ADDRESS
    size = SCENE_VM_DESCRIPTOR_COUNT * 4
    if offset < 0 or offset + size > len(overlay):
        raise data_mod.DataModError("scene VM descriptor table is outside overlay 7")
    source_descriptors = struct.unpack_from(
        f"<{SCENE_VM_DESCRIPTOR_COUNT}I", overlay, offset
    )
    if source_descriptors != descriptors:
        raise data_mod.DataModError(
            "scene VM schema does not match the private overlay-7 descriptor table"
        )


def format_variable(variable: int) -> str:
    if variable in SCENE_VARIABLE_NAMES:
        return SCENE_VARIABLE_NAMES[variable]
    if 0x7010 <= variable <= 0x702F:
        return f"scene.shared[{variable - 0x7010}]"
    return vm_codec.format_variable(variable)


def parse_variable(value: Any, context: str) -> int:
    if isinstance(value, str):
        if value in SCENE_VARIABLE_VALUES:
            return SCENE_VARIABLE_VALUES[value]
        match = SHARED_RE.fullmatch(value)
        if match:
            index = int(match.group(1))
            if index >= 32:
                raise data_mod.DataModError(
                    f"{context} scene.shared index must be below 32"
                )
            return 0x7010 + index
    return vm_codec.parse_variable(value, context)


def successors(command: dict[str, Any], command_end: int, opcode: int) -> list[int]:
    if opcode in TERMINAL_OPCODES:
        return []
    reference_index = CORE_CODE_REFERENCES.get(opcode)
    if reference_index is None:
        reference_index = SCENE_BRANCH_REFERENCES.get(opcode)
    if reference_index is not None:
        displacement = vm_codec.literal_argument(command, reference_index)
        if displacement is None:
            return [command_end]
        target = command_end + displacement * 2
        if opcode in BRANCH_WITH_FALLTHROUGH or (
            opcode == 0x02 and vm_codec.literal_argument(command, 0) == 1
        ):
            return [target, command_end]
        return [target]
    if opcode in INLINE_SCRIPT_OPCODES:
        displacement = vm_codec.literal_argument(
            command, INLINE_SCRIPT_OPCODES[opcode]
        )
        if displacement is None:
            return [command_end]
        return [command_end, command_end + displacement * 2]
    if opcode in SPAWN_SCRIPT_OPCODES:
        displacement = vm_codec.literal_argument(
            command, SPAWN_SCRIPT_OPCODES[opcode]
        )
        if displacement is None:
            return [command_end]
        return [command_end, command_end + displacement * 2]
    if opcode in WAIT_AND_SKIP_OPCODES:
        displacement = vm_codec.literal_argument(
            command, WAIT_AND_SKIP_OPCODES[opcode]
        )
        return [command_end] if displacement is None else [command_end + displacement * 2]
    return [command_end]


def code_references(
    command: dict[str, Any], command_end: int, opcode: int
) -> list[int]:
    values = successors(command, command_end, opcode)
    if opcode in TERMINAL_OPCODES:
        return []
    if opcode not in {
        *CORE_CODE_REFERENCES,
        *SCENE_BRANCH_REFERENCES,
        *INLINE_SCRIPT_OPCODES,
        *SPAWN_SCRIPT_OPCODES,
        *WAIT_AND_SKIP_OPCODES,
    }:
        return []
    return list(dict.fromkeys(values))


def analyze_entry(
    entry: bytes,
    entry_id: int,
    descriptors: tuple[int, ...],
    names: dict[int, str],
    source: str,
) -> dict[str, Any]:
    context = f"{source} entry {entry_id}"
    if len(entry) < 2:
        raise data_mod.DataModError(f"{context}: entry is shorter than its header")
    header_size = struct.unpack_from("<H", entry)[0]
    if header_size < 2 or header_size > len(entry) or header_size % 2:
        raise data_mod.DataModError(
            f"{context}: invalid pointer-table size 0x{header_size:X}"
        )
    pointers = list(struct.unpack_from(f"<{header_size // 2}H", entry))
    if pointers[0] != header_size:
        raise data_mod.DataModError(f"{context}: first script does not follow its header")
    if any(
        pointer and (pointer < header_size or pointer >= len(entry) or pointer % 2)
        for pointer in pointers
    ):
        raise data_mod.DataModError(f"{context}: invalid script entry pointer")

    roots = sorted(set(pointer for pointer in pointers if pointer))
    root_names = {
        root: f"entry_{min(index for index, value in enumerate(pointers) if value == root):03d}"
        for root in roots
    }
    merged: dict[int, tuple[dict[str, Any], int, int]] = {}
    for root in roots:
        graph = vm_codec.decode_graph(
            entry,
            root,
            header_size,
            descriptors,
            names,
            context,
            successors,
            format_variable,
        )
        for position, decoded in graph.items():
            previous = merged.get(position)
            if previous is not None and previous[1:] != decoded[1:]:
                raise data_mod.DataModError(
                    f"{context}: inconsistent decode at 0x{position:X}"
                )
            merged[position] = decoded

    occupied: dict[int, int] = {}
    for position, (_, end, _) in merged.items():
        for byte_offset in range(position, end):
            other = occupied.get(byte_offset)
            if other is not None and other != position:
                raise data_mod.DataModError(
                    f"{context}: commands overlap at 0x{other:X} and 0x{position:X}"
                )
            occupied[byte_offset] = position

    labels: dict[int, list[str]] = {}
    for root, name in root_names.items():
        labels.setdefault(root, []).append(name)
    for command, end, opcode in merged.values():
        for target in code_references(command, end, opcode):
            if header_size <= target < len(entry) and target % 2 == 0:
                labels.setdefault(target, []).append(f"loc_{target:04X}")

    commands = []
    for position, (raw_command, end, opcode) in sorted(merged.items()):
        command = dict(raw_command)
        if position in labels:
            command["labels"] = list(dict.fromkeys(labels[position]))
        targets = code_references(command, end, opcode)
        if targets:
            command["code_targets"] = [f"0x{target:04X}" for target in targets]
        commands.append(command)
    return {
        "entry_id": entry_id,
        "source_entry_size": len(entry),
        "source_sha1": data_mod.sha1(entry),
        "pointer_table_size": header_size,
        "entry_points": [None if not pointer else root_names[pointer] for pointer in pointers],
        "reachable_command_count": len(commands),
        "private_byte_count": len(entry) - header_size - len(occupied),
        "commands": commands,
    }


def export_document(
    files_root: Path,
    version: str,
    descriptors: tuple[int, ...],
    names: dict[int, str],
) -> dict[str, Any]:
    archives = []
    for source in SOURCES:
        path = files_root / source
        entries = data_mod.parse_offset_archive(path.read_bytes())
        archives.append(
            {
                "source": source,
                "source_sha1": data_mod.sha1(path.read_bytes()),
                "outer_entry_count": len(entries),
                "entries": [
                    analyze_entry(entry, entry_id, descriptors, names, source)
                    for entry_id, entry in enumerate(entries)
                ],
            }
        )
    return {
        "schema": SCHEMA,
        "version": version,
        "layout": "fixed-command-boundaries",
        "archives": archives,
    }


def build_document(
    document: dict[str, Any],
    files_root: Path,
    descriptors: tuple[int, ...],
    names: dict[int, str],
) -> dict[str, bytes]:
    if document.get("schema") != SCHEMA:
        raise data_mod.DataModError(f"scene script document must use schema {SCHEMA!r}")
    if document.get("layout") != "fixed-command-boundaries":
        raise data_mod.DataModError("scene script layout must remain fixed-command-boundaries")
    archives = data_mod._require_list(document.get("archives"), "scene archives")
    if [archive.get("source") for archive in archives if isinstance(archive, dict)] != list(SOURCES):
        raise data_mod.DataModError("scene script document must retain the three source archives")
    rebuilt_archives: dict[str, bytes] = {}
    for archive in archives:
        if not isinstance(archive, dict):
            raise data_mod.DataModError("every scene archive must be an object")
        source = archive["source"]
        source_data = (files_root / source).read_bytes()
        if archive.get("source_sha1") != data_mod.sha1(source_data):
            raise data_mod.DataModError(f"{source} does not match the private source")
        entries = data_mod.parse_offset_archive(source_data)
        rows = data_mod._require_list(archive.get("entries"), f"{source} entries")
        if archive.get("outer_entry_count") != len(entries) or len(rows) != len(entries):
            raise data_mod.DataModError(f"{source} has the wrong entry count")
        rebuilt_entries = []
        for entry_id, (entry, row) in enumerate(zip(entries, rows)):
            if not isinstance(row, dict) or row.get("entry_id") != entry_id:
                raise data_mod.DataModError(f"{source} entry IDs must stay contiguous")
            context = f"{source} entry {entry_id}"
            expected = analyze_entry(entry, entry_id, descriptors, names, source)
            for key in (
                "source_entry_size",
                "source_sha1",
                "pointer_table_size",
                "entry_points",
                "reachable_command_count",
                "private_byte_count",
            ):
                if row.get(key) != expected[key]:
                    raise data_mod.DataModError(f"{context} has modified structural field {key}")
            expected_commands = {
                int(command["source_offset"], 0): command
                for command in expected["commands"]
            }
            command_rows = data_mod._require_list(row.get("commands"), f"{context} commands")
            rebuilt = bytearray(entry)
            actual_offsets = []
            for command_index, command in enumerate(command_rows):
                if not isinstance(command, dict):
                    raise data_mod.DataModError(
                        f"{context} command {command_index} must be an object"
                    )
                try:
                    offset = int(command.get("source_offset"), 0)
                except (TypeError, ValueError) as exc:
                    raise data_mod.DataModError(
                        f"{context} command {command_index} has invalid source_offset"
                    ) from exc
                actual_offsets.append(offset)
                if offset not in expected_commands:
                    raise data_mod.DataModError(
                        f"{context} command {command_index} is not at an original boundary"
                    )
                _, end, _ = vm_codec.decode_command(
                    entry, offset, descriptors, names, context, format_variable
                )
                encoded = vm_codec._compile_command(
                    command,
                    descriptors,
                    names,
                    f"{context} command {command_index}",
                    parse_variable,
                )
                if len(encoded) != end - offset:
                    raise data_mod.DataModError(
                        f"{context} command {command_index} changes encoded size"
                    )
                rebuilt[offset:end] = encoded
            if actual_offsets != sorted(expected_commands):
                raise data_mod.DataModError(
                    f"{context} must retain every command once in source-offset order"
                )
            roots = sorted(
                set(
                    struct.unpack_from(
                        f"<{expected['pointer_table_size'] // 2}H", rebuilt
                    )
                )
                - {0}
            )
            for root in roots:
                graph = vm_codec.decode_graph(
                    bytes(rebuilt),
                    root,
                    expected["pointer_table_size"],
                    descriptors,
                    names,
                    context,
                    successors,
                    format_variable,
                )
                if not set(graph) <= set(expected_commands):
                    raise data_mod.DataModError(
                        f"{context} changed control flow into unexported bytes"
                    )
            rebuilt_entries.append(bytes(rebuilt))
        rebuilt_archives[source] = data_mod.build_offset_archive(rebuilt_entries)
    return rebuilt_archives


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("export", "check", "build"))
    parser.add_argument("--version", default="eur")
    parser.add_argument(
        "--files-root", type=Path, default=ROOT / "extract" / "eur" / "files"
    )
    parser.add_argument(
        "--document",
        type=Path,
        default=ROOT / "data" / "eur" / "scripts" / "MenuAI__scene_scripts.json",
    )
    parser.add_argument("--output-root", type=Path)
    args = parser.parse_args()
    descriptors, names = load_vm_schema(args.version)
    if args.command == "export":
        document = export_document(args.files_root, args.version, descriptors, names)
        data_mod.write_battle_script_json(args.document, document)
        command_count = sum(
            entry["reachable_command_count"]
            for archive in document["archives"]
            for entry in archive["entries"]
        )
        print(f"Exported 3 scene archives and {command_count} commands to {args.document}")
        return 0
    document = data_mod.read_json(args.document)
    if not isinstance(document, dict):
        raise data_mod.DataModError("scene script document must be an object")
    rebuilt = build_document(document, args.files_root, descriptors, names)
    if args.command == "check":
        for source, data in rebuilt.items():
            if data != (args.files_root / source).read_bytes():
                raise data_mod.DataModError(f"{source} contains modifications")
        print(f"Verified byte-identical scene scripts in {args.document}")
        return 0
    if args.output_root is None:
        raise data_mod.DataModError("build requires --output-root")
    for source, data in rebuilt.items():
        output = args.output_root / source
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_bytes(data)
        print(f"Built {output}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (data_mod.DataModError, OSError) as exc:
        print(f"error: {exc}")
        raise SystemExit(2) from exc
