#!/usr/bin/env python3
"""Export, validate, and rebuild the script-bearing members of FEvData.dat.

The current schema deliberately keeps the original container and command
boundaries.  It makes opcode and argument edits safe while the nine typed
field-data sections and every size-changing control-flow operation are still
being recovered.
"""

from __future__ import annotations

import argparse
from collections import deque
import json
import re
import struct
from pathlib import Path
from typing import Any

import data_mod


ROOT = Path(__file__).resolve().parents[1]
SCHEMA = "pit-field-event-scripts-v1"
VM_SCHEMA = "pit-script-vm-descriptors-v1"
SOURCE = "FEvent/FEvData.dat"
FIXED_POINTER_COUNT = 9

# All displacements are signed halfword counts from the already-advanced VM
# program counter.  Inline scripts begin exactly at that counter.
CORE_CODE_REFERENCES = {0x02: 1, 0x04: 4, 0x0C: 3}
FIELD_CODE_REFERENCES = {0x33: 1, 0x34: 0, 0x36: 1, 0x3E: 1}
BRANCH_WITH_FALLTHROUGH = {0x04, 0x0C, 0x33}
SPAWN_WITH_FALLTHROUGH = {0x36, 0x3E}
INLINE_SCRIPT_OPCODES = {0x3C, 0x3D}
TERMINAL_OPCODES = {0x00, 0x01, 0x10B}

NAMESPACES = {
    0x1000: "state",
    0x2000: "save_flags_48",
    0x3000: "field",
    0x4000: "extension_4",
    0x5000: "save_words",
    0x6000: "save_bytes_d0",
    0x7000: "extension_7",
    0x8000: "context",
    0xA000: "state_24",
    0xB000: "state_flags_64",
    0xC000: "save_words_40",
    0xD000: "save_flags_50",
}
NAMESPACE_VALUES = {name: value for value, name in NAMESPACES.items()}
VARIABLE_RE = re.compile(r"([a-z][a-z0-9_]*)\[(0|[1-9][0-9]*)\]")
FIELD_VARIABLE_NAMES = {
    0x3004: "field.owner_subtype",
    0x3006: "field.owner_entity_id",
    0x3008: "field.current_side",
    0x3009: "field.current_room_id",
    0x300A: "field.paired_room_id",
}
FIELD_VARIABLE_VALUES = {name: value for value, name in FIELD_VARIABLE_NAMES.items()}


def load_vm_schema(version: str) -> tuple[tuple[int, ...], dict[int, str]]:
    if not re.fullmatch(r"[a-z0-9_-]+", version):
        raise data_mod.DataModError(f"invalid version {version!r}")
    path = ROOT / "config" / version / "field_vm.json"
    document = data_mod.read_json(path)
    if not isinstance(document, dict) or document.get("schema") != VM_SCHEMA:
        raise data_mod.DataModError(f"{path} must use schema {VM_SCHEMA!r}")
    descriptors = tuple(
        data_mod.parse_integer(value, 7, f"field VM descriptor {index}")
        for index, value in enumerate(
            data_mod._require_list(document.get("descriptors"), "field descriptors")
        )
    )
    names_value = document.get("known_names")
    if not isinstance(names_value, dict):
        raise data_mod.DataModError("field VM known_names must be an object")
    names: dict[int, str] = {}
    for raw_opcode, name in names_value.items():
        try:
            opcode = int(raw_opcode, 0)
        except (TypeError, ValueError) as exc:
            raise data_mod.DataModError(
                f"invalid named field opcode {raw_opcode!r}"
            ) from exc
        if not 0 <= opcode < len(descriptors):
            raise data_mod.DataModError(
                f"named field opcode 0x{opcode:X} is outside the descriptor table"
            )
        if not isinstance(name, str) or not re.fullmatch(r"[a-z][a-z0-9_]*", name):
            raise data_mod.DataModError(f"invalid field opcode name {name!r}")
        names[opcode] = name
    if len(set(names.values())) != len(names):
        raise data_mod.DataModError("field VM opcode names must be unique")
    semantics = document.get("opcode_semantics", {})
    if not isinstance(semantics, dict):
        raise data_mod.DataModError("field VM opcode_semantics must be an object")
    for raw_opcode, row in semantics.items():
        try:
            opcode = int(raw_opcode, 0)
        except (TypeError, ValueError) as exc:
            raise data_mod.DataModError(
                f"invalid field semantic opcode {raw_opcode!r}"
            ) from exc
        if opcode not in names or not 0 <= opcode < len(descriptors):
            raise data_mod.DataModError(
                f"field semantic opcode 0x{opcode:X} must have a known name"
            )
        if not isinstance(row, dict):
            raise data_mod.DataModError(
                f"field semantic opcode 0x{opcode:X} must be an object"
            )
        arguments = data_mod._require_list(
            row.get("arguments"), f"field semantic opcode 0x{opcode:X} arguments"
        )
        if len(arguments) != descriptors[opcode] & 0x1F or not all(
            isinstance(argument, str) and argument for argument in arguments
        ):
            raise data_mod.DataModError(
                f"field semantic opcode 0x{opcode:X} has the wrong argument contract"
            )
        has_result = bool(descriptors[opcode] & 0x20)
        if has_result != (row.get("result") is not None):
            raise data_mod.DataModError(
                f"field semantic opcode 0x{opcode:X} has the wrong result contract"
            )
        for key in ("control_flow", "yield", "evidence"):
            if not isinstance(row.get(key), str) or not row[key]:
                raise data_mod.DataModError(
                    f"field semantic opcode 0x{opcode:X} needs {key} evidence"
                )
    return descriptors, names


def format_variable(variable: int) -> str:
    if variable in FIELD_VARIABLE_NAMES:
        return FIELD_VARIABLE_NAMES[variable]
    if variable & 0xE000 == 0xE000:
        return f"save_flags_1f0[{variable & 0x1FFF}]"
    namespace = variable & 0xF000
    name = NAMESPACES.get(namespace)
    if name is None:
        return f"0x{variable:04X}"
    return f"{name}[{variable & 0x0FFF}]"


def parse_variable(value: Any, context: str) -> int:
    if not isinstance(value, str):
        return data_mod.parse_integer(value, 16, context)
    if value in FIELD_VARIABLE_VALUES:
        return FIELD_VARIABLE_VALUES[value]
    match = VARIABLE_RE.fullmatch(value)
    if match:
        namespace_name, raw_index = match.groups()
        index = int(raw_index)
        if namespace_name == "save_flags_1f0":
            if index >= 0x2000:
                raise data_mod.DataModError(
                    f"{context} save_flags_1f0 index must be below 8192"
                )
            return 0xE000 | index
        namespace = NAMESPACE_VALUES.get(namespace_name)
        if namespace is None:
            raise data_mod.DataModError(
                f"{context} has unknown VM namespace {namespace_name!r}"
            )
        if index >= 0x1000:
            raise data_mod.DataModError(f"{context} namespace index must be below 4096")
        return namespace | index
    return data_mod.parse_integer(value, 16, context)


def opcode_name(opcode: int, names: dict[int, str]) -> str:
    return names.get(opcode, f"op_{opcode:03X}")


def parse_opcode(
    value: Any, descriptors: tuple[int, ...], names: dict[int, str], context: str
) -> int:
    if isinstance(value, str):
        reverse = {name: opcode for opcode, name in names.items()}
        if value in reverse:
            return reverse[value]
        match = re.fullmatch(r"op_([0-9A-Fa-f]{3})", value)
        if match:
            opcode = int(match.group(1), 16)
        else:
            try:
                opcode = int(value, 0)
            except ValueError as exc:
                raise data_mod.DataModError(
                    f"{context} has invalid opcode {value!r}"
                ) from exc
    else:
        opcode = data_mod.parse_integer(value, 16, f"{context} opcode")
    if not 0 <= opcode < len(descriptors):
        raise data_mod.DataModError(
            f"{context} opcode 0x{opcode:X} is outside the field VM table"
        )
    return opcode


def decode_command(
    member: bytes,
    position: int,
    descriptors: tuple[int, ...],
    names: dict[int, str],
    context: str,
) -> tuple[dict[str, Any], int, int]:
    if position + 2 > len(member):
        raise data_mod.DataModError(f"{context}: truncated opcode at 0x{position:X}")
    opcode = struct.unpack_from("<H", member, position)[0]
    if opcode >= len(descriptors):
        raise data_mod.DataModError(
            f"{context}: invalid opcode 0x{opcode:X} at 0x{position:X}"
        )
    descriptor = descriptors[opcode]
    argument_count = descriptor & 0x1F
    cursor = position + 2
    command: dict[str, Any] = {
        "source_offset": f"0x{position:04X}",
        "opcode": opcode_name(opcode, names),
    }
    if descriptor & 0x20:
        if cursor + 2 > len(member):
            raise data_mod.DataModError(
                f"{context}: truncated result variable at 0x{position:X}"
            )
        command["result"] = format_variable(struct.unpack_from("<H", member, cursor)[0])
        cursor += 2
    mode_mask = 0
    if descriptor & 0x40 and argument_count:
        if cursor + 2 > len(member):
            raise data_mod.DataModError(
                f"{context}: truncated argument modes at 0x{position:X}"
            )
        mode_mask = struct.unpack_from("<H", member, cursor)[0]
        cursor += 2
    end = cursor + argument_count * 2
    if end > len(member):
        raise data_mod.DataModError(
            f"{context}: truncated arguments at 0x{position:X}"
        )
    arguments: list[Any] = []
    for index in range(argument_count):
        raw = struct.unpack_from("<H", member, cursor + index * 2)[0]
        if mode_mask & (1 << index):
            arguments.append({"variable": format_variable(raw)})
        else:
            arguments.append(raw if raw < 0x8000 else raw - 0x10000)
    command["args"] = arguments
    unused = mode_mask & ~((1 << argument_count) - 1)
    if unused:
        command["unused_mode_bits"] = f"0x{unused:04X}"
    return command, end, opcode


def literal_argument(command: dict[str, Any], index: int) -> int | None:
    value = command["args"][index]
    return value if isinstance(value, int) and not isinstance(value, bool) else None


def successors(
    command: dict[str, Any], command_end: int, opcode: int
) -> list[int]:
    if opcode in TERMINAL_OPCODES:
        return []
    reference_index = CORE_CODE_REFERENCES.get(opcode)
    if reference_index is None:
        reference_index = FIELD_CODE_REFERENCES.get(opcode)
    if reference_index is not None:
        displacement = literal_argument(command, reference_index)
        if displacement is None:
            return [command_end]
        target = command_end + displacement * 2
        if opcode in SPAWN_WITH_FALLTHROUGH:
            return [target, command_end]
        if opcode in BRANCH_WITH_FALLTHROUGH or (
            opcode == 0x02 and literal_argument(command, 0) == 1
        ):
            return [target, command_end]
        return [target]
    if opcode in INLINE_SCRIPT_OPCODES:
        displacement = literal_argument(command, 2)
        if displacement is None:
            return [command_end]
        return [command_end, command_end + displacement * 2]
    return [command_end]


def decode_graph(
    member: bytes,
    root: int,
    header_size: int,
    descriptors: tuple[int, ...],
    names: dict[int, str],
    context: str,
) -> dict[int, tuple[dict[str, Any], int, int]]:
    pending = deque([root])
    decoded: dict[int, tuple[dict[str, Any], int, int]] = {}
    occupied: dict[int, int] = {}
    while pending:
        position = pending.popleft()
        if position in decoded:
            continue
        if position < header_size or position >= len(member) or position % 2:
            raise data_mod.DataModError(
                f"{context}: control flow reaches invalid offset 0x{position:X}"
            )
        command, end, opcode = decode_command(
            member, position, descriptors, names, context
        )
        for byte_offset in range(position, end):
            other = occupied.get(byte_offset)
            if other is not None and other != position:
                raise data_mod.DataModError(
                    f"{context}: commands at 0x{other:X} and 0x{position:X} overlap"
                )
            occupied[byte_offset] = position
        decoded[position] = (command, end, opcode)
        pending.extend(successors(command, end, opcode))
    return decoded


def parse_member_header(member: bytes, context: str) -> tuple[int, list[int]]:
    if len(member) < FIXED_POINTER_COUNT * 2:
        raise data_mod.DataModError(f"{context}: field member is shorter than 18 bytes")
    header_size = struct.unpack_from("<H", member)[0]
    if (
        header_size < FIXED_POINTER_COUNT * 2
        or header_size > len(member)
        or header_size % 2
    ):
        raise data_mod.DataModError(
            f"{context}: invalid u16 pointer-table size 0x{header_size:X}"
        )
    pointers = list(struct.unpack_from(f"<{header_size // 2}H", member))
    if pointers[0] != header_size:
        raise data_mod.DataModError(
            f"{context}: first pointer does not equal the table size"
        )
    for slot, pointer in enumerate(pointers):
        if pointer and (pointer >= len(member) or pointer % 2):
            raise data_mod.DataModError(
                f"{context}: pointer {slot} has invalid target 0x{pointer:X}"
            )
    fixed = [pointer for pointer in pointers[:FIXED_POINTER_COUNT] if pointer]
    if any(left > right for left, right in zip(fixed, fixed[1:])):
        raise data_mod.DataModError(f"{context}: fixed-section pointers decrease")
    return header_size, pointers


def _command_references(
    command: dict[str, Any], end: int, opcode: int
) -> list[int]:
    reference_index = CORE_CODE_REFERENCES.get(opcode)
    if reference_index is None:
        reference_index = FIELD_CODE_REFERENCES.get(opcode)
    targets: list[int] = []
    if reference_index is not None:
        displacement = literal_argument(command, reference_index)
        if displacement is not None:
            targets.append(end + displacement * 2)
    if opcode in INLINE_SCRIPT_OPCODES:
        targets.append(end)
        displacement = literal_argument(command, 2)
        if displacement is not None:
            targets.append(end + displacement * 2)
    return targets


def analyze_member(
    member: bytes,
    room_id: int,
    room_part: int,
    descriptors: tuple[int, ...],
    names: dict[int, str],
) -> dict[str, Any]:
    context = f"room {room_id} part {room_part}"
    header_size, pointers = parse_member_header(member, context)
    script_pointers = pointers[FIXED_POINTER_COUNT:]
    unique_candidates = sorted(set(pointer for pointer in script_pointers if pointer))
    valid_graphs: dict[int, dict[int, tuple[dict[str, Any], int, int]]] = {}
    invalid_candidates: set[int] = set()
    for root in unique_candidates:
        try:
            valid_graphs[root] = decode_graph(
                member, root, header_size, descriptors, names, context
            )
        except data_mod.DataModError:
            invalid_candidates.add(root)

    root_names = {
        root: f"script_{min(index for index, value in enumerate(script_pointers) if value == root):03d}"
        for root in valid_graphs
    }
    slots: list[Any] = []
    for pointer in script_pointers:
        if pointer == 0:
            slots.append(None)
        elif pointer in root_names:
            slots.append(root_names[pointer])
        else:
            slots.append({"private_target": f"0x{pointer:04X}"})

    merged: dict[int, tuple[dict[str, Any], int, int]] = {}
    for root, graph in valid_graphs.items():
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
                    f"{context}: entry roots decode overlapping commands at "
                    f"0x{other:X} and 0x{position:X}"
                )
            occupied[byte_offset] = position

    labels: dict[int, list[str]] = {}
    for root, name in root_names.items():
        labels.setdefault(root, []).append(name)
    for command, end, opcode in merged.values():
        for target in _command_references(command, end, opcode):
            if header_size <= target < len(member) and target % 2 == 0:
                labels.setdefault(target, []).append(f"loc_{target:04X}")

    commands: list[dict[str, Any]] = []
    for position, (raw_command, end, opcode) in sorted(merged.items()):
        command = dict(raw_command)
        if position in labels:
            command["labels"] = list(dict.fromkeys(labels[position]))
        targets = _command_references(command, end, opcode)
        if targets:
            command["code_targets"] = [f"0x{target:04X}" for target in targets]
        commands.append(command)

    fixed_sections = []
    fixed_offsets = pointers[:FIXED_POINTER_COUNT]
    boundaries = sorted(
        set(pointer for pointer in pointers if pointer) | {len(member)}
    )
    for slot, offset in enumerate(fixed_offsets):
        next_offset = next((value for value in boundaries if value > offset), len(member))
        payload = member[offset:next_offset] if offset else b""
        fixed_sections.append(
            {
                "slot": slot,
                "source_offset": None if not offset else f"0x{offset:04X}",
                "size_to_next_pointer": len(payload),
                "sha1_to_next_pointer": data_mod.sha1(payload),
            }
        )

    return {
        "room_id": room_id,
        "room_part": room_part,
        "source_entry_id": room_id * 3 + room_part,
        "source_entry_size": len(member),
        "source_sha1": data_mod.sha1(member),
        "pointer_table_size": header_size,
        "fixed_sections": fixed_sections,
        "script_slots": slots,
        "valid_script_count": len(valid_graphs),
        "private_target_count": len(invalid_candidates),
        "reachable_command_count": len(merged),
        "commands": commands,
    }


def export_document(
    source_path: Path,
    version: str,
    descriptors: tuple[int, ...],
    names: dict[int, str],
) -> dict[str, Any]:
    source_data = source_path.read_bytes()
    entries = data_mod.parse_offset_archive(source_data)
    if len(entries) % 3:
        raise data_mod.DataModError(
            f"{source_path}: expected room triplets, got {len(entries)} entries"
        )
    members = []
    for entry_id, member in enumerate(entries):
        room_part = entry_id % 3
        if room_part == 1 or not member:
            continue
        members.append(
            analyze_member(
                member, entry_id // 3, room_part, descriptors, names
            )
        )
    return {
        "schema": SCHEMA,
        "version": version,
        "source": SOURCE,
        "source_sha1": data_mod.sha1(source_data),
        "outer_entry_count": len(entries),
        "room_count": len(entries) // 3,
        "layout": "fixed-command-boundaries",
        "members": members,
    }


def _compile_command(
    row: Any,
    descriptors: tuple[int, ...],
    names: dict[int, str],
    context: str,
) -> bytes:
    if not isinstance(row, dict):
        raise data_mod.DataModError(f"{context} must be an object")
    opcode = parse_opcode(row.get("opcode"), descriptors, names, context)
    descriptor = descriptors[opcode]
    encoded = bytearray(struct.pack("<H", opcode))
    has_result = bool(descriptor & 0x20)
    if has_result != ("result" in row):
        requirement = "requires" if has_result else "does not use"
        raise data_mod.DataModError(f"{context} opcode {requirement} a result variable")
    if has_result:
        encoded.extend(struct.pack("<H", parse_variable(row["result"], f"{context} result")))
    arguments = data_mod._require_list(row.get("args"), f"{context} args")
    argument_count = descriptor & 0x1F
    if len(arguments) != argument_count:
        raise data_mod.DataModError(
            f"{context} opcode needs {argument_count} arguments, got {len(arguments)}"
        )
    mode_mask = 0
    raw_arguments = bytearray()
    for index, argument in enumerate(arguments):
        argument_context = f"{context} argument {index}"
        if isinstance(argument, dict):
            if set(argument) != {"variable"} or not descriptor & 0x40:
                raise data_mod.DataModError(
                    f"{argument_context} must be a supported variable reference"
                )
            value = parse_variable(argument["variable"], argument_context)
            mode_mask |= 1 << index
        else:
            value = data_mod.parse_signed_integer(argument, 16, argument_context) & 0xFFFF
        raw_arguments.extend(struct.pack("<H", value))
    if descriptor & 0x40 and argument_count:
        unused = row.get("unused_mode_bits", "0x0000")
        mode_mask |= data_mod.parse_integer(unused, 16, f"{context} unused_mode_bits")
        if mode_mask & ((1 << argument_count) - 1) != sum(
            1 << index for index, argument in enumerate(arguments) if isinstance(argument, dict)
        ):
            raise data_mod.DataModError(
                f"{context} unused_mode_bits overlaps encoded argument modes"
            )
        encoded.extend(struct.pack("<H", mode_mask))
    elif "unused_mode_bits" in row:
        raise data_mod.DataModError(f"{context} cannot contain unused_mode_bits")
    encoded.extend(raw_arguments)
    return bytes(encoded)


def build_document(
    document: dict[str, Any],
    source_data: bytes,
    descriptors: tuple[int, ...],
    names: dict[int, str],
) -> bytes:
    if document.get("schema") != SCHEMA:
        raise data_mod.DataModError(f"field script document must use schema {SCHEMA!r}")
    if document.get("source") != SOURCE:
        raise data_mod.DataModError(f"field script source must remain {SOURCE!r}")
    if document.get("source_sha1") != data_mod.sha1(source_data):
        raise data_mod.DataModError("field script document does not match the private source")
    if document.get("layout") != "fixed-command-boundaries":
        raise data_mod.DataModError("field script layout must remain fixed-command-boundaries")
    entries = data_mod.parse_offset_archive(source_data)
    if document.get("outer_entry_count") != len(entries):
        raise data_mod.DataModError("field script document has the wrong outer entry count")
    if document.get("room_count") != len(entries) // 3:
        raise data_mod.DataModError("field script document has the wrong room count")
    rows = data_mod._require_list(document.get("members"), "field members")
    expected_ids = [index for index, entry in enumerate(entries) if index % 3 != 1 and entry]
    actual_ids: list[int] = []
    rebuilt_entries = list(entries)
    for row in rows:
        if not isinstance(row, dict):
            raise data_mod.DataModError("every field member must be an object")
        entry_id = data_mod.parse_integer(row.get("source_entry_id"), 16, "source_entry_id")
        actual_ids.append(entry_id)
        if entry_id >= len(entries) or entry_id % 3 == 1 or not entries[entry_id]:
            raise data_mod.DataModError(f"invalid field member entry {entry_id}")
        source_member = entries[entry_id]
        context = f"field member {entry_id}"
        if row.get("source_sha1") != data_mod.sha1(source_member):
            raise data_mod.DataModError(f"{context} does not match the private source")
        expected = analyze_member(
            source_member, entry_id // 3, entry_id % 3, descriptors, names
        )
        for key in (
            "room_id",
            "room_part",
            "source_entry_size",
            "pointer_table_size",
            "fixed_sections",
            "script_slots",
            "valid_script_count",
            "private_target_count",
            "reachable_command_count",
        ):
            if row.get(key) != expected[key]:
                raise data_mod.DataModError(f"{context} has modified structural field {key}")
        expected_commands = {
            int(command["source_offset"], 0): command
            for command in expected["commands"]
        }
        command_rows = data_mod._require_list(row.get("commands"), f"{context} commands")
        actual_offsets: list[int] = []
        rebuilt = bytearray(source_member)
        for command_index, command_row in enumerate(command_rows):
            if not isinstance(command_row, dict):
                raise data_mod.DataModError(f"{context} command {command_index} must be an object")
            raw_offset = command_row.get("source_offset")
            try:
                offset = int(raw_offset, 0) if isinstance(raw_offset, str) else int(raw_offset)
            except (TypeError, ValueError) as exc:
                raise data_mod.DataModError(
                    f"{context} command {command_index} has invalid source_offset"
                ) from exc
            actual_offsets.append(offset)
            expected_command = expected_commands.get(offset)
            if expected_command is None:
                raise data_mod.DataModError(
                    f"{context} command {command_index} is not at an original boundary"
                )
            original, end, _ = decode_command(
                source_member, offset, descriptors, names, context
            )
            encoded = _compile_command(command_row, descriptors, names, f"{context} command {command_index}")
            if len(encoded) != end - offset:
                raise data_mod.DataModError(
                    f"{context} command {command_index} changes encoded size"
                )
            rebuilt[offset:end] = encoded
        if actual_offsets != sorted(expected_commands):
            raise data_mod.DataModError(
                f"{context} must retain every command once in source-offset order"
            )
        # Re-decode every original script root so invalid branch edits fail now,
        # rather than crashing the game at runtime.
        parse_member_header(bytes(rebuilt), context)
        for slot in expected["script_slots"]:
            if isinstance(slot, str):
                root = next(
                    int(command["source_offset"], 0)
                    for command in expected["commands"]
                    if slot in command.get("labels", [])
                )
                graph = decode_graph(
                    bytes(rebuilt),
                    root,
                    expected["pointer_table_size"],
                    descriptors,
                    names,
                    context,
                )
                if not set(graph) <= set(expected_commands):
                    raise data_mod.DataModError(
                        f"{context} changed control flow into unexported bytes"
                    )
        rebuilt_entries[entry_id] = bytes(rebuilt)
    if actual_ids != expected_ids:
        raise data_mod.DataModError("field document must retain every nonempty script-bearing member")
    return data_mod.build_offset_archive(rebuilt_entries)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("export", "check", "build"))
    parser.add_argument("--version", default="eur")
    parser.add_argument(
        "--source", type=Path, default=ROOT / "extract" / "eur" / "files" / SOURCE
    )
    parser.add_argument(
        "--document",
        type=Path,
        default=ROOT / "data" / "eur" / "scripts" / "FEvent__FEvData.dat.json",
    )
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    descriptors, names = load_vm_schema(args.version)
    if args.command == "export":
        document = export_document(args.source, args.version, descriptors, names)
        data_mod.write_battle_script_json(args.document, document)
        print(
            f"Exported {len(document['members'])} field members to {args.document}"
        )
        return 0
    document = data_mod.read_json(args.document)
    if not isinstance(document, dict):
        raise data_mod.DataModError("field script document must be an object")
    rebuilt = build_document(document, args.source.read_bytes(), descriptors, names)
    if args.command == "check":
        if rebuilt != args.source.read_bytes():
            raise data_mod.DataModError("field script document contains modifications")
        print(f"Verified byte-identical field scripts in {args.document}")
        return 0
    if args.output is None:
        raise data_mod.DataModError("build requires --output")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(rebuilt)
    print(f"Built {args.output}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except data_mod.DataModError as exc:
        print(f"error: {exc}")
        raise SystemExit(2) from exc
