# Partners in Time reassembly plan

## Target definition

“Completely moddable” means more than producing an NDS file. The project is
finished only when:

1. ARM9, ARM7, ITCM/DTCM autoloads, and every meaningful overlay can be built
   from maintained source;
2. native code may move or grow without unidentified absolute references;
3. the BAI battle VM and FEvent field VM have lossless source formats;
4. proprietary archives can be extracted and rebuilt by tools without shipping
   their contents;
5. clean and modified builds pass automated structural and emulator tests.

Bit matching is a verification technique, not the final limitation. Original
functions should match while they are reconstructed; mod builds may deliberately
produce different code and layouts.

## Repository hygiene

Commit:

- human-written C/C++ and ARM/Thumb assembly;
- symbol, relocation, section, and format metadata;
- extraction/repacking tools and synthetic tests;
- hashes, documentation, and analysis results.

Do not commit:

- `.nds` files or extracted NitroFS contents;
- generated raw `.word`/`.byte` dumps;
- sprites, audio, text, maps, or other original assets;
- IDA databases or proprietary compiler binaries.

The bootstrap materializes ROM-derived data only below the ignored `build/`
directory. Public builds therefore require a user-supplied matching ROM.

## Verified executable inventory (EUR ARMP)

| Component | ROM location | Runtime address | Stored size |
|---|---:|---:|---:|
| ARM9 container | `0x00004000` | n/a | `0x56758` |
| Resident ARM9 | `0x00004000` | `0x02004000` | `0x55F40` |
| ITCM autoload | `0x00059F40` | `0x01FF8000` | `0x7A0` |
| DTCM autoload | `0x0005A6E0` | `0x027E0000` | `0x60` |
| Autoload descriptors | `0x0005A740` | n/a | `0x18` |
| ARM7 container | `0x0019F800` | n/a | `0x26F24` |
| Resident ARM7 | `0x0019F800` | `0x02380000` | `0x170` |
| ARM7 autoload 0 | `0x0019F970` | `0x037F8000` | `0xFA84` |
| ARM7 autoload 1 | `0x001AF3F4` | `0x027E0000` | `0x17318` |
| ARM7 autoload descriptors | `0x001C670C` | `0x023A6F0C` | `0x18` |
| ARM9 overlay table | `0x0005A800` | n/a | 37 entries |

There are 39 native module records in total: ARM9, ARM7, and 37 ARM9 overlays.
Seventeen overlay entries contain only a 32-byte placeholder. The remaining 20
overlays are real code/data modules. All overlay compression-size fields are
zero in the verified European ROM; executable overlay compression is therefore
not a blocker for this build.

The ARM9 container is serialized as resident image, 32-byte-aligned ITCM image,
DTCM image, then two 12-byte autoload descriptors. The relinker derives the
first three sizes from the DSD section maps and rejects the ROM unless both
descriptors reproduce the derived destination, stored size, and BSS size.

The ARM9 secure area begins at ROM `0x4000`, while executable entry is at
runtime `0x02004800`. The verified decomp base ROM is already in decrypted
secure-area form: its opening marker is `E7FFDEFF E7FFDEFF`, and DSD extracts
the resident image byte-for-byte with `encrypted: false`. The conservative
Stage-0 builder still blocks edits in this region until checksum and hardware
behavior are covered by tests.

The current `dsd` symbol maps list roughly 4,967 functions across about 1.49 MiB
of ARM9/overlay text. This includes SDK routines, thunks, and tiny helpers; it is
not a count of unique gameplay systems.

## Stage 0 — lossless fixed-layout bootstrap

Status: **implemented and byte-identical for EUR**.

`tools/reassembly.py`:

1. verifies the exact input hash and game code;
2. reads ARM9/ARM7 locations from the NDS header;
3. resolves every overlay payload through the overlay table and FAT;
4. emits local `.s` files using lossless `.word`/`.byte` directives;
5. adds zero-byte aliases for symbols already present in the `dsd` maps;
6. assembles every module with LLVM for ARMv5TE;
7. rejects every size mismatch;
8. patches the rebuilt payloads into a copy of the input ROM;
9. can require complete byte equality with the original.

Exit criterion: a new ROM with the original SHA-1. Achieved for EUR:
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

Limitations: no new relocation is generated, module sizes are fixed, and most
instructions are still represented as raw words.

## Stage 1 — relocatable symbolic assembly

Goal: every native byte is represented by assembler-aware sections and every
address-bearing location is explicit.

Status: **fixed-address native relink implemented for EUR**.
`tools/relink_native.py` discovers both CPUs' resident/autoload layouts,
validates four autoload descriptors, and links resident ARM9, ITCM, DTCM, all
37 overlays, resident ARM7, and both ARM7 autoloads as 43 components containing
420 independent units. Those units cover raw
`.text` fragments around maintained functions, `.init`, `.rodata`,
constructors, alignment padding, `.data`, and explicitly mixed ARM7 fallback
images. There are 31,138 currently known relocations. `BattleActor_GetPartySlot`
at `0x02076F44` and
`BattleActor_GetById` at `0x02076F64` are byte-identical C. Their
`gBattleContext` literal is emitted as `R_ARM_ABS32` and resolved by LLD from a
DSD-validated external definition. `BattleSceneObject_GetById` at `0x02076FB4`
now exposes the field, party, enemy, and auxiliary visual-object pools, while
`BattleSceneObject_GetActiveModel` at `0x020A3370` selects the primary or
alternate bound model. `BattlePosition_StoreViewRelative` at `0x02076B0C`
stores raw coordinates or subtracts either battle-view offset pair while
clamping depth to zero. Its `BattlePosition_StoreBattleRelative` caller at
`0x02087BCC` converts the battle anchor and vertical/depth offsets used by turn
effects. The adjacent matching helpers expose the asynchronous DS square-root
unit and the object/pool-ID forms of the standard battle-animation setup.
The matching `0x02087C84`-`0x02087EC4` task group makes the scene transition
boundary explicit as well: it loads a new background into the inactive buffer,
toggles the active buffer, drives the resident background fade API, and reloads
the current map's field assets after battle. A typed partial `BattleContext`
now anchors the background ID at `+0x3A`, runtime state at `+0xD000`, and its
flags at `+0x3A0`.
The adjacent `0x02087EC4`-`0x020884E8` common battle-asset loader is matching C
too. It exposes the asynchronous archive-open/read sequence, offset-table
relocation, the twenty-one-entry runtime pointer table, and the language-based
entry mapping driven by save byte `+0x515`. The partial context now also types
the shared archive request at `+0x140`, the pointer table at `+0x68FC`, and the
runtime archive source/header at `+0xE160/+0xE168`.
The following `0x020884E8`-`0x02088BDC` interface-loader family is also linked
matching C. Its eight functions recover the two-stage shared/localized asset
load, two selectable screen-resource loads, four typed 2D-layer descriptors,
and their original allocation sizes, coordinates, dimensions, and VRAM
offsets. No opaque graphical asset is checked into the repository.
The adjacent `0x02088BDC`-`0x02088EF4` action/party script-loader family is
linked matching C too. It resolves packed object-data IDs, schedules aligned
archive reads into the action and party script buffers, waits on the shared
resource request, and starts the corresponding battle-AI VM. The one compiler
literal-load anomaly is confined to a documented inline-assembly fragment;
the loader state machine and public request APIs remain editable C.
The task-list and task-pool functions at
`0x020A50D4`-`0x020A5294` are byte-matching C and expose callback iteration,
owner-slot invalidation, deferred recycling, explicit free-list access, task
insertion, allocation, and fixed-payload pool initialization. The model- and
sprite-effect constructors at `0x020ACB44`-`0x020ACD18` are symbolic too,
including resource-table lookup, coordinates and scale,
owner-slot binding, and the 64-entry tracked sprite-effect table.
The four-slot delayed battle screen-effect scheduler at `0x02065E30`-
`0x02066004` is symbolic too. It exposes immediate/deferred preset dispatch,
slot allocation, countdown and primary/secondary routing used by damage and KO.
The scene-object animation activity helpers at `0x02091A18` and `0x02091A58`
validate their channel/object IDs and expose the four per-object animation
slots used by reaction state machines. Their full `BattleSceneObject_SetAnimation`
producer at `0x02091198` is maintained too: it classifies party/enemy objects,
creates or retires models, selects party status/HP animation variants, handles
linked party forms, and notifies the scene after resource changes.
The remaining general battle object-data path is symbolic: its 48-byte state
lookup, asynchronous queue setup, duplicate-resource guard, ordinary-slot
routing, and selection of eight large enemy-load slots are exposed by the
functions at `0x02077058`, `0x02089EEC`, and `0x02092048`. The four callbacks
that actually fill each large enemy-data slot are matching C below.
`BattleObjectData_CopyResource` at `0x02089320` is linked matching C. It uses
the original overlap-safe forward/backward copy helpers, adjusts the copied
length around an active stream writer, rebases five internal component
pointers, and carries only the two appropriate source flags into the existing
destination slot before scheduling its resource upload.
The party knockout task pair at `0x020A90F4` and `0x020A9280` now exposes
status clearing, animation completion, actor/global locks, form-specific sound
pairs, linked-character movement and the follow-up character-load callbacks.
The complete twelve-callback Mario/Luigi linked-knockout recovery graph from
`0x020A87F4` through `0x020A90F4` is symbolic too, including resource and
animation waits, timed retreats, Baby Mario/Luigi reloads, actor rebinding,
lock transfer and the ordinary/enemy load-pending test at `0x02091EDC`.
The launch-reaction closure additionally includes immediate/interpolated
scene-object deltas, their fixed-point update callback, DS-hardware square-root
ballistic velocity setup, and the form-specific landing effect. The surrounding
motion infrastructure now exposes coordinate snapshots, synchronized target
updates, active-object list membership, channel lookup and initialization, and
motion-list unlinking. Its normalized accelerated-motion solver and per-frame
callback are maintained too, including both DS square-root operations, signed
quadratic-root selection and the fixed-point terminal correction.
The enemy AI task bridge at `0x0207E928`-`0x0207EE1C` now exposes the distinct
action and reaction VM states, sorted per-actor task insertion, 184-byte state
initialization, script pointer binding and completion handling through
`VM_Run`. Its order-wait gate scans both task families, compares the signed
script-order field with an actor-ID tie break, and resumes a paused state only
when no earlier live task remains; this register-sensitive gate remains
maintained symbolic assembly. The byte-matching C stop path clears one of four
party VM slots directly or decodes typed IDs `0x1000`-`0x4000`, removes the
sorted task from the corresponding pool, and invalidates its attached script
state. Hit-queue damage against an enemy explicitly starts its reaction VM.
The other two VM families now expose byte-matching C update paths as well.
Object scripts use fixed 192-byte states and may save a continuation pointer
plus inherited ordering fields; VM result 2 activates that continuation in the
same frame.
Enemy auxiliary scripts use the actor-embedded state at `+0x1E0`. Both task
families are initialized, linked, updated, and completed in byte-matching C.
The action and reaction callbacks are matching C too, as is the common helper
that finds an existing actor task or inserts a new node in actor-ID order.
Their common starter now selects the enemy's action or reaction state, clears
its 184-byte VM state, binds the resource script, and attaches the sorted task
in matching C.
The resident `VM_WriteVariable`, `VM_ReadVariable`, `VM_Run`,
`VM_CheckJumpCondition`, `VM_ExecuteCommand`, and `VM_ReadCommand` functions at
`0x020055F8`, `0x02005938`, `0x020063A4`, `0x02006414`, `0x02006508`, and
`0x020071B0` are byte-matching C as well. They expose the yield countdown and
read/execute loop, all eleven comparison modes, and the complete 51-command
core executor: call and loop stacks, branches, table reads, integer and
fixed-point arithmetic, trigonometry, random values, and extension-command
fallback. The variable layer maps its encoded namespaces to VM-local state,
save-data word/byte/bit regions, and runtime-overlay extension hooks. The
decoder implements the 260-entry command descriptor format used by the BAI
JSON assembler: optional result variables, optional argument-mode masks,
signed literals, and variable-resolved arguments.
Overlay 2's `BattleVM_ReadVariable` and `BattleVM_WriteVariable` extension is
matching C too. It resolves the current battle owner and target actor IDs,
reads three filtered context masks, and provides the 32 shared signed script
variables at battle-context offset `+0x69E4`. Unproven context fields remain
named by offset until script analysis or runtime traces establish semantics.
The adjacent matching helper unit resolves all fixed and typed Battle-AI state
IDs and exposes four target selectors used by dispatcher opcodes `0x48`,
`0x73`, and `0xE1`: three thresholded enemy-HP scans and uniform reservoir
sampling across eligible status targets.
The common script-ID router, four fixed party-VM starters, script-block state
initializer, all-family per-frame updater, and task-pool initializer are now
byte-matching C as well. This closes the scheduler around the large opcode
dispatcher: all four typed families and all four party slots have named start,
update, continuation, cancellation, and allocation paths.
`tools/analyze_battle_ai_dispatch.py` validates the user-supplied ROM and
extracts the dispatcher's complete 182-entry jump table into JSON or Markdown.
The report also groups shared case entries, observes direct command-record
loads, and joins DSD call relocations to their current symbolic names. This is
the working index for translating the remaining `0x4AE0`-byte dispatcher.
`BattleDamage_CalculateBase` at `0x0209BF38`
and `BattleDamage_CalculateAttack` at `0x02071C84` expose the compact and full
level/POW/DEF calculations, including Q8 modifier tables, category minima,
global battle scaling, and clamps. `BattleActor_ApplyDamage` at `0x0209D694`
resolves actors symbolically, subtracts positive damage, clamps HP to zero,
sets the KO flag, and byte-matches overlay 2. `BattleStatus_ClearEffect` at `0x0209C278` and
`BattleStatus_ClearAll` at `0x0209C3F8` now expose individual ailment/stat
removal, actor-local timer/delta cancellation, base-stat restoration, and the
unrolled eight-effect reset path. The shared cancellation dispatcher at
`0x020A8320` covers all eight status IDs for party and enemy actors.
The adjacent `BattleStatus_TryApply` is maintained in full, including enemy
resistance fields, RNG chance scaling, equipment guards, ailment-state setup,
temporary stat calculation and clamping, sound cues, and effect spawning.
`BattleDamage_ApplyToEnemy` at `0x0209D718` is maintained in full: it resolves
the enemy slot, clamps and applies damage, sets the hit reaction, computes the
popup position, emits the number, and selects the two special impact effects.
The shared `BattleDamage_SpawnNumber` constructor at `0x0209CD9C` applies the
battle-wide popup offsets and creates either a free or actor-attached number.
Its three maintained callbacks merge overlapping values, transition the popup
according to actor flags, and release the actor's `+0x2C` task pointer.
`BattleDamage_ApplyToParty` at `0x0209D9DC` now covers the parallel party path,
including the nonlethal hit type, status-1 clearing, six reaction variants,
Mario/Luigi popup metadata, and optional post-hit status application.
`BattleDamage_DispatchHit` at `0x0209DE8C` validates target IDs, derives hit
offsets from the queued record and scene object, then routes the pending damage
and status payload to the appropriate maintained enemy/party path.
The party/enemy reaction-task starters and the party first-hit equipment hook
are symbolic too, exposing task allocation/reuse, actor hit-lock flag `0x200`,
attached hit effects, and equipment effect `0x3024` consuming POW status 6.
Both reaction-task callbacks are maintained too: they wait for the scene
animation, handle the party KO/alive split, restore enemy impact offsets where
applicable, clear task callbacks, and release actor hit-lock flag `0x200`.
The alternate attached-effect reaction pair is maintained as well, including
task reuse, effect replacement, resource validity, and hit-lock release.
The party launch/impact task pair is now maintained too, including off-screen
phase changes, the three-effect burst, HP-dependent restore, and task cleanup.
`BattleDamage_ReflectQueuedHits` walks the packed eight-entry hit queue,
recalculates reflected damage, swaps source/target IDs, and refreshes the
status ID, chance, and magnitude bytes from the reflected attack descriptor.
The maintained 16-byte hit-descriptor helpers at `0x0209E0B4`-`0x0209E20C`
and `0x0209EBAC` configure source/target IDs, callback, hit kind and status
payload, link active descriptors, resolve them by actor ID, and disable them.
`BattleCollision_GetBounds` at `0x0209E918` adds the party-form, special-object,
and resource-backed collision volumes consumed during hit expansion.
Its resource path now reaches the maintained `BattleModel_GetAnimationBounds`
at `0x020681E8`, which temporarily advances or rewinds the live model when
requested, decodes the selected animation frame's compact signed origin and
extent metadata, applies optional model scaling, and restores the original
frame state. Missing metadata falls back to the original `[-16,-32,16,0]` box.
`BattleCollision_TestObjects` at `0x0209EBFC` iterates those bounds in world
coordinates and returns the collision result plus hit position.
The complete `BattleHitQueue_Update` compiler at `0x0209E20C` dispatches the
old queue, expands active descriptors and wildcard targets, filters collisions
and duplicates, calculates pending damage, and prepares the next eight records.
The adjacent signed effect-variant setter at `0x0209DFDC` is maintained too.
The collision chain now reaches the maintained swept-volume solver at
`0x0209EF3C`, including fixed-point entry/exit time and impact interpolation.
`BattleDamage_CalculateByObject` now exposes the scene-object-to-actor bridge,
target immunity flag, active-turn mode selection, per-party-member Q8 inputs,
equipment effect `0x301B` (140-percent damage), and final 999 clamp.
`BattleDamage_ApplyEquipmentModifiers` and its 25-percent-HP helper are exact
source as well. They expose defender multipliers 25/50/150 percent, attacker
multipliers 50/150/250 percent, and the inventory-count-dependent modifier for
equipment effect `0x301D`.
`BattleItemEffect_Apply` at `0x02076584`, its healing-badge multiplier at
`0x020768A4`, and resident `ItemEffect_CalculateValue` at `0x02018F48` expose
fixed and percentage healing, revival, cures, POW/DEF/SPD items, maximum-HP
clamps, and the 150/200-percent equipped-badge bonuses.
The matching `0x0207F080`-`0x0207F5A0` reward/effect unit owns the coin and
experience counter sprites used by the post-battle tally and the visible
sprite/model/number feedback used when a party member regains HP. Its typed
effect and position records also distinguish the signed `+0xEA` visual anchor
from the scene object's actor ID at `+0xEC`.
The adjacent party progression work now separates exact gameplay logic from a
still-matching-in-progress visual transition. `BattleParty_AddExperience` at
`0x0207FC78` is linked byte-matching C and documents the save record's two
24-bit experience fields, all four character growth tables, and the level and
experience caps. `BattleParty_StartFormationTransition` and
`BattleParty_UpdateFormationTransition` are grouped in a readable formation
source unit with equivalent control flow and exact function sizes. Until their
remaining compiler register-allocation differences are eliminated, the linker
keeps the original delinked object for those two functions.
`BattleTaskQueue_Enqueue` at `0x020726B0` is matching C. The four-function
enemy-data pipeline at `0x02088EF4`-`0x020890B4` is linked matching C as well:
`BattleEnemyData_RequestLoad` and `BattleEnemyData_LoadStatRecord` enqueue the
request and select one 44-byte `BDataMon.dat` record. The adjacent
`BattleEnemyData_LoadObjectData` and `BattleEnemyData_FixupObjectPointers`
stages cover the 8,148-byte object read and its offset-to-pointer conversion.
The typed 0x200C-byte request exposes its selected stat record and object-data
pointers without checking either payload into Git. `BattleObjectData_ResolveSlot` at `0x0209234C`
decodes packed object IDs into 44-byte runtime descriptors.
`BattleEntity_BindResource` at `0x02091C20` now exposes the party-resource
binding branch plus enemy HP, POW, DEF, SPD, level, trait, and initial-flag
setup. All resident ARM7 bytes are maintained
ARMv4T assembly or symbolic module parameters. One hundred and eighteen maintained units
from autoload 0 now cover `ARM7_Main`, its `SVC_Halt` thunk, `OS_Init`, IRQ mask
and
handler registration, `OS_IrqHandler`, callback-backed VBlank/timer/DMA IRQ
dispatch, scheduler suppression, alarm-backed sleep, priority scheduling and
context switching, direct and wait-queue sleep/wakeup, thread creation and exit,
thread bootstrap and switch-callback
state, thread-list/ID/context primitives, message queues, mutexes, the heap
allocator, V-count alarms, DMA waiting, CPU memory primitives, X/Y button
sampling, sound output/sleep/shutdown, channel control and utilities, the sound
thread/message/timer/startup path, capture configuration, LFO/sweep modulation,
voice priority/initialization, sequence-track allocation/player dispatch and
note-to-voice translation, channel/track lifecycle, and cached sequence-data
decoding, playback controls, preparation, runtime update, wave lookup, and
instrument/bank resolution, shared status/sequence-variable publication, and
sound-alarm scheduling/PXI notification, and low-level sound-command timer,
channel, driver-info, FIFO/queue initialization helpers, plus all 34 ARM7
sound-command opcodes, ARM7 file-system/CARD-back-end initialization, CARD
worker-priority control, common CARD worker/PXI initialization, asynchronous
CARD request dispatch, PXI request reception, and backup-memory erase, verify,
read, write, and program operations, their bytewise SPI transfer engine,
VCOUNT-bounded command completion, ARM7 CARD ROM-service initialization, and
ROM-ID/data request setup, page reads, cache-backed unaligned transfers,
ARM9-command-to-ARM7-operation dispatch, and cartridge-removal polling,
notification, ROM-ID comparison, and termination callbacks,
wave invalidation, channel locking,
extended-voice allocation, envelopes, mixing and hardware commits,
CPSR interrupt
control, reset coordination, IRQ-table, arena, lock-ID,
and Game Pak lock setup, PXI FIFO initialization and public send/callback APIs,
the timer-0 tick subsystem, alarm initialization and scheduling, the Game Pak
initializer, all 25 Thumb SVC wrappers, the population-count helper, SPI
PXI/worker dispatch, the common SPI request ring, result path and exception
lock, complete SPI service/thread initialization, and touch-panel worker
sampling, alarm scheduling, pen-state debounce processing, and touch-command
validation/queueing, VAlarm-pool setup, touch SPI initialization, and complete
sample/status encoding, five-sample channel filtering, and pen-signal detection
through SPI/RCNT, followed by power-command execution for sleep, utility,
register, and LED requests, plus PXI fragment decoding and validation for the
same command family, PM state initialization, PXI response encoding, and
power-control bit updates, direct PMIC SPI register transfers, and LED/utility
state switching, sleep entry, wake-source configuration, and runtime-state
restoration, plus LED-pattern storage and periodic blink evaluation. They
include wireless service parameter setup, shutdown/initialization, and the
cross-autoload `WMSP_Init` call, microphone timer sampling/IRQ publication,
one-shot and buffered sampling control, timer-prescaler selection, and PXI
microphone command/parameter validation, state initialization, raw 8-/12-bit
ADC transfers, and the nested microphone IRQ veneer, dispatch table, and IRQ
function installation controls. There are 1,558 verified autoload relocations; ARM7 now has
1,577 verified
relocations in total. Every linked component and the resulting NDS have zero
differing bytes from the verified European ROM.

Work items:

1. Normalize the existing `symbols.txt`, `relocs.txt`, and `delinks.txt` files
   into one machine-readable module graph. Overlay parsing and validation are
   implemented for the EUR ARM9 graph and all resident ARM7 bytes. The mixed
   ARM7 autoload images still need full symbol and relocation discovery.
2. Split ARM9 and overlays into `.text`, `.rodata`, constructors, `.data`, BSS,
   ITCM, and DTCM according to the verified `dsd` boundaries.
3. Emit one assembly translation unit per delink unit rather than one flat
   module dump. Fixed-address section units are implemented for every EUR
   ARM9 component; source/function boundaries are the next split level.
4. Convert branch/call words to symbolic ARM or Thumb instructions only when
   their target and execution mode are proven.
5. Convert literal pools, pointer tables, vtables, jump tables, and constructor
   lists to explicit relocation expressions.
6. Keep ambiguous bytes as `.word`/`.byte`, but classify them and attach an
   expected-byte test.
7. Link with a generated linker script at the original addresses and compare
   each module byte-for-byte. Achieved for all EUR ARM9 and ARM7 runtime
   components.

Exit criterion: deleting the locally generated flat module source does not
change the matching build; every byte comes from sectioned source units.

The dangerous cases are ARM/Thumb interworking, function-pointer low bits,
PC-relative literal loads, switch tables, duplicate overlay runtime addresses,
and pointers into ITCM/DTCM autoloads. Each conversion must be guarded by an
object-level diff.

## Stage 2 — semantic native-code reconstruction

Goal: replace symbolic assembly with maintainable C/C++ without losing known
behavior.

Recommended order:

1. identified NitroSDK/library functions;
2. small leaf functions and accessors;
3. resource and archive loaders;
4. save/inventory/player-state structures;
5. battle actors, damage, statuses, items, and rewards in overlay 2;
6. field actors and maps in overlay 0;
7. menu/shop/load overlays;
8. the FEvent and BAI VM implementations;
9. giant dispatchers only after their leaf operations and state structures are
   named.

Use the original Metrowerks ARM compiler and exact flags for matching work.
Every function gets a declaration, ABI/type tests, an `objdiff` match record,
and at least one behavioral owner or caller. IDA pseudocode is evidence, not
source of truth.

Exit criterion: all native modules build without locally generated ROM-byte
sources. ARM7 may progress independently, but cannot remain a permanent opaque
binary under the stated project goal.

## Stage 3 — size-extensible mod linker

An exact reconstruction still inherits the original memory layout. Serious
mods need controlled growth.

Work items:

- reserve a new mod-code overlay or another explicitly managed executable
  region;
- generate trampolines for replaced fixed-address functions;
- distinguish stable ABI entry points from movable internal symbols;
- regenerate every known relocation after layout changes;
- update overlay RAM/BSS sizes and reject collisions with heaps or neighboring
  regions;
- use the ROM builder to regenerate FAT/overlay metadata instead of patching
  fixed file offsets;
- add branch-range/veneer handling for ARM and Thumb calls;
- define a small public mod API rather than exposing all globals directly.

Exit criterion: a test mod can add code and data larger than the original
function/overlay while booting from a rebuilt ROM and leaving unrelated modules
unchanged.

## Stage 4 — script and data source formats

Native-code completion alone does not expose the game. PiT stores much of its
behavior in proprietary archives and bytecode.

The first data-source milestone is implemented for EUR.  Versioned JSON plus
`tools/data_mod.py` now round-trips all 21 MFset archives, every localized
battle and field-dialogue chunk (including field event labels), the 98 enemy
records, all 765 treasure records, four tiered shop-stock datasets, and the 99
resident item-master records. It also decodes all 14 `BAI_scn_*`/`BAI_mon_*`
and related battle-VM archives: 230 blocks, 242 entry-point scripts, and
129,127 commands. The fixed-boundary assembler recompiles those commands while
splicing in verified private tails that have not yet been decoded. The normal
CLion/PowerShell build stages these sources into a derived NitroFS plus copies
of patched ARM9/overlay data and still produces the verified ROM hash when the
JSON is unchanged. See `docs/DATA_MODDING.md`.

Priority formats:

1. label control flow and name semantics in the implemented `BAI_scn_*` battle
   scenario and `BAI_mon_*` enemy-AI command assembler;
2. `FEvent/FEvData.dat` field events, flags, warps, and story progression;
3. add/remove support for the implemented enemy, treasure, shop, and item
   tables after their executable count references are understood;
4. field maps, object placement, sprites, palettes, animation banks, text, and
   audio containers.

Each format needs a lossless `extract -> source representation -> rebuild`
roundtrip, schemas with preserved unknown fields, and synthetic fixtures. Tools
may consume the user's ROM. Understood non-graphical text/stat data may be
versioned as reviewable source; opaque binaries and graphical/audio assets stay
private and ignored.

Exit criterion: a mod can add or replace an encounter, enemy script, field
event, item, and associated resources through documented source files.

## Stage 5 — continuous verification

Every pull request should run:

- source/config format checks;
- assembler/compiler builds for EUR and later USA;
- `dsd check modules` and `check symbols` for matching targets;
- exact changed-byte/module manifests for mod targets;
- lossless roundtrip tests for every archive and VM assembler;
- deterministic rebuild checks;
- emulator smoke tests covering boot, title, load/save, field entry, normal
  battle, boss battle, menu, and shop.

Hardware smoke tests remain valuable for DMA, timing, audio, and secure-area
behavior that permissive emulators may hide.

## Immediate execution order

1. Keep the Stage-0 matching build green.
2. Continue promoting small overlay-2 battle leaf functions using the seventy-four
   exact symbolic actor/damage units as the template.
3. Return to the `ARM7_Main` call graph when game-code dependencies require it;
   recover further ARM/Thumb boundaries and relocations without blocking the
   overlay-2 work on unrelated SDK cleanup.
4. Split section fallbacks at DSD translation-unit boundaries, then retire
   fixed-layout patch registration.
5. Promote the confirmed battle actor/stat structures into maintained headers.
6. Replace a tiny matching function deliberately, build a nonmatching mod ROM,
   and test it in an emulator.
7. Expand outward by translation unit while never accepting unexplained diffs.

This order gives a playable artifact at every stage without treating original
binary slices as the desired final architecture.
