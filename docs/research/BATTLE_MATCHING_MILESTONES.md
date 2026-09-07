# Battle matching milestones toward 20 percent

## Global properties, damage, and motion channels

These four European overlay-2 units reproduce all 3,320 original bytes,
including relocations and function order:

| Unit | Bytes | Implementation |
|---|---:|---|
| `battle_global_property_set.c` | 1,304 | C |
| `battle_damage_by_object.c` | 728 | C |
| `battle_damage_numbers.c` | 860 | C with one ARM instruction |
| `battle_scene_motion_stop.c` | 428 | C |

`SceneBackground_StartFade` takes both a direction and a duration. Its resident
implementation at `0x02027DB8` reads `r1`, substitutes 20 or 60 when the duration
is -1, and calculates the fixed-point fade step. The global-property setter
passes its property value through as this second argument. The previous
one-argument declaration incorrectly allowed the compiler to overwrite it.

`BattleSceneObject_StopMotionChannel` returns `void`. The C callers discard its
result, and the original assembly's exit values are temporary values from
channel scanning and stores. Removing artificial pointer/integer return values
restores the original data flow. The scan retains separate index and channel
variables, applies deferred movement in order, and clears the stopped channel.

The damage calculator converts each object ID to an actor ID in the same input
variable before looking up the actors. Those value lifetimes reproduce the
original register reuse across the two object lookups.

The damage-number cleanup callback explicitly materializes the payload address
at task + 12. One `add` in inline assembly preserves that address boundary;
the actor-ID read, actor lookup, effect check, and cleanup remain C. The other
three functions in this unit are entirely C.

Validation uses objdiff for each function, the complete module and symbol
checks, and the normal unmodified-data ROM build. The reference ROM SHA-1 is
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

## Fixed-point coprocessor routines

The resident `fx_coprocessor.c` unit adds 680 matching bytes across eleven
functions. In `FX_InvSqrt`, the Q32 reciprocal multiplied by the Q22 square-root
result produces Q54. Returning Q12 requires adding `1 << 41` and shifting by
42. The previous expression rounded and shifted the low word by ten bits,
losing the required 32-bit scale adjustment. The corrected 64-bit C expression
reproduces the complete original multiply, carry, and arithmetic-shift sequence.

`FX_Sqrt` preserves its nonpositive-input guard and calls the C result reader.
Its seven-instruction hardware setup block (28 bytes) retains the order of
the mode write and the two operand-word writes. All other functions in this
unit are C, including the corrected inverse square root.

## VRAM bank management

Four new GX units reconstruct the complete resident range
`0x02035444..0x020364AC`: 40 functions and 4,200 bytes, entirely in C.

The 26-byte `GxVramState` records assignments for LCDC, the two engines,
textures, clear images, and extended palettes. The code initializes the bank
state and control registers, calculates capacity from bank masks, disables
and unlocks banks, moves assignments back into LCDC address space, and sets
each engine's bank mappings. The assignments preserve both mapping-slot bits
and the enable bits in the display control registers.

The background and texture switches retain the original fallthrough groups
and register-write order. The background switch's 33-entry ARM branch table
was reconstructed directly from its branch destinations; the local decompiler
did not recover that table. Unsupported masks preserve the original switch
default behavior. The shared assignment expression reads the old engine mask
and combines it with LCDC before replacing the engine mask; MWCC schedules
the independent stores in the same order as the original.

All 40 functions match at their original sizes. The whole-module and symbol
checks, reference-ROM SHA-1 check, generated-progress check, public-content
audit, and 66 tests pass for the linked milestone.

## Background addresses, blending, and geometry command lists

Three further GX units reconstruct `0x020364AC..0x02036CDC`: 35 functions
and 2,096 bytes, entirely in C. The background address getters distinguish
text, affine, and bitmap modes and account for the main engine's global
character/map offsets. The affine setup retains the signed Q12-to-Q8
coefficient conversion before packing two 16-bit hardware coefficients.

The geometry command list preserves four opcode bytes per command word,
parameter cursor advancement, and the padding required by parameterless
commands. Its matrix and zero-to-three-parameter emitters reproduce the
original stores and call sequence. No assembly fallback is needed.

All 35 functions match at their original sizes. The linked module/symbol
checks and the complete reference-ROM hash pass.

## Geometry state and graphics transfers

Seven further GX units add 52 matching functions and 5,468 bytes, entirely
in C. They cover matrix-stack reset, geometry initialization, fog and clear
values, extended palette mapping/restoration, CPU/DMA transfer selection,
texture mapping setup and cleanup, and command-list finalization.

All hardware stores and reloads are checked against the original ARM code.
The decompiler initially folded repeated MMIO accesses; the C implementation
uses volatile accesses and preserves the complete register-write sequence.
Inline C accessors for the fixed object-memory bases preserve the original
address materialization without assembly.

Texture transfers that cross a bank boundary remain outside the linked C
set: their current C reconstruction has a register-allocation difference.
The five matching texture setup, cleanup, and palette functions are linked
independently. The shared command-list layout now identifies its buffer
start, as established by the finalizer's byte-count calculation.

The module/symbol checks, original-ROM SHA-1, progress consistency, public
content audit, and all 66 tests pass for this milestone.

## Interrupts, threads, message queues, and mutexes

Five OS units add 43 functions and 3,152 matching bytes, entirely in C.
They reconstruct interrupt masks and callbacks, thread selection and context
switch coordination, priority-list insertion/removal, thread creation and
exit, blocking/nonblocking message queues, and recursive mutex ownership.
The low-level context save/load and initial ARM register setup retain their
original assembly implementations.

The recovered layouts have compile-time size checks: context `0x64`, thread
`0x94`, mutex `0x14`, and thread-info `0x54`. Thread queues use 16-bit masks;
the wake operation widens its mask to 32 bits for the scan. Mutex ownership
uses a doubly linked list in each thread. Interrupt-mask updates retain the
master-enable read before restoring its previous value.

The IRQ callback fields are now expressed as one structure. Objdiff reports
relocation-name differences for the old separately named field aliases;
after linking, those addresses and all instructions are byte-identical.
The complete module and symbol checks, original-ROM SHA-1, generated progress,
public-content audit, and 66 tests verify this milestone.

## Timer ticks and alarms

The tick and alarm units add 12 functions and 1,644 matching bytes in C.
Timer 0 supplies a 16-bit counter with a 48-bit software overflow count.
The read preserves volatile accesses, interrupt exclusion, and the pending
overflow correction. Alarms maintain a deadline-ordered doubly linked list,
reschedule periodic events using unsigned 64-bit division, and program timer 1
with the original late-event and long-delay reload values.

The unsigned 64-bit division entry at `0x0204626C` is identified as `_ll_udiv`,
allowing the C division expression to link to the original runtime helper.
All functions and the linked modules match. The reference-ROM hash, generated
progress check, public-content audit, and 66 tests pass.

## DMA control and transfers

Two MI units add 11 functions and 1,340 matching bytes in C. They preserve
channel-zero source restrictions, incompatible automatic-transfer checks,
stop/wait register accesses, synchronous copy/fill, display-timed transfers,
and asynchronous copy callbacks. Channel zero's idle reset retains its
three register writes, and stopping DMA retains both volatile control reads.

All linked instructions and module symbols match. The complete reference-ROM
SHA-1, generated progress, public-content audit, and 66 tests pass.

## Sound commands and ARM7 command queues

The sound command builders, initialization, command-pool and queue management,
alarm callbacks, and shared-state setup add 49 functions and 3,864 C bytes.
The mutex initializer adds another 20-byte C function. Commands have a 24-byte
layout, use a 256-entry pool, and track up to eight submitted chains through
a nine-slot ring. Serial completion comparisons retain unsigned wraparound.
The shared ARM7 status structure is 640 bytes; its per-track and global
variables are initialized to -1 and flushed before submission.

Callback generation checks and cache maintenance preserve the native access
order. The 64-byte serial getter at `0x0203C570` remains native because its
conditional load order still differs in C. Every included function links to
identical original bytes, including the pool-tail relocation aliases.
The complete ROM SHA-1, module/symbol checks, progress check, public-content
audit, and all 66 tests pass.

## Instrument banks and processor FIFO

The instrument-bank and processor-FIFO units, plus the shared-status getter,
add 16 functions and 2,628 matching bytes in C. The bank reader handles simple
instruments, contiguous key ranges, and eight-region key splits. Wave archives
retain their pointer/offset distinction and linked references from banks;
reference changes preserve cache-store ranges and lock boundaries.

The FIFO packet is a 5-bit tag, 1-bit error flag, and 26-bit payload. Its C
bitfields reproduce the original packet updates. Interrupt-driven receive,
unhandled-tag replies, transmit errors, callback registration, and startup
handshake all match. The 40-byte remote callback-mask getter remains native:
its current C form materializes the fixed shared-memory base differently.
No assembly fallback is added. Module/symbol checks, the original ROM SHA-1,
progress consistency, public-content audit, and all 66 tests pass.

## Filesystem archives and command scheduling

Two FS units add 16 matching C functions and 2,284 bytes. They cover archive
registration, memory callbacks, suspension/resumption, command submission,
synchronous and asynchronous execution, cancellation-list processing, and
completion wakeups. The reconstructed layouts are 80 bytes for an archive,
68 bytes for a file, and 12 bytes for a directory position, checked at compile
time. Status flags and results retain their volatile accesses.

Small inline C status predicates preserve the original explicit Boolean
conversion before branch decisions. Command execution and shutdown paths use
the native branch order, including unlock/relock boundaries and callbacks.
Archive-table loading and name packing remain outside the linked C set until
their remaining instruction differences are resolved. Module/symbol checks,
the reference-ROM SHA-1, generated progress, public-content audit, and all
66 tests pass.

## Filesystem file operations

The file unit adds all 16 functions in `0x0203ED24..0x0203F33C`, totaling
1,560 matching C bytes. It implements seeking, synchronous/asynchronous reads,
cancellation and waiting, direct and path-based opens, close, file/list
initialization, and archive-prefix path selection. Read requests keep the
requested length separate from the clamped transfer length and actual result.
The native structure-return ABI and directory-position copies are preserved.
All module/symbol checks, the original-ROM SHA-1, progress consistency,
public-content audit, and 66 tests pass.

## Directory reads and filesystem command handlers

Eight additional functions add 1,264 matching C bytes. They implement directory
record reads, directory seeks, archive read/write callbacks, case-insensitive
name comparisons, synchronous metadata reads, and the archive/default command
dispatch sequence. The directory-entry union has a checked 148-byte layout.
Callbacks retain the original four-argument ABI, including offset and length;
result handling preserves completion flags and thread wake/wait behavior.

The adjacent archive-state functions are consolidated into the same command
module. Every included function matches byte for byte. The whole-ROM hash,
module/symbol checks, generated progress, public-content audit, and all 66
tests verify the milestone.

## ROM archives and overlay image loading

The ROM and overlay I/O units add 17 functions and 1,952 matching bytes,
bringing linked C/C++ to 208,568 bytes (13.34%). The reconstructed 44-byte
overlay descriptor includes the 24-bit compressed length, flags, processor,
and resolved file extent. ROM callbacks, DMA selection, direct and cached
overlay metadata, synchronous/asynchronous image reads, cache invalidation,
BSS clearing, and digest comparison retain the native behavior.

Three small inline-assembly blocks total 44 instruction bytes: the ROM
registration argument setup, the ordered overlay-table selection, and one
indexed digest-word read. Their surrounding control flow and data handling
are C. These blocks preserve instruction scheduling and literal-pool order;
they are not assembly bodies for the reconstructed functions. The full ROM
matches the reference SHA-1, every module and symbol check passes, and all
66 tests, progress checks, and the public-content audit pass.

## Overlay initialization and destructor cleanup

All five overlay lifecycle functions add 724 matching bytes in pure C. Loading
checks metadata and image reads before initialization. Startup preserves digest
verification, optional backward decompression, cache flushing, and constructor
iteration. Named start/end aliases retain the empty digest-table boundaries
without folding away the original verification branch.

Unload detaches matching destructors under the interrupt lock, invokes them
after restoring interrupts, and repeats to handle callbacks that register more
destructors. The image-size intermediate uses signed subtraction as in the
native bounds calculation; no assembly fallback is needed. Module/symbol
checks, the reference-ROM SHA-1, progress consistency, the public-content audit,
and all 66 tests pass.

## SHA-1 block processing and HMAC

All six digest routines add 2,024 matching bytes in pure C. The 104-byte SHA-1
context preserves the split bit count, block index, finalization state, and
error result. Padding, endian conversion, the 80-word schedule, four round
groups, optional zeroing/restoration of two input words, and HMAC key reduction
and inner/outer hashes match the native code. Volatile accesses preserve the
original counter reloads and byte-by-byte construction of the first 16 words.

The linked total is 211,316 bytes (13.51%). Every module/symbol check passes,
the rebuilt ROM retains the original SHA-1, and all 66 tests, generated-progress
checks, and public-content checks pass.

## Touchscreen sampling and coprocessor context

Twelve touchscreen functions and both coprocessor-context functions add 1,960
matching C bytes, bringing the total to 213,276 bytes (13.64%). Touchscreen
initialization, single/automatic sampling requests, result waits, user settings,
calibration setup, and fixed-point coordinate conversion match. The checked
work structure is 56 bytes and each sample is eight bytes. Calibration keeps
the native 64-bit arithmetic and signed coordinate clamps. Division/square-root
context save and restore preserve the original paired MMIO accesses.

The calibration-parameter calculator and FIFO callback remain native while
their remaining scheduling differences are investigated; they are excluded
from the C total. No assembly fallback is added. The complete module/symbol
check, reference-ROM SHA-1, progress check, public-content audit, and all 66
tests pass.

## Power management and sleep transitions

Twenty-eight power-management functions add 2,756 matching bytes in pure C,
raising the linked total to 216,032 bytes (13.82%). The implementation covers
sleep callback lists, LCD/LED/backlight control, synchronous/asynchronous
commands, ARM7 replies, request locking/completion, and sleep/wake transitions.
Sleep preserves display state, interrupt state and masks, frame-boundary waits,
wake-source filtering, and the power-off path. Its saved state remains in the
native stack slots, and explicit inline C conversions reproduce reply narrowing.

The 120-byte asynchronous register-read submission remains native pending its
literal-load scheduling match. Every included function is byte-identical;
module/symbol checks, the original-ROM SHA-1, generated progress, public-content
audit, and all 66 tests pass.

## RTC initialization and time requests

Eight RTC functions add 560 matching C bytes: initialization, synchronous and
asynchronous time requests, FIFO command submission, the sync-result callback,
and validated eight-digit BCD conversion. The FIFO response handler and its
busy-wait helper remain native pending exact register/scheduling matches.
The linked total is 216,592 bytes (13.85%). The complete module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress, and public-content audit
pass. No assembly fallback is added.

## Card initialization and backup write workers

Sixteen card functions add 1,376 matching bytes in pure C. They cover ROM and
backup lock wrappers, resource release, card availability/enabling, common
initialization, task dispatch/completion, backup waiting, and chunked write/
program operations followed by verification. The checked command structure is
64 bytes; the common work structure is 480 bytes, including its OS thread and
256-byte transfer buffer. Cache maintenance and ARM7 request retries retain
their native order. Remaining card candidates stay outside the linked total.

The total is 217,968 bytes (13.94%). Module and symbol checks, the original-ROM
SHA-1, all 66 tests, generated progress, and the public-content audit pass.

## Card scheduling, backup reads, and ROM requests

The card work grows by 2,820 matching bytes to 4,196 linked card bytes. Resource
locking/waiting, the worker loop and FIFO callback, backup identification and
submission, chunked backup reads, ROM initialization/submission/completion,
cache reads, ROM command registers, and DMA eligibility now match. Related
source fragments are consolidated. A pointer-through-integer accessor preserves
the shared work block as a register-based address, preventing the front end
from prematurely substituting individual global-field references. Transfer
fields retain their original volatile reloads.

DMA eligibility uses one four-byte inline-assembly load to preserve the transfer
length's register lifetime. All other newly linked instructions come from C.
The backup-specification initializer, polled ROM reader, and DMA interrupt
completion remain native until their remaining scheduling details match.

The total is 220,788 bytes (14.12%). All module/symbol checks, the original-ROM
SHA-1, all 66 tests, generated progress, and the public-content audit pass.

## Card ARM7 requests and removal handling

Five functions add 644 matching C bytes: shared-command submission, ARM7
initialization handshakes, retry/wait handling, removal notifications, callback
registration, and the power-off/termination path. The request flags remain
volatile so each required shared-state read survives compilation. The linked
total is 221,432 bytes (14.16%). Module/symbol checks, original-ROM SHA-1, all
66 tests, generated progress, and the public-content audit pass.

## GBA cartridge control and identification

Fourteen cartridge functions add 1,220 matching bytes in pure C: initialization,
FIFO messages, bus locking, bus-timing save/restore, module identification and
presence checks, and removal callbacks. The GBA header uses ordinary ROM reads
while the ARM7-shared module flags remain volatile, preserving the native access
order. The module-header import routine remains native pending its scheduling
match. No assembly fallback is added.

The total is 222,652 bytes (14.24%). Module/symbol checks, the original-ROM SHA-1,
all 66 tests, generated progress, and the public-content audit pass.

## C runtime termination, signals, and stream buffers

Nine runtime functions add 1,356 matching bytes in pure C: exit-handler and
global-destructor dispatch, abort and signal handling, flushing all streams,
individual stream flushing, and buffer preparation. The stream structure is
reconstructed with its original 76-byte layout and mode/state bitfields. The
newline conversion routine is an empty function in the original executable.
Explicit lock-owner and current-thread values preserve the native register
allocation in both critical-region callers.

The total is 224,008 bytes (14.33%). All nine functions match completely in
objdiff; module/symbol checks, original-ROM SHA-1, all 66 tests, generated
progress, and the public-content audit pass.

## Runtime console I/O and object lifetime

Ten more functions add 884 matching C bytes: console reads/writes, compact
signed/unsigned integer decoding, caught-exception cleanup, exception-table
binary search, global constructors/destructors and registration, and terminate
dispatch. All ten functions match completely in objdiff.

The internal 256-byte lookup table at 0x020469B4 retains its data label with a
zero ELF symbol size: its bytes are already covered by its enclosing function.
This prevents the MW linker's overlapping-symbol-size error when the surrounding
reference object becomes smaller. The table bytes and function extent remain
unchanged, and the table is not counted as reconstructed C.

The linked total is 224,892 bytes (14.38%). Module/symbol checks, original-ROM
SHA-1, all 66 tests, generated progress, and the public-content audit pass.

## Foundation lists and frame heaps

Twenty-three functions add 1,688 matching C bytes: intrusive list traversal and
mutation, nested heap registration/lookup, aligned frame-heap allocation from
both ends, reset, and tagged state save/restore. The reconstructed layouts are
12-byte lists, 36-byte heap headers, 12-byte frame data, and 16-byte state records.
The original option checks and optional allocation clearing are retained.

All 23 functions match completely in objdiff, without inline assembly. The total
is 226,580 bytes (14.49%). Module/symbol checks, original-ROM SHA-1, all 66 tests,
generated progress, and the public-content audit pass.

## Sound sequence players, resource locks, and volume fades

Twenty-seven functions add 2,356 matching C bytes. Resource management reserves
alarms, capture units, and sound channels. Sequence players now have exact
priority insertion, handle and heap ownership, allocation/preemption, deferred
start, fade-out, and per-frame volume updates. Their 68-byte layout and the
36-byte player-group layout are reconstructed, along with the 16-byte fader.
All 27 functions match completely in objdiff, without inline assembly.

The total is 228,936 bytes (14.64%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress, and the public-content audit pass.

## Sound API and main-loop integration

Twenty functions add 1,188 matching C bytes: sequence handles and archive IDs,
play-count queries, archive stop operations, channel priority/volume, player heap
creation, global variables, sound initialization, the main loop, and pre/post
sleep callbacks. Caller disassembly also establishes all four arguments of
sequence preparation; the C declaration now carries sequence data, offset, and
bank explicitly while retaining identical instructions.

All 20 new functions and the updated player unit match completely in objdiff.
The total is 230,124 bytes (14.72%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress, and the public-content audit pass.

## Sound heap allocation and state rollback

Eleven functions add 1,020 matching C bytes. Sound heaps now reconstruct aligned
allocation, section lists, save/load levels, reverse-order disposal callbacks,
command completion waits, and destruction. The 16-byte heap, 20-byte section,
and 32-byte allocation header have explicit layouts. The sequence-heap disposal
callback now declares the full four-argument callback contract confirmed by its
callers; its instructions remain unchanged.

All new and affected functions match completely in objdiff. The total is
231,144 bytes (14.78%). Module/symbol checks, original-ROM SHA-1, all 66 tests,
generated progress, and the public-content audit pass.

## Sound archive tables and file I/O

Twenty-one functions add 1,864 matching C bytes: SDAT archive initialization,
header/INFO/FAT/SYMB loading and disposal, current-archive switching, all eight
INFO table lookups, file offset/size/address access, and bounded file reads.
The 140-byte archive state combines the 48-byte SDAT header, the existing
68-byte filesystem handle, its file ID, and the three loaded block pointers.

All 21 functions match completely in objdiff, without inline assembly. The
total is 233,008 bytes (14.90%). Module/symbol checks, original-ROM SHA-1, all
66 tests, generated progress, and the public-content audit pass.

## Sound archive asset loading

Twenty-one functions add 2,856 matching C bytes: cached sequence/bank/wave
files, individual-wave header tables, archive-safe disposal, bank instrument
traversal, dependency loading, group dispatch, and public load wrappers. The
SDAT sequence, bank, wave, and group records now have explicit C layouts.
The single-wave allocator remains original code until its register allocation
is matched; no partial match is linked.

All 21 new functions match completely, without inline assembly. The total is
235,864 bytes (15.08%). Module/symbol checks, original-ROM SHA-1, all 66 tests,
generated progress, and the public-content audit pass.

## Sound archive playback and player setup

Six functions add 944 matching C bytes: sequence and sequence-archive startup,
player configuration and heap creation, and bounded sequence-archive entry
lookup. The sequence-ID setters now accept full integer arguments, matching
the original calling contract; storage still truncates to the original
16-bit fields. Both setters retain their exact instructions.

All new and affected functions match completely, without inline assembly.
The total is 236,808 bytes (15.14%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress, and the public-content audit pass.

## Sound stream channel and ring-buffer control

Fourteen functions add 1,652 matching C bytes: stream initialization, channel
reservation, aligned buffer setup, alarm callbacks, channel volume and pan,
start/stop, and sleep suspension/resumption. The 92-byte stream state, signed
status bitfields, channel records, and six-argument buffer callback are typed.
Unsigned buffer division now identifies the resident `_u32_div_f` helper.

All 14 functions match completely, without inline assembly. The total is
238,460 bytes (15.25%). Module/symbol checks, original-ROM SHA-1, all 66 tests,
generated progress, and the public-content audit pass.

## Sound capture callbacks, effects, and fades

Eleven functions add 1,716 matching C bytes: stereo block callbacks, an
eight-entry callback queue and worker thread, sleep transitions, resource
release, volume-fade updates, and effect/reverb wrappers. The 84-byte capture
state and 20-byte callback record have explicit layouts; the existing OS
message-queue layout now lives in a shared header. The central capture-start
routine and effect-stop wrapper remain original code pending complete matches.

All linked functions are byte-identical, without inline assembly. The total
is 240,176 bytes (15.36%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Game archive I/O and overlay request queues

Twenty functions add 2,448 matching C bytes. The game's 320-byte archive
manager now reconstructs queued file opens, 512-byte read/overlay progress,
request waiting, archive entry sizes, file-table setup, and synchronous and
asynchronous overlay wrappers. Its 40-byte read request and 16-byte open
request have explicit layouts. Archive opening now exposes the status result
confirmed by its original success and queue-full paths.

All 20 functions match completely, without inline assembly. The total is
242,624 bytes (15.52%). Module/symbol checks, original-ROM SHA-1, all 66 tests,
generated progress, and the public-content audit pass.

## Archive lifecycle and compressed read requests

Thirteen functions add 2,440 matching C bytes: archive construction/destruction,
scanline-based work scheduling, immediate reads, compressed request setup,
block progression, and the BattleArchive asynchronous read entry point.
The 64-byte compressed request extends the ordinary read request, while the
derived manager supplies its 512-byte input buffer. The archive-read API now
expresses its verified status result and 16-bit flags argument.

All linked functions match completely, without inline assembly. The small
decoder selector remains original code pending its complete match. The total
is 245,064 bytes (15.67%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Main and interrupt task managers

Nineteen functions add 2,608 matching C++ bytes. Both 24-byte task layouts
now describe priority-ordered insertion, removal during callbacks, activation,
and update accounting. Main tasks retain their signed four-bit interval and
four-bit counter; interrupt tasks preserve VBlank IRQ masking around list
operations. Virtual callbacks use the original C++ calling convention.

All functions match completely, without inline assembly. The total is
247,672 bytes (15.84%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Memory decompression and compressed archive lifecycle

Thirteen functions add 1,024 matching C bytes. The 60-byte memory decoder
queues existing compressed buffers, chooses byte or halfword output for the
destination memory region, advances blocks, waits for requests, and applies
the same scanline/frame budget as file-backed reads. The compressed archive's
constructors and destructors now use the reconstructed base lifecycle.

All functions match completely, without inline assembly. The total is
248,696 bytes (15.90%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Game heap allocation and release

Nine functions add 1,020 matching C bytes: aligned allocation, forward and
backward block splitting, free-block coalescing, dynamic heap creation/removal,
and the game's New/Delete entry points. Block headers and region entries are
both 16 bytes. The previously separate head/cursor/size symbols now identify
one region table, with explicit relocation addends for its columns.

All functions match completely, without inline assembly. The total is
249,716 bytes (15.97%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Fixed-point angles and vectors

Six functions add 1,724 matching C bytes. Quadrant reduction and signed table
lookups reconstruct Atan2 in radians and angle indices, plus one-argument
AtanIdx. Vector dot/cross products preserve 64-bit intermediates and rounding.
Normalization reconstructs the hardware divide/square-root sequence and the
full-precision reciprocal scale, including its final rounding.

All six functions match completely, without inline assembly. The total is
251,440 bytes (16.08%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Four-by-four matrix operations

Three functions add 1,332 matching C bytes: matrix concatenation, scaling,
and translation. Concatenation preserves the original row/column snapshots
and temporary matrix when the output aliases the right input. Scaling copies
the untouched translation row in pairs; translation narrows each shifted
64-bit dot product before the final 32-bit addition.

All three functions match completely, without inline assembly. The total is
252,772 bytes (16.16%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Camera and projection matrices

Three functions add 1,668 matching C bytes: LookAt, orthographic projection,
and perspective projection, each supporting a returned matrix and direct
geometry-FIFO loading. The reconstruction preserves asynchronous division,
the optional W scale, and fixed-point depth rounding. The former OrthoW label
now identifies its full G3 interface; the signed 64-bit divide runtime is
identified as `_ll_sdiv`.

All three functions match completely, without inline assembly. The total is
254,440 bytes (16.27%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Save backup and storage access

Thirty-two functions add 2,588 matching C bytes. The backup layer now covers
EEPROM/flash selection, cartridge locks, asynchronous read/write callbacks,
status and checksum handling. The storage layer reconstructs signature,
settings, directory, slot and footer access, whole-image reads and erasure.
The separate image-initialization routine remains outside the linked C set.

All linked functions match completely, without inline assembly. The total is
257,028 bytes (16.44%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Rumble, input, display and frame timing

Twenty-one functions add 2,400 matching C bytes. Rumble reconstruction covers
cartridge initialization, pulse validation, millisecond-to-tick conversion,
alarm scheduling, repeat limits and removal callbacks. Input reconstruction
covers button edges and repeat countdowns; display helpers upload and clear
OAM, set background colors and track frame/VBlank timing. The random generator
preserves its zero-seed fallback, mixing and signed modulo behavior. Shared
alarm declarations now expose the existing SDK operations by name.

All functions match completely, without inline assembly. The total is
259,428 bytes (16.59%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Display initialization and text-formatting helpers

Fourteen functions add 1,400 matching C bytes. Graphics initialization now
reconstructs the power/display register sequence, DMA/CPU clearing, display
mode save/restore, scanline comparison, interrupt enable bits and master
brightness. The formatting helpers cover string padding, string length,
binary and hexadecimal conversion. Decimal conversion remains unlinked.

All linked functions match completely, without inline assembly. The total is
260,828 bytes (16.68%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Stream players, worker commands and callbacks

Twenty-five functions add 3,160 matching C bytes. The recovered 348-byte
stream-player structure connects archive/file state, decoding, channels,
faders, handles and callbacks. Matching code covers command pooling and
queues, worker-thread creation/execution, channel references, player
allocation and stopping, fades, stream changes, buffer disposal, player
setup and initialization. The large read/decode and prepare routines remain
outside this linked set.

All linked functions match completely, without inline assembly. The total is
263,988 bytes (16.88%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Stream preparation and output effects

Linked `NNSi_SndArcStrmPrepare` and seven output-effect routines in matching C.
The reconstructed paths cover stream headers, file opening, ADPCM seek setup,
channel allocation, effect callbacks, mode changes, mono mixing and surround
history. These add 1,768 matching bytes, bringing the total to
265,756 / 1,563,700 bytes (17.00%, rounded).

All linked modules and the rebuilt ROM remain byte-identical; ROM SHA-1 is
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. All 66 tests, generated progress,
and the public-content audit pass. The stream decoder and headphone mixer
remain private analysis candidates and do not contribute to this total.

## Equipment, task factories and debug console

Sixteen functions add 2,436 matching C bytes. Equipment handling recovers the
five signed bonuses, base-stat application, original clamps, HP adjustment
and equipment selection. Eight factories allocate and construct the menu,
scene and credits tasks. The six console routines reconstruct text and
variadic formatting, cursor state, clearing, tile uploads and display setup.

All functions match completely without inline assembly. The total is
268,192 bytes (17.15%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass.

## Game audio playback and music preparation

Thirty functions add 3,048 matching C/C++ bytes. They recover delayed
effects, effect-handle selection, voice playback, stream controls, output
effects, music fades and bank preparation, common-bank loading and heap
destruction. The handle-validity paths preserve the original C++ Boolean
conversion. The previously named `SceneBackground` calls are identified
as music controls and now use the `GameAudio` names in their callers.

All linked functions match completely without inline assembly. The total
is 271,240 bytes (17.35%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass. The full
initialization, frame-update and loaded-music registration candidates remain
outside this linked set.

## Game audio loading queue and sample buffers

Nineteen functions add 2,492 matching C bytes. The reconstruction covers
reverb startup, heap-disposal callbacks, file-cache invalidation, intrusive
read queues, per-frame read budgets, loader transitions, sequence/bank
allocation, deferred wave headers and individual sample reads. Commands are
copied before reads overwrite the buffers that hold the queue links.

All linked functions match completely without inline assembly. The total
is 273,732 bytes (17.51%). Module/symbol checks, original-ROM SHA-1, all 66
tests, generated progress, and the public-content audit pass. The small
bank-instrument traversal candidate remains outside this linked set.

## OS locking and initialization

Eighteen functions add 980 matching C bytes. They implement card/cartridge
bus ownership, interrupt-protected lock acquisition and release, spin waits,
shared lock initialization, and the central OS initialization sequence.
The total is 274,712 bytes (17.57%), with no additional inline assembly.
Module/symbol checks, original-ROM SHA-1, all 66 tests, generated progress,
and the public-content audit pass. The RTC callback remains an unlinked
analysis candidate until its remaining callback accesses match.

## Rumble control and sprite output

Sixteen functions add 2,440 matching C/C++ bytes. Rumble control recovers
pattern playback, repetition counts, frame limits and accessory removal.
Sprite output includes stable OAM group sorting, sorted-buffer assembly,
state copies, palette dirty tracking, standard/extended VRAM uploads and
intrusive palette/allocation unlinking.

The total is 277,152 bytes (17.72%), with no additional inline assembly.
Module/symbol checks, original-ROM SHA-1, all 66 tests, generated progress,
and the public-content audit pass. Affine copying and the remaining OAM
registration routines retain their original binary implementations.

## Sprite animation initialization and frame updates

Ten functions add 1,892 matching C/C++ bytes. They reconstruct both animator
constructors, destruction, per-screen loading, tile uploads with the original
VRAM alignment, virtual frame dispatch, and effect-pool allocation/list setup.
The recovered layouts distinguish animation ranges, frames and OAM objects.

The total is 279,044 bytes (17.85%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress and the public-content audit pass, with no
additional inline assembly. Animation registration and tile remapping remain
private matching candidates until their remaining instructions agree.

## Sprite effects and planar image decoding

Ten functions add 2,536 matching C bytes. Effect activation, frame updates,
scaling, pause/resume and release now reproduce the original intrusive-list
behavior. The small returned image-size structure preserves the native ABI.
Three image decoders recover 8-by-8, 8-by-12 and 8-by-16 two-plane images
into the shared buffer with its 20-pixel row pitch.

The total reaches 281,580 bytes (18.01%). Module/symbol checks, original-ROM
SHA-1, all 66 tests, generated progress and the public-content audit pass.
No inline assembly was added. The wider/taller decoder and draw-dispatch
candidates remain outside the linked source set.

## Impact particle motion and scene audio reset

Three functions add 616 matching C bytes. Both impact-particle update paths
use the frame count captured after rendering for their remaining-distance
interpolation. The scene audio reset restores the sound heaps and player
channel allocation.

The total is 282,196 bytes (18.05%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress and the public-content audit pass. No inline
assembly was added. Encoded sound dispatch remains a private candidate.

## Text layout, control codes and character rendering

Eight functions add 4,512 matching C/C++ bytes. They reconstruct the 48-byte
text state, initialization/reset, line and bounds measurement, callback
registration, control-code dispatch and the complete character reader.
This includes font-bank selection, extended character codes, spaces, line
alignment, scaling, glyph drawing and accumulated text bounds. Callback
implementations confirm that the control mode is passed as a third argument.
The measured dimensions use a packed 16-bit return, while drawing returns
the small size aggregate.

The total reaches 286,708 bytes (18.34%). Module/symbol checks, original-ROM
SHA-1, all 66 tests, generated progress and the public-content audit pass.
No inline assembly was added. The callback-table relocations resolve to the
original addresses, and the existing battle-interface callers still match.

## Filesystem path lookup and reconstruction

Two functions add 1,500 matching C bytes. Path lookup handles both separators,
relative directory components, case-insensitive entry lookup and file/directory
results. Path reconstruction caches the required length, finds parent entries,
and writes the archive prefix and components into a capacity-checked buffer.
The directory-command argument retains its full input width until storage in
the on-disk directory-position representation.

The total is 288,208 bytes (18.43%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress and the public-content audit pass. No inline
assembly was added; the existing filesystem command functions still match.

## Map sound-wave selection and list loading

Two functions add 1,084 matching C bytes. Map-specific instrument lists extend
the common wave list, and the loader resolves each instrument's wave, queues
the sample reads, connects the bank to its wave archive, and starts deferred
loading at the requested byte rate. The former FieldAssets_LoadForMap symbol
is now named GameAudio_LoadMapWaves to reflect this recovered behavior.

The total is 289,292 bytes (18.50%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress and the public-content audit pass. No inline
assembly was added, and the battle background's callers remain identical.

## Sprite allocation and compaction

Two functions add 3,160 matching C bytes. Allocation handles fixed positions,
shared resource reuse, and forward or backward free-space searches. Compaction
closes gaps in either direction and marks moved allocations for upload. Both
use the recovered OBJ capacity calculation, including the display boundary
limit and available VRAM. The allocation metadata has both byte and word views.

The total is 292,452 bytes (18.70%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress and the public-content audit pass. No inline
assembly or compiler-setting changes were added. Existing sprite units remain
identical.

## Texture allocation, palette uploads and graphics offsets

Thirteen functions add 4,856 matching C bytes. Texture and palette allocation
handle fixed positions, shared resources and free-space searches. Palette
buffer management, dirty uploads and unlinking preserve the recovered flags
and list layouts. Graphics offset generation reuses the storage of repeated
images, both across an object list and within individual groups. Touchscreen
calibration reconstructs both hardware divisions and their range checks.

The total is 297,308 bytes (19.01%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress and the public-content audit pass. No inline
assembly or compiler-setting changes were added.

## Party initialization, inventory and matrix animation

Fifteen functions add 5,316 matching C bytes. Party initialization restores
starting levels, experience, growth stats, equipment and item counts. Inventory
queries and changes handle the four item categories, coins and beans, including
the original count limits. The equipment application signature now reflects
the byte-sized party member argument; existing callers and the callee match.

The matrix animation subsystem now includes all five curve evaluators, its
transform interpreter, track allocation and initialization, frame updates,
completion callbacks and owner release. The interpreter handles nested
sequences, scaling, translation, rotation and parameter channels. The recovered
layouts are 56 bytes per track and 100 bytes for the animation manager.

The total is 302,624 bytes (19.35%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress and the public-content audit pass. No inline
assembly or compiler-setting changes were added.

## Sprite animation management and vector scaling

Fifteen functions add 2,948 matching C bytes. Sprite animation management now
covers initialization, allocation, starting tracks, duration scans, frame
updates and owner release. Color packing clamps the three components before
encoding RGB15. Resource loading and overlapping 16-bit moves also match.

Vector helpers reconstruct bit-width measurement, individual and paired
scaling, cross products and cosine calculation using the hardware square-root
unit. Leading-zero counting uses a one-instruction inline-ASM helper; the
surrounding arithmetic and control flow are C. The sprite geometry interpreter
and nonmatching private candidates remain outside this milestone.

The total is 305,572 bytes (19.54%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress and the public-content audit pass. No compiler
settings were changed.


## Window uploads, background control and lifecycle

Twelve functions add 3,432 matching C bytes. Window graphics and tilemap
uploads, scroll registers, and background configuration now use recovered
window and manager layouts. Window release unlinks both embedded records,
stops active sounds, and updates the screen counts. Text measurement,
vertical alignment, tilemap rebuilds, and text sounds also match.

The total is 309,004 bytes (19.76%). Module/symbol checks, original-ROM SHA-1,
all 66 tests, generated progress and the public-content audit pass. No inline
assembly or compiler-setting changes were added.


## Window allocation and the 20% C/C++ milestone

Twenty-two functions add 3,824 matching C/C++ bytes. Window allocation now
searches free buffer regions, initializes the selected record, and inserts it
in address order. Pool creation and release, tilemap generation, text-cache
reset, pause/resume, property defaults and close operations also match.

The frame update handles both screen lists, text processing and sprite scroll
deltas. IRQ callbacks upload pending graphics through the original C++ virtual
call ABI. The base and sprite-window destruction paths release their owned
resources and tasks. Window properties, embedded sprite links and text caches
have compile-time size checks.

The total is 312,828 of 1,563,700 bytes (20.01%), exceeding the 312,740-byte
20% target. Module/symbol checks, original-ROM SHA-1, all 66 tests, generated
progress and the public-content audit pass. This milestone adds no inline
assembly and changes no compiler settings.


## Sprite windows

Eight functions add 1,528 matching C/C++ bytes. The sprite-window manager
derives from the ordinary window manager and presents window pixels through
OAM objects instead of a background tilemap. Its slot array is allocated in one
block that holds the slots followed by twelve hardware objects per slot; every
slot embeds a direct-mode sprite animation entry and the object-tile
allocation that backs it.

Slot construction, release and teardown, window opening (free-slot search,
object-tile allocation and animator registration), and closing (unlink plus
allocation release) match. The per-frame dirty scan dispatches tilemap windows
to the ordinary upload path and sprite windows to the object blitter.
`GameSpriteWindow_ApplyTileOffsets` rebases every object tile index after the
allocator has chosen a VRAM offset; recovering its return value, which the
original leaves in the object cursor, was what fixed its register assignment.

`GameSpriteWindow_Upload` and `GameSpriteWindow_BuildObjects` are reconstructed
but not yet byte-identical, so they stay outside `linked_sources.txt`.

The total is 314,356 of 1,563,700 bytes (20.10%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass. This milestone adds no inline assembly and changes no compiler
settings.


## Overlay 5 object, draw and sound services

Seventy-two functions add 4,300 matching C/C++ bytes in overlay 5. The overlay
owns three fixed-size object pools that share one shape: a constructed element
array, a parallel link array, and free/taken singly linked lists spliced
between them. Recovered pools cover the 336-byte sprite, the eight-byte draw
entry and the 64-byte item.

Sixty-four ordered draw lists per screen hold those entries; each list head
doubles as the sentinel its tail pointer parks on while the list is empty. The
per-frame update, draw and collect passes walk every list and dispatch through
the drawn object's virtual table, so those units are C++ with a minimal
interface declaration and extern "C" entry points.

Also recovered: touch-panel start-up and shutdown, the reference-counted
resource registry with its embedded head and tail markers, the affine matrix
built from the sine table, the object shape/size lookup, and the 32-slot sound
request queue with its priority-based replacement and per-frame countdown.

Two functions are reconstructed but not yet identical and stay outside
`linked_sources.txt`: a sprite pre-draw offset helper and an item reset whose
16-bit clear value the original materializes through memory.

The total is 318,656 of 1,563,700 bytes (20.38%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass. No inline assembly and no compiler-setting changes.


## Overlay 5 archive queue, element lists and display windows

Fifty-eight functions add 3,840 matching C bytes. The overlay queues archive
reads through a 128-entry request ring whose head, tail and free-list markers
are embedded in the owning object, dispatching each queued read either to the
battle archive reader or to the compressed-read path and recycling the entry
once its state word reports completion. Twelve opened archives keep their
descriptor and offset table side by side.

A second, 72-byte element pool feeds twelve update lists; elements carry an
update callback, a release callback and a flag word that marks them for removal
during the next sweep. Also recovered: the file read helper that sizes its
buffer from the open file, the resource-holder release paths, master
brightness, and the hardware window bounds and plane masks for both engines.

The total is 322,496 of 1,563,700 bytes (20.62%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass. No inline assembly and no compiler-setting changes.


## Overlay 5 asset loading and touch state

Six functions add 1,560 matching C bytes. Compressed and plain archive entries
are now loaded through the queue recovered earlier: each load first asks the
registry whether the key is already resident, otherwise constructs a
reference-counted buffer entry, prepares the read, sizes and allocates the
destination and links the result into the requesting asset. The synchronous
variant reads into a scratch buffer, dispatches on the four-bit compression tag
in the header word and frees the scratch buffer before returning.

The archive request record, its opened-archive table and the registry entry
types moved into `include/game/overlay005_archive.h`, which both archive units
now share. Touch input reads the four newest auto-sampling slots, keeps the
calibrated points, and derives held, pressed and repeat flags with a delay
countdown that reloads from two separate initial and repeat constants.

The total is 324,056 of 1,563,700 bytes (20.72%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass. No inline assembly and no compiler-setting changes.


## Overlay 5 asset row lookup

One function adds 344 matching bytes. An asset row names the image and palette
entries for a graphic; a per-row flag selects an alternate row for the current
save file, and the alternate rows are stored directly after the row array. The
lookup reuses a resident registry entry when the key matches, otherwise builds
a reference-counted asset and issues the image, tile and palette loads.

The total is 324,864 of 1,563,700 bytes (20.78%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass.


## Overlay 7 party value helpers

Thirteen functions add 884 matching C bytes in overlay 7. A roster keeps a
rotating display order over up to 128 slots plus the per-slot value the
interface shows; adding to a value clamps it to zero below and to a per-kind
limit above. The availability query subtracts the members already committed to
the same slot, counting the four save entries either in or out of the current
party depending on which query is asked, and reports a full bar for slot zero.
The scroll checks compare the rotating origin against the roster length.

The total is 325,748 of 1,563,700 bytes (20.83%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass.


## Overlay 7 point motion

Nine functions add 1,004 matching C bytes. A tracked point keeps its current
position, the position it held last frame and the position the interface draws
it at. Moves either apply immediately or start a timed motion whose per-frame
update interpolates a linear step or evaluates a quadratic curve, both scaling
by a 4.12 fraction of the elapsed time over the duration. The immediate path
walks a list to decide whether the previous position should follow the move.
Travelled distance is averaged through the hardware square-root unit.

The total is 326,752 of 1,563,700 bytes (20.90%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass.


## Overlay 7 item rows and caption measurement

Eight functions add 840 matching C bytes. The roster struct grows the per-kind
starting offsets and the 48-byte text object the captions are built in, so the
three units that model it now share include/game/overlay007_party.h. The item
table has 33 rows of 28 bytes: identifiers, a packed word holding an amount, a
percentage and a one-bit group, and five stat deltas that only the gear kind
copies out. Captions run the same tail: build the string, clear the text
cursor's x and y and measure the line. The membership count duplicates its loop
because each arm compares and increments on its own, which lets the compiler
unswitch the kind test out of the loop.

The total is 327,592 of 1,563,700 bytes (20.95%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass.


## Battle model accessors and the render lists

Twenty-seven functions add 1,624 matching C++ bytes across two units. The model
struct gains the render-list back pointer, the shared render-state block, the
resource header, the per-frame table and the anchor offset it carries between
models; its animation table turns out to be an array of eight-byte frame ranges
indexed by animation id, which is how the flee controller was already reading
it. The flag word splits into the animation mode, a three-bit state and the bit
that selects which render callback the scene installs. The render list itself is
one head and one tail per display engine, walked by two virtual-dispatch passes
and by the draw pass that records the object range each model claimed.

The total is 329,216 of 1,563,700 bytes (21.05%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass.


## Overlay 7 scene motion commands

Eight functions add 2,076 matching bytes. Five of them are script commands that
move a scene object: each joins the pairs of script words the argument-mode bits
say are still split, divides the fixed-point results down by sixteen, picks the
curve or the linear helper and writes the returned duration back into the
command's result variable when the caller asked for one. Alongside them sit the
animation-layer reset, which reaches the model through its virtual table, and
two small selectors over the scene block. The scene object gains the two signed
halfwords ahead of its animation id, which is itself signed.

The total is 331,292 of 1,563,700 bytes (21.19%). Module/symbol checks,
original-ROM SHA-1, all 66 tests, generated progress and the public-content
audit pass.

## Resident rendering, affine inverse and field transfers

Nineteen functions add 2,848 matching C bytes. The render-list operations
identify the model's display selector and membership bit. The model-animation
pool connects four model pointers to a matrix-animation track and releases its
context through the track's completion callback. The Q8 matrix inverse preserves
the original determinant rounding, saturation and overlapping hardware/software
division. Its callers now share the matrix type and the correct return type.

The field-background transfer object has a 16-entry queue, separate source,
destination and size arrays, and deferred alpha-blend parameters. Reconstructed
helpers upload palettes and tilemaps, apply scroll offsets, and drain the queue.
GX_LoadTex handles both contiguous texture storage and transfers crossing the
split between two physical VRAM spans.

Runtime verification booted the canonical EUR ROM from the raw battery save,
loaded the field, walked, opened the menu and returned to the field. The captured
calls include 70 render-list insertions, 64 removals, 666 matrix inversions,
104 queued transfers, 18 queue drains, 15 tilemap uploads, two palette uploads
and 702 scroll updates. Both field render lists passed link, display and
membership checks. RAM, mapped VRAM, palette, OAM and display-register captures
remain private under build/runtime/. The texture split and model-animation pool
did not receive runtime coverage in this route; their exact linked bytes passed.

The total is 334,140 of 1,563,700 bytes (21.37%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Field palettes and palette-animation commands

Fifteen functions add 3,264 matching C bytes. Field palette allocation separates
the three extended-palette workspaces from the shared standard palette and its
saved base copy. Upload and restoration preserve the original fallback and
backdrop-color paths. Following these consumers establishes that field 0x783
selects 256-color layers: the adjacent getter returns bytes per tile (32 or 64),
not tilemap width. The public names now reflect that distinction.

Nine palette-animation routines reconstruct keyframe selection, scaled color
operations, crossfading, cyclic rotation and reflected rotation. They preserve
overlapping source/destination handling and the signed time/amount conversions.
Three animation-table lookups resolve default and indexed component tracks.
The small RGB555 arithmetic kernels remain original code; naming them does not
increase the matching-C count.

Nine unreferenced symbols were actually unreachable trailing epilogues of the
preceding palette functions. Their bytes are now included in the correct
function sizes, verified against the original linked image. This changes no
code bytes or progress denominator.

The canonical EUR field/menu replay entered reflected cycling 329 times,
keyframe selection ten times, additive color animation eight times, blending
twice, default-track lookup 42 times, indexed-track lookup once, palette
allocation twice and palette upload twice. The observed sub-screen background
had mask 7 at 0x783 and BG1/2/3 control words 0x4A89, 0x4C92 and 0x4E97; all
three have the hardware 256-color bit set. This provides live register evidence
for the corrected field name. Other animation modes passed byte matching but
were not exercised by this route.

The total is 337,404 of 1,563,700 bytes (21.58%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Field palette controls and background animation state

Twenty-four functions add 2,440 matching C/C++ bytes. They reconstruct palette
effect setup, base-palette editing, layer parameters, character-memory selection,
blend-animation updates, palette-animation initialization and playback controls,
and tile-animation start/stop. The palette update preserves the original maximum
duration accumulated across active tracks. Adjacent palette allocation, upload
and restoration functions now share the same source module.

The resident vtables establish the C++ dispatch slots. Shared structures describe
the 24-byte palette effects, 12-byte tile-animation entries, four-byte blend
frames and animation state; compile-time checks verify the recovered object
sizes. C++ virtual calls reproduce the native BLX dispatch without inline ASM.

The canonical EUR menu-to-field replay executed 207 frames and captured 726 layer
parameter queries, 116 character-memory queries and two effect-list assignments.
The resulting field has 30 and 40 models on its two valid render lists. Captures
show 23,477 changed main-BG bytes, 81,042 changed sub-BG bytes and 764 changed
palette bytes. This route does not enter the background palette/blend playback
routines, so those have byte-match coverage but no playback coverage yet. Entry
snapshots of animation fields may contain uninitialized values before setup.
Evidence and the compatible checkpoint remain private under build/runtime/.

The total is 339,844 of 1,563,700 bytes (21.73%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Background lifetime, VBlank transfers and effect templates

Fifteen functions add 2,124 matching C/C++ bytes. The background constructor and
both destructors recover task registration, transfer-task allocation, initial
state, palette cleanup and shared tile-source ownership. Destruction clears
earlier duplicate tile-source pointers after freeing a source, avoiding repeated
frees. The transfer task initializes scroll registers, waits for background load
status 101, then processes dirty tilemaps, scroll, palettes, queued copies and
blend settings while the frame is waiting for VBlank. The original impossible
comparison of the one-bit palette-dirty field with 2 remains byte-identical.

Resource accessors, origin retrieval and scroll-parameter setup fill the small
contiguous accessor block. A 32-byte palette-effect template describes the
command header and two endpoint layouts by name; the setup routine no longer
needs unexplained halfword-array indices. Its battle caller uses the shared
prototype and palette-pointer types. Tile-animation resource fields were also
mapped from their consumers, but the larger playback routines remain assembly.

Runtime verification navigated from the save-point field through its normal
downward exit into the throne room. The rebuilt ROM captured one background
deletion and initialization, one transfer-task deletion and initialization,
38 readiness queries and 683 transfer updates. Transfer state progressed through
0, 1 and 255 with background status 101. Of the active-state entries, 651 were
inside the VBlank wait and 30 outside it. The constructor received priority 8
and its owning background pointer, agreeing with the reconstructed call.
The destination render lists contain 36 and 40 models without link errors.
Private captures contain 97,433 changed main-BG bytes and 218 changed palette
bytes. The effect-template routine was not exercised by this route.

The total is 341,968 of 1,563,700 bytes (21.87%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Model resource controls and controller lifetime

Thirty-eight functions add 2,080 matching C/C++ bytes. They reconstruct model
resource initialization and destruction, texture-conversion sizing, palette
access and masks, sort-key callbacks, animation-layer copying, controller
reset/configuration/restoration, and render-model construction/destruction.
Restoring detached render-list links and conditional texture release extend the
existing render-list module. Shared declarations replace the sort callbacks'
old void signatures and callers' incompatible constructor declarations.

The 0xA8-byte controller work area now describes both animation tracks and the
eight layer components, step values, states, modes, track pointers and animation
IDs. Reset and copy consumers establish those offsets; the resource flags also
distinguish normal and alternate texture boundaries. Compile-time checks retain
the complete model's 0x1B8-byte layout. All additions are structured C/C++.

The canonical EUR room-transition replay ran 362 frames and captured 52 controller
and resource-state initializations, 46 base destructions, 44 resets, eight full
configurations and 36 restorations. Every reset was followed by layer setup with
eight zero states, eight null track pointers and mode 2 in every layer. The final
render lists contain 36 and 40 models without errors. Captures include 31,610
changed main-OBJ bytes, 97,433 main-BG bytes and 218 palette bytes. Evidence and
the compatible checkpoint remain private under
build/runtime/eur_model_controller_verified. Render-model construction/deletion,
layer copying and conditional texture release were not reached on this route;
their complete linked bytes match. Constructor entry hooks intentionally omit
uninitialized storage from decoded snapshots.

The total is 344,048 of 1,563,700 bytes (22.00%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Palette-effect controls, localized text and window helpers

Sixteen functions add 1,364 matching C/C++ bytes. The eight palette-effect
controller routines cover construction, destruction, entry reset, setup, lookup,
pause status and reversal. Their shared 20-byte entry describes the effect mode,
color, palette mask, duration, elapsed Q8 time, speed, repeat mode and flags;
the existing model palette-mask accessor now uses that same type. The controller
is 12 bytes. Larger palette playback dispatch remains original assembly.

Two localized-text routines load fourteen resource tables and select entries
through the current language's relative-offset table. Number-control callbacks,
text-derived window dimensions and background/sprite release dispatch extend
the window subsystem. Loaded-music setup registers the sequence, bank and wave
files before starting playback and applying the current fade volume. The render
descriptor initializer extends the adjacent model lifecycle module. Each source
addition uses structured C/C++, with shared declarations and checked layouts.

A fresh canonical-ROM boot and save load ran 2,033 frames. It captured the text
table loader once, render-descriptor initialization 22 times, palette-controller
initialization four times, entry reset eight times and deletion twice. Both
screens initialized two entries per controller. Loaded-music setup played
sequences 37 and 32 from bank zero. The field's 30/40-model render lists remained
valid. A separate 330-frame item-menu replay captured 20 localized entry lookups
from tables 6 and 8, plus two palette-entry resets. Its screenshots show item
names and descriptions; main-BG memory changed by 58,124 bytes and palette memory
by 88 bytes. Menu overlays 5 and 7 matched their original loaded images.

Normal dialogue advancement exercised window release once across 1,830 frames,
with valid 36/40-model lists afterward. Number measurement, window sizing,
effect configuration/reversal/pause queries and entry lookup did not receive
runtime coverage on these routes. All their linked bytes match. Captures remain
private under build/runtime/eur_resources_boot_verified,
eur_menu_resources_verified and eur_throne_dialogue_verified.

The total is 345,412 of 1,563,700 bytes (22.09%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Render-model texture updates and palette state

Twenty-two functions add 916 matching C/C++ bytes. They cover filtered texture
updates across the main render list, individual texture uploads, alpha control,
texture/palette dirty flags, palette access, controller forwarding, resource
release, object-size queries and global texture-list reset. The 3D allocation
and palette occupy model offsets 0x130 and 0x148. These shared types replace the
opaque render storage and misleading palette flag alias; the model size remains
0x1B8. Two overlay-17 palette wrappers now declare and forward their receiver
instead of using incompatible prototypes. All affected callers still match.

A 400-frame canonical-ROM opening-animation replay captured 109 list updates,
2,398 individual texture-update calls, 2,420 palette-buffer queries, 77 dirty-flag
assignments and 22 controller restorations. Entry RAM showed 2,396 clean early
returns and two dirty uploads. Both uploads reached the copy routine from the
new model update at 0x0200FCD0, targeting CPU-visible addresses 0x06814E80 and
0x06805100. Sampled palettes contain 320 or 512 bytes. Other calls to the copy
routine came from existing 2D sprite paths and are counted separately.

The opening animation renders stars and clouds with 22 main and 51 sub-screen
models on valid render lists. Overlays 5 and 6 match their original loaded images.
Main/sub BG captures changed by 53,762/52,543 bytes and palettes by 82 bytes.
LCDC snapshots taken only before and after the run do not capture transient
texture-bank mappings, so they are not evidence of individual texture writes.
The trace and compatible checkpoint remain private under
build/runtime/eur_model_texture_verified. The frame-specific upload branch,
explicit palette editing, alpha control, resource release and list reset were
not exercised in this replay; their linked bytes match.

The total is 346,328 of 1,563,700 bytes (22.15%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Animation controls and background origin

Eleven functions add 1,028 matching C/C++ bytes. Model controllers now have
readable primary/layer animation selection, speed, state and mode controls,
including all-eight-layer updates and optional component reset. Background
origin updates use the configuration's three-bit relative-scroll mask to select
between Q8 displacement and per-layer scaling. A window effect callback clears,
redraws and marks its window dirty; it shares the adjacent scroll-link module.

A 362-frame normal field replay captured 137 primary animation selections,
47 layer animation selections, three background origin updates and one call
each to the layer state and mode setters. Entry RAM confirmed animation 3's
resolved layer track before the latter setters. The background configuration
used mask zero, with origin requests ending at (87, 126) from (8, 8).
The relative-scroll branch was not entered on this route.

The throne-area screenshot renders correctly, and both model lists remain
valid with 36/40 entries. Main/sub BG captures changed by 97,433/442 bytes and
palettes by 218 bytes. Evidence and a compatible checkpoint remain private
under build/runtime/eur_animation_controls_verified. Speed setters, primary
state/mode controls and the window redraw callback were not exercised here;
all linked bytes match. The harder frame-selection and component-preservation
routines remain assembly after isolated candidates showed register mismatches.

The total is 347,356 of 1,563,700 bytes (22.21%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Projection, tilemap patch activation and window dispatch

Six functions add 896 matching C/C++ bytes: orthographic projection setup,
tilemap patch activation, window presentation dispatch and three 16-pixel-wide
sprite decoders. The window dispatcher selects background or sprite rendering,
and the sprite-opening API now correctly describes its properties pointer and
requested index. Existing callers still match after that declaration correction.
The wider decoders compose two existing eight-pixel decoder calls.

Tilemap patches have a checked 20-byte layout containing a tile source,
an unresolved word, a rectangle and set/clear flag IDs. Vtable slot 0x3C takes
the patch index; its corrected declaration preserves the argument in the
activation routine. The actual tile-copy loop remains assembly after its C
candidate showed register differences. The field background's three source
tilemaps now have checked size/pointer pairs in the shared layout.

A fresh 2,033-frame boot/save-load replay captured projection setup once with
near/far Q12 values 0/-4096. Two patch activations reached the original tile-copy
loop: rectangles (13, 19, 8, 8) and (11, 9, 12, 10), setting flags 480/481 and
clearing 605/606. RAM showed these flags already had their requested values at
entry and retained them before tile copying; this run does not demonstrate a
flag transition. The resulting field has valid 30/40-model lists. Main/sub BG
memory changed by 55,110/47,962 bytes and palette memory by 243 bytes.

The window dispatcher and wider decoder wrappers were not entered during this
boot or the subsequent normal throne dialogue replay. Their linked bytes match.
Evidence remains private under build/runtime/eur_render_controls_verified and
eur_window_dispatch_verified. The total is 348,252 of 1,563,700 bytes (22.27%).
All module/symbol checks, 74 tests, generated progress and public-content checks
pass. The rebuilt ROM retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Session initialization and task lifetime

Eight functions add 836 matching C bytes: session settings initialization,
save-variable/party reset, session startup, task initialization/destruction and
two state-request helpers. Checked shared layouts describe the 16-byte session
settings and 44-byte task. Unknown option fields remain unnamed by purpose.
Settings use byte-width option arguments and retain the original language
fallback. The two request entry points select scheduler phases zero and two.

A fresh 2,033-frame boot/save-load replay captured startup, settings and task
initialization once each. Settings requested the system language with option
values zero/one. Task initialization used priority eight; the first state request
was nine, followed by phase-two requests for states two, eight and zero as the
opening sequence, load menu and field were reached. The final render lists are
valid with 30/40 models. Main/sub BG memory changed by 55,110/47,962 bytes and
palettes by 243 bytes. Evidence remains private under
build/runtime/eur_session_init_verified. Reset and destructor paths were not
entered on this route; their linked bytes match.

The total is 349,088 of 1,563,700 bytes (22.32%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## System startup and VBlank reset controls

Five functions add 740 matching C bytes: system initialization, owner-information
and MAC-address copying, reset-key polling and VBlank task dispatch. Shared
layouts describe the 80-byte owner-information result and 108-byte game system
record. Session language selection now uses that typed record. The firmware
value at settings offset 0x68 remains unresolved and its getter remains assembly.

A 400-frame cold boot exercised initialization and both copy routines once,
VBlank dispatch 394 times and reset-key polling 390 times. The opening animation
renders correctly with valid 22/51-model lists. Main/sub BG memory changed by
53,762/52,543 bytes and palettes by 82 bytes. A separate 184-frame replay from
the field held L+R+Start+Select for two frames: the reset request and fade loop
ran once at frame zero, followed by the original system-reset entry at frame one
with parameter one. The final screenshot shows the Nintendo/AlphaDream startup
logos. The replay's main/sub BG memory changed by 67,933/26,117 bytes.

Evidence and compatible checkpoints remain private under
build/runtime/eur_system_init_verified and eur_reset_keys_verified. The rumble
and deferred-reset branches were not covered. The larger main/session loops
and reset-fade routine remain assembly after isolated candidates showed literal
loading or register differences. The total is 349,828 of 1,563,700 bytes (22.37%).
All module/symbol checks, 74 tests, generated progress and public-content checks
pass. The rebuilt ROM retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## SDK initialization, reset handshake and file opening

Nine functions add 976 matching C bytes: arena initialization, vertical alarm
and VRAM ownership initialization, reset FIFO setup/send/receive, and two file
opening commands. Arena IDs and PXI interfaces now have shared declarations.
The file commands use the existing archive, file and cursor layouts; the fast
command reads a FAT entry before dispatching the direct-open command. The VRAM
unlock loop remains assembly after its candidate showed register differences.

A 400-frame cold boot entered all five initializers once, fast-open 1,163 times
and direct-open 1,171 times. The arena table captured from main RAM contains
main bounds 0x020CBFE0..0x023E0000 and disabled extended-main bounds 0/0.
The opening animation has valid 22/51-model lists. Main/sub BG memory changed
by 53,762/52,543 bytes and palettes by 82 bytes.

A separate 184-frame replay from the field held L+R+Start+Select for two frames.
At frame one the reset sender received command 0x10 and the callback received
tag 12, data 0x1000 and error zero. Reset initialization ran again at frame
seven, and the final screenshot shows the Nintendo/AlphaDream startup logos.
The callback's unexpected-command termination branch was not exercised.
Evidence remains private under build/runtime/eur_sdk_initializers_verified and
eur_sdk_reset_fifo_verified. The total is 350,804 of 1,563,700 bytes (22.43%).
All module/symbol checks, 74 tests, generated progress and public-content checks
pass. The rebuilt ROM retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Field timer controls and clock updates

Seven functions add 764 matching C bytes. The checked 40-byte timer layout
contains signed minute/second/frame counters, a signed count step, displayed
hundredths, screen position and sprite allocation. The setter takes frames as
its third clock argument; it converts those to hundredths using frames*100/60.
The field VM now includes the shared declarations. Its compiled bytes remain
unchanged. The timer's OAM builder remains assembly after its isolated C
candidate showed register differences.

A normal 2,033-frame boot/save-load replay entered initialization and image
setup once each, then updated the timer 251 times. The live timer resides at
0x02324AC0 and uses image 0x020CE624. This route leaves its count step zero and
its display unallocated. Render lists remain valid with 30/40 models; main/sub
BG memory changed by 55,110/47,962 bytes and palettes by 243 bytes.

Seven controlled runtime cases then reloaded that same compatible field state,
seeded only pause bit 2 and the five clock bytes, and executed one normal frame
each. The loaded update function was compared to the original overlay bytes
before every case. Each case entered the updater once and produced the expected
result: minute rollover, saturation at 99:59.99, countdown borrowing a minute,
stopping exactly at zero, clearing an underflow, retaining paused counters,
and retaining stopped counters. These are live RAM-seeded boundary checks,
separate from the normal gameplay replay; no ROM or battery save was modified.

Evidence remains private under build/runtime/eur_field_timer_verified and
eur_field_timer_boundaries. Visibility changes, position/value setters and
destruction were not entered by the normal route. The total is 351,568 of
1,563,700 bytes (22.48%). All module/symbol checks, 74 tests, generated progress
and public-content checks pass. The rebuilt ROM retains SHA-1
BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Field model and sprite animation ownership

Nine functions add 1,008 matching C bytes: model-animation allocation, cleanup,
model selection and track creation, plus sprite-animation allocation, cleanup,
track creation, cancellation and display restoration. Shared headers now own
the existing model-animation pool/context layouts and checked field owner
layouts. Empty matrix/sprite finalizers now accept the owner pointer passed by
the original callers. All existing compiled bytes remain unchanged.

A normal 2,033-frame boot/save-load replay entered both owner initializers once.
Read-only inspection of that checkpoint verified three acyclic free lists and
their tail pointers: 32 matrix tracks at stride 56, 32 model contexts at stride
40, and 128 sprite tracks at stride 48. The model owner is at 0x023209A0 and the
sprite owner at 0x020C9B58. The final render lists contain 30/40 valid models;
main/sub BG memory changed by 55,110/47,962 bytes, OBJ by 12,297 and palettes by
243 bytes. A subsequent 362-frame throne-room transition retained valid 36/40
model lists but did not enter additional helpers in this batch.

Track creation, cancellation, display restoration and cleanup remain outside
the exercised gameplay routes; their linked bytes match. The larger sprite
update loop and model preparation callback remain assembly. Evidence remains private under
build/runtime/eur_field_animation_verified, eur_field_animation_pools and
eur_field_animation_transition_verified. The total is 352,576 of 1,563,700 bytes
(22.55%). All module/symbol checks, 74 tests, generated progress and public-content
checks pass. The rebuilt ROM retains SHA-1
BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Timed field renderer animation controls

Four functions add 452 matching C++ bytes: animation update/expiry, reset,
repeat-limit selection and restarting at the first or last frame according to speed.
The restart helper uses the existing virtual animation interface. A checked
0x140-byte layout extends the field renderer with a signed 11-bit repeat counter,
completion flag and expiration counter. The shared renderer now describes its
animation ranges, resource animation, speed and positional offsets.

The model preparation callback's bytes 0x134..0x137 correspond to the existing
FieldRenderObject overlap priorities used by field VM command 0x062. Model
animation pool interfaces now use that renderer type and name the corresponding
context fields. The callback itself remains assembly after its isolated C
candidate showed register differences. Existing pool, owner and VM bytes still
match after these declaration corrections.

A normal 2,033-frame boot/save-load replay entered the updater 6,260 times
across 41 renderers and the reset helper 12 times. Updates included 1,772 active
and 4,488 inactive cases; all had zero remaining time. The field retained valid
30/40-model lists, with main/sub BG changes of 55,110/47,962 bytes and palette
changes of 243 bytes. Repeat-limit selection and restart were not entered by this route.

Three separate live RAM-seeded cases reloaded that normal field checkpoint.
A remaining count of one set the expired flag after one frame, then disabled
animation on the next frame. A finished animation in behavior state one
disabled animation immediately. An inactive animation retained its remaining
count of two. Hooks confirmed the disable calls and their zero argument.
These controlled counter/flag checks changed neither ROM nor battery save.
Evidence remains private under build/runtime/eur_timed_renderer_verified and
eur_timed_renderer_expiry. The total is 353,028 of 1,563,700 bytes (22.58%).
All module/symbol checks, 74 tests, generated progress and public-content checks
pass. The rebuilt ROM retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Field renderer repeat limits and geometry helpers

Five functions add 648 matching C/C++ bytes: finite animation repetition,
resource-section size/address lookup, a rounded Q12 orientation predicate and
eight-direction vector lookup. The repeat handler establishes that the signed
11-bit field is a loop counter. Field VM command 0x06C now explicitly passes
argument 3 to the correctly declared setter; the original instructions already
kept that argument in r1, and the corrected C++ call produces identical bytes.

Checked layouts distinguish the 0x13C-byte animation renderer from its 0x140-byte
extension with a frame-lifetime counter. Runtime decoding reads the extension
only for the timed update entry, avoiding unrelated memory after base objects.
The larger vector-scaling helpers remain assembly after isolated candidates
showed register and control-flow differences.

A normal 2,033-frame boot/save-load replay entered animation update 10,630 times,
the orientation predicate 340 times, direction lookup 236 times, section-size
lookup 27 times and section-address lookup twice. It retained valid 30/40-model
lists; main/sub BG memory changed by 55,110/47,962 bytes and palettes by 243 bytes.

Three live RAM-seeded cases reloaded an active renderer with a two-frame
animation, placed it on its final frame and selected loop counts two, one and
zero. Count two became one and called restart once, returning to frame zero.
Counts one and zero marked completion without restarting and became inactive
on the next frame. These tests modified only transient renderer state, leaving
ROM and battery save unchanged. The setter was not entered by the normal route;
its source and the corrected VM call both match the linked reference.

Evidence remains private under build/runtime/eur_renderer_loop_geometry_verified
and eur_timed_renderer_loops. The total is 353,676 of 1,563,700 bytes (22.62%).
All module/symbol checks, 74 tests, generated progress and public-content checks
pass. The rebuilt ROM retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Field renderer construction, priorities and cleanup

Seven functions add 596 matching C++ bytes: both animation-renderer constructor
entries, animation/timed base destructors, descriptor restoration, mode-indexed
overlap-priority lookup and screen render-list cleanup. They extend the existing
contiguous renderer module. The shared field layout now describes its render
links and sprite allocation; descriptor bits 0..1 supply all four overlap
priorities. All seven initial structured candidates matched the original bytes.

A normal 2,033-frame boot/save-load replay entered the complete constructor 41
times, base constructor 45 times, controller restoration 70 times and priority
lookup 12,445 times. Live descriptors on the DTCM stack requested priorities
zero and one, also observed by subsequent priority lookups. Runtime decoding
now accepts this stack region for descriptor and sort-key arguments. The final
render lists contain 30/40 valid models; main/sub BG memory changed by
55,110/47,962 bytes and palettes by 243 bytes. Visual inspection confirmed the
normal field screen. List cleanup and the two destructors were not entered by
this route; their linked bytes match exactly.

Evidence remains private under build/runtime/eur_field_renderer_lifecycle_verified.
The total is 354,272 of 1,563,700 bytes (22.66%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Deferred field-entity script controls

Eleven functions add 1,336 matching C++ bytes: entity initialization and cleanup,
state copying, script stop/pause/resume, blink completion, and a 180-frame timed
pause. A checked 0x524-byte extension names the deferred phase and pause counter;
unresolved flags and constructor arguments retain neutral names. Removing an
unnecessary explicit halfword cast reproduces the original counter decrement.
Other differences in the isolated objects were ordinary call relocations.

Following virtual slot 0x74 through its wrapper and default callbacks confirms
that it starts a visibility sequence: the callbacks enable and disable the
primary renderer and eligible auxiliary renderer. The shared interface now
names this method start_blink and types its duration table and callbacks.
Base-state bits 13..14 select the blink mode. These names follow original code
data flow; no particular gameplay encounter is inferred for this subclass.

A normal 2,033-frame boot/save-load replay completed with valid 30/40-model
lists and the expected field display. Main/sub BG memory changed by
55,110/47,962 bytes and palettes by 243 bytes. None of this subclass's eleven
entries was reached on that route, so this is a ROM smoke check, not runtime
coverage of the new functions. Their complete linked bytes match the original.
Evidence remains private under build/runtime/eur_field_deferred_entity_verified.
The total is 355,608 of 1,563,700 bytes (22.74%). All module/symbol checks,
74 tests, generated progress and public-content checks pass. The rebuilt ROM
retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Shared field blink sequencing and renderer callbacks

Eight functions add 1,124 matching C bytes: blink start/update/stop, the renderer
callback wrapper, show/hide callbacks and two sort-key-to-anchor copies. The
shared layout now describes the signed duration table, phase callbacks, mode,
pause flag, phase bit, signed eight-bit frame count and seven-bit table offset.
Mode three advances through duration pairs; other active modes move backward.
A signed -1 duration terminates the sequence. The virtual blink interface now
correctly uses signed duration bytes. Unresolved auxiliary-renderer conditions
retain their existing neutral field names.

A normal 2,033-frame boot/save-load replay entered blink update 2,774 times,
the two-anchor copy 5,554 times and the single-anchor copy 354 times. These
entities had blink mode zero. The final render lists contain 30/40 valid models;
main/sub BG memory changed by 55,110/47,962 bytes and palettes by 243 bytes.

Seven live RAM-seeded checks then reloaded one normally updated entity at
0x02315BC8, installed the original duration table and renderer callbacks, and
executed one ordinary game frame per case. Each entered the updater exactly
once. The checks verified first-phase switching, forward/reverse pair stepping,
both end sentinels, paused state and stopped state. Callback traces and the
primary renderer's active flag agreed: reverse completion hid then showed the
renderer and restored property flag zero; forward completion hid it and ended
the sequence. Loaded updater/callback bytes were checked against the original
overlay before each case. Only transient blink state and property flag zero
were seeded; ROM and battery save were unchanged.

Start, stop and the default-callback wrapper were not entered on these routes.
Evidence remains private under build/runtime/eur_field_blink_verified and
eur_field_blink_boundaries. The total is 356,732 of 1,563,700 bytes (22.81%).
All module/symbol checks, 74 tests, generated progress and public-content checks
pass. The rebuilt ROM retains SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Field entity lifetimes, motion parameters and bounds

Twenty-one functions add 1,428 matching C++ bytes: planar/spatial constructors
and destructors, primary/auxiliary renderer allocation and release, renderer
deletion, locomotion/vertical parameter setters and restoration, behavior-mode
selection and interaction bounds. Shared declarations identify the 0x2B0-byte
planar allocation, 0x520-byte spatial allocation and 0x13C-byte renderer. The
larger FieldRuntimeEntity interface describes offsets shared by both variants;
fields beyond the planar allocation belong to the spatial extension.

Allocation uses heap bits 9..11 of entity property 0x00A and retains the
constructor's returned pointer. Release preserves the primary animation frame,
unlinks a linked renderer, frees its texture-offset array and invokes the
deleting virtual method. The shared layout now names the current and initial
locomotion parameters, signed bounds records, heap selector and optional-renderer
allocation flag. Field VM calls use the shared behavior, bounds and locomotion
declarations with unchanged compiled bytes. The packed bounds-table lookup
remains assembly after its candidate selected different address instructions.

A normal 2,033-frame boot/save-load replay entered spatial initialization four
times and its planar initializer four times. It entered primary renderer
allocation 16 times and the spatial allocation wrapper 13 times, locomotion
restoration 69 times, spatial motion restoration 35 times, bounds selection
35 times, behavior selection ten times and locomotion selection twice. Live
initial locomotion parameters were [8192, 0, 8192, -8192, 0, 8192] in Q12 units.
Observed bounds included x=-8, y=0, width=16, height=8 or 16 and vertical extent=32.
Final render lists contain 30/40 valid models; main/sub BG memory changed by
55,110/47,962 bytes, main OBJ memory by 12,297 and palettes by 243 bytes.

Cleanup, alternate constructor entries and the vertical setter were not entered
by this route; their linked bytes match. Evidence remains private under
build/runtime/eur_field_lifetimes_verified. The total is 358,160 of 1,563,700 bytes
(22.90%). All module/symbol checks, 74 tests, generated progress and
public-content checks pass. The rebuilt ROM retains SHA-1
BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Planar entity placement initialization

Two constructor entries add 1,048 matching C++ bytes and extend the existing
entity lifecycle module. A shared, checked 28-byte spawn record describes
eight-pixel cells, signed pixel offsets, animation/facing selection, appearance
and placement flags. Both entries share one inline implementation. The
remaining register difference was resolved by preserving the original operand
order when combining the record's animation flag with the caller's enable bit.

Live arguments and the base constructor establish that argument one is an
entity index, stored at byte +4. The placement and deferred-entity interfaces
now use an integer index and integer resource-set selector, correcting earlier
opaque pointer declarations. Unresolved argument three remains neutral. The
adjacent base initializer candidates remain assembly because a conditional-move
ordering difference persisted; their additional typed fields come from direct
inspection of the original stores.

A normal 2,033-frame boot/save-load replay entered the complete placement entry
31 times and the base placement entry three times. Read-only entry/return hooks
checked all 34 calls, including 16 with null spawn records. At return, indices,
Q12 positions, resource indices, presentation bytes, initial locomotion
parameters, Q8 animation speed 256 and bounds index -1 matched the inputs and
constructor defaults. Null records produced zero positions and resource
selection. Loaded constructor bytes were checked against the original overlay.
No runtime state was seeded for these checks.

Final render lists contain 30/40 valid models; main/sub BG memory changed by
55,110/47,962 bytes and palettes by 243 bytes. Evidence and the 34 postcondition
results remain private under build/runtime/eur_field_spawn_verified. The total
is 359,208 of 1,563,700 bytes (22.97%). All module/symbol checks, 74 tests,
generated progress and public-content checks pass. The rebuilt ROM retains
SHA-1 BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Spatial entity placement initialization

Two constructor entries add 3,272 matching C++ bytes to the spatial lifecycle
module. Their shared inline implementation initializes height, collision and
navigation flags, shadow selection, render priorities and vertical motion
parameters from the checked spawn record. Direct instruction comparison resolved
the subtype switch and a five-bit field that is initialized to 15. Keeping the
subtype predicate next to its switch reproduces the original register allocation.
The collision-channel masks preserve the original signed 32-bit extension into
the 64-bit policy; the resulting default is 0xFFFFFFFFC0FFFFFF.

A normal 2,033-frame boot/save-load replay entered the complete constructor 21
times and the base entry ten times. Read-only entry/return hooks checked the
loaded instructions against the original overlay and verified all 31 results:
Q12 height, initial and current vertical parameters [21496, 1296, 0], signed
sentinels, collision category and policy, roaming defaults, spawn collision
flags, synchronization, priority selection, subtype predicate and shadow flags.
All checks passed without seeding runtime state. Observed height included 48
pixels (196608 in Q12 units).

Final render lists contain 30/40 valid models; main/sub BG memory changed by
55,110/47,962 bytes and palettes by 243 bytes. Evidence remains private under
build/runtime/eur_field_spatial_spawn_verified. Matching C/C++ is 362,480 of
1,563,700 bytes (23.18%). All module/symbol checks, 74 tests, generated progress
and public-content checks pass. The rebuilt ROM retains SHA-1
BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Planar and spatial orbit controllers

Twelve functions add 2,792 matching C++ bytes: radius adjustment, planar and
spatial position calculation, pause/resume, cancellation, frame advancement,
braking, movement updates and spatial stop masks. A checked 72-byte controller
replaces the opaque +0x238 block. Vtable slots and the reconstructed calculations
establish orbital movement, correcting the earlier vertical-controller name in
the Scene VM interface. Spatial modes operate in Y/Z, X/Z or X/Y and can follow
another entity's displacement on the remaining axis. The fixed-point expressions
retain explicit intermediate values and the original addition grouping.

A normal 2,033-frame boot/save-load replay entered the spatial update and frame
advancement functions 2,100 times each and the stop-mask check 5,578 times. All
observed controllers were inactive. The planar helpers, spatial cancellation
and active position calculation were not entered naturally by this route.
Final render lists contain 30/40 valid models; main/sub BG memory changed by
55,110/47,962 bytes and palettes by 243 bytes.

Thirteen additional controlled cases reload that checkpoint, seed only the
embedded orbit controller of a normally updated field entity, and execute one
game frame. All pass: each plane with a fixed center and a live entity center,
paused/inactive controllers, fixed-duration and remaining-angle completion,
entering braking distance, staying outside it and the braking speed floor.
Read-only entry/return hooks verify calculated coordinates against live positions
and the game's sine table. The cases perform 13 position checks, including two
for each completion case. Loaded function bytes match the original overlay.
These are RAM-seeded boundary checks, not naturally triggered story events;
planar pause/resume and cancellation remain covered by exact linked bytes.

Evidence remains private under build/runtime/eur_field_spatial_orbit_verified
and build/runtime/eur_field_orbit_boundaries. Matching C/C++ is 365,272 of
1,563,700 bytes (23.36%). All module/symbol checks, 74 tests, generated progress
and public-content checks pass. The rebuilt ROM retains SHA-1
BA4EC2F99B4F2E0047601552BCCF00AA73E28701.

## Orbit startup and Field VM integration

Five starters add 2,688 matching C++ bytes: four planar variants for timed or
speed-based orbits around points/entities, plus timed spatial orbits around a
point. The shared interface now identifies initial angle and vertical radius.
The Field VM calls typed, named starters instead of opaque assembly declarations.
Angle wrapping preserves complete turns from the high word, winding direction,
and the original in-place accumulation. Three spatial starter candidates remain
assembly because register allocation differences persisted.

Three controlled live tests invoke the new spatial starter through Field VM
opcode 0x88, once for each plane. Each test reloads the normal field checkpoint,
checks that entity table index 9 resolves to the observed runtime entity, and
replaces one decoded RAM command at a natural dispatch. All 72 original command
bytes are restored at the starter's entry after argument passing. No script
bytecode, ROM or battery save is changed. Read-only entry/return hooks verify
all fourteen arguments, success, flags, center coordinates, initial/destination
angles, radius, circumference, duration and derived speed. Every case passed
within one game frame. Radii were 46340/32768/32768 and speeds 6065/4289/4289 in
Q12 units for planes Y/Z, X/Z and X/Y respectively.

These are controlled command-injection checks, not naturally triggered story
events. The planar starter entries remain covered by exact linked bytes; normal
field smoke and orbit boundary evidence from the preceding milestone uses the
same byte-identical ROM. New evidence remains private under
build/runtime/eur_field_orbit_start_verified. Matching C/C++ is 367,960 of
1,563,700 bytes (23.53%). All module/symbol checks, 74 tests, generated progress
and public-content checks pass. The rebuilt ROM retains SHA-1
BA4EC2F99B4F2E0047601552BCCF00AA73E28701.
