# Working on Partners in Time

## Scope and working priorities

This is a matching C/C++ reconstruction of Mario & Luigi: Partners in Time for
Nintendo DS. The primary target is the European ROM. The intended result is
readable, editable source that reproduces the original game byte for byte.
The decompilation is generated with AI under human direction; preserve the
disclosure in the README and follow [the project policy](docs/AI_USAGE.md).

- Follow the current request. A documentation or research task does not start
  another decompilation batch. Honor requests to stop after the next block.
- For continued decompilation, prioritize small, well-understood functions and
  related groups with clear data flow. The current next milestone is 50%
  matching C/C++, unless the user sets another target.
- Work systematically from instructions, callers, types and runtime evidence.
  Do not brute-force source permutations or spend hours guessing at register
  allocation. Record a difficult gap and move to another useful target.
- Complete authorized work without repeatedly asking for confirmation. The
  user has authorized intermediate commits and pushes. Give concise updates
  and report actual results, remaining gaps and verification limits.

## Start with the current checkout

Use the Git repository root for all commands; the terminal's initial directory
may instead contain ROMs and other private files. On the current workstation,
the checkout is `C:\Users\Julian\Desktop\PartnersInTime-Decomp` and the shell is
PowerShell. Quote paths containing spaces.

Read `git status --short`, the recent log and the relevant source before editing.
Preserve unrelated changes, including changes left by the user or another tool.
Use `rg` for searches. Keep independent reads parallel where useful, but run
shared build and source-metadata changes sequentially.

Read these as needed rather than loading every research log:

- [Source policy](docs/DECOMPILATION_STYLE.md): layout, naming and matching rules.
- [Overlay map](docs/research/OVERLAY_MAP.md): subsystem roles and source links.
- [Progress guide](docs/PROGRESS.md): what counts as matching C/C++.
- [Runtime guide](docs/research/RUNTIME_ANALYSIS.md): saves, inputs and RAM probes.
- [IDA guide](tools/ida/README.md): imports, disassembly, callers and pseudocode.
- [Data modding](docs/DATA_MODDING.md): text, tables and script source formats.
- [Reassembly plan](docs/REASSEMBLY_PLAN.md): native relinking and remaining work.

Old milestone notes describe the state at the time they were written. Current
source, `delinks.txt`, `symbols.txt`, the linked-source manifest and fresh checks
take precedence over old counts or claims that a function is unfinished.

Private IDA databases live under `build/ida/`; previous pseudocode exports and
candidate units live under `build/analysis/`, including
`build/analysis/local_decompiler/`. Recompile a candidate against current headers
before trusting an old matching object. Abandoned drafts may contain incorrect
experimental prototypes. Parse large JSON reports and select the relevant
records instead of dumping entire reports into the conversation.

## Toolchain and build

Use Python 3.11+, Ninja, and the compatible Metrowerks ARM compiler installed
under `tools/mwccarm/1.2/base/` (`mwccarm.exe` and `mwldarm.exe`). The compiler
identifies internally as 2.0 build 72. `tools/configure.py --compiler PATH eur`
accepts another compiler root. LLVM's `llvm-mc`, `ld.lld` and `llvm-objcopy` are
also needed for the native reassembly/relink workflow.

If Ninja is missing from `PATH` on this workstation, the installed executable
is `C:\Program Files\JetBrains\CLion 2023.2.2\bin\ninja\win\x64\ninja.exe`.
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

For a milestone, also verify the native relink and run the unit suite:

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
3. Keep related contiguous functions in a subsystem module. Temporary isolated
   units are acceptable while an intervening assembly gap remains; consolidate
   once that gap is recovered. Shared declarations and layouts belong in headers.
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

When only an object's prefix is known, describe it explicitly as a prefix view;
its `sizeof` does not establish the full allocation size. Decode literal pools
as little-endian data, not as ARM instructions. Check ARM/Thumb state and
interworking relocations when comparing calls; a private comparison script
does not replace the full module and symbol checks.

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

Use ordinary buttons and live RAM observations to navigate automatically.
Inspect screenshots and active overlays before interpreting a trace: a save can
start beside a save block, and A may open a menu instead of advancing the field.
Selected actions and party formations also change which button performs a jump.

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

The debug menu can teleport without fully initializing the destination state.
Controlled RAM edits or temporary decoded-command substitutions are useful
probes, but record exactly what changed and when it was restored. They do not
demonstrate normal gameplay accessibility. Prefer read-only observation after
the controlled setup. Never infer complete branch coverage from a matching ROM.

When explaining a memory edit, specify CPU/address space, ROM region, pointer
dereferences, field offset, access width and timing. The tested EUR Nawatobi
entry uses ARM9 main RAM with overlay 7 loaded. From a fully initialized pause
menu, break at pause update `0x02071F80` and perform this one-time edit
(pseudocode; `read32` and `write32` use little-endian 32-bit accesses):

```text
task = read32(0x0208E1E0)
assert task == r0 and read32(task) == 0x0208D9B8
assert read32(task + 0x30) == 2
write32(task + 0x30, 7)
```

Disable the edit hook and resume so the native transition can advance the
phase. The task is dynamically allocated; an observed task address is not a
universal address to patch. Its code is in `src/overlay007/nawatobi_*`;
the selector and rope simulation were exercised, but a normal entry route
remains unconfirmed.

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
