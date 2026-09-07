"""Validate input chords and dump bounds without booting an emulator."""

import argparse
import importlib
import sys

import pytest

pytest.importorskip("desmume")

from tools.runtime_inputs import action_mask, parse_action


def test_chord_preserves_each_button_bit():
    name, frames = parse_action("SELECT+A:30")
    assert frames == 30
    assert action_mask(name) == action_mask("select") | action_mask("a")
    assert action_mask("a+a") == action_mask("a")
    assert action_mask("wait") == 0
    assert parse_action("start") == ("start", 1)


@pytest.mark.parametrize("value", ["wait+a:1", "a+:1", "a+bogus:2", "a:0", "a:-1", "a:no"])
def test_invalid_actions(value):
    with pytest.raises(argparse.ArgumentTypeError):
        parse_action(value)


def test_capture_regions_and_labels(monkeypatch):
    # The executable probe imports its sibling module when launched as a script.
    from tools import runtime_inputs
    monkeypatch.setitem(sys.modules, "runtime_inputs", runtime_inputs)
    probe = importlib.import_module("tools.runtime_probe")
    for value in [
        "ram=0x02000000:0x02400000", "banks=0x04000240:0x0400024a",
        "palette=0x05000000:0x05000800", "tiles=0x06000000:0x06020000",
        "oam=0x07000000:0x07000800",
    ]:
        assert probe.parse_memory_range(value).end > probe.parse_memory_range(value).start
    for value in [
        "ram=0x023ffff0:0x02400001", "gap=0x0607ffff:0x06200001",
        "../escape=0x02000000:0x02000004", "empty=0x02000000:0x02000000",
    ]:
        with pytest.raises(argparse.ArgumentTypeError):
            probe.parse_memory_range(value)
