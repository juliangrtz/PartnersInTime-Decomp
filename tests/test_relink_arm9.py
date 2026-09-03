from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import reassembly  # noqa: E402
import relink_arm9  # noqa: E402
import relink_overlay  # noqa: E402


class Arm9LayoutTests(unittest.TestCase):
    def test_align_up_requires_power_of_two(self) -> None:
        self.assertEqual(relink_arm9.align_up(0x794, 32), 0x7A0)
        self.assertEqual(relink_arm9.align_up(0x60, 32), 0x60)
        with self.assertRaises(reassembly.ReassemblyError):
            relink_arm9.align_up(7, 3)

    def test_stored_bounds_excludes_bss(self) -> None:
        sections = [
            relink_overlay.DsdSection("text", 0x1000, 0x1024, "code", 4),
            relink_overlay.DsdSection("data", 0x1028, 0x1030, "data", 4),
            relink_overlay.DsdSection("bss", 0x1040, 0x1050, "bss", 4),
        ]
        self.assertEqual(relink_arm9.stored_bounds(sections), (0x1000, 0x1030, 0x10))

    def test_validates_autoload_descriptor_values(self) -> None:
        modules = (
            reassembly.Module("itcm", "arm9", 0x20, 0x40, 0x01FF8000, bss_size=8),
            reassembly.Module("dtcm", "arm9", 0x60, 0x20, 0x027E0000, bss_size=0),
        )
        data = bytearray(0x40)
        struct.pack_into("<3I", data, 4, 0x01FF8000, 0x40, 8)
        struct.pack_into("<3I", data, 16, 0x027E0000, 0x20, 0)
        relink_arm9.validate_autoload_descriptors(bytes(data), 4, modules)

        struct.pack_into("<I", data, 20, 0x24)
        with self.assertRaises(reassembly.ReassemblyError):
            relink_arm9.validate_autoload_descriptors(bytes(data), 4, modules)


if __name__ == "__main__":
    unittest.main()
