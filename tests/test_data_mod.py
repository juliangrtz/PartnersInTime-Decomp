from __future__ import annotations

import struct
import sys
import tempfile
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

    def test_merges_disjoint_views_of_the_same_archive(self) -> None:
        source = data_mod.build_offset_archive([b"first", b"second", b"third"])
        first = data_mod.build_offset_archive([b"FIRST-LONGER", b"second", b"third"])
        second = data_mod.build_offset_archive([b"first", b"second", b"THIRD"])
        merged = data_mod.merge_disjoint_offset_archive_edits(
            source, first, second, "test archive"
        )
        self.assertEqual(
            data_mod.parse_offset_archive(merged),
            [b"FIRST-LONGER", b"second", b"THIRD"],
        )

    def test_rejects_conflicting_archive_member_edits(self) -> None:
        source = data_mod.build_offset_archive([b"original"])
        first = data_mod.build_offset_archive([b"first edit"])
        second = data_mod.build_offset_archive([b"second edit"])
        with self.assertRaisesRegex(data_mod.DataModError, "conflicting edits"):
            data_mod.merge_disjoint_offset_archive_edits(
                source, first, second, "test archive"
            )


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

    def test_rebuilds_fevent_chunk_with_event_labels(self) -> None:
        rows = [
            {
                "header_hex": "0c 05",
                "text": "Hello<$END>",
                "event_label": "Mario_00",
            }
        ]
        inner = data_mod.build_mfset_entry(rows, "english", header_size=2)
        metadata = data_mod.build_mfset_entry(
            [{"text": "Mario_00"}], "english"
        )
        source = struct.pack("<I", len(inner)) + inner + metadata
        chunk_format, parsed = data_mod.parse_dialogue_chunk(source, "english")
        self.assertEqual(chunk_format, "fevent-mfset")
        parsed[0]["text"] = "A longer greeting<$END>"
        rebuilt = data_mod.build_dialogue_chunk(
            parsed, "english", chunk_format, source
        )
        rebuilt_text_end = 4 + struct.unpack_from("<I", rebuilt)[0]
        self.assertEqual(rebuilt[rebuilt_text_end:], metadata)
        self.assertEqual(
            data_mod.parse_dialogue_chunk(rebuilt, "english")[1], parsed
        )

    def test_round_trips_localized_container_with_empty_slots(self) -> None:
        segments = [b"" for _ in range(91)]
        segments[0] = b"metadata"
        segments[84] = b"japanese"
        segments[85] = b"english"
        container = data_mod.build_localized_container(segments)
        self.assertEqual(data_mod.parse_localized_container(container), segments)


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


class TreasureTableTests(unittest.TestCase):
    def test_round_trips_archive_and_record_layout(self) -> None:
        entry = data_mod.TREASURE_STRUCT.pack(1, 2, 300, 4, 500, 600, 700)
        source = data_mod.build_offset_archive([entry, b""])
        document = {
            "schema": data_mod.TREASURE_SCHEMA,
            "source": "Treasure/TreasureInfo.dat",
            "source_sha1": data_mod.sha1(source),
            "record_size": data_mod.TREASURE_STRUCT.size,
            "files": [
                {
                    "file_id": 0,
                    "records": [
                        {
                            "record_id": 0,
                            "type": 1,
                            "subtype": 2,
                            "contents": 300,
                            "id": 4,
                            "x": 500,
                            "y": 600,
                            "z": 700,
                        }
                    ],
                },
                {"file_id": 1, "records": []},
            ],
        }
        self.assertEqual(data_mod.build_treasure(document, source), source)


class ShopStockTests(unittest.TestCase):
    def test_patches_only_validated_overlay_item_pools(self) -> None:
        overlay = bytearray(0x14000)
        shops = []
        for shop_id, descriptor_address, pool_address, pool_count in data_mod.SHOP_LAYOUT:
            counts = [pool_count // 24 + (index < pool_count % 24) for index in range(24)]
            descriptor_offset = descriptor_address - data_mod.OV009_LOAD_ADDRESS
            pool_offset = pool_address - data_mod.OV009_LOAD_ADDRESS
            classes = []
            cursor = 0
            for class_id, expected_tag, class_name, _ in data_mod.SHOP_CLASSES:
                tiers = []
                for tier in range(6):
                    count = counts[class_id * 6 + tier]
                    struct.pack_into(
                        "<I",
                        overlay,
                        descriptor_offset + 4 * (class_id * 6 + tier),
                        cursor | (count << 16),
                    )
                    items = []
                    for index in range(count):
                        item_id = expected_tag | (index & 0x0FFF)
                        struct.pack_into("<H", overlay, pool_offset + 2 * cursor, item_id)
                        items.append({"item_id": f"0x{item_id:04X}", "name_hint": ""})
                        cursor += 1
                    tiers.append({"tier": tier, "pool_start": cursor - count, "items": items})
                classes.append(
                    {
                        "class_id": class_id,
                        "class": class_name,
                        "item_tag": f"0x{expected_tag:04X}",
                        "tiers": tiers,
                    }
                )
            descriptor_data = bytes(overlay[descriptor_offset : descriptor_offset + 96])
            pool_data = bytes(overlay[pool_offset : pool_offset + pool_count * 2])
            shops.append(
                {
                    "shop_id": shop_id,
                    "descriptor_address": f"0x{descriptor_address:08X}",
                    "item_pool_address": f"0x{pool_address:08X}",
                    "descriptor_sha1": data_mod.sha1(descriptor_data),
                    "item_pool_sha1": data_mod.sha1(pool_data),
                    "classes": classes,
                }
            )
        document = {
            "schema": data_mod.SHOP_SCHEMA,
            "binary": "arm9_overlay_9",
            "load_address": f"0x{data_mod.OV009_LOAD_ADDRESS:08X}",
            "source_sha1": data_mod.sha1(bytes(overlay)),
            "shops": shops,
        }
        self.assertEqual(data_mod.build_shop_stock(document, bytes(overlay)), bytes(overlay))

        first_item = document["shops"][0]["classes"][0]["tiers"][0]["items"][0]
        first_item["item_id"] = "0x200A"
        rebuilt = data_mod.build_shop_stock(document, bytes(overlay))
        first_pool = data_mod.SHOP_LAYOUT[0][2] - data_mod.OV009_LOAD_ADDRESS
        self.assertEqual(struct.unpack_from("<H", rebuilt, first_pool)[0], 0x200A)
        differences = [
            index for index, (before, after) in enumerate(zip(overlay, rebuilt)) if before != after
        ]
        self.assertEqual(differences, [first_pool])


class ItemMasterTests(unittest.TestCase):
    def test_round_trips_records_and_patches_price_at_runtime_address(self) -> None:
        arm9 = bytearray(0x4D000)
        categories = []
        for tag, class_name, address, count, stride, _ in data_mod.ITEM_MASTER_LAYOUT:
            offset = address - data_mod.ARM9_LOAD_ADDRESS
            records = []
            for index in range(count):
                record = struct.pack("<6HH", *(range(6)), 10 + index)
                record += bytes([index & 0xFF]) * (stride - 0x0E)
                arm9[offset + index * stride : offset + (index + 1) * stride] = record
                records.append(
                    {
                        "index": index,
                        "item_id": f"0x{tag | index:04X}",
                        "name_hint": "",
                        "unknown_words_00_0A": [
                            f"0x{value:04X}" for value in range(6)
                        ],
                        "price": 10 + index,
                        "unknown_0E_hex": (
                            bytes([index & 0xFF]) * (stride - 0x0E)
                        ).hex(" "),
                    }
                )
            table = bytes(arm9[offset : offset + count * stride])
            categories.append(
                {
                    "class": class_name,
                    "item_tag": f"0x{tag:04X}",
                    "runtime_address": f"0x{address:08X}",
                    "record_count": count,
                    "record_size": stride,
                    "source_region_sha1": data_mod.sha1(table),
                    "records": records,
                }
            )
        document = {
            "schema": data_mod.ITEM_MASTER_SCHEMA,
            "binary": "arm9_main",
            "load_address": f"0x{data_mod.ARM9_LOAD_ADDRESS:08X}",
            "source_sha1": data_mod.sha1(bytes(arm9)),
            "categories": categories,
        }
        self.assertEqual(data_mod.build_item_master(document, bytes(arm9)), bytes(arm9))

        document["categories"][0]["records"][0]["price"] = 1234
        rebuilt = data_mod.build_item_master(document, bytes(arm9))
        first_price = (
            data_mod.ITEM_MASTER_LAYOUT[0][2] - data_mod.ARM9_LOAD_ADDRESS + 0x0C
        )
        self.assertEqual(struct.unpack_from("<H", rebuilt, first_price)[0], 1234)


class BattleScriptTests(unittest.TestCase):
    def setUp(self) -> None:
        descriptors = [0] * data_mod.BATTLE_VM_DESCRIPTOR_COUNT
        descriptors[2] = 0x02
        descriptors[3] = 0x41
        descriptors[4] = 0x45
        descriptors[5] = 0x01
        descriptors[13] = 0x62
        self.descriptors = tuple(descriptors)
        self.names = {0: "end", 1: "return"}

        entry = bytearray(struct.pack("<3H", 6, 6, 0))
        entry.extend(struct.pack("<3H", 3, 1, 0x4001))
        entry.extend(struct.pack("<5H", 13, 0x4010, 0x0011, 0x4002, 0xFFFD))
        entry.extend(struct.pack("<H", 1))
        entry.extend(b"TAIL")
        self.source = data_mod.build_offset_archive([bytes(entry)])

    @staticmethod
    def _commands(entry: dict) -> list[dict]:
        return [
            command
            for segment in entry["segments"]
            if segment["kind"] == "code"
            for command in segment["commands"]
        ]

    def _document(self) -> dict:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test_battle_scripts.dat"
            path.write_bytes(self.source)
            return data_mod.export_battle_scripts(
                path, "BAI/test.dat", self.descriptors, self.names
            )

    def test_round_trips_commands_duplicate_entry_points_and_private_tail(self) -> None:
        document = self._document()
        entry = document["entries"][0]
        self.assertEqual(entry["entry_points"], ["entry_000", "entry_001", None])
        private = [
            segment for segment in entry["segments"]
            if segment["kind"] == "private_data"
        ]
        self.assertEqual(private[0]["size"], 4)
        self.assertNotIn("data", private[0])
        command = self._commands(entry)[1]
        self.assertEqual(command["result"], "battle.shared[0]")
        self.assertEqual(
            command["args"], [{"variable": "battle.constant_zero_2"}, -3]
        )
        self.assertEqual(command["unused_mode_bits"], "0x0010")
        self.assertEqual(
            data_mod.build_battle_scripts(
                document, self.source, self.descriptors, self.names
            ),
            self.source,
        )

    def test_edit_changes_only_the_selected_argument(self) -> None:
        document = self._document()
        code_segment = next(
            segment for segment in document["entries"][0]["segments"]
            if segment["kind"] == "code"
        )
        commands = code_segment["commands"]
        commands[1]["args"][1] = 7
        rebuilt = data_mod.build_battle_scripts(
            document, self.source, self.descriptors, self.names
        )
        differences = [
            index
            for index, (before, after) in enumerate(zip(self.source, rebuilt))
            if before != after
        ]
        self.assertEqual(differences, [28, 29])
        self.assertEqual(rebuilt[-4:], b"TAIL")

    def test_rejects_legacy_opcode_with_malformed_descriptor(self) -> None:
        with self.assertRaisesRegex(
            data_mod.DataModError, "cannot safely emit opcode 0x03F"
        ):
            data_mod._parse_battle_opcode(
                "op_03F", self.descriptors, self.names, "test command"
            )

    def test_relocates_branch_labels_after_command_insertion(self) -> None:
        entry = bytearray(struct.pack("<H", 2))
        entry.extend(struct.pack("<7H", 4, 0, 0, 0, 2, 0, 3))
        entry.extend(struct.pack("<3H", 3, 0, 1))
        entry.extend(struct.pack("<H", 1))
        source = data_mod.build_offset_archive([bytes(entry)])
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "branch.dat"
            path.write_bytes(source)
            document = data_mod.export_battle_scripts(
                path, "BAI/branch.dat", self.descriptors, self.names
            )
        code_segment = next(
            segment for segment in document["entries"][0]["segments"]
            if segment["kind"] == "code"
        )
        commands = code_segment["commands"]
        target = commands[-1]["labels"][0]
        self.assertEqual(commands[0]["args"][4], {"label": target})
        commands.insert(
            -1,
            {"opcode": "op_005", "args": [123]},
        )
        rebuilt = data_mod.build_battle_scripts(
            document, source, self.descriptors, self.names
        )
        rebuilt_entry = data_mod.parse_offset_archive(rebuilt)[0]
        self.assertEqual(struct.unpack_from("<h", rebuilt_entry, 14)[0], 5)
        self.assertEqual(len(rebuilt_entry), len(entry) + 4)


if __name__ == "__main__":
    unittest.main()
