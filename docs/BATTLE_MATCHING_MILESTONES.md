

### Green and Red Shell setup and movement (2026-09-08)

Thirteen Overlay 11 functions add 2,976 matching C++ bytes, reaching
456,616 / 1,563,700 bytes (29.20%). The batch covers the attack and controller
initializers, attached-effect slots, speed lookup and stepped travel time,
and support-character movement. Explicit layouts describe the 612-byte work
area, 44-byte controller, 48-byte projectile view and 28-byte support state.
The controller initializer copies four bytes of initial motion settings;
this length was recovered from the call arguments in the original instructions.

The unmodified rebuilt ROM has SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`; full linking, ROM checks,
74 tests, progress consistency and the public-content audit pass.

Runtime verification used checkpoint 83, `Star Shrine - Third area (Before boss)`,
with save SHA-1 `2cb577d3008975c390a2f00e2b2cd646e4005c1b`. The existing
Petey battle checkpoint was created with the previously documented temporary
Field-VM encounter command; these new attack runs use only ordinary button inputs.
Green Shells (configuration 1, resource `0xc0000047`) ran for 2,110 frames,
and Red Shells (configuration 0, resource `0xc0000048`) for 2,150 frames.
Both attacks returned to Luigi's command wheel. The battery save remains unchanged.

Full-function byte guards and stack-matched return hooks observed nine of the
thirteen new functions. Read-only RAM checks confirmed both controller setups,
configuration pointers, resources, initial positions, projectile readiness,
travel-time and speed results, support reset and completion of effect slots.
The effect-spawn, delayed-intercept, support-launch and speed-update paths did
not run in these unsuccessful kick sequences; their coverage is byte matching.
Private evidence is recorded in `shell_green_full83.json` and
`shell_both_check_red83.json` under `build/runtime/eur_attack_helpers/`.


### Cannonballer resources, hit effects and arcs (2026-09-08)

Twelve Overlay 12 functions add 2,808 matching C++ bytes, reaching
459,424 / 1,563,700 bytes (29.38%). The reconstruction covers the four-participant
resource initializer, object preparation and cleanup, launch scheduling,
three-stage airborne animation, hit feedback and the damped vertical trajectory.
The initializer retains one inline `mov` to keep the original independent
formation index instead of the compiler's equivalent moving stack pointer.

The trajectory remains ordinary double-precision C++ arithmetic. Above height
270, the excess is multiplied by 179.2 and divided by 256. The emitted runtime
calls identify `_dflt`, `_dmul`, `_ddiv`, `_dadd` and `_dfix`; their existing
resident symbols now carry those compiler names. No replacement runtime arithmetic
was introduced. The full ROM remains byte-identical with SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`; 74 tests and all build, progress and
public-content checks pass.

The new checkpoint 83 supplied the same unchanged battery save described above.
A 540-frame setup selected Cannonballers through the normal Bros. menu. The
2,210-frame attack run read each participant's input countdown and supplied the
corresponding ordinary A, B, X and Y presses at frames 417, 447, 477 and 507.
All four hit results were successful (`result == 1`), and the attack returned
to Luigi's command wheel. These runs did not patch game code or battle state.
The source battle checkpoint retains the previously documented encounter setup.

Byte-guarded return hooks observed eleven of twelve new functions. Read-only
oracles checked controller formations, resource setup, start positions, idle and
launch states, cleanup, and all 1,104 computed heights: 548 ordinary samples,
552 damped samples and four final positions. The continuation helper and the
above-threshold final-position branch were not reached. Private evidence is in
`cannon_setup83.json` and `cannon_auto_verified83.json` under
`build/runtime/eur_attack_helpers/`.

### Shared Bros. attack setup and feedback (2026-09-08)

Eleven Overlay 10 functions add 2,260 matching C++ bytes, reaching
461,684 / 1,563,700 bytes (29.53%). They reconstruct party-stat averaging,
badge selection, entry configuration and background movement, shared damage
and rating calls, enemy hit animation and position helpers. Existing attack
callers now use the recovered names. The rebuilt ROM remains byte-identical
with SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`; all build checks,
74 tests, progress checks and the public-content audit pass.

Checkpoint 83 supplied the unchanged battery save described above. Normal
menu input selected Cannonballers; four timed A/B/X/Y inputs completed the
attack in a 2,750-frame run. A separate 2,110-frame Green Shell run returned
to Luigi's command wheel as well. Full-byte-guarded, stack-matched hooks
observed eight of eleven reconstructed functions. Independent read-only RAM
checks covered the averaged actor statistics, badge result, initial transition
configuration, callback, timers and rating flags. The graphic-bound wrapper,
enemy animation restore and enemy center helper were not reached. These
runs did not modify game state or code and used the existing, documented
Petey encounter checkpoint. Private evidence is in `common_cannon_verified83.json`
and `common_shell_verified83.json` under `build/runtime/eur_attack_helpers/`.

### Flower attack sequence, targeting and effect ownership (2026-09-08)

Eleven Overlay 14 functions add 2,604 matching C++ bytes, reaching
464,288 / 1,563,700 bytes (29.69%). They cover the complete entry and main
sequence, timeout extension, random target eligibility, participant scheduling,
projectile availability, fade setup and stop, and allocation-state initialization
and release for eighteen enemy effect objects. The shared 304-byte attack-work
layout is now available separately from the Overlay 10 call declarations.

The rebuilt ROM remains byte-identical with SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`; full linking, 74 tests,
progress consistency and the public-content audit pass. The initial build caught
an argument-width declaration changing an existing Overlay 10 caller's code;
the caller declarations were separated and the full build was rerun successfully.

Checkpoint 83 was used through the normal Bros. menu. A 2,790-frame Bro Flower
run and a 2,830-frame Ice Flower run both returned to Luigi's command wheel.
For Ice Flowers, read-only participant state drove 71 ordinary A/B presses,
producing nine participant handoffs and 73 projectile-slot searches. No code,
RAM state or battery save was modified during these runs. The starting Petey
battle checkpoint uses the previously documented encounter setup.

Guarded hooks observed ten of eleven new functions. RAM oracles verified all
18 effect resources and initial fields, both fades, nine handoffs, free-slot
results, entry timers and phases, 1,483 selected-target eligibility checks,
effect release and final attack-work cleanup. The badge-dependent duration
extension was not entered. Private evidence is in `flower_fire_verified83.json`
and `flower_ice_auto83.json` under `build/runtime/eur_attack_helpers/`.

### Trampoline motion, effects and battle restoration (2026-09-08)

Seven Overlay 13 functions add 3,324 matching C++ bytes, reaching
467,612 / 1,563,700 bytes (29.90%). Two contiguous source units reconstruct
resource initialization, jump launch, bounce and airborne effects, retreat,
landing feedback and final battle restoration. The 580-byte work allocation
and 52-byte participant pair now have checked layouts. A model virtual method
at vtable offset 0x38 receives the second model pointer shown by its native
caller; its unknown name is retained pending fuller semantic evidence.

The complete ROM is byte-identical with SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Linking, 74 tests, progress
consistency and the public-content audit pass. These functions use ordinary
C++ without new inline assembly. The larger entry sequencer remains an
assembly gap because its compiler allocation differences are still unresolved.

Normal menu inputs from the new checkpoint 83 selected Trampolines. The
3,190-frame run exercised all seven functions and returned to Luigi's command
wheel. Full-byte guards and stack-matched return hooks verified seven resource
IDs, sizes and encoded load indices, saved background and original resources,
six jump configurations, two bounce launches and copy positions, 404 arc
completion results, 642 retreat completion results, and final background,
anchor and work-allocation cleanup. Resource indices store the configured
value plus one; motion destinations reside in channel parameters, separately
from the object's accumulated motion target.

The attack run used only ordinary button inputs and read-only RAM probes.
The original battery save remained unchanged. The starting Petey battle state
retains the previously documented temporary native encounter-command setup.
The private trace is `build/runtime/eur_attack_helpers/trampoline_verified83.json`.

### Item resources, positions and saved party state: 30% milestone (2026-09-08)

Ten Overlay 26 functions add 1,672 matching C++ bytes. Matching C/C++ now
covers 469,284 / 1,563,700 bytes (30.01%), independently of the additional
symbolic-assembly coverage. The new source reconstructs item resource loading,
launch trajectories, world and view positions, phase-specific offsets, party
effect selection and ownership, facing animation and state initialization.
Five source units preserve the contiguous native regions.

The initializer establishes the full 96-byte allocation size. The previous
72-byte prefix view is extended with an explicitly unknown tail, and the saved
object-data ID at tracked-object offset 6 now has a typed field. The item actor's
callback at offset 120 has a named view. Compile-time checks cover the state,
tracked object, effect slot and item actor layouts. Native loads establish that
the phase-animation table and save field at +0x558 are signed. No inline assembly
was added. The full ROM remains byte-identical with SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`; linking, 74 tests, progress
consistency and the public-content audit pass.

The new checkpoint 83 supplied the battery save. Normal command-wheel and
item-menu inputs first used a Red Pepper in a 1,320-frame run. Luigi then used
an ordinary jump, and Petey's unblocked attack reduced Mario from 100 to 87 HP
and Luigi from 114 to 101 HP. A 1,720-frame Mushroom run restored Mario to
his 100-HP maximum and returned to Luigi's command wheel. No code or RAM state
was patched during these runs, and the source battery save was unchanged.
The starting encounter retains the earlier documented native-command setup.

Full-byte-guarded, stack-matched hooks observed nine of the ten new functions.
Read-only checks covered resource IDs, allocation sizes and encoded indices,
all four saved actor states and original resources, the entry callback,
world and phase positions, three launched-item destinations, three view
positions using the live camera offsets, and effect-slot ownership. The
facing-animation helper, used by the revival sequence, was not reached.
Private evidence is in `item_red_pepper83.json`, `item_enemy_attack83.json`
and `item_healing_verified83.json` under `build/runtime/eur_attack_helpers/`.

### Item entry and resource selection (2026-09-08)

The first batch toward 50% adds 1,244 matching C++ bytes in two Overlay 26
functions, reaching 470,528 / 1,563,700 bytes (30.09%). It reconstructs the
resource choices for the user, target, partner and carried-baby cases, plus
the complete four-phase entry sequence and item-kind dispatch. Adjacent
resource, launch and effect-slot functions are consolidated into
`src/overlay026/item_sequence.cpp`. The state view now names the observed
signed phase bytes and timers without changing allocation size or offsets.

Checkpoint 83 replays cover both new functions: Red Pepper enters the sequence
61 times, and Mushroom enters it 54 times. Guarded RAM checks verify resource
selection for the user, resource-load waiting, phase transitions, the launch
timer and dispatch callbacks, alongside the existing position/resource
oracles. Both runs return to Luigi's command wheel, and Mushroom restores
Mario from 87 to 100 HP. The source save remains unchanged; the replays use
normal inputs from the previously documented battle setup. Revival and KO
resource branches are not covered by these two replays.

All 74 tests, native linking, progress consistency and the public-content
audit pass. The full ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Private evidence is in
`item_entry_pepper83.json` and `item_entry_healing83.json` under
`build/runtime/eur_attack_helpers/`. The larger group-item sequence is still
private WIP: its behavior and missing stack arguments are recovered, but
36 instruction words differ in register allocation at the exact native size.

### Shared motion effects (2026-09-08)

Eleven functions add 1,404 matching C++ bytes, reaching 471,932 / 1,563,700
bytes (30.18%). Five contiguous source units reconstruct scale-wave setup and
updates, transform and pursuit setup, follower setup and updates, sine-motion
setup, and appear/shrink effects. Typed parameter views fit the existing
16-byte motion-channel storage; compile-time size checks cover each layout.
Existing attack callers now use the recovered names. No inline assembly was
added.

Checkpoint 83 replays exercise five of the eleven functions. Bro Flower
verifies two transform setups, including scale/rotation endpoints and mode.
Pocket Chomp verifies two follower setups and 425 follower updates against
live object coordinates, previous positions and movement targets. The three
Bro-item replays each observe appear setup and verify nine appear updates
against the resident sine table. These checks use full native-byte guards
and stack-matched return hooks. The other six functions remain statically
verified; these replays do not establish their runtime coverage.

Trampoline, Bro Flower and Pocket Chomp runs use 3,190, 2,590 and 3,110 frames
respectively and return to Luigi's command wheel. Inputs are ordinary buttons
from the previously documented controlled encounter; checks only read RAM,
and the source battery save remains unchanged. Private evidence is in
`motion_trampoline83.json`, `motion_flower83.json` and `motion_chomp83.json`
under `build/runtime/eur_attack_helpers/`. Native linking, all 74 tests,
progress consistency and the public-content audit pass; the full ROM retains
SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

### Mix Flowers effect pairs and participants (2026-09-08)

Twenty functions add 2,320 matching C bytes, reaching 474,848 / 1,563,700
bytes (30.37%). Normal selection of Mix Flowers confirms Overlay 16's item
identity. Seven contiguous units reconstruct the effect-pair pool, resource
and animation changes, kind transitions, pursuit setup, reset and destruction,
plus participant initialization and three resource/animation phases. Checked
structures describe paired 260-byte scene objects in each 524-byte effect,
the 32-byte controller and 24-byte participants. Unobserved fields retain
neutral names. The native pursuit caller supplies `1` for the animation-state
argument omitted by the initial decompiler output.

A 3,070-frame checkpoint 83 replay executes and checks all 20 new functions.
Read-only oracles cover object positions, resource pointers, animation flags,
phase/kind updates, free-slot and idle queries, participant fields and model
release. The conditional reset is observed 452 times without a request and
four times with a request. The replay returns to Luigi's command wheel and
preserves the source save. Inputs are ordinary buttons from the previously
documented controlled encounter. This run also expands coverage of the prior
shared-motion batch to scale-wave setup and eight updates, four sine-move
setups and one pursuit setup; all corresponding RAM oracles pass.

Native linking, 74 tests, progress consistency and the public-content audit
pass, with canonical ROM SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Private evidence is
`build/runtime/eur_attack_helpers/ov16_mix_verified83.json`; its menu capture
confirms the selected item. No new inline assembly was added.

### Nawatobi rope updates (2026-09-08)

Five functions add 596 matching C bytes, reaching 472,528 / 1,563,700 bytes
(30.22%). They reconstruct point integration, ground and pinned-point
constraints, velocity damping, link-collision dispatch and the rope update
sequence. Checked layouts describe 40-byte points, 12-byte links and the
1,652-byte rope. The distance solver remains native assembly.

The checkpoint 65 probe enters Super Nawatobi Bros. using the previously
documented single phase-selector write in the fully initialized pause menu.
The native transition constructs and later destroys the minigame; this is a
controlled scene-entry test, not proof of an ordinary gameplay entry route.
After scene selection, checks only read memory. A 491-frame run executes all
five new functions and verifies 24,832 complete point records, 97 update
dispatch sequences and the arguments of 3,007 segment-collision calls.
The existing bitmap oracle also verifies 294,912 uploaded bytes. The run
exits with B and preserves the original battery save. Private evidence is
`build/runtime/eur_nawatobi/rope_evidence_065.json`.

Native linking, all 74 tests, progress consistency and the public-content
audit pass. The ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

### Mix Flowers entry, particles and passing (2026-09-08)

Twelve functions add 1,752 matching C bytes, reaching 476,600 / 1,563,700
bytes (30.48%). They reconstruct entry-object updates, growing and shrinking
particles, linked-object phases, participant scheduling and effect positioning,
and the large-fireball controller's scale, motion-step and distance queries.
Adjacent functions are consolidated into contiguous source units. Checked
layouts cover the complete 17,292-byte attack work area and its nested records.
Native signed loads identify the particle timer as a signed byte; the sine-move
call also establishes full-width integer coordinate arguments. Correcting that
shared prototype preserves the previously matching callee.

A 4,270-frame checkpoint 83 replay checks all twelve new functions, alongside
all twenty functions from the previous Mix Flowers batch. Automated ordinary
button inputs follow the participant phases and variant buttons read from RAM;
665 such inputs are supplied through frame 1,900, then the attack finishes and
returns to Luigi's command wheel. Checks cover 34 participant handovers, 36,636
distance queries, 666 effect placements, 560 shrinking-particle updates and
208 growing-particle updates. Full native-byte guards and stack-matched return
hooks protect the oracles. The source save remains unchanged, and the replay
uses the previously documented controlled encounter. Private evidence is
`build/runtime/eur_attack_helpers/ov16_extension_complete83.json`.

Native linking, all 74 tests, progress consistency and the public-content
audit pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Jump attack lifecycle, support and view coordinates (2026-09-08)

Thirteen functions add 1,704 matching C/C++ bytes, reaching 478,304 /
1,563,700 bytes (30.59%). Overlay 20 gains attack allocation and resource
loading, main-actor initialization, phase dispatch, approach movement,
animation-duration fitting and four support-state helpers. The checked
112-byte work layout and 116-byte configuration expose only fields supported
by these routines. The animation-duration function uses the existing C++
model class for its native virtual call. Three adjacent battle-core helpers
cover view-relative positions, text-resource lookup and resource-54 binding;
the last helper retains its address name because its wider role is unresolved.

Normal Jump replays from checkpoint 83 cover nine of the ten new Overlay 20
functions. A replay from the field checkpoint repeats the previously documented
72-byte encounter-command substitution, restores it at battle entry and
confirms carrying formation 4 and native support allocation. Subsequent
checks only read RAM. The successful Bros. Jump replay reads the game's input
window flags, presses X at frame 170 for the support jump, then A at frame 227
for the second impact. It verifies the support's placement and movement deltas,
four animation-rate calculations and the attack's return to Luigi's command
wheel. An ordinary single-jump replay separately covers support mode updates.
The original source save remains unchanged.

The position oracle checks 29,947 calls in the ordinary replay, covering both
camera views and raw screen coordinates, including the nonnegative depth
clamp. Text lookup is also executed. Support StopInput and the resource-54
binding helper are not reached by these encounters and remain statically
verified. Private evidence is in `ov20_jump83.json`,
`ov20_input_diagnostic83.json`, `ov20_bros_jump83.json` and
`ov20_fresh83.json` under `build/runtime/eur_attack_helpers/`.

Native linking, all 74 tests, progress consistency and the public-content
audit pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Encounter map selection and archive loading (2026-09-08)

Six functions add 8,720 matching C bytes, reaching 487,024 / 1,563,700
bytes (31.15%). The largest is the 7,568-byte mapping from encounter IDs to
battle-map resource indices. Its 620 explicit cases are recovered from the
native jump destinations and return values, independently compared with the
private decompiler export, and listed in their original code order. Separate
return blocks reproduce the native function exactly; grouping equal results
or replacing the switch with a table would change its binary representation.
The original -1/current-encounter fallback and default -1 result are preserved.

The adjacent functions open and queue `/BMap/BMap.dat`, open and read the
party/enemy/interface texture catalogs, and read the selected scene AI archive.
They reuse the checked context, queued-task and archive-request layouts. The
scene read is consolidated into the existing AI archive module. Native call
arguments establish the asynchronous read descriptors and offset tables that
the initial decompiler output omitted.

Fresh entries from checkpoints 83 and 65 execute and check all six functions.
The mapping returns 576 for encounter 459 and 416 for encounter 348. Each
replay opens and reads all three texture catalogs, checks allocation pointers
and aligned entry sizes against live archive offsets, verifies map request
state and the selected AI read buffer, and reaches a command wheel. Checkpoint
83 additionally completes the automated Bros. Jump. The encounter setup uses
the documented native command substitution, restored at battle entry; all
following checks are read-only and both source saves remain unchanged.
The current-encounter fallback, unmapped IDs and already-loaded catalog branch
are not covered by these two live replays. The complete mapping is statically
byte-verified. Private evidence is `map_catalog83.json` and `map_catalog65.json`
under `build/runtime/eur_attack_helpers/`.

Native linking, all 74 tests, progress consistency and the public-content
audit pass. The ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Battle-map transfer and display setup (2026-09-08)

Seven functions add 828 matching C bytes, reaching 487,852 / 1,563,700
bytes (31.20%). They read raw and compressed map sections, preserve the
palette, queue the second-display read and configure its background planes.
The archive offsets, destination pointers and section sizes retain their
native 16-entry layout.

Fresh battle entries from story checkpoints 83 and 65 execute all seven
functions. Read-only return hooks check section indices, sizes and destinations,
asynchronous callbacks and queue flags against live RAM. Each run verifies
all 512 copied palette bytes and 2,080 cleared VRAM bytes, plus both display
plane masks and the sub-display tile destination. Compressed sizes are checked
after decoding against the request's live output length. Checkpoint 83 also
completes an automated Bros. Jump; checkpoint 65 reaches Mario's command wheel.
The documented encounter-command substitution is restored at battle entry,
and both source battery saves remain unchanged. Private evidence is
`map_transfer83.json` and `map_transfer65.json` under
`build/runtime/eur_attack_helpers/`.

Native linking, all 74 tests, progress consistency and the public-content
audit pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Sub-display tilemap upload (2026-09-08)

The next 828-byte map-loader callback is now matching C, bringing the total
to 488,680 / 1,563,700 bytes (31.25%). It completes four asynchronous read
stages, splits a 64-by-32 tilemap into the DS's two 32-by-32 screen blocks,
clears absent optional backgrounds and starts the sub-display matrix animation.
The callback is consolidated into the existing contiguous map-transfer module.

Fresh checkpoint 83 and 65 battle entries execute all four stages. Each replay
compares all 4,096 uploaded BG1 bytes with an independently rearranged copy of
the live source data and verifies the 4,096 cleared bytes of each optional
background. Task completion, resource flags and read-stage transitions also
pass. Checkpoint 83 additionally exercises animation setup and completes the
automated Bros. Jump; checkpoint 65 covers completion without that animation.
Nonempty optional BG2/BG3 uploads are statically byte-verified but are not
reached by these two maps. The encounter substitution is restored at entry,
and the source battery saves are unchanged. Private evidence is
`map_sub_update83.json` and `map_sub_update65.json` under
`build/runtime/eur_attack_helpers/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Hammer attack lifecycle (2026-09-08)

Two more functions add 492 matching C++ bytes, reaching 489,172 / 1,563,700
bytes (31.28%). They allocate and initialize the 68-byte Hammer work area,
load its attack resource and advance the entry/active/completion lifecycle.
The adjacent initialization and return helpers are consolidated with them in
`src/overlay021/hammer_lifecycle.cpp`; the checked work layout is shared in
the attack header.

The checkpoint 83 replay enters Hammer through the command wheel and confirms
the target with X. Live input-window data drives the X impact input at frame
69. Return hooks verify the work pointers, formation-specific configuration,
resource 52, animation anchor and actor callback. All three lifecycle phases
execute, with 127 update calls; the callback and work pointer are cleared at
completion and the game returns to Luigi's command wheel. The replay uses
normal buttons and read-only memory checks, and preserves the source save.
Private evidence is `hammer_lifecycle_active83.json` under
`build/runtime/eur_attack_helpers/`, starting from the recorded target-selection
state `hammer_lifecycle83.dst`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Save/load menu model creation and cursor motion (2026-09-08)

Three functions add 664 matching C/C++ bytes, reaching 489,836 / 1,563,700
bytes (31.33%). The save and load menus now create their title and cursor
models in readable C++, preserving the native virtual animation calls,
resource ownership and task callbacks. The load-menu cursor's acceleration,
velocity, position and countdown update is reconstructed in C.

A normal cold boot with checkpoint 83 enters both menus using keypad input.
Read-only hooks validate both pairs of model/task pointers, task priority,
title timer, animation IDs, render flags, title coordinates and cursor callback.
Seven cursor updates are compared with independently calculated live RAM
results. All three new functions execute. The source battery save remains
unchanged. Private evidence is
`build/runtime/eur_save_load_models/evidence_083.json`, including the input
sequence, ROM/save hashes and observed object pointers.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Battle triangle transforms and depth sorting (2026-09-08)

Four functions add 1,028 matching C bytes, reaching 490,864 / 1,563,700
bytes (31.39%). The contiguous mesh module reconstructs triangle centering,
optional per-triangle updates, signed fixed-point vertex/center transforms,
and depth-sorted linked lists with optional backface rejection. Shared layouts
have compile-time checks for 6-byte vertices, 12-byte centers and 64-byte
triangles. Each matrix product retains the original independent truncation.

The checkpoint 83 probe uses the original Elder Shrooboid encounter command
from room 460, offset 0x18CE, with arguments [8231, -1, -1, 1, 0, 2]. The
72-byte temporary command is restored at native battle entry. Subsequent
Mario/Luigi Jump selections use only ordinary keypad input. The boss's
20-triangle effects execute all four new functions: 103,860 vertex transforms,
34,620 center transforms and 1,731 calls each to preparation and sorting.
Read-only return hooks compare transformed coordinates and sorted next pointers
with independent calculations from live RAM. Preparation also checks the
original colors, alpha values and centered source geometry when no triangle
callback is installed; callbacks may intentionally change these values.
All checks pass, with no pending returns. The active renderer uses culling
mode zero, so backface rejection remains statically verified only. The source
battery save is unchanged. Private evidence is `mesh_elder_battle83.json` and
`mesh_elder_verified83.json` under `build/runtime/eur_mesh/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Field snapshots, mesh tasks and title scrolling (2026-09-08)

Seven functions add 996 matching C bytes, reaching 491,860 / 1,563,700
bytes (31.45%). Field-area construction, destruction and packed snapshots use
checked 11,216-byte context and 16-byte snapshot layouts. Mesh hit-task setup,
fade completion and matrix rotation/normalization share checked 28-byte task
and 76-byte resource layouts. Title-panel scrolling preserves the four native
BG2/BG3 scroll writes across both display engines. The Battle VM now uses the
shared mesh declarations, and its complete dispatch function remains matching.

All seven functions execute in checkpoint 83 replays. A normal cold boot
checks all 164 MMIO writes made by 41 title-scroll calls. These write-only
registers are verified from the CPU operands at their native store instructions,
with a full-function byte guard, rather than by reading the registers back.
Normal save-block interaction captures room 459 and room 472 snapshots,
including preserved flag bits, signed area values and camera coordinates.
Two construction/destruction pairs are checked across the entire context.
The source battery save remains unchanged.

The native Elder Shrooboid encounter command from room 460 is restored at
battle entry as documented above. Its replay adds one checked construction/
destruction pair, 1,726 independently recalculated matrix rotations and
normalizations, two initialized hit tasks for objects 40/41 and actors 56/57,
and 182 fade-task updates including both completions. Rotation axes 0, 1 and 2
execute. All RAM and CPU-store checks pass with no pending return hooks.
Private evidence is `scene_support_cold_verified83.json`,
`scene_support_save83.json` and `scene_support_entry83.json` under
`build/runtime/eur_mesh/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Title textures and orbit animation (2026-09-08)

Fourteen functions add 1,472 matching C bytes, reaching 493,332 / 1,563,700
bytes (31.55%). Two contiguous modules reconstruct texture/palette loading,
release and metadata access, plus orbit initialization, reset, drawing,
destruction and three animation transitions. Checked resource and animation
layouts preserve the native fixed-point coordinates, model ownership and
separate child-renderer position view.

Two normal cold boots with checkpoint 83 exercise all fourteen functions.
The complete intro checks 17,234 orbit draws across all four rendering phases,
including 1,120 child draws, and all fourteen entry, expansion and pulse
transitions. Pressing Start during the intro adds fourteen checked resets.
Read-only hooks independently calculate coordinates, scales, timers and
callback changes from live RAM. Texture checks include resource-loader and
allocation arguments, actual file sizes, header metadata, image/palette
addresses and release state. All five loaded images use compression; the
uncompressed loader branch remains statically verified only. Both replays
finish without failed checks or pending return hooks. The source battery save
is unchanged. Private evidence is `title_effects_intro83.json` and
`title_effects_skip83.json` under `build/runtime/eur_title_effects/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Battle text, number formatting and VRAM uploads (2026-09-08)

Five functions add 1,500 matching C bytes, reaching 494,832 / 1,563,700
bytes (31.64%). One contiguous module reconstructs number requests, decimal
formatting, incremental text/number processing and tile-to-row pixel uploads.
The number buffer stores digits in reverse order for right-to-left drawing;
negative sentinels produce dashes. Checked request and upload-prefix layouts
replace opaque payload offsets. The target-label renderer uses the shared
upload declaration and remains byte-identical.

Checkpoint 83 battle entry verifies 34 resource-text updates and 142 uploads,
including independent comparison of 287,744 bytes in live VRAM. The original
Elder Shrooboid encounter command from room 460 is restored at native battle
entry as documented above. Scrolling the saved Bros.-Item list with normal
keypad input executes all five functions: 15 number requests, formatting calls
and completed number tasks, 241 resource-text updates and 53 uploads. Checks
cover packed coordinates and flags, all fourteen text-initialization arguments,
reversed number strings, per-token cursor movement, task completion and pixel
layout. Another 151,552 VRAM bytes match the independent tile conversion.
Nested text-parser calls are excluded using the verified direct call sites.
No runtime checks fail and no returns remain pending. Both source saves are
unchanged. Negative number sentinels and number-task clearing remain statically
verified only. Private evidence is `interface_text_entry83.json` and
`interface_text_bros_verified83.json` under `build/runtime/eur_interface_text/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Pocket Chomp table adjustments and object links (2026-09-08)

Three functions add 452 matching C bytes, reaching 495,284 / 1,563,700
bytes (31.67%). The badge-dependent table setup adds signed row adjustments
to six parameter tables. The two modified public threshold-table declarations
now correctly describe mutable data. A checked eight-byte tether payload
records the linked object and four signed offsets; its setup and the
24-to-192 distance clamp are reconstructed in a contiguous pair of functions.

Checkpoint 83 replay selects Pocket Chomp from the Bros.-Item list. A new
input driver reads the original adult-controller phase, countdown and pending
grade, then presses the actor's native button when the countdown is one.
The next game update decrements it to the documented timing point. These are
ordinary keypad inputs; no RAM or register values are changed. Seventeen
successful input events produce seventeen checked tether setups and sixteen
checked distance results. Both directions execute, with eight unrestricted
results and eight upper-bound clamps. All payload bytes, motion callback,
channel timing and return pointers match live RAM. The lower-bound clamp and
badge-dependent table adjustment are statically verified only; the equipped
badge in this replay does not enable that branch. Source save 83 is unchanged.
Private evidence is `chomp_common83.json` and `chomp_auto83.json` under
`build/runtime/eur_chomp_common/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Shop row updates and sprite/model submission (2026-09-09)

Six adjacent callbacks add 1,320 matching C++ bytes, reaching
541,724 / 1,563,700 bytes (34.64%). A checked task layout describes the row's
displayed value, palette, priority, scale, tile base and position. The callbacks
update that state from the existing list, refresh changed numeric text, position
the row and anchor models, configure sprite affine matrices, and submit the
visible pieces. The preceding constructor is deferred with twelve register-only
instruction differences; it contributes no matching-C credit.

The established shop probe enters normal and equipment shops from story saves
86 and 65. It temporarily substitutes the decoded Field VM shop command and
restores all 72 bytes at the native helper before the scene transition. Ordinary
inputs open the lists, move the selection, cancel and exit without a purchase.
The two relevant runs cover 2,202 frames and all six new callbacks, including
438 visible row-model submissions in the equipment shop. They pass 4,784 full
row-task comparisons, 38,988 renderer-prefix comparisons, 32,669 affine-argument
checks, 31,245 submission-argument checks and 13 numeric-refresh argument checks.
The probe computes positions, scales, tile indices and visibility from live
data; the existing virtual value/availability helpers are observed separately
to check the caller's use of their returns. The previous list, panel, text and
lifecycle checks also pass, including 196,608 background VRAM bytes in the
equipment-shop run.

Private reports are `evidence_086_00.json` and `evidence_065_02.json` under
`build/runtime/eur_shop_rows/`. Both shops close completely, the temporary command
is restored, and the supplied battery saves remain unchanged. Native linking,
all 74 tests, progress checks and the public-content audit pass. The ROM retains
SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`; no inline assembly was added.

### Field party pairing and collision-state transitions (2026-09-08)

Seven functions add 3,820 matching C++ bytes, reaching 499,104 / 1,563,700
bytes (31.92%). Checked partial party-entity and controller views describe
paired members, their separate linear controllers, saved collision flags and
saved presentation modes. The area entity list reuses the existing resource
context. Two contiguous source regions cover paired waiting/following/linking
and collision override preparation, restoration and restart, including the
still neutrally named state-34 transition. A 136-byte contact scanner between
the regions remains in assembly; its C version still differs in register use.

Ordinary inputs from story checkpoints 83 and 65 verify the paired state
transitions, reciprocal entity links, flags, zeroed movement speeds, matching
locomotion states, eight-frame following duration and target pointers in live
RAM. The checkpoint-83 replay checks three waits, three following starts and
six member links; checkpoint 65 adds one wait, one following start and two
links. All return checks pass with no pending hooks. The four collision/state
transitions have not yet been reached by these input sequences and remain
statically verified only. In particular, the state-34 name does not claim a
gameplay identity. Source battery saves are unchanged. Private evidence is
`party_inputs83.json` and `party_actions65.json` under
`build/runtime/eur_party_states/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Field party auxiliary objects and follower transitions (2026-09-08)

Six adjacent functions add 2,988 matching C++ bytes, reaching
502,092 / 1,563,700 bytes (32.11%). The auxiliary-object setup and restart
restore renderer and collision state, bind the resource record and follow the
leader with a direction-table lookup. Three follower transitions use the
saved separation, elapsed movement time and navigation height to configure
timed following and vertical movement. Checked views now distinguish the
auxiliary motion payload from the linear controller occupying the same bytes.
The setup writes the confirmed animation field at +0x19E; the follower return
preserves subtraction before addition in its height calculation.

The new six functions are statically verified only: the ordinary-input replay
from checkpoint 55 does not enter these special modes. A checkpoint-83 replay
on the rebuilt ROM rechecks one paired wait, one following start and two
reciprocal member links, with no failed checks or pending returns. This
confirms the existing pairing paths after the shared-layout extensions; it
does not claim execution of the new auxiliary paths. Both battery saves are
unchanged. Private evidence is `party_movement55.json` and
`party_auxiliary_smoke83.json` under `build/runtime/eur_party_states/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Compact field-party entity snapshots (2026-09-08)

Two contiguous functions add 3,156 matching C bytes, reaching
505,248 / 1,563,700 bytes (32.31%). The checked 136-byte snapshot layout
packs animation, locomotion, presentation and contact state, stores linked
entities by signed index, and preserves reserved bits and byte +0x51.
Restoration resolves the index and synchronizes current and previous position,
relative height and support clearance. Signed blink frames and resource indices
remain explicit in the shared layouts; animation IDs are unsigned halfwords.

Read-only entry/return probes on the rebuilt ROM check four complete 136-byte
snapshot writes from checkpoint 83. A normal battery boot and load of the same
checkpoint checks four complete 1,436-byte entity views after restoration,
including bytes that must remain unchanged and the returned entity pointer.
All comparisons pass, with no failed or pending callbacks. These runs exercise
the null linked-entity case; non-null index resolution remains statically
verified only. The cold-load screenshot shows the party in the Space Castle
field beside the save block. The source battery save is unchanged. Private
evidence is `snapshot83.json` and `snapshot_cold83.json` under
`build/runtime/eur_party_snapshot/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Entity state copy hierarchy (2026-09-09)

Three functions add 4,028 matching C bytes, reaching 509,276 / 1,563,700
bytes (32.57%). Base, planar and spatial copies form a checked hierarchy.
They preserve destination vtables and renderer bindings, transfer selected
flag fields independently, copy the embedded controller payloads in an
overlap-safe direction, and reset the base self pointer to the destination.
The spatial copy transfers its still unidentified 260-byte payload as an
aggregate. Its internals retain neutral names. Existing deferred-entity code
now calls the named spatial copy through the shared declaration.

Normal checkpoint-83 battery boot/load followed by save-block interaction
executes each function 60 times. Read-only return probes compare the entire
236-byte base, 688-byte planar and 1,300-byte spatial views, including retained
bytes, against the original load/store ranges and masks. Thirty calls per
function use each address ordering for the directional block copies. Every
comparison and returned destination pointer passes; no callbacks remain
pending. This establishes the copy behavior during the interaction; the final
screen is black and does not establish completion of the save-menu flow.
A separate cold-load replay reaches the visible field normally. The source
battery save is unchanged. Private evidence is `copy_cold83.json` and
`copy_field83.json` under `build/runtime/eur_entity_copy/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Party controller storage and snapshot capture (2026-09-09)

Three contiguous functions add 1,440 matching C bytes, reaching
510,716 / 1,563,700 bytes (32.66%). The checked controller allocation is
8,356 bytes and contains two 1,440-byte backup entities. Its 384-byte snapshot
stores party flags, input masks, action selection, separation, two compact
entity snapshots and two opaque 32-byte records. Capture selects live entities,
backup entities or existing compact snapshots according to the backup flags.
Initialization constructs the backups, clears the controller and sets the
signed unknown byte at +0x55 to -1; destruction releases the backup array.

A normal checkpoint-83 boot/load and save-block interaction verifies two
complete controller initializations and two complete 384-byte snapshot writes
from live entities. The return probes compare reserved bytes as well as
transferred fields; all checks pass with no pending callbacks. Backup capture
branches and backup destruction remain statically verified only. The source
battery save is unchanged. Private evidence is `storage_cold83.json` under
`build/runtime/eur_party_storage/`. The companion controller restoration
routine remains assembly: a private C candidate has nine differing register
uses in the two-member pointer loop. No claim of completion is made for it.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Variable-bound entities and specialized state copies (2026-09-09)

Five functions add 2,976 matching C bytes, reaching 513,692 / 1,563,700
bytes (32.85%). Two constructors initialize an entity from either the generic
spawn format or a 12-byte placement with a persistent script-variable binding.
A checked 16-byte state describes the transferred fields without assigning an
unconfirmed gameplay class. The constructors preserve the original signed
32-bit collision masks extended to 64 bits and wrapped variable-bank offset.
Three copies transfer variable-bound, party and auxiliary state before calling
the shared spatial copy. Signed byte and nibble fields now retain their -1
sentinels in the checked party layout.

A normal checkpoint-83 boot/load and save-block interaction reaches all five
functions: six generic constructions, twelve variable-bound constructions,
eight party copies, thirty-two auxiliary copies and eighteen variable-entity
copies. Read-only probes compare complete destination views, including retained
bytes. Constructor checks use the observed base-initializer result, placement
record, direction table and original VM variable return. All twelve variable
lookups select the enabled case; the disabled case remains statically verified
only. The copy runs cover both source/destination address orderings equally.
All comparisons and returned pointers pass, with no pending callbacks. The
source battery save is unchanged. Private evidence is `specialized_cold83.json`
under `build/runtime/eur_specialized_entity/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Renderer presentation snapshots (2026-09-09)

Two contiguous functions add 1,296 matching C bytes, reaching
514,988 / 1,563,700 bytes (32.93%). A checked 36-byte view names the saved
animation, scale, rotation, ordering and overlap-priority fields. The existing
animation-renderer extension supplies the finished flag and signed loop count;
its base allocation size is unchanged. Both operations can use the entity's
embedded storage or a caller-provided record. Invalid snapshots and absent
renderers retain the original early-return behavior.

Opening the pause menu with Start and closing it with B from checkpoint 83
checks 36 saves and 36 restore returns. Nineteen saves contain valid renderer
state, sixteen clear the valid flag and one has no renderer. Nineteen restores
compare every transferred field immediately before the final virtual refresh;
seventeen return without applying a snapshot. Full saved records, including
reserved bytes, match. The external-storage path remains statically verified
only. The replay returns to the visible field normally. Additional identical
pause-menu replays recheck all three base/planar/spatial copies (60 calls each)
and all five specialized entity routines, closing the prior end-to-end screen
observation gap for those paths. No callback failures or pending returns occur,
and the source battery save is unchanged. Private evidence is `render_pause83.json`
under `build/runtime/eur_render_snapshot/`, `copy_pause83.json` under
`build/runtime/eur_entity_copy/` and `specialized_pause83.json` under
`build/runtime/eur_specialized_entity/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Field area state and graphics refresh (2026-09-09)

Two contiguous functions add 1,936 matching C bytes, reaching
516,924 / 1,563,700 bytes (33.06%). The checked 11,216-byte area view now
represents the copied state flags, camera values, input masks and embedded
script manager. State transfer retains resource pointers and reserved bytes;
four opaque blocks use the original directional copy helpers. Graphics refresh
uploads the selected screen's sprites and palettes when the area state or
refresh flag requires it, then runs the state-71 update callbacks.

A checkpoint-83 Start/B pause-menu roundtrip verifies two complete area copies,
one for each source/destination address ordering, and their returned pointers.
Another 526 return checks verify the graphics-refresh flag against its native
entry conditions across the observed area states. These checks establish flag
handling; they do not independently validate every graphics upload. All checks
pass, no callbacks remain pending, and the replay returns to the visible field.
The source battery save is unchanged. Private evidence is `area_pause83.json`
under `build/runtime/eur_area_state/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Field scaling, rotation and animation control (2026-09-09)

Eleven functions add 1,752 matching C++ bytes, reaching
518,676 / 1,563,700 bytes (33.17%). A checked 48-byte controller describes
independent X/Y scaling and rotation, pause flags, signed steps, elapsed frames
and destination values. Reconstruction covers frame advancement, completion
and overshoot checks, timed rotation, both scaling start modes, scaling
pause/resume, cancellation and the default blink animation callbacks. Existing
VM and motion callers now share the typed interface. The adjacent speed-based
rotation starter remains assembly: its private candidate has four instruction
scheduling differences. No compiler-configuration search was used.

A checkpoint-83 Start/B pause-menu roundtrip checks 2,112 returns each from
advancement and completion against full controller and renderer views. A normal
checkpoint-27 battery boot/load checks another 4,798 returns each and four
scaling cancellations. All observed controllers are inactive, so active
transforms, external controller storage, the start/pause/resume routines and
blink callbacks remain statically verified only. No failures or pending
callbacks occur, both replays reach the visible field, and the source battery
saves are unchanged. Private evidence is `transform_pause83.json` and
`transform_cold27.json` under `build/runtime/eur_field_transform/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Field system initialization and aggregate snapshots (2026-09-09)

Eight functions add 1,596 matching C bytes, reaching
520,272 / 1,563,700 bytes (33.27%). Checked layouts describe the 952-byte
field task, 16,764-byte party manager, 772-byte party-manager snapshot and
804-byte aggregate field snapshot. The field constructor either allocates its
persistent timer/model/party systems or reuses them, creates the archive and
update/IRQ tasks, and initializes sprite animation. Its ARM-to-Thumb VBlank
call is verified in the native link. The touch poll preserves the original
signed nine-bit coordinate sentinels and pressed/held/released state handling.

A normal checkpoint-83 boot/load verifies the party manager's complete zeroed
allocation and the field constructor's 912-byte storage plus retained prefix
fields. Scheduler-maintained previous/next links are excluded from the
constructor comparison. A save-block interaction verifies the reused-system
constructor path, the full 772-byte manager snapshot and the full 804-byte
aggregate snapshot, including reserved fields. The deleting destructor's
returned pointer is checked. Both constructor paths and all snapshot comparisons
pass. The save-block replay ends black, so it does not establish completion of
the save-menu return; the cold-load replay reaches the visible field normally.

The two replays also check 936 touch returns. Touch input remains disabled in
these field scenes, including during ordinary simulated stylus contacts; active
sampling branches remain statically verified only. Manager destruction, the
non-deleting field destructor and allocation failures are also static-only.
No callback failures or pending returns remain, and the source battery save is
unchanged. Private evidence is `system_cold83.json` and `system_snapshot83.json`
under `build/runtime/eur_field_system/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Field display setup and live palette backups (2026-09-09)

Three contiguous functions add 1,180 matching C bytes, reaching
521,452 / 1,563,700 bytes (33.35%). Display initialization clears VRAM and
palette memory, assigns both engines' banks and establishes the 3D state.
Palette backup captures the standard BG/OBJ palettes, three selected extended
BG slots and the extended OBJ palettes into one checked 10,752-byte record per
screen. It temporarily maps the relevant banks for CPU reads and restores their
previous assignments. Both native swap-command writes are retained.

A checkpoint-83 save-block interaction verifies all twelve copies directly
against live palette RAM and the temporarily mapped VRAM, then compares both
complete backup records. The main/sub records contain 3,109 and 1,912 nonzero
bytes respectively. All 26 bytes of the bank-assignment state and both engines'
extended-palette enable bits return to their entry values. Reinitialization
checks the complete 671,744-byte VRAM clear and 2,048-byte palette clear, the
eight configured bank assignments and display-mode bits. Guarded instruction
hooks verify five 3D-control stores, both swap commands and the viewport command
from their CPU operands, including write-only registers.

A separate normal battery boot/load independently repeats both initialization
checks and reaches the visible field. The save-block flow still has the already
recorded black final screen; the palette-copy evidence does not assert successful
menu completion. All checks pass with no pending callbacks, and the source
battery save is unchanged. Private evidence is `display_save83.json` and
`display_cold83.json` under `build/runtime/eur_field_display/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Shared field resources, sprite windows and HP helpers (2026-09-09)

Four contiguous functions add 548 matching C bytes, reaching
522,000 / 1,563,700 bytes (33.38%). Resource teardown frees graphics and
animation buffers for 85 shared primary records, optional bounds, two secondary
buffers and their owning arrays. Window setup constructs the sprite-window
manager and allocates eight windows and two sprite slots. HP helpers refill a
member or apply a signed delta clamped to zero and maximum HP, then refresh the
second area's HUD when its state is 71. Existing VM calls now use these names
and the shared field-system interface.

Checkpoint-83 boot/load and save-block replays each verify the window pool's
eight-entry capacity, two sprite slots and allocated pointers. The save-block
replay verifies all 231 resource-array deletion arguments in exact native order,
including 57 non-null bounds arrays; null bounds are skipped. HP operations and
allocation failures were not reached and remain statically verified only.
No callback failures or pending returns occur and the source battery save is
unchanged. The cold-load replay reaches the visible field; the save-block flow
retains the previously documented black final screen. Private evidence is
`resources_cold83.json` and `resources_save83.json` under
`build/runtime/eur_field_resources/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Party following and indicator helpers (2026-09-09)

Eleven contiguous functions add 2,276 matching C bytes, reaching
524,276 / 1,563,700 bytes (33.53%). Following controls retain the original
collision-category masks, reset contact flags, transfer locomotion parameters,
and start or finish the follower's rejoin movement. Indicator helpers set both
renderer anchors, place the individual indicators, and advance their short
vertical slides. The checked party layout now exposes the indicator coordinates
and the embedded following-state buffer. Existing VM commands use the shared
interface. Unknown renderer fields and state values retain neutral names.

Normal battery boots of story checkpoints 1 and 83 reach the visible field.
Read-only probes verify five contact resets against both complete 1,440-byte
party entities, three indicator-layer changes against complete 312-byte renderer
records (including both layer choices), and three following-flag updates after
the native history helper returns. Live pointers confirm that the follower flag
can alias the controller's embedded buffer. Two active following initializations
verify the six locomotion parameters, animation speed, cleared velocity fields
and all arguments passed to the native history initializer. In addition, 1,907
idle rejoin checks and 1,701 idle slide updates preserve the full controller.
A separate movement/pause replay also completes without a mismatch.

Active rejoin movement, enabling/disabling following, indicator positioning and
active slide branches were not reached by these ordinary-input replays and
remain statically verified only. All observed checks pass with no pending
callbacks; both source battery saves are unchanged. Private evidence is
`following_cold83.json`, `following_story1.json` and `following_move83.json`
under `build/runtime/eur_party_following/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Party indicator visibility and swap animation (2026-09-09)

Four more functions complete the adjacent indicator module, adding 1,668
matching C bytes and reaching 525,944 / 1,563,700 bytes (33.63%). They show or
hide indicators immediately or through a vertical transition, coordinate area
refreshes, and play the eleven-frame table-driven swap animation. The verified
flow also corrects opcode `0x0B7`: its C name and research description now identify
indicator visibility. The old `set_party_facing_direction` string is retained
as a legacy serialized name so existing editable script files remain compatible.

A normal checkpoint-83 boot/load and pause round trip verify 1,618 idle visibility
updates against the complete party controller. A separate `R`, `L`, `X`, `Y`
replay exercises two complete eleven-frame swaps: both starts, all 22 updates,
the frame counter's terminal transition, offsets from the live animation table,
eleven sets of indicator positions, and both renderer-layer selections pass.
Nested following initialization also checks its forwarded parameters. The replay
ends at the game's save-choice menu, reached through ordinary controls; it does
not select a save operation. All checks pass with no pending callbacks and the
source battery save remains unchanged.

The actual show/hide transition branches, reversal and area-transfer paths were
not reached by these replays and remain statically verified only. Private
evidence is `visibility_cold83.json`, `visibility_pause83.json` and
`visibility_controls83.json` under `build/runtime/eur_party_visibility/`.
Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Packed field boundary records (2026-09-09)

Two adjacent conversion routines add 1,016 matching C bytes, reaching
526,960 / 1,563,700 bytes (33.70%). Checked 24-byte packed records expand into
40-byte boundary records or 44-byte paired bounds. The C reproduces fixed-point
coordinate conversion, packed plane values, individual mode/enable fields,
reserved bits and the extra output terminator. Names remain neutral where the
exact gameplay role of a coordinate or flag is still unknown.

Normal battery boots of story checkpoints 83 and 27 exercise both converters:
nine boundary records across two calls and two paired-bounds records in one
call. The probe compares all output bytes, including retained fields and the
additional terminators, against predictions made from the live packed records.
It also verifies that both source streams and source battery saves remain
unchanged. All checks pass with no pending callbacks. Null source streams and
negative coordinates were not observed and remain statically verified only.
Private evidence is `bounds_cold83.json` and `bounds_cold27.json` under
`build/runtime/eur_bounds_records/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Quad regions and variable-entity creation (2026-09-09)

Two adjacent field initialization functions add 1,212 matching C bytes, reaching
528,172 / 1,563,700 bytes (33.78%). Quad-region setup expands four corners,
vertical limits and packed metadata into checked 44-byte records, restores each
screen's saved activation mask, writes a terminator, and clears the consumed
mask. Variable-entity setup consumes a terminated 12-byte placement stream,
skips mode zero, constructs modes one through three, and updates the area's
entity table and counters. The placement terminator bit and previously opaque
area fields now have explicit names.

Normal battery boots of checkpoints 83 and 27 verify four calls to each routine
against the complete 11,216-byte area context. Quad checks cover two populated
records with finite height and two allocated empty lists, including their full
output buffers, terminators and consumed masks. Variable checks cover all four
placement modes: five mode-three, one mode-one and one mode-two objects are
constructed; one mode-zero record is skipped. All seven constructors receive
the expected entity index, screen and placement pointer, and the returned
objects occupy the expected slots. Null placement lists are also observed.

The live packed records and both source battery saves remain unchanged. All
checks pass with no pending callbacks. Negative coordinates, unbounded heights,
null quad sources and allocation failures were not reached and remain
statically verified only. Private evidence is `regions_cold83.json` and
`regions_cold27.json` under `build/runtime/eur_area_regions/`.
Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Navigation records, bounds and saved changes (2026-09-09)

Six functions add 1,948 matching C++ bytes, reaching 530,120 / 1,563,700
bytes (33.90%). They allocate the room's navigation and boundary arrays,
expand checked 40-byte source records into 92-byte surfaces, compute vertex
bounds, derive the dominant slope axis, and capture/restore per-screen change
masks. The event application wrapper also marks eligible entities for position
synchronization. Typed virtual methods reproduce the original dispatch order;
packed flags with unresolved gameplay roles keep neutral names.

Normal battery boots of checkpoints 83 and 27, followed by a checkpoint-83 pause
round trip, verify all 374 expanded surfaces and their full output buffers,
retained fields, terminators and source records. All four observed surface types
are covered, with 128 triangles and 246 quadrilaterals. For 32 sloped records,
96 native height queries receive the expected center/offset coordinates, and
their returned heights produce the expected stored slope axis. Six allocation
passes check the full area context, eight allocation requests, null resources,
and each allocated buffer's initial terminator without modifying retained bytes.

The pause round trip also verifies two save-mask calls over modes zero and three,
and two restore calls with empty masks. Enabled saved changes, event application,
negative coordinates, the other slope-axis result and allocation failures were
not reached and remain statically verified only. Every probe passes with no
pending callbacks; both source battery saves remain unchanged. Private evidence
is `navigation_cold83.json`, `navigation_cold27.json` and `navigation_pause83.json`
under `build/runtime/eur_navigation/`.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Spin-jump and partner movement transitions (2026-09-09)

Ten adjacent state-transition functions add 4,220 matching C++ bytes, reaching
534,340 / 1,563,700 bytes (34.17%). The shared 32-byte party state record now
exposes movement parameters, facing/rotation values and its timer, while keeping
its original word-array view for existing callers. The reconstructed code covers
spin-jump entry and falling, partner stacking/separation, timed following, ground
movement restoration and state-specific resume paths. Unobserved states retain
neutral numeric names. Virtual locomotion dispatch remains explicit C++.

A reproducible checkpoint-83 control sequence moves away from the save block,
sets the babies down, changes the adults' action mode, and performs their paired
spin jump. Six new functions execute: stacking, restoring movement with the
partner hidden, jump entry, falling, restoring ground movement, and separation.
The probe checks the full 8,356-byte controller, both 1,440-byte entities and
both aliased 32-byte state records at 23 checkpoints. Expectations are rebased
after existing helper calls, so those helpers' changes are not attributed to
the new caller. Movement, position, facing, renderer and timed-following call
arguments are checked separately. All comparisons pass; the final frame shows
the separated adults standing normally. A shorter replay captures the paired
spin jump in midair. No save operation or RAM patch is used.

The standalone state-11 timer setter, state-11/17 resume, ground resume and
follower-offset starter were not reached and remain statically verified only.
The ground restore used the saved-behavior branch; its default branch remains
static. Private evidence is `transitions_spin83_verified.json` and
`transitions_air83.json` under `build/runtime/eur_party_transitions/`; the common
starting state `transitions_action_mode83.dst` has SHA-1
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`. Every probe finishes without pending
callbacks and the checkpoint-83 source battery save remains unchanged.

Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Bros. Ball entry, rolling and target-height following (2026-09-09)

Three preceding functions extend the same transition module by 1,496 matching
C++ bytes, reaching 535,836 / 1,563,700 bytes (34.27%). Bros. Ball entry resets
movement and presentation flags, applies a facing-dependent position offset,
and hides the partner. Rolling setup selects initial speed, acceleration,
movement flags and an effect request. The third helper starts following an
entity only in the accepted locomotion states and within a +/-16-unit height
window. Its exact gameplay target has not been observed, so its name describes
the verified condition rather than guessing a particular object.

From the existing checkpoint-83 action state, normal `A`, wait, and left inputs
enter Bros. Ball and begin rolling. Both new action routines pass nine full-memory
checkpoints covering the party controller, entities, aliased state records and
both renderers. The probe verifies the facing-six offset from the live table,
normal initial speed, acceleration, renderer request and effect arguments.
Expectations are rebased after existing helper calls as in the preceding batch.
Screenshots show the active Bros. Ball and the adults standing separately after
an ordinary `A` exit. Private evidence is `transitions_roll83.json` and
`transitions_roll_exit83.json` under `build/runtime/eur_party_transitions/`.

The fast rolling entry and target-height following helper were not reached and
remain statically verified only. All runtime comparisons pass with no pending
callbacks and the source battery save remains unchanged. Native linking, all
74 tests, progress consistency and the public-content audit pass. The complete
ROM retains SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
No inline assembly was added.

### Bros. Ball restoration, scripted movement and separation (2026-09-09)

Five adjacent functions extend the party transition module by 3,000 matching
C++ bytes, reaching 538,836 / 1,563,700 bytes (34.46%). They restore rolling
parameters, resume idle or rolling presentation, separate the adults, freeze
the idle animation, and start the directional movement used by state 23.
The last state retains its numeric name. Native call setup establishes the
eight-argument direction helper and the three-argument animation virtual call;
the decompiler's apparent extra virtual-call argument was stale stack data.

A checkpoint-83 replay enters Bros. Ball, rolls left and exits with A. It
observes restoration from state 84 to rolling and separation of both adults,
passing 17 full-memory checkpoints and all 15 helper-call argument checks.
The probe compares the complete party controller, both entities, their aliased
state records and both renderers. Live direction-six offsets, both 16-unit
vertical movements, the 16-frame separation controllers, distinct leader and
follower animations, and the sound requests agree with the reconstruction.
The screenshot shows both adults standing separately. A second ordinary-input
replay verifies separation again. Private evidence is `ball_roll_exit83.json`
and `ball_pause83.json` under `build/runtime/eur_party_ball/`.

State-23 entry, the resume branches and the scripted stop helper were not
reached and remain statically verified only. Their VM callers explain why
releasing the direction button does not exercise the scripted stop. Runtime
comparisons pass without pending callbacks or changes to the source battery
save. Native linking, all 74 tests, progress consistency and the public-content
audit pass; the ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Baby hammer entry, cleanup and active-state query (2026-09-09)

Three functions add 1,568 matching C++ bytes, reaching
540,404 / 1,563,700 bytes (34.56%). They start a member's hammer swing,
restore movement and presentation afterward, and test the four hammer states.
The shared state record exposes the flag and saved area value used by cleanup.
The corresponding field VM commands now call the typed public functions.
Closing this gap allows the adjacent collision, auxiliary and action transition
sources to become one contiguous module, with the original function order.

Normal X, wait and R inputs select the babies' hammer action from the existing
checkpoint-83 setup. Another X press performs the swing. Entry and cleanup pass
12 full-memory checkpoints, covering the controller, both party entities and
state records, the selected auxiliary entity, its animation renderer and the
active area. Ten helper calls have their arguments checked. The auxiliary
resource, facing, overlap priorities, one-loop animation and sound request all
agree; cleanup restores both saved behavior and the saved area value. A short
replay visibly captures Baby Mario's hammer. Evidence is `hammer83.json` and
`hammer_swing83.json` under `build/runtime/eur_party_hammer/`; the starting state
has SHA-1 `cf09cafb85756ef83e5febafc8e0d89e20deea29`.

The active-state query, member-one path and enabled-rumble branch were not
observed and remain statically verified only. The source save remains unchanged
and every probe finishes without failures or pending callbacks. Native linking,
all 74 tests, progress checks and the public-content audit pass. The complete
ROM remains byte-identical, with SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. No inline assembly was added.

### Inventory row quantities, values and digit visibility (2026-09-09)

Five adjacent callbacks add 796 matching C++ bytes, reaching
542,520 / 1,563,700 bytes (34.69%). They update the inventory row's quantity,
displayed value, palette, tile base and position, submit its text and value
sprites, and suppress a zero tens digit while retaining the ones digit. A
checked 72-byte layout describes the inventory task. Both row modules share
the sprite prefix and common renderer declarations in an internal header.

The existing controlled shop entry uses story save 86. The original 72-byte
command is restored before scene entry. One replay opens and cancels the
quantity view; another uses ordinary R inputs in the inventory list to reach
other categories, including single-digit clothing quantities. The two runs
cover 2,102 frames and pass 7,630 complete task comparisons, 68,670 sprite-prefix
comparisons, 67,816 submission argument checks and 27 numeric-refresh checks.
The category replay observes 854 suppressed tens digits and always retains the
ones digit. Virtual quantity/value helpers and availability checks are observed
separately; the probe validates how the reconstructed caller stores their
results. Previous shop, list, panel and text checks pass as well.

Private reports are `evidence_086_01.json` under
`build/runtime/eur_shop_inventory_rows/` and
`build/runtime/eur_shop_inventory_categories/`. Both scenes are fully cleaned
up, no purchase occurs, all temporary command bytes are restored and the source
battery save is unchanged. Native linking, all 74 tests, progress consistency
and the public-content audit pass. The ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`; no inline assembly was added.

### Inventory panel selection, categories and price bonus (2026-09-09)

Nine functions add 1,400 matching C++ bytes, reaching
543,920 / 1,563,700 bytes (34.78%). The reconstructed methods hide and show
inventory rows, retain the selection, cycle nonempty categories, rebuild the
packed item list, report quantities and sellability, display the empty-list
message, and calculate the difference between the displayed price and its
quarter-price base. The common panel now exposes its inline item storage;
the checked 848-byte selling-panel layout adds the four category offsets.
The adjacent row callbacks and methods share one contiguous source module.
The price and category-append functions between that module and the list
rebuilder remain assembly because their C candidates differ in register use.

The existing controlled shop entry, with all 72 decoded command bytes restored
before the scene transition, was replayed from story checkpoints 86 and 65.
Ordinary R input changes categories and B exits. Across 1,882 frames, eight of
the nine new methods pass 16,364 complete panel-memory comparisons, including
6,090 quantity queries, 6,090 sellability queries, two list rebuilds and two
show/hide pairs. Both complete 8-KiB save-memory comparisons confirm the two
selection-byte writes. There are 2,610 checked price-bonus returns; their base
comes from the live item record and their displayed-price input is observed
at the existing virtual helper's return. Rebuild checks derive the item list
from inventory and equipment RAM, observing the badge-unlock VM result.
The previous task, sprite, text, background VRAM and cleanup oracles also pass.

Private reports are `evidence_086_01.json` and `evidence_065_01.json` under
`build/runtime/eur_shop_inventory_panel/`. Neither replay performs a transaction
or changes a supplied battery save, and both finish with complete scene cleanup
and no pending callbacks. The empty-list message, removal-adjustment branch,
and backward category wrap remain statically verified only. An exploratory
checkpoint-30 replay reached an additional shop conversation and did not close
the scene within its input sequence; it is excluded from these passing totals.
Native linking, all 74 tests, progress consistency and the public-content audit
pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

### Save/load menu scrolling text and direction arrows (2026-09-09)

Three adjacent callbacks add 644 matching C++ bytes, reaching
544,564 / 1,563,700 bytes (34.83%). The checked 72-byte task layout covers the
scroll controller and its text/arrow children. The controller waits briefly,
scrolls automatically, responds to L/R, wraps at both ends and requests a new
strip when its source changes. The child callbacks position the text and
animate the two direction arrows. The strip constructor and text-generation
helper remain assembly; their private C++ candidates are not credited.

A normal cold boot using story save 86 and keypad input reaches the load menu
and exercises all three callbacks. The 3,057-frame replay checks 35,616 complete
task buffers and 34,503 complete renderer buffers, plus every corresponding
submission's object and priority arguments. This includes 1,113 controller
updates, 32,277 text submissions and 2,226 arrow submissions. It observes the
initial delay, 751 automatic updates, 120 R-directed updates, 180 L-directed
updates, and one wrap at each end. The paused and source-replacement paths were
not observed and remain statically verified only. The captured screen was
inspected; the existing summary, localization, model and scene checks pass.

Private evidence is `build/runtime/eur_save_scroll/evidence_086.json`.
No ROM, script or RAM substitution is used, the supplied battery save is
unchanged and no callbacks remain pending. Native linking, all 74 tests,
progress consistency and the public-content audit pass. The ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

### Baby Spin entry, hovering, glide and cancellation (2026-09-09)

Seven adjacent functions add 3,040 matching C++ bytes, reaching
547,604 / 1,563,700 bytes (35.02%). The checked party layouts now expose the
linked member at entity offset 0x570 and its delayed start at record offset
0x18. Five functions start Baby Spin, hold both babies at their shared target
height, begin gliding, join them on landing, and cancel into a fall. The
alternate state-68 entry and state-62/74 restoration retain neutral names.

The caller's collision conditions and live character coordinates determine a
normal-input route from story checkpoint 83: stack the adults with B, start
their spin with B again, then move left into the stationary babies. The
observed babies rise to height 94 and hover. Right starts the glide; X instead
cancels it. Two complete replays of 551 and 495 frames verify all five observed
functions. Independent models check 36 complete party/entity/record/renderer
memory checkpoints and 27 helper-call argument sets. Both first-member and
linked-member target-height branches are observed, including the four-frame
delayed start. The glide and cancellation finish with both babies grounded
in ordinary movement mode. The hovering screenshot was inspected.

Private evidence is `baby_spin_glide83.json` and `baby_spin_cancel83.json` in
`build/runtime/eur_party_state7/`. The input state has SHA-1
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`; the supplied battery save retains
SHA-1 `2cb577d3008975c390a2f00e2b2cd646e4005c1b`. No script or RAM substitution
is used, and no callbacks remain pending. The alternate entry and restore
functions are statically verified only. Native linking, all 74 tests, progress
consistency and the public-content audit pass. The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

### Field contact selection and roaming/path control (2026-09-09)

Eleven functions add 2,100 matching C/C++ bytes, reaching
549,704 / 1,563,700 bytes (35.15%). Two adjacent subsystem modules reconstruct
body-contact selection, collision-flag groups, resource/default body bounds,
roaming wait/update/pause/resume/stop, and waypoint start/advance/clear. Checked
layouts expose the shared path/roaming payload, waypoint counters and four
roaming options. The existing script interaction now uses the typed active
flag. The path loader remains assembly: its private candidate still differs
in register allocation and is not credited.

Story save 1 provides an ordinary, reproducible NPC interaction. Live RAM
identifies the roaming Toad and its coordinates; keypad movement approaches
it, A opens its conversation, and further A presses close it. A cold boot and
three continuation runs cover 2,812 frames. They check 8,893 whole 1,300-byte
entity snapshots across 8,883 roaming updates and one pause, plus nine calls
to the existing random-step helper. Observed paths include movement in
progress, starting a wait, countdown, selecting another roaming step, and
remaining paused during the conversation. The dialog and final field screen
were inspected. The Toad resumes roaming after the dialog through existing
script code; this does not establish coverage of the new Resume function.

A separate 479-frame replay of the full approach/conversation verifies 1,572
contact selections (1,179 accepted and 393 retained), plus 479 collision-flag
updates, each against the complete entity buffer. A cold boot of checkpoint
65 additionally verifies one collision-flag update. The bounds loader, masked
flag setter, waypoint functions, Resume and Stop remain statically verified
only. No RAM or script substitution is used. Supplied saves 1 and 65 retain
SHA-1 `3fcc1ab79db3f829b7a45f0f234cffb7f608ae93` and
`0844b75810855bc3a738122b29382ed5a6c9f983`, respectively; no callbacks remain
pending. Private reports are the `roaming_load1*` JSON files under
`build/runtime/eur_roaming_paths/`, excluding the two-frame position survey,
and `body_contact_load1.json`/`body_load65.json` under
`build/runtime/eur_body_bounds/`. All 74 tests, native linking, progress
consistency and the public-content audit pass. The complete ROM retains
SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

### Shop selection cursors and scroll-arrow models (2026-09-09)

Eight functions add 1,916 matching C++ bytes, reaching
551,620 / 1,563,700 bytes (35.28%). The ordinary panel and curved list each have
readable construction, cursor rendering, arrow rendering and cleanup methods.
They share the checked task/model layouts. Cursor coordinates follow the
current selection; arrow animation and offscreen hiding depend on the window
and selection edges. Signed-byte row offsets and the original fixed-point
rounding are preserved.

The established controlled shop-entry route is replayed from checkpoint 86
for both shop 1 (selling) and shop 0 (buying). Every temporary decoded command
byte is restored at the field helper before the shop transition. Across 941
and 1,231 frames, all eight new functions are reached. Independent checks cover
15,568 complete task/model memory checkpoints, 13,430 helper-call checkpoints,
nine created models, 2,133 model submissions and four cleanup group arguments.
This includes 1,176 ordinary-panel and 957 curved-list draw returns, plus 162
hidden arrows. Animation choices 1, 2 and 9 are observed; the last-edge
animation 10 and the non-scrolling/empty-list alternatives remain static-only.
Allocator-owned list links and animation-helper effects are recaptured after
those existing helpers; each reconstructed write and every animation and
submission argument is checked before the next helper or return.

The previous inventory, row, string, VRAM and complete scene cleanup oracles
also pass. No transaction occurs and the supplied battery save is unchanged.
The selling screen was inspected. Private reports are `evidence_086_01.json`
and `evidence_086_00.json` in `build/runtime/eur_shop_navigation/`. Native
linking, all 74 tests, progress consistency and the public-content audit pass.
The complete ROM retains SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

### Scene movement channels and accelerated paths (2026-09-09)

This batch adds 3,772 matching bytes in 16 functions, reaching
555,392 / 1,563,700 mapped ARM9 bytes (**35.52% matching C/C++**).
The Scene VM gains nine contiguous tracking and accelerated-motion functions
at `0x02085CB4..0x02086560`. Five scheduler functions at
`0x02086D60..0x02087240` extend the existing point-motion module. The identical
304-byte target-tracking pair at `0x020A3B2C..0x020A3C5C` is merged into the
battle accelerated-motion module.

The shared Scene object/channel header now names the linked list, pause and
Q8 time-step fields, deferred deltas and per-channel frame deltas. Existing
point-motion code uses the same checked 228-byte object and 40-byte channel
layouts. Callers and VM evidence maps use the named declarations. The eight
bytes at `0x02087228` are the scheduler stop routine's shared return epilogue,
not a separate function: its predecessor branches there and the reconstructed
428-byte routine reproduces the entire span exactly. Correcting that symbol
boundary does not change the mapped-code denominator.

The normal pause-menu route from story checkpoints 86 and 65 opens Bros.
Items (Start, three Down presses, A), then advances through the animated item
previews. The full nine-item replay for checkpoint 86 lasts 8,730 frames; the
three-item checkpoint 65 replay lasts 2,010 frames. All five new scheduler
functions are observed. Independent models pass 147,485 whole-object and
103,651 list-head checkpoints. The scheduler additionally passes 19,443
callback-argument, 14,523 distance-helper and 3,018 stop-helper checks. Completed
motions, active-channel replacement, deferred motion storage, and removal of
both list heads and interior nodes are observed. Each native callback effect
is recaptured for the scheduler model; all callback kinds reached in these
runs also have separate independent whole-object return models. No RAM or
script substitution is used, and both supplied battery saves remain unchanged.

The battle target-tracking pair is exercised against Elder Shrooboid using
checkpoint 83's previously verified encounter state. Two B presses select and
confirm Luigi's jump. Across 530 frames, the new constructor runs once and
its callback runs eight times, including the terminal frame. The complete
260-byte battle-object models pass 680 object and 570 list-head checkpoints,
including the existing motion helpers. This replay uses no injection; the
state's original encounter-entry probe had restored all 72 substituted decoded
command bytes before entering battle. The Scene overlay's nine new tracking
and acceleration functions remain static-only in this batch. Their original
bytes, helper relocations and complete native relink all match; the runtime
claim is deliberately limited to the observed scheduler and battle paths.

Private evidence: `build/runtime/eur_scene_motion/evidence_previews86.json`,
`evidence_previews65.json`, and
`build/runtime/eur_battle_tracking/evidence_elder_cursor83.json`. The item
preview and battle screens were inspected. Native linking, all 74 tests,
progress consistency and the public-content audit pass. The complete ROM
retains SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.


## Scene sound scheduler and VM state access: 35.68% matching C/C++

Reconstructed 17 functions (2,516 bytes): six Scene sound task functions at
`0x02087240..0x020876E8`, nine save/menu/input accessors at
`0x020876E8..0x02087BA8`, and the adjacent 108-byte path callback/input reset
at `0x02087BE0..0x02087C4C`. The 56-byte input injection function between the
ranges remains an assembly gap. The shared save-party layout now exposes the
confirmed clothing and badge bytes without changing the existing views or size.

The sound task has a checked 72-byte layout with 16 manager slots, a signed
16-bit delay/interval, a 14-bit repeat count, and a two-bit audio handle. A
negative interval measures the first sound's duration using the native audio
handle mask; positive intervals schedule periodic playback. Finite playback
and explicit cancellation clear the owned slot before deleting the task.

Runtime verification used story save 86 and its derived HUD state. Normal
Bros. Item preview navigation observed eight selected-item getter returns with
complete 280-byte menu and 332-byte roster comparisons. A separate 2,350-frame
controlled run exercised all six sound functions: four constructors, ten
one-shot updates, 252 periodic updates, ten measured-playback starts, eighteen
interval measurements, and one explicit stop. It passed 336 full 72-byte task
comparisons, 344 complete 64-byte slot-table comparisons, and all 49 native
helper argument checks. The cases covered delayed playback, finite completion,
active/inactive audio handles, indefinite repetition, and cancellation.

The controlled run inserts extra decoded Scene VM B8/B9 commands using the
known menu sound effect 2. At the native helper, it restores the entire 72-byte
command and pre-decode script cursor, then verifies that the original command
is decoded again unchanged. Interval/count argument overrides are explicit API
fixtures, not natural script coverage. The indefinite task is cancelled using
its returned slot. Other newly linked state accessors and the path callback
have static matching evidence only. Supplied battery saves were unchanged.

Private reports: `build/runtime/eur_scene_sound_state/evidence_previews86.json`
and `evidence_sound_fixtures86.json`. Source save SHA-1:
`349b84f6004bffedfef94de4554f2d1679645e83`; derived state SHA-1:
`23b87fbd8bca1084bc3016e55b3fb633f7856fa8`.

Validation: zero-difference native relink and canonical rebuilt ROM SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`; all 74 tests, source audit,
and whitespace checks passed. Matching C/C++: **557,908 / 1,563,700 bytes
(35.68%)**; C/C++ plus assembly: **36.01%**.


## Pause-menu scrolling and blending: 35.77% matching C/C++

Reconstructed six functions (1,424 bytes): scroll creation, integration and
background application at `0x0207F5F8..0x0207F868`; blend creation/update and zoom
integration at `0x0207FB40..0x0207FE60`. Both 72-byte task layouts and the 44-byte
parent prefix have size checks. Scroll coordinates use four fractional bits,
zoom scale uses eight, and the blend progresses to 16/16 alpha before switching
BG3 character data and rewriting its palette bits. The intervening 728-byte
zoom constructor remains an assembly gap because of register-allocation
mismatches. Background setup/reset candidates also remain private.

`Overlay5DisplayBg_SetOffset` now takes full-width integer coordinates: native
callers pass negated signed halfwords without a second narrowing conversion,
and the callee applies the hardware's nine-bit coordinate mask. Its own code
and all previously linked callers remain byte-identical after the declaration
and definition correction.

Read-only DeSmuME runs replayed normal Items/Clothing/Status navigation from
story save 86, then entered and left the clothing list on saves 86 and 65. All
six new functions were reached. Across 3,240 frames, independent models
passed 6,328 complete task comparisons,
5,974 full scroll-context comparisons,
20 parent-prefix comparisons,
30 full 1,536-byte live BG3 tilemap comparisons,
and 90 blend-alpha register checks.
The checks included every native helper argument, all nine affine-transform
parameters, signed division results, and 1,966
BG1 write addresses/values at the native store instruction. BG offsets are
write-only registers, so their writes are inspected directly rather than
using emulator register readback. Intermediate movement, terminal snapping,
parent completion, both blend directions, and task deletion were observed.
The original scroll-application behavior for target Y = 0 is preserved; its
self-deletion branch was not reached by these menu paths.

Private reports: `build/runtime/eur_scene_menu_motion/evidence_menu86.json`,
`evidence_clothing86.json`, and `evidence_clothing65.json`. No RAM/script
injection, equipment changes, purchases, or writes to supplied battery saves.
Save 86/state SHA-1: `349b84f6004bffedfef94de4554f2d1679645e83` /
`23b87fbd8bca1084bc3016e55b3fb633f7856fa8`; save 65/state SHA-1:
`0844b75810855bc3a738122b29382ed5a6c9f983` /
`ece238ed785fda646a77cde5e886aa7da3e009d7`.

Validation: all 74 tests, public-content audit, whitespace checks, and native
zero-difference relink passed. ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**559,332 / 1,563,700 bytes (35.77%)**; C/C++ plus assembly: **36.11%**.

## Field-party special actions: 35.91% matching C/C++

Reconstructed five contiguous functions (2,204 bytes) at
`0x0208A540..0x0208ADDC`: states 76/78 with an auxiliary renderer,
mode-5 restoration, baby-party downward transfer and per-member landing.
The existing checked party, entity, resource and renderer layouts are reused.
Room VM opcode C2 identifies mode 5 with the drill/underground gimmick;
the two intermediate states retain numeric names pending fuller observation.

An 801-frame DeSmuME run from story save 51 reached restoration, downward
transfer and both member landings. Independent models checked all direct writes
against 16 complete 8,356-byte party snapshots, 64 complete 1,440-byte entity
snapshots and 64 complete 312-byte renderer snapshots, plus 12 native helper
argument lists. Resource slot 0 was resolved from live room 291 data. The
controlled C2/C7 fixtures restored each full 72-byte decoded command and the
pre-decode script cursor at the native wrapper; subsequent execution of the
original command was verified. These fixtures are API coverage, not natural
story-script coverage. States 76/78 have static matching evidence only.

Private report: `build/runtime/eur_party_special/evidence_special51.json`.
Source save SHA-1: `459c9b510b361c4e800a396d8b56ddf847235c74`;
derived state SHA-1: `3d6b30e0af0e2bc4a5f80d821e3408fc9e7c431f`.
The supplied battery save was unchanged.

Validation: all 74 tests, public-content audit, whitespace checks and native
zero-difference relink passed. ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**561,536 / 1,563,700 bytes (35.91%)**; C/C++ plus assembly: **36.25%**.

## Menu and shop particle updates: 35.95% matching C/C++

Reconstructed the two particle update functions at `0x0207E4D0..0x0207E5E8`
(overlay 7, 280 bytes) and `0x0207689C..0x020769BC` (overlay 9, 288 bytes).
A shared checked 72-byte task and 64-byte sprite prefix expose delayed motion,
velocity damping, animation frame selection and draw-list selection. The two
tile-index calculations remain distinct. Constructors stay private because
register allocation still differs.

From story save 86, normal clothing-menu interaction and a purchase in the
clothing shop each produced twelve particles. Across 2,730 frames, all 600
update returns passed: 1,152 complete task and 1,152 complete sprite comparisons,
576 division argument checks, 552 draw argument checks and actual render-list
insertions, and 24 task-deletion argument checks. Delays and all eight image
indices were reached in both overlays. Both runs then backed out of the menus.
The shop was entered in a preparatory run with decoded Field VM command 0x121,
ID 2, restoring all 72 command bytes at the existing scene-transfer helper.
The particle replay itself used only normal keypad input. Purchase/equipment
changes were confined to the derived emulator session; the supplied battery
save was unchanged.

Private reports: `build/runtime/eur_menu_particles/evidence_equip86.json`
and `evidence_clothes86.json`. Source save SHA-1:
`349b84f6004bffedfef94de4554f2d1679645e83`; menu/shop state SHA-1:
`23b87fbd8bca1084bc3016e55b3fb633f7856fa8` /
`1fefcb71d4077210b9fe16378752550145c9dee7`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**562,104 / 1,563,700 bytes (35.95%)**; C/C++ plus assembly: **36.28%**.

## Field auxiliary entity updates and lifecycle: 36.11% matching C/C++

Reconstructed fourteen functions in `0x020B6744..0x020B6E98` and
`0x020B72E0..0x020B756C` (2,528 bytes). A checked 1,360-byte auxiliary entity
layout exposes owner/target links, local and previous offsets, animation and
render-priority settings. Spatial wrappers preserve all forwarded arguments;
the native calls established two arguments omitted by the initial decompiler
output. The intervening motion function remains private pending a register
allocation match.

Three normal-input DeSmuME replays from story saves 51 and 83 covered 1,139
frames: field updates, a room transition, and baby spin animation. Ten of the
fourteen functions ran. Independent models passed 113,453 full auxiliary
object comparisons, 4,754 complete 312-byte renderer comparisons and 1,806
8-byte resource record comparisons, with direct writes checked before helper
calls and on return. Active animation states 10/59, 420 renderer updates,
32 renderer allocations, 16 placement initializations and 16 deletions were
observed. Helper arguments and 32 pointer returns were checked. Navigation,
category mapping, standalone initialization/destruction, active contact and
collision branches, and target-relative render priority retain static matching
evidence only.

Private reports: `build/runtime/eur_field_auxiliary/evidence_room51.json`,
`evidence_spin83.json`, and `evidence_rooms51.json`. Source save SHA-1 values:
`459c9b510b361c4e800a396d8b56ddf847235c74` (51) and
`2cb577d3008975c390a2f00e2b2cd646e4005c1b` (83). Derived-state hashes and
the exact keypad sequences are recorded in each report. Supplied saves were
unchanged; these replays did not inject VM commands.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**564,632 / 1,563,700 bytes (36.11%)**; C/C++ plus assembly: **36.44%**.

## Party render propagation and bounds queries: 36.29% matching C/C++

Reconstructed seventeen functions (2,768 bytes) in five contiguous regions:
`0x020B780C..0x020B7B68`, `0x020B7EF8..0x020B80A4`,
`0x020B85F8..0x020B86EC`, `0x020B8760..0x020B8A90`, and
`0x020B8CA8..0x020B8E4C`. The first region joins the existing party-copy
module, now compiled as C++. The intervening unmatched routines remain raw.

The checked party layout now exposes six auxiliary slots and a state-record
resource view. New code forwards render snapshots, screen coordinates, sort
anchors and priorities, binds stored resources, handles blink callbacks, copies
partner bounds and configures collision channels. Two type-3 volume queries
retain neutral attribute-bit names. A paired-bounds query selects the first
record matching height, facing direction and the appropriate body edge, while
preserving the previous selected index. The underlying renderer helper shows
that its byte argument is a default overlap priority; affected parameter and
virtual-method names were corrected accordingly.

Three normal-input replays from story saves 51/83 covered 1,239 frames and seven
new functions. A room transition restored four render snapshots; movement
exercised screen positions, sort anchors and auxiliary priorities. Thirty
paired-bounds queries included 29 misses and one successful direction-0 match.
The type-3 query examined 26,931 navigation records over 441 calls, all returning
clear; its blocking branch was not reached. Independent models passed 94,034
complete party-member, 263,444 auxiliary and 357,478 renderer comparisons,
plus 27,372 navigation-surface and 135 paired-record comparisons. All 77,409
helper argument checks and 471 query returns passed. Native helper outputs were
recaptured between direct-write checks. Unreached routines and branches retain
static matching evidence only.

Private reports: `build/runtime/eur_party_entity_helpers/evidence_spin83.json`,
`evidence_rooms51.json`, and `evidence_bounds51.json`. Derived-state SHA-1:
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`,
`a450b563a2ba4c96e2b21cbe9ecb96560f865572`, and
`4648a623a447e9a682f4173e9a6d97d438baa0bc`. Source save hashes remain
`2cb577d3008975c390a2f00e2b2cd646e4005c1b` (83) and
`459c9b510b361c4e800a396d8b56ddf847235c74` (51).
No VM commands were injected and supplied saves were unchanged.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**567,400 / 1,563,700 bytes (36.29%)**; C/C++ plus assembly: **36.62%**.

## Shared field renderer updates: 36.38% matching C/C++

Reconstructed four contiguous functions at `0x020A9778..0x020A9D1C`
(1,444 bytes): screen projection and sort keys, shadow visibility/animation,
and locomotion-dependent animation updates. Named views expose the four screen
coordinate fields and the renderer's six-bit entity index / 22-bit vertical
order. Capturing the renderer and Y coordinates before writing X preserves the
original aliasing behavior. Animation speed remains an integer at the renderer
helper boundary, avoiding an incorrect extra narrowing conversion.

Two normal-input replays from saves 83 and 51 covered 775 frames and all four
functions, with 45,956 returns. Independent models passed 74,476 complete
1,300-byte entity comparisons, 58,798 complete 312-byte renderer comparisons,
23,376 complete 316-byte animation-renderer comparisons, 28,520 helper argument
checks and all 6,762 shadow predicate results. Observations included walking
animation transitions, both shadow visibility outcomes, shadow styles 1/3,
all three style-1 height bands, and direct screen-coordinate/sort-key updates.
Support-entity sort inheritance and speed normalization through square root /
division were not reached. Unobserved modes retain static matching evidence.

Private reports: `build/runtime/eur_field_renderer_common/evidence_spin83.json`
and `evidence_walking51.json`. Derived-state SHA-1:
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61` and
`4648a623a447e9a682f4173e9a6d97d438baa0bc`; source save SHA-1:
`2cb577d3008975c390a2f00e2b2cd646e4005c1b` (83) and
`459c9b510b361c4e800a396d8b56ddf847235c74` (51).
No commands were injected and supplied saves remained unchanged. The following
overlap-priority routine stays private because its initial C draft does not
yet match.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**568,844 / 1,563,700 bytes (36.38%)**; C/C++ plus assembly: **36.71%**.

## Battle window resources and transfers: 36.48% matching C/C++

Reconstructed 16 functions (1,584 bytes) in three contiguous regions:
`0x02070780..0x020708BC`, `0x020709B0..0x02070CDC`, and
`0x020713B4..0x0207157C`. They cover the four-slot window pool, texture and
sprite palettes, manager construction/update/destruction, animator setup,
screen-dependent drawing, reverse OAM copying, and queued texture/text/tilemap
transfers. Checked layouts describe the 5,608-byte manager, 132-byte animator,
276-byte per-window state and 16-byte transfer request. The sprite file header
now exposes its three ten-bit allocation counts for the OBJ boundary modes.

The ITCM entry at `0x01FF86D0` was previously hidden inside a wrongly identified
function beginning eight bytes earlier. Those eight bytes are two instruction
templates read by the rectangle-fill routine when it builds its store-multiple
sequence. They are now data, and the real function entry and overlay relocation
are explicit. The implementation remains original assembly; this correction
does not add matching C bytes or change the mapped-code denominator.

Three runtime reports cover 2,783 frames and 6,656 returns across nine of the
16 functions. Full-memory models passed 13,237 complete manager comparisons,
13,668 animator comparisons, 4,930 transfer-request comparisons, 1,856 window
state comparisons and 3,080 animation-entry comparisons. Independent transfer
models verified **1,102,336 copied texture bytes** and **1,402,720 text-tile
bytes** in RAM/VRAM; explicit clear checks covered another 1,355,688 bytes.
Both constructors returned their original pointers, the two texture palettes
and sprite palette had the expected resource/size fields, and both pool
allocations produced four slots. Observed OBJ mode 2 selected 64 tiles.

The entry replays use decoded battle-command fixtures from room 306
(`0x2926`, encounter -32748) and room 583 (`0x0536`, encounter 8232).
All 72 decoded-command bytes and the script cursor are restored and checked
at the native battle wrapper. The room-583 fixture supplies the dialogue
sequence; it is not a claim of natural story progression from checkpoint 55.
Subsequent dialogue advancement uses ordinary A-button input. Destruction,
the sub-screen paths, reverse OAM copying and texture filling were not reached
and retain static matching evidence only. The text-clear and full 3D drawing
implementations remain assembly gaps.

Private reports: `build/runtime/eur_battle_windows/evidence_entry55.json`,
`evidence_dialogue55.json`, and `evidence_dialogue_entry55.json`.
Input-state SHA-1: `e44df106d65e52df1ffc2b125538354f67cb1b22` for both entries,
`a8b485d4cc4b72586af611455830b5f89f67bc07` for the dialogue replay.
Supplied save 55 remains unchanged:
`239ff9d26a5806eada7c1b73a95e27e38872681c`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**570,428 / 1,563,700 bytes (36.48%)**; C/C++ plus assembly: **36.81%**.


## Battle animation and feedback: 36.59% matching C/C++

Reconstructed 13 functions (1,692 bytes) at `0x0207157C..0x02071938` and
`0x020719A4..0x02071C84`: animation durations and modes, restoration of party
idle animations, formation-dependent sounds, actor flags, nonfatal damage
feedback, rating-effect selection and placement, and shared resource loading.
Named declarations replace the old address labels in the attack overlays.
The effect structure now exposes its completion bit, and a checked 16-byte
owner prefix describes the common effect pointer without imposing a complete
layout on the different attack work objects. The original inclusive immunity
range, narrowing conversions and upper-only screen clamps are preserved.

Three headless replays from the derived checkpoint-55 battle state covered
1,735 frames and 125 returns across eleven of the thirteen functions.
Independent models checked complete scene objects (137 comparisons), models
(60), party actors (29), enemy actors (8), effect/owner structures and graphics
animation/frame ranges, together with helper arguments and direct writes.
Ten animation-duration calculations and three quarter-HP predicates passed.
Observed animation durations span 3, 4, 5 and 17 frames; both animation modes,
normal idle restoration, resource readiness and effect completion were reached.

The jump controller uses ordinary keypad input selected from the live input
window and participant phase, with the loaded overlay identity checked first.
Successful normal jumps select variant 1, which intentionally has no popup.
A separate controlled replay substituted the valid variants 2 and 0 at the
native feedback entry to exercise effect allocation, position projection,
screen clamping, owner attachment and the effect's resource field. Each original
two-byte selector was restored and verified at return. These substitutions are
controlled branch coverage, not naturally earned ratings. Nonfatal damage and
the immunity predicate were not reached and retain static matching evidence.

Private reports: `build/runtime/eur_battle_animation_helpers/evidence_jump55.json`,
`evidence_timed_jump55.json`, and `evidence_feedback55.json`.
Input-state SHA-1: `496c6a6836f08c15e95655bb5d78c4cde65dc688`.
Supplied save 55 remained unchanged:
`239ff9d26a5806eada7c1b73a95e27e38872681c`.
The two neighboring functions with unresolved code-generation differences
remain private; they are not counted as matching C.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**572,120 / 1,563,700 bytes (36.59%)**; C/C++ plus assembly: **36.92%**.


## Attack overlay loading: 36.65% matching C/C++

Reconstructed the four contiguous functions at `0x0206F56C..0x0206F948`
(988 bytes): command-specific overlay selection, item-use dispatch, asynchronous
load requests and callback handoff. The party actor layout now exposes its
callback at `+0x78` and command ID at `+0x7C`. A checked prefix describes the
shared allocation through its 16-byte overlay-load state.

The original small overlay IDs are linker-resolved values. DSD already emits
`OVERLAY_n_ID` absolute symbols; declaring and referencing those symbols
reproduces the original literal loads without compiler workarounds. This also
allowed the existing 176-byte `BattleActionScript_RequestLoad` function to lose
its inline-assembly block while remaining exact. That function was already
counted as C, so this cleanup adds no percentage credit. Its special encounter
path deliberately queues the active-overlay slot, whereas the general attack
loader queues the requested-overlay slot.

Four headless replays from checkpoint-55 battle states covered 2,950 frames,
all four new functions and 28 returns including the existing script requester.
Models passed 44 complete 132-byte party-prefix comparisons, four 672-byte
action-state comparisons and 48 complete 16-byte loader-state comparisons.
All helper arguments, direct state writes, command/callback mappings and four
callback handoffs matched. Jump and baby-hammer attacks loaded overlays 20
and 21; using a mushroom loaded overlay 26. Repeated use exercised unloading
an active overlay and reusing the common overlay. All six loader phases were
observed, including both first-load and already-requested common-code paths.
Input selection followed the displayed A/B/X prompts; item use was tested after
normal enemy damage made Luigi a valid healing target.

The replays used ordinary keypad input after loading their existing derived
states; no RAM fields were substituted in this batch. Other Bros-item mappings
and the special-encounter overlay-25 branch retain static matching evidence.
Private reports under `build/runtime/eur_battle_attack_loader/`:
`evidence_jump55.json`, `evidence_baby_hammer55.json`, `evidence_luigi55.json`,
and `evidence_mushroom55.json`. Input-state SHA-1 values respectively:
`496c6a6836f08c15e95655bb5d78c4cde65dc688`,
`f38289316b42e48e93903aa758219b0b34b34a1e`,
`55f6bef8d462a1fa15ee267bbb3a7963d54106da` (last two).
Supplied save 55 remained unchanged:
`239ff9d26a5806eada7c1b73a95e27e38872681c`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**573,108 / 1,563,700 bytes (36.65%)**; C/C++ plus assembly: **36.99%**.

## Battle frame orchestration (2026-09-09)

Reconstructed `BattleMain_Update` (2,464 bytes) in readable C++, with a checked
context view for its task queues, UI storage, cameras, animation state and palette
buffers. The function preserves the original helper order, signed scroll steps,
one-frame camera offsets, animation matrix setup, and HP/equipment persistence
when leaving battle. Shared pointers are re-read at the native call boundaries.

Two ordinary keypad replays from existing derived save-55 battle states exercised
jump attacks and mushroom use: **1,572 frames and 1,572 verified returns**.
An independent caller-write model compared the context header and rendering
control block, neighboring command state, complete 260-byte camera objects,
64-byte matrix, 40-byte sprite animation and 100-byte matrix animation at helper
boundaries and returns. All helper ordering and arguments matched, including
3,144 camera projections, 12,576 signed scroll accumulations, and 100,608 bytes
cleared in matrix parameter buffers. The GPU FIFO writes, palette-command and
image-effect branches, and battle-exit persistence retain static byte-matching
evidence; these replays did not establish runtime coverage of those branches.

Private evidence: `build/runtime/eur_battle_frame/evidence_jump55.json` and
`evidence_mushroom55.json`. Input-state SHA-1 values:
`496c6a6836f08c15e95655bb5d78c4cde65dc688` and
`55f6bef8d462a1fa15ee267bbb3a7963d54106da`.
No RAM fixtures were applied. Supplied save 55 remained unchanged:
`239ff9d26a5806eada7c1b73a95e27e38872681c`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**575,572 / 1,563,700 bytes (36.81%)**; C/C++ plus assembly: **37.14%**.

## Battle entry resources and task sequence (2026-09-09)

Reconstructed five contiguous functions, **4,300 bytes**, covering battle heap
and animation-storage initialization, archive requests and completion, formation
and approach-dependent resource selection, and the final scene setup. Heap names
follow the native allocation labels. A checked encounter view preserves the signed
approach nibble and initiating party side. The two explicit empty `case -2`
branches reproduce the original resource-selection jump tables.

Scene setup preserves repeated resource bindings and position adjustments, UI
coordinate modes, saved actor positions, the party animation-controller copy,
and the handoff to `BattleMain_Update`. All five functions match their original
machine code exactly.

Two controlled encounters from the existing save-55 field state exercised the
whole sequence: **1,412 frames, 100 verified function returns**, including pending
resource and scene readiness paths. Independent models checked every helper call,
all six heap assignments, task and frame callbacks, resource selection, complete
132-byte party actors, 260-byte scene objects, 440-byte models and 48-byte resource
records. The two 420-byte controller copies matched their live source data exactly.
Formation 2 with approach 0 was observed; other formation/approach branches and
allocation-failure paths retain static matching evidence.

The encounter fixture used native Scene VM opcode `0x11C` with encounter `-32748`
from room 306 offset `0x2926`, or `8232` from room 583 offset `0x0536`. At native
battle entry, all 72 substituted decoded-command bytes and the script cursor were
restored and verified. These are controlled entry tests, not naturally played
story transitions. Private reports:
`build/runtime/eur_battle_entry/evidence_entry55.json` and `evidence_dialogue55.json`.
Input-state SHA-1: `e44df106d65e52df1ffc2b125538354f67cb1b22`.
Supplied save 55 remained unchanged:
`239ff9d26a5806eada7c1b73a95e27e38872681c`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**579,872 / 1,563,700 bytes (37.08%)**; C/C++ plus assembly: **37.42%**.

## Battle display entry and capture pipeline (2026-09-09)

Reconstructed fourteen functions, **4,100 bytes**, covering battle-context
initialization, clock-based random seeding, field fading, display capture,
transition sound selection, display/VRAM bank setup, the initial 3D command
sequence, and palette-animation dispatch. Typed frame fields expose the native
brightness levels, RTC time, texture/palette banks and VBlank callback.

Direct instruction inspection recovered omitted blend and clear-color parameters,
the repeated GPU vertex writes, and copy-direction comparisons retained even for
fixed hardware addresses. The native signed-to-unsigned approach narrowing and
materialized display-register snapshot are preserved. Two remaining capture/OAM
loops retain their original assembly while their register-allocation differences
are deferred.

Two emulator replays covered **1,412 frames and 66 verified returns across 13 of
the 14 functions**. Independent models checked all helper arguments, caller RAM
writes and **136 direct MMIO/FIFO stores**, including their addresses, widths and
values. They compared **313,344 copied bytes** against live source data: main/sub
BG and OBJ VRAM, standard palette/OAM, and the relocated background and palette
banks. They also verified 115,368 cleared context bytes and 2,048 cleared OAM bytes.
Palette-animation dispatch retains static byte-matching evidence; these replays
did not invoke it. Transition sound 4 and both display-selection paths were observed.

Both runs used the documented Scene VM encounter fixture (`-32748`, room 306,
offset `0x2926`), restoring the 72 decoded bytes and cursor at battle entry.
The second run additionally set only bit 3 of the live encounter flags to exercise
the sub-display transfer. That bit was restored after `BattleEntry_ShowBattleDisplay`,
preserving other flags changed by normal game execution. This is controlled branch
coverage, not a naturally played entry from the lower screen. The harness accounts
for predicated ARM instructions skipped by the emulator's execution hooks.

Private reports: `build/runtime/eur_battle_entry_display/evidence_entry55.json`
and `evidence_sub_display55.json`. Input-state SHA-1:
`e44df106d65e52df1ffc2b125538354f67cb1b22`.
Supplied save 55 remained unchanged:
`239ff9d26a5806eada7c1b73a95e27e38872681c`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**583,972 / 1,563,700 bytes (37.35%)**; C/C++ plus assembly: **37.68%**.

## Battle lifecycle and VBlank uploads (2026-09-09)

Reconstructed five contiguous functions, **2,512 bytes**, covering battle task
creation, the recurring texture-upload callback, VBlank transfers and destruction.
The checked context view now includes its seventy scene objects, seventy-two
resource models, signed parallax scales and separate final sub-palette source.
Native array access patterns reproduced the original induction variables directly;
nullable inline cleanup retains the original virtual deletion guards.

Four emulator replays covered **1,389 frames and 1,792 verified returns across all
five functions**: 902 texture-upload callbacks, 886 VBlank calls, two destructors,
one constructor and one allocation entry. Independent models checked helper targets
and arguments, caller RAM changes, all **5,336 direct MMIO stores**, and **1,814,528
bytes each of OAM upload and clearing**. The VBlank runs covered both settings of
the sub-upload gate and changing brightness during entry. Destruction verified
26 virtual model deletions, four animation finalizations/deletions, twelve heap
destructions, and both native session-state requests (0 and 7).

A one-VBlank fixture exercised negative and fractional parallax, odd brightness
levels, all three pending palette sources (**1,024 bytes**) and captured-image
conversion. A deterministic **24,576-byte** pattern in live LCDC VRAM was compared
with the complete independently rearranged destination at `0x0621A000`. After the
checks, all 50,254 bytes of modified input fields, image source/destination,
palettes and scroll/brightness registers were restored and verified. The battle
command wheel remained visible after this test. Hardware restoration uses halfword
writes, preserving the DS palette/VRAM access semantics.

Entry used the documented restored Scene VM encounter fixture (`-32748`). Exit
tests temporarily set the native frame-end turn-state condition to zero and
restored it before entering destruction. A second exit test additionally selected
exit mode 1; native cleanup cleared the ten-byte encounter request and reached the
retry/load menu through state 7. The state-0 test verified all cleanup operations
but remained black after a further 601 frames, so it does **not** establish a
complete natural return to the field. No naturally won/lost battle is claimed.
Allocation failure, null-context VBlank and defeat-effect kind 4 retain static
matching evidence only.

Private reports: `build/runtime/eur_battle_lifecycle/evidence_entry55.json`,
`evidence_exit55.json`, `evidence_exit_mode7_55.json`, `evidence_graphics55.json`.
Entry input-state SHA-1: `e44df106d65e52df1ffc2b125538354f67cb1b22`;
battle input-state SHA-1: `496c6a6836f08c15e95655bb5d78c4cde65dc688`.
Supplied save 55 remained unchanged:
`239ff9d26a5806eada7c1b73a95e27e38872681c`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**586,484 / 1,563,700 bytes (37.51%)**; C/C++ plus assembly: **37.84%**.

## Battle object and party initialization (2026-09-09)

Reconstructed `BattleEntry_InitializeObjects`, **1,324 bytes**, extending the
existing contiguous entry-task module. The frame view now describes the 56-entry
scene table, four 148-byte party slots, eight enemy slots and UI scene aliases.
The party actor's former tail padding holds its initial formation value. Typed
subarrays reproduce the original separate scene, actor and save-record strides;
the linked partner calculation preserves its explicit parity XOR.

Two 706-frame replays, using derived states from supplied saves **51 and 55**,
verified both initializer returns against independent live-RAM models. Together
they checked 140 complete scene objects, eight party slots, sixteen enemy slots,
160 resource IDs, all UI bindings and both item-list rebuild calls per entry.
The complete known initialization prefix was compared except for the twelve RTC
bytes written by the asynchronous clock response. Both runs used formation 2;
the other formation branch retains static matching evidence.

Party values differed between the saves and matched their live sources: adult
Mario initialized with HP 62 and level 17 in save 51, versus HP 64 and level 18
in save 55; the baby slots likewise matched their different levels and stats.
Both used the documented Scene VM encounter fixture (`-32748`, room 306 offset
`0x2926`), restoring the decoded command and cursor at native battle entry. These
are controlled encounter tests, not naturally triggered story battles.

Private reports: `build/runtime/eur_battle_entry_objects/evidence_entry51.json`
and `evidence_entry55.json`. Input-state SHA-1 values:
`3d6b30e0af0e2bc4a5f80d821e3408fc9e7c431f` and
`e44df106d65e52df1ffc2b125538354f67cb1b22`.
All **104 supplied `.sav` files** were hashed before and after both runs and
remained unchanged. Save 51 SHA-1: `459c9b510b361c4e800a396d8b56ddf847235c74`;
save 55 SHA-1: `239ff9d26a5806eada7c1b73a95e27e38872681c`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**587,808 / 1,563,700 bytes (37.59%)**; C/C++ plus assembly: **37.93%**.

## Callback models and sub-window uploads (2026-09-09)

Reconstructed fourteen functions in two contiguous modules, **820 bytes**:
callback-model initialization, dispatch, palette binding and cleanup, plus
sub-window upload scheduling, tilemap transfer, link flags and readiness.
The callback subclass has a checked 320-byte view with its four-field extension
at offset 304. The upload scheduler preserves the native packed count test,
including its high main-count bit.

Three replays totaled **1,088 frames and 2,942 verified returns across eleven
functions**. Independent RAM models checked callback arguments, the sixteen-byte
constructor clear, palette binding, both link-flag values and native deletion
of the owned buffer and model. Prepare callbacks were null; draw and update
callbacks ran 718 times each. The private tracer now distinguishes `BLX ip`
from tail-call `BX ip`, validating the actual return path in both cases.

The dialogue entry used the documented encounter 8232 from room 583, offset
`0x0536`, restoring its decoded command and cursor at native battle entry.
A separate native-exit-condition fixture was restored before destruction;
the battle context was checked as cleared at the destructor return. This is
cleanup evidence, not a naturally won battle.

A controlled one-frame sub-count fixture exercised the otherwise inactive
upload branch with the existing empty sub-window list. Native upload and reset
tasks transferred **4,096 tilemap bytes**, compared against the complete source
in live VRAM. Count, flags, blend state, OAM data, destination VRAM and scroll
registers were restored; the second reset-task copy was also restored after its
return. The non-deleting destructor, readiness query and individual sub-window
upload retain static byte-matching evidence. Non-null prepare callbacks and
null draw/update callbacks were not reached in these replays.

Private reports: `build/runtime/eur_battle_callback_upload/evidence_dialogue55.json`,
`evidence_exit55.json`, `evidence_upload55.json`. Initial dialogue input-state
SHA-1: `e44df106d65e52df1ffc2b125538354f67cb1b22`; derived battle-state SHA-1:
`61c13949eb320122c8d9e313809d7f605efa87ef`. All **104 supplied saves** were hashed
before and after every successful run and remained unchanged.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**588,628 / 1,563,700 bytes (37.64%)**; C/C++ plus assembly: **37.98%**.

## Battle text initialization and resource requests (2026-09-09)

Reconstructed four contiguous functions, **592 bytes**, for the tiled and clipped
battle text variants. Checked 72-byte and 92-byte views describe their text
prefix, dimensions, alignment modes, upload flags, palette and destination;
the clipped variant also stores horizontal clipping limits. Initializers clear
the entire variant and bind its GameText state to the battle font table.
Resource requests preserve the variants' different signedness for alignment
modes and their distinct flag updates.

Two restored encounter replays using save 55 totaled **1,642 frames**. Both
reached the tiled initializer, checking all 72 cleared bytes, subsequent field
assignments, all fourteen GameText initialization arguments and final font/buffer
bindings. The observed dimensions were 32 by 8 tiles. The clipped initializer
and both resource-request wrappers retain static byte-matching evidence; these
particular replays did not reach them. The adjacent tile-copy routine remains
unlinked while its loop code generation differs.

Private reports: `build/runtime/eur_battle_text_controls/evidence_entry55.json`
and `evidence_dialogue55.json`. Both began from input-state SHA-1
`e44df106d65e52df1ffc2b125538354f67cb1b22`. Encounter fixtures were `-32748`
(room 306, offset `0x2926`) and `8232` (room 583, offset `0x0536`), respectively;
both restored the decoded command and cursor at battle entry. All **104 supplied
saves** were hashed before and after both runs and remained unchanged.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**589,220 / 1,563,700 bytes (37.68%)**; C/C++ plus assembly: **38.02%**.

## Save slot serialization and live-state restoration (2026-09-09)

Reconstructed `SaveState_PackSlot` and `SaveState_UnpackSlot`, **1,716 bytes**,
in one contiguous module. Checked views describe the 20-byte stored party
record, 232-byte slot tail and 1,380-byte live-state prefix. Packing retains the
944-byte persistent block, base stats, HP, experience, equipment, inventory and
metadata. Unpacking reconstructs member IDs, remaining experience and equipment
bonuses. Native signed inventory limits and asymmetric speed/stache bounds are
preserved. Distinct traversal variables reproduce the original compiler's
register lifetimes without assembly substitutions.

Normal cold boots and keypad input loaded supplied saves **1, 55 and 103**.
Independent models compared the complete live view and stored slot at each
return, including preservation of untouched bytes. Adult Mario's observed level
was 1, 18 and 31; experience to the next level was 6, 426 and 4,069. All twelve
party records matched their growth tables and equipment data.

The save-55 replay then used the actual save block and Save & Continue dialog.
Selecting Yes reached the native packer and the visible Saved confirmation.
The model checked the complete 1,176-byte slot and 848-byte extra area, including
its **804-byte OAM-state copy**, while confirming that packing left the live view
unchanged. There were no code, script or RAM fixtures. Slot 1, level 100 and
out-of-range inventory clamps retain static matching evidence.

Four verification reports total **6,627 frames and four checked returns**:
`build/runtime/eur_save_state_transfer/evidence_load1.json`, `evidence_load55.json`,
`evidence_load103.json`, `evidence_pack55.json`. Two additional keypad-only stages
navigated from the loaded field to the confirmation prompt. The pack input-state
SHA-1 is `78e17c1fe46026d1115c27b212d6f0e39fa4948a`. Supplied save SHA-1 values:
`3fcc1ab79db3f829b7a45f0f234cffb7f608ae93` (1),
`239ff9d26a5806eada7c1b73a95e27e38872681c` (55),
`c264e8a8b26cb4994da8a93b164979377b7b4090` (103).
All **104 supplied saves** were hashed before and after the runs and remained
unchanged; emulator writes stayed in its working backup data.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**590,936 / 1,563,700 bytes (37.79%)**; C/C++ plus assembly: **38.13%**.

## Save write task and menu messages (2026-09-09)

Reconstructed four contiguous functions, **1,048 bytes**: the save-write request,
its asynchronous state machine, and message show/hide wrappers. A checked
72-byte task view exposes source/destination slots, checksum mode, selection
mode, saved lock state, result and error flags. The writer updates the primary
slot and its backup at destination + 2, then records occupancy and optionally
selects the slot. Primary-write failure and settings failure retain their
distinct result codes; backup failure alone does not block the settings update.
Direct compound bitfield assignments reproduce the native settings operations.
The message wrappers hide BG1/BG2, draw the requested background text, and enable
both planes while updating the menu's visibility flag.

Normal keypad input exercised **save, copy and delete** on emulator working
copies of supplied save 55. These reached all four reconstructed functions,
with **155 checked returns across 1,254 frames**: 3 requests, 146 writer updates,
2 message displays and 4 hides. The three operations observed selection modes
+1, 0 and -1, both destination slots, six successful slot writes and three
settings writes. Independent models compared complete task and parent records,
lock bytes, live-save flag bit 5, occupancy/selection, storage-call arguments and
state transitions driven by the actual storage results. Message checks covered
entries 16 and 20, all text/helper arguments and the BG1/BG2 display bits. Error
paths, a direct state-1 entry and null live-save pointers retain static evidence.

Private reports: `build/runtime/eur_save_menu_write/evidence_pack55.json`,
`evidence_copy55.json` and `evidence_delete_confirm55.json`. Input-state SHA-1:
`78e17c1fe46026d1115c27b212d6f0e39fa4948a` (save),
`b99ae49572f9d3d22933b903c66355958e3709f9` (copy),
`95a6bc8106164f65635d6ee06e747bd7339d3904` (delete). The load-menu operations
were reached through a normal cold boot and recorded keypad navigation; there
were no code or RAM fixtures. All **104 supplied saves** were hashed before and
after every run and remained unchanged. Deletion affected only the imported
emulator working copy. The battle capture-tile converter remains unlinked
while its compiler register allocation differs.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**591,984 / 1,563,700 bytes (37.86%)**; C/C++ plus assembly: **38.19%**.

## Pause-menu healing, glow and star updates (2026-09-09)

Reconstructed five routines, **1,724 bytes**, covering HP-item application, the
combined effect request, glow creation/update and star update. Checked shared
views describe 72-byte tasks, 64-byte sprites and the 10-byte animation sequence.
Named phases preserve immediate transitions between star growth, hold and
shrink; glow completion releases its parent's wait counter. Signed fixed-point
arithmetic, unsigned affine arguments, animation wrapping and sprite attribute
updates match the original. The intervening star-group constructor and preceding
heal request still have compiler-allocation differences; the two C modules keep
their verified contiguous ranges and can merge when that constructor matches.

A normal cold boot of supplied save **20** reached the pause menu and Items.
The save already had injured babies: a Mushroom restored Baby Mario from
**21/23 to 23/23 HP**, and another restored Baby Luigi from **26/28 to 28/28 HP**.
Both updates were independently checked against the live item record and maximum
HP. Screenshots showed the star/glow animation and the final HP/item counts.
No code or RAM fixtures were used.

The two effect replays total **243 frames and 894 checked returns**: 42 healing
updates, 2 effect requests, 2 glow constructors, 38 glow updates and 810 star
updates. Independent models checked the complete task/sprite records before
helper calls and at returns, the party record, trigonometric movement from the
live sine table, all three star phases, four animation frames, delay counters,
fade levels, parent completion and all 34 task releases. All helper targets and
arguments were validated, including the nine-argument star-group requests.
All five reconstructed functions were reached. The replays used the normal
Mushroom effect parameters; other item effects and unusual glow duration inputs
were not exercised.

Private reports: `build/runtime/eur_menu_item_effect/evidence_heal20.json` and
`evidence_heal_luigi20.json`. Input-state SHA-1:
`84c9eda6208235f00d4f3e11eee0fad1c9817cc1` and
`c2cf48910c782602604c48b2acbf11129840d982`, respectively. Supplied save SHA-1:
`e78faca7c148cb07941601554f06f279a29c4488`. Preceding keypad-only stages retained
cold-boot/menu screenshots and derived states. All **104 supplied saves** were
hashed before and after each run and remained unchanged.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**593,708 / 1,563,700 bytes (37.97%)**; C/C++ plus assembly: **38.30%**.

## Equipment-menu number graphics (2026-09-09)

Reconstructed three routines, **804 bytes**, for narrow/wide decimal values,
unavailable-value markers and preparation of the two digit fonts. The number
renderer suppresses leading zeroes while retaining the units digit, selects
transparent blitting by pixel parity and converts each 32-by-16-pixel strip to
OBJ tiles. Confirmed buffer roles now replace the three opaque pointer names
in the shared pause workspace and its resource lifetime code. The two blitters
between the reconstructed ranges retain assembly while their compiler output
differs; both were nevertheless checked through their live callers.

Normal keypad navigation on supplied save **20** opened the pause menu, Gear
and Clothing. Browsing adult and baby clothing exercised both font widths,
equal/decreased stat values and five unavailable-value markers. Independent
pixel models checked all **2,048 prepared glyph bytes**, **43 complete scratch
buffers** after transparent blits, and **4,864 bytes written to OBJ VRAM**.
All helper targets/arguments, division results and glyph-source return pointers
were checked. Screenshots showed the equipment comparisons and the five `---`
markers for Starly Jeans selected for Mario. No code or RAM fixtures were used.
The observed decimal values were two-digit values; zero, one-digit and
three-digit values retain static matching evidence.

The three substantive replays total **630 frames and 20 checked returns**:
one preparation, fourteen value draws and five unavailable draws. Private
reports: `build/runtime/eur_menu_number/evidence_menu20.json`,
`evidence_clothing20.json` and `evidence_unavailable20.json`. Input-state SHA-1:
`d2c8a01fd324265a80ade9948d2d5d397de5a314`,
`6a1ea86baa3653c0c7e6364e1b07f8d2b096244d`, and
`61373ecc7bc36608401e3d46a91fcf5637bf297d`, respectively. The intervening Gear
navigation is retained separately. Save 20 SHA-1:
`e78faca7c148cb07941601554f06f279a29c4488`. All **104 supplied saves** remained
unchanged in before/after hash checks.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**594,512 / 1,563,700 bytes (38.02%)**; C/C++ plus assembly: **38.35%**.

## Equipment comparison displays and panel motion (2026-09-09)

Reconstructed five routines, **1,968 bytes**: heading creation/update, the stat
panel's horizontal motion, comparison-arrow animation and number-strip motion.
Checked 72-byte task views distinguish heading interpolation from the stat
panel's acceleration. Shared pause-workspace fields now describe active state,
five comparison kinds, the three per-update animation-frame caches and heading
position. The common animated-model interface reproduces the native virtual
animation calls. The decimal renderer's value parameter is now `int`, with its
16-bit remainder conversion explicit; caller evidence established this ABI and
both already reconstructed number functions remain byte-identical.

Normal Gear/Clothing navigation on supplied save **20** reached all five new
routines. The main replays total **660 frames and 7,887 checked returns**: one
heading constructor, 462 heading updates, 464 stat-panel updates, 2,320 arrow
updates and 4,640 number-strip updates. Two rightward character changes reached
Luigi and Baby Mario. Closing the submenu exercised all 17 releases: heading,
stat panel, five arrows and ten strips. The model checks covered all six motion
phases, the heading's delay counters, signed fixed-point coordinates, per-frame
cache filling/reuse, unavailable-arrow suppression, parent closing and native
helper targets/arguments. Complete tasks, 64-byte OBJ sprites, 128-byte animated
model prefixes and 28 bytes of shared equipment state were compared at helper
boundaries and returns. Screenshots confirmed the character and submenu changes.

Private reports: `build/runtime/eur_menu_equipment/evidence_clothing20.json` and
`evidence_switch20.json`. Input-state SHA-1:
`6a1ea86baa3653c0c7e6364e1b07f8d2b096244d` and
`34fa1984e24a4bb1d09444d48b18b16f14e6a78e`. Save SHA-1:
`e78faca7c148cb07941601554f06f279a29c4488`. Supplementary clothing/character
browsing is retained in `evidence_unarmed20.json` and
`evidence_baby_comparison20.json`. No code or RAM fixtures were used, and all
**104 supplied saves** retained their before/after hashes. The heading's
single-member animation choice and the higher-stat arrow retain static evidence;
equal, lower and unavailable comparisons were observed. The intervening stat
calculation and digit-sprite renderer remain assembly while compiler differences
are unresolved.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**596,480 / 1,563,700 bytes (38.15%)**; C/C++ plus assembly: **38.48%**.

## Pause-menu label selection and row positioning (2026-09-09)

Reconstructed five routines, **488 bytes**, for main-page and submenu labels,
release of the label task group/allocation, and relative positioning of text
and icon rows. A checked 72-byte row task holds integer-pixel offsets. The
shared pause workspace now names the selected menu and label origin. Text
sprites receive coordinates with twelve fractional bits; animated icons use
integer halfwords. Both join draw list 40. The allocation/create gaps remain
assembly pending compiler differences, so the small source regions can merge
when those gaps are reconstructed.

Normal navigation from supplied save **20** entered the pause menu, Equipment,
its Clothing/Badges choice, returned to the main menu and visited the remaining
available pages. Two replays total **680 frames and 1,029 checked returns**:
seven main-page labels, one submenu label, 204 icon-row updates, 816 text-row
updates and one release. Checks compared complete task/sprite records and the
shared origin, and validated text entry arguments plus task-group/allocation
release targets. Pages 0 through 4, party count 4 and submenu entry 0 were
observed. Page 5, party counts 1/2 and other submenu indices retain static
matching evidence. An additional run from early save 1 did not enter the
pause menu and contributes no target coverage.

Private reports: `build/runtime/eur_menu_label/evidence_menu20.json` and
`evidence_pages20.json`; input-state SHA-1:
`d2c8a01fd324265a80ade9948d2d5d397de5a314` and
`31a76da9975161ba9520a0be73de41a7cf9ecbed`. Save 20 SHA-1:
`e78faca7c148cb07941601554f06f279a29c4488`. Screenshots confirmed the label and
submenu transitions. No code or RAM fixtures were used. All **104 supplied
saves** retained their before/after hashes.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**596,968 / 1,563,700 bytes (38.18%)**; C/C++ plus assembly: **38.51%**.

## Field vector scaling and model-animation placement (2026-09-09)

Reconstructed **600 bytes**: the 3D vector scaler, the model-animation update
wrapper and the callback that applies matrix translation, context offsets and
parent-relative placement. The callback also copies all four parent overlap
priorities or assigns the context's explicit priority. Closing the update gap
allows both animation routines to join the existing contiguous module. The
vector scaler reduces large components before squaring and restores the length
scale before division; its callers now use the shared named declaration.

The spatial-motion fixture exercised thirteen controlled decoded-command cases
and checked eight native scaler returns against independent integer square-root
and rounded signed-division calculations. All three input shifts (0, 4 and 8),
positive and negative components, and default speed were observed. The
nonpositive-scale fallback retains static matching evidence. Three additional
controlled anchored-animation cases used live entity 9 and the native matrix
animation engine. A minimal translation/draw script was placed in unused
context storage; temporary bank pointers and all 72 decoded-command bytes were
restored at starter entry. Complete 312-byte renderer comparisons checked
parent-relative placement, inherited priorities and explicit priorities with
and without a parent. These are RAM fixtures, not ordinary story navigation.

Private reports: `build/runtime/eur_field_scale/evidence.json` and
`build/runtime/eur_field_model_prepare/evidence.json`. Both reload the compatible
spatial-motion checkpoint, SHA-1 `4eeb1538e58d43fe03dd2b994296afc28f4061b4`.
All **104 supplied saves** retained their hashes. The nearby 2D scaler, line
intersection and timer OAM renderer remain assembly pending compiler differences.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**597,568 / 1,563,700 bytes (38.22%)**; C/C++ plus assembly: **38.55%**.

## Field palette-bank crossfade setup and transfer (2026-09-09)

Reconstructed **804 bytes** for starting a palette-bank crossfade and transferring
its endpoint/result palettes. A checked 1,544-byte structure names the control
bits, duration, elapsed time and three 256-color buffers. The field context now
owns a typed pointer, and the VM and field graphics callers use shared names.
The palette address table identifies standard/extended OBJ palettes, standard BG
palettes and four extended BG regions for each screen. Extended-palette transfers
temporarily restore CPU access through LCDC and then restore the bank assignment.

Fourteen controlled Field VM starts cover all seven regions on both screens,
using live mapped palette data and bank 0-to-1 transitions of four ticks. The
84-frame replay checked **14 starts, 70 transfers and 84 copy calls**, totaling
**31,488 copied bytes**, through completion. Complete 1,544-byte records were
compared at returns; an independent RGB555 interpolation model also checked 84
calls to the existing assembly updater. That 432-byte updater remains assembly
while compiler differences are unresolved and is not counted as reconstructed C.

Private report: `build/runtime/eur_field_palette_blend/evidence.json`.
Input-state SHA-1: `4eeb1538e58d43fe03dd2b994296afc28f4061b4`. Each fixture
restores the decoded command at starter entry and temporarily changes only the
copier's screen-selection bit, restoring it at return. These runs establish
transfer behavior, not normal story-event coverage. No ROM or supplied-save
edits were made; all **104 supplied saves** retained their hashes.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**598,372 / 1,563,700 bytes (38.27%)**; C/C++ plus assembly: **38.60%**.

## Pause-menu background loading and bank swaps (2026-09-09)

Reconstructed **3,044 bytes** across thirteen routines: queued and immediate
background bank selection, graphics uploads, page/alternate loader tasks and
extended palette uploads. The checked 72-byte task names buffer ownership,
loader state and parent completion counters. The scene work now names its two
tile buffers, two map buffers, two secondary map buffers and active bank.
The adjacent map-strip preparation routine remains assembly while its compiler
register allocation differs; it is not included in the C total.

Normal keypad navigation from supplied save 20 exercised every new routine,
including Items, equipment, the remaining menu pages, both bank selections,
both alternate loaders and all five page-loader states. Three replays totaled
**2,485 frames and 68 checked returns**. SP-matched, original-byte-guarded
hooks checked complete task records, parent counters and helper arguments.
Independent LZ10 and tilemap models verified decompressed data and palette tags;
VRAM comparisons covered **393,216 tile bytes**, **34,816 map bytes**,
**67,264 directly decompressed tile bytes**, **4,096 direct map bytes**
and **8,192 extended palette bytes**. The still-assembly map-strip
helper was also checked against an independent copy/strip model. Coverage is
normal menu navigation, not a claim to all resource rows or archive-wait branches.

Private reports: `build/runtime/eur_pause_background/evidence_items20.json`,
`evidence_pages20.json` and `evidence_submenu20.json`. The Items replay starts at
menu state SHA-1 `bda7166108c11486ee60cef2753342bbf0a4634f`; the underlying
save-20 SHA-1 is `e78faca7c148cb07941601554f06f279a29c4488`. No code or RAM
fixtures were used and all **104 supplied saves** retained their hashes.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**601,416 / 1,563,700 bytes (38.46%)**; C/C++ plus assembly: **38.80%**.

## Pause-menu icon highlighting and cursor shadow (2026-09-09)

Reconstructed **1,132 bytes** for the selected menu icon and the cursor shadow.
The icon task creates/releases its renderer as the selection changes, stretches
vertically and horizontally, then alternates reverse/forward palette pulses.
The shadow follows the parent cursor and draws on alternating frames. A checked
72-byte task distinguishes cursor motion from icon scale fields, and the scene
work now names the signed highlighted-menu index.

A normal 890-frame replay opened the pause menu from supplied save 20, visited
all five pages, opened a submenu and returned. It checked **3,115 icon returns
and 623 shadow returns**, all seven icon states, both shadow frame parities,
four icon releases and one immediate reactivation. Complete 72-byte tasks and
336-byte sprite prefixes were compared against independent position, scale and
lifetime expectations, including 919 draw calls and 24 virtual palette requests
(14 reverse, 10 forward). No code or RAM fixtures were used; all **104 supplied
saves** retained their hashes. The adjacent cursor constructor and motion
callback remain assembly pending compiler register/scheduling differences.

Private report: `build/runtime/eur_menu_cursor/evidence_menu20.json`.
Input-state SHA-1: `d2c8a01fd324265a80ade9948d2d5d397de5a314`.
Save-20 SHA-1: `e78faca7c148cb07941601554f06f279a29c4488`.
Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**602,548 / 1,563,700 bytes (38.53%)**; C/C++ plus assembly: **38.87%**.

## Field-party trail emitter and active-copy query (2026-09-09)

Reconstructed **852 bytes** for configuring a timed auxiliary-copy emitter,
placing its next copy and querying active copies in the stored trail. Checked
layouts identify the twelve-byte emitter, eight-byte direction records and two
1,540-byte history blocks within the existing party controller. The history
interiors remain opaque while their update/initialization routines remain ASM.
The emitter carries animation offsets, a start/count range, cyclic next slot
and timer/interval; emitted copies inherit position, height, facing and overlap
priorities, with table-driven position and render-order offsets.

Three normal keypad replays on supplied save 83 exercised Bros-Ball rolling and
spin jumping over **836 frames**. They verified **4 emitter starts, 143 updates
and 8 active-copy queries** against independent full-record models (8,356-byte
party, 1,440-byte owner, 1,360-byte auxiliary and 312-byte renderers). The runs
observed all five emission slots, two range wraps, directions 0/2/6 and both
active/inactive query results. Resource 61, five copies and a nine-frame interval
were observed. The vertical-motion suppression branch remains statically
verified rather than claimed as covered by these replays.

Private reports: `build/runtime/eur_party_trail/evidence_roll83.json`,
`evidence_long83.json` and `evidence_spin83.json`. Input-state SHA-1:
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`; save-83 SHA-1:
`2cb577d3008975c390a2f00e2b2cd646e4005c1b`. No code or RAM fixtures were
used and all **104 supplied saves** retained their hashes.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**603,400 / 1,563,700 bytes (38.59%)**; C/C++ plus assembly: **38.92%**.

## Field-party reunion, piggyback mount and transfer completion (2026-09-09)

Reconstructed **1,140 bytes** for restoring the paired member, starting a
piggyback mount and completing an airborne transfer. The mount preserves prior
behavior, coordinates both party controllers and derives the second member's
eight-way facing from its linked member. Completion stops vertical motion,
hides the transferred entity and starts the paired animation.

A controlled 685-frame Field VM replay on supplied save 83 checked both members
in all three routines (**six returns**), including the linked-member angle path.
Original-byte-guarded, SP-matched hooks verified helper arguments and complete
8,356-byte party, 1,440-byte member and 312-byte renderer records. Reunion,
cancellation and mount requests replaced decoded VM commands; all 72 command
bytes and the script cursor were restored at wrapper entry and the original
command replay was checked. The separate C8 completion fixture additionally
prepared the two live members in the caller's documented states 57 and 58.
This is controlled function coverage, not a normal story-transition replay.
No ROM changes were used and all **104 supplied saves** retained their hashes.

Private report: `build/runtime/eur_party_rejoin/evidence_fixture83.json`.
Input-state SHA-1: `3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`.
Save-83 SHA-1: `2cb577d3008975c390a2f00e2b2cd646e4005c1b`.
Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**604,540 / 1,563,700 bytes (38.66%)**; C/C++ plus assembly: **39.00%**.

## Field-party recovery, hit reactions and airborne transfer (2026-09-09)

Reconstructed **3,176 bytes** across seven routines: paired hit reactions,
rebound, movement-mode-8 setup, recovery blink entry/exit, sinusoidal hovering
and airborne-transfer setup. Six routines form one contiguous recovery module;
the transfer starter extends the existing piggyback module. Checked state records
now expose saved speed/movement, hover phase and the paired vertical-bound
snapshot. Unknown recovery subfields retain neutral names.

Two normal keypad replays from supplied save 83 covered the Bros-Ball collision,
paired reaction, subsequent movement setup and recovery of both members. These
**1,285 frames** checked seven returns, including saved behavior, the rebound's
midpoint movement, prior-blink cancellation and both recovery completions.
A separate **578-frame replay** substituted one decoded B1 reunion request,
restored all command bytes and the script cursor, and checked original-command
replay. Subsequent A/X and B/Y keypad sequences produced **four airborne starts**,
two per member. The oracle verified corner arrays, swept bounds, helper arguments
and **76 scanned navigation records**. All observed collision tests allowed the
transfer; the blocked-transfer branch remains statically verified.

A **121-frame controlled RAM fixture** exercised 80 hover updates on the live
second member, covering all 64 sine phases and two wraps. At the native party
dispatcher, it prepared state 92, phase 62, the member-update enable and the
sibling recovery substate. All four prepared fields were restored after 80
frames. This is an explicitly prepared hover test, not normal story coverage.
Every run used original-byte guards, SP-matched returns and independent complete
8,356-byte party, 1,440-byte member and 312-byte renderer comparisons.
All **104 supplied saves** retained their hashes; no ROM code was patched.

Private reports: `build/runtime/eur_party_recovery/evidence_normal83.json`,
`evidence_recovery83.json`, `evidence_airborne83.json` and
`evidence_hover_fixture83.json` (**1,984 frames, 91 checked returns**).
Initial-state SHA-1: `3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`;
save-83 SHA-1: `2cb577d3008975c390a2f00e2b2cd646e4005c1b`.
Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**607,716 / 1,563,700 bytes (38.86%)**; C/C++ plus assembly: **39.20%**.

## Field-party manager reunion and scripted action entry points (2026-09-09)

Reconstructed **2,880 bytes** across thirteen contiguous manager routines.
The reunion implementation binds adult/baby partners, restores presentation,
sets movement mode 6 and hides the carried members. Its starter supports an
immediate placement or movement toward the other group's midpoint. The same
module handles reunion cancellation, piggyback-mount requests/status, airborne
completion and the script-facing drill, anchor, cannon and elevation wrappers.
The Field VM now calls their typed public declarations.

Controlled VM replays on supplied save 83 exercised moving reunion from side 0,
immediate reunion from side 1, rejection after reunion, completion queries,
mounting, cancellation and both airborne completions. Full 16,764-byte manager,
1,440-byte member and 312-byte renderer comparisons checked each helper boundary
and return. The immediate reunion also reached region 71's special indicator
palette path. The observed queries reported completion; pending-query results
and independent-member mounting remain statically verified.

Seven separate 41-frame replays from the same unmodified checkpoint exercised
all six thin script wrappers, including both anchor sides. These verify wrapper
dispatch and argument forwarding; they do not claim complete cannon, drill or
elevation story sequences. Every decoded-command fixture restored all 72 command
bytes and the script cursor and checked the original command replay. Only the
C8 completion case additionally prepared the documented caller states 57/58 on
the two live members, as in the preceding transfer verification.

Private reports: `build/runtime/eur_party_reunite/evidence_reunion83.json`,
`evidence_instant83.json` and the eight `evidence_*.json` reports in
`build/runtime/eur_party_manager_more/`. Together these cover **1,484 frames and
19 checked returns**, with every new routine observed. Initial-state SHA-1:
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`; save-83 SHA-1:
`2cb577d3008975c390a2f00e2b2cd646e4005c1b`. No ROM code was patched and all
**104 supplied saves** retained their hashes.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**610,596 / 1,563,700 bytes (39.05%)**; C/C++ plus assembly: **39.38%**.


## Field-party manager mode dispatch and action cancellation (2026-09-09)

Reconstructed **1,628 bytes** across three contiguous routines, extending the
existing manager module. Typed VM calls now select field modes, cancel the
selected action types and query active member actions. The dispatcher preserves
the original mode guards, paired-group reset, optional preservation arguments
and recovery-state flag writes. The busy query retains the VM caller's unused
third argument without inventing a role for it.

Nineteen controlled decoded-command replays from supplied save 83 checked
**3,019 frames and 35 returns**. They covered both group indices, the unchanged
mode-0 guard, dispatch to modes 1 through 8, mode-dependent cancellation and
both idle queries. Mode-5 entry was verified separately; its cancellation did
not complete in this checkpoint, which lacks the associated area-specific
resources. That branch and cancellation states not reached by these replays
remain statically verified. These are explicit VM request fixtures, not claims
of normal story progression through every special action.

Original-byte guards and SP-matched helper/return checks compared the complete
16,764-byte manager, four 1,440-byte members, renderers, helper arguments and
returned busy values. All 72 substituted decoded-command bytes and the script
cursor were restored at native wrapper entry, and original-command replay was
checked. No additional state fields or ROM code were patched. All 104 supplied
saves retained their hashes. Reports: the nineteen `evidence_*.json` files in
`build/runtime/eur_party_manager_modes/`. Initial state SHA-1:
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`; save-83 SHA-1:
`2cb577d3008975c390a2f00e2b2cd646e4005c1b`.

Validation: all 74 tests, public-content audit, whitespace checks and
zero-difference native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**612,224 / 1,563,700 bytes (39.15%)**; C/C++ plus assembly: **39.49%**.


## Field-party elevation launch and state-89 entry (2026-09-09)

Reconstructed **1,068 bytes** across two contiguous routines, extending the
special-actions module through `0x0208B208`. The elevation launcher validates
both members' movement/recovery states and support heights, handles room 458's
relative-height restriction, stops prior blink/vertical movement when needed
and starts both linear movements toward the requested elevation. State-89 entry
saves behavior, changes resources and starts vertical movement when inactive.

A **181-frame decoded-VM fixture** on supplied save 83 requested baby mode 8
then C6 elevation 120. Both members launched in room 459; complete party/member/
renderer comparisons verified the two stop calls and both thirteen-argument
linear movement calls. Original decoded-command bytes and script cursors were
restored and original-command replay checked. Recovery and room-458 rejection
branches remain statically verified.

A separate **424-frame controlled RAM fixture** reached state 89 through the
native Bros-Ball contact dispatcher. The test temporarily cleared only bit 9 at
`gSaveData + 0x48` (VM variable `0x2009`), selecting the dispatcher's pre-ability
fallback during normal keypad contact, then restored that bit before saving
the private checkpoint. Both behavior saves, both resource selections and both
vertical starts passed independent checks at every helper boundary and return.
This explicitly prepared ability state is not ordinary story coverage.

Reports: `build/runtime/eur_party_elevation/evidence_launch83.json` and
`evidence_state89_fixture83.json` (**605 frames, two checked returns**).
Initial-state SHA-1: `3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`;
save-83 SHA-1: `2cb577d3008975c390a2f00e2b2cd646e4005c1b`.
No ROM code was patched; all **104 supplied saves** retained their hashes.
Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**613,292 / 1,563,700 bytes (39.22%)**; C/C++ plus assembly: **39.56%**.


## Field-party auxiliary placement and launch (2026-09-09)

Reconstructed **3,044 bytes** across six contiguous routines, extending the
special-actions module back to `0x0208995C`. The functions manage auxiliary
placement, attached-sprite visibility, state-76 cancellation, renderer cleanup
and launching/finishing auxiliary objects. Checked shared layouts now expose
the launch record and all six special-resource indices; resource-table size
remains 11,068 bytes. Names remain neutral where the gameplay role is uncertain.

Two post-integration replays checked **1,152 frames and 33 target returns**,
observing all six functions. In room 284, decoded C2/CA/BB requests prepared the
action and ordinary Y presses launched two objects, covering both remaining-
count and empty-count finishes. In room 287, BD cancellation at native follower
state 76 reached placement, interrupted-state cleanup and both resource binds.
The saved-area-value restoration branch remains statically verified.

Room 287 was selected from the actual FEvData resource lists and confirmed in
live RAM: its primary resources include IDs 113/114 as well as 34/35/36. A
separate **601-frame D0 preparation** used the native transition for both groups
to establish room assets and party-presence flags; one incidental visibility
return also passed its oracle. The room transition terminates the old script,
so its 72 decoded bytes were restored without claiming original-script replay.
Both subsequent action replays restored the 72 bytes and cursor and verified
original-command replay. No additional RAM state or ROM code was patched.

SP-matched helper/return models compared complete 8,356-byte parties, 1,440-byte
members, 1,360-byte auxiliaries, 11,216-byte areas, 1,300-byte attached entities
and 312-byte renderers, including helper arguments. All **104 supplied saves**
retained their hashes. Reports: `build/runtime/eur_party_marker/` files
`evidence_launch284.json`, `evidence_cancel287.json`, `evidence_both287.json`.
Initial action states SHA-1: `011112768f64870262327dedb06dbecf1f6e223d` and
`28477b36a7f040b7dded7e4e78f1e1ab83da8a24`; save-51 SHA-1:
`459c9b510b361c4e800a396d8b56ddf847235c74`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**616,336 / 1,563,700 bytes (39.42%)**; C/C++ plus assembly: **39.75%**.


## Shared field-party action reset and resource overrides (2026-09-09)

Reconstructed **3,236 bytes** across five routines in two contiguous modules.
The shared action reset restores member presentation, independently saved
collision flags, normal motion and follower placement. Resource overrides save
animation speed and optionally restart animation; attached render priorities
can be copied to both members. Two entry helpers align movement to facing and
start the original fixed-point entry movement. Typed callers share declarations
in the party header; the collision backup view retains the checked entity size.

Three successful replays checked **1,486 frames and 78 returns**. Ordinary
keypad input on supplied save 83 covered nine resource binds, animation restart,
repeated speed-save guards, paired-group resource use and normal action reset.
Two decoded-VM mode sequences covered resetting adult modes 3/4 and baby mode 1,
restoring all eight independently saved collision bits and the collision-face
flag, and the region-71 follower-position/navigation calculation. The priority
helper was observed 60 times with no attached source; its copy branch and both
entry helpers remain statically verified. Preserved-state reset and nested
behavior restoration were not reached. No coverage is claimed for exploratory
cold-load or screen-transfer attempts that did not complete.

Byte-guarded, SP-matched helper/return models compared complete 8,356-byte
parties, 1,440-byte members, 1,360-byte auxiliaries and 312-byte renderers, along
with native helper arguments and calculated navigation bounds. VM preparations
restored all 72 decoded-command bytes and the script cursor, and checked the
original replay. No other RAM state or ROM code was patched; all **104 supplied
saves** retained their hashes. Reports: `build/runtime/eur_party_reset/` files
`evidence_keypad83.json`, `evidence_modes83.json`, `evidence_spin83.json`; the
last was rerun after integration. Initial-state SHA-1:
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`; save-83 SHA-1:
`2cb577d3008975c390a2f00e2b2cd646e4005c1b`.

Validation: all 74 tests, source audit, whitespace checks and zero-difference
native relink passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**619,572 / 1,563,700 bytes (39.62%)**; C/C++ plus assembly: **39.96%**.


## Field renderer resource configuration and rebinding (2026-09-09)

Reconstructed **1,448 bytes** across two routines, extending the contiguous
bounds/resource module through `0x020A7310`. They configure or replace graphics,
secondary animation data and palettes, allocate conversion storage when needed,
restore animation speed, normalize animation indices and refresh entity bounds.
The common header now checks the 88-byte descriptor and the 24-byte room-palette
record, including the allocator result after its 20-byte palette prefix.

Native call analysis recovered the fourth renderer-controller argument omitted
by the decompiler. The base descriptor initializer's ignored mode argument is
now explicit in its shared declaration and callers. Existing reconstructed
callers use checked resource types; the complete ROM remains byte-identical.

Five read-only runs covered **8,556 frames and 110 target returns**: ordinary
movement/action input from the save-83 field checkpoint, plus cold loads of
supplied saves **1, 51, 83 and 103**. They observed both screens and resource
sets, 55 conversion-buffer allocations, both direction modes (four/eight-way),
retained facing, explicit/retained speed and two out-of-range animation resets.
Configuration ran 94 times and rebinding 16 times. Rebinding with explicit
secondary/palette replacements or with bounds refresh disabled remains
statically verified; no decoded commands or RAM fixtures were used.

Byte-guarded, SP-matched oracles checked complete 1,300-byte entities, 316-byte
animation renderers, 88-byte descriptors and primary/secondary/palette records
at native helper boundaries and returns. Descriptor defaults and every helper
argument were checked, as were the renderer's resulting graphics, palette and
controller pointers. Screenshots and live main/sub OBJ-VRAM and standard OBJ
palette dumps accompany each report; these memory dumps preserve observations,
not a separate pixel-equivalence claim. All **104 supplied save hashes** remain
unchanged. Reports: `build/runtime/eur_field_resource_bind/evidence_keypad83.json`
and `evidence_cold{1,51,83,103}.json`. Initial field-state SHA-1:
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**621,020 / 1,563,700 bytes (39.71%)**; C/C++ plus assembly: **40.05%**.


## Shared sprite resource initialization and texture-offset lookup (2026-09-09)

Reconstructed **940 bytes**, extending the contiguous model-resource module
back to `0x0200BEF0`. Initialization copies the 44-byte presentation record,
selects packed tile counts for the hardware OBJ boundary, allocates texture
space, chooses palette indices, prepares or finds texture offsets and clamps
animation/frame values. The four-entry texture-layout lookup is now C++ too.
The checked 88-byte descriptor is shared by the model and field headers; the
model and graphics-resource views expose the confirmed fields without changing
any structure size. The adjacent resource-update routine remains private until
its register allocation matches.

Four read-only replays covered **6,523 frames and 1,117 target returns**:
405 initializations and 712 layout searches. These used ordinary keypad input
from the save-83 field state and cold loads of supplied saves **1, 51 and 103**.
They covered both screens, normal/alternate resources, both color formats,
allocation results 0/1/3, 195 alternate-offset conversions, 149 retained
conversion buffers, and 349 palette-index adjustments. Layout searches observed
the empty-list terminator; successful precomputed-layout lookup, explicit tile
counts, normal-offset conversion and animation/frame clamps remain statically
verified.

SP-matched helper/return models compared the complete 128-byte base model,
88-byte descriptor and graphics/palette records. They checked the graphics
binding, copied presentation bytes, helper arguments, packed tile calculations,
allocation flags and returned values. The fixed-state replay reproduced all
three captured images exactly. Final main/sub OBJ-VRAM and standard palette
regions matched the preceding resource-binding run for all four replays.
Cold-run screenshots had differences in animated regions, so no full image-
equivalence claim is made for those runs. All **104 supplied save hashes** were
unchanged. Evidence: `build/runtime/eur_model_resource_init/` files
`evidence_keypad83.json`, `evidence_cold{1,51,103}.json` and
`display_comparison.json`. Initial field-state SHA-1:
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**621,960 / 1,563,700 bytes (39.77%)**; C/C++ plus assembly: **40.11%**.

## Graphics offset decoder setup and screen texture tables (2026-09-09)

Reconstructed **728 bytes** in a contiguous graphics-resource module:
`GameGraphics_InitOffsetDecoder` and `GameGraphics_BuildScreenTextureOffsets`.
The shared, size-checked 36-byte decoder replaces the battle-only opaque view.
The screen table builder preserves repeated image offsets and aligns new images
using the hardware OBJ boundary and the native shape/size table. Existing model
and battle callers now use the typed interfaces.

Runtime verification covered **2,739 frames and 96 target returns**. An ordinary
cold load of supplied save **103** exercised 62 screen-table builds on both
screens, including **972 new aligned images and 1,151 reused image offsets**.
A battle-entry fixture from the save-55 field checkpoint exercised 34 decoder
initializations: 17 texture boundaries and 17 sub-screen OBJ boundaries, with
both normal and alternate resources. The fixture temporarily supplied decoded
command 0x11C from room 306, offset 0x2926 (encounter -32748); all 72 command bytes
and the script cursor were restored at native battle entry. This establishes
battle-entry coverage, not replay of the original command.

Byte-guarded, SP-matched helper/return models checked complete decoder records,
source headers, group records, object tables and generated output tables. The
same-boundary direct-copy path, empty tables and repeated images with no earlier
match remain statically verified. Screenshots and live main/sub OBJ-VRAM and
palette dumps accompany `build/runtime/eur_graphics_offsets/` reports
`evidence_cold103.json` and `evidence_entry55.json`. All **104 supplied saves**
retained their hashes. Battle checkpoint SHA-1:
`e44df106d65e52df1ffc2b125538354f67cb1b22`.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**622,688 / 1,563,700 bytes (39.82%)**; C/C++ plus assembly: **40.16%**.

## Save-menu text streaming and VRAM uploads (2026-09-09)

Reconstructed **1,372 bytes** across four functions: object-text streaming,
object-row drawing, background upload and object-strip upload. The streaming
path emits 256-byte blocks and rolls its glyph buffer at 256 pixels. The row
path handles explicit newline controls before queuing the two 128-byte halves
of each sprite strip. Native source alignment and background character-base
selection are preserved. Larger dialog constructors and the adjacent texture
upload callback remain private while their compiler allocation differs.

Three ordinary keypad runs covered **2,959 frames and 92 target returns**:
3 streamed strings, 12 row draws, 76 object-strip uploads and 1 background
upload. A cold load of supplied save **103** covered 61 streamed blocks and
6 buffer rolls. The established save-55 confirmation and copy-menu checkpoints
covered 7 explicit newlines, multiline text and the copied-save message. These
operations used emulator working backup data; no original save was modified
and no command or RAM fixture was injected.

Byte-guarded, SP-matched helper/return models compared complete 24,724-byte text
contexts, 72-byte tasks and output offsets. All helper arguments were checked;
independent snapshots verified **60,480 copied or cleared bytes** at actual RAM
and VRAM destinations. Streaming was observed on the sub screen and object /
background uploads on the main screen; the opposite screen combinations and
zero-column rows remain statically verified. The final four OBJ-VRAM/palette
regions of the cold run matched the preceding graphics-offset run. Screenshots
and memory dumps accompany `build/runtime/eur_save_menu_object_text/` reports
`evidence_cold103.json`, `evidence_pack55.json` and `evidence_copy55.json`.
Checkpoint SHA-1 values: `78e17c1fe46026d1115c27b212d6f0e39fa4948a` (save),
`b99ae49572f9d3d22933b903c66355958e3709f9` (copy). All **104 supplied saves**
retained their hashes.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**624,060 / 1,563,700 bytes (39.91%)**; C/C++ plus assembly: **40.24%**.

## Sprite resource replacement and allocation policy (2026-09-09)

Reconstructed `BattleModel_UpdateSpriteResources`, **1,032 bytes**. It detaches
the renderer, replaces graphics and conversion references, selects packed tile
counts, and applies exact-size, grow-only or forced reallocation. It optionally
replaces the palette, converts or looks up image offsets, resets animation and
restores the render-list link. The hardware boundary value retains its native
unsigned 32-bit working type. The shared model-resource module now also contains
the formerly separate lifecycle functions, completing the contiguous region
from `0x0200BA34` through `0x0200C5D8`. Its controller wrapper now has the void
return type used by the virtual interface.

Two ordinary movement/action replays from supplied saves **83 and 51** covered
**595 frames and 19 checked returns**. They observed 18 texture reallocations
and one retained allocation, normal resources on the main screen, header tile
counts, palette replacement, texture-offset lookup and palette-index adjustment.
Grow-only/forced policy, explicit tile counts, screen changes, retained palettes
and conversion-buffer branches remain statically verified. The cold-load-only
attempt did not reach this routine and is excluded from coverage.

Byte-guarded, SP-matched helper/return models compared the full 128-byte model,
88-byte descriptor, 24-byte graphics header and 20-byte palette record at every
native helper boundary. They verified exact arguments, resource pointers,
allocation results, flags and animation reset. Screenshots and main/sub
OBJ-VRAM and palette dumps accompany `build/runtime/eur_model_resource_update/`
reports `evidence_keypad83.json` and `evidence_split51.json`. Initial-state SHA-1:
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61` and
`819304ed66859f343c2b324f49a84833d79c029c`. No code, command or RAM fixtures;
all **104 supplied save hashes** remained unchanged.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**625,092 / 1,563,700 bytes (39.98%)**; C/C++ plus assembly: **40.31%**.

## Sprite texture upload dispatch (2026-09-09)

Reconstructed `BattleModel_UpdateSpriteTexture`, **260 bytes**. Dirty sprites
upload either their current frame group or all alternate-resource images, using
the screen's OBJ-VRAM base and hardware boundary. The texture dirty flag is
cleared only after the upload. The render-list functions now share the contiguous
C++ model-resource module (`0x0200B6C4` through `0x0200C5D8`); every moved function
retains its exact native bytes. The C vtable view exposes the confirmed texture-
dirty query slot, and the overlay-5 wrapper uses the typed upload interface.

Ordinary field input from save **83** and a cold load of save **103** covered
**2,457 frames and 94,179 returns**: 93,542 clean-texture skips and 637 uploads.
Uploads reached both screens (513 main, 124 sub), normal/alternate resources
(617/20) and OBJ boundary modes 1/2. Byte-guarded, SP-matched models checked full
128-byte sprite states, graphics headers and animation/frame records, exact
upload destinations and arguments, preservation of the model through the native
uploader, and dirty-bit clearing afterward. The pixel converter itself remains
assembly and is not claimed as newly reconstructed.

Final main/sub OBJ-VRAM and standard palette dumps matched the preceding runs
for both replays; screenshots accompany `build/runtime/eur_model_sprite_texture/`
reports `evidence_keypad83.json` and `evidence_cold103.json`. No fixtures were
used and all **104 supplied save hashes** remained unchanged.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**625,352 / 1,563,700 bytes (39.99%)**; C/C++ plus assembly: **40.33%**.

## Save-menu dialog dispatch and sprite controls (2026-09-09)

Reconstructed four adjacent controls, **360 bytes**: dialog construction
dispatch, task-group reset, dialog OBJ-VRAM clearing, and slot-choice drawing.
The draw callback selects the enabled or disabled palette from the selected
slot's occupancy. The shared, layout-checked 64-byte `Overlay5Sprite` record
also replaces the identical private menu-item sprite definition.

Four ordinary keypad runs covered **2,841 frames and 4,274 target returns**:
4,256 choice draws and six returns each from construction, reset and clearing.
They observed occupied slots (1,984 draws), empty slots (144), retained palettes
on other rows (2,128), and dialog kinds 1, 3 and 4. Kinds 0, 2 and the default
branch remain statically verified. The constructors reached by the dispatcher
remain assembly; their complete text contexts are not claimed as newly modeled.

Byte-guarded, SP-matched models checked full 72-byte tasks, 64-byte sprites,
slot selection and occupancy, all dispatch/reset/draw helper arguments, and
**36,864 bytes cleared in live OBJ-VRAM**. Reports and screenshots are in
`build/runtime/eur_save_menu_dialog_control/`: `evidence_cold103.json`,
`evidence_pack55.json`, `evidence_empty55.json` and `evidence_copy_menu55.json`.
The latter three checkpoints have SHA-1 values
`78e17c1fe46026d1115c27b212d6f0e39fa4948a`,
`40767c169c83cce614cbf00c2f575050e398f706` and
`2f0e7761766fc37095b4838693bd76764d5c35cf`. These runs used emulator working
backup data without command or RAM fixtures. All **104 supplied saves** retained
their hashes. Cold-load and save-confirmation final OBJ-VRAM/palette regions
matched the preceding corresponding runs.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**625,712 / 1,563,700 bytes (40.01%)**; C/C++ plus assembly: **40.35%**.

## Erase-menu confirmation and Game Over models (2026-09-09)

Reconstructed seven functions, **812 bytes**: the contiguous erase-menu
confirmation controls at `0x02076934` through `0x02076B38`, plus
`GameOverMenu_CreateModels`. They create and position the confirmation cursor,
show/hide the dialog's display planes, queue its background text and configure
the Game Over models. The 72-byte cursor task and menu-control prefix have
checked layouts. The adjacent sprite-grid constructor and Game Over cursor
update remain assembly.

Two cold boots with supplied saves **1 and 103** used ordinary keypad input:
hold **A+B+X+Y+L+R**, wait for the erase confirmation, optionally move left/right,
and cancel with B. No session-state redirection was needed. They covered
**1,884 frames and 676 returns**, including all six erase-menu functions,
666 cursor callbacks (632 drawn, 34 hidden), both selections (30/636), and
10 live display-plane checks. A guard confirmed `SaveStorage_Erase` never ran.
The visible confirmation and both final OBJ-VRAM/palette sets were checked;
the two runs ended with identical display-memory hashes.

A separate save-86 checkpoint covered Game Over model creation in **481 frames**.
The documented decoded-command fixture substitutes opcode `0x123` with fade 1
at natural Field VM dispatch and restores all 72 command bytes at the native
Game Over helper entry. This reaches the normal scene transition; it does not
claim an ordinary gameplay defeat or replay of the replaced script command.
The observed constructor enabled the cursor; the cursor-disabled branch remains
statically verified. Initial checkpoint SHA-1:
`23b87fbd8bca1084bc3016e55b3fb633f7856fa8`.

Across the three runs, byte-guarded, SP-matched models checked complete 72-byte
tasks and 128-byte sprite models at native helper boundaries, exact task/resource/
animation arguments, parent links, positions, flags and display writes.
Reports/screenshots: `build/runtime/eur_save_scene_cursors/`,
`evidence_erase_keys1.json`, `evidence_erase_keys103.json` and
`evidence_gameover86.json`. All **104 supplied saves** retained their hashes.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**626,524 / 1,563,700 bytes (40.07%)**; C/C++ plus assembly: **40.40%**.

## Pause-menu spring chains (2026-09-09)

Reconstructed six contiguous functions, **1,004 bytes**, at `0x02076374`
through `0x02076760`. The menu's hanging character elements use four points
and three distance constraints per chain. The new module initializes these
links, integrates motion, applies minimum-Y limits and distance corrections,
updates damped velocities, restores pinned points, and advances available
chains' states. Shared point/link/chain layouts are checked at 44/12/236 bytes;
positions use 12 fractional bits and gravity scale uses 8.

Three ordinary pause-menu/input replays from supplied save **20** covered
**1,900 frames and 61,892 returns**: 12 initializations, 4,760 full updates,
4,760 integrations, 47,600 constraint passes and 4,760 velocity updates.
They checked **142,800 link corrections**, pinned/free points and 307 minimum-Y
clamps. All four chain indices and the normal ten solver iterations were
observed. `MenuSpring_AdvanceActiveChains`, unavailable-party skipping,
zero-length constraints, zero counts and other iteration counts remain
statically verified; the additional item-menu replays did not reach the
state-advance function and are not claimed to cover it.

Byte-guarded, SP-matched models independently checked complete point, link and
chain records, fixed-point arithmetic and exact `FX_Sqrt`/`FX_Div` arguments.
They use those existing helpers' returned values when modeling corrections;
the helpers themselves are not newly reconstructed in this batch. Screenshots
and main/sub OBJ-VRAM/palette dumps accompany
`build/runtime/eur_menu_springs/evidence_menu20.json`, `evidence_items20.json`
and `evidence_select_item20.json`. Initial state SHA-1:
`d2c8a01fd324265a80ade9948d2d5d397de5a314`. No command or RAM fixtures were used;
all **104 supplied saves** retained their hashes.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**627,528 / 1,563,700 bytes (40.13%)**; C/C++ plus assembly: **40.47%**.

The shared 64-byte draw-list record is now named `Overlay5ObjectSprite` to
distinguish it from the existing 336-byte `Overlay5Sprite` model-pool record.
This type-name correction changes no generated bytes or progress; the full
validation suite and canonical ROM hash remain unchanged.

## Field auxiliary renderer resource binding (2026-09-09)

Reconstructed `FieldEntity3D_ConfigureAuxiliaryResources`, **488 bytes**, exactly
on the first private compilation. It retains the primary, secondary and palette
records, allocates texture-offset conversion storage when needed, initializes
the 88-byte descriptor and configures/restarts the auxiliary renderer.
The twelve newly identified resource-pointer bytes complete `FieldRuntimeEntity`
at **0x520 bytes**, matching the existing native allocation/clear size. The four
derived entity layouts lose their equivalent opaque padding and retain all
original sizes and field offsets. Bounds-resource binding and the new routine
now share the contiguous spatial-lifecycle module (`0x020B5364`â€“`0x020B64E8`).

A cold load of supplied save **103** covered **2,033 frames and 15 returns**,
including 12 main-screen and 3 sub-screen bindings, all with conversion-buffer
allocation. Byte-guarded, SP-matched models checked full 1,312-byte entities,
312-byte renderer records, 88-byte descriptors and all resource records, plus
every conversion-size, heap-allocation and virtual-call argument. Missing
renderers and zero-sized conversion buffers remain statically verified.

Evidence and screenshots: `build/runtime/eur_field_aux_resources/`,
`evidence_cold103.json`. Final main/sub OBJ-VRAM and palette hashes match the
preceding verified cold-103 replay. No fixtures; all **104 supplied save hashes**
remained unchanged.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**628,016 / 1,563,700 bytes (40.16%)**; C/C++ plus assembly: **40.50%**.

## Spatial field bounds update (2026-09-09)

Reconstructed `FieldEntity3D_UpdateBounds`, **1,160 bytes**, and extended the
contiguous spatial-lifecycle module to `0x020B4EDC`–`0x020B64E8`. The routine
selects the five signed indices in each six-byte animation record, builds body,
navigation and interaction extents, computes four corner angles and resolves
optional four-byte lookup rectangles. Checked rectangle/lookup types and named
cached indices replace opaque fields without changing entity layouts. Default
bounds and navigation-change flags preserve the native behavior.

Ordinary input on supplied saves **103** (cold load) and **83** (movement and
jumping) covered **2,457 frames and 98 returns**. Independent full-entity models
verified 70 resource-backed and 28 default updates, 40 default and 30 custom
lookup selections for both rectangle groups, six changed-navigation updates,
39 unchanged/unsynchronized updates and 25 skipped navigation updates. All 392
angle-helper argument pairs and 98 interaction-bound updates were checked;
the existing angle helper's returned values supply the corner-angle results.
The out-of-range-animation fallback remains statically verified.

Evidence, screenshots and display-memory dumps are in
`build/runtime/eur_field_spatial_bounds/evidence_cold103.json` and
`evidence_movement83.json`. The movement state SHA-1 is
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`; final cold-103 OBJ-VRAM/palette hashes
match the previous verified resource-binding replay. No fixtures were used;
all **104 supplied saves** retained their original hashes.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**629,176 / 1,563,700 bytes (40.24%)**; C/C++ plus assembly: **40.57%**.

## Party transition requests and paired bounds (2026-09-09)

Four adjacent functions add **1,044 matching C++ bytes**:
`FieldPartyManager_QueuePairedBounds`, `FieldPartyManager_CheckRegion71Bounds`,
`FieldPartyManager_ApplyPendingTransition` and
`FieldPartyManager_TransitionThroughBounds`. They occupy one contiguous module
at `0x0209F230`–`0x0209F644`. The manager now exposes its checked 12-byte pending
transition (signed room, script, coordinates and BGM plus packed options), the
pending paired-bound flags and the selected record pointer. Its 16,764-byte
layout is unchanged. The bound-based transition interpolates the applicable
axis using the leader's navigation extents, then passes the native screen and
entry-slide options to the existing transition routine.

Live RAM identified the south exit of room 459 at X=352–416, Y=764–768.
Ordinary `right:20`, `down:90`, `wait:180` input from the verified checkpoint-83
state entered room 458 in **293 frames** and reached
`FieldPartyManager_CheckRegion71Bounds` once. Its missing-manager-flag return
was checked against the complete manager record. Screenshots confirm the room
change. Evidence: `build/runtime/eur_field_party_transition_requests/`
`evidence_south_exit83.json`, source state SHA-1
`3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61`.

Coverage limit: the queue, pending-application and bound-interpolation functions,
and the deeper branches of the region check, remain statically verified only.
Additional keypad navigation on checkpoints 20/83 did not reach those paths;
zero-return reports are navigation evidence, not runtime coverage. All four
routines have byte-guarded full-record runtime models ready for a suitable
cross-screen transition. No fixtures were used in these replays; all **104
supplied save hashes** remain unchanged.

Validation: all 74 tests, public-source audit, whitespace checks and native
relink with zero differences passed. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**630,220 / 1,563,700 bytes (40.30%)**; C/C++ plus assembly: **40.64%**.

### Battle map reload pipeline: 40.53% matching C/C++

Six functions from `0x02089F2C` through `0x0208AD54` add **3,624 exact C
bytes** to the contiguous battle map transfer module. The queue validates the
resource index, opens its archive, computes sixteen section sizes and loading
destinations, binds the map objects, resets scroll and animation state, clears
both displays, and transfers palettes, tilemaps and animation streams. The
eight-byte channel records have a checked layout. Native register data flow
established that resource access and the conditional track-command loads must
stay in their respective branches; no compiler permutation search was used.

Runtime evidence: `build/runtime/eur_battle_map_reload/evidence_reload55.json`
and `evidence_reload83.json`, using checkpoint 55/83 battle states with SHA-1
`a8b485d4cc4b72586af611455830b5f89f67bc07` and
`850ad8741a5474f336724f26698a502f1fb52457`. Each run injects one explicitly
controlled decoded battle VM command `0xC1`: reload the current resource
(352 or 576), extent 32, fade-out/in durations 8. It restores all 72 command
bytes and 172 VM-state bytes at the native dispatcher return. The next native
decode is checked to revisit the original script position. This is a fixture,
not a claim that ordinary keypad input initiated the background transition.

Across **602 frames**, all six functions return **34 times**, including all
twelve normal reload states twice. Exact helper arguments and full task,
resource, archive-request, section-table and map-control records pass checks.
Copies verify **18,496 bytes** after execution; display clears verify
**49,408 bytes**. Resource 576 also exercises both previous-track resets,
palette-animation lookups and both new matrix-animation tracks. Screenshots
show the restored forest battle and animated star-room battle; main/sub BG
VRAM and palettes are dumped and hashed in the reports. All **104 original
save hashes** remain unchanged.

Coverage limits: invalid indices, empty resources, dispatch state 16, nonempty
optional tilemap layers and reusing an already-existing track during the load
remain statically verified. Adjacent initial-load routines are still deferred
because their register allocation has not matched.

Validation: native relink has zero differing bytes; all 74 tests, public-source
audit and whitespace checks pass. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++:
**633,844 / 1,563,700 bytes (40.53%)**; C/C++ plus assembly: **40.87%**.


## Alternate battle model afterimages (40.56% C/C++)

- Reconstructed the contiguous 0x0206BD74..0x0206BF3C region in
  `src/battle/battle_model_afterimage.c`: capture the alternate model's animation,
  frame, flip flags, scale, rotation and projected position; render that snapshot;
  advance its depth and remove its callback when its signed lifetime expires.
  The existing checked impact-particle layout is shared with these callers.
  Three functions add 456 exact bytes: 634300 / 1563700 (40.56%) C/C++;
  C/C++ plus ASM is 40.90%. No ASM is counted as C/C++.
- Native relink has zero differing bytes; all 74 tests, progress consistency,
  public-content audit and whitespace checks pass. Rebuilt ROM SHA-1 remains
  `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Runtime: save 83, Star Shrine before Petey Piranha. Existing cannon setup state
  SHA-1 `850ad8741a5474f336724f26698a502f1fb52457`, A for eight frames, then
  2200 frames with four RAM-timed ordinary A/B/X/Y presses: 531 snapshots,
  2124 renders and 2655 lifetime updates, including all 531 expirations.
  A second run selects the trampoline through the ordinary Bros-item menu from
  state `21d2e64a24b389689627292539103880c6761b47`: 3190 frames, 295 snapshots,
  1180 renders and 1475 lifetime updates, including all 295 expirations.
  Both return to a usable battle screen, inspected visually. No command or RAM
  fixture was injected in either probe; the initial states are existing battle
  checkpoints, not fresh cold boots from the supplied save.
- Byte-guarded entry/call/return hooks compare the complete 36-byte task record,
  unchanged scene/model input records and exact pool/projection/draw arguments.
  All 826 projected positions agree with an independent arithmetic oracle;
  3304 renders restore the observed animation/frame, scale, rotation, flags and
  position fields. Main/sub BG VRAM and palette dumps and hashes accompany
  `build/runtime/eur_battle_model_afterimage/evidence_cannon83.json` and
  `evidence_trampoline83.json`. All 104 supplied save hashes remain unchanged;
  save 83 SHA-1 is `2cb577d3008975c390a2f00e2b2cd646e4005c1b`.
- Coverage limits: these attacks used unflipped alternate models. Null-model
  rejection, flipped inputs and signed-coordinate overflow are covered by the
  exact native comparison, not claimed as observed runtime branches. The larger
  alternate-model drawing routine remains assembly.


### Room party placement and item collection: 40.66% matching C/C++

Four contiguous functions at 0x0207EF18..0x0207F4D0 add **1,464 exact C++
bytes** in `src/field/field_room_interaction.cpp`. They ensure a party is present
in the current room, search uncollected mode-zero placements against the actor's
navigation bounds, and collect items from placement records or runtime blocks.
The field's manager pointer is typed. Checked placement/entity layouts now expose
item, amount index and remaining uses; the shared five-bit placement parameter
means a navigation change for mode zero and a use count for runtime blocks.
Signed halfword coordinates, the wrapped variable-bank offset and the shared
null/end return are retained from native evidence. No compiler search was used.

Runtime reports live under `build/runtime/eur_field_room_interaction/`:

- `evidence_block83.json`: checkpoint 83, room 459, state SHA-1
  `590c4bc2877e1f1781faf521ca2a8b87ea1f9258`. A decoded D0 room-position fixture
  uses a block's observed coordinates (168,320,64); the party stands at height
  zero. Two D3 requests cover the backup-active and already-present branches.
  An ordinary eight-frame A press hits the block: item 0x1003 goes **45 -> 47**,
  its final use is consumed and its persistent variable is written. 811 frames.
- `evidence_bean19.json`: checkpoint 19, state SHA-1
  `1be5dcdb562896ed9359e48181353393fb1c985d`. The bean placement at (48,320,0) in
  room 73 was read from live RAM. D0 loads that room; BC resets the original
  adult piggyback mode; CA switches to the babies; D3 positions them; BC selects
  their drill mode. An ordinary eight-frame X press surfaces, finds the
  uncollected record and collects the bean: its separate counter goes **2 -> 3**.
  Both persistent-variable writes and the navigation-change call are checked.
  991 frames.
- `evidence_bean20.json`: the same room/placement with checkpoint 20, state
  SHA-1 `d2c8a01fd324265a80ade9948d2d5d397de5a314`. Its persistent collection
  variable is already one, so the search returns null and no item is awarded.
  991 frames.

These are controlled setup fixtures followed by ordinary pickup input, not
claims of uninterrupted navigation from the save point. The D3 restore cases
explicitly set only the tested party's backup-active bit before entry; the
native routine clears it. D3, BC and CA restore all 72 decoded-command and 172
pre-decode state bytes and verify the next decode revisits the original script.
D0 restores the 72 command bytes before its native wrapper and rebuilds the
room scripts; no original-command replay is claimed for D0.

Across **2,793 frames**, all four new routines return (eight returns total).
Byte-guarded entry/call/SP-matched return hooks check complete 11,216-byte areas,
16,764-byte managers, party entities and twelve-byte records. Exact helper
arguments, independent inventory-cap arithmetic, resulting inventory counters
and party coordinates agree. The checks include both party sides, successful
and already-collected searches, the special bean counter and a block's final
use. Screenshots of the resulting field scenes were inspected; OBJ VRAM and
palette dumps are hashed in the reports. All **104 supplied save hashes** remain
unchanged. Save 19/20/83 SHA-1 values are respectively
`ee08102d192b97a70b787025fc791c8e3c474439`,
`e78faca7c148cb07941601554f06f279a29c4488`, and
`2cb577d3008975c390a2f00e2b2cd646e4005c1b`.

Coverage limits: null placement tables, multiple-record scans, full inventory,
coin collection, ordinary non-bean placement rewards and multi-use blocks remain
statically verified. No runtime claim is made for those branches.

Validation: zero differing native relink bytes, all 74 tests, progress check,
public-content audit and whitespace checks pass. Canonical ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++ is
**635,764 / 1,563,700 bytes (40.66%)**; C/C++ plus ASM is **40.99%**.


## 2026-09-09 - Field room departure and transition requests (+3,600 bytes)

`src/field/field_room_transition.cpp` reconstructs the contiguous region
0x0207F4D0..0x020802E0 as four byte-identical C++ routines:
FieldArea_PrepareRoomDeparture (1,352 bytes), FieldArea_QueueRoomChange (176),
FieldArea_QueueBothPartyRoomChange (652), and FieldArea_QueuePartyRoomChange
(1,420). These preserve groups left behind, detach outgoing actors, persist
active variable entities, stop screen-owned effects, select music fade policy,
prepare arrival coordinates/modes, reset special party actions and request a
paired-screen room change when its native eligibility conditions hold.

The checked field layout now contains the twenty-byte transition record,
shared system/timer pointers and paired area. Room metadata and public request
declarations are shared with existing VM/bounds callers. Narrow parameter types
follow native stack loads; signed coordinates, unsigned locomotion state and
the visibility expression reproduce the native code without compiler searches.
Unknown state bits retain neutral names.

Runtime reports are under `build/runtime/eur_field_room_transition/`:

- `evidence_south83.json`: checkpoint 83, state SHA-1
  `590c4bc2877e1f1781faf521ca2a8b87ea1f9258`. Ordinary right:20, down:90,
  wait:180 inputs cross from room 459 to 458. The piggyback group requests a
  room change with fade, both groups detach, and music policy -3 retains the
  current track. No fixture or RAM mutation. 293 frames, two target returns.
- `evidence_reload83.json`: the same state, 801 frames. Decoded D0 and D1 VM
  fixtures reload current room 459 using live party coordinates. The D0 request
  carries both groups; D1 carries neither, so departure snapshots both groups.
  Music policies -1 and -2 respectively stop and retain music. Four returns.
- `evidence_reload19.json`: checkpoint 19, room 63, state SHA-1
  `1be5dcdb562896ed9359e48181353393fb1c985d`; the same fixtures exercise another
  story location and room resources. 801 frames, four returns.

The reload fixtures restore all 72 decoded-command bytes before their native
wrappers. Native room reload destroys the room script, so these are controlled
requests, with no claim of original-command replay or ordinary traversal.
Across 1,895 frames all four routines return, ten returns total. Byte-guarded
entry/call/SP-matched return checks cover complete 11,216-byte areas, 16,764-byte
managers, 1,440-byte actors, forty-byte timers, 952-byte shared systems and
variable entities where present. Arrival descriptors, direct memory writes and
all called-helper arguments agree; helper side effects are refreshed explicitly.
Eight party-state copies occur across the reload scenarios. Screenshots show
normal field scenes after all three probes; OBJ VRAM/palettes are dumped and
hashed. All 104 supplied saves remain unchanged. Save 19 and 83 SHA-1 values are
`ee08102d192b97a70b787025fc791c8e3c474439` and
`2cb577d3008975c390a2f00e2b2cd646e4005c1b`.

Coverage limits: non-piggyback single-party requests, special-action cleanup,
eligible paired-screen synchronization, active timer hiding, entity-variable
persistence and remaining music branches are statically verified, not claimed
as runtime-covered. An exploratory X/right/down input entered the save menu
and reached no target; it contributes no coverage.

Validation: native relink has zero differing bytes; all 74 tests, progress,
public-content and whitespace checks pass. ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++ is
**639,364 / 1,563,700 bytes (40.89%)**; C/C++ plus ASM is **41.22%**.


## 2026-09-09 - Overlay 25 projectile effect launch (+1,460 bytes)

`src/overlay025/enemy_projectile_effects.cpp` reconstructs three exact C++
routines in 0x020C57F0..0x020C5DA4: ReleaseProjectileEffects (640 bytes),
WaitProjectileEffectAnimation (376), and BeginProjectileEffects (444). The
adjacent existing ResetAnimation (88) is merged into the same contiguous
module. The sequence positions object 40 relative to the boss, projects into
the selected view, spawns sprite/model effects 525..527 and 821..823, waits for
the animation trigger and attached effect, releases two impact emitters, and
prepares object 42 and the sibling animation-reset task. Shared checked task,
work, object and model layouts retain neutral names for unknown fields.

Runtime: checkpoint 103, original save SHA-1
`c264e8a8b26cb4994da8a93b164979377b7b4090`. A decoded FieldVM 0x11C fixture
uses encounter 8232 from room 583 offset 0x0536; the probe restores all 72
command bytes and the original cursor at native battle entry. The normal first
phase has overlay 25 loaded but executes none of its callbacks. A separate
two-byte live-HP fixture (1800 to 1) followed by an ordinary Jump reaches the
native giant second phase of Elder Princess Shroob. The resulting state is
`build/runtime/eur_ov25_projectile_launch/phase103.dst`, SHA-1
`f83e3507a0e81de426d54c5da1d4593a365d5f8c`. Ordinary inputs from that state
exercise the native overlay-25 movement and other attacks, confirming the
callback dispatcher at 0x020B6114 and its task/work pointers.

`evidence_effects_fixture103.json` in the same directory covers all three new
routines: Begin 1 return, Wait 22, Release 121 over 1,670 frames. At frame 320 a
four-byte live task callback change selects native initializer 0x020C60D4
before the dispatcher loads it. Native code initializes and loads the resources.
At frame 500 a second four-byte callback change enters 0x020C5BE8 after that
initialization, bypassing the preceding motion gate. The gate did not complete
with this substituted attack, so this is explicitly isolated effect coverage,
not validation of a complete natural attack or its script prerequisites.

Byte-guarded entry/call/SP-matched return checks verify all direct writes to
complete 36-byte tasks, 7,088-byte work records, 260-byte objects and 440-byte
models, plus exact helper arguments. Five independently calculated projections
and three final object positions agree. Both wait paths and both transitions
are covered: 21 animation waits, one animation completion, 120 attached-effect
waits and one release. External helper effects are refreshed explicitly. Raw
position and alternate-view branches remain static-only. BG VRAM/palettes are
dumped and hashed; screenshots show the giant battle scene, with no claim that
the substituted attack reaches normal completion. All 104 supplied save hashes
remain unchanged; executable code is never modified by the probes.

Validation: zero native relink differences, canonical ROM SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`, 74 tests and progress/public-content/
whitespace checks pass. Matching C/C++ is **640,824 / 1,563,700 bytes (40.98%)**;
C/C++ plus ASM is **41.32%**. The adjacent projectile-growth callback remains
private because its first readable draft still differs in register allocation.


## 2026-09-09 - Overlay 25 projectile emission, hits and linked effects (+1,604 bytes)

Five more routines match exactly: Overlay25Enemy_EmitProjectile (464 bytes),
Overlay25Projectile_CheckHit (208), Overlay25Enemy_BeginLinkedEffects (216),
Overlay25Enemy_InitializeLinkedEffects (416), and
Overlay25Enemy_WaitLinkedEffectAnimation (300). They join existing neighboring
functions in two contiguous modules: `enemy_linked_effects.cpp`,
0x020CAF7C..0x020CB664, and `enemy_projectile_sequence.cpp`,
0x020CB838..0x020CBBB0. Three old small source files are consolidated. The
checked 7,088-byte work layout now exposes all sixteen source slots, sixteen
secondary slots and sixteen depth offsets used by native linked-object updates.

The emission routine spaces shots by 54 frames, selects the party target from
the current attack mask, and initializes sibling tasks. Hit checking scans the
twenty-byte native collision records, distinguishing outgoing and incoming hits
from a missed projectile. The linked-effect sequence selects an eligible party
actor, loads resource 52, binds object pairs 40/41 and 42/43, and transitions
through effects 510/511 and 802/803. Unknown scene bits retain neutral names.

Runtime reports: `build/runtime/eur_ov25_projectile_burst/`.
`evidence_ordinary103.json` runs 2,570 frames using ordinary B/wait inputs from
the previously documented giant-phase state (SHA-1
`f83e3507a0e81de426d54c5da1d4593a365d5f8c`). This probe makes no RAM changes:
163 EmitProjectile returns include four shots, two targeting each party member,
159 delay frames and the final-shot transition. The 224 CheckHit returns include
220 movement waits and four outgoing collisions. The battle reaches Mario's
next command wheel normally, with party HP 47/64.

`evidence_linked_fixture103.json` uses one four-byte task callback substitution
at frame 320, selecting 0x020CB248 immediately before the native dispatcher loads
it. All resource loading and later transitions remain native. Its 1,170 frames
cover BeginLinkedEffects once, InitializeLinkedEffects twice (including one
load wait), and WaitLinkedEffectAnimation once. The target is Mario; the final
screenshot shows the large purple/white orb and particles above the giant boss.
This is controlled sequence-entry coverage, not a claim of a complete ordinary
attack. Unlike the preceding launch probe, it needs no intermediate gate bypass.

Across both reports, byte-guarded entries, calls and SP-matched returns validate
391 completed calls to all five new routines. Full task/work/object/model,
party-actor, effect and hit records agree at the relevant boundaries. Six
independent view projections and one final object position agree with native
results; called-helper arguments agree, while helper side effects are explicitly
refreshed. Incoming collisions, missed projectiles, ineligible-party selection
and the linked animation wait remain static-only. BG VRAM/palettes are dumped
and hashed; all 104 original supplied saves remain unchanged.

Validation: zero native relink differences, canonical ROM SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`, all 74 tests, progress/public-content
and whitespace checks pass. Matching C/C++ is **642,428 / 1,563,700 bytes
(41.08%)**; C/C++ plus ASM is **41.42%**. Drafts of the adjacent motion starter
and loading/movement callback remain private because they still differ.


## 2026-09-09 - Pause scene deleting destructor and interior labels (+1,036 bytes)

`PauseScene_Delete` at 0x02072270 is now matching C++ in the existing contiguous
`src/overlay007/pause_scene_lifecycle.cpp` module. Its typed cleanup mirrors the
native non-deleting destructor and additionally frees the task allocation. It
stops IRQ/DMA work, releases scene resources, all sprite slots and owned arrays,
palette/sprite allocations and frontend systems, clears global owners, destroys
the base task and frees the scene task itself. An audit of prior private objects
identified this exact candidate; it was rebuilt against current shared headers
before integration, rather than accepting an old object as evidence of progress.

The original scene path callbacks refer to 0x020722D8 and 0x020724B0 inside this
function. Compiled C does not export these assembly labels automatically.
`config/eur/arm9/linker_aliases.json` records their owner and original symbol map;
`tools/apply_linker_aliases.py` generates function-relative assignments at offsets
0x68 and 0x240 after DSD generates the linker script. These assignments emit no
bytes and preserve the shipped targets, including their unusual placement. The
tool rejects unaligned/out-of-range entries, functions used as labels, duplicate
definitions and missing/ambiguous object placements. Seven focused tests cover
these constraints. This does not claim that the affected scene path commands
were executed or that their original destinations should be reinterpreted.

Runtime reports under `build/runtime/eur_pause_scene_delete/`:
- `evidence_065.json`: story save 65, Thwomp Volcano after the eruption stops;
  original save SHA-1 `0844b75810855bc3a738122b29382ed5a6c9f983`, derived state
  `ece238ed785fda646a77cde5e886aa7da3e009d7`. 1,089 frames.
- `evidence_103.json`: save 103, Peach's Castle before Shrowser, cold boot from
  original save SHA-1 `c264e8a8b26cb4994da8a93b164979377b7b4090`. 3,382 frames.

Both use ordinary Start/A/B/directional inputs to open the pause menu, visit
submenus and return to the field. Each has one checked initialization and one
checked deleting-destructor return. Full code bytes guard entry; return hooks
match SP. All 48 sprite slots are populated after initialization and cleared
after deletion. Each deletion clears all recorded owned-array slots and releases
all seventeen non-null arrays, with the free-event set reset at destructor entry
to exclude older allocations. The task allocation is also passed to scalar
delete during that call; scene/party/global owners are cleared. This checks
ownership outcomes, not every byte of the heap or every helper side effect.
Final screenshots show normal field scenes. BG VRAM/palettes are dumped and
hashed. No RAM fixtures are used and all 104 supplied saves remain unchanged.

Validation: zero differing bytes in all native modules, original interior
symbols resolve at their exact addresses, all 81 tests and progress/public-content/
whitespace checks pass. ROM SHA-1 remains
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Matching C/C++ is
**643,464 / 1,563,700 bytes (41.15%)**; C/C++ plus ASM is **41.49%**.


## 2026-09-09 - Field party action selection and input gates (+844 bytes)

Five byte-exact C functions now cover resetting and refreshing action selection,
setting a member action, toggling unlocked actions and dispatching field input.
The four adjacent selection routines occupy `field_party_action_selection.c`
(0x0209CE00..0x0209D024); `field_party_input.c` starts the next still-incomplete
contiguous region at 0x0209D12C. Shared declarations and existing callers use
descriptive names. The neighboring indicator configuration helper remains
private because three instruction scheduling differences remain. No inline
assembly is counted in this gain.

The reconstruction preserves native selection tables, VM unlock checks, the
piggyback action override and action 9's save-byte variant. Unknown state bits
retain neutral names. Room state, room identity, transition and party-active
checks gate input; changing actions masks member buttons from the pressed set.

Runtime reports are under `build/runtime/eur_field_party_actions/`:
- `evidence_actions83.json`: checkpoint 83, state SHA-1
  `590c4bc2877e1f1781faf521ca2a8b87ea1f9258`, 396 ordinary-input frames. The
  original save SHA-1 is `2cb577d3008975c390a2f00e2b2cd646e4005c1b`. There are
  478 checked input returns, including 32 suppressed-input cases during room
  transition. This area's state blocks action toggles.
- `evidence_actions65.json`: checkpoint 65, state SHA-1
  `ece238ed785fda646a77cde5e886aa7da3e009d7`, original save SHA-1
  `0844b75810855bc3a738122b29382ed5a6c9f983`, 276 ordinary-input frames. R/L/R
  changes both members' actions three times, including unlock variables 8194
  and 8195. Action-table results 0, 3 and 4, button masking, refresh and reset
  are observed. All five new functions execute; the final A input opens the
  Thwomp Volcano save-book menu. No save command is confirmed.

Across both runs, 1,410 SP-matched returns pass full code guards, exact helper
arguments and independent checks of complete party/area/leader records where
used. External helper effects are refreshed at return and are not claimed as
independently reconstructed. Piggyback overrides, locked unlock variables and
save-byte action variants remain static-only. OBJ VRAM and palettes are dumped
and hashed. Neither run changes game RAM; all 104 original saves retain their
hashes.

Validation: zero native relink differences, canonical ROM SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`, all 81 tests, progress, public-content
and whitespace checks pass. Matching C/C++ is **644,308 / 1,563,700 bytes
(41.20%)**; C/C++ plus ASM is **41.54%**.


## 2026-09-09 - Boss projectile loading and positioning (+780 bytes)

`Overlay25Enemy_LoadProjectiles` (224 bytes) and
`Overlay25Enemy_PositionLoadedProjectiles` (556 bytes) are byte-exact C++.
They are merged with the adjacent WaitReturn into the contiguous
`enemy_projectile_preparation.cpp`, 0x020C5EA8..0x020C61E0. Typed actor/resource,
scene/model and work layouts express the load flags, Q8 damage scale, paired
slots, zero starting scales, positions and accelerated motion. The shared
attack table contains actor IDs in this sequence; it is now explicitly named
PROJECTILE_TARGETS at these accesses while other sequences retain their
existing count interpretation. The integer distance helper reads the DS square
root unit once and passes that result as both motion distance parameters.

Runtime report: `build/runtime/eur_ov25_projectile_prepare/evidence_prepare103.json`.
Original save 103 SHA-1 `c264e8a8b26cb4994da8a93b164979377b7b4090`; giant boss
phase state SHA-1 `f83e3507a0e81de426d54c5da1d4593a365d5f8c`. The 600 frames
use B/wait inputs and two explicit sequence-entry fixtures at frame 320:
the first callback changes from 0x020CBE28 to 0x020C60D4, and the shared six-word
table at 0x020D3A3C changes from [85,4,0,0,0,0] to [56,57,0,0,0,0]. The latter
supplies valid Mario/Luigi target IDs for this initializer. The previous
callback uses the shared storage differently; using its stale values yielded
a null scene-object lookup. No executable code, object positions or models are
patched. The 24-byte table is written once; only its first two words differ.

This resolves the prerequisite behind the earlier launch probe's motion-gate
bypass. With valid target IDs, the native motion gate completes after 58 calls
and proceeds through BeginProjectileEffects, WaitProjectileEffectAnimation and
ReleaseProjectileEffects without an intermediate callback substitution. This
is controlled sequence coverage, not proof of a naturally selected complete
attack or battle victory.

There are 149 byte-guarded, SP-matched checked returns: Load once, Position four
times (three resource waits), Begin once, WaitAnimation 22 and Release 121.
Complete task/work/resource/actor/scene/model records agree at modeled
boundaries; all helper arguments agree. Two starting positions and square-root
distances, five view projections and three subsequent positions are checked
independently. Helper side effects are refreshed at return. Slots 0/1 are active
and 2..5 are empty in this probe. The final screenshot shows the ongoing giant
boss attack. BG VRAM and palettes are dumped and hashed; all 104 supplied saves
remain unchanged. No full attack completion is claimed.

Validation: zero native relink differences, canonical ROM SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`, all 81 tests and progress,
public-content and whitespace checks pass. Matching C/C++ is **645,088 /
1,563,700 bytes (41.25%)**; C/C++ plus ASM is **41.59%**. A nearby travel starter
still differs in four register-allocation words and remains private.


## 2026-09-09 - Field window sprite preparation (+512 bytes)

FieldArea_LoadWindowSprites (0x02079B74..0x02079D74) is now byte-exact C in
src/field/field_window_sprites.c. It derives the allocation size from the
sprite-file tile counts and the selected screen's OBJ boundary, allocates and
loads that screen's animator, selects palette 15 and resets the window origin.
Shared declarations and overlay linking use the descriptive name.

Runtime: build/runtime/eur_field_window_sprites/evidence_cold65.json records
2,033 ordinary-input frames from checkpoint 65 cold boot. Both screens execute
the loader: boundary shift 2 selects 64 tiles on each. Full code guards,
SP-matched returns, complete area/manager/animator records, independent tile
count calculations and exact helper arguments pass. Effects of external helpers
are refreshed on return; uploaded tile bytes are not independently modelled.
The final image shows the ordinary Thwomp Volcano field. OBJ VRAM and palettes
are dumped and hashed. There are no RAM fixtures; all 104 original saves retain
their hashes. Checkpoint 65 SHA-1 is 0844b75810855bc3a738122b29382ed5a6c9f983.

Validation: all 81 tests, zero native relink differences and canonical ROM SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701. Matching C/C++ is
645,600 / 1,563,700 bytes (41.29%); C/C++ plus ASM is 41.62%.


## 2026-09-10 - Message-window slide and hardware clipping (+1040 bytes)

Four byte-exact C routines cover slide advancement, starting a slide by speed,
computing its hardware clip rectangle and enabling/disabling window 0. Their
contiguous ranges are 0x0206F288..0x0206F378 and 0x0206F448..0x0206F768, in
field_message_window_slide.c and field_message_window_clip.c. A checked 36-byte
FieldMessageWindowSlide names the confirmed state at field offset 0x249C;
the field snapshot copier and VM callers use the shared type/declarations.
The timed initializer and completion checker remain assembly: their private
drafts still have compiler differences. No inline assembly is included in the gain.

The clip routine preserves signed halfword window indexing and edge arithmetic,
fixed-point truncation, independent 0..255/0..192 limits and the original
WININ/WINOUT/WIN0H/WIN0V writes on both display engines. The five layer bits and
color-effect bit retain their original separate operations.

Runtime evidence: build/runtime/eur_field_window_scroll/.
- evidence_scroll65.json: 320 ordinary-input frames, 90 inactive update returns.
- evidence_clip65.json: 301 frames, 682 checked returns across all four routines.
  Four isolated decoded-VM commands start and later disable a slide on each
  screen. Each 72-byte command and its original 4-byte cursor are restored at
  dispatcher return. Native code initializes velocities and advances/completes
  the movement: 72 moving updates, 74 independently computed rectangles,
  68 clamped rectangles, and both enable states are checked.

This controlled test uses the existing, currently empty slot 0 window records,
including their native default dimensions. It establishes arithmetic, helper
arguments, state writes and register packing, not a naturally selected story
dialogue or visible scrolling text. Complete area, manager, selected window and
display registers pass at each relevant helper boundary and SP-matched return.
External helper effects are refreshed at return. The final image shows the
ordinary Thwomp Volcano field after clipping is disabled. OBJ VRAM and palettes
are dumped and hashed; all 104 original saves remain unchanged.
State SHA-1: 9f9bbb7689b7c4322ef073d34d4c86b14e59bdff.
Checkpoint 65 SHA-1: 0844b75810855bc3a738122b29382ed5a6c9f983.

Validation: all 81 tests, zero native relink differences, canonical ROM SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701, progress and public-content checks pass.
Matching C/C++ is 646,640 / 1,563,700 bytes (41.35%); C/C++ plus ASM is 41.69%.


## 2026-09-10 - Baby Spin anchor following and orbit (+864 bytes)

The three routines at 0x0208BDC8..0x0208C128 now match in C++ and extend the
existing contiguous field_party_baby_spin.cpp module. They request following
an existing field entity, initialize the orbit angle and advance a radius-eight
orbit from the native sine/cosine table. FieldPartyAnchorOrbit provides a checked
32-byte alternate view of the party state record. Shared prototypes replace
raw names in the party manager and the existing state-74 restore path.

Runtime evidence: build/runtime/eur_field_baby_spin_anchor/evidence_anchor83.json.
The source is checkpoint 83's transitions_action_mode83.dst, SHA-1
3f4ab4244cfc7c6c8521ff54ebf7fdd096b92c61; original save SHA-1 is
2cb577d3008975c390a2f00e2b2cd646e4005c1b. The 551-frame ordinary-input sequence
B/wait/B/wait/left/wait/right/wait starts Baby Spin. At frame 89, while the
leader is in state 61, one controlled decoded C3(1, 0) command selects the
nearest existing eligible field entity (area 0x0220788C, target 0x0220CA60).
The original 72-byte command and four-byte cursor are restored at dispatcher
return. No party state, position, resource or target record is directly changed.
This is isolated anchor coverage, not a naturally selected story anchor.

Native following proceeds into both members' orbit initializers and 322 orbit
updates. All 325 SP-matched returns pass complete party/member/area/target/state
records, full native code guards and exact helper arguments. Independent
sine-table calculations check displacements in all four quadrants. The atan2
helper result and other external-helper effects are accepted at their return;
they are not claimed as independently reconstructed here. The state-61 path
is covered; height acceptance/rejection in states 62/63/75 remains static-only.
The final image is the ordinary Star Hill field. OBJ VRAM and palettes are
dumped and hashed, and all 104 original battery saves retain their hashes.

Validation: all 81 tests, zero native relink differences and canonical ROM SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701. Matching C/C++ is
647,504 / 1,563,700 bytes (41.41%); C/C++ plus ASM is 41.74%.


## 2026-09-10 - Reflected boss-projectile impact (+792 bytes)

Overlay25Projectile_UpdateReflectedImpact (0x020C44B0..0x020C47C8) is byte-exact
C++. It extends the adjacent initialization/spin module, now named
src/overlay025/enemy_projectile_impact.cpp (0x020C4268..0x020C47C8). The routine
emits trails, spins the returned projectile, waits for its movement to finish,
calculates/applies enemy damage, emits impact effects and starts the final
ballistic bounce. Its mode-2 variant offsets the damage popup by -32 pixels.
Native disassembly recovers the damage and motion arguments omitted by the
decompiler, including the returned ballistic duration used by the X channel.

Runtime: build/runtime/eur_ov25_projectile_impact/evidence_reflect103.json.
Source prepare103.dst SHA-1 c2c8c46bf017c3f5a9b048ecfbf536dc68170d17 already
contains the documented two-target preparation fixture from the preceding
projectile work. Checkpoint 103 SHA-1 is
c264e8a8b26cb4994da8a93b164979377b7b4090. A 1,501-frame run continues that attack.
At frame 150, one empty 20-byte hit-record sentinel is temporarily populated
with contact source 56, target 44 and the live projectile coordinates. Native
collision handling consumes the record, stops the old motion, starts the
return flight and selects the reconstructed callback; all 20 original bytes
are restored at handler return. This is isolated reflection coverage, not a
timed player dodge or naturally selected attack.

There are 22 checked returns from the new routine and 111 from the adjacent
spin callback. The run covers two trail emissions, the mode-0 impact at frame
172, damage-helper argument 32, four independently projected positions, the
24-unit vertical adjustment, the ballistic/move arguments and two completed
spin tasks. Complete task/work/scene/model records and native code guards pass.
External damage and motion helpers are refreshed at return; this probe does
not independently establish the HP formula. Mode 2 remains static-only.
Subsequent native boss attacks proceed to the next player command menu, shown
in the inspected final image. BG VRAM and palettes are dumped and hashed.
All 104 supplied saves retain their hashes.

Validation: all 81 tests, zero native relink differences, canonical ROM SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701 and public-content checks pass.
Matching C/C++ is 648,296 / 1,563,700 bytes (41.46%); C/C++ plus ASM is 41.79%.


## 2026-09-10 - Boss chain preparation (+1,024 bytes)

Overlay25Enemy_InitializeSegmentChains (0x020C6C4C, 692 bytes) and
Overlay25Chain_BeginTracking (0x020C93D0, 332 bytes) are byte-exact C++.
They extend the adjacent modules enemy_segment_chain_preparation.cpp and
enemy_chain_tracking.cpp. Shared checked layouts describe the 836-byte chain
state, its 12-byte joints and the 32-byte motion parameters. Preparation clears
the second four chain states, binds scene objects and initializes collision
status; tracking initializes fixed-point coordinates, binds the resource and
snapshots the initial position before selecting the native movement callback.

Runtime evidence: build/runtime/eur_ov25_chain_preparation/
evidence_segments103.json and evidence_tracking103.json. Both probes compare
complete task/work/scene/model/resource/hit records and external helper
arguments. Helper side effects are refreshed at return. New independent
checks include the 3,344 cleared bytes, initial positions and position snapshots.

The 1,501-frame segment run starts from prepare103.dst (SHA-1
c2c8c46bf017c3f5a9b048ecfbf536dc68170d17), including its documented target
preparation fixture. It repeats the previous temporary 20-byte reflected-hit
fixture and reaches chain initialization through subsequent native attacks.
There are 22 checked initialization returns, two initialized scene objects
(40/41), and 133 checked returns from the preceding projectile callbacks.

The 970-frame tracking run starts from phase103.dst (SHA-1
f83e3507a0e81de426d54c5da1d4593a365d5f8c). At frame 320 a controlled fixture
changes the shared attack-table contact count from 85 to 1 and selects
Overlay25Enemy_BeginModelEffect in task 0. Each change is four bytes. Native
preparation selects chain indices 2/3 and reaches two checked BeginTracking
returns. Subsequent native tracking, grab and unlock callbacks execute. This
establishes isolated setup coverage, not natural attack selection or independent
verification of the entire attack. The JSON template's generic fixture label
does not describe the count's tracking-specific role; the actual edits are
recorded there and described here.

Both final images were inspected: the segment run reaches the player's Jump
menu and the tracking run shows the active boss encounter. BG VRAM and palettes
are dumped and hashed. All 104 supplied save hashes remain unchanged; source
checkpoint 103 SHA-1 is c264e8a8b26cb4994da8a93b164979377b7b4090.

Validation: all 81 tests, zero native relink differences, canonical ROM SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701 and public-content checks pass.
Matching C/C++ is 649,320 / 1,563,700 bytes (41.52%); C/C++ plus ASM is 41.86%.


## 2026-09-10 - Boss chain target selection (+584 bytes)

Overlay25Enemy_SelectTrackingTargets (0x020C9580..0x020C97C8) is byte-exact
C++. It joins the existing setup and tracking routines in the contiguous
enemy_chain_tracking.cpp module. Its checked 32-byte parameter view names the
target ID and contact count. The routine waits for the attached effect, chooses
one or two eligible characters, accounts for alternate formation, assigns
scene IDs 40/41 and starts the corresponding chain tasks.

Runtime: build/runtime/eur_ov25_chain_selection/evidence_select103.json.
The 970-frame run reuses the prior phase103.dst and the explicitly recorded
four-byte contact-count/four-byte attack-callback fixture. No additional state
changes are needed. It checks 31 selection returns (30 waiting and one two-target
selection), two subsequent tracking setup returns, complete task/work/scene
records and exact eligibility-helper arguments. External eligibility results
are observed; their internal formula is not independently reimplemented here.
Single-character/alternate-formation selection remains static-only.
Native following/grab/unlock routines continue and the inspected final image
shows the active boss encounter. BG VRAM and palettes are dumped and hashed;
all 104 original save hashes remain unchanged.

Validation: all 81 tests, zero native relink differences, canonical ROM SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701 and public-content checks pass.
Matching C/C++ is 649,904 / 1,563,700 bytes (41.56%); C/C++ plus ASM is 41.90%.


## 2026-09-10 - Launch of the boss linked effect (+632 bytes)

Overlay25Enemy_LaunchLinkedEffect (0x020CAD04..0x020CAF7C) is byte-exact C++.
It closes the gap between the existing routines in enemy_linked_effects.cpp,
which now also contains the adjacent BeginNextAnimation callback. Native
disassembly establishes all ten arguments for each of the two impact emitters,
including mode 3 and the distinct 16/8 dimensions omitted by the decompiler.
The routine waits for the attached effect, starts the distance-based flight,
emits a sprite and an enemy-relative model effect, and advances the callback.

Runtime: build/runtime/eur_ov25_linked_launch/evidence_launch103.json. The
970-frame phase103.dst run repeats the documented tracking attack selection
fixture; the later native linked-effect attack needs no additional fixture.
There are 121 checked launch returns (120 waiting, one launch), alongside 33
checked selection/tracking returns. Independent checks reproduce two projected
positions and the hardware square root used for motion; exact helper arguments
and complete task/work/scene records agree at calls and returns. External
motion/effect helper side effects are refreshed, rather than claimed as an
independent model of those subsystems. The subsequent native attack continues.
The inspected final image shows the boss encounter. BG VRAM/palettes are dumped
and hashed, and all 104 supplied saves retain their hashes.

Validation: all 81 tests, zero native relink differences, canonical ROM SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701 and public-content checks pass.
Matching C/C++ is 650,536 / 1,563,700 bytes (41.60%); C/C++ plus ASM is 41.94%.


## 2026-09-10 - Feeding and growing the linked boss effect (+2,160 bytes)

Overlay25Enemy_FeedLinkedEffect (0x020CA858, 1,104 bytes) and
Overlay25Enemy_UpdateLinkedEffectGrowth (0x020CA438, 1,056 bytes) are byte-exact
C++, extending the contiguous enemy_linked_effects.cpp module. Weighted counters
select an effect from either side of the boss; the next object moves toward the
existing effect, which grows at stages 1/2/4/6/9. Once the counters are exhausted,
the completed object accelerates toward the selected character and updates the
enemy's damage scale. Scene offsets, particle-stop flags, emitter parameters
and stage-dependent distances follow the native instructions.

The declaration of func_ov002_0206c148 now accepts full-width effect index and
X/Y coordinates. Native callee instructions use R0 directly as an archive index
and store all 32 bits of R2/R3 in the animation state; only stack Z is loaded as
a signed halfword. The older declaration incorrectly narrowed all these values.
The existing trail caller retains its explicit actor-parameter truncation, and
the full relink verifies that its output remains exact.

Runtime: build/runtime/eur_ov25_linked_growth/evidence_growth103.json, following
the earlier narrower evidence in eur_ov25_linked_feed/evidence_feed103.json.
The 970-frame phase103.dst run reuses the documented tracking-selection fixture.
The subsequent native linked attack needs no additional RAM edits. The new
callbacks have 50 and 149 checked returns, respectively. Eight feed operations
cover both sides (two/six), stages 0..7 and five distance margins. Growth covers
stages 1..8, four size changes, positive/expired timers, seven repeated feeds
and the final launch at the target. Stage 9 and the final feed-distance branch
remain static-only. Random-helper results are observed without rewriting RNG.

Independent checks reproduce eight feed positions, 24 projected positions,
nine hardware-square-root results and four growth offsets. Complete task/work/
scene/model/counter records, damage-scale writes and exact helper arguments
agree at calls and returns. External motion/effect helper side effects are
refreshed; these checks do not independently prove the entire attack or its
damage application. The final frame has the same inspected battle image as
the preceding probe. BG VRAM and palettes are dumped and hashed. All 104
supplied saves retain their hashes.

Validation: all 81 tests, zero native relink differences, canonical ROM SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701 and public-content checks pass.
Matching C/C++ is 652,696 / 1,563,700 bytes (41.74%); C/C++ plus ASM is 42.08%.


## 2026-09-10 - Boss attack landing and reflected damage (+1,104 bytes)

Three byte-exact C++ callbacks complete damage application for two boss attacks:
Overlay25Chain_ApplyLandingDamage (0x020C8634, 328 bytes),
Overlay25LinkedEffect_ApplyPartyLandingDamage (0x020C98D8, 368 bytes) and
Overlay25LinkedEffect_ApplyReflectedDamage (0x020C9A48, 408 bytes).
The first extends the adjacent unlock routine in enemy_chain_landing.cpp;
the latter two form the contiguous enemy_linked_damage.cpp module. Native
instructions recover the complete damage/effect arguments and the distinct
5/10 status magnitude and chance parameters. Landing clears the party lock and
status-model suppression flags; the reflected branch applies equipment-adjusted
damage to the boss, stops particles and clears the effect slots.

Runtime: build/runtime/eur_ov25_attack_damage/evidence_damage103.json and
build/runtime/eur_ov25_linked_reflection/evidence_reflected103.json. Both are
1,470-frame runs from phase103.dst with the documented initial tracking attack
fixture. The first reaches two actual chain landings (26 checked returns) and
one linked-effect party landing (112 checked returns), including status-helper
arguments and clearing the party flags/effect slots. The reflected run checks
54 returns, including an actual enemy-damage application and particle cleanup.

Reflection coverage adds two explicitly controlled contacts at frames 962 and
1,042. At the native collision timer gate, each fills an empty 20-byte hit-record
sentinel with the current target character (57 then 56), object 40 and its live
coordinates. Native handling consumes the record, alternates the target and
starts the bounce/return flight. All 20 original bytes are restored at each
SP-matched handler return. No callback or position is directly changed for
these reflections. This is isolated contact coverage, not a player-timed dodge.

Complete task/work/scene/actor records, helper arguments and independently
projected positions agree. Damage/status helpers are observed and refreshed at
return; their probability and HP formulas are not independently established by
these probes. Both runs continue into the following native segment attack,
visible in the inspected final images. BG VRAM/palettes are dumped and hashed;
all 104 original saves retain their hashes.

Validation: all 81 tests, zero native relink differences, canonical ROM SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701 and public-content checks pass.
Matching C/C++ is 653,800 / 1,563,700 bytes (41.81%); C/C++ plus ASM is 42.15%.


## 2026-09-10 - Enemy removal and vertical interpolation (+420 bytes)

Fresh compilation identifies three exact routines in previously unlinked C:
BattleEnemy_UpdateDelayedDefeatRemoval (0x020A9478, 120 bytes),
BattleEnemy_UpdateDefeatRemoval (0x020A94F0, 220 bytes) and
BattleSceneObject_UpdateVerticalMotion (0x020A47EC, 80 bytes).
The removal callbacks now share battle_enemy_removal.c; their checked payload
and task layouts move to battle_enemy_defeat.h. The vertical callback extends
the adjacent battle_scene_movement.c module and its checked 12-byte parameter
layout moves to battle_scene.h. The unfinished surrounding functions remain
unlinked and do not count toward matching C progress.

Runtime: build/runtime/eur_battle_removal_motion/evidence_removal103.json.
From phase103.dst, one two-byte fixture changes the live boss HP at 0x020D2066
from 2,000 to 1. Ordinary B inputs perform the attack; native battle code
reaches one normal removal and five completed delayed removals, with 151 total
removal returns. The model checks HP writes, timers, context flags, model
detachment/reset and task termination. All 69 vertical-motion returns match
independent polynomial calculations, including three final-position writes.
Complete records and exact helper arguments agree; external helper side effects
are refreshed at return. The normal-removal null-model branch is static-only.

The 1,170-frame run ends on a black screen. A further 911 frames with wait/A
(evidence_after_removal103.json) do not establish a story transition. Therefore
this is callback/attack coverage in the derived boss state, not verification of
natural boss completion or the following story scene. The limitation is kept
explicit; the ROM is still byte-identical to the original. BG VRAM/palettes are
dumped and hashed and all 104 original saves retain their hashes.

Validation: all 81 tests and zero native relink differences pass. A trailing
blank line reported by the final whitespace check was removed; diff checking
then passes. Canonical ROM SHA-1 is ba4ec2f99b4f2e0047601552bccf00aa73e28701;
public-content checks pass. Matching C/C++ is 654,220 / 1,563,700 bytes (41.84%);
C/C++ plus ASM is 42.17%.


## 2026-09-10 - Battle selection, resource queue and capture copy (+404 bytes)

Freshly compiled C reproduces five formerly unlinked routines exactly:
BattleObjectData_QueueLoad (0x02089EEC, 64 bytes), BattleTargetCursor_Update
(0x02098B08, 60), BattleTargetCursor_TriggerBounce (0x02098B44, 76),
BattleCommandWheel_TriggerEntryBounce (0x02099508, 144) and
BattleCaptureSurface_CopyTask (0x020AD058, 60). Exact contiguous ranges are
extracted from the unfinished surrounding modules. Checked command-wheel,
cursor and copy-task layouts now live in shared headers.

Runtime: build/runtime/eur_battle_remaining_helpers/evidence_ui103.json,
evidence_cursor83.json and evidence_entry83.json. The first two use ordinary
buttons in existing boss states (770 and 270 frames); the third enters a battle
with the documented temporary decoded command 0x11C fixture (601 frames).
All 1,570 cursor returns, two new wheel bounces and 31 resource enqueues match
complete record models and exact helper arguments, including queue results.
Visible/reset and hidden cursors are covered; rotating cursors, restarted or
dismissed wheel entries, TriggerBounce and CopyTask remain static-only. No
runtime claim is made for the independent copy-byte oracle, which was not hit.
Screenshots for ui103 and the source cursor encounter were inspected. BG VRAM
and palettes were saved and hashed; all 104 supplied saves retain their hashes.

Validation: all 81 tests, public-content audit, whitespace checks and zero
native relink differences pass. Canonical ROM SHA-1 remains
ba4ec2f99b4f2e0047601552bccf00aa73e28701. Matching C/C++ is
654,624 / 1,563,700 bytes (41.86%); C/C++ plus ASM is 42.20%.


## 2026-09-10 - Title panels, trails and moving-sprite lifecycle (+1,732 bytes)

Two contiguous modules now compile exactly: title_panel_motion.c covers
0x0206DB4C..0x0206E09C (1,360 bytes) and title_moving_sprite_lifecycle.c covers
0x0206E644..0x0206E7B8 (372 bytes). Fourteen routines handle the common element
list, panel initialization, pulse/fade states, randomized trail starts,
sine-based movement, overlapping trail copies, and moving-sprite setup/entry/
orbit/destruction. Shared checked layouts include the 4-byte common list link,
120-byte panel, 8-byte trail sample, 72-byte texture/palette resource set and
68-byte moving sprite. Unknown fields retain neutral names.

Native register data flow explains the remaining matching differences: branch
body order follows the original state machine; x interpolation computes its
start and delta before the sine lookup; moving-sprite entry first loads the
base coordinate and then adds its signed displacement. No compiler-variant
search or inline assembly is used.

Runtime: build/runtime/eur_title_panel_motion/evidence_boot83.json and
evidence_complete83.json. Both import original checkpoint 83 into a fresh
emulator; there are no RAM fixtures. The 2,243-frame start/button run reaches
the normal load menu. The 6,001-frame idle run exercises the animated title
sequence, all panel states 0..5, both sprite sides/variants, and a native title
sequence restart. The screenshots were inspected. All 43,552 panel update
returns, 8,178 trail-start attempts and eight sprite initializations agree.
Independent checks cover 680 sine-alpha calculations, 190 x/y interpolations,
480 cleared trail bytes and 9,600 bytes copied in both overlap directions.
Ten randomized trails start (six in one direction, four in the other), drain
through 50 shifts, and terminate normally. Exact helper arguments, full records
and return values are checked; external helper side effects are refreshed.
The common list append has 118 returns, including two empty lists; the probe's
earlier name TitlePanel_Append was subsequently refined to TitleElement_Append
because native callers append several element types. The StartPulse forwarding
entry remains static-only; its ResetPulse target is exercised 96 times.

All 104 supplied saves retain their hashes. BG VRAM and palettes are dumped and
hashed. All 81 tests, public-content and whitespace checks pass; native relink
has zero differences and ROM SHA-1 remains
ba4ec2f99b4f2e0047601552bccf00aa73e28701. Matching C/C++ is
656,356 / 1,563,700 bytes (41.97%); C/C++ plus ASM is 42.31%.


## 2026-09-10 - Title sprite rendering, orbit reset and panel release (+936 bytes)

TitleMovingSprite_Draw (0x0206E7B8, 556 bytes) extends the adjacent lifecycle
module, now named title_moving_sprite.c. TitleMovingSprite_ResetOrbit
(0x0206EC20, 300 bytes) and TitlePanelResources_Release (0x0206E388, 80 bytes)
occupy their separate exact ranges while the intervening functions remain
unfinished. All three are readable C with no assembly. The 48-byte child view
is checked and shared; moving-sprite offset 0x38 is now named angle, confirmed
by native trigonometric lookup and rotation data flow. Original switches and
repeated child-pointer loads are preserved rather than collapsing state checks
or caching an alias across writes.

Runtime: build/runtime/eur_title_panel_motion/evidence_draw83.json and
evidence_release83.json, from ordinary cold boots with checkpoint 83 and no RAM
fixtures. The first run waits 3,600 frames, presses Start and reaches the load
menu (4,233 frames total). All 13,324 draw returns agree across states 0..3;
9,848 child orbit positions and 4,736 temporary parent render offsets are
independently checked. Rotation resets, depth, exact draw/animation helper
arguments, complete 68-byte work, 48-byte child and 440-byte model records agree.
All 29 orbit resets cover all four side/variant combinations: fourteen choose
a random animation and fifteen read the paired model's animation through its
actual virtual callback. Helper side effects are refreshed at return; this
does not independently reimplement the external renderer or resource loader.

The 1,533-frame release run reaches one normal panel-resource release: both
owned buffers are freed, their pointers are cleared, and texture/palette lists
are unlinked using the exact native helper sequence. Null-buffer release
branches remain static-only. Title screenshots were inspected; the load-menu
BG VRAM/palette hashes agree with the previously inspected normal boot. All
104 supplied saves are unchanged and display-memory dumps are recorded.

All 81 tests, whitespace and public-content checks pass. Native relink has zero
differences; canonical ROM SHA-1 is ba4ec2f99b4f2e0047601552bccf00aa73e28701.
Matching C/C++ is 657,292 / 1,563,700 bytes (42.03%); C/C++ plus ASM is 42.37%.


## 2026-09-10 - Battle particle sweep constructors (+368 bytes)

BattleParticleSweep_CreateEmitter (0x020AF97C, 244 bytes) and
BattleParticleSweep_CreateParticle (0x020AF57C, 124 bytes) are matching C.
The shared header checks the 16/12-byte parameter and 28/24-byte task layouts.
The battle VM uses the named emitter entry point for its two sweep commands.
Native rendering data flow identifies the five-bit field as polygon alpha;
the high bit records whether particles remain active. The surrounding emitter
update and particle renderer remain unfinished and are not counted here.

Runtime: build/runtime/eur_battle_particle_sweep/evidence_sweep103.json records
571 frames from the existing checkpoint-103-derived boss fixture. One decoded
battle VM command 0xA7 starts an emitter at the live camera-relative origin,
with arguments [128, 136, 512, 0, 16912, 24, 0]. After native dispatch, all
244 replaced command/state bytes are restored and the original command is
confirmed to replay. The native emitter then creates all 63 particles.
This is controlled effect coverage, not a naturally scripted attack.

All 64 constructors agree with the independent record model: exact task-pool
insertion arguments, complete allocated 28/24-byte records and returned
pointers. Particle sizes 5 through 14 occur. Emitter update/render mathematics
are not independently covered by this constructor check. Screenshots at
frames 40 and 571 were inspected: the particle fan is visible and the battle
continues. BG VRAM and palettes are dumped and hashed; all 104 supplied saves
are unchanged.

All 81 tests, whitespace and public-content checks pass. Native relink has zero
differences; canonical ROM SHA-1 is ba4ec2f99b4f2e0047601552bccf00aa73e28701.
Matching C/C++ is 657,660 / 1,563,700 bytes (42.06%); C/C++ plus ASM is 42.39%.
Work stops after this block at the user's request; the 50% milestone remains
unfinished.


## 2026-09-10 - Japanese title sprite sequence and movement setup (+2432 bytes)

Eighteen functions are matching readable C in three contiguous ranges:
0x0206ED4C..0x0206F3F0 (sequence lifecycle/update/draw dispatch and fade setup),
0x0206F4DC..0x0206F5CC (fade update and rotating-sprite initialization), and
0x0206FBA4..0x0206FD90 (movement setup, state predicates and sprite initialization).
The shared header checks the 40-byte common header, 1364-byte actor view,
108/112/52-byte sprites and 1928-byte sequence. Unknown fields remain explicit.
All helper arguments were recovered from native instructions, including
depths, arc parameters and positions omitted by the decompiler. The two
intervening render functions remain native and are not counted as C.

Runtime evidence is in build/runtime/eur_title_sprite_sequence. Ordinary cold
boot with save 83 reaches the English load menu in 1833 frames (boot83),
checking the sequence release and its two texture releases plus actor release.
Native title initialization selects the reconstructed sequence only when the
archive language byte is zero. The Japanese runs set this one byte at
0x0206C298, after copying the language and before loading title resources.
Each JSON records the address, original byte and frame. This is a controlled
language fixture confined to the private emulator session, not an ordinary
English-language game path; the live save context and supplied saves are not
modified.

The 1801-frame japanese83 run completes the title animation. All used sequence
states 0,1,2,3,4,5,7,8,9 occur: 1513 sequence/fade updates and 1512 draw-dispatch
returns agree. All three predicates cover both Boolean outcomes (227 returns).
Two arc starts cover states 1 and 3, followed by vertical movement, squash,
and the 32-frame final fade. The 31 division and alpha interpolation results,
vertical target and squash origin are independently checked. The 1293-frame
skip83 run skips the opening with A, then presses Start and reaches the load
menu, adding 742 sequence/fade updates, 742 draw dispatches, Show, Finish and
Release. Combined runs observe all eighteen newly linked entry points.

Complete 1928/112/108/52-byte records and exact helper/callback arguments are
checked before helper calls and at SP-matched returns. External renderer,
resource and child-update effects are refreshed after their native returns;
this probe does not independently reimplement their behavior. Japanese title
and both load-menu screenshots were inspected. BG VRAM/palette dumps and
hashes are recorded. All 104 supplied save hashes remain unchanged.

All 81 tests, whitespace and public-content checks pass. Native relink has zero
differences; canonical ROM SHA-1 is ba4ec2f99b4f2e0047601552bccf00aa73e28701.
Matching C/C++ is 660,092 / 1,563,700 bytes (42.21%); C/C++ plus ASM is 42.54%.


## 2026-09-10 - Title sprite rendering and animation lifecycle (+2856 bytes)

Fourteen further matching C functions complete the fade, rotating-sprite and
sequence-sprite draw paths and reconstruct the actor's initialization, update,
draw, start, stop, state predicate and release, plus four sprite movement
setters. The checked actor layout now identifies its texture, two command
streams, two track pointers, animation controller, four 48-byte track records,
1024-byte animation buffer and pulse/fade fields. A checked 16-byte layout view
names the independently consumed height; other unconfirmed entries remain
unknown.

Explicit inline Translate/Scale helpers preserve the native evaluation of all
coordinates before FIFO writes. The nullable animation-deletion helper retains
the original outer ownership check and inner deletion guard. No assembly is
needed. The former title_sprite_callbacks.c is merged into the now-contiguous
title_sprite_sequence.c (0x0206ED4C..0x0206F710); title_sprite_movement.c extends
through 0x0206FEC4, and title_sprite_animation.c covers
0x020703E0..0x02070BA4. The two remaining large sprite-update routines are still
native and excluded from matching C coverage.

Runtime evidence: build/runtime/eur_title_sprite_sequence/evidence_render83.json,
evidence_render_skip83.json and evidence_render_english83.json. The first two
use the documented one-byte archive-language fixture for Japanese title
resources; the last is an ordinary English boot. All original saves and the
live save context are untouched. Full animation: 2083 frames, followed by
Start and the load menu; 1532 actor updates/draws and 3064 sprite draws agree.
All four actor states occur. Its 18 fade steps, 60 pulse updates and division
results are independently checked, along with the two animation-track starts
and their speeds. All thirteen new functions used by this path agree.

Late skip: 1403 frames, A after frame 760 and then Start. This additionally
checks Stop with both live track command pointers cleared, completing coverage
of all fourteen new functions. Across both runs, all 29,998 graphics register
writes in the four linked draw routines agree in exact order, including
matrix mode/identity/push/pop, color, translation and scale. This covers 1904
translations, 634 scales and 113 independently checked 4x4 rotation matrices.
The full 1928/1364/112/108/52-byte records and track contents are checked around
helpers and at SP-matched returns. External unlinked rendering and resource
helpers are checked for arguments but not independently reimplemented.

Both allocated animation/command release branches occur in the Japanese runs.
The 1783-frame ordinary English boot checks the null animation and null command
release branches. Title-animation screenshots at frames 761 and 1801 were
inspected; all three final display-memory hashes agree with the previously
inspected English load menu. BG VRAM/palette dumps are recorded and all 104
supplied saves are unchanged.

All 81 tests, whitespace and public-content checks pass. Native relink has zero
differences, including the ARM-to-Thumb matrix call; canonical ROM SHA-1 is
ba4ec2f99b4f2e0047601552bccf00aa73e28701. Matching C/C++ is
662,948 / 1,563,700 bytes (42.40%); C/C++ plus ASM is 42.73%.


## 2026-09-10 - Complete title sprite motion controllers (+2480 bytes)

TitleSequenceSprite_Update (0x0206FEC4, 1308 bytes) and
TitleRotatingSprite_Update (0x0206F710, 1172 bytes) are matching readable C.
The formerly separate sequence, movement and actor modules are merged into
title_sprite_sequence.c: all 34 functions in the contiguous
0x0206ED4C..0x02070BA4 region, 7768 bytes, are now C. This completes this
Japanese title-animation region, not the whole title overlay. Explicit idle
cases preserve the native dispatch table; negation precedes multiplication
where required by native fixed-point arithmetic. Adjacent arc-math helpers
remain outside this range and are not counted here.

Runtime: build/runtime/eur_title_sprite_sequence/evidence_motion83.json uses
the same documented archive-language fixture and 2083-frame boot/Start/load
sequence. All 3064 sprite-update returns and 1532 rotating-update returns agree,
covering sprite states 0,1,2,3,4,5,6,16 and rotating states 0,1,2,3,4,8,16.
Independent checks cover 130 arc heights and X positions, 46 interpolated
positions, 114 squash/height compensations, 29 rotation-angle calculations,
368 divisions and 38 transitions to the final drawing depth. The original
integer truncation and signed 32-bit arithmetic are preserved. Both arc-start
parameter sets and the chained movement/squash callbacks agree.

All surrounding lifecycle and draw probes remain enabled, including the
22,312 ordered graphics-register writes and 113 checked rotation matrices in
this run. Full sprite/actor/sequence records agree around helpers and at
SP-matched returns. State 16 is observed with immediate disappearance; its
positive-duration interpolation branch remains static-only. Arc-height helper
results are independently checked, while the separate hardware-square-root
coefficient helper remains checked for inputs and refreshed after return.

The final BG VRAM/palette hashes match the previously inspected load menu.
All 104 original saves are unchanged. All 81 tests, whitespace and public
content checks pass. Native relink has zero differences; canonical ROM SHA-1
is ba4ec2f99b4f2e0047601552bccf00aa73e28701. Matching C/C++ is
665,428 / 1,563,700 bytes (42.55%); C/C++ plus ASM is 42.89%.


## 2026-09-10 - Localized title lifecycle and texture transfers (+724 bytes)

Four contiguous functions at 0x02070C7C..0x02070ED8 are readable matching C:
TitleLocalizedSequence_Finish, Start, Release and Init. Their checked 1644-byte
record owns five textures, two command streams and four animation tracks.
TitleTexture_Upload and TitleTexture_DrawLayout at 0x020718FC..0x02071974
add the common transfer and rectangle-dispatch helpers. TitleSpriteLayout
now names its confirmed image X/Y, width and height fields and lives with
the shared texture declarations. The temporary title_texture_draw.c range
will merge with title_texture.c when the intervening quad renderer matches.
The large localized draw/state controller remains outside the C count.

Runtime evidence: build/runtime/eur_title_localized_lifecycle/evidence_english83.json,
evidence_english_skip83.json and evidence_japanese83.json. English boot uses
save 83 without memory fixtures (1783 frames); a second run skips the active
animation after 900 frames (1343 total), clearing both live track-command
pointers. A 2083-frame run with the previously documented private archive
language fixture exercises the Japanese path and nullable localized cleanup.
Together these runs observe all six new C functions, 1460 layout dispatches,
and 13 DMA transfers. All 140544 transferred bytes match their source buffers
in live VRAM. Full lifecycle, texture, track and layout records agree around
helpers and at SP-matched returns; external helper effects are refreshed.
English allocated cleanup and Japanese null cleanup are both covered.
Allocation failure and Finish with null tracks remain static-only.

All final BG VRAM and palette hashes match the established load-menu output;
all 104 original saves are unchanged. Native relink has zero differences;
canonical ROM SHA-1 is ba4ec2f99b4f2e0047601552bccf00aa73e28701.
All 81 tests and public-content/whitespace checks pass. Matching C/C++ is
666152 / 1563700 bytes (42.60%); C/C++ plus ASM is 42.93%.


## 2026-09-10 - Complete title texture quad renderer (+536 bytes)

TitleTexture_DrawQuad at 0x02071974 is readable matching C. The native
geometry command interface is expressed through inline polygon, texture
coordinate and vertex helpers, preserving signed fixed-point narrowing and
argument evaluation. This closes the last gap in 0x020718FC..0x02071D70:
all ten texture functions, 1140 bytes, now live in title_texture.c. The
temporary title_texture_draw.c has been merged and removed.

Runtime evidence in build/runtime/eur_title_localized_lifecycle:
evidence_quad_english83.json (1783 frames) and evidence_quad_japanese83.json
(2083 frames with the documented language fixture). There are 4071 checked
DrawQuad returns, including two zero-alpha early exits. All 4069 visible
rectangles agree with independent calculations of polygon attributes,
texture and palette offsets, UV coordinates and centered vertices. All
56966 graphics-register stores are checked in order. The checks cover
explicit polygon IDs 16..21 and automatic opaque ID 0; automatic translucent
ID 8 remains static-only. Texture getters are checked independently against
the live headers and allocation records. Eight DMA uploads still agree byte
for byte with 91264 source bytes. Original save hashes are unchanged.

The English title screenshot was inspected; both final display dumps match
the established load-menu hashes. All 81 tests, native zero-difference relink,
public-content audit and whitespace checks pass. Canonical ROM SHA-1 remains
ba4ec2f99b4f2e0047601552bccf00aa73e28701. Matching C/C++ is now
666688 / 1563700 bytes (42.64%); C/C++ plus ASM is 42.97%.


## 2026-09-10 - Complete title star-panel renderer and resources (+1192 bytes)

TitlePanel_Draw (0x0206E09C, 748 bytes) and TitlePanelResources_Load
(0x0206E3D8, 444 bytes) are matching C. All 13 functions in the contiguous
0x0206DB4C..0x0206E594 range (2632 bytes) now live in title_panel.c, replacing
the separate motion/resource modules. Shared geometry helpers now live in
title_graphics_internal.h and are used by the panel, texture and sequence
modules. A small inline quad function expresses the fixed vertex coordinates
without a large rendering macro. The panel alpha field is confirmed signed
by native LDRSB accesses; all existing panel functions still match exactly.
The six trail alpha values use the native linear interpolation expression.

Runtime: build/runtime/eur_title_panel_renderer/evidence_english83.json uses
ordinary English boot with save 83, waits 1500 frames, presses Start and
continues to the load menu (1783 frames total). All 9848 panel draw returns
agree, covering states 0..5, zero/visible alpha and all five trail positions.
There are 5919 independently checked quads and translations, including 267
active trail slots and 93 skipped trail slots. All 208951 graphics-register
stores agree in order: matrices, scale, translation, texture/palette offsets,
UV coordinates, centered vertices, alpha and polygon IDs.

Resource loading independently verifies the 512-byte image copy, its 512-byte
VRAM upload and all six alpha values (24,20,16,12,8,4). The existing motion
probes also pass: 9848 updates, 57 position interpolations and recorded trail
entries, both trail directions, 175 division/alpha calculations, 2760 copied
trail bytes, 4924 moving-sprite draws, and cleanup. Alternate copy direction
and absent initial image remain static-only for the resource loader.

All 104 original saves remain unchanged; final display hashes match the
established load-menu output. Native relink has zero differences; all 81
tests and public-content/whitespace checks pass. The final inline-helper
cleanup was recompiled: all 13 functions match, native link/check passes and
canonical ROM SHA-1 is ba4ec2f99b4f2e0047601552bccf00aa73e28701.
Matching C/C++ is 667880 / 1563700 bytes (42.71%); C/C++ plus ASM is 43.04%.


## 2026-09-10 - Complete localized title animation and arc math (+2812 bytes)

TitleLocalizedSequence_Draw (0x02070ED8, 2596 bytes) now matches in C,
completing the 3200-byte localized sequence module. Its draw callback also
advances the native animation states: delayed actor entry, pulsing, the logo
arc, shrinking actor, separating subtitle halves, final fade and rotating
backdrop. Argument and fixed-point evaluation are explicit; native 32-bit
temporaries preserve multiplication and truncation without inline assembly.

The adjacent 216-byte helper range is also C: TitleSprite_EvaluateArc,
TitleSprite_CalculateArc, TitleSprite_StartDisappear and TitleSprite_Hide.
The shared sprite sequence module now contains 38 functions / 7984 bytes.
Together with the localized sequence and texture module this completes the
entire contiguous 0x0206ED4C..0x02071D70 region (12324 bytes). This is the
title-animation region, not all of overlay 6 or the whole title scene.

Eight runtime sessions use save 83 and produce evidence_complete_*.json
under build/runtime/eur_title_localized_lifecycle. English, French, German,
Italian and Spanish each run the complete 1783-frame title/Start/load path;
Japanese runs 2083 frames. Separate English and Japanese skip sessions run
1343 and 1403 frames. Non-English sessions change only the private archive
language byte at the already byte-guarded pre-initialization hook. Fixtures
record the original/new values; the live save context and all 104 original
save files remain unchanged. German title output was visually inspected.

Across 13744 frames, all 6946 localized draw returns and all twelve probed
functions agree. Every normal localized state (0,1,2,3,5,6,7,8,9,10) is
observed; reserved state 4 and unknown states remain static-only. Independent
checks include 372 pulses, 234 arc positions, 114 shrinking fades, 133 split
subtitle steps, 75 final fades and 3147 complete rotation matrices. Six
localized square-root coefficient calculations and six shared arc-helper
calculations agree with integer square-root/division models; 160 shared arc
heights and 748 helper divisions also agree. Hide/disappear transitions and
Finish with both null and live command tracks are covered.

All 383146 direct graphics/SQRT stores are checked in native order and width.
The complete quad model checks 15804 visible rectangles plus 11 zero-alpha
returns. All 36 texture uploads, 379648 bytes, match live VRAM byte for byte.
Full lifecycle, texture and track records are checked around helper calls and
at SP-matched returns; external helper effects are refreshed after return.
All final display hashes match the established load-menu output. Allocation
failure and automatic translucent polygon ID 8 remain static-only.

All 81 tests pass, native relink has zero differences and public-content and
whitespace checks pass. Canonical ROM SHA-1 is unchanged:
ba4ec2f99b4f2e0047601552bccf00aa73e28701. Matching C/C++ is
670692 / 1563700 bytes (42.89%); C/C++ plus ASM is 43.22%.
Since resuming from e1c4416, 13032 additional bytes are matching C.


## 2026-09-12 - Shared title models and brightness fades (+1560 bytes)

Twelve functions in the contiguous 0x02073F20..0x02074538 range are now
matching C/C++: six title-model loading, drawing and cleanup functions
(1024 bytes), and six brightness lifecycle/interpolation functions (536 bytes).
The model helpers share declarations with the existing moving-sprite and orbit
modules. Recovered 48-byte model-element and 44-byte brightness layouts have
compile-time size checks. Readable matrix fields describe the main-screen
transform; the sub-screen path tracks OAM counts and subtracts the native
244-pixel vertical offset. No inline assembly was needed.

The 1783-frame English cold boot with original save 83 uses ordinary inputs:
wait:1500, start:30, wait:250. All twelve functions execute. Evidence in
build/runtime/eur_title_model_brightness/evidence_english83.json records
50 brightness interpolations, 100 master-brightness register checks, 108046
model-position checks, 70276 OAM count/group checks, and 37770 rotation and
identity-reset matrices. Resource construction and cleanup include 51 model
loads/releases and both sub-only and combined-screen elements. Full object
snapshots and SP-matched returns are checked around helper calls.

The final load-menu screenshot was inspected; both display buffers and both
palette hashes match the previous English replay. All 104 original save
hashes remain unchanged. Sine-curve and negative brightness, matrix flip bits,
and allocation failures were not reached and remain covered by byte matching.

The canonical package retains SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701. Module and symbol checks pass.
Native relinking also produces that hash with zero differences across all
43 components, after correcting twelve stale external symbol references in
the maintained assembly and patch manifest. All 81 tests, progress consistency,
public-content audit and whitespace checks pass. Matching C/C++ is now
672252 / 1563700 bytes (42.99%); C/C++ plus assembly is 43.32%.


## 2026-09-12 - Complete title panel transition and backgrounds (+2688 bytes)

The contiguous 0x02074538..0x02075014 range now matches in C: thirteen
functions / 2780 bytes, including the existing 92-byte panel-scroll routine.
The old single-function scroll file is merged into title_panel_transition.c.
Twelve newly reconstructed functions add 2688 bytes of matching C. The shared
header describes the 68-byte transition record and its four states; confirmed
HBlank/resource flags have names, while other bits retain neutral labels.

The code loads and releases the two background resource sets, uploads the
panel characters, screens and palettes, configures windows and blending,
interpolates the panel displacement, and selects display layers at scanlines
128 and 140. Original bitfield widths, repeated resource-pointer loads and
an unsigned interpolation intermediate preserve native instruction order.
The complete module matches without inline assembly.

Runtime evidence is in build/runtime/eur_title_panel_transition:
evidence_english83.json (1783 frames) and evidence_english_skip83.json
(1343 frames). Both start from cold boot with original save 83 and ordinary
buttons; no RAM or language fixtures are used. All thirteen functions run,
including the skip callback. All four upload states and the upper-screen,
main-only, both-screen and VBlank scanline cases are observed. The finish
branch that loads previously absent assets and resource allocation failures
remain unexercised.

Independent checks cover 78 displacement interpolations, 83 scroll updates,
166 blend-register results, 13744 direct register stores, 3084 complete
68-byte record snapshots, and 20258 HBlank/scanline returns each. All 38 VRAM
uploads match 335360 live source bytes; screen clears verify 12288 zero bytes,
and four background-color writes agree with their loaded palette entries.
The probe respects ARM conditions when observing conditional register stores.

The title screenshot was inspected. Both final display buffers and both
palette hashes agree with the corresponding established load-menu replays;
all 104 original save hashes are unchanged. Module/symbol checks, all 81 tests,
progress consistency, public-content audit and whitespace checks pass.
Canonical packaging and the 43-component native relink both retain SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701, with zero differing bytes.
Matching C/C++ is 674940 / 1563700 bytes (43.16%); C/C++ plus ASM is 43.49%.


## 2026-09-12 - Title startup greeting and animation callbacks (+1152 bytes)

Seven new functions add 1152 bytes of matching C/C++. The contiguous
0x02075014..0x020754e4 range now lives in title_startup.cpp: nine functions /
1232 bytes, including two previously reconstructed IRQ destructors. The code
selects title participants from the save directory and story flags, allocates
the animation controller, handles the greeting's waits and brightness fades,
requests frame uploads, and restores base vtables during destruction. Partial
controller and IRQ layouts are explicitly described as prefix views. No inline
assembly is used. The greeting's release helper receives the element pointer;
recovering that ABI resolved the remaining compiler register differences.

Six cold-boot sessions use original story saves 1, 6, 30, 65, 83 and 103 with
ordinary buttons and no RAM fixtures. Their evidence is under
build/runtime/eur_title_startup/. Across 3888 frames, all greeting states
(0, 1, 100, 101, 102, 1000) execute, with participant masks 3, 12 and 15 and
greeting streams 45, 46 and 47. Save 6 (Koopa Cruiser) supplies the middle
selection without modifying save data. Independent checks include 1374 greeting
returns, 1886 frame requests, 1606 inactive-state queries, six controller
constructions and six greeting sprite initializations/releases. Live object
snapshots, helper arguments, brightness steps and display-register writes agree
with the native behavior. Entry hooks guard the full function bytes and returns
match the calling stack pointer.

The two new nondeleting model/frame-task destructors were not reached; neither
was the preexisting nondeleting IRQ destructor. Empty-slot fallback, allocation
failure and invalid greeting states remain unexercised. Matching verifies their
compiled bytes, not runtime branch coverage. All 104 original save hashes are
unchanged; title animation was visually inspected, and the final save-83 display
and palette hashes agree with the previous title-panel replay.

Module and symbol checks, all 81 tests and progress consistency pass. The
canonical package and native relink retain SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701; the native relink has zero differing
bytes across 43 components. Matching C/C++ is 676092 / 1563700 bytes (43.24%);
C/C++ plus maintained assembly is 43.57%. Overlay 6 is 27372 / 66492 bytes.


## 2026-09-12 - Title animation controller lifecycle (+1580 bytes)

Eight functions in 0x0206bd9c..0x0206c3c8 now match in
src/overlay006/title_animation_lifecycle.cpp, adding 1580 C/C++ bytes. They
load and release five archive-offset tables, locate their entry records, select
localized resource pairs, construct the IRQ/archive/frame tasks, and clean up
the controller while restoring input-repeat delays and sound-channel allocation.
The recovered 1060-byte layout is explicitly a prefix of the 75304-byte object.
The constructor clears only offsets 40..1059, matching the native pointer bounds.
Both deleting and nondeleting destructors preserve the original virtual-call
checks. Explicit language branches preserve the native conditional stores and
resource call order without inline assembly.

Runtime evidence in build/runtime/eur_title_animation_lifecycle/ contains
three 1783-frame replays (5349 frames total): ordinary cold boots from saves
83 and 6, and a separate save-83 language-zero probe. Each replay executes seven
of the eight functions, including construction and deleting destruction.
Independent checks cover helper arguments and ordering, six entry-table layouts,
3060 cleared prefix bytes, 189 full 1060-byte snapshots, task ownership, resource
release and three restorations of the input-repeat delays. Descriptor formats
zero and one, present/absent table slots and resource entries 3, 4 and 8 run.

The language-zero probe changes one byte in private ARM9 RAM at the native
constructor's 0x0206c298 checkpoint: controller+1048 from 1 to 0, after checking
the current controller pointer and native bytes. It is logged as a controlled
fixture, not a normal language-selection route. The controller is subsequently
destroyed by normal title progression; original saves and the live save context
are unchanged. The nondeleting controller destructor, absent save context,
allocation failure, empty descriptors, table formats other than zero/one and
the music-busy loop remain unexercised. Their compiled bytes match exactly.

The final load-menu screens were visually inspected; the ordinary save-83
framebuffer and palette hashes match the preceding startup replay. All 104
original save hashes are unchanged. Module and symbol checks, all 81 tests,
progress consistency, public-content audit and whitespace checks pass. Canonical
packaging and native relinking retain SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701 with zero differences across 43 components.
Matching C/C++ is 677672 / 1563700 bytes (43.34%); C/C++ plus assembly is 43.67%.
Overlay 6 is 28952 / 66492 matching C/C++ bytes (43.54%).


## 2026-09-12 - Title animation IRQ uploads (+312 bytes)

Four functions in 0x0206ab80..0x0206acb8 now match in title_animation_irq.c:
the frame-upload callback, initializer and two destructors. The recovered
52-byte IRQ record contains an idle-frame counter and a frame-request bit.
Initialization clears only offsets 40..47; the final unknown word is preserved.
The callback counts idle frames until an upload is requested, temporarily maps
texture and palette banks into LCDC space, performs the native upload callbacks,
restores the bank assignments, copies/clears OAM and resets the request. The
whole 312-byte module matches in C without inline assembly.

Two normal cold-boot save-83 replays in build/runtime/eur_title_animation_irq/
cover full title playback and its shorter A-button skip route: 1783 and 1343
frames, respectively. The initializer and deleting destructor execute twice;
the callback executes 2036 times, comprising 16 idle frames and 2020 uploads.
Independent checks verify every helper argument, 28308 complete 52-byte
snapshots, all 2020 texture/palette bank restorations, 4136960 OAM bytes against
live source buffers, and the same number of cleared OAM-buffer bytes. No RAM
fixtures are used. The nondeleting destructor and other VRAM-bank configurations
remain unexercised; their compiled bytes match exactly.

Final screens were inspected, and both framebuffer/palette hash sets match the
corresponding established title replays. All 104 original save hashes remain
unchanged. Module/symbol checks, all 81 tests, progress consistency, public-content
audit and whitespace checks pass. Both canonical packaging and native relinking
retain SHA-1 ba4ec2f99b4f2e0047601552bccf00aa73e28701; the native relink has zero
differences across 43 components. Matching C/C++ is 677984 / 1563700 bytes
(43.36%); C/C++ plus assembly is 43.69%. Overlay 6 is 29264 / 66492 bytes (44.01%).


## 2026-09-12 - Title archive reads and resource cleanup (+852 bytes)

Three functions add 852 bytes of matching C/C++: the archive reader and model
resource cleanup at 0x0206b77c..0x0206b9ec (624 bytes), and renderer cleanup at
0x0206b1dc..0x0206b2c0 (228 bytes). Shared internal layouts describe sixteen
48-byte resource records, their owned conversion buffers and two virtual cleanup
objects. The existing controller lifecycle uses the same checked 1060-byte
prefix. All title archive callers now use the shared declaration. The loader's
entry argument is full-width and is truncated internally, matching the native
calls and halfword stores. Ordinary reads use a 44-byte request because the
queue also writes compressed_state at +40; compressed requests use 64 bytes.

Three 1783-frame sessions (5349 frames) in build/runtime/eur_title_animation_resources/
exercise all three functions: ordinary cold boots from saves 83 and 6, plus a
separate save-83 language-zero controller fixture. The fixture changes only
controller+1048 after the native constructor reaches 0x0206c298, as recorded in
the evidence; it is not a normal language-selection route. Every source-save
hash remains unchanged. All five archive families are used: BObjUI, BObjPc,
TitleBG, option-menu messages and StatFontSet.

Across 119 completed entry reads, the probe independently compares 67 ordinary
payloads (80864 bytes) with the extracted archive entries and checks offsets,
request fields, allocation arguments, returned pointers and optional sizes.
The 52 compressed reads complete for 757132 output bytes; their request setup
and completion are checked, but this probe does not independently decode the
compressed payloads. Cleanup verifies 30 freed raw resources, 21 owned conversion
arrays, 24 released VRAM-bank assignments, six blend-register resets and six
display-control writes. The blend helper retains alpha mode with no first target
planes and coefficients 8/8. Full controller and request snapshots are checked
around helper calls.

Descriptor reuse, malformed entries, allocation failures, absent renderer objects
and owned screen-group offset buffers were not observed. The final screen was
inspected, and all three framebuffer/palette hash sets agree with their previous
lifecycle replays. Module/symbol checks, all 81 tests, progress consistency,
public-content audit and whitespace checks pass. Canonical packaging and native
relinking retain SHA-1 ba4ec2f99b4f2e0047601552bccf00aa73e28701 with zero differences
across 43 components. Matching C/C++ is 678836 / 1563700 bytes (43.41%);
C/C++ plus assembly is 43.74%. Overlay 6 is 30116 / 66492 bytes (45.29%).

The adjacent 944-byte resource-selection candidate at 0x0206b9ec remains private:
its loop still assigns two registers differently. It is excluded from coverage.


## 2026-09-12 - Title frame passes and exit transition (+604 bytes)

Four functions add 604 matching C/C++ bytes: the 396-byte main callback at
0x0206acb8..0x0206ae44 and the 208-byte exit/draw/upload group at
0x0206c464..0x0206c534. The main callback reads input, initializes per-frame
geometry registers, advances the title sequence, prepares and sorts the sub-screen
OAM, dispatches two renderer passes and submits the frame. Exit state 1 deletes
the controller, unloads overlays 6 and 5, then requests session state 8 or 2.
The native post-deletion read of exit_kind is preserved and documented.
The element passes traverse the live list, checking each callback and its
individual disable bit. All functions match without inline assembly.

Two original save-83 cold boots use normal title playback and the shorter
A-button skip route (1783 and 1343 frames). Evidence in
build/runtime/eur_title_animation_update/ records all four functions executing:
2024 main updates, 2022 draw passes, 2020 upload passes and two exit requests.
Across 3126 frames, the probe checks 119298 draw-list visits and 119180 upload-list
visits, including present/absent callbacks and both disabled/enabled cases.
It verifies 107889 dispatched draw callbacks and 2155 upload callbacks with exact
arguments and SP-matched returns. Geometry-register writes, both renderer task
passes, exit field writes, overlay unload order and the final state-8 request
are checked independently of the reconstructed code.

No RAM fixtures are used. Exit states 0 and 1 are observed; nonzero exit kinds,
the state-2 destination, empty element lists and other exit-state values remain
unexercised. The second replay releases each visited node snapshot after checking
its callback and next link, avoiding repeated reads of earlier nodes. This keeps
the same traversal checks while reducing probe overhead.

Both framebuffer/palette hash sets match the established IRQ replays, whose
screens were visually verified. All 104 source saves remain unchanged. Module
and symbol checks, all 81 tests, progress consistency, public-content audit and
whitespace checks pass. Canonical packaging and native relinking retain SHA-1
ba4ec2f99b4f2e0047601552bccf00aa73e28701, with zero differences across 43 components.
Matching C/C++ is 679440 / 1563700 bytes (43.45%); C/C++ plus assembly is 43.78%.
Overlay 6 is 30720 / 66492 bytes (46.20%).


## 2026-09-12: Title display initialization (+1,212 matching C++ bytes)

- Reconstructed `TitleAnimation_InitDisplay` at `0x0206B2C0` (1,212 bytes),
  linked from `src/overlay006/title_animation_display.cpp`. It clears VRAM,
  palettes and OAM; configures geometry, bank assignments and backgrounds;
  assigns sprite/texture palette buffers; creates palette-effect controllers;
  and enables the displays after VBlank. No inline assembly was added.
- Recovered the two controller pointers at offsets 904/908 and two pairs of
  20-byte palette entries at 912/952 in the shared controller header. A separate
  42,536-byte prefix view describes the extended/sub sprite and texture palette
  buffers at 1064, 9256 and 9768; this does not redefine the full 75,304-byte
  controller allocation. Existing lifecycle, render cleanup and update callers
  now use the confirmed field names and public initializer declaration.
- The private candidate matched in size immediately. The only byte difference
  was the 32-bit literal for the DISP3DCNT mask: the native `~0x3002` retains
  upper set bits, whereas `0xCFFD` did not. Correcting that expression matched
  the literal pool; the complete linked module and symbol checks then passed.
- Cold-booted original story saves 1 (Peach's Castle) and 83 (Star Shrine),
  each with `wait:1500 start:30 wait:250`: 3,566 frames and two completed
  initializer calls. Both were ordinary input routes without RAM fixtures.
  Independent checks covered 1,347,584 cleared VRAM/palette bytes, 4,096 OAM
  buffer bytes and their 4,096 uploaded bytes, all nine bank mappings on both
  runs, 16 bank-state fields, four blend pairs, sprite/texture buffer pointers,
  four palette controllers and 160 bytes of default entries. Every direct MMIO
  store was checked for address, width and value; 78 full controller-prefix
  snapshots bracketed helper calls and returns. Allocation failure and the
  display-mode-1 single-screen helper branches were not exercised.
- Evidence: ignored `build/runtime/eur_title_animation_display/`, with
  `evidence_early1.json` and `evidence_english83.json`; corresponding probes and
  logs are under `build/analysis/`. All 104 original save hashes remain unchanged.
  Both final screenshots were visually inspected at the load menu. Save 83's
  four final VRAM/palette buffer hashes also match the previous title replay.
- Verification: `ninja check`, canonical `build_nds.ps1 -DisableDataMods`, and
  native relink (43 components, zero differing bytes) passed. Both ROM SHA-1s
  are `ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Regenerated progress, its
  `--check`, and `python -m pytest -q tests` passed (81 tests).
- Matching C/C++ is now **680,652 / 1,563,700 bytes (43.53%)**; C/C++ plus
  symbolic assembly is **43.86%**. Overlay 6 is **31,932 / 66,492 (48.02%)**.


## 2026-09-12: Title sequence cleanup and orbit readiness (+1,044 matching C bytes)

- Added `TitleAnimation_ReleaseSequence` at `0x0206CF18` (924 bytes) and
  `TitleAnimation_AreOrbitsInactive` at `0x0206CEA0` (120 bytes), together in
  `src/overlay006/title_sequence_cleanup.c`. Their contiguous region is linked
  and byte-identical. No inline assembly was added.
- The shared internal header maps the existing brightness, transition, orbit,
  localized/sprite sequence, moving-model and panel groups into a 57,288-byte
  prefix of the 59,340-byte allocation. Unknown prompt fields retain neutral
  names; the save setting at byte 1300, bit 6 is still not given a gameplay name.
  Cleanup updates that bit, calls the native release sequence, clears five owned
  resource pointers and frees/clears the sequence global. The six identical
  calls to the first panel's empty release function are intentionally preserved.
- A C++ draft emitted 916 bytes because it omitted two byte-mask instructions
  after boolean comparisons. The same typed operation in C recovered those
  masks. Giving each cleanup loop its own index scope then reproduced the
  remaining native register assignments without source permutation searches.
  Both functions passed the complete module and symbol checks after integration.
- Runtime: saves 1 and 83 cold-booted with `wait:1500 start:30 wait:250`, plus
  one separately labeled save-83 fixture that changed the signed override at
  sequence offset 1746 from -1 to 1 immediately before cleanup. The object was
  then freed normally; original battery saves were not edited. This fixture
  establishes the alternate cleanup branch, not ordinary menu accessibility.
- The three runs covered 5,349 frames, 804 orbit-query returns, 1,779 individual
  state checks and three complete cleanups. Both query outcomes were observed:
  798 early returns and six all-inactive results. Cleanup checks covered the
  ordered calls for 75 auxiliary elements, 42 orbit elements and 12 moving/model
  pairs; 18 repeated first-panel calls; 15 cleared resource pointers; three
  sequence frees/global clears; and both saved-flag values. Full snapshots
  checked the sequence prefix, save byte and global pointer around helper calls.
  Null owned-resource and final-null-global branches were not observed; neither
  were all possible early-return positions or every selection value.
- Private evidence is under `build/runtime/eur_title_sequence_cleanup/` as
  `evidence_early1.json`, `evidence_english83.json` and
  `evidence_override1_fixture83.json`. Probe source and logs are in
  `build/analysis/`. All 104 original save hashes are unchanged. All three final
  VRAM/palette hash sets match the corresponding prior display-initialization
  replay; the new ordinary save-83 load-menu screenshot was visually inspected.
- Verification: `ninja check`, canonical ROM packaging with `-DisableDataMods`,
  native relink of all 43 components with zero differing bytes, progress
  generation/`--check`, and `python -m pytest -q tests` (81 passed). Canonical
  and native ROM SHA-1: `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is now **681,696 / 1,563,700 bytes (43.60%)**; C/C++ plus
  symbolic assembly is **43.93%**. Overlay 6 is **32,976 / 66,492 (49.59%)**.
  The 50% project milestone still requires 100,154 additional matching bytes.


## 2026-09-12: Title sequence phases and exit controls (+2,568 matching C++ bytes)

- Added `TitleAnimation_UpdateSequence` at `0x0206C534` (2,412 bytes),
  `TitleAnimation_BeginExit` at `0x0206C3E4` (128 bytes), and
  `TitleAnimation_ResetIdleTimer` at `0x0206C3C8` (28 bytes). They share
  `src/overlay006/title_sequence_update.cpp` with the three previously matched
  draw/upload/exit-request functions, replacing the temporary elements module.
  The complete six-function, 2,776-byte region matches; no assembly was added.
- The state machine preserves callback gating and reads the next link and phase
  after callbacks. Named phases cover timed music/entry, orbit and panel motion,
  presentation readiness, prompt activity, automatic exit, and skipping. Native
  table references retain prompt delays and signed brightness targets. The exit
  lookup uses the full-width argument before storing its low byte. An explicit
  `localized == 1` comparison reproduces the native condition in the model wait.
- Source and the observed menu confirm save-object byte `+0x514`, bit 6 as the
  rumble preference. The sequence header and cleanup now use rumble/input names
  for confirmed prompt fields; unknown fields remain neutral.
- Five successful replays under `build/runtime/eur_title_sequence_update/`
  cover 11,166 frames, 8,327 sequence updates, five begin-exit calls and two idle
  resets. The probe independently checks state-owned writes, helper ordering
  and arguments, 489,711 list visits, 449,390 update callbacks, 2,938 element
  readiness results and 1,362 orbit-group results. All 14 implemented phases
  execute across the runs; external helper effects are refreshed after return,
  not independently reimplemented in full.
- Ordinary save-83 startup and save-1 early skip use buttons only. An additional
  save-83 attempt to enter options finds only PRESS START with Slot-2 absent;
  it remains ordinary replay evidence, not proof of options accessibility.
- A separate save-83 fixture changes four bytes of the initialized primary
  prompt immediately after its native constructor: selection/min/max from
  `[2,2,2]` to `[0,0,1]`, and layout from 1 to 0. This reproduces the rumble-menu
  layout without changing hardware availability. Buttons then open the visible
  Use Rumble Feature menu, select No, and exit. Both reset calls occur here;
  the screenshot was inspected. This verifies UI behavior, not physical rumble.
- A language-zero fixture changes only controller language byte `+1048` after
  initialization. Its 4,202-frame replay exercises the alternate presentation,
  music completion, the 32-frame auto-exit threshold, exit kind 1, and a fresh
  normal title sequence. One final frame drains a pending monitored call.
  Earlier probe-end assertions were traced to a normal buffer-processing loop;
  stopping new outermost captures while completing pending calls resolved them.
- Unexercised cases include the 1,800-frame rumble-prompt-seen timeout, resetting
  a nonzero idle timer, empty lists, absent-and-disabled callbacks, unknown
  phases, other skip participant masks, language-zero skipping and exit kinds
  outside 0/1. Fixture setup and ordinary input are recorded separately.
- All 104 original save hashes remain unchanged. The ordinary save-83 load-menu
  screenshot was inspected; its four final VRAM/palette hashes match the prior
  title-update replay. Distinct fixture routes are not claimed to share that
  baseline. Current canonical and native ROM hashes match every replay's ROM.
- Module/symbol checks, canonical packaging, native relink (43 components,
  zero differences), progress consistency, and all 81 tests pass. Both ROMs
  retain SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **684,264 / 1,563,700 bytes (43.76%)**; C/C++ plus symbolic
  assembly is **44.09%**. Overlay 6 is **35,544 / 66,492 (53.46%)**.
  The project-wide 50% milestone still requires 97,586 matching bytes.


## 2026-09-12: Title sequence construction (+2,200 matching C++ bytes)

- Added `TitleAnimation_InitSequence` at `0x0206D2B4` (2,200 bytes), linked
  from `src/overlay006/title_sequence_init.cpp`. It allocates the sequence,
  loads its graphics, combines two screen maps, constructs and links the title
  elements, selects the language presentation and participants, enables the
  rumble preference and completes both music loads. No assembly was added.
- The shared allocation view preserves the 57,288-byte cleared prefix within
  the full 59,340-byte object. Only 512 copied palette bytes at offset 57,292
  are named in the tail; the preceding word and remaining 1,536 bytes stay
  unknown. Cleanup still uses the prefix view.
- The private draft produced the native size immediately. Twelve differing
  words were limited to the screen-map interleave's pointer loads and register
  operands. Preserving the native left/right/destination load order matched
  all bytes. The public module and symbol checks passed after integration.
- Four cold-boot replays use `wait:1500 start:30 wait:250`: ordinary story saves
  1, 6 and 83, plus an explicitly labeled save-83 language-zero fixture. They
  cover 7,132 frames and four complete initializer calls. The participant masks
  3, 12 and 15 are checked against live save-directory and slot flags; both
  localized and alternate sprite presentations execute.
- Independent checks cover 229,152 cleared prefix bytes; 10,240 copied palette
  and screen-map bytes; 128 interleaved rows; 16 owned archive-pointer stores;
  20 cache-flush arguments; 232 element appends and the complete final lists;
  56 orbit and 12 participant initializers; and eight completed music loads.
  The probe checks full 59,340-byte snapshots around direct helper calls, exact
  helper arguments, save preference and initial timers. External helper effects
  are refreshed after return, with explicit result checks for the listed data.
- All four runs have a save context and non-null resources. They observe the
  rumble preference already enabled, so a false-to-true preference change is
  not separately demonstrated. Null/failure cases, missing-slot fallback and
  other languages remain unexercised. The language fixture changes only the
  controller byte after language setup, leaving the original saves untouched.
- Evidence: `build/runtime/eur_title_sequence_init/`; private probe/oracle source
  and logs are under `build/analysis/`. All 104 source save hashes are unchanged.
  Save 1, save 6 and the language fixture's final load-menu screenshots were
  visually inspected. Ordinary save 1/83 VRAM and palette hashes agree with
  their prior captures; all captured buffer files match their recorded hashes.
- Verification: full module/symbol checks, canonical packaging, native relink
  of 43 components with zero differing bytes, progress generation/consistency,
  and all 81 tests passed. Canonical and native ROM SHA-1 remains
  `ba4ec2f99b4f2e0047601552bccf00aa73e28701`, matching all replay reports.
- Matching C/C++ is **686,464 / 1,563,700 bytes (43.90%)**; C/C++ plus symbolic
  assembly is **44.23%**. Overlay 6 is **37,744 / 66,492 (56.76%)**.
  The project-wide 50% milestone still requires 95,386 matching bytes.


## 2026-09-12: Title sequence model entry (+348 matching C bytes)

- Reconstructed `TitleSequenceModel_FinishEntry` (32 bytes), `StartEntry`
  (20), `Init` (112) and `Update` (184) in the contiguous region
  `0x02072988..0x02072AE4`, linked from `src/overlay006/title_sequence_model.c`.
  All four private candidates matched on the first compile. The shared header
  now declares the model helpers and names idle, delay and moving states;
  sequence initialization/update use those declarations. No assembly was added.
- The model waits eight updates, moves from y=500 to y=424 over 24 updates,
  and holds x=128. Positions use Q12 values and preserve the native signed
  division/truncation. The immediate-finish helper resets elapsed time and
  places the model at the final coordinates; the initializer retains resource
  `0xC2000033`, selector 15 and its native model-loading arguments.
- Three ordinary button replays use save 83 and save 1: normal startup, skip
  after 900 frames, and skip after 500 frames. Across 4,119 frames the probe
  records three initializations, 2,463 updates, two entry starts and two forced
  finishes. Independent 48-byte checks cover both delay completions, all 23
  intermediate positions twice, both motion completions, callback assignment,
  and exact initialization helper arguments. The 500-frame skip confirms an
  actual y=500 to y=424 change before entry begins; the later skip observes
  completion after landing. No RAM fixtures are used.
- Unknown states, abnormal elapsed values and skipping partway through motion
  remain unexercised. Evidence is under `build/runtime/eur_title_sequence_model/`,
  with probes and logs under `build/analysis/`. All 104 original saves remain
  unchanged. Both earlier routes match their prior VRAM/palette captures; the
  two save-1 final load-menu screenshots were visually inspected. The additional
  earlier-skip route is not claimed to share a prior display baseline.
- Full module/symbol checks, canonical packaging, all 43 native components
  with zero differences, progress consistency and all 81 tests pass. Both ROMs
  retain SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **686,812 / 1,563,700 bytes (43.92%)**; C/C++ plus symbolic
  assembly is **44.25%**. Overlay 6 is **38,092 / 66,492 (57.29%)**.
  The project-wide 50% milestone still requires 95,038 matching bytes.


## 2026-09-12: Title pixel trails (+544 matching C bytes)

- Added the seven trail helpers at `0x02072768..0x02072988` in
  `src/overlay006/title_trail.c`: buffer initialization, update, tiled upload
  and empty release; stamp initialization, clipping/draw and empty release.
  Six matched immediately. The 348-byte draw routine required an unsigned
  height shift to reproduce the native logical shifts; the complete linked
  region and symbol layout then matched. No assembly was added.
- Native calls establish all eight rasterizer arguments, including source
  dimensions, position, destination, stride and height. The first buffer uses
  y-origin zero; the second uses 244. Clipping checks signed endpoints before
  calling the native saturated 4bpp blitter.
- Recovered two 52-byte trail records and two 24,576-byte pixel buffers in the
  shared sequence layout. The sprite header now names its upload callback and
  pass flags. Initialization, update, cleanup and orbit drawing use the typed
  records and named helpers.
- Correction to the preceding sequence-allocation notes: the 512-byte payload
  at offset 57,292 is a 32x32 4bpp trail stamp, not palette data. Its use by the
  pixel rasterizer establishes this directly; the allocation/source names now
  reflect that role. The other tail bytes remain unknown.
- Two ordinary cold-boot replays use save 83 with normal startup and save 1
  with an A press after 500 frames: 2,776 frames, 546 buffer updates, 542 upload
  passes, 2,008 stamp-draw calls, both initializers twice and four empty releases.
  No RAM fixtures are used. Private evidence and captures are under
  `build/runtime/eur_title_trail/`; probes and logs are under `build/analysis/`.
- Independent oracles verify 26,836,992 faded bytes using a per-nibble lookup;
  26,640,384 tiled upload bytes using a linear-to-tile mapping and destinations
  derived from display registers; and 593,344 stamped pixels using clipped,
  saturated addition. The 723 actual blits include 288 clipped and 435 unclipped
  cases, both screens, all eight horizontal alignments and 10,432 saturated
  pixels. Zero/nonzero buffers, rejected draw calls, callback/pointer stores and
  unchanged empty-release records are checked. Other stamp dimensions and a
  non-null stamp update callback remain unexercised.
- All 104 original saves remain unchanged. Both final VRAM/palette hash sets
  match their prior normal/early-skip captures, and the save-83 final load-menu
  screenshot was visually inspected. Full module/symbol checks, canonical
  packaging, native relink (43 components, zero differences), progress checks
  and all 81 tests pass. Both ROMs retain SHA-1
  `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- The neighboring 176-byte tilemap/palette initializer remains unmatched; its
  current C draft is 172 bytes with different loop scheduling. A separate
  108-byte resident fade draft has register-operand differences. Neither is
  linked or counted as progress; both are deferred without permutation searches.
- Matching C/C++ is **687,356 / 1,563,700 bytes (43.96%)**; C/C++ plus symbolic
  assembly is **44.29%**. Overlay 6 is **38,636 / 66,492 (58.11%)**.
  The project-wide 50% milestone still requires 94,494 matching bytes.


## 2026-09-12: Title prompt and shared menu selection (+1,168 matching C bytes)

- Added the five primary prompt helpers at `0x02072AE4..0x02072EA8`
  (964 bytes) in `src/overlay006/title_prompt.c`, and shared menu initialization
  and selection at `0x02073870..0x0207393C` (204 bytes) in
  `src/overlay006/title_menu_selection.c`. Both contiguous regions are linked
  and byte-identical; no assembly was added.
- The prompt constructs language-specific item positions, waits for its delay,
  scales into view over 16 updates, draws the selected entries, handles confirm
  and direction inputs, and waits for the rumble prompt. Shared selection retains
  signed-byte wrapping, fixed-selection no-ops, cursor coordinates and the
  caller-supplied next state. Full-width parameters retain native truncation.
- Recovered a 60-byte common menu prefix and distinct 96-byte primary and
  120-byte rumble records. The rumble record does not contain the full primary
  prompt; its unrecovered tail remains neutral. Shared declarations and sequence
  callers use the corrected layouts and named prompt helpers.
- Four prompt functions matched immediately. The draw helper's initial 180-byte
  candidate became the native 176 bytes by preserving both scale loads before
  stores and direct layout-table indexing. The two shared helpers also matched
  without permutation searches. The neighboring cursor-show helper remains
  native after a same-size private candidate retained register-operand differences.
- Runtime: four cold boots, 7,371 frames, using ordinary save-83 startup,
  save-1 fixed-selection inputs, a save-83 rumble-layout fixture, and a separate
  save-83 language-zero fixture. Private reports, states and captures are under
  `build/runtime/eur_title_prompt/`; the independent probe is in `build/analysis/`.
- All seven functions execute: eight shared initializations, four prompt
  initializations/starts/releases each, 2,614 draws, 2,610 updates and eight
  selection calls. Oracles check 11,484 full prompt snapshots, 30 common-prefix
  snapshots, 8,488 model-scale checks, 3,073 drawn items, all 15 intermediate
  scales four times, both observed delays (32/80), two fixed-selection no-ops,
  six cursor moves, wrapping in both directions and the complete rumble wait.
  Exact direct-helper arguments and inactive-query results are verified;
  other helper effects are observed by refreshing memory after return.
- The rumble fixture changes four initialized primary-menu bytes, retaining
  the absent-hardware flag. Screenshots confirm opening Use Rumble Feature and
  moving from Yes to No before returning and choosing Start Game. This exercises
  UI behavior, not the constructor's hardware-present branch or physical rumble.
  The separate language fixture changes only the title controller language byte.
- Unexercised paths include inactive state 0, cursor-wait state 4, disabled
  active input, unsupported states/selections, hardware-present construction,
  other language rows and abnormal counters. The normal move call supplies
  next state 3, so it does not establish cursor-wait state coverage.
- All 104 original save hashes and captured buffer-file hashes are verified.
  Ordinary save-83 final VRAM/palette hashes match the preceding trail replay;
  its load-menu screenshot and both rumble-choice screenshots were inspected.
  The other routes are not claimed to share that baseline. No drain frames
  were needed in these four runs.
- Full module/symbol checks, canonical packaging, native relink of 43 components
  with zero differences, progress checks and all 81 tests pass. Canonical and
  native ROM SHA-1 remains `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **688,524 / 1,563,700 bytes (44.03%)**; C/C++ plus symbolic
  assembly is **44.36%**. Overlay 6 is **39,804 / 66,492 (59.86%)**.
  The project-wide 50% milestone still requires 93,326 matching bytes.


## 2026-09-12: Rumble menu opening, input and cleanup (+808 matching C bytes)

- Added `TitleRumblePrompt_Release` (64 bytes) and `TitleRumblePrompt_Open`
  (92 bytes) at `0x02072EA8..0x02072F44` in `title_rumble_control.c`, plus
  `TitleRumblePrompt_Update` (652 bytes) at `0x02073580..0x0207380C` in
  `title_rumble_update.c`. All three matched on the first private compile and
  pass the complete linked module/symbol checks. No assembly was added.
- The shared 120-byte rumble record now identifies its tile/palette allocations,
  owned palette buffer, saved cursor coordinates and first tile. The native text
  constructor and drawing routine remain between the two linked source ranges;
  consolidate the temporary split once those intervening functions match.
- Named states describe entry, active input, cursor waiting, exit and inactivity.
  Opening preserves the cursor position; entry and exit each use eight updates
  with the native signed sine-table calculation. Confirmation stores the selected
  override, cancellation preserves it, and both restore the cursor. Selection
  changes reset the idle timer and request or stop rumble as the native flags allow.
  Cleanup unlinks both graphics allocations, frees the owned palette and clears it.
- Three replays total 5,819 frames: ordinary unopened save-1 startup, save-83
  confirmation of No, and save-83 cancellation/reopening/confirmation of Yes.
  The latter two use the documented four-byte post-constructor layout fixture,
  leaving hardware availability false. Private evidence is under
  `build/runtime/eur_title_rumble/`; the independent oracle is in `build/analysis/`.
- Four opens, 296 updates and three releases are checked through 359 full
  120-byte snapshots. Oracles verify all seven intermediate entry/exit positions
  four times (56 positions), four cursor moves, 16 cursor-readiness results,
  four cursor restorations, both confirmed choices and two cancellations.
  Cancellation preserves both the initial -1 override and an earlier confirmed
  Yes override of 0. Reopening with No selected exercises entry without a rumble
  request; four timed-rumble helper calls are observed with exact arguments.
  These calls do not establish physical accessory support.
- All 104 original saves and capture-file hashes remain unchanged/consistent.
  Captures include OAM, both VRAM regions and palettes. Ordinary save-1 display
  hashes agree with its earlier sequence-initialization replay, and its load-menu
  screenshot was inspected along with the Yes selection and post-cancel menu.
  The fixture routes are not claimed to share that baseline. No drain frames
  were required. Inactive/unknown update states, disabled active input, hardware
  stop, null owned palette and abnormal motion counters remain unexercised.
- Full matching checks, canonical packaging, native relink of all 43 components
  with zero differences, progress checks and all 81 tests pass. Both ROMs retain
  SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Deferred private candidates: the 1,224-byte text constructor emits 1,188 bytes;
  the 372-byte draw routine has the correct size but register/load scheduling
  differences. Its initial extra 16 bytes were traced to premature tile, palette
  and y-coordinate narrowing. No source-permutation search was performed, and
  neither deferred function contributes to coverage.
- Matching C/C++ is **689,332 / 1,563,700 bytes (44.08%)**; C/C++ plus symbolic
  assembly is **44.41%**. Overlay 6 is **40,612 / 66,492 (61.08%)**.
  The project-wide 50% milestone still requires 92,518 matching bytes.


## 2026-09-12: Shared title menu cursor (+700 matching C bytes)

- Added all six cursor helpers at `0x0207393C..0x02073BF8` in
  `src/overlay006/title_menu_cursor.c`: release (12 bytes), initialization (100),
  appearance (68), movement (72), drawing (12) and update (436). Every function
  matched on its first private compile; the complete linked range and symbol
  layout pass. No assembly was added.
- A recovered 64-byte `TitleMenuCursor` replaces the sequence's opaque cursor
  storage and types the shared menu pointer. It contains the model prefix,
  initial coordinates and target coordinates. Callers use named cursor helpers;
  states distinguish idle, movement and appearance.
- Appearance starts 16 pixels left of the target with zero scale. Movement
  snapshots the current position and requests the native cursor sound. Updates
  preserve signed angle division, sine-table interpolation, coordinate rounding
  before multiplication and signed scale conversion. Completion stores the
  exact target and returns to idle; the appearance path restores both scales.
- Two save-83 cold boots total 3,768 frames. Ordinary PRESS START startup only
  initializes/releases this cursor; the absent-accessory layout never shows it.
  A separate documented rumble-layout fixture exercises all six functions with
  ordinary buttons after changing the four constructor-result bytes. This is
  UI evidence, not proof of accessory detection or physical rumble support.
- Runtime observes two initializers/releases, one appearance, seven moves,
  731 draws and 730 updates. Independent checks cover 2,261 full 64-byte snapshots,
  17 scale pairs, 44 division results/interpolated positions, all 15 intermediate
  appearance frames, five four-frame moves and two eight-frame moves. Positive,
  negative and zero coordinate deltas occur, and all eight animations complete.
  Direct helper arguments and return values are checked; external model/audio
  effects are refreshed rather than independently reimplemented.
- Evidence and OAM/VRAM/palette captures are in `build/runtime/eur_title_cursor/`;
  the probe is under `build/analysis/`. All 104 source saves are unchanged, captured
  buffer hashes agree with the reports, and both routes' final VRAM/palette
  hashes match their corresponding prompt-batch baselines. The new screenshot
  after wrapping to Options was visually inspected. No drain frames were needed.
- Unknown states, abnormal/zero durations, nonintegral coordinate deltas,
  interrupted motion and missing-model cases remain unexercised. The ordinary
  replay alone would not establish movement or rendering coverage.
- Full module/symbol checks, canonical packaging, the 43-component native relink
  with zero differences, progress checks and all 81 tests pass. Both ROMs retain
  SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **690,032 / 1,563,700 bytes (44.13%)**; C/C++ plus symbolic
  assembly is **44.46%**. Overlay 6 is **41,312 / 66,492 (62.13%)**.
  The project-wide 50% milestone still requires 91,818 matching bytes.


## 2026-09-12: Title backdrop group construction (+456 matching C bytes)

- Added `TitleBackdrop_OnSkip` (4 bytes), `TitleBackdrop_Release` (12) and
  `TitleBackdrop_InitAll` (440) at `0x02073BF8..0x02073DC0` in
  `src/overlay006/title_backdrop_init.c`. The contiguous region is linked and
  byte-identical. The initializer constructs 25 model elements in four groups
  of 4/5/8/8, with separate coordinates, velocities, animation IDs and depths.
- Recovered the 52-byte backdrop layout: model prefix, signed horizontal velocity,
  four-bit group and unknown remainder. The shared sequence now names its
  backdrop array. Public names describe the rendering role without identifying
  particular artwork from the animation IDs alone.
- The first initializer candidate already had the native size. Restricting its
  per-group variable lifetimes removed register-assignment differences; advancing
  the point pointer alongside the loop counter then restored the last two
  instruction positions. No brute-force permutations or assembly were used.
- Two ordinary cold boots use save 83 (normal startup) and save 1 (A after
  500 frames), totaling 2,776 frames. Independent checks verify 50 constructed
  elements, original-ROM table coordinates, velocity/depth/group/callback stores,
  model flags and exact helper arguments. The probe checks 102 full 1,300-byte
  group snapshots, 1,250 model-flag words, 125 full 52-byte records, all 50 releases
  with cleared model slots and 25 unchanged skip hooks.
- Evidence is under `build/runtime/eur_title_backdrop/`; probe source and logs
  are in `build/analysis/`. All 104 original saves are unchanged. OAM, VRAM and
  palettes are captured, their file hashes checked, and both routes' final
  VRAM/palette hashes agree with the prior trail replays. The new save-1 load-menu
  screenshot was inspected. Neither run uses a RAM fixture or needs drain frames.
  Allocation failure, missing models and alternate table data remain unexercised;
  this batch does not reconstruct or independently verify the scrolling update.
- Full module/symbol checks, canonical packaging, native relink of 43 components
  with zero differences, progress checks and all 81 tests pass. Both ROMs retain
  SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Deferred candidates: backdrop update `0x02073DC0` emits 188 versus 172 bytes,
  with separately loaded endpoint literals where native code adds to each table
  base. Model selector `0x02073E6C` has the native 180-byte size but differs in
  conditional-value and mask scheduling. C++ boolean comparison initially omitted
  two byte masks; preserving an integer conditional restored size but not all bytes.
  Neither deferred function is linked or counted.
- Matching C/C++ is **690,488 / 1,563,700 bytes (44.16%)**; C/C++ plus symbolic
  assembly is **44.49%**. Overlay 6 is **41,768 / 66,492 (62.82%)**.
  The 50% project milestone still requires 91,362 matching bytes.


## 2026-09-12: Credits illustration placement and fade transition (+824 matching C bytes)

- Added five byte-identical functions: `CreditsFade_InitAll` (100 bytes) and
  `CreditsFade_Update` (464) at `0x02075F94..0x020761C8`, plus the layout-center
  (68), cell (132) and origin (60) target helpers at `0x02078B8C..0x02078C90`.
  Related functions are grouped in `credits_fade.c` and `credits_positions.c`.
- Recovered the 84-byte credits motion record's callback, unsigned phase/counter
  and variant/column/row bytes. The existing motion fields retain their offsets.
  The shared 0x823C-byte workspace now has typed layout, variant, screen,
  active-count and polygon-alpha fields, with the remaining storage left unknown.
  Both layouts have compile-time size checks.
- Native unsigned switch dispatch established the phase type. Sixteen-byte
  layout records require a correspondingly typed table view; this removed an
  extra index shift. Declaring the workspace as its actual object type preserved
  the original accesses where a cast from a byte array added instructions.
  No compiler flags or inline assembly were needed.
- The position helpers derive Q12 coordinates from the layout table, with
  5/4-pixel cell spacing, an 80/64-pixel center offset and a 224-pixel origin
  offset for the second screen. The fade retires all but the first of 1,024
  cells, fades polygon alpha down, replaces the grid, waits 120 frames and fades
  the final illustration back in. The original layout table selects this fade
  for illustration 15; an initial 5,000-frame replay had not reached it.
- Two successful replays from the compatible checkpoint-86 Shroob Castle state
  total 17,000 frames. A decoded field command is temporarily replaced with
  opcode 0x122/fade=1 and all 72 bytes are restored at the credits helper before
  native scene initialization. This verifies controlled credits entry and the
  subsequent native sequence, not ordinary story completion. Evidence and probe
  source are local under `build/runtime/eur_credits_transition/` and `build/analysis/`.
- The extended 12,000-frame run reaches all five functions, all fade phases
  (0/1/2/3/4/100/101), all 16 layouts and both screen modes for every position
  helper. Together the runs independently verify 51,823 coordinate pairs,
  53,167 complete 84-byte records, two full 86,016-byte grids and 1,346 complete
  52-byte workspace prefixes. Initialization and replacement each check all
  1,024 cells. The fade has 61 fade-out, 121 wait and 121 fade-in updates.
- All 104 original save hashes remain unchanged. Fourteen VRAM/palette/OAM/display
  register dumps and nine screenshots have verified file hashes. Fade-out,
  replacement and final credits screenshots were inspected. Neither successful
  replay needed drain frames. Idle phase 101 was independently checked 16 times;
  311,999 further idle entries were counted without snapshots. Invalid layouts,
  other screen values, abnormal counters and phase-100 entry with an already-zero
  active count remain unexercised. The native motion-advance helper remains unlinked.
- Full module/symbol checks, canonical packaging, native relink of 43 components
  with zero differing bytes, generated-progress checks and all 81 tests pass.
  Both ROMs retain SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **691,312 / 1,563,700 bytes (44.21%)**; C/C++ plus symbolic
  assembly is **44.54%**. Overlay 6 is **42,592 / 66,492 (64.06%)**.
  The project-wide 50% milestone still requires 90,538 matching bytes.


## 2026-09-12: Credits collapse and scatter transitions (+3,072 matching bytes)

- Added eight byte-identical functions at `0x02076E68..0x02077A68` in
  `credits_particle_transitions.c`: four 92-byte initializers and their column
  collapse (672), pattern-delay collapse (584), scatter (884), and random-delay
  collapse (564) callbacks. The initializers share an inline array-reset helper.
- Recovered the 84-byte motion record's texture coordinates and X/Y scales at
  offsets 16..28 from the renderer and grid setup. Offset 64 remains neutrally
  named because transitions reuse it for different parameters. The four effects
  independently delay, move, reshape and retire 1,024 illustration cells.
- Matching required preserving logical shifts after byte/halfword promotion,
  the full-width atan result until its native narrowing point, and the order of
  multiplication and Q12 scaling. No compiler flags or inline assembly changed.
  The adjacent 244-byte grid initializer remains deferred: its private candidate
  has a four-byte size gap and different induction-variable scheduling.
- A successful 12,000-frame replay uses the compatible checkpoint-86 field state
  and the previously verified credits-entry fixture. One decoded 72-byte command
  becomes opcode 0x122/fade=1 and is fully restored before native scene entry.
  This is controlled entry, not evidence of ordinary story completion.
- `build/analysis/probe_credits_effects.py` and
  `build/runtime/eur_credits_effects/evidence_story86_full.json` independently
  verify all eight functions, twice per initializer, and phases 0/100/101/102/103/104
  in both screen modes for each callback. The replay checks 812,589 complete
  particle records, 812,597 complete workspace prefixes and RNG words, and eight
  complete 86,016-byte arrays. It initializes and retires 8,192 cells, predicts
  28,672 RNG results, 2,048 table-derived angles across all four quadrants,
  8,192 acceleration setups, 6,144 ease-out setups and 520,050 motion ticks.
- The oracle derives coordinates, RNG, angle quantization, integer arithmetic,
  scales, frame counts and phase changes without refreshing helper results from
  observed output. An initial probe used the wrong resident-image base for its
  trig tables. The extracted image starts at 0x02004000; the corrected probe
  derives that base from metadata and guards the tables against live RAM.
  Failed probe attempts are not counted as successful runtime evidence.
- All 104 original saves remain unchanged. Hashes and lengths were checked for
  151 graphics/register/grid captures and 17 screenshots. Two transition images
  were visually inspected. All seven final VRAM/palette/OAM/display-register
  captures and the final PNG are byte-identical to the preceding full credits
  replay. No return-drain frames were needed. Idle phase 105 is checked 16 times
  per callback; 215,507 additional idle entries are counted without snapshots.
- The replay covers both column-delay directions and scatter wait/growth phases.
  Scatter's positive-X launch branch, an already-zero active count at retirement,
  RNG zero-seed fallback, invalid layout/screen values and abnormal counters
  remain unexercised. This does not claim physical-hardware testing.
- Full module/symbol checks, canonical packaging, native relink of 43 components
  with zero differing bytes, progress checks and all 81 tests pass. Both ROMs
  retain SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **694,384 / 1,563,700 bytes (44.41%)**; C/C++ plus symbolic
  assembly is **44.74%**. Overlay 6 is **45,664 / 66,492 (68.68%)**.
  The project-wide 50% milestone still requires 87,466 matching bytes.


## 2026-09-12: Credits spiral, image loading and music (+1,724 matching bytes)

- Added `CreditsSpiralCollapse_Update` (748 bytes) at `0x02076B7C`, directly
  extending the existing particle-transition module. Added the 84-byte delayed
  music stop and 292-byte music sequence task in `credits_music.c`, and the
  600-byte `CreditsImage_LoadNext` task in `credits_image_loader.c`.
- The spiral contracts a Q4 radius around the previous illustration center,
  advances an angle, then launches toward the next center and eases cells into
  place. The shared motion record remains 84 bytes; its remaining parameter
  words are signed and retain neutral names because other effects reuse them.
- Consolidated the 72-byte `MenuElement` layout in the shared frontend header,
  replacing the title-startup duplicate. Credits establish its parent pointer
  at 16, resource slot at 28, phase/counter at 32/36 and arguments from 40.
  The parent controller's argument 7 marks music completion. The workspace's
  final word at 0x8238 is now named `image_loading`; its total size is unchanged.
- The image task queues a texture read, waits for its archive key to disappear,
  decompresses 32 KB, queues sixteen texture chunks, reads/copies a 512-byte
  palette, queues its upload and clears the loading flag. Upload callbacks and
  the particle renderer remain native; private matching of the 100-byte texture
  callback is not counted until integrated. No inline ASM or compiler flags changed.
- A successful 12,241-frame replay uses the compatible checkpoint-86 state and
  the fully restored decoded-command fixture: wait 12,000 frames, press A for
  one frame, then wait 240 frames. It reaches all four functions and returns to
  the Nintendo/AlphaDream startup logos. This is controlled credits entry,
  not verification of ordinary story completion. No drain frames were needed.
- The independent spiral oracle checks 212,212 complete 84-byte records and
  workspace/RNG snapshots, including all active phases on both screens,
  84,304 polar positions, 98,304 motion ticks and 2,048 completed cells.
  Both the minimum radius-step clamp (8 cells) and normal calculation (2,040)
  are exercised. Idle phase 105 is checked 16 times; 51,980 further entries
  are counted without snapshots.
- The loader runs 330 times and completes all fifteen subsequent illustrations.
  An independent LZ10 decoder verifies fifteen complete images; byte-copy
  expectations verify fifteen palettes. All 240 texture upload indices and
  fifteen palette jobs are checked, along with resource-pointer clearing and
  deferred-removal flags. Archive completion is independently derived by walking
  the pending-key list. Allocation addresses and freshly created child prefixes
  are observed helper outputs; this does not independently verify the allocator
  or the archive I/O implementation.
- The music task runs 9,648 times through phases 0..3: it starts track 40, waits
  for completion, waits 120 ticks, loads track 43 and starts it. The probe checks
  9,272 music-state results and 74 loading-state results against live fields;
  loading is busy on 73 checks. The exit task runs once and calls the 32-frame
  music fade. Exact helper arguments and full task records are checked around
  calls; audio mixing and physical output are not independently verified.
- All 104 original save hashes remain unchanged. Fifty memory-capture lengths
  and hashes and six PNG hashes were checked. All seven pre-exit graphics/register
  dumps and the PNG match the preceding 12,000-frame replay byte for byte.
  A spiral image and the post-exit startup image were visually inspected.
  Evidence: `build/runtime/eur_credits_followup/evidence_story86_full.json`;
  probe: `build/analysis/probe_credits_followup.py` (both private).
- Unexercised branches include pending archive reads on the loader's polling
  frames, music cancellation before completion, a nonzero delayed-stop counter,
  exit before the completion flag, invalid states and allocation failures.
  The initial probe read the loader's next-pointer slot at +28 as its active
  flag; the native load establishes +32. That oracle error was corrected and
  the complete replay rerun successfully. Failed attempts are not counted.
- Final module/symbol checks, canonical packaging, 43-component native relink
  with zero differing bytes, progress checks and all 81 tests pass. Both ROMs
  retain SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **696,108 / 1,563,700 bytes (44.52%)**; C/C++ plus symbolic
  assembly is **44.85%**. Overlay 6 is **47,388 / 66,492 (71.27%)**.
  The 50% milestone still requires 85,742 matching bytes.


## 2026-09-12: Credits background animation, grid fade and radial delay (+1,044 matching bytes)

- Linked `CreditsCloud_Update` (96 bytes, `0x02079B98`) and
  `CreditsStar_Update` (220 bytes, `0x02079DAC`). Clouds advance by their stored
  horizontal velocity and wrap at the variant-specific boundary. Stars display
  their bright frame for four updates, then wait for an independently randomized
  interval. The neighboring draw routines remain native.
- Linked `CreditsRadialDelay_Update` (564 bytes, `0x020762EC`): cells wait, move
  toward the next illustration center, shrink, wait according to their stored
  radius and ease into their new grid positions. Its initializer remains native.
- Added `CreditsGridFade_Update` (164 bytes, `0x02077B5C`) to the adjacent motion
  module. The first cell advances the shared illustration alpha; the other cells
  retire immediately. The combined module preserves the original contiguous
  range `0x02077B5C..0x02077CE8`. Shared declarations were updated; no inline ASM
  or compiler flags changed. The isolated callbacks can be consolidated when
  their intervening native routines are recovered.
- The checkpoint-86 probe repeats the compatible field-state route, replacing
  and restoring all 72 bytes of one decoded credits command before transition.
  This is controlled credits entry, not evidence of ordinary story completion.
  It runs 12,000 frames with no pending returns or extra drain frames.
- Independent expectations pass for 556,586 returns: 155,467 cloud updates,
  131,549 star updates, 1,102 grid-fade calls and 268,468 radial-delay calls.
  Every recorded return checks the full 72-byte task or 84-byte particle,
  52-byte workspace and RNG word; helper results are not refreshed from observed
  output. Native function bytes and the original layout table are guarded.
- The replay covers both cloud variants, including 169 wraps (97/72), and 567
  complete blink cycles with 1,134 independently predicted RNG results. The
  initial fade visits alpha 0 through 31 and retires all 1,024 cells.
- Radial motion visits every active phase on both screens. It checks 2,048
  center targets, acceleration setups, radius delays, grid targets, easing
  setups and completed cells, plus 184,320 motion ticks. Signed division,
  fixed-point order and 32-bit wrapping are modeled explicitly. Idle phases
  are checked 16 times per callback; 63,410 additional grid-fade and 22,348
  radial idle entries are counted without snapshots.
- All 104 original save hashes remain unchanged. All 52 memory-capture lengths
  and hashes and six PNG hashes were verified. The final seven graphics/register dumps and
  PNG are byte-identical to the preceding full-credits baseline. The radial
  phase-103/screen-0 image and final credits image were visually inspected.
  Private evidence: `build/runtime/eur_credits_small/evidence_story86_full.json`;
  probe: `build/analysis/probe_credits_small.py`.
- Unexercised cases include radial launch with positive X velocity, an already
  zero active count at retirement, zero RNG seed, invalid states/layouts and
  overflow outside the observed input range. The captures do not independently
  verify the native drawing callbacks or physical hardware.
- Final module/symbol checks, canonical ROM packaging, 43-component native
  relink with zero differing bytes, progress checks and all 81 tests pass.
  Both ROMs retain SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **697,152 / 1,563,700 bytes (44.58%)**; C/C++ plus symbolic
  assembly is **44.91%**. Overlay 6 is **48,432 / 66,492 (72.84%)**.
  The 50% milestone still requires 84,698 matching bytes.


## 2026-09-12: Smash Eggs actor initialization, launches and hit windows (+1,292 matching bytes)

- Confirmed overlay 15 as Smash Eggs: the menu selection enters native callback
  `0x020C5B4C` with actor command 8. The menu's battle-context field at `+0x11A`
  still reads 2 there; active dispatch follows actor fields `+0x7C` and `+0x78`.
- Linked eight new routines: `UpdateHitWindow` (240 bytes),
  `BeginSecondaryLaunch` (508), its callback thunk (12), `BeginReverseAnimation`
  (196), `IsIdle` (32), `RestoreActors` (100), `InitializeHitMotion` (44) and
  `InitializeActorController` (160), all with the `Overlay15Attack_` prefix.
  Actor setup is consolidated into `actor_controller.cpp`, preserving the full
  `0x020C4028..0x020C4570` range. The lifecycle module covers
  `0x020C5440..0x020C5570`; intervening native logic remains separate.
- Recovered the embedded motion parameters and formation index in shared types.
  The complete block matches with actual C++ virtual calls, correct C linkage,
  a common motion pointer and native integer narrowing. No inline ASM or
  compiler changes were needed. The three earlier setup routines still match
  in the combined C++ module, and the battle-hit API now declares C linkage.
- The compatible checkpoint-83 battle menu comes from the previously recorded
  controlled Petey Piranha encounter (-32748, room 306, decoded opcode 0x11C).
  Its 72 command bytes were restored before battle transfer. This replay uses
  buttons only, including short A/B presses and X/Y support launches; it does
  not demonstrate ordinary Star Shrine boss entry.
- The final 2,110-frame replay reaches all eight new routines and the three
  preceding setup routines. Its 230 completed calls check 227 full 52-byte
  controller records, two full 24-byte motion records and one full 28-byte pair
  prefix. All returns match the caller stack pointer; none need drain frames.
- Both carrying formations initialize and restore. Six launches check 12 world
  positions and six model-offset pairs. Hit windows open six times and close
  five times across 113 updates with observed timer values from 10 to -11; one
  support sequence is interrupted by the attack's native failure path.
  The replay checks 82 idle results and one reverse animation, including its
  independently derived frame count, last frame and signed animation speed.
  There are 70 helper-argument checks and 22 direct object/model-field checks.
- Object/model pointers and other external helper effects are observed, not
  independently reconstructed by this oracle. A preliminary assertion wrongly
  used the menu field as active command; another used a four-byte animation
  stride. Current source/native loads establish the actor command and eight-byte
  animation records. The entire successful replay was rerun after both fixes.
- Selection, attack and final command-wheel screenshots were inspected. Captures
  retain controller records, OBJ VRAM, palettes, OAM and display registers during
  both support variants, plus final graphics dumps. All 104 original saves keep
  their hashes. Private evidence: `build/runtime/eur_overlay15/evidence_final83.json`;
  probe: `build/analysis/probe_ov15_actors.py`.
- Unexercised: formations 0..3, absent support objects/models, phase-17 idle,
  reverse animation for formation 4, and arithmetic outside the observed range.
  These captures do not independently verify native rendering or hardware.
- Full module/symbol checks, canonical packaging, the 43-component native relink
  with zero differing bytes, progress checks and all 81 tests pass. Both ROMs
  retain SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **698,444 / 1,563,700 bytes (44.67%)**; C/C++ plus symbolic
  assembly is **45.00%**. Overlay 15 is **1,960 / 13,164 (14.89%)**.
  The 50% milestone still requires 83,406 matching bytes.


## 2026-09-12: Smash Eggs targeting, badge motion and pair retreat (+584 matching bytes)

- Linked `Overlay15Attack_ApplyBadgeMotionAdjustments` (96 bytes),
  `SelectRandomEnemy` (120), `BeginPairRetreat` (260) and `HideAttackObjects`
  (108), all with the same prefix. The first two share the contiguous range
  `0x020C2AE0..0x020C2BB8`; retreat and hiding remain isolated across native gaps.
  Shared declarations support C and C++. The pair fields at +0x18/+0x1A are now
  named target actor ID/pending damage, as established by their attack consumers.
  No inline ASM or compiler flags changed.
- The neighboring rotation update at `0x020C31DC` remains deferred. Its 148-byte
  candidate still folds the Z-position addition/subtraction; the native function
  has 156 bytes. Keep the unlinked candidate separate from reported progress.
- Ordinary checkpoint-83 input replay: two target selections, two retreats and
  one hiding call, with 235 completed monitored calls including the preceding
  actor block. Full records, helper arguments, targeting order, RNG result/seed,
  direct object fields and formation-5 retreat parameters pass. Eligibility is
  observed helper output; only one enemy is selectable in this encounter.
- A second replay overrides the native `Overlay10Party_HasBadgeTwo` result in
  ARM9 R0 from 0 to 1 when returning to `0x020C5D30`. This single recorded
  register fixture enables the constructor's optional table update. All 60
  halfwords of the resulting 120-byte table are checked independently. The run
  completes 279 monitored calls, including that modifier and all other new helpers.
  This is not evidence of ordinary badge-equipping behavior.
- Both runs advance 2,110 frames, drain no pending calls and return to the
  command wheel. All 104 original save hashes remain unchanged. The ordinary
  run's 22 PNGs, nine final graphics dumps and two launch captures match the
  prior actor-batch baseline exactly. The badge run's 19 PNGs and corresponding
  dumps were hash/size checked; its attack and final images were inspected.
- Private probe: `build/analysis/probe_ov15_pair.py`; reports:
  `build/runtime/eur_overlay15/evidence_pair83.json` and `evidence_badge83.json`.
  Both retain the previous controlled encounter provenance; ordinary replay
  has no additional edits, and the badge report records its one register override.
  Unexercised: multiple/no eligible targets, zero RNG seed, 16-bit table overflow,
  retreat for even formations, and invalid/out-of-range inputs.
- Full module/symbol checks, canonical ROM packaging, the 43-component native
  relink with zero differing bytes, progress checks and all 81 tests pass.
  Both ROMs retain SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
- Matching C/C++ is **699,028 / 1,563,700 bytes (44.70%)**; C/C++ plus symbolic
  assembly is **45.03%**. Overlay 15 is **2,544 / 13,164 (19.33%)**.
  The 50% milestone still requires 82,822 matching bytes.


## 2026-09-12: Smash Eggs entry, egg launch and reward icon (+940 matching bytes)

- Linked `Overlay15Attack_UpdateEntry` (464 bytes), `BeginPairLaunch` (308)
  and `ShowRewardItem` (168). Pair launch and hiding now share `pair_motion.c`;
  the entry and reward display remain isolated across native gaps.
- Replaced the old private context views with one shared 584-byte layout.
  Extended the pair prefix to 36 bytes, keeping its later tail explicitly
  unknown. Its signed byte at +0x17 counts launches; phase/alternate-actor
  bits are at +0x16. Recovered hit count, angle and angle step at +0x1C/+0x1E/+0x20.
  The existing initializer, targeting, retreat and actor functions still match.
- Native stack arguments establish the entry movement call and launch timing.
  Reusing the duration variable preserves the native conversion order; compound
  negation of the signed angle step reproduces SMULBB. Shared bitfield access
  preserves the original alternate-actor extraction. No inline ASM or compiler
  flag changes were needed. The entry object is named `smash_eggs_entry.o` to
  avoid the linker's existing overlay-21 `attack_entry.o` basename.
- Full Ninja module/symbol checks pass. Canonical packaging with data mods
  disabled and the 43-component native relink both produce SHA-1
  `ba4ec2f99b4f2e0047601552bccf00aa73e28701`, with zero differing bytes.
  Progress regeneration/check and all 81 pytest tests pass.
- `build/analysis/probe_ov15_entry.py` extends the existing replay. The ordinary
  2,110-frame run checks 32 complete 584-byte entry contexts, one egg launch,
  five complete 36-byte pair records and the prior actor/motion paths.
  It verifies callback handoff, helper arguments, launch timing and coordinates,
  target/damage fields, flags and signed rotation. Distance and initial arc
  duration remain observed helper outputs, rather than independently verified
  helper algorithms. All 22 screenshots, nine final graphics dumps and both
  support-launch captures match the earlier baseline exactly.
- A second replay changes the initialized pair launch counter from 1 to 6 at
  frame 373 after a completed enemy hit. It changes one native reward-selection
  roll from 92 to 0 at caller 0x020C2F60, frame 496. Native transitions then
  create and display the reward. The icon routine runs at frame 574 with width
  79; flags, origin, anchor and model offsets pass their checks. Frame 584 has
  eight object/model and graphics captures plus a visually inspected screenshot.
  The final command wheel was also visually inspected. This is controlled
  end/reward coverage, not an ordinary six-launch sequence or natural drop odds.
- Successful reports: `build/runtime/eur_overlay15/evidence_entry83.json` and
  `evidence_reward_final83.json`; artifact checks in
  `build/analysis/verify_ov15_artifacts.py`. Both replays preserve all 104 source
  saves. Invalid entry phases, other reward widths/items, alternate launch
  damage, positive rotation and ordinary six-launch completion remain uncovered.
- Deferred: reward-message construction, drop selection, pair return/rotation
  and the main state machines. The separate reward-selection and failure-motion
  drafts still have classified mismatches and are not counted.
- Matching C/C++ is **699,968 / 1,563,700 bytes (44.76%)**; C/C++ plus symbolic
  assembly is **45.09%**. Overlay 15 is **3,484 / 13,164 (26.47%)**.
  The 50% milestone still requires 81,882 matching bytes.


## 2026-09-13: Credits gradient background (+388 matching bytes)

- Linked `CreditsBackground_DrawGradient` at 0x02079744 (388 bytes) in
  `credits_background_gradient.c`. The shared credits workspace now names its
  depth, polygon ID and background variant fields. Its complete size and all
  later offsets are unchanged. The following star/cloud sprite pass remains
  native and is not included in this byte increase.
- Recovered all repeated GPU FIFO writes from the instructions. An ordinary
  depth local preserves the native input load before the translation writes.
  Both color variants, matrix operations and four-vertex quad strip match
  exactly; no inline ASM or compiler flag changes were needed.
- Full Ninja module/symbol checks, canonical packaging with data mods disabled,
  native relinking, progress regeneration/check and all 81 pytest tests pass.
  Both ROMs retain SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`;
  the native relink checks 43 components with zero differing bytes.
- `build/analysis/probe_credits_gradient.py` replays 12,000 frames from the
  compatible checkpoint-86 state. The one 72-byte decoded credits command is
  restored at the request helper; this is controlled entry, not ordinary story
  completion. There are 11,959 completed gradient calls: 5,980 variant-0 calls
  and 5,979 variant-1 calls, with no variant override or drain frames.
- Independent expectations verify 298,975 ordered 32-bit GPU/workspace stores,
  complete 52-byte workspace records before the sprite helper and at return,
  143,509 nested star/cloud calls and the helper's workspace counter effects.
  Sprite/texture rendering internals remain outside the oracle. No incomplete
  monitored calls were discarded.
- Evidence: `build/runtime/eur_credits_gradient/evidence_story86_full.json`.
  `build/analysis/verify_credits_gradient_artifacts.py` validates three PNGs,
  27 memory dumps, state/ROM hashes, counter consistency and all 104 unchanged
  original saves. Screenshots at frames 1,500, 6,000 and 12,000 were visually
  inspected. The final PNG and all seven shared graphics buffers equal the
  preceding `eur_credits_small` 12,000-frame replay byte for byte.
- Matching C/C++ is **700,356 / 1,563,700 bytes (44.79%)**; C/C++ plus symbolic
  assembly is **45.12%**. Overlay 6 is **48,820 / 66,492 (73.42%)**.
  The 50% milestone still requires 81,494 matching bytes.


## 2026-09-13: Nawatobi followers and rope sprites (+500 matching bytes)

- Linked `NawatobiFollower_Create` (128 bytes), `NawatobiFollower_Update` (168),
  `NawatobiRope_UpdateAndDraw` (44), and `NawatobiRope_UpdateSubSprite` (160).
  The first three share the contiguous `nawatobi_render_tasks.cpp` region;
  the sub-screen callback remains separate across the native primary callback.
  Shared views describe the 60-byte participant position prefix, 72-byte tasks
  and existing 64-byte overlay-5 sprite layout.
- Follower animation uses three signed-height bands. Keeping the animation
  temporary as int and narrowing at the virtual call preserves the native mask.
  Ordered comparisons and the incremental screen adjustment match the native
  branches and arithmetic. Rope sprite copies retain a 208-pixel screen offset,
  clamped depth-derived draw list and source angle/scale. No inline ASM or
  compiler flag changes were required.
- Full module/symbol checks, canonical packaging, native relinking, progress
  checks and all 81 pytest tests pass. Both ROMs retain SHA-1
  `ba4ec2f99b4f2e0047601552bccf00aa73e28701`; the 43-component native relink has
  zero differing bytes. Neighboring initializer, primary rope draw, depth selector
  and scene update drafts remain unlinked with classified compiler differences.
- `build/analysis/probe_nawatobi_render.py` uses the compatible checkpoint-65
  pause state. Its sole RAM intervention changes the initialized pause task's
  phase from 2 to 7 once. Ordinary buttons then exercise menu entry/exit, Level 1
  and Level 2 in separate 491-, 1,351- and 1,619-frame replays. This verifies
  controlled entry and native level selection/updates, not normal story access,
  successful level completion or access to locked levels.
- Across the three replays: 24 follower creations, 12,488 follower updates,
  1,561 rope update/draw callbacks and 48,391 sub-screen sprite updates complete.
  All three follower animations run on both screens. Rope draw lists include
  both clamp results and interior values 1 through 51; values 52-62 remain
  unexercised. There are 282,637 helper-argument checks and 60,879 verified
  draw-list appends, with no pending calls discarded or drain frames needed.
- Oracles independently derive follower coordinates/animation choices, task
  fields, model flags, rope positions, depth values, fixed-point affine matrices
  and queue links. They account for task-list insertion and resource-attachment
  writes. Allocation/resource initialization, animation-helper side effects and
  native rope physics are observed outputs. Bitmap rasterization and animation
  internals remain outside this probe's oracle.
- Reports: `build/runtime/eur_nawatobi_render/evidence_menu65.json`,
  `evidence_level1_65.json`, and `evidence_level2_65.json`.
  `build/analysis/verify_nawatobi_render_artifacts.py` validates 21 screenshots,
  189 graphics dumps, ROM/state hashes, oracle totals and all 104 unchanged
  source saves. Menu entry/exit PNGs match the preceding Nawatobi baseline.
  Menu selection and both levels' moving-rope captures were visually inspected.
- Matching C/C++ is **700,856 / 1,563,700 bytes (44.82%)**; C/C++ plus symbolic
  assembly is **45.15%**. Overlay 7 is **55,832 / 142,264 (39.25%)**.
  The 50% milestone still requires 80,994 matching bytes.
