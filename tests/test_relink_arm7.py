from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import reassembly  # noqa: E402
import relink_arm7  # noqa: E402


class Arm7LayoutTests(unittest.TestCase):
    def test_resolves_arm_calls_and_literal_loads(self) -> None:
        self.assertEqual(
            relink_arm7.resolve_arm_relocation(0xEB00003E, 0x1000, "arm_call"),
            0x1100,
        )
        self.assertEqual(
            relink_arm7.resolve_arm_relocation(0xE59F0010, 0x2000, "load"),
            0x2018,
        )
        with self.assertRaises(reassembly.ReassemblyError):
            relink_arm7.resolve_arm_relocation(0xE1A00000, 0x2000, "load")

    def synthetic_layout(self) -> tuple[bytes, reassembly.Module]:
        load_address = 0x02380000
        rom_offset = 0x20
        payload_size = 0x90
        data = bytearray(rom_offset + payload_size)
        container = reassembly.Module(
            "arm7", "arm7", rom_offset, payload_size, load_address
        )

        resident_size = 0x30
        first_size = 0x20
        second_size = 0x28
        table_address = load_address + resident_size + first_size + second_size
        struct.pack_into(
            "<6I",
            data,
            rom_offset + 0x10,
            table_address,
            load_address + payload_size,
            load_address + resident_size,
            load_address + resident_size,
            load_address + resident_size,
            0,
        )
        table_offset = rom_offset + table_address - load_address
        struct.pack_into("<3I", data, table_offset, 0x037F8000, first_size, 0x10)
        struct.pack_into(
            "<3I", data, table_offset + 12, 0x027E0000, second_size, 0x18
        )
        return bytes(data), container

    def test_parses_resident_and_autoload_components(self) -> None:
        data, container = self.synthetic_layout()
        layout = relink_arm7.parse_layout_candidate(data, container, 0x10)
        self.assertIsNotNone(layout)
        assert layout is not None
        self.assertEqual(layout.module_parameters_address, 0x02380010)
        self.assertEqual(layout.resident.size, 0x30)
        self.assertEqual(
            [module.name for module in layout.autoloads],
            ["arm7_autoload_000", "arm7_autoload_001"],
        )
        self.assertEqual(
            [module.rom_offset for module in layout.autoloads], [0x50, 0x70]
        )
        self.assertEqual(
            [module.load_address for module in layout.autoloads],
            [0x037F8000, 0x027E0000],
        )
        self.assertEqual([module.bss_size for module in layout.autoloads], [0x10, 0x18])

    def test_rejects_descriptor_sizes_that_do_not_cover_payload(self) -> None:
        data, container = self.synthetic_layout()
        corrupted = bytearray(data)
        struct.pack_into("<I", corrupted, 0x98 + 4, 0x1C)
        self.assertIsNone(
            relink_arm7.parse_layout_candidate(bytes(corrupted), container, 0x10)
        )


if __name__ == "__main__":
    unittest.main()
