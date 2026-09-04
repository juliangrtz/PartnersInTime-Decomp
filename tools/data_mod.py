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


ENEMY_STRUCT = struct.Struct("<HHBBHHHHHH14sHHII")


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
    write_json(
        project_root / "project.json",
        {
            "schema": PROJECT_SCHEMA,
            "version": args.version,
            "text_documents": text_documents,
            "stat_documents": [enemy_relative],
        },
    )
    print(
        f"Exported {len(text_documents)} text archives and one enemy table to {project_root}"
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
        ("text_documents", MFSET_SCHEMA),
        ("stat_documents", ENEMY_SCHEMA),
    )
    for key, expected_schema in document_groups:
        for relative_document in _require_list(project.get(key), key):
            if not isinstance(relative_document, str):
                raise DataModError(f"every {key} item must be a path string")
            document = read_json(project_root / relative_document)
            if not isinstance(document, dict) or document.get("schema") != expected_schema:
                raise DataModError(
                    f"{relative_document} must use schema {expected_schema!r}"
                )
            source = _validated_source(project_root, document)
            if source in replacements:
                raise DataModError(f"more than one document rebuilds {source}")
            source_path = files_root / Path(source)
            if not source_path.is_file():
                raise DataModError(f"private source file is missing: {source_path}")
            source_data = source_path.read_bytes()
            if expected_schema == MFSET_SCHEMA:
                rebuilt = build_mfset(document, source_data)
            else:
                rebuilt = build_enemy_stats(document, source_data)
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


def write_modded_rom_config(
    input_path: Path, output_path: Path, staged_files: Path
) -> None:
    config = input_path.read_text(encoding="utf-8")
    relative = os.path.relpath(staged_files, output_path.parent).replace("\\", "/")
    replacement = f"files_dir: '{relative}'"
    config, count = re.subn(r"(?m)^files_dir:\s*.*$", replacement, config)
    if count != 1:
        raise DataModError(f"expected exactly one files_dir in {input_path}")
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(config, encoding="utf-8")


def command_check(args: argparse.Namespace) -> None:
    replacements, report = compile_project(
        args.files_root.resolve(), args.project_root.resolve()
    )
    del replacements
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

    staged_files.mkdir(parents=True, exist_ok=True)
    shutil.copytree(files_root, staged_files, dirs_exist_ok=True)
    for source, rebuilt in replacements.items():
        output = staged_files / Path(source)
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_bytes(rebuilt)

    if args.rom_config_input is not None or args.rom_config_output is not None:
        if args.rom_config_input is None or args.rom_config_output is None:
            raise DataModError(
                "--rom-config-input and --rom-config-output must be used together"
            )
        write_modded_rom_config(
            args.rom_config_input.resolve(),
            args.rom_config_output.resolve(),
            staged_files,
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
