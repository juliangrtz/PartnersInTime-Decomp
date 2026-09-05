# Editable game data

The European data project under `data/eur/` turns understood NitroFS formats
into reviewable JSON. This directory is generated from the user's own ROM and
is deliberately ignored by Git; do not commit its contents. It is rebuilt
during the normal ROM build, so a data mod does not need an ad-hoc binary patch
or fixed-length replacement text.

Only formats with a validated inverse encoder belong here. The exporter and
builder currently round-trip all covered files byte for byte before edits.
Unknown files remain in the user's private `extract/eur/files/` tree and are
copied unchanged when the modded NitroFS tree is staged.

## Current coverage

- all 21 `mfset*.dat` archives: item, equipment, badge, enemy, help, area,
  save/load, menu, outline, option, and shop text in all six ROM language slots;
- all 834 battle-message strings from `BAI/BMes.dat` and all 9,676 field
  dialogue strings from `FEvent/FEvData.dat`, including their per-message
  headers and localized container structure;
- all 98 fixed-size enemy records from `BData/BDataMon.dat`, including level,
  HP, POW, DEF, speed, experience, coins, drops, traits, and unknown bytes;
- all 765 twelve-byte treasure records from `Treasure/TreasureInfo.dat`, grouped
  by their 283 original file/room entries;
- all four overlay-9 shop-stock datasets, each split into four item classes and
  six progression tiers with resolved English item-name hints;
- all 99 resident ARM9 item-master records, with the confirmed purchase-price
  field named and every not-yet-understood word/byte preserved explicitly;
- all 14 battle-scenario, enemy-AI, and related `BAI_*.dat` VM archives: 230
  archive entries, 243 non-null entry points, and 81,854 control-flow-reachable
  commands using 189 distinct opcodes;
- all three Menu/UI scene-VM archives: 18 entries and 6,585 reachable commands
  using 60 distinct opcodes, with unreachable data retained from the private
  extraction;
- all 778 nonempty field-event members, sharded into one source file for each
  of the 638 rooms: 18,615 valid script roots and 387,272 reachable commands
  using 289 distinct opcodes;
- all 569 statically referenced field-entity movement records: 534 random
  roaming profiles and 35 waypoint paths, exported as typed editable data;
- length-changing MFset edits: string pointers, language-entry sizes, and outer
  archive offsets are regenerated instead of patched in place.

Player growth/base stats are not present in this DAT corpus.  They are created
by executable and save-data logic and therefore remain part of the C
reconstruction.

Field-event bytecode is part of `project.json` and therefore of the normal ROM
build. Its compact manifest is
`scripts/FEvent__FEvData.dat.json`; the actual sources live in
`scripts/FEvent__FEvData.dat/room_NNN.json`, with both script-bearing members of
a room kept together. Field dialogue and field scripts occupy different members
of the same `FEvent/FEvData.dat` outer archive. The builder compiles both views
against the private original, verifies that they do not touch the same member,
and merges their changes while regenerating the outer offsets. A simultaneous
length-changing dialogue edit and fixed-size event-command edit is therefore
safe.

The same exporter and builder remain available as standalone diagnostics:

```powershell
python .\tools\field_event_mod.py export
python .\tools\field_event_mod.py check
python .\tools\field_event_mod.py build --output build\FEvData.modded.dat
```

The exporter separates
valid VM entry points from private/sentinel pointer aliases by decoding the
complete reachable control-flow graph. It follows opcodes `0x093` and `0x09A`
into their embedded data and emits `entity_roaming_profile` and
`entity_waypoint_path` records. Their movement speed, step distance, delays,
direction mode, traversal controls, and signed x/y waypoints are editable; the
builder validates and rewrites them at their original fixed size. Shared data
records are emitted only once and commands name the record they reference.

The current fixed-layout room schema permits opcode/argument edits only when the
encoded command size stays
unchanged; branch and embedded-data targets are revalidated during every build.
The number of profiles or waypoints cannot yet change. General size-changing
field-script edits remain locked until relocation rules for all field
control-flow and embedded-data opcodes are complete.

The Menu/UI scene VM in overlay 7 uses three smaller `MenuAI` archives. Its
editable document is `data/eur/scripts/MenuAI__scene_scripts.json`; the normal
data build compiles all three archives from that one document. It covers all 18
entries, 6,585 reachable commands, and 60 actually used opcodes while retaining
26,076 non-code bytes privately. It round-trips byte-identically and currently
enforces fixed command boundaries. The standalone commands are:

```powershell
python .\tools\scene_script_mod.py export
python .\tools\scene_script_mod.py check
python .\tools\scene_script_mod.py build --output-root build\scene_mod
```

## Building a data mod

Populate the private project from your own matching extraction if it does not
already exist:

```powershell
python .\tools\data_mod.py export `
  --version eur `
  --files-root .\extract\eur\files `
  --project-root .\data\eur
```

Edit JSON below the ignored `data/eur/`, then use the existing CLion **Build
and Run EUR NDS** configuration. `tools/build_nds.ps1` automatically detects
`data/eur/project.json`, validates the files, stages a complete derived NitroFS
tree below ignored `build/eur/data_mod_files/`, packages the ROM, and launches
the configured emulator.

From PowerShell, the equivalent build without launching an emulator is:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\build_nds.ps1
```

Useful standalone commands are:

```powershell
# Validate every editable file and report which binaries differ from the base.
python .\tools\data_mod.py check `
  --files-root .\extract\eur\files `
  --project-root .\data\eur

# Re-export the currently supported formats from your private extraction.
# This overwrites the ignored local data project.
python .\tools\data_mod.py export --version eur `
  --files-root .\extract\eur\files --project-root .\data\eur
```

Re-exporting overwrites the editable documents with data from the private base
files, so do that only when intentionally refreshing the project.  Pass
`-DisableDataMods` to `tools/build_nds.ps1` to package the unmodified private
NitroFS, or `-DataProject path` to test a separate data-project directory.

## Text documents

Each file under `data/eur/text/` rebuilds the NitroFS path in its `source`
field.  Languages contain ordered string records.  The numeric `id` is the
index used by game code and must stay contiguous; only edit `text` unless a
format investigation establishes a reason to change the two-byte
`header_hex` present in menu, battle, and field message records.  Field
containers also identify their `room_id`/`room_part`; battle containers expose
their `battle_group`, making the large archives searchable without relying on
binary offsets.  Every field string also carries its paired `event_label`
(for example `Mario_00` or `Opening_1`).  These labels select event-side
presentation resources/behavior and can be rebuilt at arbitrary lengths, but
should only be changed when the referenced field-event behavior is understood.

Control sequences use an explicit notation:

| Editable form | Encoded bytes | Meaning |
| --- | --- | --- |
| a real JSON newline | `FF 00` | line break |
| `<$END>` | `FF 0A` | end of message |
| `<$TEXTBOX:03>` | `FF 0B 03` | text-box control with one argument |
| `<$PAUSE:xx>` | `FF 0C xx` | timed/controlled pause |
| `<$WAIT:xx>` | `FF 11 xx` | wait control |
| `<$COLOR_RED>` | `FF 2D` | select red text |
| `<$CTRL:xx>` | `FF xx` | not-yet-named control |
| `<$BYTE:xx>` | `xx` | raw byte or game-specific glyph/control |

Literal backslashes are written as `\\`; a literal `<` is written as `\<` so
it cannot be mistaken for a token.  Western text uses Windows-1252, matching
the current European files.  The Japanese slot uses a game-specific one-byte
font map rather than Shift-JIS, so its glyphs deliberately remain lossless
`<$BYTE:xx>` tokens until that map is recovered.  This is less pretty than
plausible-looking mojibake and prevents accidental corruption.

Longer strings are supported by the archive builder, but the original text
renderer still has its original box widths, line limits, timing, and script
assumptions.  Test layout and message flow in-game after substantial edits.

## Enemy records

`data/eur/stats/enemies.json` is ordered by `record_id`; records cannot yet be
added or removed because executable code is known to use the original table
shape.  Decimal gameplay quantities can be edited directly.  Flags, unknowns,
and packed drop values are displayed in hexadecimal so bit changes are visible
in diffs.

The known compiled fields are:

| Offset | Field | Size |
| --- | --- | --- |
| `0x00` | `name_id` | 16 bits |
| `0x02` | `flags_or_ai_id` | 16 bits |
| `0x04` | `unknown_04` | 8 bits |
| `0x05` | `level` | 8 bits |
| `0x06` | `max_hp` | 16 bits |
| `0x08` | `power` | 16 bits |
| `0x0A` | `defense` | 16 bits |
| `0x0C` | `speed` | 16 bits |
| `0x0E` | `traits` | 16 bits |
| `0x10` | `unknown_10` | 16 bits |
| `0x12` | `unknown_12_hex` | 14 bytes |
| `0x20` | `experience` | 16 bits |
| `0x22` | `coins` | 16 bits |
| `0x24` | `item_drop_1` | 32 bits |
| `0x28` | `item_drop_2` | 32 bits |

`name_hint` is a human-readable lookup from the English monster-name MFset and
is not compiled.  Change the corresponding string in
`text/BData__mfset_MonN.dat.json` to rename an enemy.  The builder range-checks
every numeric field and requires all 14 unknown bytes, so malformed edits fail
before ROM packaging.

## Treasure records

`data/eur/stats/treasure.json` exposes every existing treasure object as
`type`, `subtype`, `contents`, unique `id`, and `x`/`y`/`z` coordinates.  The
outer `file_id` corresponds to the original archive entry.  The semantic item
mapping of `contents` is not named yet, so values remain numeric rather than
being assigned speculative labels.  Schema v1 keeps the original file and
record counts fixed while allowing every record field to be changed.

## Shop stock

`data/eur/shops/stock.json` exposes the four shops' 24 stock descriptors as
four item classes by six progression tiers.  Unlike the NitroFS formats, these
tables reside in ARM9 overlay 9.  The build wrapper copies the freshly linked
overlay, validates the original descriptor and pool bytes, patches the copy,
and redirects only overlay 9 in a derived ROM configuration.  C changes to
other parts of the same overlay are therefore preserved.

Each `item_id` has a high-nibble class tag:

| Tag | JSON class | Name source |
| --- | --- | --- |
| `0x1000` | `action_items` | `mfset_AItmN.dat` |
| `0x2000` | `usable_items` | `mfset_UItmN.dat` |
| `0x3000` | `badges` | `mfset_BadgeN.dat` |
| `0x4000` | `wear` | `mfset_WearN.dat` |

`name_hint` is generated from the English singular-name string and is not
compiled.  Schema v1 intentionally keeps descriptor starts/counts fixed:
replace IDs inside a tier, do not add/remove rows, and keep the required class
tag.  These constraints make malformed stock fail validation before packaging.

## Item master records and prices

`data/eur/items/master.json` contains the 14 usable-item, 11 action-item, 41
badge, and 33 wear records resident in the main ARM9 image.  The game resolves
the high-nibble item tag to these four tables.  Record sizes are 20 or 28 bytes
depending on class; the `u16` at `+0x0C` is the confirmed shop price.

All bytes outside the price are currently retained as six
`unknown_words_00_0A` values plus `unknown_0E_hex`.  They remain editable for
controlled experiments, but do not assign gameplay meaning to them until a
code access or runtime probe establishes it.  `name_hint` comes from the
English MFsets and is not compiled.

These tables live at runtime addresses `0x02050044..0x02050960` in ARM9, whose
actual load base is `0x02004000`.  The builder uses runtime-address-minus-load-
base offsets, validates each original table region, patches a copy of the
freshly linked `arm9.bin`, and redirects the derived ROM config.  This preserves
unrelated C changes in resident ARM9.

## Battle scenarios and enemy AI

The 14 files under `data/eur/scripts/` expose every statically reachable command
from the `BAI_scn_*`, `BAI_mon_*`, `BAI_iwasaki`, and `BAI_sugiyama` archives.
The exporter starts at every archive entry point and follows both the resident
VM's branches and overlay 2's script-spawning/branching opcodes. This avoids the
old schema-v1 failure mode where embedded lookup tables happened to decode as
plausible instructions. Each JSON line inside a `commands` array is one
instruction and retains its original `source_offset` for IDA and runtime-trace
correlation. For example:

```json
{"opcode": "subtract", "result": "state[0]", "args": [512, 0], "source_offset": "0x00F4"}
{"opcode": "subtract", "result": "state[1]", "args": [{"variable": "state[0]"}, -128], "source_offset": "0x00FE"}
{"opcode": "jump", "args": [0, {"label": "loc_00D2"}], "source_offset": "0x00E6"}
```

A plain integer is a signed 16-bit literal. `{"variable": "state[0]"}` tells the
VM to resolve that argument through its variable accessor, and `result` is the
optional destination variable used by opcodes that return a value. A small
number of original commands also carry `unused_mode_bits`; keep those preserved
bits unless runtime research proves their meaning.

Schema v2 renders the known variable namespaces symbolically, including
`state[n]`, `context[n]`, `battle.shared[n]`, and named battle values such as
`battle.owner_actor_id`. Hexadecimal IDs from schema v1 remain accepted by the
compiler. Static control-flow and table references use `{"label": "..."}`.
The assembler recalculates their signed halfword displacements as commands are
inserted, removed, or changed in size, and also rebuilds every archive and entry
offset table. `source_offset` and the command counters are provenance fields;
the assembler recalculates the real layout and does not require those counters
to be edited after adding or removing commands.

Opaque `private_data` segments are not committed as original bytes. Their
source range, size, and SHA-1 are recorded, and the builder copies each verified
segment from the user's matching extraction. This retains embedded tables while
preventing them from being mistaken for code. All 14 unchanged schema-v2
documents currently rebuild byte for byte.

The compact command layouts come from the validated 260-entry descriptor table
in `config/eur/battle_ai_vm.json`. All 260 opcodes now have unique names and all
209 battle-specific opcodes have evidence-backed argument, result, control-flow,
yield, and behavior contracts. The checked-in script sources therefore contain
no neutral `op_000`-style battle instructions. One unused legacy command,
`control_script_execution_legacy`, has a malformed original descriptor and is
documented for analysis but must not be added to a mod. The generated
[semantic coverage report](research/SCRIPT_VM_SEMANTICS.md) lists every
descriptor, real usage count, variable namespace, contract, and evidence trail.

## Generated build artifacts

`build/eur/data_mod_report.json` lists each rebuilt source, old/new size and
SHA-1, and whether it changed.  `build/eur/build/rom_config_data_mod.yaml` is a
derived `dsd` configuration pointing at the staged NitroFS and, when needed,
the copied/patched overlay under `build/eur/data_mod_code/`.  These outputs are
ignored and may be deleted at any time; original extracted or linked files are
never modified.
