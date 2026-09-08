

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
