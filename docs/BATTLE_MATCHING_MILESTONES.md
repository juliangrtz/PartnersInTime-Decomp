

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
