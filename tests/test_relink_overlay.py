from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import reassembly  # noqa: E402
import relink_overlay  # noqa: E402


class DsdParsingTests(unittest.TestCase):
    def test_reads_sections_and_stops_at_translation_units(self) -> None:
        text = """\
    .text       start:0x02001000 end:0x02001020 kind:code align:32
    .rodata     start:0x02001020 end:0x02001028 kind:rodata align:4

translation_unit.o:
    .text       start:0x02001000 end:0x02001020 kind:code align:4
"""
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "delinks.txt"
            path.write_text(text, encoding="utf-8")
            sections = relink_overlay.read_sections(path)

        self.assertEqual([section.name for section in sections], ["text", "rodata"])
        self.assertEqual(sections[0].start, 0x02001000)
        self.assertEqual(sections[1].end, 0x02001028)

    def test_reads_relocation_addends(self) -> None:
        text = (
            "from:0x02001004 kind:load to:0x02002000 "
            "add:0x0100 module:overlay(2)\n"
        )
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "relocs.txt"
            path.write_text(text, encoding="utf-8")
            relocations = relink_overlay.read_relocations(path)

        self.assertEqual(
            relocations,
            [
                {
                    "source": 0x02001004,
                    "kind": "load",
                    "target": 0x02002000,
                    "add": 0x100,
                    "module": "overlay(2)",
                }
            ],
        )


class UnitPlanningTests(unittest.TestCase):
    def test_sections_cover_module_and_patch_splits_code(self) -> None:
        module = reassembly.Module(
            "arm9_ov002", "arm9", 0x100, 0x30, 0x02001000
        )
        sections = [
            relink_overlay.DsdSection("text", 0x02001000, 0x02001020, "code", 4),
            relink_overlay.DsdSection("data", 0x02001028, 0x02001030, "data", 4),
            relink_overlay.DsdSection("bss", 0x02001030, 0x02001040, "bss", 4),
        ]
        intervals = relink_overlay.raw_intervals(module, sections)
        self.assertEqual(
            intervals,
            [
                ("text", "code", 0x02001000, 0x02001020),
                ("gap_000", "gap", 0x02001020, 0x02001028),
                ("data", "data", 0x02001028, 0x02001030),
            ],
        )

        patches = [
            {
                "address": "0x02001008",
                "size": "0x8",
                "section": ".pit_test",
                "source": "battle/test.s",
            }
        ]
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            units = relink_overlay.plan_units(
                module,
                intervals,
                patches,
                directory / "generated",
                directory / "maintained",
            )

        self.assertEqual(
            [(unit.start, unit.end) for unit in units],
            [
                (0x02001000, 0x02001008),
                (0x02001008, 0x02001010),
                (0x02001010, 0x02001020),
                (0x02001020, 0x02001028),
                (0x02001028, 0x02001030),
            ],
        )
        self.assertEqual(
            [unit.kind for unit in units],
            ["code", "code", "code", "gap", "data"],
        )
        self.assertTrue(units[1].maintained)
        self.assertTrue(
            all(left.end == right.start for left, right in zip(units, units[1:]))
        )


if __name__ == "__main__":
    unittest.main()
