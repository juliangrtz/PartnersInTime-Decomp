#!/usr/bin/env python3
"""Export and rebuild editable Partners in Time data files.

The tool deliberately covers only formats whose binary layout is understood
well enough to rebuild after size changes.  Unknown NitroFS files are copied
unchanged from the user's private extraction when a modded file tree is staged.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import struct
import sys
from pathlib import Path
from typing import Any, Iterable


PROJECT_SCHEMA = "pit-data-project-v1"
MFSET_SCHEMA = "pit-mfset-v1"
ENEMY_SCHEMA = "pit-enemy-stats-v1"
DIALOGUE_SCHEMA = "pit-localized-dialogue-v1"
TREASURE_SCHEMA = "pit-treasure-v1"
SHOP_SCHEMA = "pit-shop-stock-v1"
ITEM_MASTER_SCHEMA = "pit-item-master-v1"
LANGUAGES = ("japanese", "english", "french", "german", "italian", "spanish")

CONTROL_CODES = {
    0x00: "NEWLINE",
    0x0A: "END",
    0x0B: "TEXTBOX",
    0x0C: "PAUSE",
    0x11: "WAIT",
    0x20: "COLOR_DEFAULT",
    0x27: "COLOR_GREEN",
    0x2D: "COLOR_RED",
    0x35: "CENTER",
}
CONTROL_VALUES = {name: code for code, name in CONTROL_CODES.items()}
CONTROL_ARGUMENT_BYTES = {0x01: 1, 0x0B: 1, 0x0C: 1, 0x11: 1}
TOKEN_RE = re.compile(r"<\$([^>]+)>")

OV009_LOAD_ADDRESS = 0x0206AB80
ARM9_LOAD_ADDRESS = 0x02004000
SHOP_LAYOUT = (
    (0, 0x0207E3C4, 0x0207E88C, 199),
    (1, 0x0207E364, 0x0207E5F8, 143),
    (2, 0x0207E484, 0x0207E4E4, 137),
    (3, 0x0207E424, 0x0207E718, 186),
)
SHOP_CLASSES = (
    (0, 0x2000, "usable_items", "BData/mfset_UItmN.dat"),
    (1, 0x1000, "action_items", "BData/mfset_AItmN.dat"),
    (2, 0x4000, "wear", "BData/mfset_WearN.dat"),
    (3, 0x3000, "badges", "BData/mfset_BadgeN.dat"),
)
ITEM_MASTER_LAYOUT = (
    (0x2000, "usable_items", 0x02050044, 14, 20, "BData/mfset_UItmN.dat"),
    (0x1000, "action_items", 0x0205015C, 11, 28, "BData/mfset_AItmN.dat"),
    (0x3000, "badges", 0x02050290, 41, 20, "BData/mfset_BadgeN.dat"),
    (0x4000, "wear", 0x020505C4, 33, 28, "BData/mfset_WearN.dat"),
)


class DataModError(ValueError):
    """A user-facing validation or format error."""


def sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def read_json(path: Path) -> Any:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise DataModError(f"could not read JSON {path}: {exc}") from exc


def write_json(path: Path, value: Any) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        json.dumps(value, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
    )


def relative_posix(path: Path, root: Path) -> str:
    return path.relative_to(root).as_posix()


def parse_offset_archive(data: bytes) -> list[bytes]:
    """Parse a standard offset-only DAT without discarding empty entries."""
    if len(data) < 8:
        raise DataModError("offset archive is shorter than its minimum header")
    table_size = struct.unpack_from("<I", data)[0]
    if table_size < 8 or table_size > len(data) or table_size % 4:
        raise DataModError(f"invalid offset-table size 0x{table_size:X}")
    count = table_size // 4
    offsets = list(struct.unpack_from(f"<{count}I", data))
    if 0xFFFFFFFF in offsets:
        offsets = offsets[: offsets.index(0xFFFFFFFF)]
    if len(offsets) < 2 or offsets[0] != table_size or offsets[-1] != len(data):
        raise DataModError("offset archive does not span the complete file")
    if any(a > b or b > len(data) for a, b in zip(offsets, offsets[1:])):
        raise DataModError("offset archive contains decreasing/out-of-range offsets")
    return [data[offsets[i] : offsets[i + 1]] for i in range(len(offsets) - 1)]


def build_offset_archive(entries: Iterable[bytes]) -> bytes:
    entries = list(entries)
    table_size = (len(entries) + 1) * 4
    offsets = [table_size]
    for entry in entries:
        offsets.append(offsets[-1] + len(entry))
    return struct.pack(f"<{len(offsets)}I", *offsets) + b"".join(entries)


def language_encoding(language: str) -> str:
    # PiT's Japanese slot uses game-specific one-byte glyph indexes, not
    # Shift-JIS.  Keep those bytes explicit until the font map is documented.
    return "pit-japanese-byte-map" if language == "japanese" else "cp1252"


def _escape_plain_text(text: str) -> str:
    return text.replace("\\", "\\\\").replace("<", "\\<")


def _decode_plain(raw: bytes, encoding: str) -> str:
    if encoding == "pit-japanese-byte-map":
        return "".join(f"<$BYTE:{value:02X}>" for value in raw)
    decoded = raw.decode(encoding, errors="surrogateescape")
    result: list[str] = []
    plain: list[str] = []

    def flush() -> None:
        if plain:
            result.append(_escape_plain_text("".join(plain)))
            plain.clear()

    for character in decoded:
        value = ord(character)
        if 0xDC80 <= value <= 0xDCFF:
            flush()
            result.append(f"<$BYTE:{value - 0xDC00:02X}>")
        else:
            plain.append(character)
    flush()
    return "".join(result)


def decode_text(raw: bytes, language: str) -> str:
    """Decode text into an editable, byte-round-trippable token notation."""
    encoding = language_encoding(language)
    output: list[str] = []
    plain = bytearray()

    def flush() -> None:
        if plain:
            output.append(_decode_plain(bytes(plain), encoding))
            plain.clear()

    position = 0
    while position < len(raw):
        value = raw[position]
        if value == 0xFF and position + 1 < len(raw):
            flush()
            code = raw[position + 1]
            argument_count = CONTROL_ARGUMENT_BYTES.get(code, 0)
            end = position + 2 + argument_count
            if end > len(raw):
                output.append(f"<$BYTE:{value:02X}>")
                position += 1
                continue
            arguments = raw[position + 2 : end]
            if code == 0x00:
                output.append("\n")
            else:
                name = CONTROL_CODES.get(code, f"CTRL:{code:02X}")
                suffix = f":{arguments.hex().upper()}" if arguments else ""
                output.append(f"<${name}{suffix}>")
            position = end
        elif value < 0x20:
            flush()
            output.append(f"<$BYTE:{value:02X}>")
            position += 1
        else:
            plain.append(value)
            position += 1
    flush()
    return "".join(output)


def _parse_hex_byte(value: str, context: str) -> int:
    if not re.fullmatch(r"[0-9A-Fa-f]{2}", value):
        raise DataModError(f"{context} requires exactly one hexadecimal byte")
    return int(value, 16)


def _encode_token(token: str) -> bytes:
    parts = token.split(":")
    if parts[0] == "BYTE" and len(parts) == 2:
        return bytes((_parse_hex_byte(parts[1], "BYTE token"),))
    if parts[0] == "CTRL" and len(parts) in (2, 3):
        code = _parse_hex_byte(parts[1], "CTRL token")
        argument_hex = parts[2] if len(parts) == 3 else ""
    elif parts[0] in CONTROL_VALUES and parts[0] != "NEWLINE":
        code = CONTROL_VALUES[parts[0]]
        argument_hex = parts[1] if len(parts) == 2 else ""
        if len(parts) > 2:
            raise DataModError(f"invalid control token <${token}>")
    else:
        raise DataModError(f"unknown text token <${token}>")

    required = CONTROL_ARGUMENT_BYTES.get(code, 0)
    if len(argument_hex) != required * 2 or not re.fullmatch(
        r"[0-9A-Fa-f]*", argument_hex
    ):
        raise DataModError(
            f"control 0x{code:02X} requires {required} argument byte(s)"
        )
    return b"\xFF" + bytes((code,)) + bytes.fromhex(argument_hex)


def encode_text(text: str, language: str) -> bytes:
    encoding = language_encoding(language)
    output = bytearray()
    position = 0
    while position < len(text):
        character = text[position]
        if character == "\\":
            if position + 1 >= len(text) or text[position + 1] not in "\\<":
                raise DataModError(
                    "a backslash in editable text must escape another backslash or '<'"
                )
            if encoding == "pit-japanese-byte-map":
                raise DataModError(
                    "Japanese glyphs require <$BYTE:XX> until their font map is known"
                )
            output.extend(text[position + 1].encode(encoding, errors="strict"))
            position += 2
        elif character == "\n":
            output.extend(b"\xFF\x00")
            position += 1
        elif text.startswith("<$", position):
            match = TOKEN_RE.match(text, position)
            if match is None:
                raise DataModError(f"unterminated token at character {position}")
            output.extend(_encode_token(match.group(1)))
            position = match.end()
        else:
            if encoding == "pit-japanese-byte-map":
                raise DataModError(
                    "Japanese glyphs require <$BYTE:XX> until their font map is known"
                )
            try:
                output.extend(character.encode(encoding, errors="strict"))
            except UnicodeEncodeError as exc:
                raise DataModError(
                    f"character {character!r} cannot be encoded as {encoding}"
                ) from exc
            position += 1
    return bytes(output)


def parse_mfset_entry(
    data: bytes, language: str, header_size: int = 0
) -> list[dict[str, str]]:
    if len(data) < 4:
        raise DataModError("MFset language entry is empty")
    table_size = struct.unpack_from("<I", data)[0]
    if table_size < 4 or table_size > len(data) or table_size % 4:
        raise DataModError(f"invalid MFset table size 0x{table_size:X}")
    count = table_size // 4
    pointers = list(struct.unpack_from(f"<{count}I", data))
    if any(pointer < table_size or pointer > len(data) for pointer in pointers):
        raise DataModError("MFset pointer is outside its language entry")
    if any(a > b for a, b in zip(pointers, pointers[1:])):
        raise DataModError("MFset pointers are not ordered")
    if len(set(pointers)) != len(pointers):
        raise DataModError("shared MFset pointers are not editable in schema v1")

    strings: list[dict[str, str]] = []
    for index, start in enumerate(pointers):
        end = pointers[index + 1] if index + 1 < len(pointers) else len(data)
        segment = data[start:end]
        trailing_zeros = len(segment) - len(segment.rstrip(b"\0"))
        if index + 1 < len(pointers) and trailing_zeros != 1:
            raise DataModError(
                f"MFset string {index} does not have exactly one terminator"
            )
        if trailing_zeros < 1:
            raise DataModError(f"MFset string {index} has no null terminator")
        body = segment[:-trailing_zeros]
        if len(body) < header_size:
            raise DataModError(f"MFset string {index} is shorter than its header")
        row = {"text": decode_text(body[header_size:], language)}
        if header_size:
            row["header_hex"] = body[:header_size].hex(" ")
        strings.append(row)
    return strings


def build_mfset_entry(
    strings: list[dict[str, str]], language: str, header_size: int = 0
) -> bytes:
    if not strings:
        raise DataModError("an MFset language must contain at least one string")
    table_size = len(strings) * 4
    encoded = []
    for string_id, row in enumerate(strings):
        header_hex = row.get("header_hex", "")
        try:
            header = bytes.fromhex(header_hex)
        except ValueError as exc:
            raise DataModError(
                f"MFset string {string_id} header_hex is not hexadecimal"
            ) from exc
        if len(header) != header_size:
            raise DataModError(
                f"MFset string {string_id} header_hex must contain {header_size} bytes"
            )
        encoded.append(header + encode_text(row["text"], language) + b"\0")
    offsets: list[int] = []
    position = table_size
    for raw in encoded:
        offsets.append(position)
        position += len(raw)
    result = struct.pack(f"<{len(offsets)}I", *offsets) + b"".join(encoded)
    return result + bytes((-len(result)) % 4)


def mfset_language_entries(entry_count: int) -> list[tuple[int, str]]:
    if entry_count == len(LANGUAGES):
        return list(enumerate(LANGUAGES))
    # mfset_Mes_MenuAI_out.dat is a 90-entry wrapper: 0 is metadata,
    # 1..83 are empty, and 84..89 are the six localized MFsets.
    if entry_count == 90:
        return [(84 + index, language) for index, language in enumerate(LANGUAGES)]
    raise DataModError(
        f"unsupported MFset outer archive with {entry_count} entries"
    )


def mfset_header_size(entry_count: int) -> int:
    return 2 if entry_count == 90 else 0


def export_mfset(source_path: Path, relative_source: str) -> dict[str, Any]:
    source_data = source_path.read_bytes()
    entries = parse_offset_archive(source_data)
    languages = []
    header_size = mfset_header_size(len(entries))
    for archive_entry, language in mfset_language_entries(len(entries)):
        strings = parse_mfset_entry(entries[archive_entry], language, header_size)
        languages.append(
            {
                "archive_entry": archive_entry,
                "language": language,
                "encoding": language_encoding(language),
                "strings": [
                    {"id": string_id, **row}
                    for string_id, row in enumerate(strings)
                ],
            }
        )
    return {
        "schema": MFSET_SCHEMA,
        "source": relative_source,
        "source_sha1": sha1(source_data),
        "languages": languages,
    }


def _require_list(value: Any, context: str) -> list[Any]:
    if not isinstance(value, list):
        raise DataModError(f"{context} must be a JSON array")
    return value


def build_mfset(document: dict[str, Any], source_data: bytes) -> bytes:
    if document.get("schema") != MFSET_SCHEMA:
        raise DataModError(f"expected schema {MFSET_SCHEMA!r}")
    expected_sha1 = document.get("source_sha1")
    if expected_sha1 != sha1(source_data):
        raise DataModError(
            f"private source {document.get('source')} does not match source_sha1"
        )
    entries = parse_offset_archive(source_data)
    expected_entries = dict(mfset_language_entries(len(entries)))
    header_size = mfset_header_size(len(entries))
    seen_entries: set[int] = set()
    for language_row in _require_list(document.get("languages"), "languages"):
        if not isinstance(language_row, dict):
            raise DataModError("every languages item must be an object")
        archive_entry = language_row.get("archive_entry")
        language = language_row.get("language")
        if not isinstance(archive_entry, int) or archive_entry not in expected_entries:
            raise DataModError(f"invalid MFset archive entry {archive_entry!r}")
        if expected_entries[archive_entry] != language:
            raise DataModError(
                f"archive entry {archive_entry} must use {expected_entries[archive_entry]}"
            )
        if archive_entry in seen_entries:
            raise DataModError(f"duplicate MFset archive entry {archive_entry}")
        seen_entries.add(archive_entry)

        rows = _require_list(language_row.get("strings"), f"{language} strings")
        strings: list[dict[str, str]] = []
        for expected_id, row in enumerate(rows):
            if not isinstance(row, dict) or row.get("id") != expected_id:
                raise DataModError(
                    f"{language} strings must have contiguous IDs from zero"
                )
            text = row.get("text")
            if not isinstance(text, str):
                raise DataModError(f"{language} string {expected_id} text must be a string")
            string = {"text": text}
            if header_size:
                header_hex = row.get("header_hex")
                if not isinstance(header_hex, str):
                    raise DataModError(
                        f"{language} string {expected_id} header_hex must be a string"
                    )
                string["header_hex"] = header_hex
            strings.append(string)
        entries[archive_entry] = build_mfset_entry(strings, language, header_size)

    if seen_entries != set(expected_entries):
        missing = sorted(set(expected_entries) - seen_entries)
        raise DataModError(f"MFset document is missing archive entries {missing}")
    return build_offset_archive(entries)


def parse_localized_container(data: bytes) -> list[bytes]:
    """Split the 91-pointer event/message container without losing metadata."""
    table_size = 91 * 4
    if len(data) < table_size or struct.unpack_from("<I", data)[0] != table_size:
        raise DataModError("not a 91-pointer localized container")
    pointers = list(struct.unpack_from("<91I", data))
    if pointers[0] != table_size:
        raise DataModError("localized container payload does not follow its table")
    if any(pointer < table_size or pointer > len(data) for pointer in pointers):
        raise DataModError("localized container pointer is outside the entry")
    if any(a > b for a, b in zip(pointers, pointers[1:])):
        raise DataModError("localized container pointers are not ordered")
    return [
        data[pointers[index] : pointers[index + 1]]
        if index + 1 < len(pointers)
        else data[pointers[index] :]
        for index in range(len(pointers))
    ]


def build_localized_container(segments: list[bytes]) -> bytes:
    if len(segments) != 91:
        raise DataModError("localized container must retain all 91 segments")
    position = 91 * 4
    pointers = []
    for segment in segments:
        pointers.append(position)
        position += len(segment)
    return struct.pack("<91I", *pointers) + b"".join(segments)


def parse_dialogue_chunk(
    data: bytes, language: str
) -> tuple[str, list[dict[str, str]]]:
    try:
        return "mfset", parse_mfset_entry(data, language, header_size=2)
    except DataModError:
        pass

    if len(data) < 12:
        raise DataModError("dialogue chunk is neither MFset nor FEv message format")
    text_end = 4 + struct.unpack_from("<I", data)[0]
    if text_end < 8 or text_end > len(data):
        raise DataModError("FEv message text-end marker is outside the chunk")
    inner = data[4:text_end]
    rows = parse_mfset_entry(inner, language, header_size=2)
    label_rows = parse_mfset_entry(data[text_end:], "english")
    if len(label_rows) != len(rows):
        raise DataModError("FEv message and event-label counts do not match")
    for row, label_row in zip(rows, label_rows):
        row["event_label"] = label_row["text"]
    return "fevent-mfset", rows


def build_dialogue_chunk(
    rows: list[dict[str, str]], language: str, chunk_format: str, source_data: bytes
) -> bytes:
    if chunk_format == "mfset":
        # Parsing the source again verifies that the editable format label was
        # not changed independently of the private binary.
        source_format, _ = parse_dialogue_chunk(source_data, language)
        if source_format != chunk_format:
            raise DataModError(f"dialogue format changed from {source_format}")
        return build_mfset_entry(rows, language, header_size=2)
    if chunk_format == "fevent-mfset":
        source_format, _ = parse_dialogue_chunk(source_data, language)
        if source_format != chunk_format:
            raise DataModError(f"dialogue format changed from {source_format}")
        inner = build_mfset_entry(rows, language, header_size=2)
        labels = []
        for string_id, row in enumerate(rows):
            event_label = row.get("event_label")
            if not isinstance(event_label, str):
                raise DataModError(
                    f"FEv message {string_id} event_label must be a string"
                )
            labels.append({"text": event_label})
        metadata = build_mfset_entry(labels, "english")
        return struct.pack("<I", len(inner)) + inner + metadata
    raise DataModError(f"unsupported dialogue chunk format {chunk_format!r}")


def discover_dialogue_containers(
    entries: list[bytes], relative_source: str
) -> dict[int, tuple[list[bytes], dict[int, tuple[str, list[dict[str, str]]]]]]:
    discovered = {}
    for archive_entry, entry in enumerate(entries):
        try:
            segments = parse_localized_container(entry)
        except DataModError:
            continue
        languages = {}
        for language_id, language in enumerate(LANGUAGES):
            slot = 84 + language_id
            if not segments[slot]:
                continue
            try:
                languages[slot] = parse_dialogue_chunk(segments[slot], language)
            except DataModError as exc:
                raise DataModError(
                    f"{relative_source} entry {archive_entry} slot {slot}: {exc}"
                ) from exc
        if languages:
            discovered[archive_entry] = (segments, languages)
    return discovered


def export_dialogue(source_path: Path, relative_source: str) -> dict[str, Any]:
    source_data = source_path.read_bytes()
    entries = parse_offset_archive(source_data)
    discovered = discover_dialogue_containers(entries, relative_source)
    kind = "battle" if relative_source == "BAI/BMes.dat" else "field"
    containers = []
    for archive_entry, (_, language_chunks) in discovered.items():
        container: dict[str, Any] = {
            "archive_entry": archive_entry,
            "languages": [],
        }
        if kind == "battle":
            container["battle_group"] = archive_entry
        else:
            container["room_id"] = archive_entry // 3
            container["room_part"] = archive_entry % 3
        for slot, (chunk_format, rows) in language_chunks.items():
            language = LANGUAGES[slot - 84]
            container["languages"].append(
                {
                    "archive_slot": slot,
                    "language": language,
                    "encoding": language_encoding(language),
                    "chunk_format": chunk_format,
                    "strings": [
                        {"id": string_id, **row}
                        for string_id, row in enumerate(rows)
                    ],
                }
            )
        containers.append(container)
    return {
        "schema": DIALOGUE_SCHEMA,
        "kind": kind,
        "source": relative_source,
        "source_sha1": sha1(source_data),
        "containers": containers,
    }


def build_dialogue(document: dict[str, Any], source_data: bytes) -> bytes:
    if document.get("schema") != DIALOGUE_SCHEMA:
        raise DataModError(f"expected schema {DIALOGUE_SCHEMA!r}")
    source = document.get("source")
    if document.get("source_sha1") != sha1(source_data):
        raise DataModError(f"private source {source} does not match source_sha1")
    entries = parse_offset_archive(source_data)
    discovered = discover_dialogue_containers(entries, str(source))
    seen_containers: set[int] = set()

    for container in _require_list(document.get("containers"), "containers"):
        if not isinstance(container, dict):
            raise DataModError("every containers item must be an object")
        archive_entry = container.get("archive_entry")
        if not isinstance(archive_entry, int) or archive_entry not in discovered:
            raise DataModError(f"invalid dialogue archive entry {archive_entry!r}")
        if archive_entry in seen_containers:
            raise DataModError(f"duplicate dialogue archive entry {archive_entry}")
        seen_containers.add(archive_entry)
        segments, expected_languages = discovered[archive_entry]
        seen_slots: set[int] = set()

        for language_row in _require_list(
            container.get("languages"), f"entry {archive_entry} languages"
        ):
            if not isinstance(language_row, dict):
                raise DataModError("every dialogue language must be an object")
            slot = language_row.get("archive_slot")
            if not isinstance(slot, int) or slot not in expected_languages:
                raise DataModError(
                    f"entry {archive_entry} has invalid language slot {slot!r}"
                )
            language = LANGUAGES[slot - 84]
            if language_row.get("language") != language:
                raise DataModError(f"archive slot {slot} must use {language}")
            if slot in seen_slots:
                raise DataModError(
                    f"entry {archive_entry} repeats language slot {slot}"
                )
            seen_slots.add(slot)
            expected_format, _ = expected_languages[slot]
            chunk_format = language_row.get("chunk_format")
            if chunk_format != expected_format:
                raise DataModError(
                    f"entry {archive_entry} slot {slot} must use {expected_format}"
                )

            strings = []
            for string_id, row in enumerate(
                _require_list(language_row.get("strings"), "dialogue strings")
            ):
                if not isinstance(row, dict) or row.get("id") != string_id:
                    raise DataModError(
                        f"entry {archive_entry} {language} strings need contiguous IDs"
                    )
                text = row.get("text")
                header_hex = row.get("header_hex")
                if not isinstance(text, str) or not isinstance(header_hex, str):
                    raise DataModError(
                        f"entry {archive_entry} {language} string {string_id} needs text/header_hex"
                    )
                strings.append({"text": text, "header_hex": header_hex})
                if chunk_format == "fevent-mfset":
                    event_label = row.get("event_label")
                    if not isinstance(event_label, str):
                        raise DataModError(
                            f"entry {archive_entry} {language} string {string_id} needs event_label"
                        )
                    strings[-1]["event_label"] = event_label
            segments[slot] = build_dialogue_chunk(
                strings, language, chunk_format, segments[slot]
            )

        if seen_slots != set(expected_languages):
            missing = sorted(set(expected_languages) - seen_slots)
            raise DataModError(
                f"dialogue entry {archive_entry} is missing language slots {missing}"
            )
        entries[archive_entry] = build_localized_container(segments)

    if seen_containers != set(discovered):
        missing = sorted(set(discovered) - seen_containers)
        raise DataModError(f"dialogue document is missing archive entries {missing}")
    return build_offset_archive(entries)


ENEMY_STRUCT = struct.Struct("<HHBBHHHHHH14sHHII")
TREASURE_STRUCT = struct.Struct("<BBHHHHH")


def parse_integer(value: Any, bits: int, context: str) -> int:
    if isinstance(value, bool):
        raise DataModError(f"{context} must be an integer, not a boolean")
    if isinstance(value, int):
        result = value
    elif isinstance(value, str):
        try:
            result = int(value, 0)
        except ValueError as exc:
            raise DataModError(f"{context} is not a valid integer") from exc
    else:
        raise DataModError(f"{context} must be an integer or 0x-prefixed string")
    if not 0 <= result < 1 << bits:
        raise DataModError(f"{context} must fit in an unsigned {bits}-bit field")
    return result


def english_enemy_names(files_root: Path) -> list[str]:
    archive = parse_offset_archive((files_root / "BData/mfset_MonN.dat").read_bytes())
    strings = parse_mfset_entry(archive[1], "english")
    return [re.sub(r"<\$[^>]+>", "", row["text"]).strip() for row in strings]


def export_enemy_stats(files_root: Path) -> dict[str, Any]:
    source = "BData/BDataMon.dat"
    raw = (files_root / source).read_bytes()
    if len(raw) % ENEMY_STRUCT.size:
        raise DataModError(
            f"{source} size is not a multiple of {ENEMY_STRUCT.size} bytes"
        )
    names = english_enemy_names(files_root)
    records = []
    for record_id, values in enumerate(ENEMY_STRUCT.iter_unpack(raw)):
        name_id = values[0]
        records.append(
            {
                "record_id": record_id,
                "name_id": name_id,
                "name_hint": names[name_id] if name_id < len(names) else "",
                "flags_or_ai_id": f"0x{values[1]:04X}",
                "unknown_04": f"0x{values[2]:02X}",
                "level": values[3],
                "max_hp": values[4],
                "power": values[5],
                "defense": values[6],
                "speed": values[7],
                "traits": f"0x{values[8]:04X}",
                "unknown_10": f"0x{values[9]:04X}",
                "unknown_12_hex": values[10].hex(" "),
                "experience": values[11],
                "coins": values[12],
                "item_drop_1": f"0x{values[13]:08X}",
                "item_drop_2": f"0x{values[14]:08X}",
            }
        )
    return {
        "schema": ENEMY_SCHEMA,
        "source": source,
        "source_sha1": sha1(raw),
        "record_size": ENEMY_STRUCT.size,
        "records": records,
    }


def build_enemy_stats(document: dict[str, Any], source_data: bytes) -> bytes:
    if document.get("schema") != ENEMY_SCHEMA:
        raise DataModError(f"expected schema {ENEMY_SCHEMA!r}")
    if document.get("source_sha1") != sha1(source_data):
        raise DataModError(
            f"private source {document.get('source')} does not match source_sha1"
        )
    rows = _require_list(document.get("records"), "records")
    if len(rows) * ENEMY_STRUCT.size != len(source_data):
        raise DataModError(
            f"enemy table must retain its original {len(source_data) // ENEMY_STRUCT.size} records"
        )
    result = bytearray()
    fields = (
        ("name_id", 16),
        ("flags_or_ai_id", 16),
        ("unknown_04", 8),
        ("level", 8),
        ("max_hp", 16),
        ("power", 16),
        ("defense", 16),
        ("speed", 16),
        ("traits", 16),
        ("unknown_10", 16),
    )
    for record_id, row in enumerate(rows):
        if not isinstance(row, dict) or row.get("record_id") != record_id:
            raise DataModError("enemy records must have contiguous record_id values")
        values = [
            parse_integer(row.get(name), bits, f"enemy {record_id} {name}")
            for name, bits in fields
        ]
        unknown_hex = row.get("unknown_12_hex")
        if not isinstance(unknown_hex, str):
            raise DataModError(f"enemy {record_id} unknown_12_hex must be a string")
        try:
            unknown = bytes.fromhex(unknown_hex)
        except ValueError as exc:
            raise DataModError(
                f"enemy {record_id} unknown_12_hex is not hexadecimal"
            ) from exc
        if len(unknown) != 14:
            raise DataModError(f"enemy {record_id} unknown_12_hex must contain 14 bytes")
        values.append(unknown)
        values.extend(
            parse_integer(row.get(name), bits, f"enemy {record_id} {name}")
            for name, bits in (
                ("experience", 16),
                ("coins", 16),
                ("item_drop_1", 32),
                ("item_drop_2", 32),
            )
        )
        result.extend(ENEMY_STRUCT.pack(*values))
    return bytes(result)


def export_treasure(files_root: Path) -> dict[str, Any]:
    source = "Treasure/TreasureInfo.dat"
    raw = (files_root / source).read_bytes()
    entries = parse_offset_archive(raw)
    files = []
    for file_id, entry in enumerate(entries):
        if len(entry) % TREASURE_STRUCT.size:
            raise DataModError(
                f"{source} entry {file_id} is not a whole number of treasure records"
            )
        records = []
        for record_id, values in enumerate(TREASURE_STRUCT.iter_unpack(entry)):
            records.append(
                {
                    "record_id": record_id,
                    "type": values[0],
                    "subtype": values[1],
                    "contents": values[2],
                    "id": values[3],
                    "x": values[4],
                    "y": values[5],
                    "z": values[6],
                }
            )
        files.append({"file_id": file_id, "records": records})
    return {
        "schema": TREASURE_SCHEMA,
        "source": source,
        "source_sha1": sha1(raw),
        "record_size": TREASURE_STRUCT.size,
        "files": files,
    }


def build_treasure(document: dict[str, Any], source_data: bytes) -> bytes:
    if document.get("schema") != TREASURE_SCHEMA:
        raise DataModError(f"expected schema {TREASURE_SCHEMA!r}")
    source = document.get("source")
    if document.get("source_sha1") != sha1(source_data):
        raise DataModError(f"private source {source} does not match source_sha1")
    source_entries = parse_offset_archive(source_data)
    files = _require_list(document.get("files"), "treasure files")
    if len(files) != len(source_entries):
        raise DataModError(
            f"treasure table must retain its original {len(source_entries)} file entries"
        )

    rebuilt_entries = []
    for file_id, (file_row, source_entry) in enumerate(zip(files, source_entries)):
        if not isinstance(file_row, dict) or file_row.get("file_id") != file_id:
            raise DataModError("treasure files must have contiguous file_id values")
        rows = _require_list(file_row.get("records"), f"treasure file {file_id} records")
        expected_count = len(source_entry) // TREASURE_STRUCT.size
        if len(rows) != expected_count:
            raise DataModError(
                f"treasure file {file_id} must retain its {expected_count} records"
            )
        entry = bytearray()
        for record_id, row in enumerate(rows):
            if not isinstance(row, dict) or row.get("record_id") != record_id:
                raise DataModError(
                    f"treasure file {file_id} needs contiguous record_id values"
                )
            entry.extend(
                TREASURE_STRUCT.pack(
                    *(
                        parse_integer(
                            row.get(name),
                            bits,
                            f"treasure {file_id}:{record_id} {name}",
                        )
                        for name, bits in (
                            ("type", 8),
                            ("subtype", 8),
                            ("contents", 16),
                            ("id", 16),
                            ("x", 16),
                            ("y", 16),
                            ("z", 16),
                        )
                    )
                )
            )
        rebuilt_entries.append(bytes(entry))
    return build_offset_archive(rebuilt_entries)


def item_name_tables(files_root: Path) -> dict[int, list[str]]:
    result = {}
    for _, tag, _, relative_source in SHOP_CLASSES:
        entries = parse_offset_archive((files_root / relative_source).read_bytes())
        rows = parse_mfset_entry(entries[1], "english")
        # Each item has singular, plural, and inventory-full strings.  The
        # singular form is the useful source annotation for stock editing.
        result[tag] = [
            re.sub(r"<\$[^>]+>", "", rows[index]["text"]).strip()
            for index in range(0, len(rows), 3)
        ]
    return result


def _shop_region(binary: bytes, address: int, size: int) -> bytes:
    offset = address - OV009_LOAD_ADDRESS
    if offset < 0 or offset + size > len(binary):
        raise DataModError(f"shop region at 0x{address:08X} is outside overlay 9")
    return binary[offset : offset + size]


def export_shop_stock(files_root: Path, overlay: bytes) -> dict[str, Any]:
    names = item_name_tables(files_root)
    shops = []
    for shop_id, descriptor_address, pool_address, pool_count in SHOP_LAYOUT:
        descriptor_data = _shop_region(overlay, descriptor_address, 24 * 4)
        descriptors = struct.unpack("<24I", descriptor_data)
        greatest_end = max(
            (descriptor & 0xFFFF) + (descriptor >> 16)
            for descriptor in descriptors
        )
        if greatest_end != pool_count:
            raise DataModError(
                f"shop {shop_id} descriptors use {greatest_end}, expected {pool_count} items"
            )
        pool_data = _shop_region(overlay, pool_address, pool_count * 2)
        pool = struct.unpack(f"<{pool_count}H", pool_data)
        classes = []
        for class_id, expected_tag, class_name, _ in SHOP_CLASSES:
            tiers = []
            for tier in range(6):
                descriptor = descriptors[class_id * 6 + tier]
                start = descriptor & 0xFFFF
                count = descriptor >> 16
                items = []
                for item_id in pool[start : start + count]:
                    tag = item_id & 0xF000
                    index = item_id & 0x0FFF
                    if tag != expected_tag:
                        raise DataModError(
                            f"shop {shop_id} class {class_id} contains item 0x{item_id:04X}"
                        )
                    name_rows = names.get(tag, [])
                    items.append(
                        {
                            "item_id": f"0x{item_id:04X}",
                            "name_hint": name_rows[index]
                            if index < len(name_rows)
                            else "",
                        }
                    )
                tiers.append(
                    {
                        "tier": tier,
                        "pool_start": start,
                        "items": items,
                    }
                )
            classes.append(
                {
                    "class_id": class_id,
                    "class": class_name,
                    "item_tag": f"0x{expected_tag:04X}",
                    "tiers": tiers,
                }
            )
        shops.append(
            {
                "shop_id": shop_id,
                "descriptor_address": f"0x{descriptor_address:08X}",
                "item_pool_address": f"0x{pool_address:08X}",
                "descriptor_sha1": sha1(descriptor_data),
                "item_pool_sha1": sha1(pool_data),
                "classes": classes,
            }
        )
    return {
        "schema": SHOP_SCHEMA,
        "binary": "arm9_overlay_9",
        "load_address": f"0x{OV009_LOAD_ADDRESS:08X}",
        "source_sha1": sha1(overlay),
        "shops": shops,
    }


def build_shop_stock(document: dict[str, Any], overlay: bytes) -> bytes:
    if document.get("schema") != SHOP_SCHEMA:
        raise DataModError(f"expected schema {SHOP_SCHEMA!r}")
    if document.get("binary") != "arm9_overlay_9":
        raise DataModError("shop stock must target arm9_overlay_9")
    shops = _require_list(document.get("shops"), "shops")
    if len(shops) != len(SHOP_LAYOUT):
        raise DataModError(f"shop stock must retain all {len(SHOP_LAYOUT)} shops")
    rebuilt = bytearray(overlay)

    for expected_layout, shop in zip(SHOP_LAYOUT, shops):
        shop_id, descriptor_address, pool_address, pool_count = expected_layout
        if not isinstance(shop, dict) or shop.get("shop_id") != shop_id:
            raise DataModError("shops must remain in contiguous shop_id order")
        descriptor_data = _shop_region(overlay, descriptor_address, 24 * 4)
        pool_data = _shop_region(overlay, pool_address, pool_count * 2)
        if shop.get("descriptor_sha1") != sha1(descriptor_data):
            raise DataModError(f"shop {shop_id} descriptor source bytes changed")
        if shop.get("item_pool_sha1") != sha1(pool_data):
            raise DataModError(f"shop {shop_id} item-pool source bytes changed")
        descriptors = struct.unpack("<24I", descriptor_data)
        classes = _require_list(shop.get("classes"), f"shop {shop_id} classes")
        if len(classes) != len(SHOP_CLASSES):
            raise DataModError(f"shop {shop_id} must retain four item classes")
        new_pool = bytearray(pool_data)

        for class_layout, class_row in zip(SHOP_CLASSES, classes):
            class_id, expected_tag, class_name, _ = class_layout
            if not isinstance(class_row, dict) or class_row.get("class_id") != class_id:
                raise DataModError(
                    f"shop {shop_id} classes must remain in class_id order"
                )
            if class_row.get("class") != class_name:
                raise DataModError(
                    f"shop {shop_id} class {class_id} must be named {class_name}"
                )
            if parse_integer(
                class_row.get("item_tag"), 16, f"shop {shop_id} class {class_id} tag"
            ) != expected_tag:
                raise DataModError(f"shop {shop_id} class {class_id} tag changed")
            tiers = _require_list(
                class_row.get("tiers"), f"shop {shop_id} class {class_id} tiers"
            )
            if len(tiers) != 6:
                raise DataModError(
                    f"shop {shop_id} class {class_id} must retain six tiers"
                )
            for tier, tier_row in enumerate(tiers):
                if not isinstance(tier_row, dict) or tier_row.get("tier") != tier:
                    raise DataModError(
                        f"shop {shop_id} class {class_id} tiers must remain ordered"
                    )
                descriptor = descriptors[class_id * 6 + tier]
                start = descriptor & 0xFFFF
                count = descriptor >> 16
                if tier_row.get("pool_start") != start:
                    raise DataModError(
                        f"shop {shop_id} class {class_id} tier {tier} pool_start changed"
                    )
                items = _require_list(
                    tier_row.get("items"),
                    f"shop {shop_id} class {class_id} tier {tier} items",
                )
                if len(items) != count:
                    raise DataModError(
                        f"shop {shop_id} class {class_id} tier {tier} needs {count} items"
                    )
                for index, item in enumerate(items):
                    if not isinstance(item, dict):
                        raise DataModError("shop item must be an object")
                    item_id = parse_integer(
                        item.get("item_id"),
                        16,
                        f"shop {shop_id} class {class_id} tier {tier} item {index}",
                    )
                    if item_id & 0xF000 != expected_tag:
                        raise DataModError(
                            f"shop {shop_id} class {class_id} requires tag 0x{expected_tag:04X}"
                        )
                    struct.pack_into("<H", new_pool, 2 * (start + index), item_id)

        pool_offset = pool_address - OV009_LOAD_ADDRESS
        rebuilt[pool_offset : pool_offset + len(new_pool)] = new_pool
    return bytes(rebuilt)


def export_item_master(files_root: Path, arm9: bytes) -> dict[str, Any]:
    names = item_name_tables(files_root)
    categories = []
    for tag, category_name, address, count, stride, _ in ITEM_MASTER_LAYOUT:
        offset = address - ARM9_LOAD_ADDRESS
        size = count * stride
        if offset < 0 or offset + size > len(arm9):
            raise DataModError(
                f"item master {category_name} at 0x{address:08X} is outside ARM9"
            )
        table = arm9[offset : offset + size]
        records = []
        for index in range(count):
            record = table[index * stride : (index + 1) * stride]
            words = struct.unpack_from("<6H", record)
            records.append(
                {
                    "index": index,
                    "item_id": f"0x{tag | index:04X}",
                    "name_hint": names[tag][index] if index < len(names[tag]) else "",
                    "unknown_words_00_0A": [f"0x{word:04X}" for word in words],
                    "price": struct.unpack_from("<H", record, 0x0C)[0],
                    "unknown_0E_hex": record[0x0E:].hex(" "),
                }
            )
        categories.append(
            {
                "class": category_name,
                "item_tag": f"0x{tag:04X}",
                "runtime_address": f"0x{address:08X}",
                "record_count": count,
                "record_size": stride,
                "source_region_sha1": sha1(table),
                "records": records,
            }
        )
    return {
        "schema": ITEM_MASTER_SCHEMA,
        "binary": "arm9_main",
        "load_address": f"0x{ARM9_LOAD_ADDRESS:08X}",
        "source_sha1": sha1(arm9),
        "categories": categories,
    }


def build_item_master(document: dict[str, Any], arm9: bytes) -> bytes:
    if document.get("schema") != ITEM_MASTER_SCHEMA:
        raise DataModError(f"expected schema {ITEM_MASTER_SCHEMA!r}")
    if document.get("binary") != "arm9_main":
        raise DataModError("item master must target arm9_main")
    categories = _require_list(document.get("categories"), "item categories")
    if len(categories) != len(ITEM_MASTER_LAYOUT):
        raise DataModError("item master must retain all four categories")
    rebuilt = bytearray(arm9)

    for layout, category in zip(ITEM_MASTER_LAYOUT, categories):
        tag, category_name, address, count, stride, _ = layout
        if not isinstance(category, dict) or category.get("class") != category_name:
            raise DataModError(f"item category must remain {category_name}")
        if parse_integer(
            category.get("item_tag"), 16, f"{category_name} item_tag"
        ) != tag:
            raise DataModError(f"{category_name} item tag changed")
        if category.get("record_count") != count or category.get("record_size") != stride:
            raise DataModError(f"{category_name} table shape changed")
        offset = address - ARM9_LOAD_ADDRESS
        source_table = arm9[offset : offset + count * stride]
        if category.get("source_region_sha1") != sha1(source_table):
            raise DataModError(f"{category_name} ARM9 source bytes changed")
        rows = _require_list(category.get("records"), f"{category_name} records")
        if len(rows) != count:
            raise DataModError(f"{category_name} must retain {count} records")

        for index, row in enumerate(rows):
            if not isinstance(row, dict) or row.get("index") != index:
                raise DataModError(f"{category_name} records must remain ordered")
            item_id = parse_integer(
                row.get("item_id"), 16, f"{category_name} record {index} item_id"
            )
            if item_id != tag | index:
                raise DataModError(
                    f"{category_name} record {index} must keep item ID 0x{tag | index:04X}"
                )
            words = _require_list(
                row.get("unknown_words_00_0A"),
                f"{category_name} record {index} unknown_words_00_0A",
            )
            if len(words) != 6:
                raise DataModError(
                    f"{category_name} record {index} needs six unknown leading words"
                )
            record = bytearray(
                struct.pack(
                    "<6H",
                    *(
                        parse_integer(
                            value,
                            16,
                            f"{category_name} record {index} unknown word {word_index}",
                        )
                        for word_index, value in enumerate(words)
                    ),
                )
            )
            record.extend(
                struct.pack(
                    "<H",
                    parse_integer(
                        row.get("price"), 16, f"{category_name} record {index} price"
                    ),
                )
            )
            tail_hex = row.get("unknown_0E_hex")
            if not isinstance(tail_hex, str):
                raise DataModError(
                    f"{category_name} record {index} unknown_0E_hex must be a string"
                )
            try:
                tail = bytes.fromhex(tail_hex)
            except ValueError as exc:
                raise DataModError(
                    f"{category_name} record {index} unknown_0E_hex is not hexadecimal"
                ) from exc
            if len(tail) != stride - 0x0E:
                raise DataModError(
                    f"{category_name} record {index} needs {stride - 0x0E} tail bytes"
                )
            record.extend(tail)
            start = offset + index * stride
            rebuilt[start : start + stride] = record
    return bytes(rebuilt)


def discover_mfsets(files_root: Path) -> list[Path]:
    return sorted(
        path
        for path in files_root.rglob("*.dat")
        if path.name.lower().startswith("mfset")
    )


def command_export(args: argparse.Namespace) -> None:
    files_root = args.files_root.resolve()
    project_root = args.project_root.resolve()
    if not files_root.is_dir():
        raise DataModError(f"NitroFS directory does not exist: {files_root}")

    text_documents = []
    for source_path in discover_mfsets(files_root):
        relative_source = relative_posix(source_path, files_root)
        document = export_mfset(source_path, relative_source)
        output_name = relative_source.replace("/", "__") + ".json"
        output_relative = f"text/{output_name}"
        write_json(project_root / output_relative, document)
        text_documents.append(output_relative)

    enemy_relative = "stats/enemies.json"
    write_json(project_root / enemy_relative, export_enemy_stats(files_root))
    treasure_relative = "stats/treasure.json"
    write_json(project_root / treasure_relative, export_treasure(files_root))
    overlay_9_path = files_root.parent / "arm9_overlays" / "ov009.bin"
    if not overlay_9_path.is_file():
        raise DataModError(f"private overlay 9 is missing: {overlay_9_path}")
    shop_relative = "shops/stock.json"
    write_json(
        project_root / shop_relative,
        export_shop_stock(files_root, overlay_9_path.read_bytes()),
    )
    arm9_path = files_root.parent / "arm9" / "arm9.bin"
    if not arm9_path.is_file():
        raise DataModError(f"private ARM9 is missing: {arm9_path}")
    item_master_relative = "items/master.json"
    write_json(
        project_root / item_master_relative,
        export_item_master(files_root, arm9_path.read_bytes()),
    )
    dialogue_documents = []
    for relative_source in ("BAI/BMes.dat", "FEvent/FEvData.dat"):
        source_path = files_root / relative_source
        if not source_path.is_file():
            raise DataModError(f"dialogue source file is missing: {source_path}")
        output_relative = f"text/{relative_source.replace('/', '__')}.json"
        write_json(
            project_root / output_relative,
            export_dialogue(source_path, relative_source),
        )
        dialogue_documents.append(output_relative)
    write_json(
        project_root / "project.json",
        {
            "schema": PROJECT_SCHEMA,
            "version": args.version,
            "text_documents": text_documents,
            "dialogue_documents": dialogue_documents,
            "stat_documents": [enemy_relative, treasure_relative],
            "binary_documents": [shop_relative, item_master_relative],
        },
    )
    print(
        f"Exported {len(text_documents)} MFsets, {len(dialogue_documents)} dialogue archives, "
        f"an enemy table, a treasure table, shop stock, and item masters to {project_root}"
    )


def _validated_source(project_root: Path, document: dict[str, Any]) -> str:
    source = document.get("source")
    if not isinstance(source, str) or not source or "\\" in source:
        raise DataModError("document source must be a non-empty POSIX relative path")
    source_path = Path(source)
    if source_path.is_absolute() or ".." in source_path.parts:
        raise DataModError(f"document source escapes the NitroFS root: {source}")
    return source


def compile_project(
    files_root: Path, project_root: Path
) -> tuple[dict[str, bytes], list[dict[str, Any]]]:
    project = read_json(project_root / "project.json")
    if not isinstance(project, dict) or project.get("schema") != PROJECT_SCHEMA:
        raise DataModError(f"project.json must use schema {PROJECT_SCHEMA!r}")
    replacements: dict[str, bytes] = {}
    report = []

    document_groups = (
        ("text_documents", {MFSET_SCHEMA}),
        ("dialogue_documents", {DIALOGUE_SCHEMA}),
        ("stat_documents", {ENEMY_SCHEMA, TREASURE_SCHEMA}),
    )
    for key, expected_schemas in document_groups:
        for relative_document in _require_list(project.get(key), key):
            if not isinstance(relative_document, str):
                raise DataModError(f"every {key} item must be a path string")
            document = read_json(project_root / relative_document)
            if not isinstance(document, dict) or document.get("schema") not in expected_schemas:
                raise DataModError(
                    f"{relative_document} must use one of {sorted(expected_schemas)!r}"
                )
            source = _validated_source(project_root, document)
            if source in replacements:
                raise DataModError(f"more than one document rebuilds {source}")
            source_path = files_root / Path(source)
            if not source_path.is_file():
                raise DataModError(f"private source file is missing: {source_path}")
            source_data = source_path.read_bytes()
            schema = document["schema"]
            if schema == MFSET_SCHEMA:
                rebuilt = build_mfset(document, source_data)
            elif schema == DIALOGUE_SCHEMA:
                rebuilt = build_dialogue(document, source_data)
            elif schema == ENEMY_SCHEMA:
                rebuilt = build_enemy_stats(document, source_data)
            else:
                rebuilt = build_treasure(document, source_data)
            replacements[source] = rebuilt
            report.append(
                {
                    "source": source,
                    "original_size": len(source_data),
                    "rebuilt_size": len(rebuilt),
                    "original_sha1": sha1(source_data),
                    "rebuilt_sha1": sha1(rebuilt),
                    "changed": rebuilt != source_data,
                }
            )
    return replacements, report


def compile_binary_project(
    project_root: Path, binaries: dict[str, bytes]
) -> tuple[dict[str, bytes], list[dict[str, Any]]]:
    project = read_json(project_root / "project.json")
    documents = _require_list(project.get("binary_documents"), "binary_documents")
    rebuilt_binaries = dict(binaries)
    report = []
    seen_targets: set[str] = set()
    for relative_document in documents:
        if not isinstance(relative_document, str):
            raise DataModError("every binary_documents item must be a path string")
        document = read_json(project_root / relative_document)
        if not isinstance(document, dict) or document.get("schema") not in {
            SHOP_SCHEMA,
            ITEM_MASTER_SCHEMA,
        }:
            raise DataModError(
                f"{relative_document} has an unsupported binary-data schema"
            )
        target = document.get("binary")
        if not isinstance(target, str) or target not in rebuilt_binaries:
            raise DataModError(f"binary input is missing for {target!r}")
        if target in seen_targets:
            raise DataModError(f"more than one document rebuilds binary {target}")
        seen_targets.add(target)
        original = rebuilt_binaries[target]
        if document["schema"] == SHOP_SCHEMA:
            rebuilt = build_shop_stock(document, original)
        else:
            rebuilt = build_item_master(document, original)
        rebuilt_binaries[target] = rebuilt
        report.append(
            {
                "source": target,
                "document": relative_document,
                "original_size": len(original),
                "rebuilt_size": len(rebuilt),
                "original_sha1": sha1(original),
                "rebuilt_sha1": sha1(rebuilt),
                "changed": rebuilt != original,
            }
        )
    return rebuilt_binaries, report


def write_modded_rom_config(
    input_path: Path,
    output_path: Path,
    staged_files: Path,
    output_code: Path | None = None,
    patched_binaries: set[str] | None = None,
) -> None:
    config = input_path.read_text(encoding="utf-8")
    relative = os.path.relpath(staged_files, output_path.parent).replace("\\", "/")
    replacement = f"files_dir: '{relative}'"
    config, count = re.subn(r"(?m)^files_dir:\s*.*$", replacement, config)
    if count != 1:
        raise DataModError(f"expected exactly one files_dir in {input_path}")
    patched_binaries = patched_binaries or set()
    if "arm9_overlay_9" in patched_binaries:
        if output_code is None:
            raise DataModError("output code directory is required for overlay patches")
        overlay_match = re.search(r"(?m)^arm9_overlays:\s*'?([^'\r\n]+)'?\s*$", config)
        if overlay_match is None:
            raise DataModError(f"could not locate arm9_overlays in {input_path}")
        input_overlay_config = input_path.parent / overlay_match.group(1)
        overlays = input_overlay_config.read_text(encoding="utf-8")
        overlay_binary_relative = os.path.relpath(
            output_code / "arm9_ov009.bin", output_path.parent
        ).replace("\\", "/")
        overlays, overlay_count = re.subn(
            r"(?m)^  file_name: arm9_ov009\.bin$",
            f"  file_name: '{overlay_binary_relative}'",
            overlays,
        )
        if overlay_count != 1:
            raise DataModError(
                f"expected one overlay-9 file_name in {input_overlay_config}"
            )
        output_overlay_config = output_path.parent / "arm9_overlays_data_mod.yaml"
        output_overlay_config.write_text(overlays, encoding="utf-8")
        config, overlay_ref_count = re.subn(
            r"(?m)^arm9_overlays:\s*.*$",
            f"arm9_overlays: '{output_overlay_config.name}'",
            config,
        )
        if overlay_ref_count != 1:
            raise DataModError("could not redirect the ARM9 overlay configuration")

    if "arm9_main" in patched_binaries:
        if output_code is None:
            raise DataModError("output code directory is required for ARM9 patches")
        arm9_relative = os.path.relpath(
            output_code / "arm9.bin", output_path.parent
        ).replace("\\", "/")
        config, arm9_count = re.subn(
            r"(?m)^arm9_bin:\s*.*$", f"arm9_bin: '{arm9_relative}'", config
        )
        if arm9_count != 1:
            raise DataModError(f"expected exactly one arm9_bin in {input_path}")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(config, encoding="utf-8")


def command_check(args: argparse.Namespace) -> None:
    replacements, report = compile_project(
        args.files_root.resolve(), args.project_root.resolve()
    )
    del replacements
    overlay_9 = args.files_root.resolve().parent / "arm9_overlays" / "ov009.bin"
    _, binary_report = compile_binary_project(
        args.project_root.resolve(),
        {
            "arm9_overlay_9": overlay_9.read_bytes(),
            "arm9_main": (
                args.files_root.resolve().parent / "arm9" / "arm9.bin"
            ).read_bytes(),
        },
    )
    report.extend(binary_report)
    changed = sum(row["changed"] for row in report)
    print(f"Validated {len(report)} editable data files; {changed} contain modifications.")
    for row in report:
        if row["changed"]:
            print(
                f"  changed {row['source']}: {row['original_size']} -> {row['rebuilt_size']} bytes"
            )


def command_build(args: argparse.Namespace) -> None:
    files_root = args.files_root.resolve()
    project_root = args.project_root.resolve()
    staged_files = args.output_files.resolve()
    replacements, report = compile_project(files_root, project_root)

    project = read_json(project_root / "project.json")
    binary_documents = _require_list(
        project.get("binary_documents"), "binary_documents"
    )
    rebuilt_binaries: dict[str, bytes] = {}
    output_code: Path | None = None
    if binary_documents:
        if (
            args.overlay_9_bin is None
            or args.arm9_bin is None
            or args.output_code is None
        ):
            raise DataModError(
                "binary data documents require --arm9-bin, --overlay-9-bin, and --output-code"
            )
        output_code = args.output_code.resolve()
        rebuilt_binaries, binary_report = compile_binary_project(
            project_root,
            {
                "arm9_overlay_9": args.overlay_9_bin.resolve().read_bytes(),
                "arm9_main": args.arm9_bin.resolve().read_bytes(),
            },
        )
        report.extend(binary_report)

    staged_files.mkdir(parents=True, exist_ok=True)
    shutil.copytree(files_root, staged_files, dirs_exist_ok=True)
    for source, rebuilt in replacements.items():
        output = staged_files / Path(source)
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_bytes(rebuilt)

    if rebuilt_binaries:
        assert output_code is not None
        output_code.mkdir(parents=True, exist_ok=True)
        (output_code / "arm9_ov009.bin").write_bytes(
            rebuilt_binaries["arm9_overlay_9"]
        )
        (output_code / "arm9.bin").write_bytes(rebuilt_binaries["arm9_main"])

    if args.rom_config_input is not None or args.rom_config_output is not None:
        if args.rom_config_input is None or args.rom_config_output is None:
            raise DataModError(
                "--rom-config-input and --rom-config-output must be used together"
            )
        write_modded_rom_config(
            args.rom_config_input.resolve(),
            args.rom_config_output.resolve(),
            staged_files,
            output_code,
            set(rebuilt_binaries),
        )

    report_path = args.report.resolve() if args.report else staged_files.parent / "data_mod_report.json"
    write_json(report_path, {"schema": PROJECT_SCHEMA, "files": report})
    changed = sum(row["changed"] for row in report)
    print(
        f"Staged {len(report)} editable data files ({changed} changed) in {staged_files}"
    )


def create_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    export_parser = subparsers.add_parser(
        "export", help="export editable text and stat data from an extracted NitroFS"
    )
    export_parser.add_argument("--version", default="eur")
    export_parser.add_argument("--files-root", type=Path, required=True)
    export_parser.add_argument("--project-root", type=Path, required=True)
    export_parser.set_defaults(handler=command_export)

    check_parser = subparsers.add_parser(
        "check", help="validate and compile an editable data project in memory"
    )
    check_parser.add_argument("--files-root", type=Path, required=True)
    check_parser.add_argument("--project-root", type=Path, required=True)
    check_parser.set_defaults(handler=command_check)

    build_parser = subparsers.add_parser(
        "build", help="stage a NitroFS tree with rebuilt editable data"
    )
    build_parser.add_argument("--files-root", type=Path, required=True)
    build_parser.add_argument("--project-root", type=Path, required=True)
    build_parser.add_argument("--output-files", type=Path, required=True)
    build_parser.add_argument("--arm9-bin", type=Path)
    build_parser.add_argument("--overlay-9-bin", type=Path)
    build_parser.add_argument("--output-code", type=Path)
    build_parser.add_argument("--rom-config-input", type=Path)
    build_parser.add_argument("--rom-config-output", type=Path)
    build_parser.add_argument("--report", type=Path)
    build_parser.set_defaults(handler=command_build)
    return parser


def main() -> int:
    parser = create_parser()
    args = parser.parse_args()
    try:
        args.handler(args)
    except (DataModError, OSError, UnicodeError, struct.error) as exc:
        parser.error(str(exc))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
