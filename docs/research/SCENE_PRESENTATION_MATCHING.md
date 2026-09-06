# Scene presentation and transition matching

These four overlay-7 units are linked into the European build and reproduce
all 2,004 original bytes, including their relocations:

| Unit | Function | Bytes | Source |
|---|---|---:|---|
| Secondary archive | `SceneScript_LoadSecondaryArchive` | 372 | C |
| Transition | `SceneTransitionController_Update` | 964 | C |
| Sprite factory | `SceneSpriteTasks_Create` | 280 | C |
| Object renderer | `SceneObjects_UpdateAndDraw` | 388 | C with 68 bytes of inline assembly |

## Recovered behavior

The secondary archive selector reads input flags at byte `0x10` of each
28-byte action-item record. The typed record preserves that field access.
The original alternate-input branch handles zero first, followed by the
nonzero case. A switch on the selected mode preserves the three asset pairs
`0/1`, `2/3`, and `4/5`, including the no-write default case.

The transition creates and positions a secondary renderable only when its
slot is empty. The previous C candidate performed the position adjustment
again for an existing renderable. Keeping initialization and the target-relative
offset calculation in one inline helper restores the original guarded block.
Object 16 uses target `(0, -24, 0)`; object 17 uses `(128, -16, -136)`.
The destruction paths and asynchronous archive readiness checks also match.

The sprite factory creates exactly eight tasks. Its loop retains separate
X and Y induction variables and a fixed layout offset. Inspection of the
compiler's virtual-register graph identifies the allocation of those values
and the single spilled target-height value. The counter advances after the
coordinate increments, preserving the original comparison schedule.

The renderer walks 56 objects, builds 192 depth lists, and draws each list
in order. Both signed render offsets are read before either position store,
preserving the original memory-access dependencies. Empty rows are guarded
before the inner list loop; the fixed-size outer loops use bottom tests.

The row calculation first narrows `height + 16 * (192 - y)` to signed 16 bits,
then divides by 16 with truncation toward zero. Sprite Y independently narrows
`y - base_y`. Rows are clamped to `0..191`. A 16-instruction ARM block at
function offsets `0x58..0x94` preserves this calculation and its lower clamp;
the upper clamp remains C. One additional `mov` at `+0x34` keeps counter
initialization after the initial object-pointer load. These are the only
17 inline-assembly instructions (68 bytes) in the new units.

## Verification

All four objdiff function comparisons report 100% at their original sizes.
`ninja PiT_eur.nds check` validates the complete linked modules and symbols.
The normal `tools/build_nds.ps1 -DisableDataMods` build reproduces reference
SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
Progress artifacts are regenerated and checked with `tools/generate_progress.py`.
All 66 tests pass with `python -m pytest tests -q --import-mode=importlib`.
