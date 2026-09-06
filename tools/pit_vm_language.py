#!/usr/bin/env python3
"""High-level source frontend for the PiT battle and scene script VMs.

This module complements :mod:`pit_language_compiler`, whose original room
frontend remains the field-VM implementation.  Battle and scene scripts share
the same expression and statement language, but retain their native archive
containers and relocation contracts:

* battle entries are relocatable sequences of code/private-data segments;
* scene entries retain fixed section anchors because their current public
  binary backend deliberately preserves unclassified bytes in place.

The module is imported lazily by ``pit_language_compiler.py``.  Keeping the
dialects here makes their different variable namespaces and control-flow
operands explicit without duplicating the lexer or statement AST.
"""

from __future__ import annotations

import copy
from dataclasses import dataclass
import hashlib
import json
from pathlib import Path
import re
from typing import Any, Callable, Iterable, Sequence

import data_mod
import field_event_mod
import pit_language_compiler as core
import scene_script_mod


BATTLE_SCHEMA = data_mod.BATTLE_SCRIPT_SCHEMA
SCENE_SCHEMA = scene_script_mod.SCHEMA
ALL_VM_CORPUS_SCHEMA = "pit-all-vm-source-corpus-v1"


@dataclass(frozen=True)
class VmDialect:
    key: str
    schema: str
    descriptors: tuple[int, ...]
    names: dict[int, str]
    semantics: dict[int, dict[str, Any]]
    format_variable: Callable[[int], str]
    parse_variable: Callable[[Any, str], int]
    code_references: dict[int, int]
    data_references: dict[int, tuple[int, int]]
    inline_opcodes: dict[int, int]
    spawn_opcodes: dict[int, int]
    wait_skip_opcodes: dict[int, int]
    branch_with_fallthrough: frozenset[int]
    terminal_opcodes: frozenset[int]

    @property
    def api_names(self) -> dict[int, str]:
        return _api_maps(self)[0]

    @property
    def reverse_api_names(self) -> dict[str, int]:
        return _api_maps(self)[1]

    def parse_opcode(self, value: Any, context: str) -> int:
        try:
            if self.key == "battle":
                return data_mod._parse_battle_opcode(
                    value, self.descriptors, self.names, context
                )
            return field_event_mod.parse_opcode(
                value, self.descriptors, self.names, context
            )
        except Exception as exc:
            raise core.PitLanguageError(str(exc)) from exc

    def opcode_name(self, opcode: int) -> str:
        if self.key == "battle":
            return data_mod._battle_opcode_name(opcode, self.names)
        return field_event_mod.opcode_name(opcode, self.names)


def _load_semantics(filename: str) -> dict[int, dict[str, Any]]:
    path = Path(__file__).resolve().parents[1] / "config" / "eur" / filename
    document = json.loads(path.read_text(encoding="utf-8"))
    return {
        int(raw_opcode, 0): row
        for raw_opcode, row in document.get("opcode_semantics", {}).items()
        if isinstance(row, dict)
    }


def load_dialect(kind: str) -> VmDialect:
    """Load one checked VM ABI from the repository descriptor tables."""
    if kind == "battle":
        descriptors, names = data_mod.load_battle_vm_schema("eur")
        return VmDialect(
            key="battle",
            schema=BATTLE_SCHEMA,
            descriptors=descriptors,
            names=names,
            semantics=_load_semantics("battle_ai_vm.json"),
            format_variable=data_mod.format_vm_variable,
            parse_variable=data_mod.parse_vm_variable,
            code_references=dict(data_mod.VM_CODE_REFERENCE_ARGUMENTS),
            data_references=dict(data_mod.VM_DATA_REFERENCE_ARGUMENTS),
            inline_opcodes={0xD1: 2},
            spawn_opcodes={0x73: 2, 0xD2: 2, 0xD4: 2},
            wait_skip_opcodes={0xD3: 1},
            branch_with_fallthrough=frozenset(
                data_mod.VM_BRANCH_WITH_FALLTHROUGH
            ),
            terminal_opcodes=frozenset({0x00, 0x01}),
        )
    if kind == "scene":
        descriptors, names = scene_script_mod.load_vm_schema("eur")
        return VmDialect(
            key="scene",
            schema=SCENE_SCHEMA,
            descriptors=descriptors,
            names=names,
            semantics=_load_semantics("scene_vm.json"),
            format_variable=scene_script_mod.format_variable,
            parse_variable=scene_script_mod.parse_variable,
            code_references={
                **scene_script_mod.CORE_CODE_REFERENCES,
                **scene_script_mod.SCENE_BRANCH_REFERENCES,
            },
            data_references={},
            inline_opcodes=dict(scene_script_mod.INLINE_SCRIPT_OPCODES),
            spawn_opcodes=dict(scene_script_mod.SPAWN_SCRIPT_OPCODES),
            wait_skip_opcodes=dict(scene_script_mod.WAIT_AND_SKIP_OPCODES),
            branch_with_fallthrough=frozenset(
                scene_script_mod.BRANCH_WITH_FALLTHROUGH
            ),
            terminal_opcodes=frozenset(scene_script_mod.TERMINAL_OPCODES),
        )
    raise core.PitLanguageError(f"unknown VM dialect {kind!r}")


_EXPLICIT_APIS = {
    # Structured occurrences are emitted as ``async Actor.Run``.  A few
    # shipped Battle-AI references point into still-private bytes, so the raw
    # spelling remains available for those not-yet-decodable child bodies.
    "start_inline_object_script": "Actor.StartInlineScriptRaw",
    "start_object_script": "Actor.StartScript",
    "start_object_script_and_yield": "Actor.StartScriptAndYield",
    "wait_object_script_and_skip": "Actor.WaitScriptAndSkip",
    "wait_object_script": "Actor.WaitScript",
    "apply_status": "Actor.ApplyStatus",
    "clear_status": "Actor.ClearStatus",
    "set_object_animation": "Actor.SetAnimation",
    "activate_and_set_object_property": "Actor.SetAnimation",
    "bind_object_resource": "Actor.BindResource",
    "bind_entity_resource": "Actor.BindResource",
    "move_object": "Actor.Move",
    "move_object_at_speed": "Actor.MoveAtSpeed",
}


def _candidate_api_name(dialect: VmDialect, name: str) -> str:
    explicit = _EXPLICIT_APIS.get(name)
    if explicit is not None:
        return explicit
    words = name.split("_")
    if name in {"jump", "jump_if", "push", "loop", "return", "end"}:
        namespace, removed = "Flow", set()
    elif any(word in words for word in ("actor", "enemy", "object")):
        namespace, removed = "Actor", {"actor", "enemy", "object"}
    elif "camera" in words:
        namespace, removed = "Camera", {"camera"}
    elif any(word in words for word in ("sound", "music", "bgm")):
        namespace, removed = "Audio", {"sound", "music"}
    elif any(word in words for word in ("effect", "sprite", "model")):
        namespace, removed = "Effect", set()
    elif "status" in words:
        namespace, removed = "Status", {"status"}
    elif "battle" in words or dialect.key == "battle" and "global" in words:
        namespace, removed = "Battle", {"battle"}
    elif any(word in words for word in ("screen", "window", "text")):
        namespace, removed = "Screen", {"screen"}
    elif any(word in words for word in ("menu", "input")):
        namespace, removed = "Menu", {"menu"}
    elif any(word in words for word in ("save", "item", "coin", "bean")):
        namespace, removed = "Save", set()
    else:
        namespace, removed = "VM", set()
    method_words = [word for word in words if word not in removed]
    method = core._pascal_case("_".join(method_words))
    return f"{namespace}.{method}"


def _api_maps(dialect: VmDialect) -> tuple[dict[int, str], dict[str, int]]:
    forward = {
        opcode: _candidate_api_name(dialect, name)
        for opcode, name in dialect.names.items()
    }
    collisions: dict[str, list[int]] = {}
    for opcode, api_name in forward.items():
        collisions.setdefault(api_name, []).append(opcode)
    for opcodes in collisions.values():
        if len(opcodes) > 1:
            for opcode in opcodes:
                forward[opcode] = f"VM.{core._pascal_case(dialect.names[opcode])}"
    reverse = {api_name: opcode for opcode, api_name in forward.items()}
    if len(reverse) != len(forward):
        raise AssertionError(f"{dialect.key} VM API mapping is not injective")
    return forward, reverse


@dataclass
class VmSegment:
    kind: str
    source_offset: int | None = None
    body: list[core.Statement] | None = None
    metadata: dict[str, Any] | None = None


@dataclass
class VmEntry:
    entry_id: int
    metadata: dict[str, Any]
    segments: list[VmSegment]


@dataclass
class VmArchive:
    source: str
    metadata: dict[str, Any]
    entries: list[VmEntry]


@dataclass
class VmProgram:
    kind: str
    metadata: dict[str, Any]
    archives: list[VmArchive]


class VmSourceParser(core.Parser):
    """Parser for battle/archive and scene/multi-archive source roots."""

    def parse_vm_program(self) -> VmProgram:
        if self.current.text == "battle":
            return self.parse_battle_program()
        if self.current.text == "scene":
            return self.parse_scene_program()
        raise self.error("expected 'battle' or 'scene'")

    def parse_battle_program(self) -> VmProgram:
        self.expect("battle")
        source = self.expect_kind("STRING", "battle archive path").value
        self.expect("{")
        metadata = self.parse_required_metadata("battle archive")
        entries: list[VmEntry] = []
        while self.current.text == "entry":
            entries.append(self.parse_entry("battle"))
        self.expect("}")
        if self.current.kind != "EOF":
            raise self.error("unexpected content after battle archive")
        return VmProgram("battle", metadata, [VmArchive(source, {}, entries)])

    def parse_scene_program(self) -> VmProgram:
        self.expect("scene")
        self.expect("{")
        metadata = self.parse_required_metadata("scene document")
        archives: list[VmArchive] = []
        while self.current.text == "archive":
            self.expect("archive")
            source = self.expect_kind("STRING", "scene archive path").value
            self.expect("{")
            archive_metadata = self.parse_required_metadata("scene archive")
            entries: list[VmEntry] = []
            while self.current.text == "entry":
                entries.append(self.parse_entry("scene"))
            self.expect("}")
            archives.append(VmArchive(source, archive_metadata, entries))
        self.expect("}")
        if self.current.kind != "EOF":
            raise self.error("unexpected content after scene document")
        return VmProgram("scene", metadata, archives)

    def parse_required_metadata(self, context: str) -> dict[str, Any]:
        if self.current.text != "metadata":
            raise self.error(f"{context} requires a metadata block")
        self.expect("metadata")
        result = self.parse_json_object()
        self.accept(";")
        return result

    def parse_entry(self, kind: str) -> VmEntry:
        self.expect("entry")
        entry_id = self.expect_kind("NUMBER", "entry ID").value
        if entry_id < 0:
            raise self.error("entry ID cannot be negative")
        self.expect("{")
        metadata = self.parse_required_metadata(f"{kind} entry")
        segments: list[VmSegment] = []
        while self.current.text != "}":
            if kind == "battle" and self.current.text == "code":
                self.expect("code")
                self.expect("{")
                body = self.parse_statements()
                self.expect("}")
                segments.append(VmSegment("code", body=body))
            elif kind == "battle" and self.current.text == "private_data":
                self.expect("private_data")
                payload = self.parse_json_object()
                self.accept(";")
                segments.append(VmSegment("private_data", metadata=payload))
            elif kind == "scene" and self.current.text == "section":
                section = self.parse_section()
                segments.append(
                    VmSegment(
                        "section", source_offset=section.source_offset, body=section.body
                    )
                )
            else:
                expected = "'code', 'private_data', or '}'" if kind == "battle" else "'section' or '}'"
                raise self.error(f"expected {expected}")
        self.expect("}")
        return VmEntry(entry_id, metadata, segments)


def _variable_from_json(dialect: VmDialect, value: Any, context: str) -> core.Value:
    if not isinstance(value, str):
        raise core.PitLanguageError(f"{context} must be a VM variable string")
    try:
        raw = dialect.parse_variable(value, context)
    except Exception as exc:
        raise core.PitLanguageError(str(exc)) from exc
    return core.Value.variable(dialect.format_variable(raw))


def _argument_from_json(
    dialect: VmDialect, value: Any, context: str
) -> core.Value:
    if isinstance(value, dict):
        if set(value) == {"variable"}:
            return _variable_from_json(dialect, value["variable"], context)
        if set(value) == {"label"} and isinstance(value["label"], str):
            return core.Value.label(value["label"])
        raise core.PitLanguageError(
            f"{context} object must contain exactly 'variable' or 'label'"
        )
    if isinstance(value, bool) or not isinstance(value, int):
        raise core.PitLanguageError(f"{context} literal must be a signed integer")
    if not -0x8000 <= value <= 0x7FFF:
        raise core.PitLanguageError(f"{context} literal does not fit signed 16 bits")
    return core.Value.literal(value)


def _instruction_size(opcode: int, dialect: VmDialect) -> int:
    return core._instruction_size(opcode, dialect.descriptors)


def _instruction_from_json(
    dialect: VmDialect,
    command: dict[str, Any],
    labels_by_offset: dict[int, list[str]],
    context: str,
) -> core.Instruction:
    offset = core._as_offset(command.get("source_offset"), context)
    opcode = dialect.parse_opcode(command.get("opcode"), context)
    end = offset + _instruction_size(opcode, dialect)
    descriptor = dialect.descriptors[opcode]
    raw_arguments = command.get("args")
    if not isinstance(raw_arguments, list):
        raise core.PitLanguageError(f"{context} args must be an array")
    arguments = [
        _argument_from_json(dialect, value, f"{context} argument {index}")
        for index, value in enumerate(raw_arguments)
    ]
    if len(arguments) != descriptor & 0x1F:
        raise core.PitLanguageError(
            f"{context} expects {descriptor & 0x1F} arguments"
        )
    reference_index = _relative_reference_index(dialect, opcode)
    if (
        dialect.key == "scene"
        and reference_index is not None
        and arguments[reference_index].kind == "literal"
    ):
        target = end + int(arguments[reference_index].value) * 2
        labels = labels_by_offset.get(target)
        if not labels:
            raise core.PitLanguageError(
                f"{context} target {core._format_offset(target)} has no source label"
            )
        preferred = next(
            (label for label in labels if label.startswith("entry_")), labels[0]
        )
        arguments[reference_index] = core.Value.label(preferred)
    result = (
        _variable_from_json(dialect, command["result"], f"{context} result")
        if "result" in command
        else None
    )
    if bool(descriptor & 0x20) != (result is not None):
        raise core.PitLanguageError(f"{context} has an invalid result contract")
    raw_mode = command.get("unused_mode_bits", 0)
    try:
        mode = int(raw_mode, 0) if isinstance(raw_mode, str) else int(raw_mode)
    except (TypeError, ValueError) as exc:
        raise core.PitLanguageError(f"{context} has invalid mode bits") from exc
    labels = command.get("labels", [])
    if not isinstance(labels, list) or not all(
        isinstance(label, str)
        and re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", label)
        for label in labels
    ):
        raise core.PitLanguageError(f"{context} labels must be identifiers")
    return core.Instruction(
        opcode=opcode,
        result=result,
        arguments=arguments,
        unused_mode_bits=mode,
        labels=list(labels),
        offset=offset,
        end=end,
    )


def _relative_reference_index(dialect: VmDialect, opcode: int) -> int | None:
    if opcode in dialect.data_references:
        return dialect.data_references[opcode][0]
    if opcode in dialect.code_references:
        return dialect.code_references[opcode]
    if opcode in dialect.inline_opcodes:
        return dialect.inline_opcodes[opcode]
    if opcode in dialect.spawn_opcodes:
        return dialect.spawn_opcodes[opcode]
    return dialect.wait_skip_opcodes.get(opcode)


def _semantic_comment(
    dialect: VmDialect, opcode: int, arguments: Sequence[core.Value]
) -> str | None:
    name = dialect.names.get(opcode, f"op_{opcode:03X}")
    if name == "apply_status" and len(arguments) >= 4:
        return (
            "[Kampfstatus] Wendet Status "
            f"{core._brief_value(arguments[1])} auf Akteur "
            f"{core._brief_value(arguments[0])} an; Stärke/Chance und Dauer "
            "bleiben als getrennte VM-Parameter erhalten."
        )
    if name == "clear_status" and len(arguments) >= 2:
        return (
            "[Kampfstatus] Entfernt den gewählten Status; -1 löscht alle "
            "aktiven Statuseffekte des Akteurs."
        )
    if name == "get_enemy_stat" and len(arguments) >= 2:
        return "[Kampfwerte] Liest einen Gegnerwert in eine VM-Variable."
    if "damage" in name:
        return "[Schaden] Diese Anweisung gehört zur Schadensberechnung oder Trefferauflösung."
    if name in {
        "start_inline_object_script",
        "start_object_script",
        "start_object_script_and_yield",
    }:
        return "[Ablauf] Startet eine nebenläufige Objekt-Choreografie."
    if name == "render_object_text_tiles" and len(arguments) >= 4:
        return (
            "[Text] Rendert Text-ID "
            f"{core._brief_value(arguments[2])}, Seite "
            f"{core._brief_value(arguments[3])}; der konkrete Sprachkatalog "
            "wird durch die zur Laufzeit gebundene Objektressource bestimmt."
        )
    if name == "set_object_animation" or name == "activate_and_set_object_property":
        return "[Animation] Wechselt die Animation bzw. sichtbare Eigenschaft des Objekts."
    if name.startswith("move_object"):
        return "[Choreografie] Bewegt ein Kampf-/Szenenobjekt mit den kodierten Ziel- und Geschwindigkeitswerten."
    if "camera" in name:
        return "[Kamera] Verändert die Kampf- oder Szenenkamera."
    return None


def _archive_context(kind: str, source: str) -> list[str]:
    if kind == "battle":
        filename = Path(source).name
        if "_mon_" in filename:
            detail = "Gegner-KI, Angriffsabläufe und trefferabhängige Reaktionen."
        elif "_scn_" in filename:
            detail = "Kampf-Szenensteuerung, Formation und Phasen-Choreografie."
        else:
            detail = "Gemeinsam genutzte Kampfskripte und Objekt-Choreografie."
        return [
            "// PiT-Battle-VM-Quelle; Sprünge werden beim Kompilieren neu relokiert.",
            f"// Archiv {source}: {detail}",
            "// Nicht eindeutig belegte Gegner-/Bossnamen werden bewusst nicht geraten.",
        ]
    return [
        "// PiT-Scene/Object-VM-Quelle aus Overlay 7.",
        "// Die Abschnittsanker schützen noch nicht klassifizierte private Bytes; Befehlsgrößen müssen daher innerhalb eines Abschnitts kompatibel bleiben.",
    ]


class VmDecompiler:
    def __init__(self, document: dict[str, Any], kind: str) -> None:
        self.document = document
        self.dialect = load_dialect(kind)
        self.api_names = self.dialect.api_names

    def run(self) -> str:
        if not isinstance(self.document, dict):
            raise core.PitLanguageError("VM document must be a JSON object")
        if self.document.get("schema") != self.dialect.schema:
            raise core.PitLanguageError(
                f"document must use schema {self.dialect.schema!r}"
            )
        if self.dialect.key == "battle":
            return self.render_battle()
        return self.render_scene()

    def render_battle(self) -> str:
        source = self.document.get("source")
        entries = self.document.get("entries")
        if not isinstance(source, str) or not isinstance(entries, list):
            raise core.PitLanguageError("battle document has invalid source/entries")
        lines = _archive_context("battle", source)
        lines.extend([f"battle {json.dumps(source, ensure_ascii=False)} {{", "  metadata"])
        metadata = {
            key: copy.deepcopy(value)
            for key, value in self.document.items()
            if key not in {"source", "entries", "command_count"}
        }
        lines.extend(core._json_lines(metadata, 2))
        for index, row in enumerate(entries):
            if not isinstance(row, dict) or row.get("entry_id") != index:
                raise core.PitLanguageError("battle entry IDs must be contiguous")
            lines.extend(self.render_battle_entry(row, index))
        lines.append("}")
        return "\n".join(lines) + "\n"

    def render_battle_entry(self, row: dict[str, Any], entry_id: int) -> list[str]:
        segments = row.get("segments")
        if not isinstance(segments, list):
            raise core.PitLanguageError(f"battle entry {entry_id} segments must be an array")
        metadata = {
            key: copy.deepcopy(value)
            for key, value in row.items()
            if key not in {"entry_id", "segments", "reachable_command_count"}
        }
        labels_by_offset: dict[int, list[str]] = {}
        for segment in segments:
            if not isinstance(segment, dict):
                raise core.PitLanguageError(f"battle entry {entry_id} has a non-object segment")
            if segment.get("kind") == "private_data":
                offset = core._as_offset(segment.get("source_offset"), "private segment")
                labels_by_offset.setdefault(offset, []).extend(segment.get("labels", []))
            for command in segment.get("commands", []):
                if isinstance(command, dict):
                    offset = core._as_offset(command.get("source_offset"), "battle command")
                    labels_by_offset.setdefault(offset, []).extend(command.get("labels", []))
        lines = ["", f"  entry {entry_id} {{", "    metadata"]
        lines.extend(core._json_lines(metadata, 4))
        for segment_index, segment in enumerate(segments):
            kind = segment.get("kind")
            if kind == "private_data":
                payload = {key: copy.deepcopy(value) for key, value in segment.items() if key != "kind"}
                lines.extend(["", "    // Unklassifizierte Originaldaten; Quellbereich und SHA-1 bleiben privat verankert.", "    private_data"])
                lines.extend(core._json_lines(payload, 4))
                continue
            if kind != "code" or not isinstance(segment.get("commands"), list):
                raise core.PitLanguageError(
                    f"battle entry {entry_id} segment {segment_index} is invalid"
                )
            commands = [
                _instruction_from_json(
                    self.dialect,
                    command,
                    labels_by_offset,
                    f"battle entry {entry_id} segment {segment_index} command {command_index}",
                )
                for command_index, command in enumerate(segment["commands"])
            ]
            lines.extend(["", "    code {"])
            if commands:
                inbound = self.inbound_edges(commands, labels_by_offset)
                lines.extend(
                    self.render_range(
                        commands,
                        0,
                        len(commands),
                        6,
                        labels_by_offset,
                        inbound,
                        commands[0].offset,
                        commands[-1].end,
                    )
                )
            lines.append("    }")
        lines.extend(["  }", ""])
        return lines

    def render_scene(self) -> str:
        archives = self.document.get("archives")
        if not isinstance(archives, list):
            raise core.PitLanguageError("scene archives must be an array")
        lines = _archive_context("scene", "")
        lines.extend(["scene {", "  metadata"])
        metadata = {
            key: copy.deepcopy(value)
            for key, value in self.document.items()
            if key != "archives"
        }
        lines.extend(core._json_lines(metadata, 2))
        for archive_index, archive in enumerate(archives):
            if not isinstance(archive, dict) or not isinstance(archive.get("source"), str):
                raise core.PitLanguageError(f"scene archive {archive_index} is invalid")
            source = archive["source"]
            lines.extend(["", f"  archive {json.dumps(source, ensure_ascii=False)} {{", "    metadata"])
            archive_metadata = {
                key: copy.deepcopy(value)
                for key, value in archive.items()
                if key not in {"source", "entries"}
            }
            lines.extend(core._json_lines(archive_metadata, 4))
            entries = archive.get("entries")
            if not isinstance(entries, list):
                raise core.PitLanguageError(f"scene archive {source} entries must be an array")
            for entry_index, entry in enumerate(entries):
                lines.extend(self.render_scene_entry(entry, entry_index, source))
            lines.extend(["  }", ""])
        lines.append("}")
        return "\n".join(lines) + "\n"

    def render_scene_entry(
        self, row: Any, entry_id: int, source: str
    ) -> list[str]:
        if not isinstance(row, dict) or row.get("entry_id") != entry_id:
            raise core.PitLanguageError(f"scene {source} entry IDs must be contiguous")
        raw_commands = row.get("commands")
        if not isinstance(raw_commands, list):
            raise core.PitLanguageError(f"scene {source} entry {entry_id} commands must be an array")
        metadata = {
            key: copy.deepcopy(value)
            for key, value in row.items()
            if key not in {"entry_id", "commands", "reachable_command_count", "private_byte_count"}
        }
        labels_by_offset = {
            core._as_offset(command.get("source_offset"), "scene command"): list(command.get("labels", []))
            for command in raw_commands
            if isinstance(command, dict)
        }
        commands = [
            _instruction_from_json(
                self.dialect,
                command,
                labels_by_offset,
                f"scene {source} entry {entry_id} command {index}",
            )
            for index, command in enumerate(raw_commands)
        ]
        commands.sort(key=lambda command: command.offset)
        sections: list[list[core.Instruction]] = []
        for command in commands:
            if not sections or sections[-1][-1].end != command.offset:
                sections.append([command])
            else:
                sections[-1].append(command)
        inbound = self.inbound_edges(commands, labels_by_offset)
        lines = ["", f"    entry {entry_id} {{", "      metadata"]
        lines.extend(core._json_lines(metadata, 6))
        for section in sections:
            lines.extend(["", f"      section {core._format_offset(section[0].offset)} {{"])
            lines.extend(
                self.render_range(
                    section,
                    0,
                    len(section),
                    8,
                    labels_by_offset,
                    inbound,
                    section[0].offset,
                    section[-1].end,
                )
            )
            lines.append("      }")
        lines.extend(["    }", ""])
        return lines

    def inbound_edges(
        self,
        commands: Sequence[core.Instruction],
        labels_by_offset: dict[int, list[str]],
    ) -> dict[int, list[int]]:
        offsets = {
            label: offset
            for offset, labels in labels_by_offset.items()
            for label in labels
        }
        inbound: dict[int, list[int]] = {}
        for command in commands:
            index = _relative_reference_index(self.dialect, command.opcode)
            if index is None or command.arguments[index].kind != "label":
                continue
            target = offsets.get(str(command.arguments[index].value))
            if target is not None:
                inbound.setdefault(target, []).append(command.offset)
        return inbound

    def render_range(
        self,
        commands: Sequence[core.Instruction],
        start: int,
        stop: int,
        indent: int,
        labels_by_offset: dict[int, list[str]],
        inbound: dict[int, list[int]],
        expected_start: int,
        expected_end: int,
    ) -> list[str]:
        lines: list[str] = []
        offsets_by_label = {
            label: offset
            for offset, labels in labels_by_offset.items()
            for label in labels
        }
        cursor = expected_start
        index = start
        while index < stop:
            command = commands[index]
            prefix = " " * indent
            if command.offset < cursor:
                raise core.PitLanguageError("overlapping VM instructions")
            if command.offset > cursor:
                gap = command.offset - cursor
                if gap & 1:
                    raise core.PitLanguageError("odd-byte VM code gap")
                lines.append(f"{prefix}padding {gap // 2};")
            for label in command.labels:
                lines.append(f"{prefix}{label}:")

            if command.opcode in self.dialect.inline_opcodes:
                reference_index = self.dialect.inline_opcodes[command.opcode]
                reference = command.arguments[reference_index]
                target = (
                    offsets_by_label.get(str(reference.value))
                    if reference.kind == "label"
                    else command.end + int(reference.value) * 2
                )
                child_stop = index + 1
                child_cursor = command.end
                while child_stop < stop and commands[child_stop].offset < (target or -1):
                    child = commands[child_stop]
                    if child.offset != child_cursor or target is None or child.end > target:
                        child_stop = index + 1
                        break
                    child_cursor = child.end
                    child_stop += 1
                if target is not None and child_stop > index + 1 and child_cursor == target:
                    lines.append(
                        f"{prefix}// [Async] Objekt {core._brief_value(command.arguments[0])} führt den eingerückten Kind-Ablauf parallel aus."
                    )
                    lines.append(
                        f"{prefix}async Actor.Run({core._format_value(command.arguments[0])}, {core._format_value(command.arguments[1])}) {{"
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
                            target,
                        )
                    )
                    lines.append(f"{prefix}}}")
                    cursor = target
                    index = child_stop
                    continue

            loop = self.find_counted_loop(commands, index, stop, inbound, offsets_by_label)
            if loop is not None:
                body_start, jump_index, exit_index = loop
                loop_command = commands[index + 1]
                lines.append(
                    f"{prefix}// [Schleife] Zählt von {core._brief_value(command.arguments[0])} bis {core._brief_value(loop_command.arguments[2])}."
                )
                lines.append(
                    f"{prefix}for (loop_counter = {core._format_value(command.arguments[0])}; loop_counter != {core._format_value(loop_command.arguments[2])}; loop_counter--) {{"
                )
                lines.extend(
                    self.render_range(
                        commands,
                        body_start,
                        jump_index,
                        indent + 2,
                        labels_by_offset,
                        inbound,
                        loop_command.end,
                        commands[jump_index].offset,
                    )
                )
                # A conditional inside the body may intentionally target the
                # hidden back-edge (the VM equivalent of ``continue``).  Keep
                # that label immediately before the compiler-generated jump.
                for label in commands[jump_index].labels:
                    lines.append(f"{' ' * (indent + 2)}{label}:")
                lines.append(f"{prefix}}}")
                cursor = commands[exit_index].offset
                index = exit_index
                continue

            comment = _semantic_comment(self.dialect, command.opcode, command.arguments)
            if comment:
                lines.append(f"{prefix}// {comment}")
            semantics = self.dialect.semantics.get(command.opcode, {})
            parameters = semantics.get("arguments", [])
            rendered: list[str] = []
            for argument_index, argument in enumerate(command.arguments):
                parameter = parameters[argument_index] if argument_index < len(parameters) else ""
                rendered.append(core._format_value(argument, "status_id" in parameter))
            assignment = f"{core._format_value(command.result)} = " if command.result else ""
            mode = f" @mode(0x{command.unused_mode_bits:04X})" if command.unused_mode_bits else ""
            lines.append(
                f"{prefix}{assignment}{self.api_names[command.opcode]}({', '.join(rendered)}){mode};"
            )
            cursor = command.end
            index += 1
        if cursor < expected_end:
            gap = expected_end - cursor
            if gap & 1:
                raise core.PitLanguageError("odd-byte tail in VM code range")
            lines.append(f"{' ' * indent}padding {gap // 2};")
        elif cursor > expected_end:
            raise core.PitLanguageError("VM source range exceeds its boundary")
        return lines

    @staticmethod
    def find_counted_loop(
        commands: Sequence[core.Instruction],
        index: int,
        stop: int,
        inbound: dict[int, list[int]],
        offsets_by_label: dict[str, int],
    ) -> tuple[int, int, int] | None:
        if index + 2 >= stop or commands[index].opcode != 0x0A:
            return None
        loop = commands[index + 1]
        if loop.opcode != 0x0C or len(loop.arguments) != 4:
            return None
        if core._literal(loop.arguments[0]) != 0x12 or core._literal(loop.arguments[1]) != 0:
            return None
        if loop.arguments[3].kind != "label":
            return None
        exit_offset = offsets_by_label.get(str(loop.arguments[3].value))
        exit_index = next(
            (
                candidate
                for candidate in range(index + 2, stop)
                if commands[candidate].offset == exit_offset
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
            or core._literal(jump.arguments[0]) != 2
            or jump.arguments[1].kind != "label"
            or offsets_by_label.get(str(jump.arguments[1].value)) != loop.offset
        ):
            return None
        if any(label.startswith("entry_") for label in loop.labels + jump.labels):
            return None
        if inbound.get(loop.offset, []) != [jump.offset]:
            return None
        if any(
            source < loop.end or source >= jump.offset
            for source in inbound.get(jump.offset, [])
        ):
            return None
        return index + 2, jump_index, exit_index


class VmLowerer:
    def __init__(self, dialect: VmDialect) -> None:
        self.dialect = dialect
        self.reverse_api_names = dialect.reverse_api_names
        self.serial = 0

    def lower(self, statements: Iterable[core.Statement]) -> list[core.LoweredItem]:
        result: list[core.LoweredItem] = []
        for statement in statements:
            if isinstance(statement, core.LabelStatement):
                result.append(core.LabelMarker(statement.name))
            elif isinstance(statement, core.PaddingStatement):
                result.append(core.PaddingItem(statement.halfwords))
            elif isinstance(statement, core.CallStatement):
                result.append(self.lower_call(statement))
            elif isinstance(statement, core.AsyncStatement):
                result.extend(self.lower_async(statement))
            elif isinstance(statement, core.ForStatement):
                result.extend(self.lower_for(statement))
            else:  # pragma: no cover
                raise AssertionError(f"unknown statement {statement!r}")
        return result

    def lower_call(self, statement: core.CallStatement) -> core.Instruction:
        opcode = self.reverse_api_names.get(statement.api_name)
        if opcode is None:
            raise core.PitLanguageError(f"unknown {self.dialect.key} VM method {statement.api_name}")
        self.validate(opcode, statement.result, statement.arguments, statement.unused_mode_bits, statement.api_name)
        return core.Instruction(opcode, statement.result, list(statement.arguments), statement.unused_mode_bits)

    def lower_async(self, statement: core.AsyncStatement) -> list[core.LoweredItem]:
        if statement.wait:
            raise core.PitLanguageError(
                f"{self.dialect.key} VM has no inline Actor.RunAndWait opcode"
            )
        if len(self.dialect.inline_opcodes) != 1:
            raise core.PitLanguageError(f"{self.dialect.key} VM has no unique inline opcode")
        opcode, argument_index = next(iter(self.dialect.inline_opcodes.items()))
        if argument_index != 2:
            raise core.PitLanguageError("unsupported inline operand layout")
        resume = f"__async_{self.serial}_resume"
        self.serial += 1
        arguments = [statement.entity_selector, statement.chain_if_active, core.Value.label(resume)]
        self.validate(opcode, None, arguments, 0, "async Actor.Run")
        return [
            core.Instruction(opcode, None, arguments),
            *self.lower(statement.body),
            core.LabelMarker(resume),
        ]

    def lower_for(self, statement: core.ForStatement) -> list[core.LoweredItem]:
        serial = self.serial
        self.serial += 1
        head = f"__loop_{serial}_head"
        exit_label = f"__loop_{serial}_exit"
        push = core.Instruction(0x0A, None, [statement.initial])
        loop = core.Instruction(
            0x0C,
            None,
            [core.Value.literal(0x12), core.Value.literal(0), statement.limit, core.Value.label(exit_label)],
        )
        jump = core.Instruction(0x02, None, [core.Value.literal(2), core.Value.label(head)])
        self.validate(0x0A, None, push.arguments, 0, "for initializer")
        self.validate(0x0C, None, loop.arguments, 0, "for condition")
        self.validate(0x02, None, jump.arguments, 0, "for back edge")
        return [
            push,
            core.LabelMarker(head),
            loop,
            *self.lower(statement.body),
            jump,
            core.LabelMarker(exit_label),
        ]

    def validate(
        self,
        opcode: int,
        result: core.Value | None,
        arguments: Sequence[core.Value],
        unused_mode_bits: int,
        context: str,
    ) -> None:
        descriptor = self.dialect.descriptors[opcode]
        count = descriptor & 0x1F
        if len(arguments) != count:
            raise core.PitLanguageError(f"{context} expects {count} arguments, got {len(arguments)}")
        if bool(descriptor & 0x20) != (result is not None):
            raise core.PitLanguageError(f"{context} has the wrong result contract")
        if result is not None and result.kind != "variable":
            raise core.PitLanguageError(f"{context} result must be a VM variable")
        if not 0 <= unused_mode_bits <= 0xFFFF:
            raise core.PitLanguageError(f"{context} mode mask must fit in 16 bits")
        if unused_mode_bits and not (descriptor & 0x40 and count):
            raise core.PitLanguageError(f"{context} does not encode a mode mask")
        used_mask = 0
        for index, argument in enumerate(arguments):
            if argument.kind == "variable":
                if not descriptor & 0x40:
                    raise core.PitLanguageError(f"{context} argument {index} cannot be a variable")
                used_mask |= 1 << index
            elif argument.kind == "literal":
                if not -0x8000 <= int(argument.value) <= 0x7FFF:
                    raise core.PitLanguageError(f"{context} argument {index} does not fit signed 16 bits")
            elif argument.kind != "label":
                raise core.PitLanguageError(f"{context} argument {index} is invalid")
        if unused_mode_bits & (((1 << count) - 1) | used_mask):
            raise core.PitLanguageError(f"{context} mode bits overlap argument modes")


@dataclass
class PlannedEntry:
    instructions: list[core.Instruction]
    private_segments: list[tuple[int, dict[str, Any], int]]
    label_offsets: dict[str, int]
    instruction_segment: dict[int, int]
    total_size: int


class VmCompiler:
    def __init__(self, source: str) -> None:
        self.program = VmSourceParser(source).parse_vm_program()
        self.dialect = load_dialect(self.program.kind)
        self.lowerer = VmLowerer(self.dialect)

    def run(self) -> dict[str, Any]:
        if self.program.kind == "battle":
            return self.compile_battle()
        return self.compile_scene()

    def compile_battle(self) -> dict[str, Any]:
        if len(self.program.archives) != 1:
            raise core.PitLanguageError("battle source must contain exactly one archive")
        archive = self.program.archives[0]
        if "source" in self.program.metadata or "entries" in self.program.metadata:
            raise core.PitLanguageError("battle metadata contains compiler-owned fields")
        document = copy.deepcopy(self.program.metadata)
        if document.get("schema") != BATTLE_SCHEMA:
            raise core.PitLanguageError(f"battle metadata must declare schema {BATTLE_SCHEMA!r}")
        document["source"] = archive.source
        entries = []
        total_commands = 0
        for expected_id, entry in enumerate(archive.entries):
            if entry.entry_id != expected_id:
                raise core.PitLanguageError("battle entry IDs must be contiguous")
            row, count = self.compile_battle_entry(entry)
            entries.append(row)
            total_commands += count
        document["command_count"] = total_commands
        document["entries"] = entries
        return document

    def plan_battle_entry(self, entry: VmEntry) -> PlannedEntry:
        header_size = entry.metadata.get("header_size")
        if isinstance(header_size, bool) or not isinstance(header_size, int) or header_size < 2 or header_size & 1:
            raise core.PitLanguageError(f"battle entry {entry.entry_id} has invalid header_size")
        cursor = header_size
        instructions: list[core.Instruction] = []
        private_segments: list[tuple[int, dict[str, Any], int]] = []
        label_offsets: dict[str, int] = {}
        instruction_segment: dict[int, int] = {}

        def bind(label: str, offset: int) -> None:
            if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", label):
                raise core.PitLanguageError(f"battle entry {entry.entry_id} has invalid label {label!r}")
            if label in label_offsets:
                raise core.PitLanguageError(f"battle entry {entry.entry_id} repeats label {label}")
            label_offsets[label] = offset

        for segment_index, segment in enumerate(entry.segments):
            if segment.kind == "private_data":
                metadata = copy.deepcopy(segment.metadata or {})
                size = metadata.get("size")
                if isinstance(size, bool) or not isinstance(size, int) or size < 0:
                    raise core.PitLanguageError(f"battle entry {entry.entry_id} private segment has invalid size")
                labels = metadata.pop("labels", [])
                if not isinstance(labels, list):
                    raise core.PitLanguageError("private_data labels must be an array")
                for label in labels:
                    bind(label, cursor)
                private_segments.append((segment_index, metadata, cursor))
                cursor += size
                continue
            if segment.kind != "code" or segment.body is None:
                raise core.PitLanguageError(f"battle entry {entry.entry_id} has invalid segment")
            pending: list[str] = []
            for item in self.lowerer.lower(segment.body):
                if isinstance(item, core.LabelMarker):
                    bind(item.name, cursor)
                    pending.append(item.name)
                elif isinstance(item, core.PaddingItem):
                    raise core.PitLanguageError(
                        "battle code cannot contain padding; model opaque bytes as private_data"
                    )
                else:
                    item.offset = cursor
                    item.end = cursor + _instruction_size(item.opcode, self.dialect)
                    item.labels = pending
                    pending = []
                    instructions.append(item)
                    instruction_segment[id(item)] = segment_index
                    cursor = item.end
            # A generated async-resume marker may legally coincide with the
            # first item of the following segment.  The global label map keeps
            # it even when no JSON command owns it yet.
        return PlannedEntry(instructions, private_segments, label_offsets, instruction_segment, cursor)

    def compile_battle_entry(self, entry: VmEntry) -> tuple[dict[str, Any], int]:
        forbidden = {"entry_id", "segments", "reachable_command_count"}
        if forbidden & entry.metadata.keys():
            raise core.PitLanguageError(f"battle entry {entry.entry_id} metadata contains compiler-owned fields")
        plan = self.plan_battle_entry(entry)
        entry_points = entry.metadata.get("entry_points")
        if not isinstance(entry_points, list):
            raise core.PitLanguageError(f"battle entry {entry.entry_id} needs entry_points")
        for label in entry_points:
            if label is not None and label not in plan.label_offsets:
                raise core.PitLanguageError(f"battle entry {entry.entry_id} entry point {label!r} is undefined")

        code_targets: dict[int, int] = {}
        data_targets: dict[int, int] = {}
        resolved_target_by_arg: dict[tuple[int, int], int] = {}
        for instruction in plan.instructions:
            ref_index = _relative_reference_index(self.dialect, instruction.opcode)
            for index, argument in enumerate(instruction.arguments):
                if argument.kind != "label":
                    continue
                if index != ref_index:
                    raise core.PitLanguageError(
                        f"battle command {core._format_offset(instruction.offset)} argument {index} is not relocatable"
                    )
                label = str(argument.value)
                if label not in plan.label_offsets:
                    raise core.PitLanguageError(f"battle command references unknown label {label}")
                target = plan.label_offsets[label]
                resolved_target_by_arg[(id(instruction), index)] = target
                if instruction.opcode in self.dialect.data_references:
                    data_targets[target] = target
                else:
                    code_targets[target] = target

        # Preserve the extractor's label ordering when possible.  This matters
        # when one byte offset is both a code and embedded-data target: the
        # original JSON intentionally records which name each operand used.
        labels_at: dict[int, list[str]] = {}
        renamed_label: dict[str, str] = {}
        for label, offset in plan.label_offsets.items():
            if label.startswith("__"):
                continue
            if label.startswith("loc_"):
                rendered = f"loc_{offset:04X}"
            elif label.startswith("data_"):
                rendered = f"data_{offset:04X}"
            else:
                rendered = label
            renamed_label[label] = rendered
            labels_at.setdefault(offset, []).append(rendered)
        for label in entry_points:
            if label is not None and label not in labels_at.setdefault(
                plan.label_offsets[label], []
            ):
                labels_at[plan.label_offsets[label]].insert(0, label)
        for target in code_targets:
            if f"loc_{target:04X}" not in labels_at.setdefault(target, []):
                labels_at[target].append(f"loc_{target:04X}")
        for target in data_targets:
            if f"data_{target:04X}" not in labels_at.setdefault(target, []):
                labels_at[target].append(f"data_{target:04X}")
        for offset in labels_at:
            labels_at[offset] = list(dict.fromkeys(labels_at[offset]))

        def preferred(target: int, source_label: str, data_reference: bool) -> str:
            labels = labels_at.get(target)
            if not labels:
                raise core.PitLanguageError(
                    f"battle target {core._format_offset(target)} has no generated label"
                )
            if source_label in renamed_label:
                return renamed_label[source_label]
            entry_label = next(
                (label for label in labels if label.startswith("entry_")), None
            )
            if entry_label is not None:
                return entry_label
            prefix = "data_" if data_reference else "loc_"
            return next((label for label in labels if label.startswith(prefix)), labels[0])

        commands_by_segment: dict[int, list[dict[str, Any]]] = {}
        for instruction in plan.instructions:
            row: dict[str, Any] = {
                "opcode": self.dialect.opcode_name(instruction.opcode),
                "args": [],
                "source_offset": core._format_offset(instruction.offset),
            }
            if instruction.result is not None:
                row["result"] = self.variable_name(instruction.result, "battle result")
                # Match the extractor's key order for readable diffs.
                row = {
                    "opcode": row["opcode"],
                    "result": row["result"],
                    "args": row["args"],
                    "source_offset": row["source_offset"],
                }
            encoded_args = []
            for index, argument in enumerate(instruction.arguments):
                target = resolved_target_by_arg.get((id(instruction), index))
                if target is not None:
                    encoded_args.append(
                        {
                            "label": preferred(
                                target,
                                str(argument.value),
                                instruction.opcode in self.dialect.data_references,
                            )
                        }
                    )
                else:
                    encoded_args.append(self.argument_json(argument, "battle argument"))
            row["args"] = encoded_args
            if instruction.unused_mode_bits:
                row["unused_mode_bits"] = f"0x{instruction.unused_mode_bits:04X}"
            labels = labels_at.get(instruction.offset)
            if labels:
                row["labels"] = labels
            commands_by_segment.setdefault(plan.instruction_segment[id(instruction)], []).append(row)

        private_by_segment = {index: (metadata, offset) for index, metadata, offset in plan.private_segments}
        segments: list[dict[str, Any]] = []
        for segment_index, segment in enumerate(entry.segments):
            if segment.kind == "code":
                segments.append({"kind": "code", "commands": commands_by_segment.get(segment_index, [])})
            else:
                metadata, placement = private_by_segment[segment_index]
                private_row = {"kind": "private_data", **metadata}
                labels = labels_at.get(placement)
                if labels:
                    private_row["labels"] = labels
                segments.append(private_row)
        row = copy.deepcopy(entry.metadata)
        row["entry_id"] = entry.entry_id
        # Keep source-owned size/hash fields, but regenerate every code-derived count.
        row["reachable_command_count"] = len(plan.instructions)
        row["segments"] = segments
        return row, len(plan.instructions)

    def compile_scene(self) -> dict[str, Any]:
        if "archives" in self.program.metadata:
            raise core.PitLanguageError("scene metadata contains compiler-owned archives")
        document = copy.deepcopy(self.program.metadata)
        if document.get("schema") != SCENE_SCHEMA:
            raise core.PitLanguageError(f"scene metadata must declare schema {SCENE_SCHEMA!r}")
        archives = []
        for expected_source, archive in zip(scene_script_mod.SOURCES, self.program.archives):
            if archive.source != expected_source:
                raise core.PitLanguageError(
                    f"scene archive order must be {', '.join(scene_script_mod.SOURCES)}"
                )
            archives.append(self.compile_scene_archive(archive))
        if len(self.program.archives) != len(scene_script_mod.SOURCES):
            raise core.PitLanguageError("scene source must contain all three archives")
        document["archives"] = archives
        return document

    def compile_scene_archive(self, archive: VmArchive) -> dict[str, Any]:
        if {"source", "entries"} & archive.metadata.keys():
            raise core.PitLanguageError(f"scene archive {archive.source} metadata contains compiler-owned fields")
        row = copy.deepcopy(archive.metadata)
        row["source"] = archive.source
        entries = []
        for expected_id, entry in enumerate(archive.entries):
            if entry.entry_id != expected_id:
                raise core.PitLanguageError(f"scene archive {archive.source} entry IDs must be contiguous")
            entries.append(self.compile_scene_entry(entry, archive.source))
        row["entries"] = entries
        return row

    def compile_scene_entry(self, entry: VmEntry, source: str) -> dict[str, Any]:
        forbidden = {"entry_id", "commands", "reachable_command_count", "private_byte_count"}
        if forbidden & entry.metadata.keys():
            raise core.PitLanguageError(f"scene {source} entry {entry.entry_id} metadata contains compiler-owned fields")
        pointer_size = entry.metadata.get("pointer_table_size")
        entry_size = entry.metadata.get("source_entry_size")
        if not isinstance(pointer_size, int) or pointer_size < 2 or pointer_size & 1:
            raise core.PitLanguageError(f"scene {source} entry {entry.entry_id} has invalid pointer_table_size")
        if not isinstance(entry_size, int) or entry_size < pointer_size:
            raise core.PitLanguageError(f"scene {source} entry {entry.entry_id} has invalid source_entry_size")
        instructions: list[core.Instruction] = []
        label_offsets: dict[str, int] = {}
        ranges: list[tuple[int, int]] = []

        def bind(label: str, offset: int) -> None:
            if label in label_offsets:
                raise core.PitLanguageError(f"scene entry {entry.entry_id} repeats label {label}")
            label_offsets[label] = offset

        previous_anchor = -1
        for section_index, section in enumerate(entry.segments):
            if section.kind != "section" or section.source_offset is None or section.body is None:
                raise core.PitLanguageError(f"scene entry {entry.entry_id} has invalid section")
            if section.source_offset <= previous_anchor or section.source_offset < pointer_size:
                raise core.PitLanguageError(f"scene entry {entry.entry_id} section anchors must increase")
            previous_anchor = section.source_offset
            cursor = section.source_offset
            pending: list[str] = []
            for item in self.lowerer.lower(section.body):
                if isinstance(item, core.LabelMarker):
                    bind(item.name, cursor)
                    pending.append(item.name)
                elif isinstance(item, core.PaddingItem):
                    if pending:
                        raise core.PitLanguageError("labels cannot target scene padding")
                    cursor += item.halfwords * 2
                else:
                    item.offset = cursor
                    item.end = cursor + _instruction_size(item.opcode, self.dialect)
                    item.labels = pending
                    pending = []
                    instructions.append(item)
                    cursor = item.end
            ranges.append((section.source_offset, cursor))
        for left, right in zip(ranges, ranges[1:]):
            if left[1] > right[0]:
                raise core.PitLanguageError(f"scene entry {entry.entry_id} relocated sections overlap")
        if ranges and ranges[-1][1] > entry_size:
            raise core.PitLanguageError(f"scene entry {entry.entry_id} exceeds source_entry_size")
        entry_points = entry.metadata.get("entry_points")
        if not isinstance(entry_points, list):
            raise core.PitLanguageError(f"scene entry {entry.entry_id} needs entry_points")
        for label in entry_points:
            if label is not None and label not in label_offsets:
                raise core.PitLanguageError(f"scene entry {entry.entry_id} has undefined entry point {label!r}")

        resolved_targets: dict[tuple[int, int], int] = {}
        code_target_lists: dict[int, list[int]] = {}
        for instruction in instructions:
            ref_index = _relative_reference_index(self.dialect, instruction.opcode)
            target: int | None = None
            for index, argument in enumerate(instruction.arguments):
                if argument.kind != "label":
                    continue
                if index != ref_index or str(argument.value) not in label_offsets:
                    raise core.PitLanguageError(
                        f"scene command {core._format_offset(instruction.offset)} has invalid label operand"
                    )
                target = label_offsets[str(argument.value)]
                resolved_targets[(id(instruction), index)] = target
            code_target_lists[id(instruction)] = self.scene_successors(instruction, target)

        labels_at: dict[int, list[str]] = {}
        for label in entry_points:
            if label is not None:
                labels_at.setdefault(label_offsets[label], []).append(label)
        instruction_offsets = {instruction.offset for instruction in instructions}
        for instruction in instructions:
            for target in code_target_lists[id(instruction)]:
                if target in instruction_offsets:
                    labels_at.setdefault(target, []).append(f"loc_{target:04X}")
        for offset in labels_at:
            labels_at[offset] = list(dict.fromkeys(labels_at[offset]))

        commands = []
        occupied_bytes = 0
        for instruction in sorted(instructions, key=lambda item: item.offset):
            args = []
            for index, argument in enumerate(instruction.arguments):
                target = resolved_targets.get((id(instruction), index))
                if target is None:
                    args.append(self.argument_json(argument, "scene argument"))
                    continue
                distance = target - instruction.end
                if distance & 1 or not -0x8000 <= distance // 2 <= 0x7FFF:
                    raise core.PitLanguageError("scene relative target is unaligned or out of range")
                args.append(distance // 2)
            row: dict[str, Any] = {
                "source_offset": core._format_offset(instruction.offset),
                "opcode": self.dialect.opcode_name(instruction.opcode),
                "args": args,
            }
            if instruction.result is not None:
                row = {
                    "source_offset": row["source_offset"],
                    "opcode": row["opcode"],
                    "result": self.variable_name(instruction.result, "scene result"),
                    "args": args,
                }
            if instruction.unused_mode_bits:
                row["unused_mode_bits"] = f"0x{instruction.unused_mode_bits:04X}"
            labels = labels_at.get(instruction.offset)
            if labels:
                row["labels"] = labels
            targets = code_target_lists[id(instruction)]
            if targets:
                row["code_targets"] = [core._format_offset(target) for target in targets]
            commands.append(row)
            occupied_bytes += instruction.end - instruction.offset
        row = copy.deepcopy(entry.metadata)
        row["entry_id"] = entry.entry_id
        row["reachable_command_count"] = len(commands)
        row["private_byte_count"] = entry_size - pointer_size - occupied_bytes
        row["commands"] = commands
        return row

    def scene_successors(
        self, instruction: core.Instruction, relative_target: int | None
    ) -> list[int]:
        opcode = instruction.opcode
        if opcode in self.dialect.terminal_opcodes:
            return []
        if opcode in self.dialect.code_references:
            if relative_target is None:
                return [instruction.end]
            targets = [relative_target]
            if opcode in self.dialect.branch_with_fallthrough or (
                opcode == 0x02 and core._literal(instruction.arguments[0]) == 1
            ):
                targets.append(instruction.end)
            return targets
        if opcode in self.dialect.inline_opcodes or opcode in self.dialect.spawn_opcodes:
            if relative_target is None:
                return [instruction.end]
            return [instruction.end, relative_target]
        if opcode in self.dialect.wait_skip_opcodes:
            return [instruction.end] if relative_target is None else [relative_target]
        return []

    def variable_name(self, value: core.Value, context: str) -> str:
        if value.kind != "variable":
            raise core.PitLanguageError(f"{context} must be a variable")
        try:
            raw = self.dialect.parse_variable(str(value.value), context)
        except Exception as exc:
            raise core.PitLanguageError(str(exc)) from exc
        return self.dialect.format_variable(raw)

    def argument_json(self, value: core.Value, context: str) -> Any:
        if value.kind == "variable":
            return {"variable": self.variable_name(value, context)}
        if value.kind != "literal":
            raise core.PitLanguageError(f"{context} contains an unresolved label")
        literal = int(value.value)
        if not -0x8000 <= literal <= 0x7FFF:
            raise core.PitLanguageError(f"{context} does not fit signed 16 bits")
        return literal


def decompile_vm_json_to_script(document: dict[str, Any]) -> str:
    """Decompile a battle-v2 or scene-v1 JSON document."""
    schema = document.get("schema") if isinstance(document, dict) else None
    if schema == BATTLE_SCHEMA:
        return VmDecompiler(document, "battle").run()
    if schema == SCENE_SCHEMA:
        return VmDecompiler(document, "scene").run()
    raise core.PitLanguageError(f"unsupported VM document schema {schema!r}")


def compile_vm_script_to_json(source: str) -> dict[str, Any]:
    """Compile a battle or scene `.pit` source document."""
    return VmCompiler(source).run()


def _sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def decompile_all_vm_corpus(
    scripts_directory: Path,
    text_directory: Path,
    output_directory: Path,
    language: str = "german",
) -> dict[str, Any]:
    """Create one private editable tree containing all three PiT VMs."""
    field_json = scripts_directory / "FEvent__FEvData.dat"
    dialogue_path = text_directory / "FEvent__FEvData.dat.json"
    try:
        dialogue = json.loads(dialogue_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise core.PitLanguageError(f"cannot read {dialogue_path}: {exc}") from exc

    field_output = output_directory / "field"
    field_manifest = core.decompile_room_corpus(
        field_json, dialogue, field_output, language
    )

    battle_output = output_directory / "battle"
    battle_rows = []
    for path in sorted(scripts_directory.glob("BAI__*.json")):
        try:
            document = json.loads(path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as exc:
            raise core.PitLanguageError(f"cannot read {path}: {exc}") from exc
        if not isinstance(document, dict) or document.get("schema") != BATTLE_SCHEMA:
            continue
        script_name = path.name.removesuffix(".json") + ".pit"
        script_path = battle_output / script_name
        core._write_text_atomic(script_path, decompile_vm_json_to_script(document))
        battle_rows.append(
            {
                "source_json": path.name,
                "script": script_name,
                "source": document.get("source"),
                "entry_count": len(document.get("entries", [])),
                "command_count": document.get("command_count"),
                "source_json_sha256": _sha256(path),
            }
        )
    if not battle_rows:
        raise core.PitLanguageError(
            f"no {BATTLE_SCHEMA} documents found in {scripts_directory}"
        )

    scene_json_path = scripts_directory / "MenuAI__scene_scripts.json"
    try:
        scene_document = json.loads(scene_json_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise core.PitLanguageError(f"cannot read {scene_json_path}: {exc}") from exc
    if not isinstance(scene_document, dict) or scene_document.get("schema") != SCENE_SCHEMA:
        raise core.PitLanguageError(
            f"{scene_json_path} must use schema {SCENE_SCHEMA!r}"
        )
    scene_output = output_directory / "scene"
    scene_script_name = "MenuAI__scene_scripts.pit"
    core._write_text_atomic(
        scene_output / scene_script_name,
        decompile_vm_json_to_script(scene_document),
    )

    scene_command_count = sum(
        len(entry.get("commands", []))
        for archive in scene_document.get("archives", [])
        if isinstance(archive, dict)
        for entry in archive.get("entries", [])
        if isinstance(entry, dict)
    )
    manifest = {
        "schema": ALL_VM_CORPUS_SCHEMA,
        "version": "eur",
        "language": language,
        "field": {
            "directory": "field",
            "manifest": "field/corpus.json",
            "room_count": field_manifest["room_count"],
            "command_count": 0,
            "message_reference_count": field_manifest["message_reference_count"],
        },
        "battle": {
            "directory": "battle",
            "archive_count": len(battle_rows),
            "entry_count": sum(row["entry_count"] for row in battle_rows),
            "command_count": sum(
                int(row["command_count"] or 0) for row in battle_rows
            ),
            "documents": battle_rows,
        },
        "scene": {
            "directory": "scene",
            "source_json": scene_json_path.name,
            "script": scene_script_name,
            "archive_count": len(scene_document.get("archives", [])),
            "command_count": scene_command_count,
            "source_json_sha256": _sha256(scene_json_path),
        },
    }
    # The field command total is cheap to derive from the already-loaded room
    # JSON and is kept in the umbrella manifest for progress tooling.
    field_command_count = 0
    for path in sorted(field_json.glob("room_*.json")):
        room = json.loads(path.read_text(encoding="utf-8"))
        field_command_count += sum(
            int(member.get("reachable_command_count", 0))
            for member in room.get("members", [])
            if isinstance(member, dict)
        )
    manifest["field"]["command_count"] = field_command_count
    core._write_text_atomic(
        output_directory / "corpus.json",
        json.dumps(manifest, ensure_ascii=False, indent=2) + "\n",
    )
    core._write_text_atomic(
        output_directory / "README.md",
        "# Privater PiT-VM-Gesamtkorpus\n\n"
        "Dieser Ordner enthält die bearbeitbaren `.pit`-Quellen aller drei "
        "nachgewiesenen Script-VMs: Field/World, Battle AI und Scene/Object. "
        "Die deutschen Dialogzeilen liegen ausschließlich als ignorierte "
        "Kommentare im Field-Unterordner.\n\n"
        "Kompilieren in einen privaten JSON-Baum:\n\n"
        "```powershell\n"
        "python .\\tools\\pit_language_compiler.py compile-all-vms `\n"
        f"  {output_directory} `\n"
        "  .\\private\\modding\\vm_json_rebuilt\n"
        "```\n\n"
        "Battle-Quellen sind vollständig relokierbar. Scene/Object-Quellen "
        "behalten bis zur Klassifikation sämtlicher privater Bereiche feste "
        "Abschnittsanker bei.\n",
    )
    return manifest


def compile_all_vm_corpus(
    input_directory: Path, output_directory: Path
) -> dict[str, Any]:
    """Compile an umbrella VM corpus back into its private JSON documents."""
    manifest_path = input_directory / "corpus.json"
    try:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise core.PitLanguageError(f"cannot read {manifest_path}: {exc}") from exc
    if not isinstance(manifest, dict) or manifest.get("schema") != ALL_VM_CORPUS_SCHEMA:
        raise core.PitLanguageError(
            f"VM corpus manifest must use schema {ALL_VM_CORPUS_SCHEMA!r}"
        )

    field_result = core.compile_room_corpus(
        input_directory / "field", output_directory / "FEvent__FEvData.dat"
    )
    battle = manifest.get("battle")
    if not isinstance(battle, dict) or not isinstance(battle.get("documents"), list):
        raise core.PitLanguageError("VM corpus has invalid battle metadata")
    battle_count = 0
    for index, row in enumerate(battle["documents"]):
        if not isinstance(row, dict):
            raise core.PitLanguageError(f"battle corpus row {index} must be an object")
        script_name = row.get("script")
        json_name = row.get("source_json")
        if (
            not isinstance(script_name, str)
            or Path(script_name).name != script_name
            or not script_name.endswith(".pit")
            or not isinstance(json_name, str)
            or Path(json_name).name != json_name
            or not json_name.endswith(".json")
        ):
            raise core.PitLanguageError(f"battle corpus row {index} has unsafe paths")
        source = (input_directory / "battle" / script_name).read_text(
            encoding="utf-8"
        )
        document = compile_vm_script_to_json(source)
        if document.get("schema") != BATTLE_SCHEMA:
            raise core.PitLanguageError(f"{script_name} did not compile as Battle VM")
        core._write_text_atomic(
            output_directory / json_name,
            json.dumps(document, ensure_ascii=False, indent=2) + "\n",
        )
        battle_count += 1

    scene = manifest.get("scene")
    if not isinstance(scene, dict):
        raise core.PitLanguageError("VM corpus has invalid scene metadata")
    scene_script = scene.get("script")
    scene_json = scene.get("source_json")
    if (
        not isinstance(scene_script, str)
        or Path(scene_script).name != scene_script
        or not isinstance(scene_json, str)
        or Path(scene_json).name != scene_json
    ):
        raise core.PitLanguageError("VM corpus scene paths are unsafe")
    scene_document = compile_vm_script_to_json(
        (input_directory / "scene" / scene_script).read_text(encoding="utf-8")
    )
    if scene_document.get("schema") != SCENE_SCHEMA:
        raise core.PitLanguageError("scene corpus source did not compile as Scene VM")
    core._write_text_atomic(
        output_directory / scene_json,
        json.dumps(scene_document, ensure_ascii=False, indent=2) + "\n",
    )
    return {
        "schema": ALL_VM_CORPUS_SCHEMA,
        "field_room_count": field_result["room_count"],
        "battle_archive_count": battle_count,
        "scene_archive_count": len(scene_document.get("archives", [])),
    }
