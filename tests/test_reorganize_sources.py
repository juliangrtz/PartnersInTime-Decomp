import unittest

from tools.reorganize_sources import PlanError, parse_delinks, rewrite_delinks, validate

DELINKS = """\
    .text start:0x02004000 end:0x02048ef8 kind:code align:32

src/game/window_alignment.c:
    .text start:0x0201da48 end:0x0201dc3c

src/game/window_voice.c:
    .text start:0x0201dc3c end:0x0201dc98

src/game/window_skin.c:
    .text start:0x0201e000 end:0x0201e100
    .rodata start:0x0204a000 end:0x0204a010

src/game/window_pool.cpp:
    .text start:0x0201e100 end:0x0201e200
"""


def table():
    return {"arm9": parse_delinks(DELINKS.splitlines())}


class ValidateTests(unittest.TestCase):
    def test_merges_contiguous_members(self):
        component, members, merged = validate(
            {
                "target": "src/game/window_text_layout.c",
                "members": ["src/game/window_alignment.c", "src/game/window_voice.c"],
            },
            table(),
        )
        self.assertEqual(component, "arm9")
        self.assertEqual(members[0], "src/game/window_alignment.c")
        self.assertEqual(merged, {".text": (0x0201DA48, 0x0201DC98)})

    def test_rejects_a_gap_between_members(self):
        with self.assertRaisesRegex(PlanError, "not contiguous"):
            validate(
                {
                    "target": "src/game/windows.c",
                    "members": ["src/game/window_voice.c", "src/game/window_skin.c"],
                },
                table(),
            )

    def test_rejects_descending_member_order(self):
        with self.assertRaisesRegex(PlanError, "ascending address order"):
            validate(
                {
                    "target": "src/game/window_text_layout.c",
                    "members": ["src/game/window_voice.c", "src/game/window_alignment.c"],
                },
                table(),
            )

    def test_rejects_mixed_languages(self):
        with self.assertRaisesRegex(PlanError, "mix languages"):
            validate(
                {
                    "target": "src/game/window_pool.cpp",
                    "members": ["src/game/window_skin.c", "src/game/window_pool.cpp"],
                },
                table(),
            )

    def test_rejects_an_unknown_member(self):
        with self.assertRaisesRegex(PlanError, "no delinks entry"):
            validate({"target": "src/game/x.c", "members": ["src/game/nope.c"]}, table())

    def test_renaming_a_single_unit_is_allowed(self):
        component, members, merged = validate(
            {"target": "src/game/window_sound.c", "members": ["src/game/window_voice.c"]},
            table(),
        )
        self.assertEqual(members, ["src/game/window_voice.c"])
        self.assertEqual(merged, {".text": (0x0201DC3C, 0x0201DC98)})


class RewriteTests(unittest.TestCase):
    def test_merged_entry_replaces_the_members(self):
        delinks = table()
        operation = {
            "target": "src/game/window_text_layout.c",
            "members": ["src/game/window_alignment.c", "src/game/window_voice.c"],
        }
        _, members, merged = validate(operation, delinks)
        rewrite_delinks(delinks["arm9"], operation["target"], members, merged)
        rendered = "\n".join(delinks["arm9"].render())
        self.assertIn(
            "src/game/window_text_layout.c:\n    .text start:0x0201da48 end:0x0201dc98",
            rendered,
        )
        self.assertNotIn("window_alignment", rendered)
        self.assertNotIn("window_voice", rendered)
        # Unrelated entries keep their own sections, including .rodata.
        self.assertIn(".rodata start:0x0204a000 end:0x0204a010", rendered)

    def test_the_target_may_reuse_a_later_member_s_name(self):
        delinks = table()
        operation = {
            "target": "src/game/window_voice.c",
            "members": ["src/game/window_alignment.c", "src/game/window_voice.c"],
        }
        _, members, merged = validate(operation, delinks)
        rewrite_delinks(delinks["arm9"], operation["target"], members, merged)
        rendered = "\n".join(delinks["arm9"].render())
        self.assertIn(
            "src/game/window_voice.c:\n    .text start:0x0201da48 end:0x0201dc98",
            rendered,
        )
        self.assertNotIn("0x0201dc3c", rendered)
        self.assertEqual(rendered.count("src/game/window_voice.c:"), 1)

    def test_the_component_wide_heading_survives(self):
        delinks = table()
        operation = {
            "target": "src/game/window_text_layout.c",
            "members": ["src/game/window_alignment.c", "src/game/window_voice.c"],
        }
        _, members, merged = validate(operation, delinks)
        rewrite_delinks(delinks["arm9"], operation["target"], members, merged)
        self.assertIn(
            "    .text start:0x02004000 end:0x02048ef8 kind:code align:32",
            delinks["arm9"].render(),
        )


if __name__ == "__main__":
    unittest.main()
