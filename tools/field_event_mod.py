#!/usr/bin/env python3
"""Export, validate, and rebuild the script-bearing members of FEvData.dat.

The checked-in form is sharded by room and deliberately keeps the original
member and command boundaries. It makes opcode, argument, roaming-profile, and
waypoint edits safe while relocatable field members are still being recovered.
"""

from __future__ import annotations

import argparse
from collections import Counter, deque
import json
import re
import struct
from pathlib import Path
from typing import Any

import data_mod


ROOT = Path(__file__).resolve().parents[1]
SCHEMA = "pit-field-event-scripts-v1"
SHARDED_SCHEMA = "pit-field-event-scripts-v2"
ROOM_SCHEMA = "pit-field-event-room-v1"
USAGE_SCHEMA = "pit-script-vm-usage-v1"
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
EMBEDDED_DATA_REFERENCE_ARGUMENTS = {0x93: 1, 0x9A: 1}

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
    0x300B: "field.current_room_companion_id",
    0x3023: "field.save_location_id",
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
    variable_formatter: Any = None,
) -> tuple[dict[str, Any], int, int]:
    if variable_formatter is None:
        variable_formatter = format_variable
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
        command["result"] = variable_formatter(struct.unpack_from("<H", member, cursor)[0])
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
            arguments.append({"variable": variable_formatter(raw)})
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


def embedded_data_target(
    command: dict[str, Any], command_end: int, opcode: int
) -> int | None:
    reference_index = EMBEDDED_DATA_REFERENCE_ARGUMENTS.get(opcode)
    if reference_index is None:
        return None
    displacement = literal_argument(command, reference_index)
    if displacement is None:
        return None
    return command_end + displacement * 2


def decode_embedded_data_record(
    member: bytes, target: int, opcode: int, context: str
) -> tuple[dict[str, Any], int]:
    if target < 0 or target + 4 > len(member) or target % 2:
        raise data_mod.DataModError(
            f"{context}: embedded data target 0x{target:X} is invalid"
        )
    size_words = struct.unpack_from("<I", member, target)[0]
    end = target + 4 + size_words * 4
    if end > len(member):
        raise data_mod.DataModError(
            f"{context}: embedded data at 0x{target:X} is truncated"
        )
    if opcode == 0x93:
        if size_words != 5:
            raise data_mod.DataModError(
                f"{context}: roaming profile at 0x{target:X} has {size_words} words"
            )
        legacy_parameter, speed, distance, delay, direction_count = struct.unpack_from(
            "<IIiII", member, target + 4
        )
        return (
            {
                "label": f"roaming_profile_{target:04X}",
                "source_offset": f"0x{target:04X}",
                "type": "entity_roaming_profile",
                "size_words": size_words,
                "legacy_parameter": legacy_parameter,
                "speed_q12": speed,
                "step_distance": distance,
                "post_step_delay_frames": delay,
                "direction_count": direction_count,
            },
            end,
        )
    if opcode == 0x9A:
        if size_words < 4 or (size_words - 4) % 2:
            raise data_mod.DataModError(
                f"{context}: waypoint path at 0x{target:X} has invalid size {size_words}"
            )
        ping_pong, relative, random_direction, delay = struct.unpack_from(
            "<IIII", member, target + 4
        )
        waypoint_count = (size_words - 4) // 2
        waypoints = [
            {"x": x, "y": y}
            for x, y in struct.iter_unpack(
                "<ii", member[target + 20 : target + 20 + waypoint_count * 8]
            )
        ]
        return (
            {
                "label": f"waypoint_path_{target:04X}",
                "source_offset": f"0x{target:04X}",
                "type": "entity_waypoint_path",
                "size_words": size_words,
                "ping_pong": ping_pong,
                "relative_coordinates": relative,
                "random_direction": random_direction,
                "post_segment_delay_frames": delay,
                "waypoints": waypoints,
            },
            end,
        )
    raise AssertionError(f"unsupported embedded-data opcode 0x{opcode:X}")


def compile_embedded_data_record(
    row: Any, expected: dict[str, Any], context: str
) -> bytes:
    if not isinstance(row, dict):
        raise data_mod.DataModError(f"{context} must be an object")
    for key in ("label", "source_offset", "type", "size_words"):
        if row.get(key) != expected[key]:
            raise data_mod.DataModError(f"{context} has modified structural field {key}")
    size_words = expected["size_words"]
    encoded = bytearray(struct.pack("<I", size_words))
    if expected["type"] == "entity_roaming_profile":
        direction_count = data_mod.parse_integer(
            row.get("direction_count"), 32, f"{context} direction_count"
        )
        if direction_count not in (4, 8):
            raise data_mod.DataModError(
                f"{context} direction_count must be 4 or 8"
            )
        encoded.extend(
            struct.pack(
                "<IIiII",
                data_mod.parse_integer(
                    row.get("legacy_parameter"),
                    16,
                    f"{context} legacy_parameter",
                ),
                data_mod.parse_integer(
                    row.get("speed_q12"), 32, f"{context} speed_q12"
                ),
                data_mod.parse_signed_integer(
                    row.get("step_distance"),
                    32,
                    f"{context} step_distance",
                ),
                data_mod.parse_integer(
                    row.get("post_step_delay_frames"),
                    12,
                    f"{context} post_step_delay_frames",
                ),
                direction_count,
            )
        )
    elif expected["type"] == "entity_waypoint_path":
        for key in ("ping_pong", "relative_coordinates", "random_direction"):
            encoded.extend(
                struct.pack(
                    "<I", data_mod.parse_integer(row.get(key), 1, f"{context} {key}")
                )
            )
        encoded.extend(
            struct.pack(
                "<I",
                data_mod.parse_integer(
                    row.get("post_segment_delay_frames"),
                    13,
                    f"{context} post_segment_delay_frames",
                ),
            )
        )
        waypoints = data_mod._require_list(
            row.get("waypoints"), f"{context} waypoints"
        )
        expected_count = (size_words - 4) // 2
        if len(waypoints) != expected_count:
            raise data_mod.DataModError(
                f"{context} must retain {expected_count} waypoints"
            )
        for waypoint_index, waypoint in enumerate(waypoints):
            waypoint_context = f"{context} waypoint {waypoint_index}"
            if not isinstance(waypoint, dict) or set(waypoint) != {"x", "y"}:
                raise data_mod.DataModError(
                    f"{waypoint_context} must contain exactly x and y"
                )
            encoded.extend(
                struct.pack(
                    "<ii",
                    data_mod.parse_signed_integer(
                        waypoint["x"], 32, f"{waypoint_context} x"
                    ),
                    data_mod.parse_signed_integer(
                        waypoint["y"], 32, f"{waypoint_context} y"
                    ),
                )
            )
    else:
        raise data_mod.DataModError(
            f"{context} has unsupported type {expected['type']!r}"
        )
    if len(encoded) != 4 + size_words * 4:
        raise data_mod.DataModError(f"{context} changes encoded size")
    return bytes(encoded)


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
    successor_resolver: Any = None,
    variable_formatter: Any = None,
) -> dict[int, tuple[dict[str, Any], int, int]]:
    if successor_resolver is None:
        successor_resolver = successors
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
            member,
            position,
            descriptors,
            names,
            context,
            variable_formatter,
        )
        for byte_offset in range(position, end):
            other = occupied.get(byte_offset)
            if other is not None and other != position:
                raise data_mod.DataModError(
                    f"{context}: commands at 0x{other:X} and 0x{position:X} overlap"
                )
            occupied[byte_offset] = position
        decoded[position] = (command, end, opcode)
        pending.extend(successor_resolver(command, end, opcode))
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

    def merge_graphs() -> dict[int, tuple[dict[str, Any], int, int]]:
        result: dict[int, tuple[dict[str, Any], int, int]] = {}
        for graph in valid_graphs.values():
            for position, decoded in graph.items():
                previous = result.get(position)
                if previous is not None and previous[1:] != decoded[1:]:
                    raise data_mod.DataModError(
                        f"{context}: inconsistent decode at 0x{position:X}"
                    )
                result[position] = decoded
        return result

    # Some pointer-table slots point at private records. Before their formats
    # were known, a record whose first u16 happened to be a valid opcode could
    # be mistaken for a script root. Resolve typed data references from genuine
    # scripts and remove those false-positive graphs before reporting coverage.
    while True:
        preliminary = merge_graphs()
        record_ranges: list[tuple[int, int]] = []
        for command, end, opcode in preliminary.values():
            target = embedded_data_target(command, end, opcode)
            if target is None:
                continue
            try:
                _, record_end = decode_embedded_data_record(
                    member, target, opcode, context
                )
            except data_mod.DataModError:
                continue
            record_ranges.append((target, record_end))
        false_roots = {
            root
            for root in valid_graphs
            if any(start <= root < end for start, end in record_ranges)
        }
        if not false_roots:
            break
        for root in false_roots:
            del valid_graphs[root]
            invalid_candidates.add(root)

    root_names = {
        root: f"script_{min(index for index, value in enumerate(script_pointers) if value == root):03d}"
        for root in valid_graphs
    }
    merged = merge_graphs()

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

    embedded_data: dict[int, tuple[dict[str, Any], int]] = {}
    embedded_occupied: dict[int, int] = {}
    for command, end, opcode in merged.values():
        target = embedded_data_target(command, end, opcode)
        if target is None:
            continue
        record, record_end = decode_embedded_data_record(
            member, target, opcode, context
        )
        previous = embedded_data.get(target)
        if previous is not None and previous != (record, record_end):
            raise data_mod.DataModError(
                f"{context}: embedded data at 0x{target:X} has conflicting types"
            )
        for byte_offset in range(target, record_end):
            command_owner = occupied.get(byte_offset)
            if command_owner is not None:
                raise data_mod.DataModError(
                    f"{context}: embedded data at 0x{target:X} overlaps command "
                    f"0x{command_owner:X}"
                )
            other = embedded_occupied.get(byte_offset)
            if other is not None and other != target:
                raise data_mod.DataModError(
                    f"{context}: embedded data at 0x{target:X} overlaps record "
                    f"0x{other:X}"
                )
            embedded_occupied[byte_offset] = target
        embedded_data[target] = (record, record_end)
        command["data_record"] = record["label"]

    slots: list[Any] = []
    for pointer in script_pointers:
        if pointer == 0:
            slots.append(None)
        elif pointer in root_names:
            slots.append(root_names[pointer])
        elif pointer in embedded_data:
            slots.append({"data_record": embedded_data[pointer][0]["label"]})
        else:
            slots.append({"private_target": f"0x{pointer:04X}"})

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
        "private_target_count": len(invalid_candidates - set(embedded_data)),
        "reachable_command_count": len(merged),
        "embedded_data_record_count": len(embedded_data),
        "embedded_data_records": [
            record
            for record, _ in (
                embedded_data[target] for target in sorted(embedded_data)
            )
        ],
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


def write_sharded_document(document: dict[str, Any], manifest_path: Path) -> None:
    """Write one reviewable source file per room plus a compact manifest."""
    if document.get("schema") != SCHEMA:
        raise data_mod.DataModError(
            f"field script document must use schema {SCHEMA!r}"
        )
    room_count = document.get("room_count")
    if not isinstance(room_count, int) or room_count < 0:
        raise data_mod.DataModError("field script document has an invalid room_count")
    members = data_mod._require_list(document.get("members"), "field members")
    by_room: dict[int, list[dict[str, Any]]] = {room_id: [] for room_id in range(room_count)}
    for member in members:
        if not isinstance(member, dict):
            raise data_mod.DataModError("every field member must be an object")
        room_id = member.get("room_id")
        if not isinstance(room_id, int) or room_id not in by_room:
            raise data_mod.DataModError(f"invalid field member room_id {room_id!r}")
        by_room[room_id].append(member)

    shard_directory_name = manifest_path.stem
    shard_directory = manifest_path.parent / shard_directory_name
    room_documents = []
    for room_id in range(room_count):
        relative = f"{shard_directory_name}/room_{room_id:03d}.json"
        room_documents.append(relative)
        data_mod.write_battle_script_json(
            manifest_path.parent / relative,
            {
                "schema": ROOM_SCHEMA,
                "source": document.get("source"),
                "room_id": room_id,
                "members": by_room[room_id],
            },
        )

    manifest = {
        key: value for key, value in document.items() if key != "members"
    }
    manifest["schema"] = SHARDED_SCHEMA
    manifest["room_documents"] = room_documents
    data_mod.write_json(manifest_path, manifest)


def load_document(document_path: Path) -> dict[str, Any]:
    """Load either the original monolithic schema or the room-sharded schema."""
    document = data_mod.read_json(document_path)
    if not isinstance(document, dict):
        raise data_mod.DataModError("field script document must be an object")
    if document.get("schema") == SCHEMA:
        return document
    if document.get("schema") != SHARDED_SCHEMA:
        raise data_mod.DataModError(
            f"field script document must use schema {SCHEMA!r} or {SHARDED_SCHEMA!r}"
        )

    room_count = document.get("room_count")
    if not isinstance(room_count, int) or room_count < 0:
        raise data_mod.DataModError("field script manifest has an invalid room_count")
    room_documents = data_mod._require_list(
        document.get("room_documents"), "field room_documents"
    )
    if len(room_documents) != room_count:
        raise data_mod.DataModError(
            f"field script manifest needs {room_count} room documents"
        )

    members: list[dict[str, Any]] = []
    for expected_room_id, relative in enumerate(room_documents):
        if not isinstance(relative, str) or not relative or "\\" in relative:
            raise data_mod.DataModError(
                f"field room document {expected_room_id} must be a POSIX relative path"
            )
        relative_path = Path(relative)
        if relative_path.is_absolute() or ".." in relative_path.parts:
            raise data_mod.DataModError(
                f"field room document {expected_room_id} escapes the manifest directory"
            )
        room = data_mod.read_json(document_path.parent / relative_path)
        if not isinstance(room, dict) or room.get("schema") != ROOM_SCHEMA:
            raise data_mod.DataModError(
                f"field room document {relative!r} must use schema {ROOM_SCHEMA!r}"
            )
        if room.get("source") != document.get("source"):
            raise data_mod.DataModError(
                f"field room document {relative!r} has the wrong source"
            )
        if room.get("room_id") != expected_room_id:
            raise data_mod.DataModError(
                f"field room document {relative!r} must describe room {expected_room_id}"
            )
        room_members = data_mod._require_list(
            room.get("members"), f"field room {expected_room_id} members"
        )
        for member in room_members:
            if not isinstance(member, dict) or member.get("room_id") != expected_room_id:
                raise data_mod.DataModError(
                    f"field room document {relative!r} contains a foreign member"
                )
            members.append(member)

    combined = {
        key: value
        for key, value in document.items()
        if key not in {"room_documents"}
    }
    combined["schema"] = SCHEMA
    combined["members"] = members
    return combined


def _compile_command(
    row: Any,
    descriptors: tuple[int, ...],
    names: dict[int, str],
    context: str,
    variable_parser: Any = None,
) -> bytes:
    if variable_parser is None:
        variable_parser = parse_variable
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
        encoded.extend(
            struct.pack("<H", variable_parser(row["result"], f"{context} result"))
        )
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
            value = variable_parser(argument["variable"], argument_context)
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
            "embedded_data_record_count",
        ):
            if row.get(key) != expected[key]:
                raise data_mod.DataModError(f"{context} has modified structural field {key}")
        expected_commands = {
            int(command["source_offset"], 0): command
            for command in expected["commands"]
        }
        command_rows = data_mod._require_list(
            row.get("commands"), f"{context} commands"
        )
        actual_offsets: list[int] = []
        rebuilt = bytearray(source_member)
        expected_data_records = {
            int(record["source_offset"], 0): record
            for record in expected["embedded_data_records"]
        }
        data_rows = data_mod._require_list(
            row.get("embedded_data_records"), f"{context} embedded_data_records"
        )
        actual_data_offsets: list[int] = []
        for record_index, record_row in enumerate(data_rows):
            if not isinstance(record_row, dict):
                raise data_mod.DataModError(
                    f"{context} embedded data record {record_index} must be an object"
                )
            raw_offset = record_row.get("source_offset")
            try:
                offset = (
                    int(raw_offset, 0)
                    if isinstance(raw_offset, str)
                    else int(raw_offset)
                )
            except (TypeError, ValueError) as exc:
                raise data_mod.DataModError(
                    f"{context} embedded data record {record_index} has invalid "
                    "source_offset"
                ) from exc
            actual_data_offsets.append(offset)
            expected_record = expected_data_records.get(offset)
            if expected_record is None:
                raise data_mod.DataModError(
                    f"{context} embedded data record {record_index} is not original"
                )
            encoded = compile_embedded_data_record(
                record_row,
                expected_record,
                f"{context} embedded data record {record_index}",
            )
            rebuilt[offset : offset + len(encoded)] = encoded
        if actual_data_offsets != sorted(expected_data_records):
            raise data_mod.DataModError(
                f"{context} must retain every embedded data record once in "
                "source-offset order"
            )
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
            original, end, opcode = decode_command(
                source_member, offset, descriptors, names, context
            )
            if command_row.get("data_record") != expected_command.get("data_record"):
                raise data_mod.DataModError(
                    f"{context} command {command_index} has modified data_record"
                )
            original_data_target = embedded_data_target(original, end, opcode)
            edited_data_target = embedded_data_target(command_row, end, opcode)
            if edited_data_target != original_data_target:
                raise data_mod.DataModError(
                    f"{context} command {command_index} cannot retarget embedded data"
                )
            encoded = _compile_command(
                command_row,
                descriptors,
                names,
                f"{context} command {command_index}",
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


def summarize_document(
    document: dict[str, Any],
    descriptors: tuple[int, ...],
    names: dict[int, str],
) -> dict[str, Any]:
    if document.get("schema") != SCHEMA:
        raise data_mod.DataModError(
            f"field script document must use schema {SCHEMA!r}"
        )
    members = data_mod._require_list(document.get("members"), "field members")
    usage: Counter[int] = Counter()
    valid_script_count = 0
    private_target_count = 0
    embedded_data_types: Counter[str] = Counter()
    for member_index, member in enumerate(members):
        if not isinstance(member, dict):
            raise data_mod.DataModError(
                f"field member {member_index} must be an object"
            )
        valid_script_count += data_mod.parse_integer(
            member.get("valid_script_count"),
            32,
            f"field member {member_index} valid_script_count",
        )
        private_target_count += data_mod.parse_integer(
            member.get("private_target_count"),
            32,
            f"field member {member_index} private_target_count",
        )
        records = data_mod._require_list(
            member.get("embedded_data_records"),
            f"field member {member_index} embedded_data_records",
        )
        for record_index, record in enumerate(records):
            if not isinstance(record, dict) or not isinstance(
                record.get("type"), str
            ):
                raise data_mod.DataModError(
                    f"field member {member_index} embedded data record {record_index} "
                    "must have a type"
                )
            embedded_data_types[record["type"]] += 1
        commands = data_mod._require_list(
            member.get("commands"), f"field member {member_index} commands"
        )
        for command_index, command in enumerate(commands):
            if not isinstance(command, dict):
                raise data_mod.DataModError(
                    f"field member {member_index} command {command_index} must be an object"
                )
            opcode = parse_opcode(
                command.get("opcode"),
                descriptors,
                names,
                f"field member {member_index} command {command_index}",
            )
            usage[opcode] += 1
    return {
        "schema": USAGE_SCHEMA,
        "version": document.get("version"),
        "instance": "field",
        "source": document.get("source"),
        "source_sha1": document.get("source_sha1"),
        "descriptor_count": len(descriptors),
        "member_count": len(members),
        "valid_script_count": valid_script_count,
        "private_target_count": private_target_count,
        "embedded_data_record_count": sum(embedded_data_types.values()),
        "embedded_data_record_types": dict(sorted(embedded_data_types.items())),
        "reachable_command_count": sum(usage.values()),
        "used_opcode_count": len(usage),
        "opcode_counts": {
            f"0x{opcode:03X}": usage[opcode] for opcode in sorted(usage)
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "command", choices=("export", "summarize", "check", "build")
    )
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
    parser.add_argument(
        "--summary-output",
        type=Path,
        default=ROOT / "config" / "eur" / "field_vm_usage.json",
    )
    args = parser.parse_args()
    descriptors, names = load_vm_schema(args.version)
    if args.command == "export":
        document = export_document(args.source, args.version, descriptors, names)
        write_sharded_document(document, args.document)
        print(
            f"Exported {len(document['members'])} field members in "
            f"{document['room_count']} room files below {args.document.parent}"
        )
        return 0
    document = load_document(args.document)
    if args.command == "summarize":
        summary = summarize_document(document, descriptors, names)
        data_mod.write_json(args.summary_output, summary)
        print(
            f"Summarized {summary['reachable_command_count']} field commands "
            f"to {args.summary_output}"
        )
        return 0
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
