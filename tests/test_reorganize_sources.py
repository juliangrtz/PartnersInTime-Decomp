import unittest
import json
import subprocess

import pytest

from tools import reorganize_sources as reorganize

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

    def test_rejects_duplicate_ownership_within_one_component(self):
        duplicate = DELINKS + "\nsrc/game/window_voice.c:\n    .text start:0x0201f000 end:0x0201f004\n"
        with self.assertRaisesRegex(PlanError, "appears in"):
            validate({"target": "src/game/voice.c", "members": ["src/game/window_voice.c"]},
                     {"arm9": parse_delinks(duplicate.splitlines())})


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


@pytest.fixture
def plan_repo(tmp_path, monkeypatch):
    """A real disposable Git index: rejection must preserve work and staging."""
    config = tmp_path / "config/eur/arm9"
    config.mkdir(parents=True)
    (config / "delinks.txt").write_text(DELINKS)
    members = [block.name for block in table()["arm9"].blocks if block.name]
    for member in members:
        path = tmp_path / member
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(f"/* {path.stem} */\n")
    (config / "linked_sources.txt").write_text("\n".join(members) + "\n")
    (config / "linker_aliases.json").write_text("[]\n")
    def git(*args):
        return subprocess.check_output(["git", *args], cwd=tmp_path)
    git("init", "-q")
    git("add", ".")
    git("-c", "user.name=Test", "-c", "user.email=test@example.invalid",
        "commit", "-qm", "fixture")
    monkeypatch.setattr(reorganize, "ROOT", tmp_path)
    return tmp_path, config, git


def run_plan(monkeypatch, root, operations, apply=False):
    plan = root / "plan.json"
    plan.write_text(json.dumps(operations))
    monkeypatch.setattr("sys.argv", ["reorganize_sources.py", "--plan", str(plan)]
                        + (["--apply"] if apply else []))
    return reorganize.main()


def test_rejects_same_object_basename_across_languages(plan_repo, monkeypatch, capsys):
    root, _, _ = plan_repo
    assert run_plan(monkeypatch, root, [{"target": "src/other/window_pool.c",
        "members": ["src/game/window_voice.c"]}]) == 1
    assert "object basename collides" in capsys.readouterr().err


def test_rejects_operations_that_consume_each_others_inputs(plan_repo, monkeypatch, capsys):
    root, _, git = plan_repo
    before = (root / "src/game/window_voice.c").read_bytes()
    operations = [
        {"target": "src/game/window_voice.c", "members": ["src/game/window_alignment.c"]},
        {"target": "src/game/voice.c", "members": ["src/game/window_voice.c"]},
    ]
    assert run_plan(monkeypatch, root, operations, apply=True) == 1
    assert "another operation's member" in capsys.readouterr().err
    assert (root / "src/game/window_voice.c").read_bytes() == before
    assert git("diff", "--cached", "--name-only") == b""


def test_rejects_merging_linked_and_unlinked_ranges(plan_repo, monkeypatch, capsys):
    root, config, _ = plan_repo
    (config / "linked_sources.txt").write_text("src/game/window_alignment.c\n")
    assert run_plan(monkeypatch, root, [{"target": "src/game/window_text.c", "members":
        ["src/game/window_alignment.c", "src/game/window_voice.c"]}]) == 1
    assert "linked and unlinked" in capsys.readouterr().err


@pytest.mark.parametrize("staged", [False, True])
def test_apply_preserves_dirty_source_and_index(plan_repo, monkeypatch, capsys, staged):
    root, _, git = plan_repo
    source = root / "src/game/window_voice.c"
    source.write_bytes(b"/* work in progress */\r\n")
    if staged:
        git("add", "src/game/window_voice.c")
        source.write_bytes(b"/* further unstaged work */\r\n")
    before, index = source.read_bytes(), git("diff", "--cached", "--binary")
    assert run_plan(monkeypatch, root, [{"target": "src/game/window_text.c", "members":
        ["src/game/window_alignment.c", "src/game/window_voice.c"]}], apply=True) == 1
    assert "dirty inputs" in capsys.readouterr().err
    assert source.read_bytes() == before
    assert git("diff", "--cached", "--binary") == index


def test_missing_later_input_does_not_partially_apply_plan(plan_repo, monkeypatch, capsys):
    root, _, git = plan_repo
    (root / "src/game/window_skin.c").unlink()
    assert run_plan(monkeypatch, root, [
        {"target": "src/game/voice.c", "members": ["src/game/window_voice.c"]},
        {"target": "src/game/skin.c", "members": ["src/game/window_skin.c"]},
    ], apply=True) == 1
    assert "missing source" in capsys.readouterr().err
    assert (root / "src/game/window_voice.c").is_file()
    assert not (root / "src/game/voice.c").exists()
    assert git("diff", "--cached", "--name-only") == b""


def test_merge_retains_later_member_name_and_alias(plan_repo, monkeypatch):
    root, config, git = plan_repo
    target = "src/game/window_voice.c"
    (config / "linker_aliases.json").write_text(json.dumps([
        {"source": "src/game/window_alignment.c", "owner": "Owner", "labels": ["Interior"]}]))
    git("add", ".")
    git("-c", "user.name=Test", "-c", "user.email=test@example.invalid",
        "commit", "-qm", "alias")
    assert run_plan(monkeypatch, root, [{"target": target, "members":
        ["src/game/window_alignment.c", target]}], apply=True) == 0
    assert (root / target).read_text() == "/* window_voice */\n\n/* window_alignment */\n"
    metadata = reorganize.load_delinks(config / "delinks.txt")
    assert metadata.get(target).sections == {".text": (0x0201DA48, 0x0201DC98)}
    assert json.loads((config / "linker_aliases.json").read_text())[0]["source"] == target
    assert "src/game/window_alignment.c" not in (config / "linked_sources.txt").read_text()


@pytest.mark.parametrize("target", ["../outside.c", "C:/outside.c", "src/../outside.c"])
def test_rejects_non_repository_target(plan_repo, monkeypatch, capsys, target):
    root, _, _ = plan_repo
    assert run_plan(monkeypatch, root, [{"target": target,
        "members": ["src/game/window_voice.c"]}]) == 1
    assert "repository-relative" in capsys.readouterr().err


if __name__ == "__main__":
    unittest.main()
