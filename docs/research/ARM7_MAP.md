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

Thirty-three proven autoload-0 units are now maintained ARMv4T/Thumb source:

| Address | Working name | Size | Evidence |
|---:|---|---:|---|
| `037F846C` | `ARM7_Main` | `0xB8` | Final startup jump; initializes services then enters the main loop |
| `037F8524` | `SVC_Halt_Thunk` | `0x0C` | ARM veneer loading the Thumb `SVC_Halt` pointer |
| `037FB458` | `OS_IrqHandler` | `0x170` | Written to the ARM7 IRQ vector by startup; dispatches and restores IRQ state |
| `037FB738` | `OS_ResetRequestIrqMask` | `0x34` | Replaces the hardware IRQ request mask while preserving IME |
| `037FB76C` | `OS_DisableIrqMask` | `0x3C` | Clears selected hardware interrupt-enable bits while preserving IME |
| `037FB7A8` | `OS_EnableIrqMask` | `0x38` | IME-preserving update of the hardware interrupt-enable mask |
| `037FB7E0` | `OS_SetIrqMask` | `0x34` | Replaces the hardware interrupt-enable mask while preserving IME |
| `037FB814` | `OSi_SetTimerCallback` | `0x4C` | Populates and enables one callback-backed timer IRQ record |
| `037FB860` | `OS_SetIrqFunction` | `0xA8` | Registers direct or callback-backed handlers for each selected IRQ bit |
| `037FB908` | `OS_InitIrqTable` | `0x20` | Clears the IRQ check flag and shared VBlank counter |
| `037FB928` | `OS_UnLockCartridge` | `0x0C` | Legacy compatibility veneer for the Game Pak unlock function |
| `037FB934` | lock-ID subsystem | `0xC8` | Allocates and releases ARM7 spin-lock IDs using the shared availability masks |
| `037FB9FC` | Game Pak lock subsystem | `0x1D8` | Try, unlock, and blocking lock wrappers plus shared lock-word primitives |
| `037FBBD4` | `OS_InitLock` | `0x78` | Initializes the shared init lock and ARM7 lock-ID allocation flags |
| `037FBCC0` | `OS_SetSwitchThreadCallback` | `0x34` | Atomically replaces the scheduler's context-switch callback |
| `037FC188` | `OS_InitThread` | `0x12C` | Builds `OSThreadInfo`, the bootstrap thread, stack guards, and shared pointer |
| `037FC7E8` | `OS_Init` | `0x38` | Top-level NitroSDK operating-system initialization sequence |
| `037FC820` | arena subsystem | `0x1BC` | Arena accessors, initial-bound helpers, and guarded initialization for all ARM7 arenas |
| `037FCF90` | tick subsystem | `0x1F8` | Timer reservation, race-safe 64-bit tick reads, overflow handling, and initialization |
| `037FD188` | alarm scheduling core | `0x3C0` | IRQ dispatch, cancellation, one-shot/periodic scheduling, and sorted queue insertion |
| `037FD548` | alarm initialization | `0x70` | Alarm construction, availability query, timer-1 reservation, and queue initialization |
| `037FD5B8` | `OSi_SetTimer` | `0x9C` | Converts the next alarm deadline into timer-1 reload and callback state |
| `037FDC90` | interrupt-control leaves | `0x78` | Six contiguous CPSR IRQ/FIQ enable, disable, restore, and mode helpers |
| `037FDD08` | reset subsystem | `0x150` | Spin wait, termination, FIFO reset callback, reset detection and execution |
| `037FDE58` | `MI_StopDma` | `0x84` | Atomically stops one DMA channel using NitroSDK's register sequence |
| `037FE1BC` | `MI_SwapWord` | `0x08` | Atomically exchanges a lock word and returns its previous owner |
| `037FE1C4` | `PXI_Init` | `0x0C` | Public entry forwarding to the guarded PXI FIFO initializer |
| `037FE1D0` | `PXIi_HandlerRecvFifoNotEmpty` | `0x120` | Drains the receive FIFO, dispatches tagged callbacks, and rejects unhandled messages |
| `037FE2F0` | PXI FIFO APIs | `0x138` | FIFO send, callback readiness, and receive-callback registration |
| `037FE428` | `PXIi_InitFifo` | `0xDC` | Initializes FIFO state, receive IRQ handling, callbacks, and processor sync |
| `03803D94` | `SVC_WaitByLoop` | `0x04` | Thumb wrapper for supervisor call 3 |
| `03803DAE` | `SVC_Halt` | `0x04` | Thumb wrapper for supervisor call 6 |
| `03806D04` | `CTRDG_Init` | `0x5C` | Initializes ARM7 Game Pak state and transitions its PXI callback |

Together these replace `0x1758` bytes of the first mixed autoload image with
symbolic instructions. `ARM7_Main` exposes 20 calls, including one into
autoload 1, plus its three literal references. The main-loop thunk records the
Thumb target as a relocation with a `+1` interworking addend. `OS_IrqHandler`
exposes the NitroSDK IRQ table, IRQ check flag, and thread-info state. LLVM
rejects two ARM7TDMI user-bank load/store-multiple spellings even though their
encodings are valid, so those two context-switch instructions use documented
`.inst` encodings in an otherwise mnemonic source unit.

## Current confidence boundary

There are 267 directly verified ARM call/branch/literal/data relocations: 19 in
resident startup and 248 covering the 33 maintained autoload-0 units. The
build decodes each instruction source and checks its calculated branch target,
referenced literal value, or stored pointer. All `0x18C8` currently promoted
ARM7 bytes match exactly. The two autoload images
still contain large mixtures of executable code, literal pools, strings,
tables, and writable data, while the upstream repository supplies no
additional ARM7 symbols, section maps, or relocation maps. Remaining ranges
are therefore marked `kind:mixed`, not mislabeled as wholly executable code.

The next ARM7 pass should crawl outward from the 20 proven `ARM7_Main` callees,
establish ARM/Thumb entry points from direct control flow and pointer low bits,
then split only proven ranges. Audio, wireless, IPC, and cartridge-service
names should be assigned only after signature or caller evidence.
