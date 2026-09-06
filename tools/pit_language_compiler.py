#!/usr/bin/env python3
"""Lossless source language for all Partners in Time script-VM instances.

The public API translates field, battle, and scene JSON documents to a small,
deliberately strict language and back.  It is a source frontend rather than a
second VM assembler: opcode descriptors and binary ABIs remain owned by the
three extraction/rebuild backends.  The battle/scene dialect implementations
live in :mod:`pit_vm_language` and are loaded lazily to keep this module's
field-room API backwards compatible.

Two details are intentionally explicit in the language:

* ``section`` anchors retain gaps which have not yet been classified as code.
  Instructions inside a section are fully relocatable.
* member ``metadata`` retains the hashes and pointer-table facts required by
  the current conservative binary rebuilder.  Labels, branch displacements,
  command boundaries, inline sizes, and command counts are never trusted from
  that metadata; the compiler derives them from the AST.
"""

from __future__ import annotations

import argparse
import copy
from dataclasses import dataclass, field
import hashlib
import json
from pathlib import Path
import re
import sys
from typing import Any, Iterable, Iterator, Sequence

import field_event_mod


ROOM_SCHEMA = field_event_mod.ROOM_SCHEMA
DEFAULT_VERSION = "eur"

CODE_REFERENCE_ARGUMENTS = {
    **field_event_mod.CORE_CODE_REFERENCES,
    **field_event_mod.FIELD_CODE_REFERENCES,
}
DATA_REFERENCE_ARGUMENTS = field_event_mod.EMBEDDED_DATA_REFERENCE_ARGUMENTS
INLINE_OPCODES = field_event_mod.INLINE_SCRIPT_OPCODES

# These names reflect the recovered runtime enum.  The five generic ailment
# slots have not all been given gameplay names in the matching decomp, so the
# compiler refuses to guess whether an individual slot is poison, burn, etc.
STATUS_NAMES = {
    0: "NONE",
    1: "AILMENT_1",
    2: "AILMENT_2",
    3: "AILMENT_3",
    4: "AILMENT_4",
    5: "AILMENT_5",
    6: "POWER_CHANGE",
    7: "DEFENSE_CHANGE",
    8: "SPEED_CHANGE",
}
STATUS_VALUES = {name: value for value, name in STATUS_NAMES.items()}

# Room facts are intentionally conservative.  Entries are added only after a
# script label/runtime trace and the playthrough chronology agree.  Unlisted
# rooms receive an honest generic note instead of fabricated story context.
ROOM_CONTEXT = {
    0: (
        "Peach's Castle opening/reunion event",
        "[Cutscene] The opening castle choreography introduces the time-machine "
        "incident and later rejoins the party members.",
    ),
    558: (
        "touch-screen mask-erasure set piece",
        "[Cutscene] This room enables the stylus sampler and drives the animated "
        "mask-erasure effect across the two DS screens.",
    ),
    583: (
        "Princess Shroob throne-room battle",
        "[Battle] This is the younger Princess Shroob encounter. Her scripted "
        "color-changing shield absorbs 15 accepted hits in five three-hit stages "
        "before it breaks and her vulnerable HP phase begins.",
    ),
    594: (
        "final farewell before the staff credits",
        "[Ending] The farewell sequence completes here and hands control to the "
        "staff-roll scene.",
    ),
    635: (
        "post-credits return-to-title event",
        "[Ending] The final field event returns the application to the title screen.",
    ),
}

CORPUS_SCHEMA = "pit-field-event-source-corpus-v1"
DIALOGUE_SCHEMA = "pit-localized-dialogue-v1"


@dataclass(frozen=True)
class MessageComment:
    language: str
    event_label: str | None
    text: str


class DialogueCatalog:
    """Read-only room/message lookup used only for private source comments."""

    def __init__(
        self,
        messages: dict[tuple[int, int], MessageComment],
        requested_language: str,
    ) -> None:
        self.messages = messages
        self.requested_language = requested_language

    @classmethod
    def from_json(
        cls, document: dict[str, Any], language: str = "german"
    ) -> "DialogueCatalog":
        if not isinstance(document, dict) or document.get("schema") != DIALOGUE_SCHEMA:
            raise PitLanguageError(
                f"dialogue catalog must use schema {DIALOGUE_SCHEMA!r}"
            )
        containers = document.get("containers")
        if not isinstance(containers, list):
            raise PitLanguageError("dialogue catalog containers must be an array")
        messages: dict[tuple[int, int], MessageComment] = {}
        for container_index, container in enumerate(containers):
            if not isinstance(container, dict):
                raise PitLanguageError(
                    f"dialogue container {container_index} must be an object"
                )
            room_id = container.get("room_id")
            languages = container.get("languages")
            if (
                isinstance(room_id, bool)
                or not isinstance(room_id, int)
                or not isinstance(languages, list)
            ):
                raise PitLanguageError(
                    f"dialogue container {container_index} has invalid room/languages"
                )
            by_language = {
                row.get("language"): row
                for row in languages
                if isinstance(row, dict) and isinstance(row.get("language"), str)
            }
            selected_language = next(
                (
                    candidate
                    for candidate in (language, "english", "japanese")
                    if candidate in by_language
                ),
                None,
            )
            if selected_language is None:
                continue
            strings = by_language[selected_language].get("strings")
            if not isinstance(strings, list):
                raise PitLanguageError(
                    f"dialogue room {room_id} {selected_language} strings must be an array"
                )
            for expected_id, row in enumerate(strings):
                if not isinstance(row, dict) or row.get("id") != expected_id:
                    raise PitLanguageError(
                        f"dialogue room {room_id} {selected_language} message IDs "
                        "must be contiguous"
                    )
                text = row.get("text")
                event_label = row.get("event_label")
                if not isinstance(text, str) or not (
                    event_label is None or isinstance(event_label, str)
                ):
                    raise PitLanguageError(
                        f"dialogue room {room_id} message {expected_id} is invalid"
                    )
                key = (room_id, expected_id)
                if key in messages:
                    raise PitLanguageError(
                        f"dialogue catalog repeats room {room_id} message {expected_id}"
                    )
                messages[key] = MessageComment(
                    selected_language, event_label, text
                )
        return cls(messages, language)

    def get(self, room_id: int, message_id: int) -> MessageComment | None:
        return self.messages.get((room_id, message_id))


class PitLanguageError(ValueError):
    """Raised for invalid JSON, invalid source, or an impossible relocation."""


@dataclass(frozen=True)
class Token:
    kind: str
    text: str
    value: Any
    line: int
    column: int


@dataclass(frozen=True)
class Value:
    kind: str
    value: int | str

    @staticmethod
    def literal(value: int) -> "Value":
        return Value("literal", value)

    @staticmethod
    def variable(value: str) -> "Value":
        return Value("variable", value)

    @staticmethod
    def label(value: str) -> "Value":
        return Value("label", value)


@dataclass
class LabelStatement:
    name: str


@dataclass
class CallStatement:
    api_name: str
    result: Value | None
    arguments: list[Value]
    unused_mode_bits: int = 0


@dataclass
class AsyncStatement:
    wait: bool
    entity_selector: Value
    chain_if_active: Value
    body: list["Statement"]


@dataclass
class ForStatement:
    initial: Value
    limit: Value
    body: list["Statement"]


@dataclass
class PaddingStatement:
    halfwords: int


Statement = (
    LabelStatement | CallStatement | AsyncStatement | ForStatement | PaddingStatement
)


@dataclass
class Section:
    source_offset: int
    body: list[Statement]


@dataclass
class DataRecord:
    label: str
    source_offset: int
    payload: dict[str, Any]


@dataclass
class Member:
    room_part: int
    metadata: dict[str, Any]
    data_records: list[DataRecord]
    sections: list[Section]


@dataclass
class Program:
    room_id: int
    metadata: dict[str, Any]
    members: list[Member]


@dataclass
class Instruction:
    opcode: int
    result: Value | None
    arguments: list[Value]
    unused_mode_bits: int = 0
    labels: list[str] = field(default_factory=list)
    offset: int = 0
    end: int = 0
    data_record: str | None = None


@dataclass
class PaddingItem:
    halfwords: int


_TOKEN_RE = re.compile(
    r"""
    (?P<SPACE>[ \t\r\n]+)
  | (?P<COMMENT>//[^\r\n]*)
  | (?P<STRING>"(?:\\["\\/bfnrt]|\\u[0-9A-Fa-f]{4}|[^"\\\x00-\x1F])*")
  | (?P<NUMBER>-?0[xX][0-9A-Fa-f]+|-?(?:0|[1-9][0-9]*))
  | (?P<OP>!=|--|\+\+)
  | (?P<IDENT>[A-Za-z_][A-Za-z0-9_]*)
  | (?P<PUNCT>[{}()\[\],;:=.&@])
    """,
    re.VERBOSE,
)


def _tokens(text: str) -> list[Token]:
    result: list[Token] = []
    position = 0
    line = 1
    column = 1
    while position < len(text):
        match = _TOKEN_RE.match(text, position)
        if match is None:
            excerpt = text[position : position + 20].splitlines()[0]
            raise PitLanguageError(
                f"line {line}, column {column}: unexpected input {excerpt!r}"
            )
        raw = match.group(0)
        kind = match.lastgroup or ""
        token_line, token_column = line, column
        newline_count = raw.count("\n")
        if newline_count:
            line += newline_count
            column = len(raw.rsplit("\n", 1)[1]) + 1
        else:
            column += len(raw)
        position = match.end()
        if kind in {"SPACE", "COMMENT"}:
            continue
        if kind == "STRING":
            value: Any = json.loads(raw)
        elif kind == "NUMBER":
            value = int(raw, 0)
        else:
            value = raw
        result.append(Token(kind, raw, value, token_line, token_column))
    result.append(Token("EOF", "", None, line, column))
    return result


class Parser:
    """Recursive-descent parser with location-rich diagnostics."""

    def __init__(self, text: str) -> None:
        self.tokens = _tokens(text)
        self.index = 0

    @property
    def current(self) -> Token:
        return self.tokens[self.index]

    def error(self, message: str, token: Token | None = None) -> PitLanguageError:
        here = token or self.current
        return PitLanguageError(f"line {here.line}, column {here.column}: {message}")

    def accept(self, text: str) -> Token | None:
        if self.current.text == text:
            token = self.current
            self.index += 1
            return token
        return None

    def expect(self, text: str) -> Token:
        token = self.accept(text)
        if token is None:
            raise self.error(f"expected {text!r}, got {self.current.text!r}")
        return token

    def expect_kind(self, kind: str, description: str) -> Token:
        if self.current.kind != kind:
            raise self.error(f"expected {description}, got {self.current.text!r}")
        token = self.current
        self.index += 1
        return token

    def parse_program(self) -> Program:
        self.expect("room")
        room_id = self.expect_kind("NUMBER", "room ID").value
        if room_id < 0:
            raise self.error("room ID cannot be negative", self.tokens[self.index - 1])
        self.expect("{")
        self.expect("metadata")
        metadata = self.parse_json_object()
        self.accept(";")
        members: list[Member] = []
        while self.current.text == "member":
            members.append(self.parse_member())
        self.expect("}")
        if self.current.kind != "EOF":
            raise self.error("unexpected content after room")
        return Program(room_id, metadata, members)

    def parse_member(self) -> Member:
        self.expect("member")
        room_part = self.expect_kind("NUMBER", "room part").value
        if room_part < 0:
            raise self.error("room part cannot be negative", self.tokens[self.index - 1])
        self.expect("{")
        metadata: dict[str, Any] | None = None
        records: list[DataRecord] = []
        sections: list[Section] = []
        while self.current.text != "}":
            if self.current.text == "metadata":
                if metadata is not None:
                    raise self.error("member may contain only one metadata block")
                self.expect("metadata")
                metadata = self.parse_json_object()
                self.accept(";")
            elif self.current.text == "data":
                records.append(self.parse_data_record())
            elif self.current.text == "section":
                sections.append(self.parse_section())
            else:
                raise self.error("expected 'data', 'section', or '}'")
        self.expect("}")
        if metadata is None:
            raise self.error("member requires one metadata block")
        return Member(room_part, metadata, records, sections)

    def parse_data_record(self) -> DataRecord:
        self.expect("data")
        label = self.expect_kind("IDENT", "data label").text
        self.expect("at")
        offset = self.expect_kind("NUMBER", "data offset").value
        if offset < 0:
            raise self.error("data offset cannot be negative", self.tokens[self.index - 1])
        payload = self.parse_json_object()
        self.accept(";")
        return DataRecord(label, offset, payload)

    def parse_section(self) -> Section:
        self.expect("section")
        offset = self.expect_kind("NUMBER", "section offset").value
        if offset < 0:
            raise self.error("section offset cannot be negative", self.tokens[self.index - 1])
        self.expect("{")
        body = self.parse_statements()
        self.expect("}")
        return Section(offset, body)

    def parse_statements(self) -> list[Statement]:
        result: list[Statement] = []
        while self.current.text != "}":
            if self.current.kind == "EOF":
                raise self.error("unterminated block")
            if self.current.text == "async":
                result.append(self.parse_async())
            elif self.current.text == "for":
                result.append(self.parse_for())
            elif self.current.text == "padding":
                result.append(self.parse_padding())
            elif (
                self.current.kind == "IDENT"
                and self.tokens[self.index + 1].text == ":"
            ):
                name = self.current.text
                self.index += 2
                result.append(LabelStatement(name))
            else:
                result.append(self.parse_call())
        return result

    def parse_padding(self) -> PaddingStatement:
        self.expect("padding")
        halfwords = self.expect_kind("NUMBER", "padding size in halfwords").value
        if halfwords <= 0:
            raise self.error("padding size must be positive", self.tokens[self.index - 1])
        self.expect(";")
        return PaddingStatement(halfwords)

    def parse_async(self) -> AsyncStatement:
        self.expect("async")
        api_name = self.parse_qualified_name()
        if api_name not in {"Actor.Run", "Actor.RunAndWait"}:
            raise self.error("async target must be Actor.Run or Actor.RunAndWait")
        self.expect("(")
        selector = self.parse_value()
        self.expect(",")
        chain = self.parse_value()
        self.expect(")")
        self.expect("{")
        body = self.parse_statements()
        self.expect("}")
        return AsyncStatement(api_name.endswith("AndWait"), selector, chain, body)

    def parse_for(self) -> ForStatement:
        self.expect("for")
        self.expect("(")
        self.expect("loop_counter")
        self.expect("=")
        initial = self.parse_value()
        self.expect(";")
        self.expect("loop_counter")
        self.expect("!=")
        limit = self.parse_value()
        self.expect(";")
        self.expect("loop_counter")
        self.expect("--")
        self.expect(")")
        self.expect("{")
        body = self.parse_statements()
        self.expect("}")
        return ForStatement(initial, limit, body)

    def parse_call(self) -> CallStatement:
        result: Value | None = None
        checkpoint = self.index
        try:
            candidate = self.parse_value()
        except PitLanguageError:
            candidate = None
        if candidate is not None and candidate.kind == "variable" and self.accept("="):
            result = candidate
        else:
            self.index = checkpoint
        api_name = self.parse_qualified_name()
        self.expect("(")
        arguments: list[Value] = []
        if self.current.text != ")":
            while True:
                arguments.append(self.parse_value())
                if not self.accept(","):
                    break
        self.expect(")")
        unused_mode_bits = 0
        if self.accept("@"):
            self.expect("mode")
            self.expect("(")
            unused_mode_bits = self.expect_kind("NUMBER", "mode mask").value
            self.expect(")")
        self.expect(";")
        return CallStatement(api_name, result, arguments, unused_mode_bits)

    def parse_qualified_name(self) -> str:
        first = self.expect_kind("IDENT", "qualified call name").text
        self.expect(".")
        second = self.expect_kind("IDENT", "method name").text
        return f"{first}.{second}"

    def parse_value(self) -> Value:
        if self.accept("&"):
            return Value.label(self.expect_kind("IDENT", "label name").text)
        if self.current.kind == "NUMBER":
            value = self.current.value
            self.index += 1
            return Value.literal(value)
        if self.current.kind != "IDENT":
            raise self.error("expected integer, variable, constant, or label reference")
        name = self.current.text
        self.index += 1
        if name == "var":
            self.expect("(")
            raw = self.expect_kind("NUMBER", "raw variable ID").value
            self.expect(")")
            if not 0 <= raw <= 0xFFFF:
                raise self.error("raw variable ID must fit in 16 bits")
            return Value.variable(f"0x{raw:04X}")
        if name == "Status":
            self.expect(".")
            status = self.expect_kind("IDENT", "status name").text
            if status not in STATUS_VALUES:
                raise self.error(f"unknown status name Status.{status}")
            return Value.literal(STATUS_VALUES[status])
        if self.accept("["):
            index = self.expect_kind("NUMBER", "variable index").value
            self.expect("]")
            if index < 0:
                raise self.error("variable index cannot be negative")
            return Value.variable(f"{name}[{index}]")
        if self.accept("."):
            member = self.expect_kind("IDENT", "named variable").text
            if self.accept("["):
                index = self.expect_kind("NUMBER", "variable index").value
                self.expect("]")
                if index < 0:
                    raise self.error("variable index cannot be negative")
                return Value.variable(f"{name}.{member}[{index}]")
            return Value.variable(f"{name}.{member}")
        raise self.error(f"bare identifier {name!r} is not a value")

    def parse_json_object(self) -> dict[str, Any]:
        value = self.parse_json_value()
        if not isinstance(value, dict):
            raise self.error("metadata payload must be a JSON object")
        return value

    def parse_json_value(self) -> Any:
        if self.accept("{"):
            result: dict[str, Any] = {}
            if self.accept("}"):
                return result
            while True:
                key = self.expect_kind("STRING", "JSON object key").value
                if key in result:
                    raise self.error(f"duplicate JSON key {key!r}")
                self.expect(":")
                result[key] = self.parse_json_value()
                if not self.accept(","):
                    break
            self.expect("}")
            return result
        if self.accept("["):
            values: list[Any] = []
            if self.accept("]"):
                return values
            while True:
                values.append(self.parse_json_value())
                if not self.accept(","):
                    break
            self.expect("]")
            return values
        token = self.current
        if token.kind in {"STRING", "NUMBER"}:
            self.index += 1
            return token.value
        if token.text in {"true", "false", "null"}:
            self.index += 1
            return {"true": True, "false": False, "null": None}[token.text]
        raise self.error("expected JSON value")


def _pascal_case(name: str) -> str:
    return "".join(part[:1].upper() + part[1:] for part in name.split("_") if part)


_EXPLICIT_API_NAMES = {
    "open_entity_message": "Actor.OpenMessage",
    "start_camera_shake": "Camera.StartShake",
    "rejoin_party_follower": "Party.RejoinFollower",
    "start_inline_entity_script": "Actor.Run",
    "start_inline_entity_script_and_wait": "Actor.RunAndWait",
}


def _candidate_api_name(name: str) -> str:
    if name in _EXPLICIT_API_NAMES:
        return _EXPLICIT_API_NAMES[name]
    words = name.split("_")
    if "camera" in words:
        namespace, removed = "Camera", {"camera"}
    elif "message" in words:
        namespace, removed = "Dialog", {"message"}
    elif "party" in words or "follower" in words:
        namespace, removed = "Party", {"party"}
    elif "entity" in words or "enemy" in words:
        namespace, removed = "Actor", {"entity"}
    elif "battle" in words:
        namespace, removed = "Battle", {"battle"}
    elif "screen" in words:
        namespace, removed = "Screen", {"screen"}
    elif "sound" in words or "music" in words or "bgm" in words:
        namespace, removed = "Audio", {"sound", "music"}
    elif "room" in words or "field" in words:
        namespace, removed = "Field", {"field"}
    elif any(word in words for word in ("save", "coin", "bean", "item")):
        namespace, removed = "Save", set()
    elif name in {"jump", "jump_if", "push", "loop", "return", "end"}:
        namespace, removed = "Flow", set()
    else:
        namespace, removed = "VM", set()
    method_words = [word for word in words if word not in removed]
    return f"{namespace}.{_pascal_case('_'.join(method_words))}"


def _api_maps(names: dict[int, str]) -> tuple[dict[int, str], dict[str, int]]:
    forward = {opcode: _candidate_api_name(name) for opcode, name in names.items()}
    collisions: dict[str, list[int]] = {}
    for opcode, api_name in forward.items():
        collisions.setdefault(api_name, []).append(opcode)
    for api_name, opcodes in collisions.items():
        if len(opcodes) > 1:
            for opcode in opcodes:
                forward[opcode] = f"VM.{_pascal_case(names[opcode])}"
    reverse = {api_name: opcode for opcode, api_name in forward.items()}
    if len(reverse) != len(forward):
        raise AssertionError("field VM API-name mapping is not injective")
    return forward, reverse


def _load_schema() -> tuple[tuple[int, ...], dict[int, str], dict[int, dict[str, Any]]]:
    descriptors, names = field_event_mod.load_vm_schema(DEFAULT_VERSION)
    path = field_event_mod.ROOT / "config" / DEFAULT_VERSION / "field_vm.json"
    document = json.loads(path.read_text(encoding="utf-8"))
    semantics = {
        int(opcode, 0): row
        for opcode, row in document.get("opcode_semantics", {}).items()
    }
    return descriptors, names, semantics


def _as_offset(value: Any, context: str) -> int:
    if isinstance(value, bool):
        raise PitLanguageError(f"{context} must be an integer offset")
    try:
        result = int(value, 0) if isinstance(value, str) else int(value)
    except (TypeError, ValueError) as exc:
        raise PitLanguageError(f"{context} has invalid offset {value!r}") from exc
    if result < 0 or result & 1:
        raise PitLanguageError(f"{context} must be a non-negative even offset")
    return result


def _instruction_size(opcode: int, descriptors: Sequence[int]) -> int:
    descriptor = descriptors[opcode]
    count = descriptor & 0x1F
    return (
        2
        + (2 if descriptor & 0x20 else 0)
        + (2 if descriptor & 0x40 and count else 0)
        + count * 2
    )


def _opcode_from_json(
    value: Any,
    descriptors: tuple[int, ...],
    names: dict[int, str],
    context: str,
) -> int:
    try:
        return field_event_mod.parse_opcode(value, descriptors, names, context)
    except Exception as exc:
        raise PitLanguageError(str(exc)) from exc


def _variable_from_json(value: Any, context: str) -> Value:
    if not isinstance(value, str):
        raise PitLanguageError(f"{context} must be a VM variable string")
    try:
        raw = field_event_mod.parse_variable(value, context)
    except Exception as exc:
        raise PitLanguageError(str(exc)) from exc
    return Value.variable(field_event_mod.format_variable(raw))


def _argument_from_json(value: Any, context: str) -> Value:
    if isinstance(value, dict):
        if set(value) != {"variable"}:
            raise PitLanguageError(
                f"{context} variable reference must contain exactly 'variable'"
            )
        return _variable_from_json(value["variable"], context)
    if isinstance(value, bool) or not isinstance(value, int):
        raise PitLanguageError(f"{context} literal must be a signed integer")
    if not -0x8000 <= value <= 0x7FFF:
        raise PitLanguageError(f"{context} literal does not fit signed 16 bits")
    return Value.literal(value)


def _format_offset(value: int) -> str:
    return f"0x{value:04X}"


def _format_variable(name: str) -> str:
    return f"var({name})" if name.startswith("0x") else name


def _format_value(value: Value, status_parameter: bool = False) -> str:
    if value.kind == "label":
        return f"&{value.value}"
    if value.kind == "variable":
        return _format_variable(str(value.value))
    literal = int(value.value)
    if status_parameter and literal in STATUS_NAMES:
        return f"Status.{STATUS_NAMES[literal]}"
    return str(literal)


def _json_lines(value: dict[str, Any], indent: int) -> list[str]:
    prefix = " " * indent
    encoded = json.dumps(value, ensure_ascii=False, indent=2)
    return [prefix + line for line in encoded.splitlines()]


def _command_arguments(
    command: dict[str, Any],
    opcode: int,
    end: int,
    labels_by_offset: dict[int, list[str]],
    data_labels_by_offset: dict[int, str],
    context: str,
) -> list[Value]:
    raw_arguments = command.get("args")
    if not isinstance(raw_arguments, list):
        raise PitLanguageError(f"{context} args must be an array")
    arguments = [
        _argument_from_json(value, f"{context} argument {index}")
        for index, value in enumerate(raw_arguments)
    ]
    reference_index = CODE_REFERENCE_ARGUMENTS.get(opcode)
    if reference_index is not None and arguments[reference_index].kind == "literal":
        target = end + int(arguments[reference_index].value) * 2
        target_labels = labels_by_offset.get(target)
        if not target_labels:
            raise PitLanguageError(
                f"{context} branch target {_format_offset(target)} has no label"
            )
        preferred = next(
            (name for name in target_labels if name.startswith("script_")),
            target_labels[0],
        )
        arguments[reference_index] = Value.label(preferred)
    data_index = DATA_REFERENCE_ARGUMENTS.get(opcode)
    if data_index is not None and arguments[data_index].kind == "literal":
        target = end + int(arguments[data_index].value) * 2
        label = data_labels_by_offset.get(target)
        if label is None:
            raise PitLanguageError(
                f"{context} embedded-data target {_format_offset(target)} has no record"
            )
        arguments[data_index] = Value.label(label)
    return arguments


def _literal(value: Value) -> int | None:
    return int(value.value) if value.kind == "literal" else None


def _brief_value(value: Value) -> str:
    if value.kind == "variable":
        return str(value.value)
    if value.kind == "label":
        return f"label {value.value}"
    return str(value.value)


def _instruction_comment(name: str, arguments: list[Value]) -> str | None:
    """Return semantic commentary without quoting copyrighted dialogue."""
    if name == "open_entity_message" and len(arguments) >= 12:
        return (
            "[Dialogue] Actor "
            f"{_brief_value(arguments[0])} opens message "
            f"{_brief_value(arguments[10])}:{_brief_value(arguments[11])}; "
            "the localized text itself remains outside this source file."
        )
    if name == "open_screen_message" and len(arguments) >= 13:
        return (
            "[Dialogue] A screen-anchored message window opens for archive/message "
            f"{_brief_value(arguments[11])}:{_brief_value(arguments[12])}."
        )
    if name == "set_entity_position" and len(arguments) >= 5:
        return (
            f"[Choreography] Place actor {_brief_value(arguments[0])} at "
            f"({_brief_value(arguments[2])}, {_brief_value(arguments[3])}, "
            f"{_brief_value(arguments[4])})."
        )
    if "entity_movement" in name and name.startswith("start_"):
        return "[Choreography] Begin an actor movement track; the matching wait opcode synchronizes the cutscene."
    if name == "set_camera_focus_entity" and len(arguments) >= 2:
        return f"[Camera] Track actor {_brief_value(arguments[1])} as the scene focus."
    if name == "start_camera_shake" and len(arguments) >= 4:
        return (
            "[Camera] Shake the scene with amplitude "
            f"{_brief_value(arguments[1])} for {_brief_value(arguments[3])} half-cycles."
        )
    if name == "detach_party_follower":
        return "[Party] Separate the follower so the next commands may choreograph it independently."
    if name == "rejoin_party_follower":
        return "[Party] Reattach the separated follower to the active field formation."
    if name in {"start_scripted_battle", "start_scripted_battle_extended"} and arguments:
        return f"[Battle] Hand off to scripted encounter {_brief_value(arguments[0])}; field execution resumes after combat."
    if name == "adjust_party_member_hp" and len(arguments) >= 2:
        return f"[Party] Apply signed HP delta {_brief_value(arguments[1])} to character slot {_brief_value(arguments[0])}."
    if name == "start_staff_credits":
        return "[Ending] Leave the field scene and start the staff roll."
    if name == "return_to_title_screen":
        return "[Ending] Tear down the field scene and return to the title screen."
    if name == "add_entity_roaming_profile":
        return "[Actor AI] Attach the referenced random-roaming profile to this actor."
    if name == "load_entity_waypoint_path":
        return "[Actor AI] Load the referenced waypoint path; a later opcode starts traversal."
    if "status" in name:
        return "[Battle status] Symbolic Status.* values preserve the recovered runtime status ID."
    return None


_MESSAGE_CONTROL_RE = re.compile(r"<\$[^>]+>")
_MESSAGE_BREAK_RE = re.compile(
    r"<\$(?:WAIT|TEXTBOX|CTRL:01)(?::[^>]*)?>", re.IGNORECASE
)


def _message_comment_lines(message: MessageComment) -> list[str]:
    """Turn editable control-token text into searchable one-line comments."""
    text = _MESSAGE_BREAK_RE.sub("\n", message.text)
    text = _MESSAGE_CONTROL_RE.sub("", text)
    text = text.replace(r"\<", "<").replace(r"\\", "\\")
    lines = [re.sub(r"[ \t]+", " ", line).strip() for line in text.splitlines()]
    lines = [line for line in lines if line]
    return lines or ["(leere Nachricht)"]


class Decompiler:
    def __init__(
        self,
        json_data: dict[str, Any],
        dialogue_catalog: DialogueCatalog | None = None,
    ) -> None:
        self.document = json_data
        self.dialogue_catalog = dialogue_catalog
        self.room_id = -1
        self.descriptors, self.names, self.semantics = _load_schema()
        self.api_names, _ = _api_maps(self.names)

    def run(self) -> str:
        if not isinstance(self.document, dict):
            raise PitLanguageError("room document must be a JSON object")
        if self.document.get("schema") != ROOM_SCHEMA:
            raise PitLanguageError(f"document must use schema {ROOM_SCHEMA!r}")
        room_id = self.document.get("room_id")
        if isinstance(room_id, bool) or not isinstance(room_id, int) or room_id < 0:
            raise PitLanguageError("room_id must be a non-negative integer")
        self.room_id = room_id
        members = self.document.get("members")
        if not isinstance(members, list):
            raise PitLanguageError("members must be an array")

        title, context = ROOM_CONTEXT.get(
            room_id,
            (
                "field-event room (story context not yet confirmed)",
                "[Context] This room has not yet been correlated with a verified "
                "playthrough timestamp; command-level comments remain semantic only.",
            ),
        )
        lines = [
            "// PiT field-event source; generated offsets are halfword-relative.",
            f"// Room {room_id}: {title}.",
            f"// {context}",
            f"room {room_id} {{",
            "  metadata",
        ]
        top_metadata = {
            key: copy.deepcopy(value)
            for key, value in self.document.items()
            if key not in {"room_id", "members"}
        }
        lines.extend(_json_lines(top_metadata, 2))
        lines.append("")
        for member_index, member in enumerate(members):
            if not isinstance(member, dict):
                raise PitLanguageError(f"member {member_index} must be an object")
            lines.extend(self.render_member(member, room_id, member_index))
        lines.append("}")
        return "\n".join(lines) + "\n"

    def render_member(
        self, member: dict[str, Any], room_id: int, member_index: int
    ) -> list[str]:
        room_part = member.get("room_part")
        if isinstance(room_part, bool) or not isinstance(room_part, int):
            raise PitLanguageError(f"member {member_index} has invalid room_part")
        if member.get("room_id") != room_id:
            raise PitLanguageError(f"member {member_index} belongs to another room")
        raw_commands = member.get("commands")
        raw_records = member.get("embedded_data_records")
        if not isinstance(raw_commands, list) or not isinstance(raw_records, list):
            raise PitLanguageError(
                f"member {member_index} commands/data records must be arrays"
            )

        metadata = {
            key: copy.deepcopy(value)
            for key, value in member.items()
            if key
            not in {
                "room_id",
                "room_part",
                "commands",
                "embedded_data_records",
                "reachable_command_count",
                "embedded_data_record_count",
            }
        }
        records: list[DataRecord] = []
        for record_index, record in enumerate(raw_records):
            if not isinstance(record, dict):
                raise PitLanguageError(
                    f"member {member_index} data record {record_index} must be an object"
                )
            label = record.get("label")
            if not isinstance(label, str) or not re.fullmatch(
                r"[A-Za-z_][A-Za-z0-9_]*", label
            ):
                raise PitLanguageError(
                    f"member {member_index} data record {record_index} has invalid label"
                )
            offset = _as_offset(
                record.get("source_offset"),
                f"member {member_index} data record {record_index}",
            )
            payload = {
                key: copy.deepcopy(value)
                for key, value in record.items()
                if key not in {"label", "source_offset"}
            }
            records.append(DataRecord(label, offset, payload))

        labels_by_offset: dict[int, list[str]] = {}
        commands: list[Instruction] = []
        seen_offsets: set[int] = set()
        for command_index, command in enumerate(raw_commands):
            context = f"member {member_index} command {command_index}"
            if not isinstance(command, dict):
                raise PitLanguageError(f"{context} must be an object")
            offset = _as_offset(command.get("source_offset"), context)
            if offset in seen_offsets:
                raise PitLanguageError(f"{context} duplicates {_format_offset(offset)}")
            seen_offsets.add(offset)
            opcode = _opcode_from_json(
                command.get("opcode"), self.descriptors, self.names, context
            )
            end = offset + _instruction_size(opcode, self.descriptors)
            labels = command.get("labels", [])
            if not isinstance(labels, list) or not all(
                isinstance(label, str)
                and re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", label)
                for label in labels
            ):
                raise PitLanguageError(f"{context} labels must be identifiers")
            labels_by_offset.setdefault(offset, []).extend(labels)
            result = (
                _variable_from_json(command["result"], f"{context} result")
                if "result" in command
                else None
            )
            mode = command.get("unused_mode_bits", 0)
            try:
                mode_value = int(mode, 0) if isinstance(mode, str) else int(mode)
            except (TypeError, ValueError) as exc:
                raise PitLanguageError(f"{context} has invalid mode mask") from exc
            commands.append(
                Instruction(
                    opcode,
                    result,
                    [],
                    mode_value,
                    list(labels),
                    offset,
                    end,
                    command.get("data_record"),
                )
            )

        commands.sort(key=lambda instruction: instruction.offset)
        records_by_offset = {record.source_offset: record.label for record in records}
        raw_by_offset = {
            _as_offset(command["source_offset"], "command source_offset"): command
            for command in raw_commands
        }
        for instruction in commands:
            instruction.arguments = _command_arguments(
                raw_by_offset[instruction.offset],
                instruction.opcode,
                instruction.end,
                labels_by_offset,
                records_by_offset,
                f"member {member_index} command {_format_offset(instruction.offset)}",
            )
            descriptor = self.descriptors[instruction.opcode]
            expected_count = descriptor & 0x1F
            if len(instruction.arguments) != expected_count:
                raise PitLanguageError(
                    f"member {member_index} command {_format_offset(instruction.offset)} "
                    f"expects {expected_count} arguments"
                )
            if bool(descriptor & 0x20) != (instruction.result is not None):
                raise PitLanguageError(
                    f"member {member_index} command {_format_offset(instruction.offset)} "
                    "has an invalid result contract"
                )

        lines = [
            f"  member {room_part} {{",
            "    // Executable event code comes first; structural rebuild metadata is below it.",
        ]
        inline_ranges = [
            (instruction.end, instruction.end + int(instruction.arguments[2].value) * 2)
            for instruction in commands
            if instruction.opcode in INLINE_OPCODES
            and instruction.arguments[2].kind == "literal"
        ]
        offsets_by_label = {
            label: offset
            for offset, labels in labels_by_offset.items()
            for label in labels
        }
        loop_ranges = [
            (commands[index - 1].offset, offsets_by_label[str(instruction.arguments[3].value)])
            for index, instruction in enumerate(commands)
            if index > 0
            and instruction.opcode == 0x0C
            and commands[index - 1].opcode == 0x0A
            and instruction.arguments[3].kind == "label"
            and str(instruction.arguments[3].value) in offsets_by_label
        ]
        structured_ranges = inline_ranges + loop_ranges
        sections: list[list[Instruction]] = []
        for instruction in commands:
            previous_end = sections[-1][-1].end if sections else None
            gap_is_inline_payload = (
                previous_end is not None
                and any(
                    body_start <= previous_end < instruction.offset <= resume
                    for body_start, resume in structured_ranges
                )
            )
            if (
                not sections
                or (
                    sections[-1][-1].end != instruction.offset
                    and not gap_is_inline_payload
                )
            ):
                sections.append([instruction])
            else:
                sections[-1].append(instruction)
        all_inbound: dict[int, list[int]] = {}
        for instruction in commands:
            index = CODE_REFERENCE_ARGUMENTS.get(instruction.opcode)
            if index is not None and instruction.arguments[index].kind == "label":
                target_label = str(instruction.arguments[index].value)
                for target_offset, target_labels in labels_by_offset.items():
                    if target_label in target_labels:
                        all_inbound.setdefault(target_offset, []).append(instruction.offset)
                        break

        for section in sections:
            lines.extend(
                ["", f"    section {_format_offset(section[0].offset)} {{"]
            )
            lines.extend(
                self.render_range(
                    section,
                    0,
                    len(section),
                    6,
                    labels_by_offset,
                    all_inbound,
                    section[0].offset,
                    section[-1].end,
                )
            )
            lines.append("    }")
        for record in records:
            lines.extend(
                [
                    "",
                    f"    // Embedded {record.payload.get('type', 'VM data')} used by actor movement.",
                    f"    data {record.label} at {_format_offset(record.source_offset)}",
                ]
            )
            lines.extend(_json_lines(record.payload, 4))
        lines.extend(
            [
                "",
                "    // Private-source layout contract used by the conservative binary rebuilder.",
                "    metadata",
            ]
        )
        lines.extend(_json_lines(metadata, 4))
        lines.extend(["  }", ""])
        return lines

    def render_range(
        self,
        commands: list[Instruction],
        start: int,
        stop: int,
        indent: int,
        labels_by_offset: dict[int, list[str]],
        inbound: dict[int, list[int]],
        expected_start: int,
        expected_end: int,
    ) -> list[str]:
        lines: list[str] = []
        index = start
        cursor = expected_start
        while index < stop:
            command = commands[index]
            prefix = " " * indent
            if command.offset < cursor:
                raise PitLanguageError("overlapping commands in structured source range")
            if command.offset > cursor:
                gap = command.offset - cursor
                if gap & 1:
                    raise PitLanguageError("odd-byte gap in structured source range")
                lines.append(
                    f"{prefix}// Preserve {gap // 2} opaque halfword(s) from the private binary source."
                )
                lines.append(f"{prefix}padding {gap // 2};")
                cursor = command.offset
            for label in command.labels:
                lines.append(f"{prefix}{label}:")

            if command.opcode in INLINE_OPCODES:
                size = _literal(command.arguments[2])
                if size is None or size < 0:
                    raise PitLanguageError(
                        f"inline command at {_format_offset(command.offset)} has a variable/negative size"
                    )
                resume = command.end + size * 2
                child_stop = index + 1
                cursor = command.end
                while child_stop < stop and commands[child_stop].offset < resume:
                    child = commands[child_stop]
                    if child.offset < cursor or child.end > resume:
                        raise PitLanguageError(
                            f"inline body at {_format_offset(command.offset)} overlaps or exceeds its boundary"
                        )
                    cursor = child.end
                    child_stop += 1
                if cursor > resume:
                    raise PitLanguageError(
                        f"inline body at {_format_offset(command.offset)} does not end at {_format_offset(resume)}"
                    )
                lines.append(
                    f"{prefix}// [Async] Run actor {_brief_value(command.arguments[0])}'s "
                    + ("blocking" if command.opcode == 0x3D else "non-blocking")
                    + " inline choreography; its encoded size is derived from this block."
                )
                method = "Actor.RunAndWait" if command.opcode == 0x3D else "Actor.Run"
                lines.append(
                    f"{prefix}async {method}("
                    f"{_format_value(command.arguments[0])}, "
                    f"{_format_value(command.arguments[1])}) {{"
                )
                lines.extend(
                    self.render_range(
                        commands,
                        index + 1,
                        child_stop,
                        indent + 2,
                        labels_by_offset,
                        inbound,
                        command.end,
                        resume,
                    )
                )
                lines.append(f"{prefix}}}")
                index = child_stop
                cursor = resume
                continue

            loop = self.find_counted_loop(commands, index, stop, inbound)
            if loop is not None:
                body_start, jump_index, exit_index = loop
                lines.append(
                    f"{prefix}// [Loop] Count from {_brief_value(command.arguments[0])} "
                    f"down to {_brief_value(commands[index + 1].arguments[2])}."
                )
                lines.append(
                    f"{prefix}for (loop_counter = {_format_value(command.arguments[0])}; "
                    f"loop_counter != {_format_value(commands[index + 1].arguments[2])}; "
                    "loop_counter--) {"
                )
                lines.extend(
                    self.render_range(
                        commands,
                        body_start,
                        jump_index,
                        indent + 2,
                        labels_by_offset,
                        inbound,
                        commands[index + 1].end,
                        commands[jump_index].offset,
                    )
                )
                for label in commands[jump_index].labels:
                    lines.append(f"{' ' * (indent + 2)}{label}:")
                lines.append(f"{prefix}}}")
                index = exit_index
                cursor = commands[exit_index].offset
                continue

            name = self.names.get(command.opcode, f"op_{command.opcode:03X}")
            comment = _instruction_comment(name, command.arguments)
            localized_message = (
                self.dialogue_catalog is not None
                and name in {"open_entity_message", "open_screen_message"}
            )
            if comment and not localized_message:
                lines.append(f"{prefix}// {comment}")
            if (
                localized_message
                and command.arguments
                and command.arguments[-1].kind == "literal"
            ):
                message_id = int(command.arguments[-1].value)
                message = self.dialogue_catalog.get(self.room_id, message_id)
                if message is None:
                    lines.append(
                        f"{prefix}// [Nachricht] Keine Katalogzeile für Raum "
                        f"{self.room_id}, ID {message_id}."
                    )
                else:
                    language_label = {
                        "german": "DE",
                        "english": "EN – keine deutsche Fassung im ROM",
                        "japanese": "JP – keine deutsche Fassung im ROM",
                    }.get(message.language, message.language)
                    event_label = (
                        f" · {message.event_label}" if message.event_label else ""
                    )
                    lines.append(
                        f"{prefix}// [Nachricht {language_label}{event_label}]"
                    )
                    for message_line in _message_comment_lines(message):
                        lines.append(f"{prefix}// {message_line}")
            semantics = self.semantics.get(command.opcode, {})
            parameter_names = semantics.get("arguments", [])
            rendered_arguments = []
            for argument_index, argument in enumerate(command.arguments):
                parameter = (
                    parameter_names[argument_index]
                    if argument_index < len(parameter_names)
                    else ""
                )
                rendered_arguments.append(
                    _format_value(argument, "status_id" in parameter)
                )
            assignment = (
                f"{_format_value(command.result)} = " if command.result else ""
            )
            suffix = (
                f" @mode(0x{command.unused_mode_bits:04X})"
                if command.unused_mode_bits
                else ""
            )
            lines.append(
                f"{prefix}{assignment}{self.api_names[command.opcode]}("
                + ", ".join(rendered_arguments)
                + f"){suffix};"
            )
            index += 1
            cursor = command.end
        if cursor < expected_end:
            gap = expected_end - cursor
            if gap & 1:
                raise PitLanguageError("odd-byte tail gap in structured source range")
            prefix = " " * indent
            lines.append(
                f"{prefix}// Preserve {gap // 2} opaque halfword(s) from the private binary source."
            )
            lines.append(f"{prefix}padding {gap // 2};")
        elif cursor > expected_end:
            raise PitLanguageError("structured source range exceeds its declared boundary")
        return lines

    def find_counted_loop(
        self,
        commands: list[Instruction],
        index: int,
        stop: int,
        inbound: dict[int, list[int]],
    ) -> tuple[int, int, int] | None:
        if index + 2 >= stop or commands[index].opcode != 0x0A:
            return None
        loop = commands[index + 1]
        if loop.opcode != 0x0C or len(loop.arguments) != 4:
            return None
        if _literal(loop.arguments[0]) != 0x12 or _literal(loop.arguments[1]) != 0:
            return None
        if loop.arguments[3].kind != "label":
            return None
        exit_label = str(loop.arguments[3].value)
        exit_index = next(
            (
                candidate
                for candidate in range(index + 2, stop + 1)
                if candidate < stop
                and exit_label in commands[candidate].labels
            ),
            None,
        )
        if exit_index is None or exit_index <= index + 2:
            return None
        jump_index = exit_index - 1
        jump = commands[jump_index]
        if (
            jump.opcode != 0x02
            or len(jump.arguments) != 2
            or _literal(jump.arguments[0]) != 2
            or jump.arguments[1].kind != "label"
            or str(jump.arguments[1].value) not in loop.labels
        ):
            return None
        # Hiding the loop opcode and back edge is safe only when the back edge
        # is their sole incoming control-flow reference and neither is a root.
        if any(label.startswith("script_") for label in loop.labels + jump.labels):
            return None
        if inbound.get(loop.offset, []) != [jump.offset]:
            return None
        if any(
            source < loop.end or source >= jump.offset
            for source in inbound.get(jump.offset, [])
        ):
            return None
        return index + 2, jump_index, exit_index


def decompile_json_to_script(json_data: dict) -> str:
    """Translate any supported field, battle, or scene JSON document."""
    if isinstance(json_data, dict) and json_data.get("schema") == ROOM_SCHEMA:
        return Decompiler(json_data).run()
    # Lazy import avoids a module cycle: the extra dialects deliberately reuse
    # this module's lexer and statement AST.
    import pit_vm_language

    return pit_vm_language.decompile_vm_json_to_script(json_data)


def decompile_json_to_script_with_messages(
    json_data: dict,
    dialogue_data: dict,
    language: str = "german",
) -> str:
    """Translate a room and embed private localized dialogue as comments."""
    return Decompiler(
        json_data, DialogueCatalog.from_json(dialogue_data, language)
    ).run()


@dataclass
class LabelMarker:
    name: str


LoweredItem = LabelMarker | Instruction | PaddingItem


class Lowerer:
    def __init__(
        self,
        descriptors: tuple[int, ...],
        names: dict[int, str],
        reverse_api_names: dict[str, int],
    ) -> None:
        self.descriptors = descriptors
        self.names = names
        self.reverse_api_names = reverse_api_names
        self.loop_serial = 0

    def lower(self, statements: Iterable[Statement]) -> list[LoweredItem]:
        result: list[LoweredItem] = []
        for statement in statements:
            if isinstance(statement, LabelStatement):
                result.append(LabelMarker(statement.name))
            elif isinstance(statement, CallStatement):
                result.append(self.lower_call(statement))
            elif isinstance(statement, AsyncStatement):
                result.extend(self.lower_async(statement))
            elif isinstance(statement, ForStatement):
                result.extend(self.lower_for(statement))
            elif isinstance(statement, PaddingStatement):
                result.append(PaddingItem(statement.halfwords))
            else:  # pragma: no cover - exhaustiveness guard
                raise AssertionError(f"unknown AST statement {statement!r}")
        return result

    def lower_call(self, statement: CallStatement) -> Instruction:
        opcode = self.reverse_api_names.get(statement.api_name)
        if opcode is None:
            raise PitLanguageError(f"unknown VM method {statement.api_name}")
        if opcode in INLINE_OPCODES:
            raise PitLanguageError(
                f"{statement.api_name} must use the scoped 'async' form"
            )
        self.validate_contract(
            opcode,
            statement.result,
            statement.arguments,
            statement.unused_mode_bits,
            statement.api_name,
        )
        return Instruction(
            opcode,
            statement.result,
            list(statement.arguments),
            statement.unused_mode_bits,
        )

    def lower_async(self, statement: AsyncStatement) -> list[LoweredItem]:
        opcode = 0x3D if statement.wait else 0x3C
        body = self.lower(statement.body)
        body_bytes = sum(
            _instruction_size(item.opcode, self.descriptors)
            for item in body
            if isinstance(item, Instruction)
        ) + sum(
            item.halfwords * 2 for item in body if isinstance(item, PaddingItem)
        )
        if body_bytes & 1:
            raise AssertionError("field instructions must have even byte sizes")
        halfwords = body_bytes // 2
        if halfwords > 0x7FFF:
            raise PitLanguageError("async body is too large for a signed halfword size")
        arguments = [
            statement.entity_selector,
            statement.chain_if_active,
            Value.literal(halfwords),
        ]
        self.validate_contract(opcode, None, arguments, 0, "async Actor.Run")
        return [Instruction(opcode, None, arguments), *body]

    def lower_for(self, statement: ForStatement) -> list[LoweredItem]:
        serial = self.loop_serial
        self.loop_serial += 1
        head = f"__loop_{serial}_head"
        exit_label = f"__loop_{serial}_exit"
        body = self.lower(statement.body)
        push = Instruction(0x0A, None, [statement.initial])
        loop = Instruction(
            0x0C,
            None,
            [
                Value.literal(0x12),
                Value.literal(0),
                statement.limit,
                Value.label(exit_label),
            ],
        )
        jump = Instruction(
            0x02, None, [Value.literal(2), Value.label(head)]
        )
        self.validate_contract(0x0A, None, push.arguments, 0, "for initializer")
        self.validate_contract(0x0C, None, loop.arguments, 0, "for condition")
        self.validate_contract(0x02, None, jump.arguments, 0, "for back edge")
        return [
            push,
            LabelMarker(head),
            loop,
            *body,
            jump,
            LabelMarker(exit_label),
        ]

    def validate_contract(
        self,
        opcode: int,
        result: Value | None,
        arguments: Sequence[Value],
        unused_mode_bits: int,
        context: str,
    ) -> None:
        descriptor = self.descriptors[opcode]
        expected_count = descriptor & 0x1F
        if len(arguments) != expected_count:
            raise PitLanguageError(
                f"{context} expects {expected_count} arguments, got {len(arguments)}"
            )
        if bool(descriptor & 0x20) != (result is not None):
            requirement = "requires" if descriptor & 0x20 else "does not return"
            raise PitLanguageError(f"{context} {requirement} a result variable")
        if result is not None and result.kind != "variable":
            raise PitLanguageError(f"{context} result must be a VM variable")
        if not 0 <= unused_mode_bits <= 0xFFFF:
            raise PitLanguageError(f"{context} mode mask must fit in 16 bits")
        if unused_mode_bits and not (descriptor & 0x40 and expected_count):
            raise PitLanguageError(f"{context} does not encode an argument mode mask")
        used_mask = 0
        for index, argument in enumerate(arguments):
            if argument.kind == "variable":
                if not descriptor & 0x40:
                    raise PitLanguageError(
                        f"{context} argument {index} cannot be a variable; opcode "
                        "descriptor bit 6 is clear"
                    )
                used_mask |= 1 << index
            elif argument.kind == "literal":
                if not -0x8000 <= int(argument.value) <= 0x7FFF:
                    raise PitLanguageError(
                        f"{context} argument {index} does not fit signed 16 bits"
                    )
            elif argument.kind != "label":
                raise PitLanguageError(f"{context} argument {index} is invalid")
        if unused_mode_bits & ((1 << expected_count) - 1):
            raise PitLanguageError(
                f"{context} @mode bits overlap the {expected_count} argument mode bits"
            )
        if used_mask & unused_mode_bits:
            raise PitLanguageError(f"{context} has overlapping mode bits")


class Compiler:
    def __init__(self, script_text: str) -> None:
        self.program = Parser(script_text).parse_program()
        self.descriptors, self.names, _ = _load_schema()
        _, reverse = _api_maps(self.names)
        self.lowerer = Lowerer(self.descriptors, self.names, reverse)

    def run(self) -> dict[str, Any]:
        if "room_id" in self.program.metadata or "members" in self.program.metadata:
            raise PitLanguageError(
                "room metadata cannot contain compiler-owned room_id or members"
            )
        document = copy.deepcopy(self.program.metadata)
        if document.get("schema") != ROOM_SCHEMA:
            raise PitLanguageError(
                f"room metadata must declare schema {ROOM_SCHEMA!r}"
            )
        document["room_id"] = self.program.room_id
        document["members"] = [
            self.compile_member(member, index)
            for index, member in enumerate(self.program.members)
        ]
        return document

    def compile_member(self, member: Member, member_index: int) -> dict[str, Any]:
        forbidden = {
            "room_id",
            "room_part",
            "commands",
            "embedded_data_records",
            "reachable_command_count",
            "embedded_data_record_count",
        }
        overlap = forbidden & member.metadata.keys()
        if overlap:
            raise PitLanguageError(
                f"member {member_index} metadata contains compiler-owned fields: "
                + ", ".join(sorted(overlap))
            )
        if member.room_part not in {0, 2}:
            raise PitLanguageError(
                f"member {member_index}: field-event room part must be 0 or 2"
            )
        entry_size = member.metadata.get("source_entry_size")
        if isinstance(entry_size, bool) or not isinstance(entry_size, int) or entry_size < 0:
            raise PitLanguageError(
                f"member {member_index}: source_entry_size must be a non-negative integer"
            )
        pointer_table_size = member.metadata.get("pointer_table_size")
        if (
            isinstance(pointer_table_size, bool)
            or not isinstance(pointer_table_size, int)
            or pointer_table_size < 0
            or pointer_table_size & 1
        ):
            raise PitLanguageError(
                f"member {member_index}: pointer_table_size must be an even integer"
            )

        record_rows, data_offsets, data_ranges = self.compile_data_records(
            member, member_index, entry_size
        )
        label_offsets: dict[str, int] = dict(data_offsets)
        instructions: list[Instruction] = []
        section_ranges: list[tuple[int, int]] = []
        source_labels: dict[int, list[str]] = {}

        previous_anchor = -1
        for section_index, section in enumerate(member.sections):
            if section.source_offset <= previous_anchor:
                raise PitLanguageError(
                    f"member {member_index}: section anchors must be strictly increasing"
                )
            previous_anchor = section.source_offset
            if section.source_offset < pointer_table_size:
                raise PitLanguageError(
                    f"member {member_index} section {section_index} starts inside the pointer table"
                )
            items = self.lowerer.lower(section.body)
            cursor = section.source_offset
            pending: list[str] = []
            section_instructions: list[Instruction] = []
            for item in items:
                if isinstance(item, LabelMarker):
                    if item.name in label_offsets:
                        raise PitLanguageError(
                            f"member {member_index}: duplicate label {item.name}"
                        )
                    label_offsets[item.name] = cursor
                    pending.append(item.name)
                    continue
                if isinstance(item, PaddingItem):
                    if pending:
                        raise PitLanguageError(
                            f"member {member_index}: labels cannot target opaque padding"
                        )
                    cursor += item.halfwords * 2
                    continue
                item.offset = cursor
                item.end = cursor + _instruction_size(item.opcode, self.descriptors)
                item.labels = pending
                if pending:
                    source_labels.setdefault(cursor, []).extend(pending)
                pending = []
                cursor = item.end
                section_instructions.append(item)
            if pending:
                raise PitLanguageError(
                    f"member {member_index}: labels must precede an instruction"
                )
            if cursor > entry_size:
                raise PitLanguageError(
                    f"member {member_index} section {section_index} ends beyond source_entry_size"
                )
            section_ranges.append((section.source_offset, cursor))
            instructions.extend(section_instructions)

        for left, right in zip(section_ranges, section_ranges[1:]):
            if left[1] > right[0]:
                raise PitLanguageError(
                    f"member {member_index}: relocated sections overlap at {_format_offset(right[0])}"
                )
        for section_start, section_end in section_ranges:
            for data_start, data_end, label in data_ranges:
                if max(section_start, data_start) < min(section_end, data_end):
                    raise PitLanguageError(
                        f"member {member_index}: code overlaps embedded record {label}"
                    )

        code_targets: dict[int, list[int]] = {}
        data_record_for_instruction: dict[int, str] = {}
        resolved_arguments: dict[int, list[Value]] = {}
        for instruction in instructions:
            resolved, targets, data_record = self.resolve_arguments(
                instruction,
                label_offsets,
                data_offsets,
                member_index,
            )
            resolved_arguments[instruction.offset] = resolved
            if instruction.opcode in INLINE_OPCODES:
                size = _literal(resolved[2])
                assert size is not None
                targets = [instruction.end, instruction.end + size * 2]
            if targets:
                code_targets[instruction.offset] = targets
            if data_record is not None:
                data_record_for_instruction[instruction.offset] = data_record

        instruction_offsets = {instruction.offset for instruction in instructions}
        local_target_offsets: set[int] = set()
        for source_offset, targets in code_targets.items():
            for target in targets:
                if target not in instruction_offsets:
                    raise PitLanguageError(
                        f"member {member_index}: command {_format_offset(source_offset)} "
                        f"targets non-instruction boundary {_format_offset(target)}"
                    )
                local_target_offsets.add(target)

        command_rows: list[dict[str, Any]] = []
        for instruction in sorted(instructions, key=lambda item: item.offset):
            descriptor = self.descriptors[instruction.opcode]
            row: dict[str, Any] = {
                "source_offset": _format_offset(instruction.offset),
                "opcode": field_event_mod.opcode_name(instruction.opcode, self.names),
            }
            if instruction.result is not None:
                row["result"] = self.variable_name(
                    instruction.result,
                    f"member {member_index} command {_format_offset(instruction.offset)} result",
                )
            row["args"] = [
                self.argument_json(
                    argument,
                    f"member {member_index} command {_format_offset(instruction.offset)} argument {argument_index}",
                )
                for argument_index, argument in enumerate(
                    resolved_arguments[instruction.offset]
                )
            ]
            if instruction.unused_mode_bits:
                if not descriptor & 0x40:
                    raise PitLanguageError("internal mode-mask contract failure")
                row["unused_mode_bits"] = f"0x{instruction.unused_mode_bits:04X}"
            labels = [
                label
                for label in source_labels.get(instruction.offset, [])
                if not label.startswith("loc_") and not label.startswith("__loop_")
            ]
            if instruction.offset in local_target_offsets:
                labels.append(f"loc_{instruction.offset:04X}")
            labels = list(dict.fromkeys(labels))
            if labels:
                row["labels"] = labels
            targets = code_targets.get(instruction.offset)
            if targets:
                row["code_targets"] = [_format_offset(target) for target in targets]
            data_record = data_record_for_instruction.get(instruction.offset)
            if data_record:
                row["data_record"] = data_record
            command_rows.append(row)

        # Every pointer-table script slot must still name a declared command.
        declared_command_labels = {
            label
            for row in command_rows
            for label in row.get("labels", [])
        }
        for slot_index, slot in enumerate(member.metadata.get("script_slots", [])):
            if isinstance(slot, str) and slot not in declared_command_labels:
                raise PitLanguageError(
                    f"member {member_index}: script slot {slot_index} references missing label {slot}"
                )
            if isinstance(slot, dict) and set(slot) == {"data_record"}:
                if slot["data_record"] not in data_offsets:
                    raise PitLanguageError(
                        f"member {member_index}: script slot {slot_index} references missing data record"
                    )

        row = copy.deepcopy(member.metadata)
        row["room_id"] = self.program.room_id
        row["room_part"] = member.room_part
        row["reachable_command_count"] = len(command_rows)
        row["embedded_data_record_count"] = len(record_rows)
        row["embedded_data_records"] = record_rows
        row["commands"] = command_rows
        return row

    def compile_data_records(
        self, member: Member, member_index: int, entry_size: int
    ) -> tuple[list[dict[str, Any]], dict[str, int], list[tuple[int, int, str]]]:
        rows: list[dict[str, Any]] = []
        offsets: dict[str, int] = {}
        ranges: list[tuple[int, int, str]] = []
        previous_offset = -1
        for record_index, record in enumerate(member.data_records):
            context = f"member {member_index} data record {record_index}"
            if record.label in offsets:
                raise PitLanguageError(f"{context}: duplicate label {record.label}")
            if record.source_offset <= previous_offset:
                raise PitLanguageError(
                    f"{context}: data records must be in source-offset order"
                )
            previous_offset = record.source_offset
            row = {
                "label": record.label,
                "source_offset": _format_offset(record.source_offset),
                **copy.deepcopy(record.payload),
            }
            expected_keys = {
                "entity_roaming_profile": {
                    "label",
                    "source_offset",
                    "type",
                    "size_words",
                    "legacy_parameter",
                    "speed_q12",
                    "step_distance",
                    "post_step_delay_frames",
                    "direction_count",
                },
                "entity_waypoint_path": {
                    "label",
                    "source_offset",
                    "type",
                    "size_words",
                    "ping_pong",
                    "relative_coordinates",
                    "random_direction",
                    "post_segment_delay_frames",
                    "waypoints",
                },
            }.get(row.get("type"))
            if expected_keys is None or set(row) != expected_keys:
                raise PitLanguageError(
                    f"{context}: invalid fields for embedded type {row.get('type')!r}"
                )
            try:
                encoded = field_event_mod.compile_embedded_data_record(
                    row, row, context
                )
            except Exception as exc:
                raise PitLanguageError(str(exc)) from exc
            end = record.source_offset + len(encoded)
            if end > entry_size:
                raise PitLanguageError(f"{context}: record exceeds source_entry_size")
            if ranges and ranges[-1][1] > record.source_offset:
                raise PitLanguageError(f"{context}: embedded data records overlap")
            offsets[record.label] = record.source_offset
            ranges.append((record.source_offset, end, record.label))
            rows.append(row)
        return rows, offsets, ranges

    def resolve_arguments(
        self,
        instruction: Instruction,
        label_offsets: dict[str, int],
        data_offsets: dict[str, int],
        member_index: int,
    ) -> tuple[list[Value], list[int], str | None]:
        context = (
            f"member {member_index} command {_format_offset(instruction.offset)}"
        )
        code_index = CODE_REFERENCE_ARGUMENTS.get(instruction.opcode)
        data_index = DATA_REFERENCE_ARGUMENTS.get(instruction.opcode)
        result: list[Value] = []
        targets: list[int] = []
        data_record: str | None = None
        for index, argument in enumerate(instruction.arguments):
            if argument.kind != "label":
                result.append(argument)
                continue
            label = str(argument.value)
            if index == code_index:
                if label not in label_offsets or label in data_offsets:
                    raise PitLanguageError(
                        f"{context} references unknown code label {label}"
                    )
                target = label_offsets[label]
                targets.append(target)
            elif index == data_index:
                if label not in data_offsets:
                    raise PitLanguageError(
                        f"{context} references unknown embedded-data label {label}"
                    )
                target = data_offsets[label]
                data_record = label
            else:
                raise PitLanguageError(
                    f"{context} argument {index} is not a relocatable reference"
                )
            delta_bytes = target - instruction.end
            if delta_bytes & 1:
                raise PitLanguageError(f"{context} target {label} is not halfword aligned")
            displacement = delta_bytes // 2
            if not -0x8000 <= displacement <= 0x7FFF:
                raise PitLanguageError(f"{context} target {label} is out of range")
            result.append(Value.literal(displacement))
        return result, targets, data_record

    @staticmethod
    def variable_name(value: Value, context: str) -> str:
        if value.kind != "variable":
            raise PitLanguageError(f"{context} must be a variable")
        try:
            raw = field_event_mod.parse_variable(str(value.value), context)
        except Exception as exc:
            raise PitLanguageError(str(exc)) from exc
        return field_event_mod.format_variable(raw)

    def argument_json(self, value: Value, context: str) -> Any:
        if value.kind == "variable":
            return {"variable": self.variable_name(value, context)}
        if value.kind != "literal":
            raise PitLanguageError(f"{context} contains an unresolved label")
        literal = int(value.value)
        if not -0x8000 <= literal <= 0x7FFF:
            raise PitLanguageError(f"{context} does not fit signed 16 bits")
        return literal


def compile_script_to_json(script_text: str) -> dict:
    """Parse PiT source and rebuild the matching VM JSON document."""
    tokens = _tokens(script_text)
    root = tokens[0].text
    if root == "room":
        return Compiler(script_text).run()
    if root in {"battle", "scene"}:
        import pit_vm_language

        return pit_vm_language.compile_vm_script_to_json(script_text)
    token = tokens[0]
    raise PitLanguageError(
        f"line {token.line}, column {token.column}: expected 'room', 'battle', or 'scene'"
    )


def _write_text_atomic(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + ".tmp")
    temporary.write_text(text, encoding="utf-8", newline="\n")
    temporary.replace(path)


def decompile_room_corpus(
    rooms_directory: Path,
    dialogue_data: dict,
    output_directory: Path,
    language: str = "german",
) -> dict[str, Any]:
    """Generate a private editable source tree from every room JSON shard."""
    if not rooms_directory.is_dir():
        raise PitLanguageError(f"room directory does not exist: {rooms_directory}")
    catalog = DialogueCatalog.from_json(dialogue_data, language)
    room_paths = sorted(rooms_directory.glob("room_*.json"))
    if not room_paths:
        raise PitLanguageError(f"no room_*.json files found in {rooms_directory}")
    manifest_rows: list[dict[str, Any]] = []
    seen_room_ids: set[int] = set()
    message_reference_count = 0
    for path in room_paths:
        try:
            document = json.loads(path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as exc:
            raise PitLanguageError(f"cannot read {path}: {exc}") from exc
        room_id = document.get("room_id") if isinstance(document, dict) else None
        match = re.fullmatch(r"room_([0-9]+)\.json", path.name)
        if (
            match is None
            or isinstance(room_id, bool)
            or not isinstance(room_id, int)
            or int(match.group(1)) != room_id
        ):
            raise PitLanguageError(f"room filename/room_id mismatch: {path}")
        if room_id in seen_room_ids:
            raise PitLanguageError(f"duplicate room ID {room_id} in corpus")
        seen_room_ids.add(room_id)
        source = Decompiler(document, catalog).run()
        message_reference_count += source.count("// [Nachricht ")
        output_name = f"room_{room_id:03d}.pit"
        output_path = output_directory / output_name
        _write_text_atomic(output_path, source)
        manifest_rows.append(
            {
                "room_id": room_id,
                "source_json": path.name,
                "script": output_name,
                "source_json_sha256": hashlib.sha256(
                    path.read_bytes()
                ).hexdigest(),
            }
        )
    manifest = {
        "schema": CORPUS_SCHEMA,
        "version": DEFAULT_VERSION,
        "language": language,
        "room_count": len(manifest_rows),
        "message_reference_count": message_reference_count,
        "rooms": manifest_rows,
    }
    _write_text_atomic(
        output_directory / "corpus.json",
        json.dumps(manifest, ensure_ascii=False, indent=2) + "\n",
    )
    _write_text_atomic(
        output_directory / "README.md",
        "# Privater PiT-Feldskriptkorpus\n\n"
        "Die `.pit`-Dateien sind die bearbeitbare Hochsprachendarstellung der "
        "lokalen `pit-field-event-room-v1`-Quellen. Dialogzeilen stehen nur als "
        "Kommentare im Quelltext und verändern beim Kompilieren keine Nachricht.\n\n"
        "Kompilieren in den lokalen Daten-Mod-Baum:\n\n"
        "```powershell\n"
        "python .\\tools\\pit_language_compiler.py compile-corpus `\n"
        f"  {output_directory} `\n"
        "  .\\data\\eur\\scripts\\FEvent__FEvData.dat\n"
        "```\n",
    )
    return manifest


def compile_room_corpus(
    input_directory: Path, output_directory: Path
) -> dict[str, Any]:
    """Compile every source named by a private corpus manifest."""
    manifest_path = input_directory / "corpus.json"
    try:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise PitLanguageError(f"cannot read {manifest_path}: {exc}") from exc
    if not isinstance(manifest, dict) or manifest.get("schema") != CORPUS_SCHEMA:
        raise PitLanguageError(
            f"corpus manifest must use schema {CORPUS_SCHEMA!r}"
        )
    rooms = manifest.get("rooms")
    if not isinstance(rooms, list) or manifest.get("room_count") != len(rooms):
        raise PitLanguageError("corpus manifest has an invalid room list/count")
    seen: set[int] = set()
    output_rows: list[dict[str, Any]] = []
    for index, row in enumerate(rooms):
        if not isinstance(row, dict):
            raise PitLanguageError(f"corpus room {index} must be an object")
        room_id = row.get("room_id")
        script_name = row.get("script")
        source_json = row.get("source_json")
        if (
            isinstance(room_id, bool)
            or not isinstance(room_id, int)
            or room_id in seen
            or not isinstance(script_name, str)
            or Path(script_name).name != script_name
            or not re.fullmatch(r"room_[0-9]+\.pit", script_name)
            or not isinstance(source_json, str)
            or Path(source_json).name != source_json
            or not re.fullmatch(r"room_[0-9]+\.json", source_json)
        ):
            raise PitLanguageError(f"corpus room {index} has invalid paths/ID")
        seen.add(room_id)
        script_path = input_directory / script_name
        try:
            source = script_path.read_text(encoding="utf-8")
        except OSError as exc:
            raise PitLanguageError(f"cannot read {script_path}: {exc}") from exc
        document = compile_script_to_json(source)
        if document.get("room_id") != room_id:
            raise PitLanguageError(
                f"{script_path} compiles as room {document.get('room_id')}, expected {room_id}"
            )
        output_path = output_directory / source_json
        _write_text_atomic(
            output_path,
            json.dumps(document, ensure_ascii=False, indent=2) + "\n",
        )
        output_rows.append(
            {
                "room_id": room_id,
                "script": script_name,
                "output_json": source_json,
            }
        )
    return {
        "schema": CORPUS_SCHEMA,
        "room_count": len(output_rows),
        "rooms": output_rows,
    }


def _build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Compile or decompile PiT field, battle, and scene VM scripts"
    )
    subparsers = parser.add_subparsers(dest="action", required=True)
    decompile = subparsers.add_parser(
        "decompile", help="translate supported PiT VM JSON to .pit source"
    )
    decompile.add_argument("input", type=Path)
    decompile.add_argument("output", type=Path)
    compile_parser = subparsers.add_parser(
        "compile", help="translate field, battle, or scene .pit source to JSON"
    )
    compile_parser.add_argument("input", type=Path)
    compile_parser.add_argument("output", type=Path)
    corpus_decompile = subparsers.add_parser(
        "decompile-corpus",
        help="generate private .pit sources for every room with localized comments",
    )
    corpus_decompile.add_argument("rooms", type=Path)
    corpus_decompile.add_argument("dialogue", type=Path)
    corpus_decompile.add_argument("output", type=Path)
    corpus_decompile.add_argument("--language", default="german")
    corpus_compile = subparsers.add_parser(
        "compile-corpus", help="compile a private .pit corpus back to room JSON"
    )
    corpus_compile.add_argument("input", type=Path)
    corpus_compile.add_argument("output", type=Path)
    all_decompile = subparsers.add_parser(
        "decompile-all-vms",
        help="generate one private source corpus for field, battle, and scene VMs",
    )
    all_decompile.add_argument("scripts", type=Path)
    all_decompile.add_argument("text", type=Path)
    all_decompile.add_argument("output", type=Path)
    all_decompile.add_argument("--language", default="german")
    all_compile = subparsers.add_parser(
        "compile-all-vms",
        help="compile a private all-VM source corpus back to JSON",
    )
    all_compile.add_argument("input", type=Path)
    all_compile.add_argument("output", type=Path)
    return parser


def main(argv: Sequence[str] | None = None) -> int:
    arguments = _build_argument_parser().parse_args(argv)
    try:
        if arguments.action == "decompile":
            source = json.loads(arguments.input.read_text(encoding="utf-8"))
            output = decompile_json_to_script(source)
            arguments.output.parent.mkdir(parents=True, exist_ok=True)
            arguments.output.write_text(output, encoding="utf-8", newline="\n")
        elif arguments.action == "compile":
            output = json.dumps(
                compile_script_to_json(
                    arguments.input.read_text(encoding="utf-8")
                ),
                ensure_ascii=False,
                indent=2,
            ) + "\n"
            _write_text_atomic(arguments.output, output)
        elif arguments.action == "decompile-corpus":
            dialogue = json.loads(arguments.dialogue.read_text(encoding="utf-8"))
            manifest = decompile_room_corpus(
                arguments.rooms,
                dialogue,
                arguments.output,
                arguments.language,
            )
            print(
                f"Generated {manifest['room_count']} private room sources with "
                f"{manifest['message_reference_count']} localized message references "
                f"in {arguments.output}"
            )
        elif arguments.action == "compile-corpus":
            result = compile_room_corpus(arguments.input, arguments.output)
            print(
                f"Compiled {result['room_count']} private room sources into "
                f"{arguments.output}"
            )
        elif arguments.action == "decompile-all-vms":
            import pit_vm_language

            manifest = pit_vm_language.decompile_all_vm_corpus(
                arguments.scripts,
                arguments.text,
                arguments.output,
                arguments.language,
            )
            print(
                "Generated all-VM private corpus: "
                f"{manifest['field']['room_count']} field rooms, "
                f"{manifest['battle']['archive_count']} battle archives, and "
                f"{manifest['scene']['archive_count']} scene archives in "
                f"{arguments.output}"
            )
        else:
            import pit_vm_language

            result = pit_vm_language.compile_all_vm_corpus(
                arguments.input, arguments.output
            )
            print(
                "Compiled all-VM private corpus: "
                f"{result['field_room_count']} field rooms, "
                f"{result['battle_archive_count']} battle archives, and "
                f"{result['scene_archive_count']} scene archives into "
                f"{arguments.output}"
            )
    except (OSError, json.JSONDecodeError, PitLanguageError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
