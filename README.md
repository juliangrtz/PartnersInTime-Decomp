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
- Every ARM9 and ARM7 runtime image now relinks as 43 components and 183
  independent, fixed-address ELF units.
- `BattleActor_GetPartySlot` and `BattleActor_GetById` are maintained symbolic
  ARM functions. Both use linker-resolved `gBattleContext` references and match
  their original bytes exactly.
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
  save/load.
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

The verified pass covers 43 components, 189 section units, 48 maintained
units, and 29,910 currently known relocations with zero differing bytes. To
iterate on one CPU family or overlay, use `tools/relink_arm7.py`,
`tools/relink_arm9.py`, or `tools/relink_overlay.py`. ROM-derived fallback
units, binaries, and JSON build reports remain below ignored `build/` paths.

The resident ARM7 startup is fully symbolic source. Its two large autoloads are
still conservatively marked as mixed code/data images; 41 proven autoload-0
units are maintained source and 330 autoload relocations are mapped, but
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
