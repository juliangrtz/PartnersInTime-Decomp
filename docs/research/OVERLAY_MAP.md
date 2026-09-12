# ARM9 overlay map

European overlay roles and the source currently linked into the matching build.
Byte counts below come from [progress.json](../progress.json), checked against
the source and link metadata on 2026-09-12.

Each fraction is **matching C/C++ bytes / mapped code bytes**. The percentage
uses that overlay's code size; separately maintained assembly is excluded.
See [the progress guide](../PROGRESS.md) for the counting rules.
The source links open each overlay's main source directory.

## Field, scenes, menus and shared code

| Overlay | Role | Matching C/C++ / mapped bytes | C/C++ % | Reconstructed code |
|---:|---|---:|---:|---|
| [0](../../src/field/) | Field and world | 146,844 / 366,712 | 40.04% | Field VM; entity movement, collision and rendering; party actions, room transitions, HUD and message windows. |
| [2](../../src/battle/) | Battle engine | 160,216 / 362,436 | 44.21% | Both battle VM dispatchers; damage, status, targeting, rewards, UI, models, effects, resource loading and battle transitions. |
| [5](../../src/overlay005/) | Shared scene, resource and display support | 13,800 / 16,760 | 82.34% | Scene tasks, archive queues, asset tables, sprite pools, draw lists, touch input and 2D display helpers. |
| [6](../../src/overlay006/) | Title screen and credits | 42,592 / 66,492 | 64.06% | Display/VRAM initialization, scene and interrupt setup, title sequence construction and phases, model entry motion, scrolling backdrop setup, fading pixel trails, title prompt and cursor animation, menu selection and rumble controls, skipping and exit controls, sequence cleanup, startup greeting and participant selection, resources, title panels, split-screen transitions, backgrounds, sprite sequences, localized animation, texture/model drawing, brightness fades, credits motion, illustration positioning and fade transitions. |
| [7](../../src/overlay007/) | Scene VM, pause menu and jump-rope minigame | 55,332 / 142,264 | 38.89% | Scene dispatcher and object management; menu text, equipment, cursors, backgrounds, effects and rope physics. |
| [8](../../src/overlay008/) | Save, load, erase and game-over menus | 21,612 / 54,068 | 39.97% | Scene setup and cleanup, models, text, location previews, scrolling, save-state transfer, save writing and erase confirmation. |
| [9](../../src/overlay009/) | Shops | 19,588 / 78,984 | 24.80% | Stock and inventory access, equipment/count queries, item panels, list navigation, resources, graphics transfers and particles. |
| [10](../../src/overlay010/) | Shared battle-attack helpers | 4,032 / 6,820 | 59.12% | Enemy/target selection, party-stat averaging, badge checks, motion, hit reactions, background movement and attack ratings. |

## Attacks and battle items

| Overlay | Role | Matching C/C++ / mapped bytes | C/C++ % | Reconstructed code |
|---:|---|---:|---:|---|
| [11](../../src/overlay011/) | Green and Red Shells | 3,684 / 13,372 | 27.55% | Attack and controller setup, projectile timing/readiness, contact checks, support movement and effect completion. |
| [12](../../src/overlay012/) | Cannonballers | 3,628 / 12,032 | 30.15% | Party order, resource and object setup, launch scheduling, airborne motion, hit effects and cleanup. |
| [13](../../src/overlay013/) | Trampolines | 4,272 / 12,316 | 34.69% | Paired objects, resource setup, jump and bounce motion, retreat, landing effects and battle restoration. |
| [14](../../src/overlay014/) | Bro Flowers and Ice Flowers | 6,536 / 13,860 | 47.16% | Attack sequencing, target selection, participant throws, projectile levels, fades and enemy-effect allocation and cleanup. |
| [15](../../src/overlay015/) | Battle attack; item identity unconfirmed | 668 / 13,164 | 5.07% | Actor selection and preparation, hit setup, model flags and object-pair initialization. |
| [16](../../src/overlay016/) | Mix Flowers | 4,880 / 18,324 | 26.63% | Motion tables, paired effects, participant phases, resource changes, controller geometry, scaling and cleanup. |
| [17](../../src/overlay017/) | Copy Flowers | 4,604 / 16,784 | 27.43% | Tuning tables, participant entry/return, home positions, animation, effect objects, display setup and model cleanup. |
| [18](../../src/overlay018/) | Pocket Chomps | 5,896 / 18,036 | 32.69% | Tuning tables, Chomp entry/exit, animation, support attachment and attacks, anchor offsets and tether setup. |
| [20](../../src/overlay020/) | Jump attack | 2,116 / 9,204 | 22.99% | Attack allocation, resources, actor approach, support states, animation timing, target-relative arcs and phase transitions. |
| [21](../../src/overlay021/) | Hammer attack | 2,228 / 5,280 | 42.20% | Entry, hit setup, primary/secondary transitions, model flags and return cleanup. |
| [25](../../src/overlay025/) | Enemy projectile and effect tasks | 15,844 / 38,024 | 41.67% | Projectile preparation, emission, trails and impacts; linked effects, damage, chain movement, task waits and cleanup. |
| [26](../../src/overlay026/) | Battle item use | 3,716 / 14,452 | 25.71% | Resource selection, item entry and launch, party pairing, effect allocation and cleanup, saved state and position helpers. |

The named attacks above are backed by recorded in-game selections and runtime
checks in the [reconstruction milestones](../BATTLE_MATCHING_MILESTONES.md),
[battle map](BATTLE_MAP.md) and [runtime notes](RUNTIME_ANALYSIS.md).
Overlay 15's item identity remains unconfirmed. Overlay 25's source establishes
projectile and effect behavior, but does not yet identify every enemy using it.
Runtime coverage varies by function; matching bytes do not imply every branch
has been exercised in the emulator.

## VM dispatchers

All four dispatchers are byte-identical and linked, totaling **53,700 bytes**.

| Overlay | Dispatcher | Matching bytes | Source |
|---:|---|---:|---|
| 0 | Field | 23,492 | [field_vm_dispatch.cpp](../../src/field/field_vm_dispatch.cpp) |
| 2 | Battle | 19,168 | [battle_vm_dispatch.cpp](../../src/battle/battle_vm_dispatch.cpp) |
| 2 | Common battle | 1,844 | [battle_vm_common_dispatch.c](../../src/battle/battle_vm_common_dispatch.c) |
| 7 | Scene/object | 9,196 | [scene_vm_dispatch.c](../../src/overlay007/scene_vm_dispatch.c) |

The scene dispatcher includes a 64-byte inline-assembly block for opcode
`0x04E`. Its [matching notes](SCENE_VM_MATCHING.md) explain the height
calculation. The [script VM reference](SCRIPT_VM_SEMANTICS.md) documents the
command tables and behavior. Dispatcher completion does not mean every function
called by a script command has been decompiled.

## Placeholder overlays

IDs `1`, `3`, `4`, `19`, `22`, `23`, `24`, and `27-36` contain the same 32-byte
placeholder image. They have no mapped code and are excluded from the totals.

## Updating this map

1. Check the overlay's `delinks.txt` under
   [the EUR overlay configuration](../../config/eur/arm9/overlays/) and the
   [linked-source manifest](../../config/eur/arm9/linked_sources.txt).
2. Run `python tools/generate_progress.py` from the repository root and copy
   the per-overlay counts from `docs/progress.json`.
3. Update the source summaries and cite runtime evidence when naming a newly
   identified attack or scene.

For disassembly setup, see the [IDA import guide](../../tools/ida/README.md).
For reconstruction and verification rules, see the
[source guidelines](../DECOMPILATION_STYLE.md).
