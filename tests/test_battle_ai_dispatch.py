from __future__ import annotations

import importlib.util
import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools"
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))
SPEC = importlib.util.spec_from_file_location(
    "pit_battle_ai_dispatch", TOOLS / "analyze_battle_ai_dispatch.py"
)
assert SPEC is not None and SPEC.loader is not None
dispatch = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = dispatch
SPEC.loader.exec_module(dispatch)


def encode_branch(address: int, target: int, link: bool = False) -> int:
    displacement = (target - address - 8) // 4
    return 0xEA000000 | (0x01000000 if link else 0) | (displacement & 0xFFFFFF)


class BattleAiDispatchTests(unittest.TestCase):
    def test_decodes_forward_and_backward_arm_branches(self) -> None:
        for address, target in ((0x2000, 0x2100), (0x2100, 0x2000)):
            word = encode_branch(address, target)
            self.assertEqual(dispatch.decode_arm_branch(address, word), target)

    def test_extracts_every_opcode_target(self) -> None:
        load_address = dispatch.TABLE_ADDRESS - 0x20
        payload = bytearray(0x20 + dispatch.OPCODE_COUNT * 4)
        expected = []
        for index in range(dispatch.OPCODE_COUNT):
            address = dispatch.TABLE_ADDRESS + index * 4
            target = dispatch.DISPATCH_ADDRESS + 0x100 + index * 4
            expected.append(target)
            struct.pack_into(
                "<I", payload, 0x20 + index * 4, encode_branch(address, target)
            )
        self.assertEqual(dispatch.extract_dispatch_table(bytes(payload), load_address), expected)

    def test_rejects_non_branch_table_entries(self) -> None:
        load_address = dispatch.TABLE_ADDRESS
        payload = bytes(dispatch.OPCODE_COUNT * 4)
        with self.assertRaises(ValueError):
            dispatch.extract_dispatch_table(payload, load_address)


if __name__ == "__main__":
    unittest.main()
