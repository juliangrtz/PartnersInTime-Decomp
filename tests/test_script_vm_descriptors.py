import json
from pathlib import Path
import sys
import unittest


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import extract_script_vm_descriptors as descriptor_tool


class ScriptVmDescriptorTests(unittest.TestCase):
    def test_generated_instances_do_not_share_name_dictionaries(self) -> None:
        instance = {
            "role": "test",
            "overlay_id": 0,
            "descriptor_table_address": 0,
            "command_handler_address": 0,
            "vm_run_call_sites": [],
        }
        names = {"0": "end"}
        first = descriptor_tool._document("eur", "first", instance, [0], names)
        second = descriptor_tool._document("eur", "second", instance, [0], names)
        first["known_names"]["1"] = "instance_only"
        self.assertNotIn("1", second["known_names"])
        self.assertNotIn("1", names)

    def test_checked_in_instance_tables_share_the_resident_prefix(self) -> None:
        battle = json.loads(
            (ROOT / "config/eur/battle_ai_vm.json").read_text(encoding="utf-8")
        )
        tables = [[int(value, 0) for value in battle["descriptors"]]]
        for name, count in (("field", 0x155), ("scene", 0x0D2)):
            document = json.loads(
                (ROOT / f"config/eur/{name}_vm.json").read_text(encoding="utf-8")
            )
            self.assertEqual(document["schema"], descriptor_tool.SCHEMA)
            self.assertEqual(document["generic_opcode_count"], 0x33)
            descriptors = [int(value, 0) for value in document["descriptors"]]
            self.assertEqual(len(descriptors), count)
            self.assertTrue(all(0 <= value <= 0x7F for value in descriptors))
            tables.append(descriptors)

        for opcode in range(descriptor_tool.GENERIC_OPCODE_COUNT):
            self.assertEqual(len({table[opcode] for table in tables}), 1)

    def test_battle_vm_has_complete_names_and_instance_contracts(self) -> None:
        battle = json.loads(
            (ROOT / "config/eur/battle_ai_vm.json").read_text(encoding="utf-8")
        )
        descriptors = [int(value, 0) for value in battle["descriptors"]]
        names = {int(opcode, 0): name for opcode, name in battle["known_names"].items()}
        semantics = {
            int(opcode, 0): contract
            for opcode, contract in battle["opcode_semantics"].items()
        }

        self.assertEqual(set(names), set(range(len(descriptors))))
        self.assertEqual(
            set(semantics),
            set(range(descriptor_tool.GENERIC_OPCODE_COUNT, len(descriptors))),
        )
        self.assertEqual(len(set(names.values())), len(names))

        for opcode, contract in semantics.items():
            descriptor = descriptors[opcode]
            self.assertEqual(len(contract["arguments"]), descriptor & 0x1F)
            self.assertEqual(contract["result"] is not None, bool(descriptor & 0x20))
            self.assertTrue(contract["control_flow"])
            self.assertTrue(contract["yield"])
            self.assertTrue(contract.get("effect") or contract["control_flow"])
            self.assertTrue(contract["evidence"])


if __name__ == "__main__":
    unittest.main()
