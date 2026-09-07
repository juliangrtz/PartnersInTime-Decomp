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
