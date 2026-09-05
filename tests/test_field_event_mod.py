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
import field_event_mod


class FieldEventScriptTests(unittest.TestCase):
    def test_checked_in_semantics_match_descriptor_contracts(self) -> None:
        descriptors, names = field_event_mod.load_vm_schema("eur")
        self.assertEqual(len(descriptors), 0x155)
        self.assertEqual(names[0x08D], "start_entity_vertical_motion")

    def setUp(self) -> None:
        descriptors = [0] * 0x10C
        descriptors[0x34] = 0x41
        descriptors[0x35] = 0x42
        descriptors[0x93] = 0x42
        descriptors[0x9A] = 0x42
        self.descriptors = tuple(descriptors)
        self.names = {
            0: "end",
            0x34: "branch_relative",
            0x35: "set_aux_script_enabled",
            0x93: "add_entity_roaming_profile",
            0x9A: "load_entity_waypoint_path",
        }

    @staticmethod
    def _member_with_private_alias() -> bytes:
        # Nine fixed-section pointers and two event slots share one u16 table.
        member = bytearray(struct.pack("<11H", *([22] * 10), 32))
        member.extend(struct.pack("<4H", 0x35, 0, 1, 2))
        member.extend(struct.pack("<H", 0))
        member.extend(struct.pack("<H", 0xFFFF))
        return bytes(member)

    def _document(self) -> tuple[dict, bytes]:
        source = data_mod.build_offset_archive(
            [self._member_with_private_alias(), b"localized", b""]
        )
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "FEvData.dat"
            path.write_bytes(source)
            document = field_event_mod.export_document(
                path, "eur", self.descriptors, self.names
            )
        return document, source

    def test_formats_confirmed_field_variables_symbolically(self) -> None:
        self.assertEqual(field_event_mod.format_variable(0x3004), "field.owner_subtype")
        self.assertEqual(
            field_event_mod.parse_variable("field.paired_room_id", "test"), 0x300A
        )
        self.assertEqual(
            field_event_mod.parse_variable("field.current_room_companion_id", "test"),
            0x300B,
        )
        self.assertEqual(
            field_event_mod.format_variable(0x300B),
            "field.current_room_companion_id",
        )
        self.assertEqual(
            field_event_mod.parse_variable("field.save_location_id", "test"),
            0x3023,
        )
        self.assertEqual(
            field_event_mod.format_variable(0x3023),
            "field.save_location_id",
        )

    def test_classifies_code_and_private_slot_targets(self) -> None:
        document, source = self._document()
        member = document["members"][0]
        self.assertEqual(member["pointer_table_size"], 22)
        self.assertEqual(member["script_slots"][0], "script_000")
        self.assertEqual(member["script_slots"][1], {"private_target": "0x0020"})
        self.assertEqual(member["valid_script_count"], 1)
        self.assertEqual(member["private_target_count"], 1)
        self.assertEqual(member["reachable_command_count"], 2)
        self.assertEqual(
            field_event_mod.build_document(
                document, source, self.descriptors, self.names
            ),
            source,
        )

    def test_summarizes_opcode_usage_without_script_bytes(self) -> None:
        document, _ = self._document()
        summary = field_event_mod.summarize_document(
            document, self.descriptors, self.names
        )
        self.assertEqual(summary["schema"], field_event_mod.USAGE_SCHEMA)
        self.assertEqual(summary["member_count"], 1)
        self.assertEqual(summary["valid_script_count"], 1)
        self.assertEqual(summary["private_target_count"], 1)
        self.assertEqual(summary["reachable_command_count"], 2)
        self.assertEqual(summary["opcode_counts"], {"0x000": 1, "0x035": 1})

    def test_rebuilds_a_fixed_size_argument_edit(self) -> None:
        document, source = self._document()
        document["members"][0]["commands"][0]["args"][1] = 7
        rebuilt = field_event_mod.build_document(
            document, source, self.descriptors, self.names
        )
        member = data_mod.parse_offset_archive(rebuilt)[0]
        self.assertEqual(struct.unpack_from("<h", member, 28)[0], 7)
        self.assertEqual(member[32:], b"\xFF\xFF")

    def test_rejects_a_branch_to_outside_the_member(self) -> None:
        header = struct.pack("<10H", *([20] * 10))
        member = header + struct.pack("<3H", 0x34, 0, 1) + b"XX" + struct.pack("<H", 0)
        source = data_mod.build_offset_archive([member, b"localized", b""])
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "FEvData.dat"
            path.write_bytes(source)
            document = field_event_mod.export_document(
                path, "eur", self.descriptors, self.names
            )
        document["members"][0]["commands"][0]["args"][0] = 100
        with self.assertRaisesRegex(data_mod.DataModError, "invalid offset"):
            field_event_mod.build_document(
                document, source, self.descriptors, self.names
            )

    def test_exports_and_rebuilds_embedded_entity_motion_data(self) -> None:
        header = struct.pack("<10H", *([20] * 10))
        commands = b"".join(
            (
                struct.pack("<4H", 0x93, 0, 0xFFFF, 6),
                struct.pack("<4H", 0x9A, 0, 0xFFFF, 14),
                struct.pack("<H", 0),
                b"\0\0",
            )
        )
        roaming_profile = struct.pack("<6I", 5, 0x100, 4096, 30, 20, 4)
        waypoint_path = struct.pack(
            "<5I4i", 8, 1, 0, 1, 20, 10, 20, -5, 30
        )
        member = header + commands + roaming_profile + waypoint_path
        source = data_mod.build_offset_archive([member, b"localized", b""])
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "FEvData.dat"
            path.write_bytes(source)
            document = field_event_mod.export_document(
                path, "eur", self.descriptors, self.names
            )

        exported_member = document["members"][0]
        self.assertEqual(exported_member["embedded_data_record_count"], 2)
        self.assertEqual(
            exported_member["commands"][0]["data_record"],
            "roaming_profile_0028",
        )
        self.assertEqual(
            exported_member["commands"][1]["data_record"],
            "waypoint_path_0040",
        )
        records = exported_member["embedded_data_records"]
        self.assertEqual(records[0]["step_distance"], 30)
        self.assertEqual(records[1]["waypoints"][1], {"x": -5, "y": 30})

        records[0]["speed_q12"] = 6144
        records[1]["waypoints"][1]["x"] = -12
        rebuilt = field_event_mod.build_document(
            document, source, self.descriptors, self.names
        )
        rebuilt_member = data_mod.parse_offset_archive(rebuilt)[0]
        self.assertEqual(struct.unpack_from("<I", rebuilt_member, 48)[0], 6144)
        self.assertEqual(struct.unpack_from("<i", rebuilt_member, 92)[0], -12)


if __name__ == "__main__":
    unittest.main()
