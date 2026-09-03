# ARM7 reverse-engineering map (European ARMP build)

This map applies to the verified European ROM with SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

## Serialized layout

The ARM7 header record describes one `0x26F24`-byte container at ROM
`0x0019F800`, loaded initially at `0x02380000`. It is not one contiguous runtime
module. Its NitroSDK static-module parameters at `0x02380158` describe a
resident startup and two autoload images:

| Component | ROM location | Runtime address | Stored size | BSS size |
|---|---:|---:|---:|---:|
| Resident ARM7 | `0x0019F800` | `0x02380000` | `0x170` | `0` |
| Autoload 0 | `0x0019F970` | `0x037F8000` | `0xFA84` | `0x3748` |
| Autoload 1 | `0x001AF3F4` | `0x027E0000` | `0x17318` | `0x18B8` |
| Autoload descriptors | `0x001C670C` | `0x023A6F0C` | `0x18` | n/a |

The relinker discovers the module-parameter block rather than assuming its
address. A candidate is accepted only when its descriptor range ends exactly
at the ARM7 container boundary and the sum of all described code images ends
exactly at the descriptor table.

## Resident startup symbols

| Address | Working name | Size | Purpose |
|---:|---|---:|---|
| `02380000` | `ARM7_Entry` | `0xC0` | CPU mode/stacks, memory clear, autoload dispatch, final jump |
| `023800C0` | `ARM7_LoadAutoloads` | `0x58` | Copies/clears each descriptor-defined autoload |
| `02380118` | `ARM7_AutoloadDone` | `0x04` | Header-provided completion hook; currently just `bx lr` |
| `0238011C` | `ARM7_MemoryAliasTest` | `0x3C` | Probes the ARM7-visible memory alias |
| `02380158` | `ARM7_ModuleParams` | `0x18` | NitroSDK static-module parameters |

All four functions above and `ARM7_ModuleParams` are maintained source units.
They assemble for ARMv4T, use symbolic cross-component references, and
byte-match all `0x170` resident bytes.

Six proven autoload-0 units are now maintained ARMv4T source:

| Address | Working name | Size | Evidence |
|---:|---|---:|---|
| `037F846C` | `ARM7_Main` | `0xB8` | Final startup jump; initializes services then enters the main loop |
| `037F8524` | `ARM7_MainLoopThumbThunk` | `0x0C` | ARM veneer loading the Thumb pointer `0x03803DAF` |
| `037FB458` | `OS_IrqHandler` | `0x170` | Written to the ARM7 IRQ vector by startup; dispatches and restores IRQ state |
| `037FB7A8` | `OS_EnableIrqMask` | `0x38` | IME-preserving update of the hardware interrupt-enable mask |
| `037FB860` | `OS_SetIrqFunction` | `0xA8` | Registers direct or callback-backed handlers for each selected IRQ bit |
| `037FC7E8` | `OS_Init` | `0x38` | Top-level NitroSDK operating-system initialization sequence |

Together these replace `0x34C` bytes of the first mixed autoload image with
symbolic instructions. `ARM7_Main` exposes 20 calls, including one into
autoload 1, plus its three literal references. The main-loop thunk records the
Thumb target as a relocation with a `+1` interworking addend. `OS_IrqHandler`
exposes the NitroSDK IRQ table, IRQ check flag, and thread-info state. LLVM
rejects two ARM7TDMI user-bank load/store-multiple spellings even though their
encodings are valid, so those two context-switch instructions use documented
`.inst` encodings in an otherwise mnemonic source unit.

## Current confidence boundary

There are 62 directly verified ARM call/branch/literal/data relocations: 19 in
resident startup and 43 covering the six maintained autoload-0 units. The
build decodes each instruction source and checks its calculated branch target,
referenced literal value, or stored pointer. All `0x4BC` currently promoted
ARM7 bytes match exactly. The two autoload images
still contain large mixtures of executable code, literal pools, strings,
tables, and writable data, while the upstream repository supplies no
additional ARM7 symbols, section maps, or relocation maps. Remaining ranges
are therefore marked `kind:mixed`, not mislabeled as wholly executable code.

The next ARM7 pass should crawl outward from the 20 proven `ARM7_Main` callees,
establish ARM/Thumb entry points from direct control flow and pointer low bits,
then split only proven ranges. Audio, wireless, IPC, and cartridge-service
names should be assigned only after signature or caller evidence.
