# Mario & Luigi: Partners in Time decompilation and reassembly

This repository is a work in progress. Its long-term goal is a fully
source-buildable, mod-friendly reconstruction of the Nintendo DS game, not a
collection of binary patches.

No ROM, extracted game asset, generated machine-code dump, or proprietary
AlphaDream/Nintendo data belongs in this repository. You must supply your own
matching ROM. Generated sources and build outputs stay below `build/`, which is
ignored by Git.

## Current status

- The upstream `dsd` project layout covers the European and US ARM9, ITCM,
  DTCM, and ARM9 overlays.
- The bootstrap reassembler covers ARM9, ARM7, and every overlay entry directly
  from the NDS header and FAT.
- The verified European ROM round-trips through 39 generated assembly modules
  and LLVM with zero differing bytes.
- The raw generated sources initially use `.word` and `.byte`; they are a
  lossless starting representation, not a claim of semantic decompilation.
- Every ARM9 and ARM7 runtime image now relinks as 43 components and 292
  independent, fixed-address ELF units.
- Twenty-one named overlay-2 battle functions are maintained symbolic ARM source,
  together with the resident item-value calculator they call.
  They cover task enqueueing, actor lookup, enemy-load requests and 44-byte stat
  reads, packed object-ID resolution, object-data loading and pointer fixups,
  live actor/resource binding, both base and fully modified POW/DEF/level damage
  formulas, HP/KO updates, the complete enemy and party hit/popup/effect paths,
  status removal/reset, healing, revival, cures, stat items, and equipped
  healing-badge boosts. Every function matches its original bytes.
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
- Only the existing `src/` files are presently maintained high-level source.

See [`docs/REASSEMBLY_PLAN.md`](docs/REASSEMBLY_PLAN.md) for the staged route
from the fixed-layout bootstrap to a relocatable, size-extensible mod SDK.

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

The verified pass covers 43 components, 292 section units, 145 maintained
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
