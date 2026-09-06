# Mario & Luigi: Partners in Time decompilation and reassembly

This repository is a work in progress. Its long-term goal is a fully
source-buildable, mod-friendly reconstruction of the Nintendo DS game, not a
collection of binary patches.

No ROM, generated machine-code dump, in-game text, event-script dump, data-table
export, graphical/audio asset, opaque extracted game binary, or proprietary
compiler belongs in this repository. Reconstructed source, reverse-engineering
metadata, original tooling, and documentation are versioned. You must supply
your own matching ROM and a lawfully obtained compatible compiler for
verification and packaging. Extracted data, local tools, generated sources, and
build outputs stay in paths ignored by Git.

## Decompilation progress

[![Mario & Luigi: Partners in Time decompilation progress](docs/progress.svg)](docs/PROGRESS.md)

The headline percentage counts byte-matching C/C++ against mapped ARM9 code.
Maintained symbolic assembly is shown separately and does not count as decompiled
source.
See the [progress methodology and regeneration instructions](docs/PROGRESS.md).

## Current status

- The upstream `dsd` project layout covers the European and US ARM9, ITCM,
  DTCM, and ARM9 overlays.
- The bootstrap reassembler covers ARM9, ARM7, and every overlay entry directly
  from the NDS header and FAT.
- The verified European ROM round-trips through 39 generated assembly modules
  and LLVM with zero differing bytes.
- The raw generated sources initially use `.word` and `.byte`; they are a
  lossless starting representation, not a claim of semantic decompilation.
- Every ARM9 and ARM7 runtime image now relinks as 43 components and 420
  independent, fixed-address ELF units.
- Decompilation is now proceeding across every real ARM9 overlay, with small,
  self-contained leaf routines taken first instead of waiting for one gameplay
  subsystem to be complete. The first cross-overlay pass adds 49 byte-matching
  C functions (2,264 bytes): field-entity lifecycle helpers in overlay 0,
  resource ownership and DS 2D-display helpers in overlay 5, enemy selection
  and value-scaling helpers in overlay 10, and attack-phase state helpers in
  overlay 21. The maintained [overlay map](docs/research/OVERLAY_MAP.md) records
  proven roles and the next triage targets without guessing at unknown modules.
- The next overlay pass adds fourteen more matching C functions (1,460 bytes)
  from special-attack overlays 12 and 20. They expose party-formation and
  animation state, the original four-member action-order shuffle, paired
  scene-object setup/cleanup, finish and mode transitions, actor-position
  restoration, and target-relative vertical-arc startup. The exact Bros. item
  identities remain conservatively unnamed until confirmed at runtime.
- Overlay 11 now contributes another seven matching projectile-attack helpers
  (708 bytes): level-scaled setup for scene object 40, animation timing,
  actor-contact testing, bounded motion, finish animation, and actor-resource
  restoration. Its shell-like behavior is documented without assigning an
  unverified Bros. item name.
- Overlay 13 contributes seven matching special-attack helpers (948 bytes).
  They advance motion tuning, prepare and align a paired scene-object setup,
  start its retreat and vertical-entry paths, classify a vertical angle into
  animation bands, and keep the active model animation synchronized. The
  original C++ virtual dispatch is represented directly instead of hidden in
  hand-written assembly.
- Overlay 14 contributes a complete nine-function attack-object state-machine
  block (932 bytes), covering approach and target motion, animation waits,
  hide/finish transitions, effects, sound, cleanup, and initialization.
- One hundred and thirty-four named overlay-2 battle functions are maintained symbolic ARM source,
  together with the resident item-value calculator they call.
  They cover task enqueueing, intrusive task lists and pools, actor lookup,
  packed object-ID resolution and ordinary object-data loading,
  live actor/resource binding, both base and fully modified POW/DEF/level damage
  formulas, HP/KO updates, the complete enemy and party hit/popup/effect paths,
  status removal/reset, healing, revival, cures, stat items, and equipped
  healing-badge boosts. Every function matches its original bytes.
- The ROM-backed battle-AI analyzer inventories all 182 dispatcher opcodes,
  their handler addresses, shared entries, command-record offsets, and direct
  call edges without copying ROM bytes into the repository.
- The complete `0x170`-byte resident ARM7 startup is maintained ARMv4T source:
  four functions plus its NitroSDK module parameters, all matching exactly.
- ARM7 autoload 0 now has exact symbolic sources for `ARM7_Main` and the
  NitroSDK `OS_IrqHandler`, including the cross-autoload call and IRQ/thread
  state references. `OS_Init`, the IRQ-table and arena initializers, IRQ-mask
  controls, `OS_SetIrqFunction`, and the `SVC_Halt` interworking thunk are
  symbolic as well. Thread bootstrap,
  switch-callback registration, CPSR/IRQ mask control, reset coordination, and
  the `SVC_Halt`/`SVC_WaitByLoop` Thumb wrappers, and `MI_StopDma` are also
  maintained. Lock initialization, lock-ID allocation, Game Pak lock
  primitives, PXI FIFO initialization and public APIs, the hardware-timer tick
  subsystem, alarm initialization, and the public Game Pak initializer are
  symbolic too. Callback-backed VBlank, timer, and DMA IRQ dispatch is also
  maintained with the SDK handler names, along with the nested scheduler
  enable/disable counter, alarm-backed thread sleep, priority updates, and
  context switching. Direct and wait-queue sleep/wakeup primitives are
  symbolic as well, including thread creation, current-thread resource
  release, exit, scheduler-list maintenance, ID allocation, and CPU context
  save/load. The blocking/nonblocking message-queue API is maintained too.
  Recursive mutex ownership and its thread wait queues are symbolic as well.
  The ARM7 heap allocator is maintained too, including arena initialization,
  allocation/free, consistency checks, and its ordered free-list helpers.
  V-count alarms now have symbolic frame tracking, IRQ dispatch, cancellation,
  one-shot/periodic setup, and sorted queue maintenance.
  DMA waiting and the ARM7 CPU fill/copy primitives are maintained as well.
  Periodic ARM7 sampling of the X/Y buttons and hinge state is symbolic too.
  Sound output, sleep, bias, master-enable, and shutdown controls are symbolic.
  Sound-channel pan, volume, timer, surround, PCM, PSG, and noise setup follows.
  Sound random, sine, logarithmic volume, and pitch/timer helpers are symbolic.
  The sound thread, message queue, interval alarm, and guarded startup are now
  symbolic too, including PiT's queue-overflow warning path.
  LFO modulation, PCM-wave invalidation, and hardware-channel locking are
  maintained as well.
  Extended-channel allocation, ADSR progression, PCM/PSG/noise voice startup,
  mixer calculations, and staged register updates are symbolic too.
- Maintained high-level source now includes the original Nitro math unit and
  six byte-matching resident VM functions used by battle and field script
  runtimes: the run loop, all eleven jump conditions, the command decoder, and
  the complete 51-command core executor plus its variable reader and writer.
  They expose yielding, call and loop stacks, control flow, integer and
  fixed-point arithmetic, descriptor flags, signed literals, VM-local state,
  save-data words/bytes/flags, and overlay extension hooks behind the editable
  BAI command format. Overlay 2's matching extension now exposes battle-script
  owner/target actor IDs and its 32 shared integer variables as editable C.
  Overlay 7's matching scene-script runtime likewise names its packed owner
  ID/type, latched input values, 32 shared variables, both global script
  slots, object-state lookup, and the complete object-script scheduler. The
  scheduler now exposes replacement versus queued execution, parent ownership,
  pause handling, VM result chaining, and task cleanup in editable C. The
  adjacent scene-manager lifecycle is reconstructed too: both global script
  starters, primary archive loading, all 56 object bindings, both reserved
  object-range resets, the 72 task slots, shared resource loading, controller
  start/stop/shutdown, and the sprite-follow callback are linked matching C.
  Readable WIP C also covers the depth-sorted renderer, secondary archive
  selection, eight-sprite factory, and its 964-byte transition controller;
  these register-sensitive units remain unlinked until their final instruction
  schedules match. The complete 9,196-byte scene command dispatcher now also
  exists as one structured, compiling C switch with named opcode families and
  all reserved slots preserved. Its current 9,284-byte build reaches 83.89%
  fuzzy instruction similarity and remains unlinked while the last branch,
  literal-pool, and Metrowerks register schedules are converged.
  The complete 19,168-byte battle-specific dispatcher is now represented the
  same way: one readable C unit covers all 182 slots from `0x033` through
  `0x0E8`, including actor damage/status handling, effect construction,
  inventory access, object-script ownership and waits, conditional branches,
  input control, and sound tasks. Unknown opcodes still delegate to the common
  battle VM executor. Fixed-opcode retries, the complete enemy-stat switches,
  object-position conversion, typed effect handles, movement-duration paths,
  the effect/task-slot scans, and the four 40-slot owner operations are now
  reintegrated into the monolith. Its current 19,168-byte function reaches
  76.21% fuzzy instruction similarity with no compiler-generated helper
  symbols left, and remains unlinked while the
  original giant-switch layout is converged without compromising the verified
  ROM build.
  Overlay 0's still larger 23,492-byte field/world dispatcher now has the same
  complete structured reconstruction. One readable C switch covers all 290
  slots from `0x033` through `0x154`: child scripts, entity state and movement,
  party control, room and time-hole transitions, cameras and palettes, field
  effects and input, battle/menu handoffs, save-backed inventory, the field
  timer, message windows, and audio ownership. Named runtime layouts replace
  raw offsets for the shared entity, render, party, and field state. All
  source-level helpers now fold into one 23,492-byte MWCC function, exactly
  matching the original function size; there are no compiler-generated code
  helpers left. Its common return path,
  cached field/party/map contexts, dynamic retry decoding, and argument-base
  register now reproduce the original dispatcher architecture, raising fuzzy
  instruction similarity to 48.02%. ROM-shaped bitfields now recover the
  entity subtype/resource selection, six-direction contact mask, map-axis
  synchronization, signed collision-policy updates, script ownership and
  lifecycle, saved presentation/animation state, transform and movement
  controllers, field-side/camera state, and signed block-bounce state without
  opaque raw arithmetic. Matching-script control flow and main/subscreen
  register paths now retain the original dispatch structure. The normal and
  transition-selecting battle starts, input-mask channels, timers, palette
  controls, roaming/waypoint controls, follower rejoin mode, and the complete
  five-argument active-party switch ABI also retain distinct ROM behavior.
  IDA verifies the persistent field-system
  pointer at `field_context + 0x24FC` and the separate map controller at
  `+0x2500`; the reconstructed prolog now loads both the field-system and party
  contexts in the same positions as the original. IDA
  confirms that the original outer 290-entry table funnels command families through several
  nested switches. The two large 49- and 47-opcode entity families now share
  one target lookup apiece and reproduce the original 91- and 96-entry inner
  tables, including all distinct case targets. The auxiliary-script and
  entity-script families do likewise; the remaining opcode bodies and local
  register schedules are the principal source of the size/layout delta
  against the 23,492-byte original. The work unit remains unlinked while those
  dispatch groups and the register layout are converged.
  High-level source also includes
  two hundred and six byte-matching overlay-2 battle functions: forty-four
  battle-AI/VM/target/state helpers, the eight-function generic task-pool unit,
  and four actor HP/lookup
  helpers plus status eligibility, base damage, the central HP/KO primitive,
  and party-healing feedback, plus the save-backed experience and level-
  threshold update. Three reward-counter effect functions, eight
  scene-object state/model/motion helpers, two DS square-root wrappers, a
  battle-relative position transform, both battle-animation setters, and the
  complete five-function hit-descriptor setup layer are high-level source too.
  The seven-function object-resource control block now exposes visual-resource
  binding, enemy stat initialization, pending-load checks, duplicate-load
  suppression, slot configuration, and heap-backed buffer allocation.
  Two matching texture helpers encode 8-1024-pixel S/T dimensions for the DS
  `G3_TEXIMAGE_PARAM` register.
  Four matching battle-interface helpers now select the active character's
  command menu, resolve ordinary and Bros. item-name resources, translate
  enemy stat/name IDs, and refresh the target-label layer. A matching enemy
  accessor exposes the stat record already bound to each loaded enemy actor.
  Two matching interface-layer helpers suppress redundant resource changes,
  pack layout and update modes into a pooled request, and queue the
  asynchronous layer refresh. The layer structure now names its pixel buffer,
  resource cursor, asset table, layout fields, and upload-control flags.
  Three matching level-up bonus helpers map the eight-phase roulette to a
  1-6 reward, move the stopped result object, and count the selected points
  into HP, POW, DEF, SPEED, or STACHE before synchronizing the active save
  stats. Their typed 36-byte party record exposes both base and active stats.
  Two matching growth-display helpers also derive the four automatic stat
  gains from cumulative character growth tables and start the five animated
  result rows at three-frame intervals.
  Eight more matching task helpers expose double-buffered battle-background
  loads/fades and the field-asset reload used when leaving battle.
  Eleven matching raster-effect functions expose finite and persistent
  scanline interpolation, the object-data-51 fade/load/fade transition, and
  the accelerating view-relative particle task used by battle-AI opcodes
  `0xC1`, `0xC2`, and `0xE8`.
  Seven matching grid-transition task functions initialize the two animation
  phases, wait on their shared angle/velocity state, coordinate battle-background
  toggling and asynchronous display capture, and retire the tasks together with
  runtime flag `0x40` when their 8-by-6 DS-geometry passes finish.
  Eleven adjacent matching line-transition functions manage two 32-strip wipe
  phases and a captured intermediate phase. Their typed state exposes the shared
  position/velocity arrays, staggered frame counter, capture configure/reset
  chain, and the child task that synchronizes the preceding grid transition.
  Seven matching cylinder-transition functions expose the ordinary and
  save-selected dual-screen paths, including VCount IRQ display swapping and
  clamped capture intensity. Two matching curtain-transition functions then
  advance and retire the following 50-column geometry wipe.
  Six matching cylinder-wipe callbacks implement the adjacent reveal,
  optional dual-screen rotation, display restoration, and captured finish
  sequence. Eight matching iris-transition callbacks then drive both radial
  phases and their asynchronous display-capture bridge over 64-frame passes.
  The adjacent six-function common-asset loader is matching C as well: it
  opens the battle archive, reads and relocates its offset table, selects the
  localized entries, and fills the twenty-one-slot runtime pointer table.
  Eight matching interface-asset functions then load the shared and localized
  UI resources, initialize four editable 2D-layer descriptors, and populate
  the two selectable screen-resource slots.
  Six adjacent action/party script-loader functions resolve packed object-data
  IDs, stream the selected BAI payloads into their dedicated buffers, and start
  the corresponding action or fixed party VM once asynchronous reads finish.
  Seven matching AI-system functions initialize the shared VM, register the
  battle opcode dispatcher, open all fourteen AI archives, initialize the task
  pools, and expose the special-handle reload plus actor-lock control helpers.
  Four adjacent actor/script commands test every hit lock, remove an enemy with
  optional damage feedback, configure a model animation layer through its
  original virtual interface, and test the three typed handle families.
  The following global-property reader is matching C and documents all values
  exposed by battle-AI opcode `0x46`, including actor selection, map/background
  state, fade progress, shared masks, and runtime flags. Its paired opcode
  `0x47` setter is fully structured C but remains unlinked while its final
  fade-toggle block has an eleven-instruction register-allocation mismatch.
  The 3,548-byte actor/object property reader is now matching C as well. It
  turns battle-AI property IDs into typed HP, POW, DEF, SPD, position,
  animation, resource, hit, transition, and scene/model state reads while
  preserving conservative offset names for fields not yet proven in play.
  Its 4,268-byte writer is matching, linked C too. Battle-AI opcode `0x4E`
  reaches it 6,042 times across the extracted BAI corpus to change those
  stats, HP, positions, animation/model flags, hit state, formation fields,
  and scene-operation channels through the same documented property ABI.
  The neighboring slot-swap routine is matching C as well. It exchanges scene
  objects between the field and actor slot banks, repairs both object IDs, and
  preserves the slot-specific actor flag when party or enemy entries trade
  places.
  The four-function scene-animation unit is matching C++ as well. It exposes
  component selection, party/enemy idle variants, linked-party formation
  offsets, resource-driven model replacement, and the renderer notification
  path while retaining the original model virtual calls.
  The adjacent seven-mode movement dispatcher is matching C and routes scene
  objects through immediate, relative, absolute, target-relative, ballistic,
  and accelerated motion primitives.
  Four matching enemy-data functions select one editable 44-byte `BDataMon`
  record, load its referenced object payload into a typed 0x200C-byte request,
  and fix its stat/object pointers after the asynchronous reads complete.
  The matching object-resource copier performs overlap-safe payload copies,
  rebases five embedded component pointers, preserves destination ownership
  fields, and schedules the resource upload when required. Its adjacent
  matching wrapper queues a new resource ID and records the pending-load flag.
  The preceding two matching callbacks reconstruct an object resource one
  component per task tick, expose the original C++ model interface and stream
  writer, build its component-offset table, and finalize the spare stream
  workspace after the last component.
  Ten following matching callbacks set up the resource's body, tail, and
  optional texture requests, drive both staged decode waits, queue the final
  transfer, and select sprite and texture uploads. The completion callback
  either clears the processing state or returns to the rebuild pipeline when
  a newer load is pending.
  The remaining game
  functions stay in symbolic assembly until an equivalent C translation
  reproduces their original code and layout. The adjacent two-function party
  formation/resource transition is also readable C; it remains on its original
  reference object until the last Metrowerks register-allocation differences
  are resolved.
- The local European editable-data exporter covers 21 multilingual MFset
  archives,
  10,510 battle/field dialogue strings, and all 98 enemy-stat records. Its
  inverse encoders also cover all 765 treasure records, reproduce every covered
  binary byte for byte before edits, and support length-changing text. Four
  shop datasets and 99 item-master records are editable through validated
  copies of their overlay-9 and resident-ARM9 tables. All 14 battle-scenario
  and enemy-AI archives are control-flow decoded from 243 entry points into
  81,854 reachable VM commands. Schema v2 gives known opcodes and variables
  semantic names, relocates labels after size-changing edits, rebuilds archive
  offsets, and preserves verified private data without checking opaque bytes
  into Git. The field, battle, and scene VM descriptor tables are now completely
  named, and every instance-specific opcode has an evidence-backed semantic
  contract. In particular, all 260 battle opcodes and all 189 opcodes reached by
  the original battle scripts are human-readable in the locally exported data
  source;
  no neutral `op_000`-style battle instructions remain. The 387,272 reachable
  field-event commands are exported as 638 room-level source files and compile
  through the normal data-mod/ROM build. A single strict high-level language now
  covers all three VM instances and 475,711 reachable commands: 638 Field room
  sources, 14 relocatable Battle archives, and all three Scene/Object archives.
  It reconstructs object-oriented calls, canonical counted loops, 2,739 Field
  inline scripts, and 115 fully decoded Battle inline scripts as scoped `async`
  blocks. The private all-VM corpus embeds 1,789 German Field-message references
  as searchable comments. Field dialogue and event changes are merged
  conflict-safely into their shared `FEvent/FEvData.dat` archive.

See [`docs/REASSEMBLY_PLAN.md`](docs/REASSEMBLY_PLAN.md) for the staged route
from the fixed-layout bootstrap to a relocatable, size-extensible mod SDK.
[`docs/DECOMPILATION_STYLE.md`](docs/DECOMPILATION_STYLE.md) records the
matching, source-organization, naming, and runtime-evidence rules for readable
high-level code.
The generated [`docs/research/BATTLE_AI_OPCODES.md`](docs/research/BATTLE_AI_OPCODES.md)
provides a compact navigation index for the large enemy-script dispatcher.
[`docs/research/SCRIPT_VM_SEMANTICS.md`](docs/research/SCRIPT_VM_SEMANTICS.md)
records the complete semantic coverage, compares the field/battle/scene
descriptor ABIs, and preserves the evidence and usage counts for each command.
The field and scene tables live in
`config/eur/field_vm.json` and `config/eur/scene_vm.json` and can be verified
against a private extraction with `tools/extract_script_vm_descriptors.py`.
[`tools/ida/README.md`](tools/ida/README.md) documents the reproducible IDA
9.1/9.2 ARM32 database imports and batch Hex-Rays helpers for the resident ARM9
and every overlay.
[`docs/DATA_MODDING.md`](docs/DATA_MODDING.md) documents editable text/stats,
control tokens, validation, ROM packaging, and the lossless high-level language
for the Field, Battle, and Scene VMs in
[`tools/pit_language_compiler.py`](tools/pit_language_compiler.py).
[`docs/research/RUNTIME_ANALYSIS.md`](docs/research/RUNTIME_ANALYSIS.md)
documents the py-desmume capture workflow, compatible-state creation, overlay
identification, memory hooks, and domain-specific battle snapshots.
[`docs/PC_PORT.md`](docs/PC_PORT.md) evaluates what a native PC build could
look like, which hardware boundary makes it feasible, and which source rules
keep that option open. It is an assessment, not a commitment; the verified ROM
build remains the primary goal and the correctness oracle.

## Verified European ROM

```text
Game code: ARMP
SHA-1:    ba4ec2f99b4f2e0047601552bccf00aa73e28701
SHA-256:  8b16b1f1f0aca4a78dae540be56a219adbde6ee3f1cd33d3f0fba777de01d6a3
```

## Bootstrap reassembly

Requirements:

- Python 3.11 or newer;
- LLVM tools `llvm-mc`, `ld.lld`, and `llvm-objcopy` on `PATH`;
- a lawfully obtained compatible Metrowerks ARM toolchain for matching C/C++
  builds, placed below ignored `tools/mwccarm/` or passed with `--compiler`;
- your own matching NDS ROM.

Check the environment:

```powershell
python .\tools\reassembly.py doctor `
  --version eur `
  --rom 'C:\path\to\your\PiT.nds'
```

Generate local assembly, assemble every native module, and require a completely
matching output ROM:

```powershell
python .\tools\reassembly.py all `
  --version eur `
  --rom 'C:\path\to\your\PiT.nds' `
  --output '.\build\PiT_eur_reassembled.nds' `
  --require-matching
```

Generated assembly is written to `build/reassembly/eur/generated/`. Editing a
generated module and running the `build` command produces a fixed-size code
mod. A maintained whole-module source with the same filename under
`reasm/eur/modules/` overrides the generated source.

This bootstrap deliberately refuses size changes and modifications to the
encrypted DS secure area. Those require the relocatable linker and encryption
stages described in the plan.

## Sectioned native-code relink

The Stage-1 linker supports resident ARM9, ITCM/DTCM, all ARM9 overlays,
resident ARM7, and both ARM7 autoload images. It validates both CPUs' serialized
autoload layouts, splits every component at verified section and
maintained-source boundaries, and links each one for its correct architecture
and runtime address:

```powershell
python .\tools\relink_native.py `
  --version eur `
  --rom 'C:\path\to\your\PiT.nds' `
  --output-rom '.\build\PiT_eur_native_relinked.nds' `
  --require-matching
```

The verified pass covers 43 components, 420 section units, 258 maintained
units, and 31,138 currently known relocations with zero differing bytes. To
iterate on one CPU family or overlay, use `tools/relink_arm7.py`,
`tools/relink_arm9.py`, or `tools/relink_overlay.py`. ROM-derived fallback
units, binaries, and JSON build reports remain below ignored `build/` paths.

The resident ARM7 startup is fully symbolic source. Its two large autoloads are
still conservatively marked as mixed code/data images; 118 proven autoload-0
units are maintained source and 1,558 autoload relocations are mapped, but
the upstream project contains no further ARM7 analysis. See
[`docs/research/ARM7_MAP.md`](docs/research/ARM7_MAP.md) for the exact confidence
boundary.

## Existing `dsd` matching-decompilation build

The upstream build remains useful as the byte-matching oracle. Put the matching
ROM in `extract/` under the filename documented there, then generate the Ninja
build with:

```powershell
python .\tools\configure.py eur
ninja
```

The two pipelines have different purposes: `dsd` supplies delinked objects,
relocations, linker layout, and `objdiff`; `tools/reassembly.py` supplies a
complete local assembly representation from day one.

The final `dsd` link substitutes compiled C only for translation units listed
in `config/eur/arm9/linked_sources.txt`. Every other C file remains available
to `objdiff`, while its original delinked object is kept in the ROM. Add a unit
to this list only after its code, size, and function order match. Experimental
edits to an already listed unit are intentionally linked even when they no
longer match; this is what makes source-level test mods possible.

## Build and run from CLion

The shared `Build and Run EUR NDS` run configuration regenerates `build.ninja`,
builds the editable C/assembly sources, packages `PiT_eur.nds` in the repository
root, and launches it in DeSmuME. Put your own matching ROM at
`extract/baserom_PiT_eur.nds` first; neither the private input nor the built ROM
is tracked by Git.

The same run configuration also detects the ignored local
`data/eur/project.json` and rebuilds editable documents into a staged NitroFS
before final packaging. It never changes the private extraction. Use
`-DisableDataMods` with `tools/build_nds.ps1` when an unmodified data tree is
desired. See [`docs/LOCAL_PRIVATE_CONTENT.md`](docs/LOCAL_PRIVATE_CONTENT.md)
before exporting or copying game-derived files.

Select `Build and Run EUR NDS` in CLion's run-configuration menu and press Run.
Its configured emulator is
`D:\NDS\DeSmuME\DeSmuME_0.9.13_x64.exe`. The configuration deliberately builds
the `rom` target without enforcing a match, so experimental code changes still
produce a testable image when their translation unit is listed as described
above. Use `ninja check` when you want to verify
byte-identical reconstruction separately. The wrapper also restores the
fixed-layout secure-area and header CRC fields that `dsd` does not preserve; it
refuses any unexpected header difference. A build without launching an
emulator is available in a terminal with:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\build_nds.ps1
```
