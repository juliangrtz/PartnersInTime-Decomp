# Reconstruction and runtime reference

The repository [AGENTS.md](../../AGENTS.md) contains the working instructions.
Read the relevant sections here before reconstructing code or extending a probe.
This reference preserves the detailed findings and tested routes accumulated
during earlier batches; current source and fresh checks take precedence.

Paths in backticks are relative to the repository root unless explicitly absolute.
Private files under `build/`, `extract/` and `PiT_SaveStates/` are local research
resources, not files included in a fresh clone. Check their availability and
provenance before reuse. Evidence from a controlled fixture does not establish
ordinary gameplay accessibility or coverage of unexercised branches.

- [Locating evidence and comparing candidates](#locating-evidence-and-comparing-candidates)
- [Compiler and linker behavior](#compiler-and-linker-behavior)
- [Reconstructing and integrating code](#reconstructing-and-integrating-code)
- [Runtime verification](#runtime-verification)
- Tested routes: [shops](#shops), [save menus](#save-menus), [Game Over](#game-over),
  [Smash Eggs](#smash-eggs), [credits](#credits), [Nawatobi](#nawatobi)

## Locating evidence and comparing candidates

Old milestone notes describe the state at the time they were written. Current
source, `delinks.txt`, `symbols.txt`, the linked-source manifest and fresh checks
take precedence over old counts or claims that a function is unfinished.
Distinguish the working tree, committed code and pushed code. A pending source
integration can leave the checked-in progress files behind the local metadata;
do not publish its extra bytes before completing the batch's verification.
Read the latest request before following a handoff's suggested next action.
The private `build/analysis/CURRENT_HANDOFF.md`, when present, records pending
candidates and completed checks. Check its commit ID against `git log`; a
handoff can lag behind a pushed batch. It is context, not an instruction to
continue a superseded task or proof that a private candidate is linked.

Resident game helpers are in `src/game/`, SDK routines in `src/nitro/`, field
code in `src/field/`, battle code in `src/battle/`, and scene/menu/attack code
in the corresponding `src/overlayNNN/` directory. Use the overlay map to select
a subsystem; do not infer a gameplay identity from an overlay number alone.
Original ARM9 bytes are under `extract/eur/arm9/` and
`extract/eur/arm9_overlays/`; derive load addresses from component metadata.
Resident ARM9 metadata is directly in `config/eur/arm9/symbols.txt` and
`config/eur/arm9/delinks.txt`. Overlay metadata is under
`config/eur/arm9/overlays/ovNNN/`. The private pseudocode directory name
`main_game` does not imply a corresponding metadata subdirectory.
The extracted EUR resident `arm9.bin` starts at `0x02004000`, not the start of
main RAM at `0x02000000`. Derive file offsets from the component's actual base;
check lookup-table bytes against live RAM before relying on a runtime oracle.

Private IDA databases live under `build/ida/`; previous pseudocode exports and
candidate units live under `build/analysis/`, including
`build/analysis/local_decompiler/`. Recompile a candidate against current headers
before trusting an old matching object. Abandoned drafts may contain incorrect
experimental prototypes. Parse large JSON reports and select the relevant
records instead of dumping entire reports into the conversation.
Keep each comparison associated with its exact source, language mode, object
and current headers. A neighboring `.cpp` draft may predate the matching `.c`
version. Private comparison scripts can overwrite address-named candidate dumps;
do not treat an older disassembly listing as the output of the latest compile.

On this workstation, private comparison helpers include
`build/analysis/compile_private_unit.py`, `check_title_unit.py` for overlay 6,
`check_overlay8_unit.py` for overlay 8, `check_overlay9_unit.py` for overlay 9,
`check_overlay15_unit.py` for overlay 15,
and `check_main_unit.py` for resident ARM9. Inspect their inputs and relocation
handling before reuse; they are local conveniences, not required public tools.
Use current public build checks as the final authority. Private paths in this
file are research references, not files supplied by a fresh clone. If one is
absent, reconstruct the needed evidence from the public tooling, native bytes
and user-supplied ROM/saves rather than treating an old report as a fresh check.
After a symbol rename, ensure the private checker resolves the new symbol in
the candidate object. A fallback to an address-named symbol or original bytes
must not turn a missing candidate function into a passing comparison.

Candidate inventories are only discovery aids. Recheck each range against the
current manifest before spending time on it: a previous candidate may already
be linked, or an old inventory may assign an overlay function to resident ARM9.
Inspect the body before calling a target easy. An indirect jump represented by
one line of pseudocode can hide a dispatcher, and a short graphics function can
omit most of its FIFO writes. Prefer a coherent group with understood callers
and a reproducible runtime route. Keep the next candidate independent of a
deferred register-allocation problem.

## Compiler and linker behavior

Do not change compiler flags to force a match. Derive individual object builds
and comparisons from the existing Ninja/objdiff configuration. The configured
ARM946E target, signed `char`, enum width, software floating point, interworking
and C++ mode are part of the original ABI.
This compiler's configured C mode requires declarations at the start of a
block. Use a justified nested scope when needed; do not switch a whole unit
to C++ merely to accept declarations after statements.

Absolute linker symbols such as `OVERLAY_5_ID` and `OVERLAY_6_ID` carry their
numeric value in the symbol address. Resolve them from the generated
`build/eur/linker_script.lcf` when checking private objects. Preserve the
native literal relocation where required; reading such a symbol as a C variable
would dereference that number as an address.

A decompiler can fold a condition after resolving a linker value to a constant,
even though both branches remain in the original instructions. The resident
thread initializer at `0x020396D8` retains a stack-bound calculation conditional
on a literal that is zero in the EUR binary. Recover the original relocation
and linker expression before replacing such a value with a C constant; do not
invent an SDK symbol name or add volatile accesses to recreate the missing
branch. Until that evidence is available, defer the function.

### OAM and transform views

Recover the relationship between different views of a graphics buffer before
changing its type to explain a compiler mismatch. `GameOamEntry` is eight bytes,
while `func_02009598` accesses 32-byte groups through `BattleTransformRow` in
[the model-state implementation](../../src/battle/battle_model_state.cpp).
It reads and rewrites the halfwords at group offsets 6, 14, 22 and 30 using
`GameAffine_InvertQ8`. These are the affine fields spread across four OAM records.
Its callee definition uses byte-sized first/end indices; existing callers and
private drafts have differing declarations. Check those accesses and every
affected caller before introducing a shared view or correcting a prototype.
The helper's implementation establishes more than an old draft's generic name
or pointer cast. A same-sized buffer alone does not establish compatible types.

## Reconstructing and integrating code

1. Locate the original function and its boundaries in the component's
   `config/eur/arm9/**/symbols.txt` and `delinks.txt`. Inspect the instructions,
   callers and neighboring functions; decompiler pseudocode is a hypothesis.
2. Recover field widths, signedness, offsets, virtual calls and ownership from
   those accesses. Preserve original behavior, including awkward edge cases.
   Use neutral names for unknown fields; do not guess item or enemy identities.
   Check call sites and callee accesses before changing a prototype, including
   apparently unused arguments. An incorrect prototype can cause the register
   differences that otherwise look like a compiler-scheduling problem.
   Recover stack-passed arguments from the instructions before the call;
   pseudocode has omitted motion parameters and cached object loads. Check
   `LDRSB` versus `LDRB` even for small phase counters: signedness affects their
   increment and comparison behavior.
3. Keep related contiguous functions in a subsystem module. Temporary isolated
   units are acceptable while an intervening assembly gap remains; consolidate
   once that gap is recovered. Shared declarations and layouts belong in headers.
   Check source basenames across all components before adding a unit. The MW
   linker script selects objects by basename, so `attack_entry.c` would collide
   with `src/overlay021/attack_entry.cpp` even in a different directory.
4. Preserve native function order. This MWCC setup generally emits the separate
   function sections in reverse source order, so definitions usually descend
   by original address. Verify the emitted order instead of assuming it.
5. Compare the compiled instructions, sizes, relocations and symbol layout.
   Only verified units belong in `config/eur/arm9/linked_sources.txt`. A source
   file can compile or appear in objdiff while the ROM still uses its original
   delinked object; that is not completed matching C.
6. Update the component metadata and affected declarations together. Preserve
   shipped interior entry points through `config/eur/arm9/linker_aliases.json`;
   do not silently turn an interior label into a different function.
   When renaming a symbol, also check `reasm/eur/patches.json` and maintained
   assembly references. Resolve names within the correct CPU and overlay;
   a raw address alone is ambiguous. Keep external addresses and relocation
   checks intact rather than weakening validation to accept stale names.
7. Run `ninja check` for every C/C++ change, including shared-header changes.
   Verify the packaged ROM and relevant runtime behavior before reporting the
   batch complete. Record functions or branches that were not reached.

Prefer structured control flow and readable expressions. Add compile-time size
checks for recovered structures. Keep casts and offset arithmetic only where
the known layout or compiler behavior requires them.
The project's integer typedefs are in `<nitro.h>`; inspect existing includes
before assuming that a header such as `nitro/types.h` exists.

When only an object's prefix is known, describe it explicitly as a prefix view;
its `sizeof` does not establish the full allocation size. Two objects can share
a prefix while having incompatible tails. The 96-byte primary title prompt and
120-byte rumble prompt share only their first 60 bytes; do not embed the complete
primary prompt as the rumble prompt's base. Check each tail against its own
constructor and consumers, even if a larger placeholder previously fit.

The same rule applies to runtime captures: a buffer read through a `BattleModel`
pointer does not prove that the concrete allocation has the complete declared
layout. Derive the capture extent from its constructor or the verified prefix;
label a larger exploratory memory window separately from an object record.

Decode literal pools as little-endian data, not as ARM instructions. Check
ARM/Thumb state and interworking relocations when comparing calls; a private
comparison script does not replace the full module and symbol checks.
When using Capstone, verify that the listing reaches every intended function's
end. An undecodable literal-pool word can silently stop a combined disassembly
before later functions. Split the input at known function boundaries or enable
`skipdata`, then identify the actual code and data regions from metadata and
references. Skipped bytes are not proof that the remaining code was inspected.

Decompiler output can omit repeated writes to the same GPU FIFO address.
Inspect every native store and preserve its width, value and order with volatile
MMIO accesses. In the credits controller, `0x04000470` receives all three
translation components, and `0x0400046C` receives all three scale components;
the final store alone does not describe either operation. The same applies to
texture-coordinate and vertex sequences. Recover missing writes before trying
to explain a short candidate through compiler scheduling.

Preserve the input loads around those stores too. For a translation sequence,
the native code can cache depth before sending the zero X/Y components. Loading
depth only for the final FIFO write changes the instruction order even though
the three intended values are unchanged. Follow the observed load/store order
with an ordinary local variable; do not add volatile fields to force scheduling.

A stored byte or halfword does not imply a narrow function parameter. Preserve
the full-width argument and the native truncation point when callers pass an
`int`; narrowing the prototype can change caller code and signedness. A table
lookup may use that full-width index before its low byte is stored elsewhere;
preserve the lookup width and signedness separately from the destination field.
The same applies to temporaries: a halfword result can be promoted to `int` for
arithmetic and narrowed only afterward. For example, `(s16)(last_frame - 1)`
preserves a different truncation point from making `last_frame` an `s16` early.
Check the native extension instructions and the callee's parameter type before
introducing masks or casts to fix a size difference.

For example, the Nawatobi console caller passes full-width cursor coordinates;
the console setter narrows them only when storing its byte fields. Declaring
those parameters as `u8` inserts extra masks in the caller. Verify both caller
and callee, plus existing users, when correcting the shared declaration.

Recover return contracts with the same care. A constant left in `r0` at return
does not by itself establish an `int` result: it may be the value of the final
store. Inspect whether callers consume that register and how the callee produces
it. Do not copy an invented `return 1` from an old draft, or change a function to
`void` merely because one caller ignores its result. Correct shared declarations
and verify every affected linked caller when the evidence supports a change.

Check array bounds across all consumers, including adjacent scenes that share a
workspace. A save-menu replay using two panels does not establish that a shared
table has only two entries; the load menu may use another index. Establish the
extent from native data and consumers before declaring the array. Matching bytes
alone do not prove that a reconstructed C access is within its declared object.

Packed fields also need their native extraction semantics. Reuse a verified
shared bitfield view when the instructions and consumers establish its layout;
an apparently equivalent shift/mask expression can emit a different extension
sequence. Check all users and the full linked module after changing a flags
union. Keep arithmetic operations justified by the instructions: the Smash Eggs
signed halfword angle step uses multiplication by -1 (`SMULBB`), whereas unary
negation produces a different instruction. See
[pair motion](../../src/overlay015/pair_motion.c) for the matching expression.

Use the recovered C++ virtual interface when native calls go through a vtable.
`BattleModel` and its virtual methods are declared in
[battle_scene.h](../../include/game/battle_scene.h); there is no separate
`battle_model.h`. Its `BattleModelAnimationData` rows are eight bytes: two
frame halfwords followed by a four-byte unknown field. Use that stride when
reading animation ranges in a probe. A C function-pointer approximation can have
the correct size while retaining different call/load ordering. When moving a
unit to C++, check the linkage of every shared C declaration. Use guarded
`extern "C"` declarations
for C APIs, keep C++ class declarations outside them, and verify exported names
and relocations. Do not hide mangled-symbol errors in the comparison script.
Headers without their own C++ linkage guard must first be included inside
`extern "C"`. If another header has already included them outside that block,
their include guard prevents a later wrapped include from fixing the linkage.

Use the actual recovered global type when exposing workspace fields. In the
shop panel callbacks, casting a raw byte-array global to a structure prefix
still produced extra loads; declaring the global with the shared `ShopSceneWork`
type reproduced the native accesses. Its named fields must coexist with the
existing byte-array view used by background setup. Check both field offsets and
the complete workspace size, then run the full matching build for all consumers.
This is evidence about that shared layout, not permission to change aliasing
rules or enumerate arbitrary type casts until one happens to match.

Account for integer promotion and the exact point where values are rounded.
A `u8` or `u16` operand promotes to `int`; an explicit `u32` cast before a shift
may be needed to reproduce a native logical shift. Signed division truncates toward
zero, which differs from an arithmetic right shift or Python's `//` for negative
values. Preserve the native order of division and Q12 scaling, and model signed
division explicitly in runtime oracles. See the verified examples in
[title model entry](../../src/overlay006/title_sequence_model.c) and
[trail drawing](../../src/overlay006/title_trail.c).

Python oracles must also reproduce the native width of intermediate arithmetic.
The [display affine helper](../../src/overlay005/display_bg.c) takes nine
arguments: engine, background, horizontal scale, vertical scale, rotation,
center X/Y and origin X/Y. Pseudocode that omits the final stack arguments is
not a complete prototype. For zero vertical scale it substitutes `0x100000`;
at zero rotation, cosine is 4096, and the native low-32-bit product wraps to
zero before the arithmetic right shift by eight. An independent oracle needs
`s32(inverse * 4096) >> 8`, where `s32` wraps to a signed 32-bit value, rather
than the unbounded equivalent `inverse * 16`. This explains the zero matrix
coefficient without changing already matching game code. Validate the native
lookup-table values and ordered hardware stores as well as the resulting
coefficient bytes; affine origin readbacks can change during rendering.

Similarly, derive task completion from the actual integer recurrence. A close
animation starting at Q12 scale 4096 with step `4096 / 6 == 682` needs seven
scaling updates to reach its clamped zero, plus its one delayed update. A probe
that expects six updates would reject the native behavior. Track the created
task through its terminal flag write and distinguish that from the later free.

Preserve reads across callbacks in their original order: a linked-list callback can
change `next`, and cleanup can change state. Do not cache those fields earlier
merely to simplify the source. Distinguish deleting and non-deleting destructor
entries when reconstructing virtual calls and reporting runtime coverage.

Overlay 5's `func_ov005_0206650c` requests deferred removal by setting mask
`0x0002` in the element's 16-bit flags at offset `+0x08`. It does not immediately
free the object. Model this write separately from later list cleanup; see
[element lifecycle](../../src/overlay005/element_lists.c). Title and credits tasks
share the 72-byte `MenuElement` view in
[the frontend header](../../src/overlay006/frontend_scene_internal.h). Reuse it instead
of copying private task definitions into each module or probe.

C comparisons yield `int`, while C++ comparisons yield `bool`; this MWCC setup
can emit different truncation masks for their results. The title sequence cleanup
matches in C with byte-sized flag input and separately scoped loop indices.
Use that type/lifetime evidence before attempting register-allocation changes;
matching only a function's size is insufficient.

For a same-size mismatch, classify the differing words before editing: changed
instructions, register operands, literal values or relocations require different
explanations. Check native load order, expression lifetime and aliasing first.
A helper that reads both inputs before writing an aliased object can preserve
native behavior that sequential field assignments do not express. Accept a
source change only when its data flow explains the difference; defer remaining
register-only mismatches instead of trying arbitrary declarations or casts.
For transformed model drawing, compute the X and Y inputs before assigning
either output field when that is the native order. `const` on an input pointer
alone does not establish that its storage cannot overlap a mutable output.

When several accesses share an embedded structure, recover that relationship
with a typed pointer and retain the native order of dependent loads. Its lifetime
can explain register use across helper calls without artificial register hints.
Narrow a local variable's lifetime to the native loop or branch when the data
flow supports it, and preserve the order of counter and pointer increments.
These changes recovered the title backdrop initializer without altering its
algorithm. A candidate with fewer instructions can instead reflect algebraic
folding, such as combining a negation and addition into one reverse subtraction;
do not mistake that for a relocation-only difference or a completed match.

An address-named data symbol may label an interior field of that same workspace,
not a separate global object. Check the native base load and field offset before
declaring another `extern`. In overlay 8, `0x0207832F` is workspace base
`0x02078290 + 0x9F`; the Game Over controllers read it through the workspace
base also used for selection fields. A separate global reference introduces an
extra literal/load and loses that relationship. Recover the shared field from
its consumers; a decompiler's separate label or an old `padding` name does not
establish independent storage or prove that the byte is unused.

Size request buffers from callee accesses, not just the apparent base type.
`ArchiveReadRequest` is 40 bytes, but `BattleArchive_ReadAsync` also writes the
halfword at offset 40, including on the raw-read path. The title loader uses a
44-byte extended raw request; a full `ArchiveCompressedRequest` is 64 bytes.
See [the shared layout](../../include/game/archive_io.h),
[the callee](../../src/game/archive_compressed.c), and
[the title loader](../../src/overlay006/title_animation_resources.cpp).
Preserve ownership flags and the native allocator/free pairing for converted
resource tables; a non-null pointer alone does not establish ownership.

The title sequence allocates 59,340 bytes but clears only its 57,288-byte prefix.
The 512 bytes at allocation offset 57,292 are a 32x32 4bpp trail stamp; older
initializer notes incorrectly called them palette data. The rasterizer establishes
their purpose. Its two 24,576-byte destination buffers are linear pixels before
tiled VRAM upload, and the second screen uses a sequence-space y-origin of 244.
Do not infer a 192-pixel screen separation from the display height. Refer to
[the sequence layout](../../src/overlay006/title_sequence_internal.h) and the trail
module when interpreting these captures; the remaining allocation tail is unknown.

The sequence's 25 backdrops use a 52-byte record with a 48-byte model prefix,
signed 16-bit horizontal velocity and a four-bit group. Their four groups have
4/5/8/8 elements. Construction, release and skip hooks are linked in
[title backdrop initialization](../../src/overlay006/title_backdrop_init.c); check
current metadata before assuming that the scrolling callback or shared animation
selector has also been reconstructed. Animation IDs alone do not identify the
artwork or establish looping semantics.

Credits use a different coordinate space from the title sequence: screen value
0 adds 224 pixels to Y, before Q12 scaling. The illustration is divided into
32x32 cells held in 1,024 records of 84 bytes. Cell centers add
`(5*column+2, 4*row+2)` pixels to the layout origin; the illustration center
adds `(80, 64)`.
The X and Y table views have a 16-byte record stride. Preserve that stride and
the actual typed workspace object; treating the workspace as a cast byte array
changed the emitted address calculations. See
[credits positions](../../src/overlay006/credits_positions.c),
[shared declarations](../../src/overlay006/credits_transition_internal.h), and
[motion setup](../../src/overlay006/credits_motion.c). Keep native integer operation
order and narrowing when modeling acceleration or easing; floating-point
interpolation is not an equivalent oracle.

Credits effects reuse the motion record's remaining parameter words for
different purposes. Keep shared names neutral until all consumers justify a
single meaning. Illustration loading uses a 32,768-byte texture staging buffer
and a 512-byte palette; the task schedules sixteen texture chunks and a palette
upload before clearing the workspace's `image_loading` field at `+0x8238`.
The current layouts and buffer declarations are in the shared headers above;
the state machine is in [credits image loading](../../src/overlay006/credits_image_loader.c).
Checking those queued jobs does not establish coverage of the native upload
callbacks themselves.

Small, explained inline-assembly fragments are authorized when a specific
instruction sequence cannot reasonably be reproduced in C. Keep the surrounding
logic readable and verify the entire function. Do not disguise raw instruction
dumps as C to inflate progress. The completed Scene VM's 64-byte height-conversion
fragment is a documented example in [its matching notes](../../docs/research/SCENE_VM_MATCHING.md).
All four VM dispatchers are already linked and byte-identical (53,700 bytes);
their callees still contain work. Check the [VM reference](../../docs/research/SCRIPT_VM_SEMANTICS.md)
before treating older dispatcher research as an open task.

## Runtime verification

Use the optional dependencies in `tools/runtime_requirements.txt`. The current
`py-desmume==0.0.9` bundles DeSmuME 0.9.12. A 0.9.13 savestate can expose plausible
CPU/RAM while rendering incorrectly or hanging on the next frame. Recreate
compatible states from battery saves; do not repeatedly advance a known-bad state.

The private `PiT_SaveStates/` directory contains story checkpoints from start to
finish. Enumerate what is present and select an appropriate save. Keep originals
unchanged and compare their hashes before and after a replay. Store new states,
screenshots, dumps and reports under ignored `build/runtime/` paths.
Use `--battery-save` with a selected `.sav` to cold boot through
`tools/runtime_drive.py`; `--state` loads an emulator snapshot instead. Old
messages mention `PiT/_SaveStates`, but the current checkout uses
`PiT_SaveStates/`. Check the filesystem rather than assuming either path exists.
Run emulator replays sequentially when they share a ROM path or battery-save
backup. Parallel instances can overwrite the same emulator save sidecar. A
different output report directory alone does not isolate that shared state.

Useful title-startup checkpoints, confirmed by cold booting the supplied saves:

| Save number | Checkpoint | Participant mask | Greeting stream ID |
|---|---|---|---|
| 1 | Peach's Castle, before the Refreshroom | 3 | 45 |
| 6 | Koopa Cruiser | 12 | 46 |
| 83 | Star Shrine, before the boss | 15 | 47 |

The local evidence is under `build/runtime/eur_title_startup/`. These cover the
three observed startup selections, not the whole story or empty-slot fallback.
Choose saves from observed branch inputs rather than repeatedly replaying the
same late-game checkpoint. Matching values at several offsets in a battery save
can be duplicated records; verify the active runtime value before treating any
file offset as a universal field location.

- `tools/runtime_drive.py`: deterministic, headless DS input; supports battery
  saves or compatible states, repeated `--action` arguments and screenshots.
- `tools/runtime_probe.py`: execution hooks, registers, memory watches, diffs
  and overlay identification. `--action` and `--frames` are alternatives.
  Use `--allow-state-advance` only with a compatible state.
- `tools/runtime_session.py`: interactive navigation and compatible-state
  creation when automatic input needs investigation.

Example using an already-created compatible state:

```powershell
python tools/runtime_drive.py --rom PiT_eur.nds --state build/runtime/before.dst --action down:60 --action wait:30 --action a:1 --save-state build/runtime/after.dst --screenshot build/runtime/after.png
```

For a reproducible ordinary title-to-load-menu cold boot using checkpoint 1:

```powershell
$titleSave = @(Get-ChildItem -LiteralPath PiT_SaveStates -Filter '1. *.sav')
if ($titleSave.Count -ne 1) { throw 'Expected exactly one checkpoint-1 save' }
python tools/runtime_drive.py --rom PiT_eur.nds --battery-save $titleSave[0].FullName --action wait:1500 --action start:30 --action wait:250 --save-state build/runtime/title_check/after.dst --screenshot build/runtime/title_check/after.png
```

`runtime_drive.py` adds one released-input frame after each action, so this
three-action replay advances 1,783 frames. The route reaches the load menu;
it does not establish that the saved field state has been entered. Use a
function-specific probe for independent object and graphics assertions, and
apply the original-save hash checks described above.

Use ordinary buttons and live RAM observations to navigate automatically.
Inspect screenshots and active overlays before interpreting a trace: a save can
start beside a save block, and A may open a menu instead of advancing the field.
Selected actions and party formations also change which button performs a jump.
Give each entry route a bounded deadline and verify that its dispatch hook ran.
Waiting on an idle field save does not necessarily execute a decoded command;
if it does not, inspect the state and choose an appropriate action or checkpoint.

Guard hooks with the expected overlay/function bytes because overlays reuse
addresses. For return hooks, track the caller and stack pointer so nested calls
do not produce false observations. Check arguments, object fields and results
against independently derived expectations. For graphics, compare the relevant
RAM buffers, VRAM, palettes, OAM and display registers as well as screenshots.
Log ROM/save/state hashes, inputs, entry/return counts and uncovered branches.
Confirm that the evidence's ROM hash matches the current packaged artifact.
Execution hooks fire before an instruction: account for ARM condition codes
before counting a conditional store as a write. For VRAM uploads, derive the
destination from the active bank mapping. Separate function coverage from branch
coverage; an uncalled destructor or allocation-failure path remains unexercised
even when every compiled byte matches.

GPU FIFOs are ordered command streams; reading the register afterward cannot
recover the sequence sent to it. At each guarded native store, check the actual
destination, width and source-register value against an independently built
command list. Verify workspace counters separately, including effects of nested
draw helpers. This establishes the submitted commands, while screenshots and
VRAM/OAM captures establish the observed rendering. State explicitly when a
texture or sprite callback itself remains outside the oracle.

When reusing a private probe, check its symbol names, structure offsets, hook
guards and helper prototypes against the current checkout. A source rename or
recovered common prefix can invalidate a previously successful probe. Track
nested monitored calls separately. Refresh memory after an external helper
returns before resuming the caller's expected writes; that refresh observes
the helper's effect but does not independently verify its implementation.
State which results the oracle actually derives. For packed graphics, a simple
per-pixel or per-nibble oracle is preferable to copying the native packed-word
algorithm and potentially reproducing the same interpretation error.

Task creation can change a source object's list links when the new task is
inserted after it. Resource attachment also writes the owner's release callback
and resource pointer. Account for these helper writes using the recovered
[element list](../../src/overlay005/element_lists.c) and
[resource owner](../../src/overlay005/resource_owner.c) behavior before asserting that
the entire source or task record stayed unchanged.

An oracle failure may be a wrong expectation. Inspect the native helper before
changing matching game code: for example, `func_02036988` always sets blend-mode
bit `0x40` and writes BLDCNT/BLDALPHA together as a 32-bit value. Read back the
actual register width. Validate raw archive payloads against the extracted
archive; compressed-request completion and size checks alone do not establish
that the decoded bytes were independently verified.

Derive asynchronous completion from the actual queue or loader field. For
example, `GameAudioLoader.active` is at `+0x20`; `+0x1C` is its next pointer.
Check [the audio layout](../../include/game/audio.h) and the native access before using
either in a probe. A nonzero pointer can look like a plausible busy flag.
After correcting an oracle, rerun the full affected replay and retain the
successful evidence separately; do not count the failed attempt as validation.

Keep probe work bounded. Check and release completed per-node snapshots during
long list traversals instead of rechecking all earlier nodes after every call.
Reuse the route and guards without automatically retaining every previous
per-frame oracle. The shop text-index probe retains show/hide, queued-text and
icon checks, but leaves the broader buying-price checks in their earlier probe;
this substantially reduces replay time without dropping checks for the new
index helpers. Preserve those earlier reports and identify checks that were
not repeated. Do not rebuild a ROM at the path an active emulator is reading;
a separate native-relink output does not replace that input file.
An emulator frame boundary can fall inside a monitored function. At the end of
a replay, stop accepting new outermost calls, keep observing pending calls and
their nested helpers, and allow a bounded number of neutral frames for returns.
Record these drain frames separately. Continuing to admit new calls can prevent
the capture from ever becoming idle. Diagnose pending calls from the PC, stack
and native instructions before treating a probe-end assertion as a game hang;
do not discard unfinished calls to make validation pass.

When using prior screenshots as a baseline, compare the relevant buffers and
palettes and state explicitly whether a new image was visually inspected or
only its hashes matched. Label language overrides and other artificial branch
fixtures separately from ordinary save replays.

Optional hardware can affect menu construction. With no Slot-2 accessory, the
title's rumble options can be hidden; repeated button presses alone do not
establish that the menu was reached. A controlled constructor-layout fixture
can exercise its UI without enabling the hardware availability flag. Record
the changed bytes, constructor timing and visible result, and distinguish UI
coverage from physical rumble support. Editing the layout after construction
does not exercise the constructor's hardware-present branch. Likewise, visiting
a phase does not establish that every condition or transition within it ran.
The save object's byte `+0x514`, bit 6 is the rumble preference; see
[rumble control](../../src/game/rumble_control.cpp).
This is an object-relative RAM field, not a universal battery-save file offset.

The debug menu can teleport without fully initializing the destination state.
Controlled RAM edits or temporary decoded-command substitutions are useful
probes, but record exactly what changed and when it was restored. They do not
demonstrate normal gameplay accessibility. Prefer read-only observation after
the controlled setup. Never infer complete branch coverage from a matching ROM.

### Shops

The private `build/analysis/probe_shop_buy_panel.py` checks the buying panel's
prices, discounts, affordability, quantities, new-item flags and show/hide behavior.
It uses compatible story-86 or story-65 HUD states and the corresponding original
battery saves. Its `coin86`, `equipment65` and `beans65` reports are under
`build/runtime/eur_shop_buy_panel/`; the selected shop IDs are 0, 2 and 14.

Entry substitutes one 72-byte decoded command at guarded EUR ARM9 field dispatch
`0x020823F8`, using opcode `0x121` with the shop ID, fade 1 and return-screen flag 1.
All original command bytes are restored at guarded request helper `0x0206ACF8`.
Ordinary buttons then select items, cancel quantity selection and leave the shop.
This establishes controlled scene entry, not interaction with the shopkeeper
through a normal story script. Older private shop probes used return-screen flag
0 and could finish with black screens; retain flag 1 for the tested visible return.

The oracle checks complete 936-byte buying panels and 1,380-byte live saves,
original-ROM item records, price arithmetic and ordered helper arguments with
SP-matched nested returns. Panel setup is observed helper output; the caller's
enable flag, row/list/category calls and hide-selection writes are derived.
Capture RAM/VRAM, palettes, OAM and both screens. Verify scene/panel pointer cleanup
at the destructor return, before the field overlay reloads and reuses those
addresses; a later nonzero value at the old shop global is not evidence of a leak.

The separate `coin86_limits` and `beans65_limits` fixtures temporarily set funds
to zero and inventory to the native limit at two guarded affordability calls.
Each edit is restored at that call's SP-matched return. They exercise rejection
branches, not real purchases or persistent changes to saves. The artifact verifier
checks source-save hashes and compares their captures with the ordinary routes.
Read each report's actual coverage before extending it; item setup, rendering
internals and unexercised controller branches remain outside this oracle.

`build/analysis/probe_shop_panel_lifecycle.py` extends those three ordinary-button
routes with constructor and destructor checks. It snapshots the full buying
allocation (936 bytes) and inventory allocation (848 bytes), including nested
base calls. The base prefix is 828 bytes. Buying initialization writes two
parallel two-byte arrays at offsets 929 and 931; inventory initialization writes
only the low two bytes of the word at offset 828. Preserve the remaining bytes.
Expected vtables, initialized fields and position-helper writes are derived;
parent oracles inherit child expectations, not observed constructor output.
Validate the object before the heap free and never read it after release.
The routes exercise base initialization/destruction and both derived initializers
and deleting entries, but not the four other destructor wrappers. Reports in
`build/runtime/eur_shop_panel_lifecycle/` and the private
`verify_shop_lifecycle_artifacts.py` compare captures with the buying-panel batch.

`build/analysis/probe_shop_text_tasks.py` checks queued item/stock text updates
and item-icon lookup. Tasks use the shared 72-byte pool; full-width arguments
at offset 40 are narrowed only when the callback calls the renderer. The checks
derive all eleven text-call arguments, icon indices from original item records,
128-byte icon copies and the deferred-removal flag at task offset 8. Allocation
and text-renderer internals remain observed helper outputs. Icon lookup leaves
the renderer and live save unchanged. Scrolling exercises the item callbacks;
a six-item equipment list fits without scrolling and does not queue those tasks.

The `coin86_purchase` fixture changes the live Mushroom inventory byte to one
at byte-guarded EUR ARM9 field request `0x0206ACF8`, before scene construction:
`write8(read32(0x02059FE8) + 0x48E, 1)` in ARM9 main RAM. Ordinary buttons
buy one Mushroom; independent checks verify the seven-coin cost, quantity change
from one to two and stock-text callback. It restores the original inventory byte
and little-endian currency word at `read32(0x02059FE8) + 0x488` at guarded shop
deletion, before field return. The equipment
and bean routes have no purchase fixture. Reports and captures are under
`build/runtime/eur_shop_text_tasks/`; source battery saves retain their hashes.

The focused `probe_shop_text_indices.py` reuses those inputs and fixtures. Name
indices use record halfword `+2`, adding one when quantity is not one; description
indices use halfword `+4` or `+6` according to the live category byte in
`data_ov009_0207e28c`. Both native functions retain a full-width accumulator and
narrow to `u16` at return. A `u16` local can instead move narrowing into the
conditional increment. Preserve the native null-record behavior too: the name
helper still adds the plural offset to zero. The three routes exercise singular,
plural and both description selectors; invalid records and index wraparound
remain unexercised. Reports under `build/runtime/eur_shop_text_indices/` check
unchanged renderer/save/selector records and compare every capture with the
preceding text-task batch. The broader buying-price API checks are not repeated.

`probe_shop_menu.py` adds cursor creation/drawing, option model/text drawing and
menu-task removal checks to those same routes. The separate
`probe_shop_menu_navigation.py coin86_menu_purchase` pass uses ordinary down/up
buttons to visit all three choices before continuing the purchase route.
Reports are under `build/runtime/eur_shop_menu/`. Menu rows use every second
point in the shared list; preserve the signed selection byte and narrowing to
an unsigned row argument. Model offsets are signed halfwords, sprite coordinates
are Q12 words, and option drawing requires a list scale greater than 409.
The probe derives row geometry, draw priorities, zero-angle matrices, draw-list
appends and full 72-byte task records; model checks cover a 128-byte prefix and
sprite checks cover a 64-byte prefix, not an inferred full allocation.
Model initialization and virtual animation internals remain observed helper
outputs. `ShopMenuNavigation_Clear` marks groups 3 and 5 for deferred removal;
it does not free them during that call. Check each live task's flag change and
unchanged links before the later cleanup. The four final routes cover all five
new entry points, all three selected rows and both sides of the draw threshold.
Negative coordinate and invalid-row branches remain unexercised. The artifact
verifier checks all captures and compares identical routes and unchanged starting
captures with the preceding text-index reports; later navigation frames are new
evidence, not an identical-input baseline.

`shop_help_text.c` owns the adjacent help-text draw and background initialization
functions. MWCC C with a named `GameTextToken` result reproduces the native
return-buffer slot; discarding the result or using the C++ draft changes three
stack offsets. The draw loop tests termination at its head, and the tilemap loop
increments its pointer separately from the store. Preserve the explicit stack
loads for the fill value rather than replacing them with a constant argument.

`probe_shop_help_text.py` repeats all four menu routes while replacing the prior
per-frame menu oracle with focused help-text checks. Derive character and map
VRAM addresses from main DISPCNT/BG0CNT, message pointers from the current language
table and original item records, all fourteen text-init arguments and the full
48-byte initialized text state, preserving reserved bits and padding. It checks
24,576-byte character clears, 1,024 sequential tilemap halfwords, 5,120-byte pixel
clears and 4,096-byte copies at character base + 20,480. Text-token rasterization
remains observed helper output; the copy and final VRAM buffers are independently
compared. The initial message-only oracle missed kind-1 item descriptions and
was corrected from the native lookup before rerunning the full affected route.
The final reports in `build/runtime/eur_shop_help_text/` cover kind-1 descriptions
and kind-2 menu messages; kind-0 names, empty strings, alternate languages and
invalid entries remain unexercised by the new functions. Every capture matches
the preceding menu-batch route, and all source saves retain their hashes.

`probe_shop_graphics.py` and `shop_graphics_oracles.py` extend the text-index
routes with the party-label callback, bitmap values and placeholder dashes,
packed OBJ placeholders and 4bpp-to-8bpp glyph expansion. The four reports in
`build/runtime/eur_shop_graphics/` exercise every new entry point. They derive
pixels/nibbles independently, including transparent pixels, palette offsets,
linear-to-tiled copying, sprite coordinates and draw-list appends. All captures
match the preceding help-text batch. Earlier help-token checks are retained in
their own reports, not repeated by this graphics probe.

Derive buffer sizes from `shop_resources.cpp`: bitmap pixels allocate 53,248
bytes, of which 49,152 are initialized and transferred; packed and expanded
glyph allocations are 2,816 and 5,632 bytes; scratch and strip buffers are each
5,120 bytes. The OBJ placeholder clears 512 scratch bytes but transfers only
256. Help text uses a separate 6,144-byte allocation. Check each full allocation
and preserve unused tails rather than assuming that a clear or upload covers it.
The bitmap value routine mixes unsigned value division with signed divisor
reduction. Glyph expansion reads the source again after the low-nibble store;
`const` does not rule out aliasing. Preserve these native operations.

This replay exercises two- and three-digit bitmap values, visible and hidden
label callbacks, even-X OBJ placeholders and 32-/128-byte glyph expansions.
Zero/one-digit values, odd-X placeholders, overlapping glyph buffers, zero-length
expansion and invalid coordinates remain unexercised. Visibility and mirror
flags are counted separately; those counts alone do not establish every combined
branch. Controlled shop entry and purchase fixtures remain distinct from normal
NPC navigation. Neighboring bitmap and number helpers still contain native gaps;
do not count an entire private candidate file because one function matches.

The adjacent scale-task creator/updater are in `shop_list_scale.cpp`, with
`shop_list_point_model.cpp` on the far side of the remaining model-initializer
gap. Scale tasks store duration/current/target/step/delay at offsets 36/40/44/48/52
in the shared 72-byte task pool. After a nonzero delay expires they request sound
234; scaling starts on the following update. The last scaling update writes the
exact target before calling the clamping setter and marking the task for deferred
removal. The point-model callback uses the list's compressed X coordinate and
workspace-relative Y, rounds each Q12 value toward zero, then stores halfwords.
The last point's draw priority decreases by two while entering or in a nonzero
list phase. Preserve the native subtraction rather than replacing it with a
constant assignment solely because the preceding offset starts at zero.

`probe_shop_list_effects.py` with `shop_list_effects_oracles.py` verifies these
fields and tracks every created scale task through its complete update sequence.
It derives clamped scale, model coordinates, priorities and draw-list appends,
checking 72-byte tasks, 656-byte lists, 1,380-byte saves and 128-byte model prefixes.
Allocation and sound-queue internals remain observed helper outputs; their call
arguments and the caller's writes are checked independently. Reports and capture
comparisons are in `build/runtime/eur_shop_list_effects/`. Use the recorded branch
counts when extending the routes; ordinary positive scaling does not establish
negative-step, division-by-zero or clamp-boundary coverage.

The currency callback and display-initialization wrapper are adjacent in
`shop_display_control.cpp`. Currency uses 72-byte tasks, with animation fields
at offsets 32 through 60. Only part zero updates the value; the other part still
submits its sprite. Coins are a signed word at live-save offset 1160, beans an
unsigned halfword at 1164, capped at 999,999 and 999 respectively. The animation
uses Q8 values and durations 1/8/16/32. Its six-digit renderer accepts a full
32-bit value; the older `u16` prototype incorrectly narrowed currency arguments.
The corrected prototype preserves the bytes of existing linked callers.

`shop_panel_zoom.cpp` owns open/close updates; `shop_panel_zoom_start.cpp` owns
the separate close-task creator. `ShopSceneWork` names the busy/visible bytes
at offsets `0xA1/0xA2` and signed Q12 scale at `0xA4`, preserving the background
code's byte view. The 72-byte tasks hold step/delay at offsets 40/44. Updates
derive panel coordinates around Y=96, submit the full nine-argument affine
transform, and change BG2 visibility and deferred-removal flags at completion.

`probe_shop_display_zoom.py`, `shop_display_zoom_oracles.py` and
`verify_shop_display_zoom_artifacts.py` cover all five functions across four
routes in `build/runtime/eur_shop_display_zoom/`. They derive full task records,
64-byte sprite prefixes, live-save/work/panel fields, draw-list appends and 217
numeric renders using per-nibble font decoding and independent tiled copies.
Checks preserve the full 2,816-byte packed font, 5,120-byte scratch/strip buffers
and 65,536-byte main OBJ VRAM. Both created close tasks complete after eight
updates. Nested affine checks derive the matrices and all 112 ordered hardware
stores; the corrected zero-scale overflow expectation was rerun successfully.
All captures match the preceding list-effects routes and all source saves remain
unchanged. Initializer helper outputs, allocation and sound internals are observed
with checked call arguments. The routes cover coins/beans, part zero/one, optional
party initialization, both zoom directions, delays and final clamps. Currency
changes cover a seven-coin decrease with duration one; increases, cap enforcement,
longer animations and special-shop initialization remain unexercised.

`shop_quantity.cpp` owns eight adjacent helpers for the quantity popup: total
price and decimal digits, selected-icon copying, item-name rendering, three
sprite callbacks and quantity arrows. The 72-byte task view names part/vertical offset at +40,
cached total/base tile at +44, previous quantity at +48 and item at +52. Workspace
unit price is a signed word at +0xA8 and quantity a signed halfword at +0xAC.
The digit callback updates the name only when changing to or from quantity one,
and suppresses a zero tens digit. Visibility and scale thresholds control drawing;
closed panels mark their remaining tasks for deferred removal. Sprite roles that
have not been identified from their art retain generic behavioral names.

`probe_shop_quantity.py` and `shop_quantity_oracles.py` repeat the four preceding
routes with focused quantity checks. They derive full task records, sprite
prefixes, matrix halfwords, draw-list appends, total/digit caches and removal
flags. Selected icons copy 128 bytes from the current row into main OBJ VRAM;
item names mirror 1,152 bytes to sub OBJ unless the special-shop flag is set.
These copies preserve the complete 65,536-byte buffers. Name-renderer pixels are
observed helper output with independently checked arguments and subsequent copy;
numeric rendering remains independently checked per nibble and tiled transfer.
All 680 artifacts from identical routes match the display/zoom batch.

The additional `probe_shop_quantity_selection.py` uses ordinary Up/Down holds
before confirming one purchase. Its 2,073-frame report under
`build/runtime/eur_shop_quantity_selection/` records 50 distinct quantities from
1 through 66, including visible tens digits and both singular/plural transitions.
Held-input acceleration skips some intermediate values; this is not coverage of
every integer in that interval. The six captures before the added inputs match
the ordinary purchase baseline. Later captures have different input/timing and
are separate evidence. All five runs total 8,601 frames, preserve all original
saves and return to the field without pending calls. Special-shop mirroring bypass,
invalid panel indices, negative quantities/prices and out-of-range scales remain
unexercised. Earlier currency/zoom checks remain in their own reports.


The quantity-arrow callback uses a model resource at task offset +28 and a
selling-mode alias at +44. The upper arrow independently checks stock capacity
and funds when buying, or available quantity when selling; the lower arrow is
hidden at quantity one. Hidden arrows move to (-128, -128) but still enter the
draw list. The shared panel virtual interface names adjustment at slot +12 and
quantity at +16. Preserve the caller's explicit halfword narrowing of quantity,
load order around callbacks, and the scale read before model scale stores.

`shop_price_adjustment.cpp` owns mode setting and three adjacent draw callbacks.
The task pointer is at workspace +0x8BC; modes 0 through 4 map to task phases
0, 100, 200, 300 and 400. The 72-byte task shares parent/part/cache fields with
its children. Value and marker widths derive from the virtual adjustment times
selected quantity; coordinates use signed Q12 division toward zero. The symbol
only requires a nonempty panel, while value/marker also require an enabled panel.
Both model coordinates are computed before either is stored, matching the
native cached loads. The styled numeric helper takes five arguments, including
the stack argument; its rasterized pixels remain observed helper output.

`probe_shop_adjustment.py` and `shop_adjustment_oracles.py` cover these five new
functions on the four preceding routes. Supplemental capacity and funds probes
use separate directories under `build/runtime/eur_shop_adjustment_capacity/`
and `eur_shop_adjustment_funds/`. Ordinary Up/Down holds reach 98 Mushrooms with
one already owned (99-item capacity), or 38 Ultra Mushrooms at 32 coins each
with 1,235 coins (the next quantity would cost 1,248). The capacity route returns
to one and buys one using the existing restored fixture. The funds route uses
unchanged inventory/currency and cancels without a purchase. The six runs total
10,282 frames and 33,740 returns through the new functions, with 95 capacity-only
and 176 funds-only blocked upper-arrow updates. They independently derive full
tasks/parents/panels/saves, model/sprite writes and 26,170 draw-list appends;
416 ordinary numeric renders are checked independently, while 187 styled-number
outputs are recorded with checked arguments and unchanged font data.

The funds route's initial final-coverage assertion incorrectly equated a list
longer than seven rows with scrolling callbacks. Two Down presses remain within
the initial visible rows; native selection code queues replacement text only
when scrolling across an edge. Correcting that route-specific assertion and
rerunning the full replay passed, without changing game code. All 108 screenshots,
972 graphics dumps and 104 source saves validate. Baseline comparisons cover
750 artifact pairs with identical starting state/input/timing; later supplementary
captures are separate evidence. The capacity and affordability popups were
visually inspected. Selling, bean-funded quantity arrows, zero-count panels,
mode zero, missing mode tasks and invalid mode inputs remain unexercised.
The nearby percentage renderer is still a native gap; do not infer its matching
status from these callbacks or their runtime helper observations.


`shop_panel_slide.cpp` owns the 20-update vertical transition used between
buying and selling scenes. Its 72-byte task stores remaining updates/current/
target/velocity/acceleration at +36/+40/+44/+48/+52. `ShopSceneWork` names
`panel_offset_y` at +0x8C8, consumed by the panel Y query. Entry starts at
-112 pixels and targets zero; exit starts at zero and targets -112 pixels,
snapping there when it reaches -80. The native update reuses the loaded target
when clamping the zero-target case. Preserving that load and the creator's
branch layout resolves the old draft's four-byte/register differences.

The earlier B-to-close routes do not execute these functions. At the category
menu, Up from Items wraps to Sell; A enters selling, B returns to buying, and
B closes the shop. For the purchase route this sequence replaces the final
four actions, beginning at frame 1,317; replacing only the last two acts after
the shop has already closed. Regular equipment/bean routes replace the last two
actions at frame 1,057. No extra RAM fixture is needed for this transition.

`probe_shop_panel_slide.py`, `shop_panel_slide_oracles.py` and their artifact
verifier cover those three routes. Twelve allocated tasks finish 240 updates,
with both directions and clamps. Expected positions and velocities use the
arithmetic-series formula from the initial state, independently of the native
recurrence; complete 72-byte tasks, 2,492-byte workspace and 1,380-byte live saves
are checked at each boundary. Allocation internals remain observed helper output.
Removal is deferred, and all three scene instances per route are checked before
overlay reuse. Twenty updates need not occupy exactly twenty emulator frames:
one bean-shop entry had an extra frame before completing its update sequence.
The verifier checks all twenty ordered updates and their recorded completion.

The three passing replays total 5,981 frames and 605 checked returns, including
252 through the new functions. All 58 screenshots, 522 graphics dumps and 104
source saves validate; 340 unchanged starting artifact pairs match the previous
batch. Later Sell transitions have different inputs and are separate evidence.
The moving panel and selling scene were visually inspected. The original numeric
oracle verifies 150 renders, while quantity/adjustment callback oracles remain
in the preceding batch's reports. Initial no-coverage routes are retained as
failed discovery attempts; they are not included in these passing totals.
Reports: `build/runtime/eur_shop_panel_slide/`.


`shop_subscreen_text.c` initializes sub-engine BG0 and draws the selling message.
The native 24-byte local initializer is a 4-by-3 halfword table, selected by
workspace variant/mode at +0x97/+0x98. An explicit manual copy produced different
instructions; a local aggregate initializer reproduces the native halfword copy,
and the two-dimensional declaration reproduces the separate row/column address
calculation. The module owns both its 420-byte code range and the 24-byte data
range at `0x0207E31C`; the latter does not count toward matching code coverage.
The compiled table is local `@312`. Metadata must describe the actual local
symbol, as with the existing overlay-2 initializer; the first full build matched
all module bytes but correctly rejected the old global table name. Reconfiguring
with the local symbol made the complete module/symbol checks pass. Verify the
emitted symbol again if future includes change the compiler's anonymous labels.

The message buffer at workspace +0x70 allocates 14,336 bytes, while this function
clears and copies 10,240. It clears 24,576 character bytes, writes 1,024 sequential
tilemap halfwords and enables BG0 before drawing. The VRAM copy begins 6,144 bytes
into the character region. The shared native VRAM getters read main DISPCNT bank
offsets even when selecting the sub engine; preserve their actual behavior.
Both clear values use native stack loads, as in the earlier help-text functions.

`probe_shop_subscreen_text.py` and `shop_subscreen_text_oracles.py` repeat the
three Sell/return routes. They check the native/local 24-byte tables, localized
message pointer, all 14 text-init arguments and the complete 48-byte initialized
text state, clears, tilemap, BG0 flag and VRAM transfer. Checks preserve the full
14,336-byte allocation, 131,072-byte sub BG VRAM, workspace, renderer and live
save. The 316 text tokens' pixels/state are observed helper output, followed by
an independently checked copy; the allocation tail remains protected.

The three runs total 5,981 frames and 356 checked returns, including three
initializer calls. They cover variant/mode pairs (0,0), (0,1), (3,2), English
language index 1 and message IDs 40/41/46. Other variants, languages, empty
messages and already-enabled BG0 remain unexercised. All 58 screenshots, 522
graphics dumps and 104 source saves validate; all 580 artifact pairs match the
preceding slide routes. The bean-shop selling message was visually inspected.
The ordinary numeric oracle checks 150 renders; slide and quantity/adjustment
oracles are retained in their earlier reports rather than repeated here.
Reports: `build/runtime/eur_shop_subscreen_text/`.

### Save menus

The private `build/runtime/eur_save_state_transfer/save55.dst` is an initialized
save menu derived from checkpoint 55. The similarly named `load55.dst` was
recorded after choosing Start and entering the field; its name does not establish
an initialized load menu. Use a cold boot and the recorded input schedule when
you need the load menu's construction or entry callbacks.
`build/analysis/probe_save_write_effects.py` records ordinary confirmation input
from it and verifies the brightness and background-zoom callbacks. Its report
is `build/runtime/eur_save_write_effects/evidence_save55.json`. The animation
starts while saving is in progress, before the final "Saved!" message; keep
that timing distinct from successful completion of the storage operation.
The probe derives full task records, blend-register writes, timers and helper
arguments, while affine-display and storage internals remain outside its oracle.
Use the source-save hash checks for every replay, including runs that write to
the emulator's battery-save copy.

`build/analysis/probe_save_write_setup.py` extends this route to effect-task
creation, rollback setup and menu exit. Its reports under
`build/runtime/eur_save_write_setup/` separate ordinary Save & Continue
(`evidence_continue55.json`) and Save & Quit (`evidence_quit55.json`) from the
controlled error-status run (`evidence_rollback_status55.json`). These reports
are runtime evidence, not proof that the corresponding source batch has been
committed; check the current manifest and Git state separately.

The error fixture uses EUR ARM9 main RAM with overlay 8 loaded. At guarded
controller entry `0x0206B598`, `r0` is the task pointer. It requires the 32-bit
phase at `r0 + 0x20` to be 201, timer at `r0 + 0x24` to be zero and completed-write
result at `r0 + 0x3C` to be 1, then writes `02 00 00 00` to that result once.
The native handler displays the failure message and starts rollback. This tests
the response to an injected status after a completed write; it does not simulate
a storage I/O failure. The observed heap address is not a universal patch.

The setup oracle derives writes to the 384-byte workspace prefix, created
72-byte tasks and their list links, plus blend and background registers. Initial
constructor records and allocation addresses are observed helper outputs. Exit
checks cover the 72-byte exit task and 48-byte scene task: `quit_to_title` is a
32-bit field, and the native exit uses positive brightness for quit and negative
brightness for continue. Text transitions, movement/fade callback internals and
physical audio remain outside this oracle. Consult the source layout and probe
before extending it; the workspace prefix does not describe the full allocation.

For normal save-menu entry and selection, `build/analysis/probe_save_menu_control.py`
leaves that initialized menu with B and re-enters by jumping at the save block.
Its `confirm55` and `navigation55` reports under `build/runtime/eur_save_menu_control/`
cover entry, both panel texts, directional wrapping on fresh presses, clamping
while holding a direction, cancellation and both save choices. They check complete
task records and a 416-byte workspace prefix. Text-stream results and workspace
changes inside model/dialog/scroll helpers are explicitly observed outputs;
do not describe those helper internals as independently verified by this probe.
The tested entry uses occupied slot zero; the empty-slot fallback remains untested.

`build/analysis/probe_save_confirmation.py` checks the adjoining confirmation and
write controller. Its seven reports under `build/runtime/eur_save_confirmation/`
separate normal continue/quit/Yes/No/B input from three error-response fixtures:
completed-write results 1 to 2 or 3, and a successful storage-probe return changed
to zero. They check complete task, workspace, live-save and lock/busy records;
the probe result and selected text/effect workspace changes are observed helper
outputs. The result-2 path invokes the selection callback synchronously, so model
its immediate state and workspace changes before checking the parent's return.
The probe-failure response reaches phase 1001 and remains on its error message.
These fixtures verify the controller's responses, not physical storage failures.

The private `build/analysis/probe_save_load_motion.py` checks save/load cursor
acceleration and updates, load-panel setup and all three panel-text widths.
Its reports are under `build/runtime/eur_save_load_motion/`. The save run starts
from the compatible menu snapshot; the load run cold boots checkpoint 55 and
navigates both file panels and the Start/Copy/Delete submenu before entering the
field. It independently derives signed division, complete 72-byte motion tasks,
the 416-byte workspace, input locks, created-task fields/list writes and BG
priorities. Constructor records and text-stream results are observed outputs.
The motion structure's checked 68-byte size describes only its prefix; the pool
allocates 72-byte tasks. Do not infer a two-entry text-width array from the save
menu alone: the load entry calls panel 2, and the shared array has three entries.

For load-menu file selection and exit, use the private
`build/analysis/probe_load_menu_control.py` and its `start55`/`cancel55` reports
under `build/runtime/eur_load_menu_control/`. Ordinary input covers both fade
signs and scene destinations, signed file-index wrapping, opposing directions,
and A/X taking priority over B/Y in this selector. Other menus use different
input priorities; recover each controller's order. The oracle derives complete
task/workspace/scene records, callback resets and both master-brightness registers;
dialog workspace effects are observed outputs with the caller's mode write
checked separately. A pre-Start title-animation frame varied in sub-screen OAM;
record that difference rather than claiming all cold-boot screenshots are equal.

### Game Over

The private `build/analysis/probe_game_over_entry.py` records a controlled entry
from checkpoint 86 using
`build/runtime/states/eur_story_086_hud_verified.dst`. At guarded EUR ARM9 field
dispatch `0x020823F8`, `r0` is the VM pointer and `r2` points to a 72-byte decoded
command. The fixture saves that record, substitutes opcode `0x123` with fade
argument 1, then restores all 72 bytes at native request helper `0x0206A9A4`,
before the transition. It changes decoded RAM once, not the ROM or stored script.
Validate the loaded code bytes and both pointer ranges before any substitution;
do not reuse an observed heap or command-buffer address as a fixed patch.

The historical report `build/runtime/eur_game_over_entry/evidence_086.json`
records scene initialization after 481 frames. It establishes a controlled
entry route, not a natural battle defeat or coverage of selection and exit.
Treat the old script as a route reference: its return hooks predate stack-pointer
matching and its name-prefix discovery can select newly linked functions without
an appropriate oracle. Before extending it, explicitly select monitored functions,
guard the request helper as well as the dispatcher, track nested returns by stack
pointer and derive expectations for each newly checked task/workspace field.

For the reconstructed entry, selection and exit controllers, use the newer
`build/analysis/probe_game_over_control.py`. Its `saved86` and `castle86` reports
under `build/runtime/eur_game_over_control/` cover ordinary menu input after the
controlled entry, returning to Shroob Castle and Peach's Castle respectively.
The `no_choices86` run additionally clears the byte at workspace `0x02078290 +
0x9F` and live-save `read32(0x02059FE8) + 0x516` once at guarded entry phase 0.
It verifies the no-choice dialog and X-triggered title return, not a naturally
empty save. The probe derives task/selection/flag/callback/fade/scene changes;
text/model/scroll workspace effects and the unpacked save payload are observed
helper outputs. Its return hooks track stack pointers. Check the report and
artifact verifier for actual branch coverage before extending the route.

`build/analysis/probe_game_over_resources.py` extends those runs to resource
initialization, checking the full `0x2808`-byte workspace, the text-pixel clear,
panel setup and ordered affine stores. Its `empty_slot86` fixture clears only
the active slot's occupancy bit in the RAM header at `0x0205E334`, once at
guarded resource entry `0x02070858`. This covers empty-slot setup and the
single-choice response; it does not delete a supplied save. Initial common
resources, stored summaries and image/text helper results are observed outputs.
Reports and the artifact verifier are under the corresponding private paths.

Do not assume affine origin readbacks stay constant across rendering. The
resource probe observed BG2Y advance without a CPU write during text setup;
DeSmuME's [renderer updates affine coordinates per scanline](https://github.com/TASEmulators/desmume/blob/master/desmume/src/GPU.cpp).
For this initializer, check the four ordered stores per background at guarded
`G2x_SetBGyAffine_` instructions, including destination, width and source-register
value. Check the coefficients separately. Record the submitted origins rather
than refreshing an expected origin from a later readback and calling it derived.

### Smash Eggs

For battle attack research, the compatible private snapshot
`build/runtime/eur_attack_helpers/ov17_bros_menu83.dst` provides a checkpoint-83
Bros. item menu. Inspect the visible item list and trace the selected command;
the filename does not prove which attack is currently selected. The native
loader mapping recorded in `build/analysis/probe_battle_attack_loader.py` maps
command 8 to overlay 15 and callback `0x020C5B4C`. This identifies a dispatch
route. The subsequent `build/analysis/probe_ov15_actors.py` replay confirmed
the visible Smash Eggs selection, actor command 8 and that callback together;
its successful report is `build/runtime/eur_overlay15/evidence_final83.json`.
At callback entry, check the actor's command halfword at `+0x7C` and callback
pointer at `+0x78`. The menu field at battle-context `+0x11A` can still contain
2 at this point; it does not identify the active attack. During battle,
`gBattleContext` is the pointer stored in ARM9
main RAM at `0x020C0718`; dereference it before reading object-relative fields.
Verify snapshot provenance and byte guards before reusing this route, and
distinguish a controlled encounter from normal story navigation.

The follow-up `build/analysis/probe_ov15_pair.py` adds targeting, RNG, retreat,
visibility and badge-table checks. Its successful reports are
`build/runtime/eur_overlay15/evidence_pair83.json` and `evidence_badge83.json`
in the same directory. Enemy eligibility is observed helper output; candidate
order, RNG state and target choice are independently derived. Both observed
selections had one eligible enemy, so they do not cover empty or multi-enemy
selection. The badge run overrides one predicate return register at caller
`0x020C5D30`; it does not demonstrate ordinary badge-equipping coverage.

The compatible `build/runtime/eur_overlay15/item_select83.dst` starts with
Smash Eggs selected. Read the probe's input schedule before reuse: it adds
timed A/B and X/Y pulses during the native attack update. Four `down:8` /
`wait:30` pairs from the preceding item menu take 160 frames, including the
released-input frame after each action. A successful attack replay returning
to the command wheel does not establish that the item-reward branches ran.
The public pair-state layout is a prefix; if it grows during reconstruction,
update the probe's record size with it. The entry/launch extension in
`build/analysis/probe_ov15_entry.py` checks 36-byte pairs and the complete
584-byte entry context. Its reward fixture changes the initialized pair's
signed launch counter at `+0x17` once, after an enemy hit, then lets the native
end transition run; the counter is distinct from the phase bits at `+0x16`.
The final reward fixture also overrides one random-helper return to select the
reward branch. Consult `build/runtime/eur_overlay15/evidence_reward_final83.json`
for both interventions and the visible reward capture; do not describe that
run as ordinary reward probability or completion of six natural launches.
The ordinary entry replay is recorded separately in `evidence_entry83.json`.
The older actor/pair probes retain their original 28-byte prefix checks.
Use the current shared
[attack layout](../../include/game/overlay015_attack.h) and native allocation/caller
evidence before interpreting fields beyond that prefix.

### Credits

For credits, the private `build/analysis/probe_credits_transition.py` established
a route from checkpoint 86 and the compatible state
`build/runtime/states/eur_story_086_hud_verified.dst`. It substitutes one 72-byte
decoded field command with opcode `0x122`, fade argument 1, and restores every
original byte at the credits request helper before the scene transition.
Inspect its guards and the recorded fixture before reuse. Successful reports
are under `build/runtime/eur_credits_transition/`; the final illustration uses
the fade transition, so an early capture alone does not cover it. This route
verifies controlled credits entry, not ordinary completion of the story.

The subsequent private probes `build/analysis/probe_credits_effects.py` and
`build/analysis/probe_credits_followup.py` reuse this route for particle effects,
spiral motion, illustration loading and music. Their successful reports are
`build/runtime/eur_credits_effects/evidence_story86_full.json` and
`build/runtime/eur_credits_followup/evidence_story86_full.json`. They include
full particle/task records, independently derived RNG and motion results,
decoded image and palette checks, and graphics captures. Allocation addresses
and newly constructed child prefixes are explicitly observed helper outputs.
Read each report's uncovered branches before extending it; a successful music
state sequence does not independently verify audio mixing or physical output.

`build/analysis/probe_credits_small.py` extends that route to cloud movement,
star blinking, grid fade and radial delay. Its successful report is
`build/runtime/eur_credits_small/evidence_story86_full.json`. It derives complete
task/particle records, the shared workspace fields and RNG state without
refreshing expectations from helper outputs. Consult its branch observations:
positive-X radial launches and a zero RNG seed were not exercised. This is
evidence for those update routines, not independent verification of every
native graphics callback. Keep new probes equally explicit about their limits.

`build/analysis/probe_credits_gradient.py` adds an ordered GPU/workspace store
oracle and checks the full 52-byte workspace prefix around the native sprite
pass. Its report is `build/runtime/eur_credits_gradient/evidence_story86_full.json`.
It records both background variants, nested star/cloud call order and counter
effects; the sprite and texture rendering internals remain outside its oracle.
Before reusing captures as a baseline, verify their recorded sizes and hashes
and compare the shared graphics buffers with the preceding credits replay.

### Nawatobi

When explaining a memory edit, specify CPU/address space, ROM region, pointer
dereferences, field offset, access width and timing. The tested EUR Nawatobi
entry uses ARM9 main RAM with overlay 7 loaded. From a fully initialized pause
menu, break at pause update `0x02071F80` and perform this one-time edit
(pseudocode; `read32` and `write32` use little-endian 32-bit accesses):

```text
task = read32(0x0208E1E0)
assert task == r0 and read32(task) == 0x0208D9B8
phase_address = task + 0x30
assert read32(phase_address) == 2
write32(phase_address, 7)
```

The edited word is at `read32(0x0208E1E0) + 0x30` in ARM9 main RAM.
Its four bytes become `07 00 00 00`. `0x0208E1E0` stores the task pointer;
do not overwrite that pointer or add the offset before dereferencing it.
This is a runtime state field, not a VRAM address or a ROM patch.

Disable the edit hook and resume so the native transition can advance the
phase. The task is dynamically allocated; an observed task address is not a
universal address to patch. Its code is in `src/overlay007/nawatobi_*`;
the selector and rope simulation were exercised, but a normal entry route
remains unconfirmed.
The compatible private starting snapshot is
`build/runtime/states/pause_subscene_menu65.dst`, derived from checkpoint 65.
Verify its provenance and the pause-update byte guard before reusing it.
The private capture records are `build/runtime/eur_nawatobi/evidence_065.json`
and `build/runtime/eur_nawatobi/rope_evidence_065.json`. Keep these local; cite
the observed setup and its limits when sharing instructions with researchers.

`build/analysis/probe_nawatobi_render.py` extends the route with follower/model
and rope-sprite oracles. Its `menu65`, `level1_65` and `level2_65` reports are in
`build/runtime/eur_nawatobi_render/`. From the selector, A starts Level 1;
one Down selection followed by A starts Level 2. The recorded button schedules
exercise all three follower animations on both screens and interior rope draw
orders. They verify controlled entry followed by native level selection and
updates, not successful level completion or a normal story entry route.
Nawatobi's rope sprite copy adds 208 pixels between screens; this differs from
the title sequence's 244 and the credits' 224. Use the subsystem's own layout.

Several different fields are called a phase or state. These are the recovered
EUR ARM9 access locations, with overlay 7 loaded; only the pause phase above is
the tested one-time entry edit. `r0` below means the argument at the specified
function's guarded entry, and `read32` dereferences a little-endian pointer.

| Field | Address or pointer expression | Access |
|---|---|---|
| Pause scene phase | `read32(0x0208E1E0) + 0x30` | 32-bit word |
| Nawatobi scene phase | `read32(0x020A6BBC) + 0x2C` | 32-bit word |
| Selector / level task phase | `r0 + 0x20` at `0x0208C5BC` / `0x0208C4D0` | 32-bit word; timer at `r0 + 0x24` |
| Active level / selected row | `0x020A6BCC` / `0x020A6BCD` | Bytes; selected row is read signed and is zero-based |
| Menu initialized / script running flags | `0x020A6BCE` / `0x020A6BCF` | Bytes |
| Scene script shared variable zero | `read32(0x020A6B90) + 0x37EC` | 32-bit level argument |
| Primary script instruction pointer | `read32(0x020A6B90) + 0x386C` | 32-bit pointer; zero means stopped |

The level callback copies the active level into shared variable zero before
starting the primary script. The primary start clears its own `0xB8`-byte slot,
preserving the shared variables; see
[the script manager](../../src/overlay007/scene_script_manager.c). Its callback phases
0, 1 and 2 mean start, wait for the script, and a 60-update return delay. These
are code-derived meanings, not a claim that a runtime replay exercised every
transition. The selector displays five rows but accepts ordinary confirmation
only for the first two; the extra rows do not establish three playable levels.
Check the current link metadata and probe reports before claiming a neighboring
selector function is also reconstructed.

`build/analysis/probe_nawatobi_menu.py` checks complete console records, level
tasks, shared variables and primary script slots. Its reports are under
`build/runtime/eur_nawatobi_menu/`. Ordinary menu and Level 1/2 runs are separate
from `evidence_terminated_level1_65.json`: that fixture clears the initialized
primary script pointer once at a guarded level-callback entry. It verifies the
stopped-script branch, all 60 delay decrements and callback return to the selector;
it does not establish natural level completion or the script's own cleanup.
The ordinary level captures match the preceding rendering probe's graphics
buffers. Use the recorded fixtures and uncovered branches when extending it.
