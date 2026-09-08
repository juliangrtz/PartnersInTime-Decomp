

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
