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
tree. Its controls are the arrow keys for the D-pad, `X` for DS A, `Z` for DS
B, `Enter` for Start, right Shift for Select, `Q`/`W` for L/R, and `S`/`A` for
DS X/Y. On Windows the runner polls those keys itself and writes the DS keypad
mask after SDL event handling. This works around py-desmume 0.0.9's SDL input
bug: its `u32` key table is copied into a `u16` buffer, corrupting the mapping
and making D-pad Down unreachable through the bundled SDL window.

For deterministic input without the SDL window, use `runtime_drive.py`. Actions
are applied in their command-line order and use actual DS keypad masks:

```powershell
python tools/runtime_drive.py `
  --rom path/to/pit.nds `
  --state build/runtime/before.dst `
  --action down:60 --action wait:30 --action a:1 `
  --save-state build/runtime/after.dst `
  --screenshot build/runtime/after.png
```

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

The probe also accepts repeatable deterministic `--action KEY[:FRAMES]`
arguments. Use `wait` as the key for unpressed frames. `--action` can be
combined with execution hooks and memory watches, which makes an input path
replayable without relying on host keyboard timing.

Party table slots are not permanent character identities. KO/reload logic can
swap actor pointers between slots while `formation_index` continues to identify
the current occupant. The probe therefore reports both `actor_id`/`slot_label`
and the member name decoded from `formation_index`. In the captured fight,
slot 56 contains Baby Mario at 32/32 HP and slot 58 contains the unconscious
Mario at 0/67 HP.

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
  damage 30, target actor slot 56 (occupied by Baby Mario in this formation),
  and Q8 damage scale 332;
- the exact 10,000 difference is explained by the decoded battle scripts:
  `BAI__BAI_mon_4_hn.dat`, entry 4, reads current/max HP through properties
  16/17, adds 10,000, and writes both values back at command offsets
  `0x0094-0x00C6`; later it subtracts 10,000 from max HP and restores the saved
  current HP at `0x0B1A-0x0B38`. Scenario archive
  `BAI__BAI_scn_4_hn.dat`, entry 22, contains the corresponding explicit setup
  for actor 60 at `0x0228-0x025A`. The temporary buffer's gameplay purpose is
  shield-phase buffer. Entry 4 sets damage immunity at `0x0076`, then clears it
  at `0x0AFC`/`0x0B48` around the HP restoration when the shield breaks.

The JSON also records each party member's HP/stats, actor flags, formation,
scene-object position, animation, target, and resource pointers. Empty enemy
slots remain present as allocated actor/scene-object storage but are marked
inactive when their resource slot is null.

### Deterministic live battle replay

A py-desmume-compatible capture made after entering the Princess Shroob fight
confirmed overlay 2 as the battle core. Selecting Baby Mario's Jump command
with `x:3, wait:180, x:3, wait:360` loaded overlays 10 and 20 and produced two
stable hook events at frame 246:

- overlay 20 called `BattleDamage_CalculateAttack` from `0x020C2E04`;
- it then called `BattleDamage_ApplyToEnemy` from `0x020C2E68` with Princess
  Shroob's scene object, popup offsets `(2, -54)`, and calculated damage `4`;
- `BattleActor_ApplyDamage` was not entered and her HP remained 11,700;
- her enemy state was `0x18`, whose bit 4 is checked by
  `BattleDamage_ApplyToEnemy` around the HP update, animation, and popup paths.

This behavior identifies enemy state bit 4 and script property 76 as
`damage_immune`, replacing the previous placeholder name `flag_04`. In this
fight it is the protection supplied by Princess Shroob's color-changing shield;
the shield absorbs the hit and her HP stays unchanged until the shield-break
sequence clears the flag. The capture is also direct runtime evidence that
overlay 20 implements this Jump attack path while overlays 2 and 10 remain
resident.

The shield progression itself is script-driven. Entry 4 initializes context VM
variable `0x8001` to 3 and `0x8004` to 5. The live replay observed `0x8001`
decrease from 3 to 2 on Baby Mario's hit and from 2 to 1 on Luigi's hit while
the calculated damage values differed (4 and 6). Commands at `0x07B8`-`0x07E6`
decrement `0x8001` for each reaction, reset it to 3 at zero, and then decrement
`0x8004`. The `0x08B2` condition enters the shield-break path when `0x8004` is
at most zero. This establishes five color stages of three hits each, or 15
accepted shield hits, rather than a damage-total threshold. The later script
path clears `damage_immune`, restores the saved 1,700-HP phase, and plays the
shield break/Princess Shroob crash sequence.

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
