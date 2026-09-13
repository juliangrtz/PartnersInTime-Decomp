# Working on Partners in Time

Reconstruct Mario & Luigi: Partners in Time as readable, editable C/C++ that
reproduces the European Nintendo DS game byte for byte. This decompilation is
fully generated with generative AI under human direction. Preserve the README
disclosure and [AI policy](docs/AI_USAGE.md).

These instructions apply throughout the repository. Detailed addresses, compiler
examples and tested routes live in the
[reconstruction reference](docs/research/RECONSTRUCTION_NOTES.md).

## Priorities and scope

- Follow the latest user request and requested stopping point. Research or
  documentation requests do not start another reconstruction batch. A handoff
  provides context; it does not authorize resuming a superseded task.
- Prefer fast, well-understood progress in coherent groups. Readability and
  exact matching both matter. Use native instructions, callers, shared types
  and runtime evidence. Do not brute-force source permutations or spend hours
  guessing at register allocation. Record difficult gaps and move on; revisit
  them only when new evidence explains the remaining difference.
- Small, explained inline-assembly fragments are authorized when necessary.
  Verify the whole function and disclose the fragment. Do not wrap machine-code
  dumps in C to inflate progress or mix symbolic ASM into the C/C++ percentage.
- Complete authorized work without repeated permission requests. Verified
  intermediate commits and pushes to the user's `origin` are authorized.
  Give concise progress updates and report what was actually checked.
- Use subagents only when the user or applicable instructions explicitly
  authorize delegation. Old agent names in session context are not authorization.

## Start in the correct checkout

The active checkout on this workstation is:
`C:\Users\Julian\Desktop\PartnersInTime-Decomp`.
`D:\NDS\Partners in Time` holds private research and points here through its
own `AGENTS.md`. Its `References/PartnersInTime-Decomp` is an older reference
clone. Verify these paths on another machine.

Before editing:

```powershell
Set-Location -LiteralPath 'C:\Users\Julian\Desktop\PartnersInTime-Decomp'
git status --short
git branch --show-current
git log -5 --oneline
git remote -v
```

The user's remote is `https://github.com/juliangrtz/PartnersInTime-Decomp.git`;
work has used `main`. `upstream` is for reference. Preserve unrelated changes,
use the configured Git identity, and stage explicit task-owned paths. History
rewriting and attribution changes are separate tasks, not routine housekeeping.

Read private `build/analysis/CURRENT_HANDOFF.md` if present, then verify its
claims against Git, source and artifacts. It can be stale even at the same
`HEAD`: another private draft or uncommitted batch may have appeared afterward.
Inventory pending files and check provenance before reusing an integration script.
An object file's existence or timestamp does not prove its comparison passed.
If the previous command's output was lost, record its result as unverified until
the relevant check is recovered or rerun; preserve the draft in the meantime.

Use `rg --files` and scoped `rg` searches. Quote PowerShell paths containing
spaces; use `-g 'pattern'` for file globs. Parse large reports and print selected
records. Serialize builds, metadata edits and replays that share ROM/save paths.

## Find the relevant evidence

| Need | Resource |
|---|---|
| Coverage and counting rules | [Progress JSON](docs/progress.json), [metric](docs/PROGRESS.md) |
| Source actually used in the ROM | `config/eur/arm9/linked_sources.txt` |
| Boundaries, load addresses and references | Resident `config/eur/arm9/{symbols,delinks,relocs}.txt`; overlays `config/eur/arm9/overlays/ovNNN/`; candidate object symbols and relocations |
| Source organization and subsystem roles | [Source policy](docs/DECOMPILATION_STYLE.md), [overlay map](docs/research/OVERLAY_MAP.md), [battle map](docs/research/BATTLE_MAP.md) |
| VMs | [Script VM semantics](docs/research/SCRIPT_VM_SEMANTICS.md), [Scene VM matching](docs/research/SCENE_VM_MATCHING.md) |
| Native inspection and relinking | [IDA guide](tools/ida/README.md), [reassembly plan](docs/REASSEMBLY_PLAN.md) |
| Emulator tooling and navigation | [Runtime guide](docs/research/RUNTIME_ANALYSIS.md), [tested routes](docs/research/RECONSTRUCTION_NOTES.md#runtime-verification) |
| RAM roots, object extents and graphics ranges | [EUR memory reference](docs/research/RECONSTRUCTION_NOTES.md#eur-memory-reference) |
| ABI and compiler lessons | [Reconstruction reference](docs/research/RECONSTRUCTION_NOTES.md#reconstructing-and-integrating-code) |
| Pause transitions | [Party lifecycle](docs/research/RECONSTRUCTION_NOTES.md#pause-party-initialization-and-cleanup), [transition evidence](docs/research/RECONSTRUCTION_NOTES.md#pause-transition-panels-and-controllers), [projection ABI](docs/research/RECONSTRUCTION_NOTES.md#pause-transition-projection-and-callback-abi), [setup calls](docs/research/RECONSTRUCTION_NOTES.md#pause-transition-setup-calls) |
| Pause navigation | [Page entry and return](docs/research/RECONSTRUCTION_NOTES.md#pause-page-entry-and-return), [main menu and member selection](docs/research/RECONSTRUCTION_NOTES.md#pause-main-menu-and-member-selection), [status and Cobalt Star pages](docs/research/RECONSTRUCTION_NOTES.md#pause-status-and-cobalt-star-pages) |
| Pause list rendering | [Row sprites](docs/research/RECONSTRUCTION_NOTES.md#pause-list-row-sprites), [queued drawing and markers](docs/research/RECONSTRUCTION_NOTES.md#pause-queued-row-drawing-and-markers), [row refresh](docs/research/RECONSTRUCTION_NOTES.md#pause-list-row-refresh) |
| Equipment list models | [Equipped-item markers](docs/research/RECONSTRUCTION_NOTES.md#pause-equipped-item-markers), [empty-equipment row sprites and ResourceA lifetime](docs/research/RECONSTRUCTION_NOTES.md#pause-empty-equipment-row-sprites) |
| Unused Nawatobi minigame | [Guarded entry, exact RAM edit and research limits](docs/research/RECONSTRUCTION_NOTES.md#nawatobi) |
| Earlier batch evidence | [Milestone log](docs/BATTLE_MATCHING_MILESTONES.md); private reports linked there |
| Assets and publication boundaries | [Data modding](docs/DATA_MODDING.md), [private-content rules](docs/LOCAL_PRIVATE_CONTENT.md) |

Native bytes are private under `extract/eur/arm9/` and
`extract/eur/arm9_overlays/`; IDA databases live in `build/ida/`, and experiments
in `build/analysis/`, including `local_decompiler/`. The EUR resident `arm9.bin`
loads at `0x02004000`, not the start of main RAM. Derive offsets from component
metadata and identify the CPU and overlay before interpreting an address.

Private scripts, snapshots and reports are local conveniences, not fresh-clone
dependencies. Check availability, inputs and source/ROM provenance before reuse.
If absent, establish the needed evidence with public tools and user-supplied
ROM/saves. Current native bytes, source, metadata and fresh checks take precedence
over historical notes. Check existing declarations before accepting a handoff's
claim that a shared prototype needs changing.

When checking candidate inventories, compare component and address ranges against
the current linked manifest and delinks. Accept variable whitespace in metadata;
ignore blank/comment manifest entries and unnamed component-wide section headings.
Otherwise an inventory can silently miss linked ranges or count the entire
component as owned. Confirm a proposed new range directly before reconstructing
or counting it; historical `EXACT` records are only discovery leads.

## Reconstruct and integrate

1. Inspect the complete native range, callers and neighbors. Pseudocode can
   omit stack arguments, repeated FIFO stores and cached loads, or invent return
   values. Check literal pools too: disassemblers can stop at data or decode it
   as instructions. Choose a group with understood behavior and a usable route.
2. Recover field widths, signedness, offsets, allocation sizes, ownership and
   virtual interfaces. Preserve arithmetic widths, integer promotion, division
   toward zero, Q12 operation order and exact truncation points. Derive each
   comparison's signedness separately. A byte/halfword access, including a load
   from a stack argument, does not establish a narrow parameter. Check caller-side
   extensions, stack and hidden ABI arguments. A register left over from a previous
   call is not an argument unless the next callee consumes its incoming value.
3. Preserve load/store order, short-circuit calls, possible aliasing and accesses
   across callbacks. `const` does not establish non-overlap. Keep native masked
   and unmasked stores, packed-field truncation and neighboring bits. Do not
   simplify apparently redundant operations without explaining the instructions.
4. Use existing shared records and workspace types; retain compatible raw views.
   Add size/offset checks and neutral names for unknowns. A prefix's `sizeof`
   does not prove the allocation size. Search all declarations before changing
   shared layouts or signatures, then migrate and rebuild affected callers together.
   Preserve verified interior data aliases and native table strides. Task payloads
   can reuse one offset for different phases; model that reuse explicitly and
   check every transition's initialization before giving the field one meaning.
5. Keep related contiguous functions in a subsystem module. Temporary isolated
   units are acceptable around native gaps; consolidate when those gaps close.
   Basenames must be globally unique because MW's linker selects by basename.
   This compiler generally emits functions in reverse source order; verify it.
   Keep C declarations at block starts without hoisting assignments across native
   sequence points. Guard C APIs with `extern "C"` in C++; a header without its
   own linkage guard must first be included inside that block.
6. Compare size, every instruction, pools, relocations and symbol layout.
   Classify the difference before editing. Keep compiler flags and ABI fixed;
   do not weaken checks or introduce arbitrary casts/volatile accesses to match.
   A scoped volatile access requires evidence of that native access pattern.
   Recompile drafts against current headers and associate each comparison with
   the actual source, language mode and object. Address-named dumps may be overwritten.
7. Inspect comparison text as well as exit status: some private checkers return
   success while reporting `DIFFERENT`. Equal sizes do not establish a match.
   Unknown relocations, missing candidate sections or unresolved functions must
   fail; never substitute original bytes for a missing candidate. Validate local
   data contents and ownership before mapping its symbol to a native address.
   A checker may resolve an address-named helper that has since been renamed.
   Use its current public declaration; address resolution does not prove the
   public linker can resolve that obsolete name.
8. Integrate exact functions into the manifest and component metadata. Update
   declarations and maintained ASM references together. Preserve interior entry
   points through `config/eur/arm9/linker_aliases.json`; check
   `reasm/eur/patches.json` when renaming symbols. Include compiler-emitted local
   data with its actual binding and ownership, but do not count it as code.
9. Complete the build and relevant runtime checks, update progress/evidence/map,
   then publish the coherent batch. A matching private neighbor is not linked
   progress, and an already linked helper contributes no new bytes when moved.

See the reference's compiler examples before attempting syntax or declaration
changes. Separate switch arms, initialization order, bitfields and pointer
increments can explain a specific mismatch; they are not a search space to enumerate.

## Build and verification

Use Python 3.11+, Ninja and compatible Metrowerks tools in
`tools/mwccarm/1.2/base/` (internally 2.0 build 72). Native relinking also uses
LLVM's `llvm-mc`, `ld.lld` and `llvm-objcopy`. Known workstation executables:

```text
C:\Users\Julian\AppData\Local\Programs\Python\Python312\python.exe
C:\Program Files\JetBrains\CLion 2023.2.2\bin\ninja\win\x64\ninja.exe
```

If missing from `PATH`, verify the path and invoke it with PowerShell's `&`.
The private EUR ROM is `extract/baserom_PiT_eur.nds`, game code `ARMP`, SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.

For a completed reconstruction batch, from the repository root:

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
ROM hash. Keep `-DisableDataMods` to exclude private data modifications. Native
relinking must report zero differing bytes. Regenerate progress before tests
when linked ranges change; use the explicit `tests` directory to avoid private
clones and incomplete unittest-only discovery.

Do not replace a ROM while an emulator reads that path. A native relink to a
separate output can run beside a replay. Documentation-only work needs content,
link and Git checks; tool changes need relevant tests. Repeat checks when changes,
failures or unresolved concerns warrant it. Report unavailable/skipped checks.

## Runtime verification

Use `tools/runtime_drive.py` for deterministic inputs, `runtime_probe.py` for
hooks/registers/memory/overlays, and `runtime_session.py` for navigation and
compatible snapshots. Read their arguments and
[runtime guidance](docs/research/RECONSTRUCTION_NOTES.md#runtime-verification).

- Enumerate `PiT_SaveStates/`; older messages call it `PiT/_SaveStates`. `.sav`
  files are battery saves, not emulator snapshots. Protect originals and compare
  their hashes before/after. Keep new artifacts under ignored `build/runtime/`.
- Optional `py-desmume==0.0.9` embeds DeSmuME 0.9.12. Supplied 0.9.13 snapshots
  can show plausible RAM but hang or render incorrectly. Cold boot battery saves
  to create compatible states; do not repeatedly advance a known-bad snapshot.
- Reuse tested routes with focused checks for the current functions and required
  helpers. The driver inserts one released frame after every action. Bound entry,
  assert that the target dispatch ran, and confirm the final scene from live state.
  A missed route is a coverage gap; do not remove its assertion to obtain a pass.
- Guard the complete native function range and owning overlay. Addresses are
  reused after transitions; positively identify a foreign owner before excluding
  a hit. The runtime tool's 90% overlay-identification threshold is only a locator,
  not an exact guard. Unexplained byte mismatches remain failures.
- Hooks fire before the addressed instruction. Respect ARM condition codes and
  distinguish helper effects from subsequent caller stores. Pair nested returns
  using entry SP and LR; tail calls can share both, so finish all matching pending
  records innermost first. Read ARM9 DTCM at `0x027E0000..0x027E4000` directly;
  do not fold stack outputs into main-RAM mirrors.
- Check whether changing a callback invokes it synchronously. The EUR overlay-5
  setter at `0x02066358` installs the callback and resets task phase to zero;
  its third argument means invoke immediately. A nonzero value can run the new
  callback before the setter returns. Model that nested call and its writes;
  do not treat the argument as a phase or assume one update per frame.
- Derive expected changes independently, including native integer wrapping and
  signed division. A fresh snapshot after a helper observes its effects; it does
  not verify that helper. Check full live allocations, padding and overlapping
  views, plus separate globals touched by the call. Constructor arguments can be
  valid before a scene global is published. An exact ROM replay confirms executed
  behavior; also review the C types and bounds, which matching bytes alone cannot
  establish as valid.
- Track allocation, initialization, updates and actual release separately. Derive
  pool slots and list writes from the allocator; do not assume payloads are zeroed.
  Removal flags, callback retargeting and pool returns are different events.
  Track attached resources and bulk-pool destruction too. Check records before
  release, then only still-live pool/list state. Address reuse starts a new lifetime.
  Virtual cleanup can release a model's internal allocations before its pool slot
  is returned. Check those boundaries separately. ResourceA and ResourceB use
  different pools and slot sizes; verify each against the current shared source
  and memory reference before extending an inherited probe.
- Derive timer behavior from its actual entry guard and recurrence. An initial
  zero may trigger now, persist indefinitely or have another meaning; integer
  truncation can add updates. Check the relevant paths instead of importing a
  neighboring callback's timing assumption. Count callback invocations separately
  from emulator frames, and preserve phase fallthrough within a single call.
- Verify RAM, mapped VRAM, palettes, OAM, ordered GPU stores and visible behavior
  as appropriate. Allocation, initialized extent and transfer size can differ.
  Hardware/FIFO readback is not the submitted sequence. Keep changing scanline
  and IRQ-driven registers in captures without asserting that they are immutable.
  Derive display engine, BG layer and tilemap buffer separately; a buffer index
  does not identify the main or sub screen. Verify both writes in a register
  clear-then-set operation, even when the final register value would be identical.
  Decode each hooked store's effective address, including shifted register
  indices, and reject unsupported forms. Check write-only BG scroll registers
  through the ordered stores rather than expecting readable register values.
- Keep verification proportional to what the target and its callees can touch.
  For a pure data callback, check full relevant live task/game records at each
  call; avoid copying all graphics memory on every invocation without a reason.
  Retain surrounding controller graphics checks and route captures where needed.
  Document the checked ranges and any sampling. Speed up host-side inspection
  only when the same expected results and assertions are preserved.
- Record per-function/branch counts, ROM/save/state hashes, inputs and explicit
  limits. Separate ordinary routes from RAM fixtures and document restoration.
  For deterministic sampling, report observed versus fully checked calls and
  the rule; track lifetimes independently. Screenshots alone do not prove execution.
- Validate referenced artifacts: existence, hashes, image dimensions, memory
  extents and report totals. Compare baselines only for identical state, fixture,
  input and capture prefixes. Distinguish visual inspection from hash equality
  and observed rasterization from an independent graphics oracle.
  A verified draw-list insertion does not prove a visible sprite: clipping can
  suppress it. State whether the evidence checks submission or rendered pixels.
  Associate every route with the exact probe source/version that produced it,
  including generated or composed copies. Preserve separate variants when the
  probe changes between routes; the current script cannot stand in for all of them.
- At replay end, stop admitting new outer calls and drain pending/nested calls
  for bounded neutral frames. Keep failures and rerun corrected oracles; do not
  discard pending calls or change matching game code to satisfy a faulty model.
  Check whether the active probe imports its editable body or a composed copy.

An optional private fast reader moves the existing DeSmuME byte-read loop into
host C without changing accesses. Before reuse, validate it against the original
reader on paused RAM, DTCM and graphics ranges; see
[transition evidence](docs/research/RECONSTRUCTION_NOTES.md#pause-transition-panels-and-controllers).
Host probe code does not contribute to matching-game coverage.

### Debug menu and Nawatobi

The debug menu teleports without necessarily initializing a complete game state.
Use the reference's tested routes for shops, save menus, Game Over, Smash Eggs,
credits and pause. Controlled access does not establish a normal story entry.
For RAM instructions, specify region, CPU/address space, dereferences, offset,
width, guard, timing and restoration; never give an unexplained dynamic address.

The tested EUR Nawatobi edit is the little-endian 32-bit phase word at
`read32(0x0208E1E0) + 0x30` in ARM9 main RAM, with overlay 7 loaded. At pause
update `0x02071F80`, require the full native-byte guard, a fully initialized pause
menu, `read32(0x0208E1E0) == r0`, vtable `read32(r0) == 0x0208D9B8` and phase 2.
Write 7 once, disable the hook and resume. The field becomes `07 00 00 00`;
do not overwrite the pointer at `0x0208E1E0` or freeze the phase. This is not
VRAM or a ROM patch. See the [procedure and coverage limits](docs/research/RECONSTRUCTION_NOTES.md#nawatobi).

## Documentation, Git and handoff

Keep the README short and human-readable, including the user's AI disclosure
and tone. Update the overlay map when linked coverage or confirmed roles change.
Avoid vague similarity claims, inflated completion language and speculative names.
Keep detailed findings in the reference and changing candidate status in the
private handoff, rather than continually extending this guide.

Track these stages separately:

| Stage | Evidence |
|---|---|
| Exact candidate | Identified source/object; full native comparison including pools and resolved relocations |
| Integrated | Current public source, headers, manifest and ownership pass build checks |
| Runtime checked | Completed route reports, function/branch counts, lifetimes, validated artifacts and stated limits |
| Published | Task-owned commit, successful push and confirmed remote revision |

Use `pending`, `failed` or `not run` where appropriate. The progress generator
reads metadata; it does not perform matching or runtime checks. If reports are
dirty, use `git show HEAD:docs/progress.json` for committed coverage. Confirm the
live remote before calling bytes published. Count only new code ranges; ARM7,
data and symbolic ASM follow the separate categories in the metric. Documentation
commits must not include pending source or claim unverified progress.
Read changing totals and milestone targets from current reports and the latest
user request; do not hardcode a moving percentage or candidate queue into this
guide. A byte-exact ROM rebuild and a successful route do not establish complete
C/C++ reconstruction or coverage of every branch.

`docs/BATTLE_MATCHING_MILESTONES.md` has a legacy non-UTF-8 byte and mixed line
endings. Append bytes while preserving the existing prefix, including when
checking the staged Git blob. Do not decode with replacement and rewrite it.
The similarly named file under `docs/research/` is an older, different log.

Keep ROMs, extracted assets, saves, proprietary tools, IDA databases, machine-code
dumps and captures private. Never force-add ignored material. Before publication:

1. Review `git diff --check`; stage explicit task-owned paths only.
2. Review staged paths, `git diff --cached` and `git diff --cached --check`.
3. Run `python tools/check_public_content.py` **after staging**; it audits the index.
4. Commit descriptively, push to the verified user remote and confirm its revision.
   Report the commit, push status, checks and any actual coverage change.

At a stop, update private `build/analysis/CURRENT_HANDOFF.md` with the latest
request, pushed revision, last verified code batch, pending paths and checks.
For every gap, record component/address, native/candidate sizes, mismatch class
and evidence needed to retry. Record exactness per function, including mixed
private units; preserve unfinished drafts during documentation work.

Keep failed replay commands, exit status, logs, frame/assertion and successful
reruns distinct. A suspected oracle correction is not a verified fix. Record
any live process's session ID, command and output path; inspect it before starting
a duplicate. Populated artifacts or elapsed time do not establish successful
completion. Historical build logs are not checks run by the current task.
