# Remaining decompilation

The September 19, 2026 audit starts from `2947768`, after the source-quality
refactoring of the `65ef0e6` milestone. Matching C/C++ remains **781,856 of
1,563,700 ARM9 code bytes (50.0004%)**. The refactor changed organization and
source expression, not the linked address coverage.

Generate the current function inventory before choosing work:

```powershell
python tools/decompilation_backlog.py
```

It writes `build/analysis/decompilation_backlog.json`, using public metadata only.
It includes addresses, native sizes, missing ranges, adjacent linked sources,
unlinked public drafts and code without function boundaries. It reconciles with
[the progress metric](PROGRESS.md); it does not certify matching, difficulty or
runtime coverage. Private drafts still need a separate provenance check.

## Audit findings

- A rebuild after removing compiled source objects produced the original EUR
  ROM SHA-1, `ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Native relinking reported
  zero differing bytes. Linked section ownership is unchanged from `65ef0e6`,
  with no missing source or duplicate owner in the current manifest.
- A textual comparison found eleven changed bodies among 3,662 uniquely named
  function definitions. Review covered the virtual call, vtable writes, archive
  parameter conversions, scene-manager byte offsets, save-menu workspace clear
  size and Bro Flower party pointer. No behavioral regression was found in these
  changes. This text comparison is a review aid, not a C/C++ semantic proof.
- The merge tool had real preflight gaps: it confused source filenames with
  object basenames (`name.c` and `name.cpp` both produce `name.o`), accepted plans
  whose operations consumed each other's inputs, and allowed linked/unlinked
  merges. Its forced removals could discard staged work. Whole-plan checks now
  reject these cases, dirty inputs, missing files and out-of-tree targets before
  writing; regression tests use disposable Git repositories.
- An older unlinked draft, `src/battle/battle_object_texture_decode.c`, failed to
  compile because two graphics-resource arguments lacked their typed pointer
  view. Those calls are corrected. `ninja objects` and the refactor gate now
  compile every source, including drafts that `ninja check` does not build.
- The enemy-selection comment incorrectly described its local argument as
  `int`; it is `u16`. The comment is corrected. The underlying disagreement with
  some attack callers' `int` declarations predates this refactor and remains an
  explicit ABI task; changing either side blindly breaks matching.
- Claims that a successful C++ build proves the original language, or a failed
  one proves original C, were removed from the style guide. Module comments
  improve navigation; a presence/range test does not validate their meaning.
- The progress parser previously recognized linked C/C++ only in `.text`.
  It now uses the component's `kind:code` declarations, so future `.init`
  reconstruction can count without moving code or counting constructor data.
  Tests cover this distinction; the current percentage and denominator are unchanged.

The handoff's 815 linked units counted manifest lines. There are **784 enabled
units and 20 unlinked source drafts**, out of 804 source files. Nineteen drafts
have delink entries; `src/battle/battle_capture_surface.c` has none. Fresh object
screening of their 30 functions found 29 differing results and one unresolved
relocation, so none can simply be enabled. These drafts do not inflate progress.

A fresh automated shop replay passed 1,323 frames, with 32 independently checked
marker calls out of 3,944 observed calls. All checked calls took the hidden-marker
branch; this adds no drawing-branch coverage. Shop entry used the documented
temporary command fixture, then ordinary inputs returned to the field. Captures
were inspected and all 104 original battery saves remained unchanged. Historical
isolated ARM checks remain separate evidence, not a full-game playthrough or a
proof of portable C++ object lifetimes.

## Where the work remains

Snapshot at the audited revision; regenerate rather than maintaining these counts
by hand. Missing C/C++ includes the 4,752 bytes currently maintained as symbolic
ARM9 assembly.

| Area | Missing C/C++ bytes | Function candidates |
|---|---:|---:|
| Field, overlay 0 | 204,572 | 218 |
| Battle, overlay 2 | 184,620 | 214 |
| Resident ARM9 | 111,016 | 350 |
| Scene/pause, save menu, shop | 133,696 | 230 |
| Attacks and battle items | 103,868 | 139 |
| Elder Princess Shroob, overlay 25 | 22,180 | 28 |
| Title and scene support | 19,952 | 50 |
| ITCM | 1,940 | 13 |
| **Total** | **781,844** | **1,242** |

The byte total also includes **2,604 bytes without function boundaries**. Keep
startup, constructor registration, alignment and code/data ownership explicit;
do not turn an unmapped interval into a guessed function.

Small functions alone cannot finish this efficiently. The 340 candidates of at
most 128 bytes account for only 17,432 missing bytes. The 393 candidates between
513 and 2,048 bytes account for 384,488 bytes; another 54 larger functions account
for 242,020. Size is a triage dimension, not an estimate of difficulty.

## Work order

1. **Recover existing evidence before starting another implementation.** Match
   private candidates against current component/address ownership and freshly
   compiled objects. The field popup clear helper at overlay 0 `0x0206F8AC`
   still matches all 76 bytes, but remains unlinked and lacks a completed runtime
   route. Its neighboring helpers do not match. Preserve the exact function
   separately before extending that group. None of the 20 public drafts is an
   already exact unit waiting only for a manifest edit.
2. **Close shared interfaces while reconstructing their users.** Overlay 10 has
   three remaining functions: `0x020C1024` (388 bytes), `0x020C1410` (2,164),
   and `0x020C2978` (236). Recover argument widths, object ownership and allocation
   contracts together with the attack callers. Resolve the selectable-enemy
   declaration discrepancy from caller instructions and callee data flow.
   These dependencies are useful across attacks, but are not all easy leaves.
3. **Work outward through medium-sized field, battle and menu groups.** Choose
   connected constructors, updates and destructors with existing shared types
   and a reachable save. Use the inventory's adjacent sources to find context,
   not to infer original compilation boundaries. Reuse the tested shop, pause,
   field and battle routes from the [reconstruction reference](research/RECONSTRUCTION_NOTES.md).
   Inspect the predicate that creates the target callback before replaying.
4. **Expand attack and boss state machines with the story saves.** Tie each batch
   to an actual encounter/script entry and record the phases exercised. The four
   remaining battle-item functions alone total 10,736 bytes; closing familiar
   shared helpers first is more useful than guessing their state contracts.
5. **Schedule the hard tail deliberately.** Keep compiler/runtime helpers, ITCM,
   startup, matrix/drawing schedules and nonstandard ABI boundaries as explicit
   work items. Existing popup, tile-upload, chain-rig and renderer drafts already
   contain failed allocation/scheduling attempts. Revisit only with a new type,
   caller, instruction or runtime observation that explains a mismatch. Small
   explained inline assembly remains allowed, with whole-function comparison.

For each batch, capture the native range and expected behavior, establish the
source/object match, integrate it, run the full build gate, and execute focused
runtime checks. Keep exactness, integration, runtime coverage and publication as
separate states. Record the next missing evidence for deferred work so that a new
session does not repeat unsuccessful source permutations. Commit and push verified
batches; report bytes gained and actual checks rather than estimated percentages.

## What counts as finished

**ARM9 milestone:** every code interval in the current metric is owned by verified
C/C++ (with any necessary inline assembly disclosed), including symbolic-assembly
replacements and startup ranges. No delinked reference code may fill an unnoticed
hole. Exact rebuilding remains required.

**Whole-game reconstruction:** ARM7 needs its own code/data boundary inventory and
C/C++ reconstruction. Its 43,852 bytes of maintained symbolic source are separate
from the ARM9 percentage; the mixed autoload images do not yet have a trustworthy
code-only denominator. Follow the [ARM7 map](research/ARM7_MAP.md), keeping CPU
address spaces and interworking distinct. ARM9 reaching 100% does not finish this.

**Readable, editable code:** recover shared interfaces and structure invariants
alongside each batch. Explain ownership, lifetime, units, phase-dependent fields
and matching constraints. Keep unknowns neutral. Consolidate only contiguous,
related ranges after they match; retain visible WIP status for other drafts.
Then make a cross-module pass over ABI declarations, duplicate views, bounds,
callback types and comments. Explicit vtable manipulation remains tied to the
verified Metrowerks ABI and needs further work for a portable build.

**Behavioral confidence:** keep a route matrix for boot/title, load/save, field,
menus/shop, normal battles, each attack and boss/endgame paths. Pair live execution
with isolated boundary cases where useful; never report a missed callback as
coverage. DMA, interrupt timing and audio need explicit emulator/hardware checks.
Exact output, gameplay coverage, portability and documentation are distinct goals.
