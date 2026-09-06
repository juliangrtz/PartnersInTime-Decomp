from __future__ import annotations

import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools"
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import data_mod
import pit_language_compiler
import scene_script_mod


class PitAdditionalVmLanguageTests(unittest.TestCase):
    @staticmethod
    def command_size(descriptors: tuple[int, ...], opcode: int) -> int:
        descriptor = descriptors[opcode]
        count = descriptor & 0x1F
        return 2 * (
            1
            + bool(descriptor & 0x20)
            + bool(descriptor & 0x40 and count)
            + count
        )

    def synthetic_battle(self) -> dict:
        descriptors, names = data_mod.load_battle_vm_schema("eur")
        opcodes = {name: opcode for opcode, name in names.items()}
        commands = []
        offset = 4

        def add(name: str, args: list, **extra: object) -> dict:
            nonlocal offset
            command = {
                "opcode": name,
                "args": args,
                "source_offset": f"0x{offset:04X}",
                **extra,
            }
            commands.append(command)
            offset += self.command_size(descriptors, opcodes[name])
            return command

        inline = add(
            "start_inline_object_script",
            [28, 0, {"label": "loc_0000"}],
            labels=["entry_000"],
        )
        add("wait", [2])
        add("return", [])
        resume_offset = offset
        resume_label = f"loc_{resume_offset:04X}"
        inline["args"][2]["label"] = resume_label
        add(
            "apply_status",
            [28, 6, 50, 3],
            result="state_24[0]",
            labels=[resume_label],
        )
        add("return", [])
        private_offset = offset
        return {
            "schema": data_mod.BATTLE_SCRIPT_SCHEMA,
            "source": "BAI/synthetic.dat",
            "source_sha1": "synthetic",
            "layout": "relocatable-control-flow",
            "command_count": len(commands),
            "entries": [
                {
                    "entry_id": 0,
                    "source_entry_size": private_offset + 2,
                    "header_size": 4,
                    "entry_points": ["entry_000", None],
                    "reachable_command_count": len(commands),
                    "segments": [
                        {"kind": "code", "commands": commands},
                        {
                            "kind": "private_data",
                            "source_offset": f"0x{private_offset:04X}",
                            "size": 2,
                            "sha1": "synthetic-private",
                        },
                    ],
                }
            ],
        }

    def test_battle_round_trip_structures_async_and_status(self) -> None:
        original = self.synthetic_battle()
        source = pit_language_compiler.decompile_json_to_script(original)
        self.assertIn("battle \"BAI/synthetic.dat\"", source)
        self.assertIn("async Actor.Run(28, 0) {", source)
        self.assertIn("Status.POWER_CHANGE", source)
        self.assertEqual(
            pit_language_compiler.compile_script_to_json(source), original
        )

    def test_battle_async_target_is_relocated_after_edit(self) -> None:
        source = pit_language_compiler.decompile_json_to_script(
            self.synthetic_battle()
        )
        source = source.replace(
            "        Flow.Return();",
            "        VM.Wait(5);\n        Flow.Return();",
            1,
        )
        rebuilt = pit_language_compiler.compile_script_to_json(source)
        commands = rebuilt["entries"][0]["segments"][0]["commands"]
        inline = commands[0]
        resume_label = inline["args"][2]["label"]
        resume = next(
            command for command in commands if resume_label in command.get("labels", [])
        )
        original_resume = self.synthetic_battle()["entries"][0]["segments"][0][
            "commands"
        ][3]
        self.assertEqual(inline["opcode"], "start_inline_object_script")
        self.assertGreater(
            int(resume["source_offset"], 0), int(original_resume["source_offset"], 0)
        )

    def synthetic_scene(self) -> dict:
        descriptors, names = scene_script_mod.load_vm_schema("eur")
        opcodes = {name: opcode for opcode, name in names.items()}
        set_size = self.command_size(descriptors, opcodes["set"])
        return_size = self.command_size(descriptors, opcodes["return"])
        entries = []
        for index, source in enumerate(scene_script_mod.SOURCES):
            commands = [
                {
                    "source_offset": "0x0002",
                    "opcode": "set",
                    "result": "scene.shared[0]",
                    "args": [{"variable": "scene.owner_id"}],
                    "labels": ["entry_000"],
                },
                {
                    "source_offset": f"0x{2 + set_size:04X}",
                    "opcode": "return",
                    "args": [],
                },
            ]
            entries.append(
                {
                    "source": source,
                    "source_sha1": f"synthetic-{index}",
                    "outer_entry_count": 1,
                    "entries": [
                        {
                            "entry_id": 0,
                            "source_entry_size": 2 + set_size + return_size,
                            "source_sha1": f"synthetic-entry-{index}",
                            "pointer_table_size": 2,
                            "entry_points": ["entry_000"],
                            "reachable_command_count": 2,
                            "private_byte_count": 0,
                            "commands": commands,
                        }
                    ],
                }
            )
        return {
            "schema": scene_script_mod.SCHEMA,
            "version": "eur",
            "layout": "fixed-command-boundaries",
            "archives": entries,
        }

    def test_scene_round_trip_preserves_scene_variable_namespace(self) -> None:
        original = self.synthetic_scene()
        source = pit_language_compiler.decompile_json_to_script(original)
        self.assertIn("scene.shared[0]", source)
        self.assertIn("scene.owner_id", source)
        self.assertEqual(
            pit_language_compiler.compile_script_to_json(source), original
        )


if __name__ == "__main__":
    unittest.main()
