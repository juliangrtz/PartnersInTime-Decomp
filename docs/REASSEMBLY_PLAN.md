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
179 independent units. Those units cover raw
`.text` fragments around maintained functions, `.init`, `.rodata`,
constructors, alignment padding, `.data`, and explicitly mixed ARM7 fallback
images. There are 29,765 currently known relocations. `BattleActor_GetPartySlot`
at `0x02076F44` and
`BattleActor_GetById` at `0x02076F64` are real ARM assembly. Their
`gBattleContext` literal is emitted as `R_ARM_ABS32` and resolved by LLD from a
DSD-validated external definition. All resident ARM7 bytes are maintained
ARMv4T assembly or symbolic module parameters. Twenty-seven maintained units
from autoload 0 now cover `ARM7_Main`, its `SVC_Halt` thunk, `OS_Init`, IRQ mask
and
handler registration, `OS_IrqHandler`, thread bootstrap and switch-callback
state, CPSR interrupt control, reset coordination, IRQ-table, arena and lock
setup, PXI FIFO initialization and public send/callback APIs, the timer-0 tick subsystem, alarm
initialization, the Game Pak initializer, and two Thumb SVC wrappers. They
include 185 verified autoload relocations; ARM7 now has 204 verified
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

Priority formats:

1. `BAI_scn_*` battle scenarios and `BAI_mon_*` enemy AI;
2. `FEvent/FEvData.dat` field events, flags, warps, and story progression;
3. `BData/BDataMon.dat` enemy stats and rewards;
4. `Treasure/TreasureInfo.dat`;
5. shop tables and item master records;
6. field maps, object placement, sprites, palettes, animation banks, text, and
   audio containers.

Each format needs a lossless `extract -> source representation -> rebuild`
roundtrip, schemas with preserved unknown fields, and synthetic fixtures. Tools
may consume the user’s ROM but generated game content stays ignored.

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
2. Crawl outward from the maintained `ARM7_Main` call graph to recover further
   ARM/Thumb function boundaries, data sections, and relocations in both ARM7
   autoload images; the upstream project supplies no additional analysis.
3. Continue promoting small overlay-2 battle leaf functions using the two
   exact symbolic units as the template.
4. Split section fallbacks at DSD translation-unit boundaries, then retire
   fixed-layout patch registration.
5. Promote the confirmed battle actor/stat structures into maintained headers.
6. Replace a tiny matching function deliberately, build a nonmatching mod ROM,
   and test it in an emulator.
7. Expand outward by translation unit while never accepting unexplained diffs.

This order gives a playable artifact at every stage without treating original
binary slices as the desired final architecture.
