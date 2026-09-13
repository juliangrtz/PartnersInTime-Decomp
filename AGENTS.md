# Working on Partners in Time

## Scope and working priorities

This is a matching C/C++ reconstruction of Mario & Luigi: Partners in Time for
Nintendo DS. The primary target is the European ROM. The intended result is
readable, editable source that reproduces the original game byte for byte.
The decompilation is generated with AI under human direction; preserve the
disclosure in the README and follow [the project policy](docs/AI_USAGE.md).
This file applies throughout the repository. Keep durable instructions here;
put batch results and temporary experiments in the appropriate research log.

Start with [checkout and resources](#start-with-the-current-checkout), then use
[build commands](#toolchain-and-build), [reconstruction rules](#reconstructing-and-integrating-code),
[runtime verification](#runtime-verification), [progress](#progress-and-documentation)
and [Git checks](#private-files-checks-and-git) as needed. The runtime section
includes the tested [save menus](#save-menus), [Smash Eggs](#smash-eggs), [credits](#credits) and
[Nawatobi](#nawatobi) entry routes.

- Follow the current request. A documentation or research task does not start
  another decompilation batch. Honor requests to stop after the next block.
- For continued decompilation, prioritize small, well-understood functions and
  related groups with clear data flow. Use the latest user-requested milestone;
  keep changing targets and coverage in the handoff and progress reports.
- Work systematically from instructions, callers, types and runtime evidence.
  Do not brute-force source permutations or spend hours guessing at register
  allocation. Record a difficult gap and move to another useful target.
- Complete authorized work without repeatedly asking for confirmation. The
  user has authorized intermediate commits and pushes. Give concise updates
  and report actual results, remaining gaps and verification limits.
- Use subagents only when the user or applicable instructions explicitly
  authorize delegation. Old agent names in the session context do not authorize
  restarting their work.

For a reconstruction block, use this sequence:

1. Check the latest request, Git state, current link metadata and relevant handoff.
2. Pick a coherent group with understood native behavior and a reachable runtime
   route. Record and defer gaps that need new evidence.
3. Recover readable source and shared types, then integrate only exact matches.
4. Run the full matching check, package the unmodified ROM and verify its hash.
   Use the native relink and unit suite as required below; regenerate progress
   before tests when linked ranges changed.
5. Replay the relevant save route and check independent RAM/graphics expectations.
   Record artificial fixtures, observed helper outputs and unexercised branches.
6. Update the map and evidence notes, stage owned files, audit the staged content,
   then commit and push the verified block. Leave an accurate handoff at a stop.

## Start with the current checkout

Use the Git repository root for all commands; the terminal's initial directory
may instead contain ROMs and other private files. On the current workstation,
the checkout is `C:\Users\Julian\Desktop\PartnersInTime-Decomp` and the shell is
PowerShell. Quote paths containing spaces.

`D:\NDS\Partners in Time` is the private research workspace, not this checkout.
Its `References/PartnersInTime-Decomp` directory is an older reference clone;
do not edit or push there as a substitute for the active repository. Its root
`AGENTS.md` points here so sessions starting in that workspace can find this file.
The user's `origin` is `https://github.com/juliangrtz/PartnersInTime-Decomp.git`;
`upstream` is a reference remote, not the authorized push destination.

Start a session with these read-only checks:

```powershell
Set-Location -LiteralPath 'C:\Users\Julian\Desktop\PartnersInTime-Decomp'
git status --short
git branch --show-current
git log -5 --oneline
git remote -v
```

Then choose the evidence for the current request:

| Need | Read first |
|---|---|
| Current matching coverage | `docs/progress.json` and `config/eur/arm9/linked_sources.txt` |
| Pending work and deferred candidates | Private `build/analysis/CURRENT_HANDOFF.md`, checked against the current commit |
| Native function, load address and linked range | The component's `symbols.txt` and `delinks.txt`, then original bytes |
| Types, ownership and parameter contracts | Current shared headers, native callers and callee accesses |
| A reproducible gameplay route | The runtime guide and the relevant tested route below |
| A completed batch's evidence | The milestone log and its referenced private probe report |

Do not hard-code the current milestone or percentage into this file. The user's
latest request sets the task; generated coverage describes linked code, and a
private candidate or handoff describes work that may still need integration.

Read `git status --short`, the recent log and the relevant source before editing.
Preserve unrelated changes, including changes left by the user or another tool.
Use `rg` for searches. Keep independent reads parallel where useful, but run
shared build and source-metadata changes sequentially.
In PowerShell, filter files with `rg PATTERN src/overlay006 -g 'title*.c'`
rather than passing an unexpanded wildcard as a directory argument.
Use `rg --files` to locate a header before guessing its name. Keep searches
scoped to the intended file types: positive `-g` patterns admit alternatives,
so adding `-g '*audio*'` alongside C/C++ patterns can also admit enormous private
JSON reports. Use `rg -l` when only filenames are needed.

Read these as needed rather than loading every research log:

- [Source policy](docs/DECOMPILATION_STYLE.md): layout, naming and matching rules.
- [Overlay map](docs/research/OVERLAY_MAP.md): subsystem roles and source links.
- [Progress guide](docs/PROGRESS.md): what counts as matching C/C++.
- [Runtime guide](docs/research/RUNTIME_ANALYSIS.md): saves, inputs and RAM probes.
- [Battle map](docs/research/BATTLE_MAP.md): battle objects and subsystem evidence.
- [VM reference](docs/research/SCRIPT_VM_SEMANTICS.md): decoded commands and
  dispatcher semantics; [Scene VM notes](docs/research/SCENE_VM_MATCHING.md)
  explain its remaining inline-assembly fragment.
- [IDA guide](tools/ida/README.md): imports, disassembly, callers and pseudocode.
- [Data modding](docs/DATA_MODDING.md): text, tables and script source formats.
- [Reassembly plan](docs/REASSEMBLY_PLAN.md): native relinking and remaining work.
- [Reconstruction milestones](docs/BATTLE_MATCHING_MILESTONES.md): completed
  batches, evidence locations and deliberately deferred gaps.

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
`check_overlay15_unit.py` for overlay 15, and `check_main_unit.py` for resident
ARM9. Inspect their inputs and relocation handling before reuse; they are local
conveniences, not required public tools. Use current public build checks as the
final authority.
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

## Toolchain and build

Use Python 3.11+, Ninja, and the compatible Metrowerks ARM compiler installed
under `tools/mwccarm/1.2/base/` (`mwccarm.exe` and `mwldarm.exe`). The compiler
identifies internally as 2.0 build 72. `tools/configure.py --compiler PATH eur`
accepts another compiler root. LLVM's `llvm-mc`, `ld.lld` and `llvm-objcopy` are
also needed for the native reassembly/relink workflow.

If Ninja is missing from `PATH` on this workstation, the installed executable
is `C:\Program Files\JetBrains\CLion 2023.2.2\bin\ninja\win\x64\ninja.exe`.
The known Python executable is
`C:\Users\Julian\AppData\Local\Programs\Python\Python312\python.exe`.
Invoke a quoted executable path with PowerShell's `&` operator. Check tool
availability before assuming that a command or an old workstation path works.

The private input is `extract/baserom_PiT_eur.nds`, game code `ARMP`:

```text
SHA-1: ba4ec2f99b4f2e0047601552bccf00aa73e28701
```

Configure and check source changes from the repository root:

```powershell
python tools/configure.py eur
ninja check
```

For the final unmodified ROM, use the packaging wrapper:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\build_nds.ps1 -DisableDataMods
Get-FileHash -Algorithm SHA1 .\PiT_eur.nds
```

The wrapper produces root-level `PiT_eur.nds` and restores the fixed-layout
secure-area/header checksum fields omitted by the direct `dsd` packaging path.
A successful direct Ninja ROM build alone does not establish the final ROM hash.
Keep `-DisableDataMods` for matching verification; omitting it can package an
existing private `data/eur/project.json` mod.

For a milestone, also verify the native relink and run the unit suite. If linked
code ranges changed, regenerate the progress files first using the commands
under Progress and documentation below. The tests check those files against
the current link metadata.

```powershell
python tools/relink_native.py --version eur --rom extract/baserom_PiT_eur.nds --output-rom build/PiT_eur_native_relinked.nds --require-matching
python -m pytest -q tests
```

The suite includes both unittest classes and pytest functions. Unittest-only
discovery omits tests; keep the explicit `tests` directory so pytest does not
collect duplicate suites from ignored private verification clones. Runtime
input tests require the optional emulator dependency and otherwise skip.

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
[pair motion](src/overlay015/pair_motion.c) for the matching expression.

Use the recovered C++ virtual interface when native calls go through a vtable.
`BattleModel` and its virtual methods are declared in
[battle_scene.h](include/game/battle_scene.h); there is no separate
`battle_model.h`. Its `BattleModelAnimationData` rows are eight bytes: two
frame halfwords followed by a four-byte unknown field. Use that stride when
reading animation ranges in a probe. A C function-pointer approximation can have
the correct size while retaining different call/load ordering. When moving a
unit to C++, check the linkage of every shared C declaration. Use guarded
`extern "C"` declarations
for C APIs, keep C++ class declarations outside them, and verify exported names
and relocations. Do not hide mangled-symbol errors in the comparison script.

Account for integer promotion and the exact point where values are rounded.
A `u8` or `u16` operand promotes to `int`; an explicit `u32` cast before a shift
may be needed to reproduce a native logical shift. Signed division truncates toward
zero, which differs from an arithmetic right shift or Python's `//` for negative
values. Preserve the native order of division and Q12 scaling, and model signed
division explicitly in runtime oracles. See the verified examples in
[title model entry](src/overlay006/title_sequence_model.c) and
[trail drawing](src/overlay006/title_trail.c).

Preserve reads across callbacks in their original order: a linked-list callback can
change `next`, and cleanup can change state. Do not cache those fields earlier
merely to simplify the source. Distinguish deleting and non-deleting destructor
entries when reconstructing virtual calls and reporting runtime coverage.

Overlay 5's `func_ov005_0206650c` requests deferred removal by setting mask
`0x0002` in the element's 16-bit flags at offset `+0x08`. It does not immediately
free the object. Model this write separately from later list cleanup; see
[element lifecycle](src/overlay005/element_lists.c). Title and credits tasks
share the 72-byte `MenuElement` view in
[the frontend header](src/overlay006/frontend_scene_internal.h). Reuse it instead
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

Size request buffers from callee accesses, not just the apparent base type.
`ArchiveReadRequest` is 40 bytes, but `BattleArchive_ReadAsync` also writes the
halfword at offset 40, including on the raw-read path. The title loader uses a
44-byte extended raw request; a full `ArchiveCompressedRequest` is 64 bytes.
See [the shared layout](include/game/archive_io.h),
[the callee](src/game/archive_compressed.c), and
[the title loader](src/overlay006/title_animation_resources.cpp).
Preserve ownership flags and the native allocator/free pairing for converted
resource tables; a non-null pointer alone does not establish ownership.

The title sequence allocates 59,340 bytes but clears only its 57,288-byte prefix.
The 512 bytes at allocation offset 57,292 are a 32x32 4bpp trail stamp; older
initializer notes incorrectly called them palette data. The rasterizer establishes
their purpose. Its two 24,576-byte destination buffers are linear pixels before
tiled VRAM upload, and the second screen uses a sequence-space y-origin of 244.
Do not infer a 192-pixel screen separation from the display height. Refer to
[the sequence layout](src/overlay006/title_sequence_internal.h) and the trail
module when interpreting these captures; the remaining allocation tail is unknown.

The sequence's 25 backdrops use a 52-byte record with a 48-byte model prefix,
signed 16-bit horizontal velocity and a four-bit group. Their four groups have
4/5/8/8 elements. Construction, release and skip hooks are linked in
[title backdrop initialization](src/overlay006/title_backdrop_init.c); check
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
[credits positions](src/overlay006/credits_positions.c),
[shared declarations](src/overlay006/credits_transition_internal.h), and
[motion setup](src/overlay006/credits_motion.c). Keep native integer operation
order and narrowing when modeling acceleration or easing; floating-point
interpolation is not an equivalent oracle.

Credits effects reuse the motion record's remaining parameter words for
different purposes. Keep shared names neutral until all consumers justify a
single meaning. Illustration loading uses a 32,768-byte texture staging buffer
and a 512-byte palette; the task schedules sixteen texture chunks and a palette
upload before clearing the workspace's `image_loading` field at `+0x8238`.
The current layouts and buffer declarations are in the shared headers above;
the state machine is in [credits image loading](src/overlay006/credits_image_loader.c).
Checking those queued jobs does not establish coverage of the native upload
callbacks themselves.

Small, explained inline-assembly fragments are authorized when a specific
instruction sequence cannot reasonably be reproduced in C. Keep the surrounding
logic readable and verify the entire function. Do not disguise raw instruction
dumps as C to inflate progress. The completed Scene VM's 64-byte height-conversion
fragment is a documented example in [its matching notes](docs/research/SCENE_VM_MATCHING.md).
All four VM dispatchers are already linked and byte-identical (53,700 bytes);
their callees still contain work. Check the [VM reference](docs/research/SCRIPT_VM_SEMANTICS.md)
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
[element list](src/overlay005/element_lists.c) and
[resource owner](src/overlay005/resource_owner.c) behavior before asserting that
the entire source or task record stayed unchanged.

An oracle failure may be a wrong expectation. Inspect the native helper before
changing matching game code: for example, `func_02036988` always sets blend-mode
bit `0x40` and writes BLDCNT/BLDALPHA together as a 32-bit value. Read back the
actual register width. Validate raw archive payloads against the extracted
archive; compressed-request completion and size checks alone do not establish
that the decoded bytes were independently verified.

Derive asynchronous completion from the actual queue or loader field. For
example, `GameAudioLoader.active` is at `+0x20`; `+0x1C` is its next pointer.
Check [the audio layout](include/game/audio.h) and the native access before using
either in a probe. A nonzero pointer can look like a plausible busy flag.
After correcting an oracle, rerun the full affected replay and retain the
successful evidence separately; do not count the failed attempt as validation.

Keep probe work bounded. Check and release completed per-node snapshots during
long list traversals instead of rechecking all earlier nodes after every call.
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
[rumble control](src/game/rumble_control.cpp).
This is an object-relative RAM field, not a universal battery-save file offset.

The debug menu can teleport without fully initializing the destination state.
Controlled RAM edits or temporary decoded-command substitutions are useful
probes, but record exactly what changed and when it was restored. They do not
demonstrate normal gameplay accessibility. Prefer read-only observation after
the controlled setup. Never infer complete branch coverage from a matching ROM.

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
[attack layout](include/game/overlay015_attack.h) and native allocation/caller
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
[the script manager](src/overlay007/scene_script_manager.c). Its callback phases
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

## Progress and documentation

After coverage changes, run:

```powershell
python tools/generate_progress.py
python tools/generate_progress.py --check
```

Matching C/C++ counts linked, verified high-level code ranges. Maintained symbolic
assembly is separate; ARM7 mixed code/data, assets, BSS and placeholder overlays
are excluded from the headline denominator. A byte-identical reassembly is not
a claim of 100% decompilation. Read current totals from `docs/progress.json`.

Update the overlay map and the relevant subsystem notes when coverage or a
confirmed role changes. Keep the README short; put function-level history in
the milestone documents. Use concrete facts and exact matching results, without
vague similarity claims, inflated completion language or speculative names.
Distinguish code-derived facts, runtime observations and unresolved questions.

`docs/BATTLE_MATCHING_MILESTONES.md` contains a legacy non-UTF-8 byte. Inspect its
encoding before editing and preserve existing bytes; do not decode with replacement
and rewrite the entire file. The similarly named document under `docs/research/`
is a different, older log. Reuse private probe scripts as references, but inspect
old integration scripts before running them: many already-applied scripts are
not safe to replay.

## Private files, checks and Git

Keep ROMs, extracted assets/text/scripts, raw machine-code dumps, proprietary
compilers, IDA databases, saves, screenshots and RAM/VRAM captures out of Git.
Use ignored `extract/`, `data/eur/`, `tools/mwccarm/`, `private/` and `build/`
locations as described in [the private-content rules](docs/LOCAL_PRIVATE_CONTENT.md).
Do not force-add ignored private material.

Before committing, run `git diff --check` and
`python tools/check_public_content.py`, and inspect the staged diff. For source
changes, also complete the matching and runtime checks above. Tool changes need
relevant unit tests. Documentation-only changes need link/content checks, not
a full ROM build or emulator run. Report unavailable checks explicitly.
The public-content audit reads Git's index, so run it after staging new files
as well. Check `git diff --cached --check` and the staged path list before
committing; an audit run before staging cannot inspect an untracked new file.

Commit coherent, verified intermediate blocks and push them to `origin` without
asking again. The current working branch is `main`; verify the checkout and
remote before pushing. Stage explicit task-owned paths, never the whole dirty
tree. Use the configured Git identity and descriptive commit messages. History
rewriting and attribution changes are separate tasks, not routine batch cleanup.
Finish with the commit ID, push status, changed coverage when applicable, and
the checks that actually ran.

At a stop or handoff, record the last pushed commit, task-owned pending files,
completed checks, checks still due and difficult candidates deliberately deferred.
Keep pending matching work distinct from committed and pushed progress so the
next session can resume without rerunning old integration scripts.
Record deferred candidates with their component/address, native and candidate
sizes, remaining mismatch class and the evidence needed to justify another
attempt. Keep these changing details in the handoff or research log, not here.
