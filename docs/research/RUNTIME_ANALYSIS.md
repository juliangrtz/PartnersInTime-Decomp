# Runtime analysis with py-desmume

Static reconstruction remains the source of truth for matching code. Runtime evidence is used to name data, confirm object layouts, identify active overlays, and observe battle state transitions.

## Setup

Install the optional runtime dependencies:

```powershell
python -m pip install -r tools/runtime_requirements.txt
```

`py-desmume` 0.0.9 bundles DeSmuME 0.9.12. Cross-version `.dst`
compatibility is not reliable enough for frame execution: the supplied 0.9.13
Princess Shroob state restores plausible CPU and main-RAM state, but its GPU
state renders incorrectly and the next frame does not complete. It is useful as
a zero-frame memory snapshot, not as a trace starting point. The probe refuses
to advance a loaded state unless `--allow-state-advance` is supplied explicitly.

ROMs, battery saves, savestates, screenshots, and RAM dumps stay outside Git. The existing ignore rules cover `*.nds`, `*.sav`, `PiT_*`, and `build/`.

## Convert the battery save into a traceable state

Use the supplied raw `.sav` with the interactive runner. Load the save in the
game, navigate to the point of interest, and close the SDL window. The runner
then writes a state made by the same embedded DeSmuME version used by the probe:

```powershell
python tools/runtime_session.py `
  --rom PiT_eur.nds `
  --battery-save PiT_SaveStates/younger_princess_shroob.sav `
  --save-state build/runtime/states/shroob_py_desmume.dst `
  --screenshot build/runtime/states/shroob_py_desmume.png
```

The interactive runner deliberately does not write anything under `data/`.
Generated states and screenshots remain under the ignored `build/runtime/`
tree.

## Capture a savestate

This command loads the supplied legacy state without advancing it, captures
CPU registers, compares each runtime overlay slot with the extracted overlay
binaries, and decodes the recovered battle context and actor layouts. The
known-invalid cross-version framebuffer is deliberately omitted:

```powershell
python tools/runtime_probe.py `
  --rom $sourceRom `
  --state PiT_SaveStates/younger_princess_shroob.dst `
  --output build/runtime/younger_princess_shroob `
  --no-screenshot
```

Omit `--no-screenshot` for a compatible state to save both DS screens as
`screens.png`.

Advance frames and record writes to a suspected field:

```powershell
python tools/runtime_probe.py `
  --rom PiT_eur.nds `
  --state build/runtime/states/shroob_py_desmume.dst `
  --output build/runtime/shroob_hp_trace `
  --frames 120 `
  --allow-state-advance `
  --watch-write 0x020D2066:2 `
  --diff-range shroob_actor=0x020D2060:0x020D2300
```

Execution hooks accept either an address or any unique function name from the DSD symbol files:

```powershell
python tools/runtime_probe.py `
  --rom PiT_eur.nds `
  --state build/runtime/states/shroob_py_desmume.dst `
  --output build/runtime/damage_trace `
  --frames 300 `
  --allow-state-advance `
  --exec BattleDamage_ApplyToEnemy
```

Only use `--allow-state-advance` for a state created by
`tools/runtime_session.py` or otherwise verified with py-desmume 0.0.9. Each
callback records the ARM9 registers, making arguments and object pointers
available for later structure analysis. Explicit diff ranges write
`.before.bin` and `.after.bin` files and summarize changed byte runs in
`evidence.json`.

## Supplied Princess Shroob snapshot

The zero-frame capture already confirms several static reconstruction results:

- the active code images are overlay 2 (battle core; 370,953 of 370,976 bytes
  agree), overlay 10 (6,944/6,944 bytes), and overlay 26 (15,008/15,008
  bytes); the other shared slot has no credible overlay match;
- `gBattleContext` points to `0x020CD058` and its recovered actor tables contain
  all four party actors plus one active enemy in actor slot 60;
- the active enemy resolves through its live `BattleEnemyDataRequest` to enemy
  record 84, name ID 42, `Princess Shroob`, level 29, with configured stats
  HP 1,700 / POW 180 / DEF 130 / SPEED 120;
- the live actor has HP 11,700/11,700, POW 180, DEF 130, SPEED 120, pending
  damage 30, target actor 56 (Mario), and Q8 damage scale 332;
- the exact 10,000 difference between configured and live maximum HP is a
  runtime observation, not yet a named mechanic. It must be observed before
  and after the relevant script transition in a compatible state before the C
  code treats it as a proven phase or protection rule.

The JSON also records each party member's HP/stats, actor flags, formation,
scene-object position, animation, target, and resource pointers. Empty enemy
slots remain present as allocated actor/scene-object storage but are marked
inactive when their resource slot is null.

The supplied `.dst` was created from a ROM reported by DeSmuME as CRC
`CC780583`; the repository's canonical European image reports a different ROM
CRC. Use the matching local source ROM for this legacy state. Every evidence
file records both ROM and state SHA-1 values so captures from different dumps
cannot be mixed silently.

## Evidence discipline

- Record the ROM and savestate SHA-1 values with every capture.
- Repeat observations from a fresh state before turning them into field names or C types.
- Prefer execution hooks on known functions over broad write watches once the responsible code is narrowed down.
- Keep raw captures under `build/runtime/`; commit only generalized findings, scripts, and documentation.
- A runtime value proves what happened in that state, not that all call paths share the same meaning.

References: [py-desmume repository](https://github.com/SkyTemple/py-desmume),
[quick start](https://py-desmume.readthedocs.io/en/latest/quick_start.html),
[savestate API](https://py-desmume.readthedocs.io/en/latest/api_docs/desmume.emulator/desmume_savestate.html),
and [memory/hook API](https://py-desmume.readthedocs.io/en/latest/api_docs/desmume.emulator/desmume_memory.html).
