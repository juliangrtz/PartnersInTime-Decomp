from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools"
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import field_event_mod
import pit_language_compiler


class PitLanguageCompilerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.descriptors, cls.names = field_event_mod.load_vm_schema("eur")
        cls.opcodes = {name: opcode for opcode, name in cls.names.items()}

    def command_size(self, name: str) -> int:
        opcode = self.opcodes[name]
        descriptor = self.descriptors[opcode]
        count = descriptor & 0x1F
        return 2 + bool(descriptor & 0x20) * 2 + bool(
            descriptor & 0x40 and count
        ) * 2 + count * 2

    def synthetic_room(self) -> dict:
        commands: list[dict] = []
        offset = 0x0020

        def add(name: str, args: list, **extra: object) -> dict:
            nonlocal offset
            row = {
                "source_offset": f"0x{offset:04X}",
                "opcode": name,
                "args": args,
                **extra,
            }
            commands.append(row)
            offset += self.command_size(name)
            return row

        add(
            "open_entity_message",
            [7, 0, 0, 1, 1, -1, 0, -1, 0, -1, 0, 2],
            result="state_24[0]",
            labels=["script_000"],
        )
        add("start_camera_shake", [1, 3, 4096, 4, -1])
        inline = add("start_inline_entity_script", [7, 0, 0])
        body_start = offset
        add("set_entity_position", [7, 0, {"variable": "context[2]"}, 300, 96])
        add("return", [])
        resume = offset
        inline["args"][2] = (resume - body_start) // 2
        inline["code_targets"] = [f"0x{body_start:04X}", f"0x{resume:04X}"]
        commands[3]["labels"] = [f"loc_{body_start:04X}"]

        add("push", [3], labels=[f"loc_{resume:04X}"])
        loop_offset = offset
        loop = add("loop", [0x12, 0, 0, 0], labels=[f"loc_{loop_offset:04X}"])
        add("wait", [1])
        jump = add("jump", [2, 0])
        exit_offset = offset
        loop["args"][3] = (exit_offset - (loop_offset + self.command_size("loop"))) // 2
        loop["code_targets"] = [f"0x{exit_offset:04X}"]
        jump_offset = int(jump["source_offset"], 0)
        jump["args"][1] = (
            loop_offset - (jump_offset + self.command_size("jump"))
        ) // 2
        jump["code_targets"] = [f"0x{loop_offset:04X}"]

        add("rejoin_party_follower", [0, 1], labels=[f"loc_{exit_offset:04X}"])
        profile = add("add_entity_roaming_profile", [7, 0])
        add("end", [])
        profile_offset = 0x00E0
        profile_end = int(profile["source_offset"], 0) + self.command_size(
            "add_entity_roaming_profile"
        )
        profile["args"][1] = (profile_offset - profile_end) // 2
        profile["data_record"] = "roaming_profile_00E0"

        return {
            "schema": "pit-field-event-room-v1",
            "source": "FEvent/FEvData.dat",
            "room_id": 0,
            "members": [
                {
                    "room_id": 0,
                    "room_part": 0,
                    "source_entry_id": 0,
                    "source_entry_size": 0x0100,
                    "source_sha1": "synthetic",
                    "pointer_table_size": 0x20,
                    "fixed_sections": [],
                    "script_slots": ["script_000"],
                    "valid_script_count": 1,
                    "private_target_count": 0,
                    "reachable_command_count": len(commands),
                    "embedded_data_record_count": 1,
                    "embedded_data_records": [
                        {
                            "label": "roaming_profile_00E0",
                            "source_offset": "0x00E0",
                            "type": "entity_roaming_profile",
                            "size_words": 5,
                            "legacy_parameter": 0,
                            "speed_q12": 4096,
                            "step_distance": 30,
                            "post_step_delay_frames": 20,
                            "direction_count": 4,
                        }
                    ],
                    "commands": commands,
                }
            ],
        }

    def test_lossless_round_trip_uses_structured_syntax(self) -> None:
        original = self.synthetic_room()
        source = pit_language_compiler.decompile_json_to_script(original)
        self.assertIn("Actor.OpenMessage(", source)
        self.assertIn("Camera.StartShake(", source)
        self.assertIn("Party.RejoinFollower(", source)
        self.assertIn("async Actor.Run(7, 0) {", source)
        self.assertIn("for (loop_counter = 3; loop_counter != 0; loop_counter--)", source)
        self.assertIn("context[2]", source)
        self.assertIn("// [Choreography]", source)
        self.assertEqual(
            pit_language_compiler.compile_script_to_json(source), original
        )

    def test_async_size_and_branch_offsets_are_relocated_after_an_edit(self) -> None:
        source = pit_language_compiler.decompile_json_to_script(self.synthetic_room())
        source = source.replace(
            "      Flow.Return();",
            "      VM.Wait(5);\n      Flow.Return();",
            1,
        )
        compiled = pit_language_compiler.compile_script_to_json(source)
        commands = compiled["members"][0]["commands"]
        inline = next(
            command
            for command in commands
            if command["opcode"] == "start_inline_entity_script"
        )
        self.assertEqual(inline["args"][2], 11)
        resume = int(inline["code_targets"][1], 0)
        push = next(command for command in commands if command["opcode"] == "push")
        self.assertEqual(int(push["source_offset"], 0), resume)
        loop = next(command for command in commands if command["opcode"] == "loop")
        loop_end = int(loop["source_offset"], 0) + self.command_size("loop")
        self.assertEqual(
            loop_end + loop["args"][3] * 2,
            int(loop["code_targets"][0], 0),
        )

    def test_literal_and_variable_arguments_remain_distinct(self) -> None:
        source = pit_language_compiler.decompile_json_to_script(self.synthetic_room())
        compiled = pit_language_compiler.compile_script_to_json(source)
        position = next(
            command
            for command in compiled["members"][0]["commands"]
            if command["opcode"] == "set_entity_position"
        )
        self.assertEqual(position["args"][1], 0)
        self.assertEqual(position["args"][2], {"variable": "context[2]"})

    def test_padding_contributes_to_inline_size_without_becoming_an_opcode(self) -> None:
        source = pit_language_compiler.decompile_json_to_script(self.synthetic_room())
        source = source.replace(
            "      Flow.Return();",
            "      Flow.Return();\n      padding 2;",
            1,
        )
        compiled = pit_language_compiler.compile_script_to_json(source)
        commands = compiled["members"][0]["commands"]
        inline = next(
            command
            for command in commands
            if command["opcode"] == "start_inline_entity_script"
        )
        self.assertEqual(inline["args"][2], 10)
        self.assertEqual(len(commands), len(self.synthetic_room()["members"][0]["commands"]))

    def test_rejects_variable_when_descriptor_bit_six_is_clear(self) -> None:
        source = pit_language_compiler.decompile_json_to_script(self.synthetic_room())
        source = source.replace("Flow.Return();", "Flow.Jump(context[0], 0);", 1)
        with self.assertRaisesRegex(
            pit_language_compiler.PitLanguageError,
            "cannot be a variable",
        ):
            pit_language_compiler.compile_script_to_json(source)

    def test_comments_do_not_affect_compilation(self) -> None:
        original = self.synthetic_room()
        source = pit_language_compiler.decompile_json_to_script(original)
        source = source.replace(
            "room 0 {", "// arbitrary editor note\nroom 0 {", 1
        )
        self.assertEqual(
            pit_language_compiler.compile_script_to_json(source), original
        )

    @staticmethod
    def dialogue_catalog() -> dict:
        return {
            "schema": "pit-localized-dialogue-v1",
            "containers": [
                {
                    "room_id": 0,
                    "languages": [
                        {
                            "language": "german",
                            "strings": [
                                {
                                    "id": 0,
                                    "event_label": "Test_00",
                                    "text": "Nicht verwendet.<$END>",
                                },
                                {
                                    "id": 1,
                                    "event_label": "Test_01",
                                    "text": "Ebenfalls nicht verwendet.<$END>",
                                },
                                {
                                    "id": 2,
                                    "event_label": "Test_02",
                                    "text": "Die Zeitmaschine startet!<$WAIT:00>Alle festhalten.<$END>",
                                },
                            ],
                        }
                    ],
                }
            ],
        }

    def test_private_german_dialogue_is_searchable_but_non_semantic(self) -> None:
        original = self.synthetic_room()
        source = pit_language_compiler.decompile_json_to_script_with_messages(
            original, self.dialogue_catalog(), "german"
        )
        self.assertIn("// [Nachricht DE · Test_02]", source)
        self.assertIn("// Die Zeitmaschine startet!", source)
        self.assertIn("// Alle festhalten.", source)
        self.assertEqual(
            pit_language_compiler.compile_script_to_json(source), original
        )

    def test_private_corpus_generation_and_compilation(self) -> None:
        original = self.synthetic_room()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            rooms = root / "rooms"
            corpus = root / "corpus"
            rebuilt = root / "rebuilt"
            rooms.mkdir()
            (rooms / "room_000.json").write_text(
                json.dumps(original), encoding="utf-8"
            )
            manifest = pit_language_compiler.decompile_room_corpus(
                rooms, self.dialogue_catalog(), corpus, "german"
            )
            self.assertEqual(manifest["room_count"], 1)
            self.assertEqual(manifest["message_reference_count"], 1)
            self.assertTrue((corpus / "README.md").is_file())
            result = pit_language_compiler.compile_room_corpus(corpus, rebuilt)
            self.assertEqual(result["room_count"], 1)
            self.assertEqual(
                json.loads(
                    (rebuilt / "room_000.json").read_text(encoding="utf-8")
                ),
                original,
            )


if __name__ == "__main__":
    unittest.main()
