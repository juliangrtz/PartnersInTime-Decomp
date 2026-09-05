import json
from pathlib import Path
import sys
import unittest


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import extract_script_vm_descriptors as descriptor_tool


class ScriptVmDescriptorTests(unittest.TestCase):
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


if __name__ == "__main__":
    unittest.main()
