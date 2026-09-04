# Decompilation progress

The README tracker is generated from repository metadata rather than manually
entered percentages:

```powershell
python .\tools\generate_progress.py
```

Run `python .\tools\generate_progress.py --check` to verify that the committed
SVG and JSON artifacts are current.

## What the percentage means

The denominator is every European ARM9, ITCM, and overlay range marked
`kind:code` in the DSD `delinks.txt` files. BSS, data, graphics, audio, and
filesystem assets do not inflate it.

- **Matching C/C++** is a `.text` range assigned to a high-level translation
  unit that is present in the repository, reproduces the original layout, and
  is enabled in the exact build's `linked_sources.txt` manifest.
  Work-in-progress source remains visible in objdiff without inflating this
  total.
- **Symbolic ASM** is a maintained source range in
  `reasm/eur/patches.json`. If high-level source and assembly cover the same
  bytes, the high-level source wins.
- **Remaining** is mapped machine code that has not yet reached either source
  state. It may already have function names or research notes; those alone do
  not count as decompilation.

ARM7 is deliberately not included in the headline percentage. Its two large
autoloads are still conservatively mapped as mixed code/data images, so there
is no trustworthy code-only denominator yet. The tracker reports the exact
number of ARM7 bytes replaced by maintained symbolic source separately. Once
the remaining code/data boundaries are proven, ARM7 can join the same metric.

The exact ROM roundtrip is a separate milestone: binary-preserving reassembly
does not imply high-level decompilation. The treemap is intentionally strict so
that moving a rectangle from dark blue to cyan, and later to green, represents
real source recovery.

## Machine-readable output

[`progress.json`](progress.json) contains the totals and one record per native
ARM9 component. It is deterministic and suitable for badges, a future website,
or CI checks. [`progress.svg`](progress.svg) is the rendered README graphic;
hovering its cells in an SVG-capable viewer shows the component, function or
source-unit label, size, and status.
