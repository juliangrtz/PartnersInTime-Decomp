# Working on Partners in Time

This repository reconstructs Mario & Luigi: Partners in Time as readable,
editable C/C++ that reproduces the European Nintendo DS game byte for byte.
The decompilation is fully generated with generative AI under human direction.
Preserve the README disclosure and [AI policy](docs/AI_USAGE.md).
These instructions apply throughout the repository.

## Priorities and scope

- Follow the latest user request and requested stopping point. Documentation or
  research requests do not start another decompilation batch. A handoff is
  context, not permission to resume a superseded task.
- For decompilation, prefer fast, well-understood progress in coherent groups.
  Readability and exact matching both matter. Use instructions, callers, shared
  types and runtime evidence; do not brute-force source permutations or spend
  hours guessing at register allocation. Record hard gaps and move on.
- Small, explained inline-assembly fragments are authorized where necessary.
  Verify the complete function and disclose the fragment. Coverage follows
  [the existing metric](docs/PROGRESS.md), which tracks symbolic assembly separately.
- Complete authorized work without repeated confirmation requests. Intermediate
  commits and pushes to the user's `origin` are authorized. Give concise progress
  updates and state what was actually verified.
- Use subagents only when the user or applicable instructions explicitly authorize
  delegation. Old agent names in session context do not authorize restarting them.

## Start in the correct checkout

The current checkout is `C:\Users\Julian\Desktop\PartnersInTime-Decomp`.
`D:\NDS\Partners in Time` is the private research workspace; its root `AGENTS.md`
points here. Its `References/PartnersInTime-Decomp` is an older reference clone,
not the active source or push destination. Verify paths on a different machine.

Run these read-only checks before editing:

```powershell
Set-Location -LiteralPath 'C:\Users\Julian\Desktop\PartnersInTime-Decomp'
git status --short
git branch --show-current
git log -5 --oneline
git remote -v
```

The user's remote is `https://github.com/juliangrtz/PartnersInTime-Decomp.git`.
`upstream` is for reference. Verify the branch before pushing; work has used `main`.
Preserve unrelated changes and use the configured Git identity. Do not rewrite
history or change attribution as routine housekeeping.

Use `rg --files` to locate files, then scoped `rg` searches. In PowerShell, quote
paths with spaces and use `-g 'pattern'` rather than passing an unexpanded wildcard
as a directory. Batch independent reads where useful; serialize shared builds,
metadata edits and emulator runs that share a ROM/save sidecar.
Parse large JSON reports and print selected records, not entire diff dumps.

## Find the relevant evidence

| Need | Resource |
|---|---|
| Working-tree coverage | [Generated progress](docs/progress.json), [counting rules](docs/PROGRESS.md); check Git for unpublished changes |
| Current pending work | Private `build/analysis/CURRENT_HANDOFF.md`; compare its commit with Git |
| What is actually linked | `config/eur/arm9/linked_sources.txt` |
| Function boundaries and load addresses | Resident `config/eur/arm9/{symbols,delinks}.txt`; overlays `config/eur/arm9/overlays/ovNNN/` |
| Source organization and confirmed subsystem roles | [Source policy](docs/DECOMPILATION_STYLE.md), [overlay map](docs/research/OVERLAY_MAP.md), [battle map](docs/research/BATTLE_MAP.md) |
| VM semantics and remaining assembly | [Script VM reference](docs/research/SCRIPT_VM_SEMANTICS.md), [Scene VM notes](docs/research/SCENE_VM_MATCHING.md) |
| Native inspection and relinking | [IDA guide](tools/ida/README.md), [reassembly plan](docs/REASSEMBLY_PLAN.md) |
| Runtime tools and save navigation | [Runtime guide](docs/research/RUNTIME_ANALYSIS.md) |
| Detailed matching lessons and tested scene routes | [Reconstruction reference](docs/research/RECONSTRUCTION_NOTES.md) |
| Previous batch evidence | [Milestone log](docs/BATTLE_MATCHING_MILESTONES.md) and its private reports |
| Assets, script editing and publication boundaries | [Data modding](docs/DATA_MODDING.md), [private-content rules](docs/LOCAL_PRIVATE_CONTENT.md) |

Read the relevant reference section before working on its subsystem. Current
native bytes, source, link metadata and fresh checks take precedence over old
notes. Private candidates are not linked progress; an old successful probe is
not proof that today's source or artifact was tested. Keep changing percentages,
milestones and candidate status in generated reports and the private handoff.
When resuming a partially integrated batch, inventory its pending files and
completed checks first. Associate each check with the source and ROM it tested.
A documentation commit must not include pending source or regenerate progress
to claim bytes whose verification is still incomplete.
If the generated reports are dirty, use `git show HEAD:docs/progress.json` for
committed coverage and inspect the verified remote-tracking revision for pushed
coverage. A local remote-tracking ref can be stale; confirm a push before calling
new bytes published. A progress target from an earlier task remains context
until the latest request actually asks to resume reconstruction.

Private original bytes are in `extract/eur/arm9/` and
`extract/eur/arm9_overlays/`; IDA databases are in `build/ida/` and experiments
in `build/analysis/`, including `local_decompiler/`. The EUR resident `arm9.bin`
starts at `0x02004000`. Derive offsets from component metadata, not from the
start of main RAM. CPU and overlay identity matter because addresses are reused.

## Reconstruct and integrate

1. Check boundaries, original instructions, callers and neighbors. Treat
   pseudocode as a hypothesis: it can omit stack arguments, FIFO stores and
   cached loads, or invent return values. Pick targets with understood data flow
   and a reproducible runtime route.
2. Recover widths, signedness, truncation points, ownership and virtual interfaces.
   Put shared layouts/prototypes in headers; use neutral names for unknowns and
   compile-time size checks. A known prefix's size does not prove allocation size.
   Preserve integer promotion, signed division toward zero, overflow behavior,
   Q12 operation order and reads across callbacks. A narrow return type does not
   imply a narrow accumulator; preserve where the native code truncates. Check
   null-record paths before replacing them with early returns.
   Track signedness per expression: an unsigned value divided by an `int`
   divisor uses unsigned division, while that divisor's own `/= 10` stays signed.
   Preserve repeated source reads around destination writes when the pointers
   can alias; `const` does not establish non-overlap.
   Prefer the actual shared workspace type over casting a raw byte global to a
   partial structure. Preserve existing byte views when naming newly understood
   fields, check offsets and size, and rebuild every affected caller.
3. Keep related contiguous functions in one subsystem module. Temporary isolated
   units are acceptable around native gaps; consolidate when those gaps close.
   Source basenames must be globally unique because MW's linker selects by
   basename. This compiler usually emits functions in reverse source order;
   verify the resulting order. Keep C declarations at block starts and C APIs
   guarded by `extern "C"` when called from C++.
   For C headers without their own linkage guard, their first inclusion must be
   inside `extern "C"`; wrapping a later include cannot undo an include guard.
4. Compare sizes, every instruction, relocations and symbol layout. Classify
   differences before editing. Do not change compiler flags, weaken checkers or
   add arbitrary casts/volatile accesses to obtain a match. Recompile private
   drafts against current headers; a missing symbol must never fall back to
   original bytes and count as a passing candidate.
   Associate comparisons with the exact source, language mode and object:
   private checkers can overwrite address-named dumps from an earlier candidate.
   Compare an already matching counterpart when one exists before changing
   declaration lifetimes or pointer increments. Separate increments versus
   postincrements, and keeping a returned structure versus discarding it, can
   change this compiler's instructions or stack layout. These are evidence to
   explain a specific difference, not a recipe for enumerating source variants.
5. Integrate exact matches into `linked_sources.txt` and the component metadata.
   Update affected declarations and maintained assembly references together.
   Preserve interior entry points with `config/eur/arm9/linker_aliases.json`;
   check `reasm/eur/patches.json` when renaming symbols.
6. Complete the checks below, update evidence/progress/map, then commit and push
   the coherent batch. Record deferred gaps instead of repeatedly retrying the
   same register mismatch without new evidence.

See the [detailed matching lessons](docs/research/RECONSTRUCTION_NOTES.md#reconstructing-and-integrating-code)
for concrete ABI, layout, aliasing, graphics and compiler examples.

## Build and verification

Use Python 3.11+, Ninja and the compatible Metrowerks tools in
`tools/mwccarm/1.2/base/` (internally 2.0 build 72). LLVM's `llvm-mc`, `ld.lld`
and `llvm-objcopy` support native relinking. Keep the configured ABI and flags.
On this workstation, known executable paths are:

```text
C:\Users\Julian\AppData\Local\Programs\Python\Python312\python.exe
C:\Program Files\JetBrains\CLion 2023.2.2\bin\ninja\win\x64\ninja.exe
```

If a tool is absent from `PATH`, verify and invoke its quoted path with
PowerShell's `&` operator. The private ROM is `extract/baserom_PiT_eur.nds`,
game code `ARMP`, SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

For a completed reconstruction batch, run from the repository root:

```powershell
python tools/configure.py eur
ninja check
powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\build_nds.ps1 -DisableDataMods
Get-FileHash -Algorithm SHA1 .\PiT_eur.nds
python tools/relink_native.py --version eur --rom extract/baserom_PiT_eur.nds --output-rom build/PiT_eur_native_relinked.nds --require-matching
python tools/generate_progress.py
python tools/generate_progress.py --check
python -m pytest -q tests
```

Every C/C++ or shared-header change needs the full `ninja check`. The packaging
wrapper restores checksum fields omitted by direct packaging; verify the final
ROM hash, not just Ninja's exit status. Keep `-DisableDataMods` to exclude a
private data mod. Native relinking must report zero differing bytes. Regenerate
progress before tests when linked ranges changed. Use pytest with the explicit
`tests` directory; unittest-only discovery misses tests and unrestricted pytest
can collect ignored private clones. Report skipped or unavailable checks.
Do not rebuild or replace a ROM while an emulator is reading that path. A native
relink to a separate output path can run alongside a replay of the packaged ROM.

Documentation-only changes need content/link and Git checks, not a ROM build
or emulator replay. Tool changes need the relevant tests. Repeat checks only
when new changes, failures or unresolved concerns justify it.

## Runtime verification

Use `tools/runtime_drive.py` for deterministic DS inputs, `runtime_probe.py` for
hooks/registers/memory/overlays and `runtime_session.py` for interactive navigation
and compatible snapshots. Optional dependencies are in
`tools/runtime_requirements.txt`. Read the tools' arguments before reuse.

- The current story collection is `PiT_SaveStates/`; old messages mention
  `PiT/_SaveStates`. Enumerate actual files. A `.sav` is a battery save, not an
  emulator snapshot. Preserve originals and compare their hashes before/after.
- `py-desmume==0.0.9` embeds DeSmuME 0.9.12. Supplied 0.9.13 snapshots can show
  plausible RAM but hang or render incorrectly when advanced. Cold boot battery
  saves to make compatible states; do not keep advancing a known-bad snapshot.
- Store reports, snapshots and captures under ignored `build/runtime/`. Run
  replays sequentially when they share ROM or battery-backup paths.
- Reuse a tested route, but monitor the current function group and the helpers
  needed to verify it. Avoid retaining every earlier per-frame oracle in each
  new probe. Keep prior evidence and state which checks were actually repeated.
  Read the driver's action semantics: `runtime_drive.py` inserts one released
  frame after each action. Bound scene entry and confirm its dispatch hook ran.
- Guard each hooked function's complete native range and the loaded overlay;
  a matching prologue alone is insufficient. Match nested returns using the
  return address and entry stack pointer. Derive expected memory changes
  independently; label helper outputs that are merely observed. Do not read
  freed objects or interpret unloaded overlay addresses as the previous function.
- For constructors, snapshot the actual allocation before entry and derive only
  the fields the native code initializes; preserve untouched bytes and padding.
  A base constructor may run inside a larger derived allocation. Propagate a
  nested constructor's independently expected changes into the parent's oracle,
  rather than accepting a fresh RAM snapshot as the expected result.
- Distinguish allocation size, initialized extent and transfer size. When the
  allocation is known, check the complete buffer and preserve untouched tails,
  padding and transparent pixels. Shop help pixels allocate 6,144 bytes, clear
  5,120 and upload 4,096; derive these extents from the allocator and consumers.
  For graphics, derive expected pixels or nibbles independently of the native
  packed-word loops, including stride, palette offset and linear/tiled layout.
- Distinguish base destruction, derived destruction and deleting entry points.
  Check the final object state before the heap free; after release, check only
  return values and still-live records. Check cleared scene globals before the
  next overlay reuses their addresses. Record each exercised entry point; a
  virtual delete does not establish coverage of every destructor wrapper.
  Task removal can be deferred: setting the removal flag is a separate event
  from unlinking and freeing. Check the actual helper before assigning lifetimes.
  Follow newly created tasks through their updates to the expected completion.
  Derive update counts from integer step, delay and clamping; division by six
  does not guarantee six updates when truncation leaves a remainder.
- Verify RAM, mapped VRAM, palettes, OAM, ordered GPU stores and visible behavior
  as appropriate. A screenshot or passing ROM hash alone is insufficient.
  Hardware register readback need not equal the earlier submitted FIFO command
  or affine origin. Record ROM/save/state hashes, inputs and uncovered branches.
- Model native arithmetic widths in Python oracles. Wrap a native 32-bit
  intermediate before its signed shift; Python integers do not overflow.
  In the zero-scale affine path, `0x100000 * 4096` wraps to zero before `>> 8`.
  Recover all stack arguments as well as register arguments: the display affine
  helper takes nine arguments. Use its verified zero-scale behavior, not an
  idealized mathematical transform. See the reconstruction reference for details.
- Check per-function and per-branch counts across the route set. A valid replay
  can miss a helper entirely; retained screenshots and equal artifact hashes do
  not establish execution coverage. Report deliberate RAM fixtures separately
  and distinguish historical helper evidence from checks repeated in this batch.
- At replay end, stop admitting new outermost calls and drain pending calls and
  their nested helpers for a bounded number of neutral frames. Do not discard
  unfinished calls to make a probe pass. After correcting an oracle, rerun it.
  An oracle failure can be a wrong expectation; inspect native instructions and
  helper effects before changing already matching game code.
- The debug menu teleports without necessarily initializing a complete state.
  Separate normal navigation from controlled RAM/decoded-command fixtures.
  Memory instructions must give ROM region, CPU/address space, dereferences,
  field offset, access width, guard, timing and whether/when to restore the edit.

Consult tested routes for [shops](docs/research/RECONSTRUCTION_NOTES.md#shops),
[save/load menus](docs/research/RECONSTRUCTION_NOTES.md#save-menus),
[Game Over](docs/research/RECONSTRUCTION_NOTES.md#game-over),
[Smash Eggs](docs/research/RECONSTRUCTION_NOTES.md#smash-eggs),
[credits](docs/research/RECONSTRUCTION_NOTES.md#credits) and
[Nawatobi](docs/research/RECONSTRUCTION_NOTES.md#nawatobi).
For the tested EUR Nawatobi entry, the phase is the little-endian 32-bit word
at `read32(0x0208E1E0) + 0x30` in ARM9 main RAM, with overlay 7 loaded.
At pause update `0x02071F80`, require a fully initialized pause menu, the native
byte guard, `read32(0x0208E1E0) == r0`, vtable `read32(r0) == 0x0208D9B8`
and phase 2; write 7 once, disable the hook and resume. Do not overwrite the
pointer at `0x0208E1E0` or freeze the phase. The linked reference explains the
snapshot and evidence; controlled access does not establish a normal story
entry or natural completion.

## Documentation, Git and handoff

Keep the README short and human-readable. Update the overlay map when linked
coverage or confirmed subsystem roles change. Separate matching C/C++ from
maintained symbolic assembly; a byte-identical reassembly is not 100%
decompilation. Avoid vague similarity claims, inflated completion language and
speculative names. Distinguish code-derived facts, observations and unknowns.

`docs/BATTLE_MATCHING_MILESTONES.md` has a legacy non-UTF-8 byte and mixed CRLF/LF.
Preserve existing bytes and append as bytes; account for Git line-ending
normalization when comparing blobs. Do not decode with replacement and rewrite
it. The similarly named file under `docs/research/` is an older, different log.

Keep ROMs, assets, saves, proprietary tools, IDA databases, machine-code dumps
and captures out of Git. Use the ignored private directories; never force-add
private material. Before committing:

1. Review `git diff --check` and stage explicit task-owned paths only.
2. Inspect `git diff --cached`, `git diff --cached --check` and the staged path list.
3. Run `python tools/check_public_content.py` after staging: it audits the index.
4. Commit with a descriptive message and push to the verified user remote.
   Report the commit, push status, actual checks and coverage change if applicable.

At a stop, leave the last pushed commit, owned pending files, completed/due checks
and deferred candidates in private `build/analysis/CURRENT_HANDOFF.md`. For each
gap record component/address, native/candidate sizes, mismatch class and evidence
needed for another attempt. Distinguish pending, committed and pushed work.
For private units containing several functions, record exactness per function;
one matching helper does not validate its neighbors. Keep proposed names and
planned runtime checks explicitly provisional until their evidence exists.
Record each replay's completion separately, including corrected oracle failures
and the successful rerun. One passing route does not complete a planned suite.
Separate the latest documentation commit from the last verified code batch;
existing build logs are historical evidence, not checks run by the current turn.
Inspect old integration scripts before reuse; many are not safe to replay.
Keep durable rules here and detailed findings in the linked reference, rather
than appending every batch's history to this entry point.
