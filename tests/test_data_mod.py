from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools"
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import data_mod


class OffsetArchiveTests(unittest.TestCase):
    def test_round_trips_empty_entries(self) -> None:
        entries = [b"first", b"", b"third"]
        archive = data_mod.build_offset_archive(entries)
        self.assertEqual(data_mod.parse_offset_archive(archive), entries)


class TextCodecTests(unittest.TestCase):
    def test_round_trips_controls_raw_bytes_and_escaped_literals(self) -> None:
        raw = (
            b"Cr\xe8me <path>\\file"
            b"\xFF\x00next"
            b"\xFF\x0B\x03"
            b"\xFF\x01\x7F"
            b"\x09"
            b"\xFF\x0A"
        )
        editable = data_mod.decode_text(raw, "english")
        self.assertIn("\n", editable)
        self.assertIn("<$TEXTBOX:03>", editable)
        self.assertIn("<$CTRL:01:7F>", editable)
        self.assertIn("\\<path>", editable)
        self.assertEqual(data_mod.encode_text(editable, "english"), raw)

    def test_japanese_bytes_remain_explicit_until_font_map_exists(self) -> None:
        raw = b"\xD4\x40\x04\xFF\x0A"
        editable = data_mod.decode_text(raw, "japanese")
        self.assertEqual(
            editable, "<$BYTE:D4><$BYTE:40><$BYTE:04><$END>"
        )
        self.assertEqual(data_mod.encode_text(editable, "japanese"), raw)


class MfsetTests(unittest.TestCase):
    def test_rebuilds_offsets_after_text_length_change(self) -> None:
        original_rows = [
            {"header_hex": "18 07", "text": "One<$END>"},
            {"header_hex": "1a 05", "text": "Two<$END>"},
        ]
        original = data_mod.build_mfset_entry(
            original_rows, "english", header_size=2
        )
        parsed = data_mod.parse_mfset_entry(original, "english", header_size=2)
        self.assertEqual(parsed, original_rows)

        parsed[0]["text"] = "A much longer first string<$END>"
        rebuilt = data_mod.build_mfset_entry(parsed, "english", header_size=2)
        table_size, second_pointer = struct.unpack_from("<II", rebuilt)
        self.assertEqual(table_size, 8)
        self.assertGreater(second_pointer, 8)
        self.assertEqual(
            data_mod.parse_mfset_entry(rebuilt, "english", header_size=2), parsed
        )


class EnemyTableTests(unittest.TestCase):
    def test_round_trips_complete_record_layout(self) -> None:
        record = data_mod.ENEMY_STRUCT.pack(
            3,
            0xA012,
            4,
            7,
            100,
            20,
            30,
            40,
            0x0600,
            2,
            bytes(range(14)),
            50,
            6,
            0x12345678,
            0x9ABCDEF0,
        )
        document = {
            "schema": data_mod.ENEMY_SCHEMA,
            "source": "BData/BDataMon.dat",
            "source_sha1": data_mod.sha1(record),
            "record_size": data_mod.ENEMY_STRUCT.size,
            "records": [
                {
                    "record_id": 0,
                    "name_id": 3,
                    "name_hint": "ignored",
                    "flags_or_ai_id": "0xA012",
                    "unknown_04": "0x04",
                    "level": 7,
                    "max_hp": 100,
                    "power": 20,
                    "defense": 30,
                    "speed": 40,
                    "traits": "0x0600",
                    "unknown_10": "0x0002",
                    "unknown_12_hex": "00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d",
                    "experience": 50,
                    "coins": 6,
                    "item_drop_1": "0x12345678",
                    "item_drop_2": "0x9ABCDEF0",
                }
            ],
        }
        self.assertEqual(data_mod.build_enemy_stats(document, record), record)


if __name__ == "__main__":
    unittest.main()
