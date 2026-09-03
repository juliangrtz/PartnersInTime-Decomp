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

Fifty-nine proven autoload-0 units are now maintained ARMv4T/Thumb source:

| Address | Working name | Size | Evidence |
|---:|---|---:|---|
| `037F846C` | `ARM7_Main` | `0xB8` | Final startup jump; initializes services then enters the main loop |
| `037F8524` | `SVC_Halt_Thunk` | `0x0C` | ARM veneer loading the Thumb `SVC_Halt` pointer |
| `037FB458` | `OS_IrqHandler` | `0x170` | Written to the ARM7 IRQ vector by startup; dispatches and restores IRQ state |
| `037FB5C8` | IRQ callback dispatch | `0x170` | VBlank, timer, and DMA veneers plus the shared callback and dummy handlers |
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
| `037FBC50` | scheduler control | `0x70` | Atomically increments/decrements the nested scheduler-suppression counter |
| `037FBCC0` | `OS_SetSwitchThreadCallback` | `0x34` | Atomically replaces the scheduler's context-switch callback |
| `037FBCF4` | sleep subsystem | `0x7C` | Alarm callback veneer and millisecond sleep using thread suspension |
| `037FBD70` | thread scheduling | `0x1B0` | Priority updates, runnable-thread selection, deferred scheduling, and context switches |
| `037FBF20` | thread sleep/wakeup | `0x10C` | Direct wakeup and 16-bit wait-queue sleep/wakeup primitives |
| `037FC02C` | `OS_ExitThread` | `0x5C` | Releases list, mutex, ID-table, and join-wait resources before termination |
| `037FC088` | `OS_CreateThread` | `0x100` | Allocates an ID, registers the thread, initializes context/stack, and installs the exit return target |
| `037FC188` | `OS_InitThread` | `0x12C` | Builds `OSThreadInfo`, the bootstrap thread, stack guards, and shared pointer |
| `037FC2B4` | thread core | `0x1A8` | Thread-list insertion/removal, ID allocation, and ARM context save/load |
| `037FC45C` | message queues | `0x1CC` | Blocking/nonblocking peek, receive, send, and fixed-capacity queue initialization |
| `037FC628` | mutex subsystem | `0x1C0` | Recursive mutex ownership, per-thread owned lists, and wait-queue sleep/wakeup |
| `037FC7E8` | `OS_Init` | `0x38` | Top-level NitroSDK operating-system initialization sequence |
| `037FC820` | arena subsystem | `0x1BC` | Arena accessors, initial-bound helpers, and guarded initialization for all ARM7 arenas |
| `037FC9DC` | heap allocator | `0x5B4` | Heap checks, construction, arena setup, allocation/free, and ordered free-list maintenance |
| `037FCF90` | tick subsystem | `0x1F8` | Timer reservation, race-safe 64-bit tick reads, overflow handling, and initialization |
| `037FD188` | alarm scheduling core | `0x3C0` | IRQ dispatch, cancellation, one-shot/periodic scheduling, and sorted queue insertion |
| `037FD548` | alarm initialization | `0x70` | Alarm construction, availability query, timer-1 reservation, and queue initialization |
| `037FD5B8` | `OSi_SetTimer` | `0x9C` | Converts the next alarm deadline into timer-1 reload and callback state |
| `037FD654` | V-count alarms | `0x63C` | Frame tracking, display-status IRQ dispatch, cancellation, one-shot/periodic setup, and a sorted queue |
| `037FDC90` | interrupt-control leaves | `0x78` | Six contiguous CPSR IRQ/FIQ enable, disable, restore, and mode helpers |
| `037FDD08` | reset subsystem | `0x150` | Spin wait, termination, FIFO reset callback, reset detection and execution |
| `037FDE58` | `MI_StopDma` | `0x84` | Atomically stops one DMA channel using NitroSDK's register sequence |
| `037FDEDC` | `MI_WaitDma` | `0x70` | Waits for one DMA channel and restores channel zero's safe register defaults |
| `037FDF4C` | CPU memory primitives | `0x270` | 16/32-bit clear/copy, fast clear, and alignment-safe 8-bit fill/copy routines |
| `037FE1BC` | `MI_SwapWord` | `0x08` | Atomically exchanges a lock word and returns its previous owner |
| `037FE1C4` | `PXI_Init` | `0x0C` | Public entry forwarding to the guarded PXI FIFO initializer |
| `037FE1D0` | `PXIi_HandlerRecvFifoNotEmpty` | `0x120` | Drains the receive FIFO, dispatches tagged callbacks, and rejects unhandled messages |
| `037FE2F0` | PXI FIFO APIs | `0x138` | FIFO send, callback readiness, and receive-callback registration |
| `037FE428` | `PXIi_InitFifo` | `0xDC` | Initializes FIFO state, receive IRQ handling, callbacks, and processor sync |
| `037FE504` | X/Y button sampling | `0x11C` | RCNT setup, shared key-state sampling, and periodic alarm initialization |
| `037FE620` | sound control | `0x170` | Output routing, master volume, sleep/bias transitions, enable/disable, and shutdown |
| `037FE790` | sound-channel control | `0x4DC` | Surround decay, pan/volume/timer, stop, and PCM/PSG/noise channel setup |
| `037FEC6C` | sound utilities | `0x258` | LCG random, indexed sine, logarithmic volume, and pitch-to-timer calculations |
| `037FEEC4` | `SndThread` | `0x80` | Initializes the ARM7 sound driver and runs its message-driven update loop |
| `037FEF44` | sound thread control | `0x1C0` | Alarm callback, queue wake/wait, interval timer, thread construction, and guarded initialization |
| `037FF438` | sound LFO | `0xE8` | Initializes, starts, advances, and samples per-channel low-frequency oscillators |
| `037FF520` | `SND_InvalidateWave` | `0x88` | Stops active PCM channels whose source lies in an invalidated wave-data range |
| `037FF5A8` | sound-channel locks | `0x21C` | Queries, reserves, releases, and stops channels using strong and weak lock masks |
| `037FF7C4` | extended-channel control | `0x35C` | Voice allocation/priority, ADSR parameters and PCM/PSG/noise voice startup |
| `037FFB20` | `SND_ExChannelMain` | `0x30C` | Advances envelopes, sweep/LFO modulation, pitch, volume, and pan for all voices |
| `037FFE2C` | extended-channel hardware update | `0x24C` | Commits staged channel changes to sound registers and initializes the voice pool |
| `03803D94` | `SVC_WaitByLoop` | `0x04` | Thumb wrapper for supervisor call 3 |
| `03803DAE` | `SVC_Halt` | `0x04` | Thumb wrapper for supervisor call 6 |
| `03806D04` | `CTRDG_Init` | `0x5C` | Initializes ARM7 Game Pak state and transitions its PXI callback |

Together these replace `0x4A10` bytes of the first mixed autoload image with
symbolic instructions. `ARM7_Main` exposes 20 calls, including one into
autoload 1, plus its three literal references. The main-loop thunk records the
Thumb target as a relocation with a `+1` interworking addend. `OS_IrqHandler`
exposes the NitroSDK IRQ table, IRQ check flag, and thread-info state. LLVM
rejects two ARM7TDMI user-bank load/store-multiple spellings even though their
encodings are valid, so those two context-switch instructions use documented
`.inst` encodings in an otherwise mnemonic source unit.

## Current confidence boundary

There are 637 directly verified ARM call/branch/literal/data relocations: 19 in
resident startup and 618 covering the 59 maintained autoload-0 units. The
build decodes each instruction source and checks its calculated branch target,
referenced literal value, or stored pointer. All `0x4B80` currently promoted
ARM7 bytes match exactly. The two autoload images
still contain large mixtures of executable code, literal pools, strings,
tables, and writable data, while the upstream repository supplies no
additional ARM7 symbols, section maps, or relocation maps. Remaining ranges
are therefore marked `kind:mixed`, not mislabeled as wholly executable code.

The next ARM7 pass should crawl outward from the 20 proven `ARM7_Main` callees,
establish ARM/Thumb entry points from direct control flow and pointer low bits,
then split only proven ranges. Audio, wireless, IPC, and cartridge-service
names should be assigned only after signature or caller evidence.
