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
- [EUR memory reference](#eur-memory-reference)
- [Compiler and linker behavior](#compiler-and-linker-behavior)
- [Reconstructing and integrating code](#reconstructing-and-integrating-code)
- [Runtime verification](#runtime-verification)
- Code-derived findings: [pause party status](#pause-party-status)
- Pause transitions: [verified exit tasks](#pause-exit-tasks-and-transition-state),
  [projection and callback ABI](#pause-transition-projection-and-callback-abi),
  [entry/exit setup calls](#pause-transition-setup-calls)
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
`check_nawatobi_unit.py` for overlay 7 (also used for pause-menu candidates),
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

## EUR memory reference

These locations are for the European game (`ARMP`), using the ARM9's address
space. `read32(address)` means a little-endian 32-bit dereference. Require the
owning overlay and a live allocation before inspecting a record; overlays can
reuse addresses, and constructors can run before their global is assigned.
The sizes below are recovered record or allocation extents, not permission to
read a freed object. See the [Nawatobi procedure](#nawatobi) for the guarded
one-time state edit; the remaining entries are inspection references.

| Record | Address expression | Extent and ownership |
|---|---|---|
| Pause scene task | `read32(0x0208E1E0)` | 56 bytes, overlay 7; signed 32-bit phase at `+0x30`, menu pointer at `+0x34` |
| Pause workspace | `0x020905F0` | 90,600-byte object, overlay 7; do not dereference its first word as a pointer |
| Pause transition workspace | `0x0208E1E8` | 9,224-byte object, overlay 7; signed 32-bit progress at offset 0, not a pointer to another allocation |
| Pause party | `read32(0x0208E1E4)` | 4,428-byte allocation, overlay 7; `Overlay7Party` describes the first 332 bytes, followed by 4,096 bytes of scratch |
| Live save record | `read32(0x02059FE8)` | 1,380-byte record used by the probes; this is distinct from the on-disk battery-save size |
| Display resources | `0x0206A180` | 176-byte object, overlay 5; archive pointer at `+0x2C` |
| Menu element pool | `0x0206A240` | 20-byte object, overlay 5; element array, link array, free list, taken list and count at offsets 0, 4, 8, 12 and 16 |
| Menu element lists | `0x0206A254` | Twelve 16-byte list records, overlay 5; each contains two eight-byte sentinel records |
| Active menu element count | `0x0206A230` | Unsigned 16-bit value, overlay 5 |
| ResourceB sprite pool | `0x0206AA18` | 20-byte pool object, overlay 5; 64-byte item slots and eight-byte links |
| Draw-node pool | `0x0206A3F8` | 20-byte pool object, overlay 5; eight-byte nodes and eight-byte links |
| Draw lists | `0x0206A40C` | Two screens of 64 twelve-byte list records, overlay 5; distinct from the menu element lists |

The shared source layouts are in
[pause_scene.h](../../include/game/pause_scene.h),
[pause_scene_internal.h](../../src/overlay007/pause_scene_internal.h),
[overlay007_party.h](../../include/game/overlay007_party.h) and
[element_lists.c](../../src/overlay005/element_lists.c).
The menu factory at `0x0206659C` takes `(callback, list, marker)` and allocates
a 72-byte slot. It initializes the common fields through offset `0x20`, leaving
the payload at `+0x24` onward unchanged. The caller initializes its own payload.
Marking at `0x0206650C`, removal at `0x020664A8` and pool return at `0x020667B0`
are distinct boundaries. Check the release callback and slot before pool return,
then check only still-live pool, list and counter records.

For sprite and draw allocation, use the actual layouts in
[item_pool.c](../../src/overlay005/item_pool.c),
[overlay005_resource.h](../../include/game/overlay005_resource.h) and
[draw_lists.cpp](../../src/overlay005/draw_lists.cpp).
`Overlay5ResourceOwner` has its release callback at `+0x18` and resource pointer
at `+0x1C`; these fields belong to the owner, not the attached sprite.
ResourceB attachment can install `Overlay5ResourceB_Release`, which releases the
sprite and clears the owner's resource pointer. Track the task and sprite as
separate lifetimes, using [resource_owner.c](../../src/overlay005/resource_owner.c)
and the removal helper to establish the actual sequence.
Whole-pool destruction also ends allocations: verify the cleared pool pointers
before discarding their allocation records. Retain any outstanding per-task
removal assertion; a bulk free does not prove that callback completed normally.

The pause and shop probes commonly retain the following graphics ranges.
These are **capture extents**, not a universal map of all available video memory;
interpret them with the captured VRAM-bank and display configuration.

| Capture | ARM9 address | Bytes captured |
|---|---|---|
| Main / sub BG | `0x06000000` / `0x06200000` | 131,072 each |
| Main / sub OBJ | `0x06400000` / `0x06600000` | 65,536 each |
| Palettes / OAM | `0x05000000` / `0x07000000` | 2,048 each |
| Main / sub display registers | `0x04000000` / `0x04001000` | 112 each |
| VRAM-bank registers | `0x04000240` | 10 |

That is nine dumps per capture, alongside a 256 by 384 screenshot of both
screens. Validate the specific probe's manifest rather than assuming it used
these defaults. Register snapshots supplement ordered GPU-write checks; a
scroll-register readback need not reproduce the submitted value.

EUR ARM9 stack outputs can reside in DTCM at `0x027E0000..0x027E4000`.
Read that CPU-visible range directly in a focused probe. The current
`runtime_probe.py` command-line capture-range validator does not include DTCM;
do not redirect such an address to a main-RAM mirror to bypass that limitation.

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

`shop_party_bitmap.cpp` draws a member's equipped item name and icon into the
shop bitmap. Its complete 484-byte C++ function at `0x0207BF1C` matches using the
shared `ShopSceneWork` type. The original top-tested token loop stops on NUL or
`FF 00`; the coordinate tables contain four pairs of halfwords. Flattening those
pairs introduced an extra address calculation in the initial private draft.
The native halfword stack clear and eight-byte token return storage are retained.

`probe_shop_party_bitmap.py` and `shop_party_bitmap_oracles.py` repeat the same
three Sell/return routes. Equipment and bean shops exercise 24 renderer calls,
covering all four members, both item categories and both native callers. The
oracle derives localized names from the original item records/category selector
and live language tables, all 14 text-init arguments and the padding-preserving
48-byte text state, the 832-byte scratch clear, 104-by-12 bitmap clear, centered
4bpp-to-8bpp tile copy with palette offset 16 and 8bpp icon copy with offset 80.
Independent pixel coordinates preserve transparent pixels and untouched tails.

Live `GameHeapBlock` headers establish complete allocations: bitmap 53,248 bytes,
scratch 5,120 and icons 1,536. The oracle also preserves their headers, the
complete localized string allocation, workspace, renderer and live save. The
346 text tokens' state and first 832 scratch bytes remain observed renderer
output; subsequent bitmap copies are independently derived. These runs use
English, NUL termination, no clipping and an already-set dirty flag. `FF 00`,
other languages, clipped names and an initially-clear dirty flag remain untested.

All three replays pass 5,981 frames and 377 checked root returns; the retained
numeric oracle verifies 150 renders. All 58 screenshots, 522 graphics dumps and
104 source saves validate, with all 580 artifact pairs identical to the preceding
selling-message batch. Equipment names and party icons were visually inspected.
Each route closes three scene instances, with no pending calls or drain frames.
Earlier caption, slide and quantity/adjustment oracles are not repeated here.
Reports: `build/runtime/eur_shop_party_bitmap/`.

`shop_confirmation_cursor.cpp` covers the contiguous 232-byte clear/create/draw
group at `0x02076FBC` through `0x020770A4`. The shared 72-byte row task now names
its cursor coordinates at +40/+44. Creation sets (86,112), attaches a model,
starts animation zero and sets the low two model flag bits to one. Drawing adds
80 pixels per Yes/No selection from the signed workspace byte +0x9F and submits
at priority 10. The clear routine preserves an indirect tail call to group 3's
deferred-removal helper. The nearby 68-byte prompt-label callback is a separate
unlinked gap; this cursor group does not establish its execution coverage.

The ordinary confirmation route opens an equipment item with two A presses,
moves right/left between Yes and No and cancels with B; the bean route moves to
No and confirms with A. Neither purchases an item or changes inventory through
a fixture. `probe_shop_confirmation_cursor.py` checks 2,742 frames and 532 root
returns, including 486 new-function calls (two creates, 482 draws, two clears).
It derives task fields, attachment/pool changes, flags, (86/166,112) positions,
draw-list appends and deferred removal. Allocator, resource initialization and
animation outputs are explicitly observed helpers.

Models occupy complete 336-byte slots in the shared pool, established by native
`0x02068CE4` allocation and `0x02068DF0` selection. Check each slot, full tasks,
workspace and save at helper boundaries. The task's removal flag is set one
frame before its unlink/resource release and pool return. Check the last task
fields before the pool return, then only still-live list/pool records. A tail
call can put several pending returns at the same address and SP; finish their
nested records in order before processing the caller's next instruction.

Reports under `build/runtime/eur_shop_confirmation_cursor/` validate 30 screenshots,
270 graphics dumps and 104 unchanged saves. All 160 equipment-route artifacts
match the earlier discovery replay; the bean route uses a different cancellation
sequence. Both confirmation positions were visually inspected. The retained
numeric oracle verifies 17 renders; broader buying, bitmap and selling checks
remain in their earlier reports. Successful purchases, post-purchase equipment
prompts and empty-group clearing are outside this route's coverage.

`shop_equipment_highlight.cpp` reconstructs the contiguous 1,172-byte group at
`0x02076B28` through `0x02076FBC`: stop, start and per-frame update. The signed
workspace flag at +0xAF controls the fade. Start copies the 2 KiB map at +0xB6,
clears the left half for a one-member party, initializes sub-screen blending and
creates a 72-byte task. Update selects the eligible party group, writes two
2-by-3 tile patterns and switches the background's vertical offset between zero
and -88. Its Q12 alpha fades between zero and eight; division by six leaves a
remainder, so both complete transitions take seven updates before clamping.
The separate `data_ov009_0207eaf2` symbol is the native alias of that map inside
the workspace. Loading it directly preserves the original literal reference.

The matching pause-resource code supplied the tile-loop counterpart: write
`map[32 * row + column]`, allowing the compiler to derive the row stride, instead
of manually introducing the decompiler's cached index. Incrementing the column
before the 16-bit tile counter preserves the native induction-update/truncation
order. This resolved both register and scheduling differences without inline
assembly, compiler changes or source permutations. All three complete functions
match, including their literal pools.

Private `probe_shop_highlight.py` checks the ordinary confirmation and short
scrolling routes after the established controlled shop entry from checkpoint 65.
Reports under `build/runtime/eur_shop_highlight/` cover 2,482 frames, 954 new
function returns, 1,928 independently expected GPU stores and 948 twelve-entry
map updates. The four selected clothing items exercise both eligible-group
results and both vertical offsets. Oracles preserve the full 2,492-byte workspace,
1,380-byte save, 72-byte task, selected-panel allocation and 128 KiB sub BG memory
at helper boundaries; item flags are checked against original resident records.
Allocator output is explicitly observed; initialized task fields, fade state,
map changes, helper results and register writes are independently derived.

Both tasks reach removal after fade-out. Here marking and pool return occur
later in the same scheduler pass, unlike the confirmation cursor's next-frame
removal. Hook ordering establishes the lifetime even when frame numbers tie.
The artifact verifier initially assumed a later frame; that assumption was
corrected and the verifier passed against both successful replay reports.
Full task contents and neighbor links are checked before pool return, then only
live pool records and counters are read. All 32 screenshots, 288 graphics dumps
and 104 source saves validate; all 160 confirmation-route artifact pairs match
the preceding cursor replay. Both highlighted portrait groups were visually
inspected. One-member clearing, unavailable party groups, early fade interruption
and successful purchases remain outside these routes' coverage.

The stock-row group adds 1,008 matching bytes: caption placement at `0x0207218C`,
quantity digits, icons, text segments and row updates at `0x02072470` through
`0x02072788`, followed by stock-panel hide/show through `0x02072818`.
The 72-byte task stores its row, quantity, palette and priority as halfwords at
+40/+42/+44/+46, and tile/position/offset words at +48 through +64. ResourceB
sprites occupy complete 64-byte slots, unlike ResourceA models' 336-byte slots.
The panel's 224-byte layout preserves its old byte views while naming enabled,
filter mode and filter item at +0/+210/+212. Row positioning uses the signed ring
index and global vertical offset; filtering compares the exact item or bits
24 through 30 of its usable-item record. Two typed 20-byte record pointers
preserve native address calculation and field-load order in the matching update.

Private `probe_shop_stock_rows.py` and `verify_shop_stock_rows_artifacts.py`
validate checkpoint 30 and 86 routes under `build/runtime/eur_shop_stock_rows/`:
1,362 frames, 46,449 new-function returns, 5,199 row updates, 39,860 independently
derived sprite submissions and 134 deferred task removals. All 28 screenshots,
252 graphics dumps and 104 unchanged source saves validate. The routes exercise
six versus nine rows, hidden and already-enabled show paths, filter modes zero
and one, both palette results and suppression of 1,384 leading-zero digits.
Full task/parent/sprite, workspace, save and panel allocations are compared at
helper boundaries. Panel setup and row/caption factory outputs remain observed
helpers. Sprite attributes, positions and draw-list links are independently
derived; captured native-renderer pixels are observations, not a pixel oracle.
Filter mode two, empty stock and successful purchases were not exercised.

The first oracle checked the caller's x store at the X helper's return address,
before that store executed. Moving caller-owned expectations to later helper
boundaries fixed the replay. Requiring leading-zero suppression in every route
also failed for checkpoint 86's two-digit stock; the successful suite checks it
on checkpoint 30. Despite its filename, `eur_story_030_hud_verified.dst` opens in
the save menu. Its tested prelude is `wait:180 b:8 wait:240`, which cancels the
menu and reaches the guarded FieldVM shop entry. Both final replays finish shop
cleanup with no pending hooks; no inventory fixture or purchase is needed.

`ShopStockPanel_RebuildList` subsequently reconstructs the 356-byte initializer
at `0x02073098`. It forwards both the workspace menu selector at +0x98 and the
caller's selection to `ShopStock_MapMenuCategoryToItemClass`; the pseudocode
omitted that second argument. It selects the inventory array and item tag,
packs entries whose signed quantity byte is nonzero, and resets the filter and
ring indices. The native clear is 198 bytes: it clears 99 of the 100 item slots
and preserves the final slot. The shared panel now names its item limit at +2.

Private `probe_shop_stock_setup.py` and `verify_shop_stock_setup_artifacts.py`
check checkpoint 30 and 86 category changes and exit in 1,152 frames. Four
initializer calls produce usable/Bros. item counts of 6/5 and 13/10 respectively.
The probes independently derive the full panel allocation at mapping, clear,
initial-renderer and return boundaries, including omitted zero-count entries,
item tags, pointers, resets, untouched fields and the nonzero final slot `00 EA`.
The workspace, save, pointers and heap header remain unchanged across the call.
All 14 screenshots, 126 graphics dumps and 104 source-save hashes validate.
The initial text renderer remains an observed helper. A separate ordinary
equipment-menu route did not call this initializer and failed its coverage
assertion; it is not counted as a successful replay. Clothing and badge branches,
empty inventory and independently calculated renderer pixels remain unexercised.
Reports are under `build/runtime/eur_shop_stock_setup/`.

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

### Pause party status

These findings come from EUR overlay 7 instructions and current shared types.
The party bitmap pair below is linked and runtime-verified; the low-HP control
findings remain code-derived. Check the manifest and private handoff for the
other candidates' status; do not count a matching private object as linked code.
Private evidence includes `build/analysis/pause_party_status.cpp` and the complete
instruction and literal-pool listing `build/analysis/pause_party_status_native.txt`.

The existing [pause workspace](../../src/overlay007/pause_scene_internal.h)
is a 90,600-byte `PauseSceneWork` at ARM9 RAM `0x020905F0` while overlay 7 is
loaded. It is an object at that address, not a pointer stored there.
The party-availability bytes start at `+0x116`. The indicator control byte is
`unknown2dc[1]`, at `+0x2DD` (`0x020908CD`); reuse that field rather than
inventing a separate global for the same storage. This byte is distinct from
the 32-bit pause phase used by the Nawatobi entry procedure below.

The native callback at `0x02080D4C` gets its attached ResourceA model before
examining the control byte. Zero requests deferred task removal; two returns
without submitting the model. Other values take the HP-dependent path, then
submit the model with argument 60. The stop helper at `0x02080C2C` clears the
byte. The creator at `0x02080C40` sets it to one and creates an indicator for
each available party member. These control-flow meanings do not establish
which ordinary menu routes exercise all three cases.

For the callback's HP check, `gSaveData` is the pointer stored at `0x02059FE8`.
Member `i` is at `read32(0x02059FE8) + 0x3F8 + 0x24 * i`, using the existing
[SavePartyMember](../../include/game/save_data.h). Maximum HP is the unsigned
16-bit field at member `+0x0C`; current HP is at `+0x0E`; level is the byte
at `+0x18`. These are live RAM offsets, not offsets to edit blindly in a `.sav`.
The callback compares `100 * current_hp > 25 * max_hp` with signed 32-bit
products after unsigned halfword loads. The products fit in a signed 32-bit
integer. Preserve this multiplication and comparison rather than introducing
division, rounding or floating point.

Above the threshold, the callback stores -256 into both model animation offsets
at `+0x5C` and `+0x5E`. At or below it, the callback loads the task's 32-bit Y
and X fields at `+0x30` and `+0x2C`, respectively, before writing X and Y as
halfwords. The task's member index is at `+0x28`. The private task layout is
72 bytes; independently confirm the actual allocator and attached model's
complete 336-byte slot when adding a runtime oracle. In particular, check the
threshold equality case, suppression/control paths and eventual pool return.

The linked [party bitmap pair](../../src/overlay007/pause_party_bitmap.cpp)
comprises `PausePartyBitmap_Rebuild` at `0x0207923C` (356 bytes) and
`PausePartyBitmap_DrawValue` at `0x020793A0` (396 bytes). Rebuild clears a
4,480-byte member slice through workspace `owned8c` at `+0x8C`, then draws
level/current/max HP labels and values. The formatter caps the value to its
requested decimal width and optionally suppresses leading zeroes.
[Pause resource initialization](../../src/overlay007/pause_resources.cpp)
establishes the complete allocation as 17,920 bytes: four 112-by-40 member
slices. Its other bitmap, `owned88`, has a separate 49,152-byte allocation.
The clear wrapper's native stack halfword store/load explains its scoped
volatile zero. Both functions match without assembly or compiler flag changes.
Call instructions establish eight arguments to `0x0207952C`, six to
`0x020793A0`, and seven each to `0x02081334` and `0x02080DEC`. Pseudocode can
omit arguments, infer an extra reused stack value, or infer narrow parameters
from callee stores; retain the caller's actual full-width argument behavior.

`build/analysis/probe_pause_party_bitmap.py` verifies the linked pair through
normal equipment-menu navigation on checkpoints 65 and 86. Returning from
equipment comparison rebuilds all four HP/level slices. The starting snapshot
`build/runtime/states/pause_subscene_menu65.dst` selects Cobalt Star: press Down
twice, with released frames and animation waits, to reach equipment. The
checkpoint-86 HUD state uses Start to open the menu, then one Down selection.
Each route enters equipment, opens the comparison, switches the character with
Right, and backs out. The reports retain the precise input schedules.

The two passing reports under `build/runtime/eur_pause_party_bitmap/` contain
eight rebuilds and 24 value draws in 2,350 frames. The probe checks the complete
bitmap allocation and heap header, font allocation and header, 90,600-byte
workspace, 1,380-byte save, pointers and offset table at helper boundaries.
Nested calls share independently derived expectations. Decimal formatting is
checked against padded strings; each destination pixel is derived from tile
coordinates, source glyph bytes, palette offset and transparency. The native
glyph-copy helper remains unlinked, but its 76 calls have this pixel oracle.
All 144 signed-division calls are checked for quotient and remainder.

The HP label starts at X = -6 on later rows. Native code uses linear addresses
without horizontal clipping, and all these writes remain within the member
slice. An initial oracle incorrectly required nonnegative X; correcting it to
check each linear destination address produced a passing rerun. The earlier
Cobalt Star and item-list discovery routes made no calls and failed coverage;
they are not passing evidence. No game-code correction or RAM fixture was used.

The suite covers two/three-digit values, 52 drawn digits, 20 leading blanks,
zero-clearing and transparent preservation. It does not exercise zero HP, value
saturation, explicit leading zeroes or a non-three-digit width. The artifact
verifier checks 34 screenshots, 306 graphics dumps and all 104 unchanged source
saves. Both routes finish with no pending calls. Final GPU output and screenshots
are observed, not independently rendered by the oracle. These results do not
verify the still-private low-HP callbacks or their lifecycle and threshold cases.

The linked [clock separator callback](../../src/overlay007/pause_numbers.cpp),
`PauseClock_UpdateSeparator` at `0x02080EF4` (164 bytes), increments its signed
timer at task `+0x24`. At 30 it subtracts 30, toggles the word at `+0x28`, and
either draws tile 268 at (72, 161) or clears the 4-by-8 rectangle at (74, 165).
The existing overlay-5 element pool establishes the complete task slot as
72 bytes. The callback does not allocate or release the slot itself.

`build/analysis/probe_pause_clock.py` checks 235 callbacks during 406 frames of
normal waiting and B exit from the checkpoint-65 pause snapshot. It verifies
227 wait returns, four show transitions and four hide transitions, with exactly
30 callback updates between toggles. Expectations cover the full task and pool
record, 49,152-byte bitmap and heap header, font allocation/header, workspace,
save and pointer. Helper arguments, per-pixel transparent glyph copies, rectangle
clears and the background-dirty flag are independently checked. The focused
artifact verifier validates nine screenshots, 81 graphics dumps and all 104
unchanged source saves under `build/runtime/eur_pause_clock/`. No fixtures or
pending calls remain. Task creation/destruction, invalid timer states and final
GPU output are outside this callback oracle; the two graphics helpers remain
unlinked even though their effects are checked here.

### Pause numeric displays

The linked [number module](../../src/overlay007/pause_numbers.cpp) contains
`PauseNumber_Create` at `0x02080DEC` (264 bytes), the existing 164-byte clock
callback, and `PauseNumber_Update` at `0x02080F98` (924 bytes). Consolidation
adds 1,188 matching bytes; the clock is not counted again. The
[72-byte task layout](../../include/game/pause_numbers.h) packs the member index
and leading-zero flag into one byte. Its six kinds select level, current HP,
maximum HP, coins, hours and minutes. Creation preserves payload fields that
the native factory and constructor leave untouched. All seven creator arguments
retain the callers' full-width behavior before the native narrowing stores.

The updater uses Q8 interpolation, signed division, states 0/10/11 and a cached
integer value. It caches before limiting the rendered value to its digit width.
Party/coin interpolation divides the difference by eight; time updates divide
by one. The save pointer is at `0x02059FE8`, coins at save `+0x488` and the 60 Hz
play-time counter at `+0x518`. Hours divide frames by 216,000; minutes use the
within-hour count and cap at 59 from 21,596,400 frames onward. These time
calculations and cap comparisons use unsigned values, unlike digit division.
The source save view is a 1,308-byte prefix, not the full 1,380-byte allocation.
Separate identical palette switch arms and native constant-initialization order
explain the final instruction layout; no assembly or compiler flag change is used.

Private `build/analysis/probe_pause_numbers.py` starts from the checkpoint-65
HUD state, opens the pause menu with Start, waits and exits with Start. Two
421-frame reports under `build/runtime/eur_pause_numbers/` verify 30 creations
and 6,240 updates in total. Each route reaches all six kinds, all four party
members, widths two/three/six, leading-zero drawing/suppression and cached
no-redraw returns. The oracle derives factory slots, list links and counters,
full task contents, interpolation/cache changes and decimal glyph selections.
It checks 270 signed quotient/remainder calls, 92 pixel-derived glyph copies,
44 rectangle clears, complete bitmap/font allocations and heap headers,
the workspace and save. All 30 tasks remain live until whole-pool shutdown;
the replay checks live payloads before shutdown and cleared pool pointers
afterward. It does not claim the individual removal path was exercised.

`evidence_ordinary65.json` uses no fixture. `evidence_hp_fixture65.json` changes
member-zero current HP from 77 to 70 once at a guarded idle update, then restores
77 after 12 callbacks. The two native interpolation sequences each take eight
updates. This controlled RAM edit is separate from ordinary gameplay and is
recorded with its pointer, halfword address, guard and restoration. Subsequent
screenshots and graphics dumps match the ordinary route. Both runs finish with
no pending calls or drain frames. The artifact verifier checks 12 screenshots,
108 graphics dumps and all 104 unchanged source saves. Time/display saturation,
unsupported kinds and other widths remain unexercised; final GPU output is
observed. The clock's previous callback oracle was not rerun for this batch.

### Pause blend background

`SceneMenu_PrepareBlendBackground` at `0x02081560` (260 bytes) is linked in
[scene_menu_background.cpp](../../src/overlay007/scene_menu_background.cpp).
It configures main BG0, optionally clears its scroll and sets the existing
window manager's `display.bits.fixed_main_scroll`, then masks and adds a tile
bias to the visible 32-by-24 map. The flag belongs to the complete 3,908-byte
`GameWindowManager` pointed to by the scene manager's first word; it is not an
unidentified scene-controller field. The scene manager allocation is 43,056
bytes. Its initialization and heap headers confirm these separate extents.

Both calls in `SceneMenu_StartBlend` pass character bank 8 or 12 and explicitly
narrow the tile bias to a halfword. The shared setup declaration now uses
`u16` for the bank and bias, matching the register setter's parameter type and
the native setup body without redundant narrowing. Full module checks also
verify both callers. The final source uses existing window/display types and
matches without assembly or compiler flag changes.

Private `build/analysis/probe_scene_menu_background.py` follows ordinary
equipment/comparison/back/exit routes from checkpoints 65 and 86. Passing
reports `evidence_equipment65_verified.json` and `evidence_equipment86.json`
under `build/runtime/eur_scene_menu_background/` cover four setup calls in
2,060 frames. They independently check the fixed-scroll flag, BG field masks,
60 ordered GPU stores, derived VRAM map pointers, all 3,072 ordered tile writes
and full main/sub mapped BG VRAM. Complete window/scene allocations and heap
headers, workspace, save, palettes, OAM and stable GPU fields are checked.
Both bank/bias pairs (12/4096 and 8/0) run on each save. Thirty screenshots,
270 graphics dumps and all 104 unchanged source saves validate; no fixture,
pending call or drain frame remains. Reset-disabled calls and other parameters
remain unexercised; final rendered output is observed.

The first checkpoint-65 probe stopped at frame 143 because its store collector
only recognized unconditional `strh`, omitting the native mosaic helper's
`strheq` and `strhne`. The corrected collector uses instruction IDs and checks
ARM condition flags before counting a write. Both final routes pass after
that oracle correction; no game-code change was required.

### Pause sprite positions and lifetime

The linked [sprite callbacks](../../src/overlay007/pause_mode_sprite.cpp) are
`PauseModeSprite_Update` at `0x0207E93C` (112 bytes) and
`PauseModeSprite_UpdateTimed` at `0x0207E9AC` (160 bytes). Their
[task layout](../../include/game/pause_mode_sprite.h) is a 72-byte pool slot.
Both obtain their ResourceA sprite before comparing task mode (`s16`, `+0x28`)
with the signed byte at `PauseSceneWork +0x110`. A mismatch requests removal.
Otherwise they convert both signed Q12 coordinates to pixels with division
toward zero, store the two sprite halfwords at `+0x5C/+0x5E`, and submit to
draw list 5. The timed version first decrements a nonzero 32-bit counter at
task `+0x24`; reaching zero requests removal without drawing. An initial zero
leaves the timer inactive. Preparing both coordinate divisions before the
assignments reproduces the native interleaved sign corrections without assembly
or compiler changes. ResourceA's concrete sprite slot is 336 bytes; the common
`BattleModel` interface does not establish a larger allocation.

Private `build/analysis/probe_pause_mode_sprite.py` follows clothing selection
from checkpoint 65 and badge selection from checkpoint 86. Enter Equipment,
select Clothing or Badges, choose a character, confirm an item, then back out
and close the pause menu. The reports `evidence_clothing65.json` and
`evidence_badges86.json` under `build/runtime/eur_pause_mode_sprite/` record
3,360 frames, 960 ordinary and 720 timed callbacks, 1,668 submissions, six timer
expirations and six removals caused by a mode change. Each timed sprite starts
at 120 and receives exactly 120 updates. All 12 tracked tasks are actually
unlinked, release their resources and return to their pools. No fixture,
unfinished call, remaining task or drain frame is present.

The independent oracle checks full tasks, sprite slots, workspace and save,
signed arithmetic, helper arguments, and draw-pool selection before submission.
It derives the free/taken link changes, node, list header and old tail writes,
including overlapping sentinel views. It also checks task unlinking, cleared
resource/release fields, counters and both pool returns without reading freed
slots. Tracking begins at the first callback; creator bodies, virtual sprite
cleanup and final graphics are observed rather than independently verified.
The artifact verifier validates 44 screenshots, 396 graphics dumps and all
104 unchanged source saves. Negative coordinates, an initial zero timer, and
the timed callback's mode-mismatch path remain unexercised. Both ordinary
routes use the 120-update timer; badge selection does not add zero-timer coverage.

The earlier `equipment65` and `items65` discovery routes completed navigation
but failed the final coverage assertion: neither reached these callbacks.
Their failure reports and logs are retained. The deeper clothing/badge routes
establish execution coverage; no oracle or game-code correction was needed.

### Pause list-row sprites

The linked [row callbacks](../../src/overlay007/pause_list_row.cpp) cover
`PauseListRow_UpdateDigitSprite` at `0x02073FA0` (252 bytes),
`PauseListRow_UpdateMarkerSprite` at `0x0207409C` (176 bytes), and
`PauseListRow_UpdateTextSprite` at `0x0207414C` (152 bytes). The parent and child
[tasks](../../include/game/pause_list_row.h) occupy 72-byte slots; their attached
ResourceB sprite occupies 64 bytes. Text uses four segments and copies the
parent's position, tile selection and palette bank. The marker uses parent
tile +16. Quantity digits preserve two signed divisions and suppress a leading
zero except in the units position. Clothing/badge eligibility retains the
native repeated kind queries and short-circuit order. The shared sprite union
preserves raw attribute access while naming tile, priority and palette bits.

Private `build/analysis/probe_pause_list_row.py` uses ordinary clothing selection
on checkpoint 65, badge selection on 86, and consumable items on 65. The three
reports under `build/runtime/eur_pause_list_row/` cover 4,080 frames and observe
80,343 target calls. They independently check 1,770 complete callback samples:
1,132 text, 283 marker and 355 digit calls. The deterministic sample includes
the first two calls per object lifetime/selector/row-input configuration and
all calls on frames divisible by 60. Branch counts describe checked samples:
all four text segments, list kinds 0/2/3, divisors 1/10, 315 drawn digits and
40 leading-zero suppressions. The oracle checks 710 signed divisions and
1,730 draw-list submissions.

Checks include full parent/child tasks, sprite slots, the actual 4,428-byte
party allocation and heap header, 90,600-byte workspace, 1,380-byte save,
display resources, helper arguments/results and unchanged padding. Expected
digits, packed attribute writes and draw-pool/list changes are derived
independently. Lifetime tracking runs independently of body sampling: all
171 tracked tasks release their ResourceB sprites, unlink and return to their
task pools. Tracking starts at the first callback, not allocation. No fixture,
pending call, live tracked task or drain frame remains.

The artifact audit validates 57 screenshots, 513 graphics dumps and 104
unchanged source saves. The 44 clothing/badge screenshots and 396 corresponding
graphics dumps equal the preceding sprite-lifetime probe at identical input
and capture boundaries. Visual inspection confirms readable lists and quantities;
these captures are observed output, not an independent pixel oracle. Unsampled
bodies, row creation/parent updates, final GPU rendering, unsupported divisors
or segments, and unobserved kinds/empty-item eligibility remain outside coverage.

### Pause list visibility and row measurement

The linked [list controls](../../src/overlay007/pause_list_control.cpp) cover
`PauseList_MeasureRowWidth` at `0x020742E0` (148 bytes), `PauseList_Hide` at
`0x02074374` (132 bytes), and `PauseList_Show` at `0x020743F8` (308 bytes).
Show allocates 340 OBJ tile units, prepares the current list, creates up to
nine visible rows and adds category-specific tasks. Hide remembers the first
row and selection by list kind, marks task group 2 for removal, frees the
Key Items image buffer when present, and unlinks the sprite allocation.
Measurement resolves the selected entry's name, clears both packed cursor
coordinates and forwards the text helper's width. Empty clothing/badge entries
use the native fallback-name path. That path matches statically but was not
exercised by these replays.

The shared [party prefix](../../include/game/overlay007_party.h) retains its
332-byte layout and existing byte views while naming visibility, five saved
first-row/selection pairs and the owned image pointer at +280. The allocation
is still 4,428 bytes. Raw workspace declarations remain with the existing raw
consumers so the new module can use the actual `PauseSceneWork` declaration.
Native branch conditions require an unsigned `count < 9` and a signed
`first + row >= count` within the same loop. Assignment order explains the
fallback-name conditional instructions; no compiler changes or assembly are used.

Private `build/analysis/probe_pause_list_control.py` checks every target call
on clothing65, badges86, items65 and key_items65 routes. The reports in
`build/runtime/eur_pause_list_control/` record 4,860 frames, five show calls,
five hide calls and two width calls. Kinds 0/1/2/3 are consumables, Key Items,
clothing and badges in these observed menus. From an open consumable list,
Right switches to Key Items; its two rows show Beans and Toadbert's Drawing.
The route checks the short-list stop and a 6,144-byte image buffer whose
contents match between load and the last live snapshot before deletion.
The other lists exercise nine-row creation; 38 row-creation calls and 294
task-removal flag writes are checked across the suite.

The oracle checks full party/work/save/display records and pointer/heap
headers at helper boundaries, native direct writes, getter results, item
selection, argument order including stack parameters, width forwarding,
group-2 flags, and allocation unlinking with neighbor/head/tail changes.
Preparation, allocation and category-helper mutations are explicitly observed
helper output. Text/heap/IO internals, row constructors and final rendering
are outside the independent oracle. Marking tasks is not proof of their later
removal. Kind 4, empty lists, null image buffers and plural/fallback width
branches remain unexercised. No fixture, pending call or drain frame remains.

All 70 screenshots, 630 graphics dumps and 104 unchanged saves validate.
The three unchanged routes equal the previous row probe's 57 screenshots and
513 dumps; visual inspection identifies the Key Items menu. The first clothing
attempt rejected an uninitialized kind before the preparation helper ran;
moving that guard to actual kind consumers corrected the oracle. The first
items attempt failed only its final assertion demanding a width call on every
route; width coverage belongs to the clothing/badge routes. Corrected reruns
pass. Earlier `bros65` and `bros_right65` discovery tags are retained but
excluded from these totals: the former stayed on consumables, and the latter's
second tab was visually identified as Key Items before the final named rerun.

### Pause list selection and row copies

The linked [selection helpers](../../src/overlay007/pause_list_selection.cpp)
cover `PauseList_GetTileRow` at `0x0207452C` (12 bytes),
`PauseList_DrawSelectedLabel` at `0x02074538` (160),
`PauseList_RedrawSelectedRow` at `0x020745D8` (220),
`PauseList_CopySelectedRow` at `0x020746B4` (140), and
`PauseList_CheckRowAvailability` at `0x02074740` (160). The party prefix's
byte +266 has a signed tile-row view alongside its existing raw byte view.
Selected rows use `(tile_row + selected) % 9`; item lookup uses the separate
`(first + selected) % count`. Preserve those two rotations and the signed
division. Each copied row is 1,088 bytes, transferred from main OBJ memory to
sub OBJ memory at +640. The dedicated label uses main allocation offset +9,792
and returns its measured width. The availability wrapper handles disabled
categories and zero quantity before forwarding the item/member predicate result.

The private `build/analysis/probe_pause_list_selection.py` is generated by
`make_pause_list_selection_probe.py` from the control probe. Edit the generator
as well as the generated file when changing that probe. Reports under
`build/runtime/eur_pause_list_selection/` cover 6,310 frames on five routes.
The four ordinary clothing/badge/consumable/Key Items routes observe tile-row
and availability calls, but only clothing/badge selection reaches the label
helper. The additional `consume65` route reaches the standalone copy and two
redraws as Mushroom quantity decreases to one and zero. Native relocation
`0x0206CF44` identifies the redraw call after inventory decrement; the old
controller pseudocode omits that branch, so use the native instructions.

Consumption uses a controlled fixture: at fully guarded EUR ARM9 menu callback
`0x0206C6A0`, phase 100, with the consumable list initialized, four current-HP
halfwords at `read32(0x02059FE8) + 0x406 + 0x24 * member` become one once.
The recorded original halfwords are restored at frame 1,450 before emulator
destruction. The original battery saves are never edited. Ordinary A presses
then exercise item use; this is fixture-assisted coverage, not an unmodified
story-save route. Captures show one remaining Mushroom followed by its removal
from the list when exhausted.

Across the suite, 49,335 target entries are observed and 988 complete calls
are independently checked: 434 tile-row, 549 availability, two label, one copy
and two redraw calls. All label/copy/redraw calls are checked. Frequent getters
and availability calls use the first two occurrences per party/arguments/list
and row state, plus calls on every sixtieth frame. Each route uses one pause
allocation. Verified samples cover kinds 0/1/2/3, 318 available results, 221
rejected results and ten disabled-category returns. Zero-quantity early returns,
kind 4, redraw without copying and other unobserved branches remain uncovered.

Checks include full party/header/work/save/display records, four signed DIV
results, item and tile selection, all helper arguments, result forwarding,
and five 1,088-byte copies. Copy/draw roots check both complete 65,536-byte OBJ
buffers, including unaffected bytes. Drawing/width helper mutations and the
quantity/availability predicate results are observed helper output; their
internals and final rendering are outside this oracle. The first clothing
attempt incorrectly expected the width helper to preserve the party text state.
The corrected probe records that known helper mutation separately and passes
without changing matching game code. Its failure log/report are retained.

All 91 screenshots, 819 graphics dumps and 104 unchanged source saves validate.
The four ordinary routes equal the prior control probe's 70 screenshots and
630 dumps. No pending call or drain frame remains; the fixture is restored.

### Pause item lookup and rotating inventory order

`src/overlay007/pause_item_lookup.cpp` owns four contiguous functions at
`0x02074D48..0x020750BC` (884 bytes): description ID, name ID, rotated
availability and rotating-order rebuilding. The lookup record strides are 20
bytes for consumables/badges and 28 for clothing/Bros. Items. Names use halfword
+2; descriptions use +4 for equipment and +6 for consumables/Bros. Items.
Key Items names use the consumable table, but their description ID is the
masked item index directly. Unsupported kinds return zero in the native code.
The full-width result accumulator and explicit halfword truncations matter;
the old pseudocode's apparent pointer-derived default return is incorrect.

Shared records retain their raw byte views and earlier interface-resource names
through aliases. The badge record type is shared, while its typed table extern
remains local: existing battle/shop users still declare the symbol as bytes.
Moving that extern to the shared header failed the full build; the corrected
local declaration passes every affected caller. Separate switch arms, the
native equipment-range check and post-increment array stores reproduce the
complete instructions and literal pools without assembly or flag changes.

The rebuilt order includes only entries with available quantity. Equipment
starts at item 1 and appends the empty-gear entry 0 at the end. Its returned
count is byte-sized, and the unused array tail stays unchanged. Rotated lookup
uses the signed saved-first byte for that category, performs signed division
before the category-enabled check, and forwards the availability predicate's
full integer result. These fields differ from the visible tile-row rotation.

The focused private probe and generator are `probe_pause_item_lookup.py` and
`make_pause_item_lookup_probe.py`; reports are in
`build/runtime/eur_pause_item_lookup/`. Four ordinary clothing65, badges86,
items65 and key_items65 routes check every one of 175 calls over 4,860 frames:
106 names, 6 descriptions, 7 order rebuilds and 56 rotated queries. Every
function runs for kinds 0/1/2/3. Checks include 56 signed divisions, 7 one-byte
stack outputs with neighboring bytes preserved, 100 retained/32 excluded
inventory entries, 2 empty-gear append operations and 2 disabled-category
returns. Full live party allocation/header, workspace, save, display and item
records are checked at the relevant boundaries. Quantity calculations use
save/equipment/displayed flags independently; the final availability predicate
has 15 positive and 39 negative observed returns, with its internals uncovered.

The first clothing replay stopped at frame 1347 because the hook at
`0x02074F20` survived the return to field. That address is then inside overlay
0's function at `0x02074F18`. The corrected probe excludes such entries only
when its entire native owner function matches and no checked call is pending;
all 8 foreign-overlay hits satisfy that guard. Arbitrary byte mismatches still
fail. The original log/report remain under the `_overlay_reuse` suffix, and all
four corrected routes pass. No matching game code changed for this correction.

Artifact validation passes for 70 screenshots, 630 graphics dumps and all 104
unchanged source saves. Every capture matches the prior ordinary list-control
route at the same input/state/time. Clothing and Key Items images were visually
inspected. No RAM fixture, pending call or drain is present. Kind 4, unsupported
kinds, zero-quantity rotated early return, count wrap and other unobserved
branches remain uncovered; equal native-renderer captures do not independently
verify its rendering algorithm.

### Pause item text and inventory tables

`src/overlay007/pause_item_text.cpp` owns `0x02075B04..0x02075E10`
(780 bytes): `PauseItem_DrawText`, `PauseItem_GetText` and
`PauseItem_GetValues`. Drawing selects name, description or built-in menu text,
including the empty-clothing/badge fallback labels, then forwards all eight
renderer arguments. Text lookup handles singular/plural names, Beans and the
other Key Items, description resource tables and localized menu-table offsets.
The values helper returns the live category array and optionally writes one
byte of capacity. Its Key Items capacity source is a halfword at `0x020A6B98`;
the output truncates only at the byte store. The native fallback returns null
but does not define a capacity for unsupported categories.

Full-width temporaries followed by explicit narrowing reproduce name-index
increments and the capacity store. The draw fallback uses the same mode-first
initialization order as the already reconstructed row-width helper. Shared
prototypes replace earlier local declarations. `PauseItem_GetText` retains a
full-width plural argument because existing native callers pass a word; the
callee reads its low stack byte through a character view. Changing the shared
parameter to a byte would add narrowing instructions to those callers. The
draw wrapper's byte parameter already matches its callers. Every affected
caller passes the full matching build; no compiler flag or assembly change.

Private `make_pause_item_text_probe.py` generates `probe_pause_item_text.py`.
Reports live in `build/runtime/eur_pause_item_text/`. The four ordinary routes
are joined by `empty_clothing/` and `bros_items/` supplemental routes, both
using normal inputs from checkpoint 65. From an initialized main pause menu,
three separate Down presses followed by A open Bros. Items (menu index 3,
category 4). Its capture shows the Green Shell tutorial and attack-item list.
From the first clothing-list row, Up wraps to the final Unequip entry; both
its name and description fallbacks run. These routes use no RAM fixture.

Across 6,880 frames the probe observes 37,287 target entries and checks 1,107:
all 229 text lookups and 74 draw wrappers, plus 804 inventory getters out of
36,984. Standalone getters are sampled on the first two calls for each
party/category/output/save/count tuple and every sixtieth frame; getters
nested inside checked text calls are all checked. Each function runs for all
five categories. Checks cover 16 capacity-byte writes, 788 null-output getters,
94 plural and 36 singular name selections, two plural Beans names, two Key
Item remaps, one empty-gear name and two empty-gear descriptions. Resource
results are derived independently from the save's table pointers, language
offsets and entry offsets. Complete party/header, workspace/save/display and
the relevant pointer records remain checked; drawing also checks both entire
OBJ buffers, with the renderer's party/main-OBJ mutations labeled observed.
The renderer's pixel algorithm is outside this probe's independent oracle.

The initial clothing replay failed at frame 341 on output pointer
`0x027E3A84`. This is inside the EUR ARM9 DTCM range
`0x027E0000..0x027E4000`, not a main-RAM allocation. An attempted main-RAM
mirror interpretation was rejected by unequal reads. The corrected probe
retains the DTCM address, verifies that it lies in the caller's stack extent,
and checks all four output/neighbor bytes there. Nine DTCM snapshots cover
three stack addresses. Both initial failures remain under `_stack_mirror`
and `_mirror_assumption` suffixes; all six corrected replays pass. Do not
normalize CPU-visible TCM addresses into ordinary main RAM.

All 104 screenshots, 936 graphics dumps and 104 unchanged source-save hashes
validate. The four unchanged input routes equal the preceding 70 images and
630 dumps; supplemental routes have separate directories. The Bros. Items
and Unequip captures were visually inspected. There are no fixtures, pending
calls or drain frames. All 252 foreign-overlay hits were excluded only after
exact full owner-function guards. Unsampled getter bodies, singular Beans,
other unobserved fallback cases and invalid-mode behavior remain uncovered.
Mode 2 also ran with an otherwise unused category value of 96; do not require
an inventory category when that mode only reads the menu-text table.

### Pause party initialization and cleanup

`src/overlay007/pause_party_lifecycle.cpp` owns the contiguous 152 bytes at
`0x020762DC..0x02076374`. Both functions matched their first compiled draft.
`PauseParty_Init` clears the image pointer and initializes the embedded
48-byte `GameText` using the caller's font table and the scratch buffer at
party +332. Its fourteen arguments set x/y 0, spacing 1, leading 3, color 1,
space width 6, tile pitch 31, unused 0, alignment sentinel 255 and margin 0.
`PauseParty_Destroy` deletes a nonnull image array, clears that pointer and
returns the party address. It does not free the party allocation itself;
the surrounding resource code owns that later operation.

Private `make_pause_party_lifecycle_probe.py` generates the focused probe;
reports and the artifact verifier are under the corresponding private
analysis/runtime names. Four routes check six initializers and six cleanup
calls over 5,240 frames. Each checks the complete 4,428-byte live allocation,
its 16-byte header, workspace, save, display and globals. The text oracle
independently derives all initialized fields and reset masks, including
language, fonts, scales, cursor/style/bounds and retained reserved bits.
Uninitialized payload, text padding and unused scratch bytes stay unchanged.
The constructor runs before the global party pointer is assigned; check its
argument allocation, not an assumed nonnull global. An incoming image-pointer
field contained allocation residue and was correctly cleared without freeing it.

The first artifact verifier assumed one initialization/cleanup pair per route.
The successful clothing/badge replays instead showed the final Start reopening
the menu, reusing the same heap address for a second object. The corrected
routes add another Start and wait to close that menu as well. Each now checks
two complete initialization/cleanup pairs; item and Key Item routes check one.
The initial reports/logs remain under `_reopened_menu`, with the original
artifact-verifier failure retained. This was a route-coverage correction, not
a game-code or per-call oracle failure.

All 74 screenshots, 666 graphics dumps and 104 unchanged source saves validate.
The common input prefixes equal 70 prior screenshots and 630 dumps; the four
additional captures cover the final exits. The final clothing capture shows
the field. No fixture, pending call or drain is present, and 3,164 foreign-overlay
hits have exact owner guards. All six cleanup calls saw a null image pointer;
nonnull image cleanup and the surrounding heap allocation/free internals are
not claimed as covered by this probe.

### Pause queued row drawing and markers

`PauseList_QueueRowDraw`, `PauseList_DrawRowTask` and `PauseItem_CopyMarker`
own `0x020757C4..0x020758F0` in `pause_item_row_draw.cpp` (300 bytes).
`PauseItem_GetMarkerId` extends `party_values.c` through `0x02075400`
with 160 new bytes; its existing neighbors are not new coverage. The creator
allocates a 72-byte element in list 11 before marker 1, then stores the row
and item as words at offsets 40 and 44. The callback draws four text segments,
copies the marker at row offset +1024, and sets the deferred-removal flag.
It reloads the global party pointer between helpers, as the native code does.

Marker selection reads byte 9 of the existing typed item records: 20-byte
consumable/badge records and 28-byte clothing/Bros. Item records. Key Items
return -1. Empty clothing selects 11 for member values 0/1 and 12 otherwise
using an unsigned shift. The copy helper skips negative IDs; otherwise it
copies 64 bytes from the archive-1/entry-69 buffer at workspace offset 192
to main OBJ memory. The sampled live allocation is 1,280 bytes. The term
marker describes its row-graphics role; individual glyph meanings remain
unassigned. No compiler flags or assembly fragments were changed.

Private `make_pause_item_row_draw_probe.py` composes the focused probe from
`pause_item_row_draw_probe_body.py`. Reports and their artifact verifier are
under `build/runtime/eur_pause_item_row_draw/` and `build/analysis/`.
Seven routes cover 9,270 frames: clothing, badges, consumables, Key Items,
empty clothing, Bros. Items and a separate clothing-scrolling route.
All 105 ID selections, 87 copy calls, six creators and six callbacks are
checked, plus all six task removals. ID/copy coverage includes all five
categories, two skipped negative IDs and two empty-clothing selections for
member 0. The other empty-clothing member cases remain uncovered.

The first clothing route reached only ID selection and copying, so its
all-functions coverage assertion failed after the replay. Its `_no_scroll`
log/report remain private. Native callers establish that lists longer than
nine rows queue a task only when scrolling across a visible edge. The added
route opens clothing, presses Down eleven times and Up twelve times, then
exits with three B presses; each direction press is followed by a 30-frame
wait. It checks three forward and three backward queued updates. The final
field capture was inspected. All seven final routes pass.

The oracle derives factory free-list selection and complete slot/list/link/
counter writes before allocation. It checks row arguments, the removal flag,
unlinking and pool return as separate events, with no freed-slot reads.
All 85 transfers independently check the selected 64 atlas bytes, both full
OBJ buffers and the untouched atlas allocation. Full party/header, workspace,
save, display and globals are also checked. The text renderer's arguments and
resulting party/main-OBJ changes are observed; its pixel algorithm and atlas
decompression are outside this oracle. Queued callbacks ran for clothing only.

All 169 screenshots, 1,521 graphics dumps and 104 unchanged source saves
validate. Matching input prefixes equal 113 prior images and 1,017 dumps;
the scrolling route has its own directory. There are no fixtures, pending
calls, live tracked tasks or drain frames. Full native guards remain active;
these routes produced no foreign-overlay hits. Invalid inputs and unobserved
marker IDs remain uncovered. Both ROM build paths retain the original SHA-1,
native relinking reports zero differing bytes, and all 81 tests pass.

### Pause inventory-list preparation

`PauseList_Prepare` at `0x02075F6C..0x0207616C` contributes 512 bytes in
`pause_list_prepare.cpp`. It selects the category from the main menu and its
submenu, captures the selected member, gets the live inventory pointer and
byte-sized limit, restores saved positions, and clears/rebuilds the slot list.
Only items with a nonzero available quantity enter the list. Clothing and
badges append the empty-equipment entry last. The function then constrains
the saved window/selection to the resulting count and requests a full redraw.

`Overlay7Party` now names bytes 2/3 as `item_limit` and `member`, preserving
the old raw view and the 332-byte prefix layout. The submenu array is the
existing `gSceneMenuSubmenus` alias at `0x02090717`, within the pause workspace
at +0x127. Resolve literal-pool references from their words and relocation
metadata; their appearance in an instruction listing is not a decoded address.
The initial matching-size draft differed around initialization order. Native
code evaluates the equipment start before resetting the count; the already
matching rotating-order helper uses that order too. Moving the count reset
after the category test produced the complete match, with no assembly or
compiler-flag changes.

Private `make_pause_list_prepare_probe.py` composes the focused probe from
`pause_list_prepare_probe_body.py`. Eight final routes under
`build/runtime/eur_pause_list_prepare/` check all ten observed preparations
over 11,660 frames. All five categories run. The oracle checks all 249
quantity results independently from inventory bytes and equipped-member
records, including signed truncation, unavailable-equipment subtraction and
the empty-gear quantity. It verifies 149 kept items, 94 excluded items, six
empty-gear appends, seven retained windows, two short-list clamps and one
full-window clamp. All party fields, padding and unused slot bytes remain
checked across the helper calls, together with the complete allocation/header,
workspace, save, display, globals and both OBJ buffers.

The original seven routes did not cover a restored full window, which the
artifact audit detected. Exiting pause entirely and reopening it also missed
that branch: the new party instance resets its saved positions. Both failed
coverage checks remain in the private `_full_window_assumption` and
`_field_exit` logs/reports. The final supplemental route scrolls to Unequip,
closes only the clothing list with B, waits, then reopens it with A in the
same pause instance. The second preparation restores first item 9 and
selection 8; its visible Unequip row and final return to the field were
inspected. All eight final replays and the artifact verifier pass.

All 234 screenshots, 2,106 graphics dumps and 104 unchanged source saves
validate; common input prefixes equal 226 prior images and 2,034 dumps.
There are no fixtures, pending calls or drain frames. The 15,828 hits after
overlay replacement were excluded only after matching the complete overlay-0
owner at `0x02075D28`. Full redraw arguments and its resulting party/main-OBJ
changes are observed, not an independent renderer check. Selection reduction,
invalid submenu/default cases and empty inventory remain uncovered. Both ROM
build paths reproduce the original hash, native relinking reports zero
differing bytes, and all 81 tests pass.

### Pause list movement

`PauseList_MoveSelection` at `0x020754D0..0x0207567C` adds 428 bytes to
`party_navigation.c`, alongside its four already linked neighbors (208 bytes).
It updates the signed selection byte, clamps or wraps short lists, and scrolls
a nine-row window for longer lists. Cursor sound 231 depends on the absolute
index changing; description drawing depends on the selected item changing.
The getter uses the unsigned view of the selection byte, while the index
arithmetic uses its signed view. Keep those access-site distinctions.

Two ternary assignments in the first draft produced one unconditional byte
store each instead of the native pairs of conditional stores. Explicit if/else
assignments recovered the missing eight bytes and matched the complete function.
Consolidation into the existing C module preserved assignment order while moving
local declarations to block starts. All five functions remain exact; no assembly
or compiler-flag changes were needed.

The private movement probe checks five routes from story checkpoints 65 and 86:
clothing and badge scrolling, consumables, Key Items and Bros. Items. Across
8,822 frames it observes 4,717 entries and fully checks 728 calls. Every nonzero
movement call is checked. Idle calls are sampled deterministically: the first
two per observed constructor lifetime, arguments, list and selection tuple,
plus every sixtieth frame. Branch counts describe checked calls. The callers
invoke movement every frame, including direction zero; held input drives repeat
movement while fresh presses enable wrapping.

All five item categories run, with short/long lists, wrapping and held-input
clamps in both directions. The oracle verifies 1,456 getter results, 1,532 signed
divisions, 90 sound/description calls and 24 scrolls (20 queued-row scrolls and
four full refreshes). It independently models control-byte changes, selection
and helper arguments while checking the complete 4,428-byte party and header,
90,600-byte workspace, save, display, globals and both entire OBJ buffers.
Wrapped redraw and description-renderer output are observed, not independently
derived pixels. Constructor entry identifies lifetimes; this probe does not
verify constructor bodies, audio internals or queued-task allocation/removal.

The original consumables replay failed at frame 1,348 because its foreign-owner
fallback assumed overlay 0. The corrected replay identifies the complete
336-byte overlay-8 owner at `0x02075390` by exact native bytes. Its final capture
shows the save menu after the route's extra field inputs. Key Item and Bros.
Items routes also encounter that owner. The old failure log/report remain
preserved; all five final routes pass with no pending calls, fixtures or drain.
No threshold-based byte comparison was accepted as proof of ownership.

Private evidence is under `build/runtime/eur_pause_list_navigation/`; its focused
generator/body/probe and the shared `verify_pause_list_graphics_artifacts.py`
are in `build/analysis/`. All 189 screenshots and 1,701 graphics dumps validate,
with 95 images and 855 dumps equal to matching input prefixes of prior routes.
All 104 original saves retain their hashes. Unsampled idle bodies, invalid
inputs and unvisited branches remain outside this runtime coverage.

### Pause list graphics initialization

`PauseList_InitGraphics` at `0x0207616C..0x020762DC` adds 368 bytes to
`pause_party_lifecycle.cpp`; its 152 neighboring initialization/cleanup bytes
were already linked. It resets both five-byte saved-position arrays and the
list control bytes, clears 4,224 main-OBJ bytes, and builds digits 0 through 9.
Each digit is rendered into scratch tiles at party offsets 332 and 1,356, then
copied as two 32-byte tiles to both screens' OBJ buffers.

The first draft already had the correct size and instructions apart from ten
stack references. Explicit clear blocks allocated their local zero values in
different slots from the native code. The inline clear wrappers already used
in nearby pause-resource code reproduce the native separation of caller text
buffer, hidden `GameTextToken` return temporary and wrapper locals. Using that
same structure yields an exact 368-byte match, including the literal pool;
all three functions in the consolidated module match. The volatile zero accesses
are supported by native stack stores/loads, not a register-allocation workaround.

Two ordinary clothing/badge routes at checkpoints 65/86 exercise four complete
initializations across separate pause lifetimes over 3,740 frames. The probe
checks every initializer, all ten digit strings, the hidden result-pointer ABI,
cursor masks, saved-position/control resets, all zero fills, OBJ address results
and all 160 tile transfers. It checks complete party/header/work/save/display/
global/OBJ records. The text decoder's state, scratch pixels and token result
are observed; copies from those pixels to both screens are independently
verified. Constructor and allocator bodies are not checked by this probe.

Private reports are under `build/runtime/eur_pause_list_digit_tiles/`, with
`make_pause_list_digit_tiles_probe.py`, its body and composed probe in
`build/analysis/`. All 48 screenshots and 432 graphics dumps validate and equal
the prior routes, all original saves remain unchanged, and no calls are pending.
The complete movement/graphics batch adds 796 bytes, passes full `ninja check`,
reproduces the EUR ROM hash through packaging and native relinking, reports zero
native differences, and passes all 81 tests. The working total at publication is
729,148 / 1,563,700 matching C/C++ bytes (46.63%); overlay 7 is 64,392 / 142,264
(45.26%). Matching C/C++ plus symbolic assembly is 46.96%.

### Pause equipment highlight

`PauseEquipmentHighlight_Stop`, `Start` and `Update` own the contiguous
`0x02077164..0x020774CC` range (872 bytes) in
[pause_equipment_highlight.cpp](../../src/overlay007/pause_equipment_highlight.cpp).
All three matched their first private draft. The state machine parallels the
already reconstructed shop highlight, with pause-specific SUB BG1 selection
and item eligibility. No compiler flags or assembly fragments changed.

The signed control byte is at workspace `+0x2DC`: 1 keeps the task active,
0 requests a fade-out and -1 cancels immediately. The selected item is an
unsigned halfword at `+0x2DE`, accessed through the native interior alias
`0x020907F0 + 0xDE`. It is an inventory index, not a bitmask. The shared workspace
keeps the old four-byte view alongside these named fields without changing size.
The 72-byte task holds phase at `+0x20`, alpha at `+0x28`, target at `+0x2C`
and step at `+0x30`; the factory and constructor leave other payload bytes intact.
The fade step divides by six, but integer truncation requires seven updates to
clamp between 0 and 32,768. The native code reloads the party global between
category lookup and eligibility checking.

All four native start call sites are guarded by category equal to 2 (clothing):
`0x0206BB48`, `0x0206C5B0`, `0x0207CB70` and `0x0207CD74`. A badge-menu replay
therefore does not exercise the update function's category-3 fallback. The
initial `badges86` replay failed its positive execution assertion after observing
only an inactive stop; its log and failure report are retained. The separate
`badges_inactive86` replay verifies the observed inactive stop and absence of
creation/update on that route. This is a corrected coverage expectation derived
from the callers, not a change to the matching game code.

Private `make_pause_equipment_highlight_probe.py` composes a focused probe from
`pause_equipment_highlight_probe_body.py`; reports are under
`build/runtime/eur_pause_equipment_highlight/`. Clothing, clothing scrolling,
immediate exit and inactive badges cover 6,520 frames. They observe four starts,
1,812 updates, four stops and four removals; all starts/stops/removals and 113
updates are fully checked. Stable phase-2 updates sample the first two per task
lifetime/control state/item/availability/category tuple and every sixtieth frame.
Control fields are checked across every callback entry, independently of sampling.

The four task lifetimes verify factory free-list selection, complete slots,
list/counter writes, deferred marks, unlinking and actual pool return. Both
eligibility results run during clothing scrolling. Three tasks fade out; pressing
Start with the clothing list open cancels the fourth immediately and returns
to the field. The probe independently checks 252 ordered blend, display-enable,
vertical-offset and direct blend-clear stores, plus full party/workspace/save,
display-resource and both BG/OBJ records. Party constructor/free hooks establish
readable lifetimes; their bodies are not newly verified here.

The dedicated artifact verifier passes all 120 screenshots, 1,080 graphics dumps
and 104 unchanged source saves. Common input/state prefixes equal 118 images
and 1,062 dumps from the earlier preparation routes. No fixtures, pending calls,
live tracked tasks or drain frames remain. The scrolling highlight and final
field capture were inspected. Native guards positively identify foreign overlay-0
owners after pause exits. Start-while-active, unavailable-party branches,
unvisited task phases and unsampled stable bodies remain uncovered; final GPU
rasterization is observed. Full public build checks, both golden ROM build paths,
progress checks and all 81 tests passed for this source and shared-header revision.

### Pause exit tasks and transition state

The exit subset in [pause_transition_tasks.cpp](../../src/overlay007/pause_transition_tasks.cpp)
is `0x0206D418..0x0206D51C` (260 bytes): `PauseScene_PrepareExitTask` drains the
archive queue and requests exit; `PauseScene_FadeOutTask` darkens both screens
and advances the scene to cleanup. The latter matched immediately. Preparation's
only differing instruction was `MOV 255` where the original materializes -1
with `MVN`; a local signed-byte access preserves that value without changing
the shared workspace's existing unsigned view.

[pause_scene_control.cpp](../../src/overlay007/pause_scene_control.cpp) owns
`0x02070AE8..0x02070B50` (104 bytes): `PauseScene_RequestExit` sets scene phase 5
and resets the menu callback's phase, selecting the simple fade for mode 0
and the existing shutter transition otherwise. `PauseTransition_GetProgress`
returns the first signed word of the 9,224-byte transition workspace at
`0x0208E1E8`. Both functions matched their first drafts. The Scene VM now uses
the recovered exit prototype and scene-pointer type; its native byte output
remains exact. The native dispatcher narrows the decoded mode to a byte before
the call, while the request function accepts a full-width integer.

Private `make_pause_exit_probe.py` and `pause_exit_probe_body.py` produce
`probe_pause_exit.py`; reports are under `build/runtime/eur_pause_exit/`.
The final routes are `normal_exit65_pool`, `fade_exit65_pool` and `repeat_exit65`,
covering 3,190 frames. They check every preparation (six), request (three), fade
update (17) and watched removal (three). The progress getter is observed 54,464
times and fully checked 998 times: the first two calls per distinct value and
all calls on every sixtieth frame. All 17 observed values from 0 through 16,384
in increments of 1,024 are represented in the checked samples.

The ordinary route exits the clothing list with Start. The alternate route
verifies the normal caller's arguments, then changes `r1` from 1 to 0 once at
the guarded `PauseScene_RequestExit` entry (`0x02070AE8`, frame 522). The native
callback reset, initialization, all 16 brightness decrements, scene phase 6 and
actual pool return then run. This verifies the alternate mode with a controlled
argument fixture; it does not establish a natural script route to that mode.
Both screens' 32 conditional brightness stores are checked in order, including
ARM condition flags. No RAM value is frozen and no battery save is modified.

Full task, scene, transition, workspace, save, display, BG/OBJ and brightness
records are checked at the relevant boundaries. Expected effects of the nested
request are composed into its caller's expected record. All three archive
flushes had empty queues; nonempty request processing is not newly verified.
The probe also checks 749 menu factories and ten pool-cleanup calls, including
repeat calls on already empty pools. It closes the three watched task lifetimes
at their actual pool return and avoids reading their slots after release.

The first long clothing replay exposed missing bulk-pool destruction tracking
when pause reopened. Its failure at frame 1,512 is retained. After that correction,
the same route failed its exit-entry coverage assertion: it exercised the getter,
but never entered preparation/request. The final repeated-exit route starts with
the known clothing-list exit and adds two ordinary pause open/close cycles; it
checks reuse across the pool lifetimes and ends in the field.

All 41 screenshots, 369 dumps and 104 unchanged source saves pass the dedicated
artifact verifier. Common input/state prefixes match 31 images and 279 dumps;
fixture comparisons stop before the argument edit. Fade and final field captures
were inspected. No pending calls, live watched tasks or drain frames remain.
Unvisited phases, nonempty archive queues, unsampled getter bodies and natural
entry to mode 0 remain uncovered. Full matching checks, golden packaged ROM,
zero-difference native relink, progress checks and all 81 tests pass.

### Pause transition panels and controllers

[pause_transition_tasks.cpp](../../src/overlay007/pause_transition_tasks.cpp)
owns `0x0206D418..0x0206E0D4`: the existing 260-byte exit subset and 3,000 new
bytes for six callbacks. Entry/exit panels use the shared 64-byte sprite,
progress controllers coordinate the screen transition, the window callback
clips the main screen, and the alternate entry callback fades into the menu.
[pause_transition_projection.cpp](../../src/overlay007/pause_transition_projection.cpp)
adds the 180-byte projection at `0x0206E1C8..0x0206E27C`; its separate unit is
temporary while the intervening 244-byte affine-row producer remains native.
Only the 3,180 new bytes count toward progress. The shared sprite's named
`screen` byte preserves its raw 20-byte prefix view and 64-byte size.

All seven functions match completely. The 532-, 396-, 856- and 380-byte bodies
matched their first drafts; the 676-byte controller matched after using the
existing factory declaration. The previously exact 160-byte callback and
180-byte projection remain exact against current headers. No assembly or
compiler-flag changes were used. Preserve the controller's phase-1 fallthrough
to phase 10, signed counters and separate masked/unmasked stores.

Four completed routes in private `build/runtime/eur_pause_transitions/` cover
3,960 frames: `normal65_regs`, `normal86_reader`, `fadein65_reader` and
`repeat65_reader`. Ordinary routes use story checkpoints 65 and 86; the repeated
route opens and closes pause three times and ends in the field. Every observed
transition callback is checked: 820 entry-panel, 408 exit-panel, 210 entry-progress,
102 exit-progress, 125 window and 18 alternate fade-in calls. Nested projections
are all checked. Other projection calls check the first occurrence of each
X/Y/angle tuple and every call on each sixtieth frame: 6,204 of 71,652 total
projection entries are independently verified.

The oracle derives wrapped signed arithmetic, packed sprite fields, all four
projection outputs, draw-list allocation, task/owner counters and window/blend
effects. It checks 684 ordered conditional GPU stores, 955 task factories,
546 ResourceB attachments, 60 actual task returns, 44 associated sprite returns
and 12 bulk-pool cleanup calls. All 61 watched callback lifetimes close: 60 by
removal and one by retargeting the alternate fade-in task to the normal menu
callback. Retargeting ends callback coverage, not the task allocation. Task and
sprite records are checked before their respective pool returns.

Alternate entry uses a controlled fixture at chooser `0x0206E918`, frame 42:
change the byte at `read32(0x02059FE8) + 0x55D` from 0 to 1, then restore it to 0
at the chooser's return in the same frame, after checking that the selected
callback is `0x0206DF58`. This exercises the native fade-in and subsequent normal
inputs; it does not establish a story checkpoint that naturally selects this
mode. The callback reaches scene phase 2 and retargets itself after 16 brightness
steps. Original battery saves remain unchanged.

All 52 PNGs and 468 graphics dumps pass the artifact verifier; 39 images and
351 dumps equal earlier captures with matching state/input prefixes. Fixture
comparisons stop before the edit. Exit frame 529, alternate menu frame 100 and
repeated-route final field frame 1,650 were inspected. No pending calls, watched
tasks or drain frames remain, and all 104 source-save hashes are unchanged.
Full matching checks, golden packaged ROM, zero-difference native relink,
generated-progress validation and all 81 tests pass.

The first probe failed at frame 44 because DISPSTAT/VCOUNT were treated as
immutable across a long helper call. Those scanline-dependent values, and
IRQ-driven affine registers, remain in captures but are excluded from immutable
record comparisons. A later benchmark variable shadowed the probe's `start`
function; that frame-44 failure and the corrected successful replay are retained.
The original slow replay completed before a proposed stop was issued; its
session returned exit 0 and its full report is retained.

Private `desmume_copy_bytes.c` and `fast_desmume_memory.py` move the loop over
DeSmuME's existing exported byte-reader into native host code. Every address and
byte access is preserved; the helper does not read emulator host RAM directly.
Boundary checks passed. Each accelerated route then compared all 4 MiB of main
RAM, 16 KiB of DTCM and the nine graphics capture ranges with the original reader
while paused: 4,608,234 identical bytes per route. Measured reads took about
0.012 seconds versus 0.66 seconds. This changes probe overhead only and remains
an optional local tool, not a fresh-clone dependency or matching-game progress.

Affine-row producer outputs and alternate fade-in setup-helper effects are
observed, not independently derived here. Final rasterization, unvisited task
phases, unsampled external projections, arbitrary angle/step ranges and natural
alternate-entry selection remain coverage limits. The generator, body and
composed probe under `build/analysis/` are `make_pause_transitions_probe.py`,
`pause_transitions_probe_body.py` and `probe_pause_transitions.py`; the dedicated
verifier is `verify_pause_transitions_artifacts.py` in the same directory.

### Pause transition projection and callback ABI

These EUR overlay-7 ABI findings explain the source and focused transition
replays above. The earlier exit-only replay did not independently verify these
bodies. Use the current manifest and recorded route evidence for their status.

The projection at `0x0206E1C8` takes seven arguments:

```c
void PauseTransition_Project(int x, int y, int angle,
                       int *out_x, int *out_y,
                       int *out_width, int *out_height);
```

At entry, `r0..r3` hold `x`, `y`, `angle` and `out_x`; the other three pointers
are at entry `sp + 0`, `sp + 4` and `sp + 8`. Account for the prologue before
interpreting later stack loads. The function explicitly masks the full-width
angle with `0xFFFF` before shifting by four to index the interleaved sine/cosine
table. A halfword parameter or cast does not reproduce that same instruction
sequence with this compiler.

Outputs are stored in the order Y, height, width, X. The stored width is read
back before calculating X; preserve that read and possible output aliasing.
The perspective calculation needs signed 32-bit wrapping before arithmetic
shifts and division toward zero. An unbounded Python integer model can produce
the wrong expectation even when a formula looks equivalent.

The progress callback at `0x0206D730` reads its signed step from task `+0x2C`.
In phase 1 it computes a sum and its `0xFFFF`-masked value, stores both in that
order to the transition workspace's first word, then compares and passes the
cached masked value. Re-reading the global after each store introduces extra
loads; deleting the first store also changes the native sequence.

The panel callback at `0x0206D51C` projects directly into the attached
`Overlay5ObjectSprite` coordinates, with width and height in stack locals.
The window callback at `0x0206D7D0` instead supplies four stack-local outputs.
A probe must check the actual live destination extents, including DTCM where
applicable, rather than assuming every output points into the transition object.
The panel's packed tile field is ten bits wide, and its affine call explicitly
narrows width and height to unsigned halfwords. Preserve neighboring attribute
bits and caller-side narrowing without changing the full-width callee signature.

The focused transition reports add arithmetic, output, GPU-store and lifetime
assertions to the earlier exit-only evidence. The affine-row producer at
`0x0206E0D4` still has observed outputs rather than an independent body oracle;
keep that distinction when extending the adjacent code.

### Pause transition setup calls

These are code-derived EUR overlay-7 findings from the native callers and
existing shared headers. They do not establish independent runtime verification
of the setup bodies. Check the manifest and private handoff for integration status.

The exit setup at `0x0206E27C` and entry setup at `0x0206E594` both call
`GameSpriteAllocation_Allocate` at `0x0200856C`. Its existing declaration in
[sprite_output.h](../../include/game/sprite_output.h) has eight arguments:

```c
int GameSpriteAllocation_Allocate(GameSpriteAllocation *allocation,
    int screen, int mode, u32 tiles, u8 shared, u32 resource,
    u8 dirty, u32 first_tile);
```

Both callers pass `&WORK.sub_allocation`, screen 1, mode 0, 384 tiles, shared 0,
dirty 1 and first tile 0. The resource argument differs: exit passes `0xFFFF`,
while entry passes `0xFF00`. At callee entry the four stack arguments are
shared, resource, dirty and first tile at `sp + 0`, `+4`, `+8` and `+12`.
The following `Overlay5Display_LoadObjTiles` at `0x02068220` takes four
arguments: sub-screen engine, `WORK.owned94`, `WORK.sub_allocation.offset` and
12,288 bytes. Its existing [display declaration](../../include/game/overlay005_display.h)
already matches this call. Do not copy omitted or invented pseudocode arguments
into a shared-header refactor.

The delay callback at `0x0206E8DC` reads the 32-bit counter at task `+0x24`.
A nonzero value is decremented and the callback returns, even when that decrement
reaches zero. A value already zero at entry calls `GameRumble_PlayRepeated(0, 2)`
and requests deferred task removal. Thus an initial delay of four triggers on
the fifth callback; an initial zero triggers immediately. The request does not
free the task. This differs from timed callbacks whose zero means indefinite.

The chooser at `0x0206E918` first calls `0x02080AE4`, then reads the unsigned
save byte at `read32(0x02059FE8) + 0x55D` once. If nonzero, it stores byte minus
one in `WORK.selected_menu` and selects `PauseScene_FadeInTask`; otherwise it
selects the entry setup at `0x0206E594`. Both callback changes pass `invoke = 0`;
the callback setter resets the task's phase to zero. The chooser then sets the phase at
`read32(0x0208E1E0) + 0x30` to 1. Earlier alternate-entry evidence observes this
selection; it does not independently check the chooser's complete body or the
preceding setup helper. The focused page-transition checks below now cover the
chooser itself; the preceding setup helper remains observed.

### Pause page entry and return

[pause_page_tasks.cpp](../../src/overlay007/pause_page_tasks.cpp) owns the
contiguous `0x0206E8DC..0x0206F04C` range: delayed rumble (60 bytes), entry
selection (124), page closing (532) and page opening (1,188), totaling 1,904
new matching C++ bytes. Shared task layouts are in
[pause_navigation.h](../../include/game/pause_navigation.h). The existing
scroll-state view at `0x020906F0` is now shared through
[scene_menu_motion.h](../../include/game/scene_menu_motion.h), preserving its
interior anchor and layout.

Closing fades for four updates, hides the selected page, waits for dependent
tasks, restores background priorities and returns to the main menu callback.
Opening creates the selected page, waits for dependencies, uploads glyph tiles
where needed and fades for eight updates. The final callback depends on page
0 through 4. During that fade, forward/back button presses update the queued
direction; these writes are checked separately from the subsequent dispatch.

The setter at `0x02066358` always installs the callback and resets task phase
to zero. Its third argument is **invoke immediately**, not an initial phase.
With a nonzero argument it calls the new callback before returning. This explains
the phase-0 and phase-1 opening calls observed in the same frame. Do not give
that parameter phase semantics when reconstructing callers or building an oracle.

Four completed private replays under `build/runtime/eur_pause_pages/` cover
8,260 frames. `pages65_stores` and `pages86_stores` open and close all five pages
using ordinary inputs at story checkpoints 65 and 86. `alternate65_stores`
repeats them with the guarded chooser-byte fixture described above, restored at
the chooser return in frame 42. `queued65` presses A then B during the opening
fade and covers both queued-direction writes. All four routes end in the field.

Every observed target call is checked: 320 openings, 208 closings, four choosers
and 15 delayed-rumble updates. The oracle derives direct task/workspace writes,
callback arguments, 720 ordered GPU stores and 14,592 OBJ-upload bytes. It checks
1,536 factories, 871 ResourceB attachments, eight pool cleanups and seven watched
task lifetimes through actual removal. Callback changes do not end allocation
tracking. Full live task, work, party, scene, save and graphics records are checked
at 4,049 boundaries; there are no pending calls, live watched tasks or drain frames.

All 109 screenshots and 981 graphics dumps pass artifact validation, and all
104 original save hashes remain unchanged. Ten screenshots and 90 dumps equal
earlier captures over identical state/input prefixes. Checkpoint-65 page screens
at frames 1,240 and 2,100 and each route's final field image were inspected.
The optional host reader matched the original reader across 4,608,234 paused
RAM/DTCM/graphics bytes on each route. Full matching checks, golden packaged ROM,
zero-difference native relink, generated-progress validation and all 81 tests pass.

Two initial probes failed at frame 204: the store hook omitted register-indexed
addressing, then the oracle incorrectly expected scroll-register readback to
equal the submitted value. The corrected probe checks the indexed store and
its ordered value directly. Full display captures remain available; write-only
scroll values are excluded from immutable readback expectations. Both failures
and successful reruns are retained. No game-code correction was needed.

Setup/list/menu/sound/rumble helper effects are observed at return unless explicitly
derived above. The native renderer, physical rumble, other input/flag combinations
and unvisited states remain outside this coverage. Probe sources are private
`make_pause_pages_probe.py`, `pause_pages_flow.py` and composed
`probe_pause_pages.py`; `verify_pause_pages_artifacts.py` validates the reports.

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
