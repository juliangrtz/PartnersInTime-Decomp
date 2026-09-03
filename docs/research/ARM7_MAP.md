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

`ARM7_AutoloadDone` is the first maintained ARM7 source function. It assembles
for ARMv4T and byte-matches the original instruction.

Two proven autoload-0 entry points are currently named:

| Address | Working name | Size | Evidence |
|---:|---|---:|---|
| `037F846C` | `ARM7_Main` | `0xB8` | Final startup jump; initializes services then enters the main loop |
| `037FB458` | `OS_IrqHandler` | `0x170` | Written to the ARM7 IRQ vector by startup; dispatches and restores IRQ state |

## Current confidence boundary

There are 41 directly verified ARM call/literal relocations: 13 in resident
startup and 28 covering `ARM7_Main` and `OS_IrqHandler`. The build decodes each
source opcode and checks its calculated branch target or referenced literal
value. The two autoload images clearly contain a mixture of executable code,
literal pools, strings, tables, and writable data, but the upstream repository
supplies no ARM7 symbols, section maps, or relocation maps. Remaining ranges
are therefore marked `kind:mixed`, not mislabeled as wholly executable code.

The next ARM7 pass must establish ARM/Thumb entry points from direct control
flow and pointer low bits, then split only proven ranges. Audio, wireless,
interrupt, IPC, and cartridge-service names should be assigned only after
signature or caller evidence.
