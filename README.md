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
- The resident ARM9 image, ITCM, DTCM, and all 37 ARM9 overlays now relink as
  40 components and 131 independent, fixed-address ELF units.
- `BattleActor_GetPartySlot` and `BattleActor_GetById` are maintained symbolic
  ARM functions. Both use linker-resolved `gBattleContext` references and match
  their original bytes exactly.
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

## Sectioned ARM9 relink

The Stage-1 linker supports the resident ARM9 image, its ITCM/DTCM autoloads,
and all uncompressed ARM9 overlays described by the existing `dsd` maps. It
validates the serialized autoload descriptors, splits every component at its
verified section and maintained-source boundaries, and links each one at its
runtime address:

```powershell
python .\tools\relink_arm9.py `
  --version eur `
  --rom 'C:\path\to\your\PiT.nds' `
  --output-rom '.\build\PiT_eur_arm9_relinked.nds' `
  --require-matching
```

The verified pass covers 40 ARM9 components, 131 section units, and all 29,561
known ARM9 relocations with zero differing bytes. To iterate on one overlay,
use `tools/relink_overlay.py --overlay-id 2` and supply `--output-bin`. Every
external used by maintained source is validated against both `symbols.txt` and
a supporting relocation record. ROM-derived fallback units, binaries, and JSON
build reports remain below ignored `build/` paths. ARM7 is still represented by
the lossless Stage-0 module because the upstream repository has no ARM7 symbol,
section, or relocation maps yet.

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
