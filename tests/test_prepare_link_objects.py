import unittest
from pathlib import Path

from tools.prepare_link_objects import validate_unique_object_basenames


class PrepareLinkObjectsTests(unittest.TestCase):
    def test_accepts_unique_object_basenames(self):
        validate_unique_object_basenames(
            [Path("src/overlay012/attack_state.c"), Path("src/overlay020/arc.c")]
        )

    def test_rejects_equal_object_basenames_in_different_directories(self):
        with self.assertRaisesRegex(ValueError, "both produce attack_state.o"):
            validate_unique_object_basenames(
                [
                    Path("src/overlay012/attack_state.c"),
                    Path("src/overlay020/attack_state.c"),
                ]
            )


if __name__ == "__main__":
    unittest.main()
