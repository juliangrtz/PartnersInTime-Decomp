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
import scene_script_mod


class SceneScriptModTests(unittest.TestCase):
    def test_checked_in_semantics_match_descriptor_contracts(self) -> None:
        descriptors, names = scene_script_mod.load_vm_schema("eur")
        self.assertEqual(len(descriptors), 0xD2)
        self.assertEqual(names[0xA8], "start_object_script")

    def setUp(self) -> None:
        descriptors = [0] * 0xD2
        descriptors[0xA8] = 0x43
        self.descriptors = tuple(descriptors)
        self.names = {0: "end", 0xA8: "start_object_script"}

    @staticmethod
    def _entry() -> bytes:
        # The spawned child and the parent fallthrough each terminate cleanly.
        return (
            struct.pack("<2H", 4, 0)
            + struct.pack("<5H", 0xA8, 0, 7, 9, 1)
            + struct.pack("<2H", 0, 0)
        )

    def _project(self, root: Path) -> dict:
        archive = data_mod.build_offset_archive([self._entry()])
        for source in scene_script_mod.SOURCES:
            path = root / source
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(archive)
        return scene_script_mod.export_document(
            root, "eur", self.descriptors, self.names
        )

    def test_round_trips_all_three_archives(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            document = self._project(root)
            rebuilt = scene_script_mod.build_document(
                document, root, self.descriptors, self.names
            )
            self.assertEqual(set(rebuilt), set(scene_script_mod.SOURCES))
            for source, data in rebuilt.items():
                self.assertEqual(data, (root / source).read_bytes())
            entry = document["archives"][0]["entries"][0]
            self.assertEqual(entry["entry_points"], ["entry_000", None])
            self.assertEqual(entry["reachable_command_count"], 3)

    def test_rebuilds_fixed_size_edits_and_scene_variables(self) -> None:
        self.assertEqual(
            scene_script_mod.format_variable(0x7000), "scene.owner_id"
        )
        self.assertEqual(
            scene_script_mod.parse_variable("scene.shared[31]", "test"), 0x702F
        )
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            document = self._project(root)
            command = document["archives"][0]["entries"][0]["commands"][0]
            command["args"][1] = 12
            rebuilt = scene_script_mod.build_document(
                document, root, self.descriptors, self.names
            )
            entry = data_mod.parse_offset_archive(
                rebuilt[scene_script_mod.SOURCES[0]]
            )[0]
            self.assertEqual(struct.unpack_from("<h", entry, 10)[0], 12)

    def test_rejects_new_control_flow_into_unexported_bytes(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            document = self._project(root)
            command = document["archives"][0]["entries"][0]["commands"][0]
            command["args"][2] = 100
            with self.assertRaisesRegex(data_mod.DataModError, "invalid offset"):
                scene_script_mod.build_document(
                    document, root, self.descriptors, self.names
                )


if __name__ == "__main__":
    unittest.main()
