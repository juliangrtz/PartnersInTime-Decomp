# Editable game data

The European data project under `data/eur/` turns understood NitroFS formats
into reviewable JSON.  It is rebuilt during the normal ROM build, so a data mod
does not need an ad-hoc binary patch or fixed-length replacement text.

Only formats with a validated inverse encoder belong here.  The exporter and
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
- length-changing MFset edits: string pointers, language-entry sizes, and outer
  archive offsets are regenerated instead of patched in place.

Player growth/base stats are not present in this DAT corpus.  They are created
by executable and save-data logic and therefore remain part of the C
reconstruction.

## Building a data mod

Edit JSON below `data/eur/`, then use the existing CLion **Build and Run EUR
NDS** configuration.  `tools/build_nds.ps1` automatically detects
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
python .\tools\data_mod.py export `
  --version eur `
  --files-root .\extract\eur\files `
  --project-root .\data\eur
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

## Generated build artifacts

`build/eur/data_mod_report.json` lists each rebuilt source, old/new size and
SHA-1, and whether it changed.  `build/eur/build/rom_config_data_mod.yaml` is a
derived `dsd` configuration pointing at the staged NitroFS and, when needed,
the copied/patched overlay under `build/eur/data_mod_code/`.  These outputs are
ignored and may be deleted at any time; original extracted or linked files are
never modified.
