

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
