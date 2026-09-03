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

One hundred and eight proven autoload-0 units are now maintained ARMv4T/Thumb source:

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
| `037FF104` | sound capture | `0x70` | Queries and configures the two DS hardware sound-capture units |
| `037FF174` | extended-channel helpers | `0x2C4` | Evaluates LFO/sweep state, compares voice priority, initializes voices, and derives ADSR decay rates |
| `037FF438` | sound LFO | `0xE8` | Initializes, starts, advances, and samples per-channel low-frequency oscillators |
| `037FF520` | `SND_InvalidateWave` | `0x88` | Stops active PCM channels whose source lies in an invalidated wave-data range |
| `037FF5A8` | sound-channel locks | `0x21C` | Queries, reserves, releases, and stops channels using strong and weak lock masks |
| `037FF7C4` | extended-channel control | `0x35C` | Voice allocation/priority, ADSR parameters and PCM/PSG/noise voice startup |
| `037FFB20` | `SND_ExChannelMain` | `0x30C` | Advances envelopes, sweep/LFO modulation, pitch, volume, and pan for all voices |
| `037FFE2C` | extended-channel hardware update | `0x24C` | Commits staged channel changes to sound registers and initializes the voice pool |
| `03800078` | sequence core helpers | `0x108` | Allocates tracks, resolves local/global variables, and dispatches all tracks belonging to one player |
| `03800B34` | sequence note-on command | `0x20C` | Reads instrument data, allocates or reuses a voice, applies envelopes/portamento, and links it to the track |
| `03800D40` | sequence channel lifecycle | `0x438` | Propagates player/track controls to voices and handles release, unlink, close, tempo, and player completion |
| `03801178` | sequence data helpers | `0x3FC` | Initializes players/tracks and decodes cached byte, 16/24-bit, variable-length, random, and variable arguments |
| `03801574` | sequence playback controls | `0x4E8` | Applies track/player parameters and handles invalidation, mute, skip, pause, stop, and start operations |
| `03801A5C` | `SND_PrepareSeq` | `0x154` | Initializes a player, allocates its main and declared tracks, primes the byte cache, and publishes active state |
| `03801BB0` | sequence runtime | `0x14C` | Advances active players, initializes player/track pools, publishes status, and resolves wave-archive entries |
| `03801CFC` | sound bank handling | `0x2F4` | Resolves instrument regions, wave archives, drum/key splits, and starts PCM, PSG, or noise voices |
| `03801FF0` | sound shared work | `0xDC` | Publishes channel/capture status and writes global or per-player sequence variables |
| `038020CC` | sound alarms | `0x194` | Configures one-shot/periodic OS alarms, tracks generations, and reports expirations over PXI |
| `03802260` | `SND_AlarmInit` | `0x2C` | Clears the active and generation bytes for all eight sound alarms |
| `0380228C` | sound command helpers | `0x378` | Snapshots driver state and controls masked channels, captures, alarms, and the PXI command queue |
| `03802604` | `SND_CommandProc` | `0x478` | Drains linked ARM9 command lists and dispatches all 34 sequence, channel, alarm, lock, and invalidation operations |
| `03802A7C` | `SND_CommandInit` | `0x40` | Initializes the eight-entry command queue, PXI callback, and shared completion pointer |
| `03802ABC` | file-system initialization | `0x8C` | Guards FS startup, allocates the CARD lock ID, initializes archive lists, and starts the CARD back end |
| `03802B48` | `CARD_SetThreadPriority` | `0x48` | Atomically changes the CARD worker's OS priority and returns the previous value |
| `03802B90` | CARD common initialization | `0xB8` | Enables the CARD service, constructs and wakes its worker thread, and registers PXI tag 11 |
| `03802C48` | `CARDi_TaskThread` | `0x124` | Sleeps until work arrives, dispatches one of 13 CARD operations, notifies ARM9, and wakes request waiters |
| `03802D6C` | `CARDi_OnFifoRecv` | `0xEC` | Receives multiword CARD requests over PXI tag 11 and wakes either the worker or current task waiter |
| `03802E58` | `CARDi_EraseBackupSectorCore` | `0x50` | Waits for idle backup memory, enables writes, sends SPI sector-erase command `0xD8`, and waits for completion |
| `03802EA8` | `CARDi_VerifyBackupCore` | `0xA0` | Reads a backup-memory range through the SPI transfer engine and records a mismatch in the shared command result |
| `03802F48` | backup transfer core | `0x208` | Implements page-chunked write/program operations and direct reads through shared SPI transfer callbacks |
| `03803150` | CARD SPI transfer engine | `0x278` | Packs one- to three-byte addresses, enables writes, transfers byte arrays, and supplies read/write/verify callbacks |
| `038033C8` | CARD command control | `0x1CC` | Starts and ends SPI commands, polls device busy state, and bounds both delay and completion with VCOUNT-aware timers |
| `03803594` | `CARD_Init` | `0x80` | Initializes ROM and backup state, starts common CARD services, installs the ROM accessor, and enables pull-out handling |
| `03803614` | CARD ROM requests | `0x144` | Serializes ROM-ID access, configures ROM reads and optional DMA, and executes the low-level read-ID cartridge command |
| `03803758` | CARD ROM transfer | `0x2AC` | Reads cartridge pages, emits eight-byte ROM commands, serves unaligned requests from cache, and completes waiting tasks |
| `03803A04` | CARD request dispatch | `0xDC` | Adapts the 13 ARM9 request IDs to ROM/backup core calls, no-op handlers, or the unsupported-operation result |
| `03803AE0` | CARD pull-out handling | `0x2B0` | Sends pull-out notifications, polls cartridge presence, compares ROM IDs, and registers the PXI termination callback |
| `03803D90` | `SVC_SoftReset` | `0x04` | Thumb wrapper for supervisor call 0 |
| `03803D94` | `SVC_WaitByLoop` | `0x04` | Thumb wrapper for supervisor call 3 |
| `03803D98` | interrupt-wait SVC wrappers | `0x16` | Waits for an interrupt or specifically for VBlank through supervisor calls 4 and 5 |
| `03803DAE` | `SVC_Halt` | `0x04` | Thumb wrapper for supervisor call 6 |
| `03803DB2` | remaining SVC wrappers | `0x5A` | Exposes stop, sound-bias, division, memory, decompression, and lookup-table supervisor calls |
| `03803E0C` | `MATH_CountPopulation` | `0x44` | Counts set bits with parallel masks and folding operations |
| `03803E50` | SPI command dispatch | `0xDC` | Routes PXI commands and worker-thread requests to touch, power, microphone, or NVRAM services |
| `03803F2C` | SPI request queue | `0x174` | Enqueues subsystem work, maintains the 16-entry request ring, and serializes hardware ownership |
| `038040A0` | SPI result and lock control | `0x1A4` | Maps service results to PXI tags and implements blocking ownership lock/unlock |
| `03804244` | `SPI_Init` and touch callbacks | `0x1E0` | Initializes four SPI-backed services, registers PXI handlers, starts the worker, and provides touch-sampling callbacks |
| `03804424` | `TP_ExecuteProcess` | `0x234` | Executes one-shot and periodic touch sampling, publishes results, and owns the SPI exception lock |
| `03804658` | `TPi_UpdateTouchData` | `0xF4` | Debounces pen-state transitions and adapts the sampling threshold |
| `0380474C` | `TP_AnalyzeCommand` | `0x1C0` | Reassembles PXI fragments, validates touch commands, and queues sampling state transitions |
| `0380490C` | `TP_Init` | `0x110` | Initializes touch state, four VAlarms, and the touch-controller SPI mode |
| `03804A1C` | `TP_ExecSampling` | `0x250` | Reads both axes, encodes validity and pen state, and publishes the consolidated sample |
| `03804C6C` | `TPi_ReadChannel` | `0x1F8` | Reads five SPI samples, measures spread, and selects a stable filtered coordinate |
| `03804E64` | `TPi_CheckTouch` | `0x15C` | Probes the pen signal through SPI/RCNT and distinguishes released, pressed, and ambiguous transitions |
| `03804FC0` | `PM_ExecuteProcess` | `0x188` | Executes sleep, utility, PM-register, and LED commands while owning the SPI exception lock |
| `03805148` | `PM_AnalyzeCommand` | `0x1D4` | Reassembles PXI fragments and validates or queues power, register, and LED-pattern commands |
| `0380531C` | PM control and PXI helpers | `0xE8` | Initializes PM state, encodes and sends PXI words, and updates control-register bits |
| `03806D04` | `CTRDG_Init` | `0x5C` | Initializes ARM7 Game Pak state and transitions its PXI callback |

Together these replace `0x9714` bytes of the first mixed autoload image with
symbolic instructions. `ARM7_Main` exposes 20 calls, including one into
autoload 1, plus its three literal references. The main-loop thunk records the
Thumb target as a relocation with a `+1` interworking addend. `OS_IrqHandler`
exposes the NitroSDK IRQ table, IRQ check flag, and thread-info state. LLVM
rejects two ARM7TDMI user-bank load/store-multiple spellings even though their
encodings are valid, so those two context-switch instructions use documented
`.inst` encodings in an otherwise mnemonic source unit.

## Current confidence boundary

There are 1,328 directly verified ARM call/branch/literal/data relocations: 19 in
resident startup and 1,309 covering the 108 maintained autoload-0 units. The
build decodes each instruction source and checks its calculated branch target,
referenced literal value, or stored pointer. All `0x9884` currently promoted
ARM7 bytes match exactly. The two autoload images
still contain large mixtures of executable code, literal pools, strings,
tables, and writable data, while the upstream repository supplies no
additional ARM7 symbols, section maps, or relocation maps. Remaining ranges
are therefore marked `kind:mixed`, not mislabeled as wholly executable code.

The next ARM7 pass should crawl outward from the 20 proven `ARM7_Main` callees,
establish ARM/Thumb entry points from direct control flow and pointer low bits,
then split only proven ranges. Audio, wireless, IPC, and cartridge-service
names should be assigned only after signature or caller evidence.
