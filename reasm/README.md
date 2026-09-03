# Maintained reassembly sources

The generated raw module sources live under ignored `build/reassembly/` paths.
Do not copy those complete dumps into Git.

During the fixed-layout bootstrap, a maintained whole-module source named
`reasm/<version>/modules/<module>.s` overrides its generated counterpart. This
is intentionally temporary. Stage 1 will replace whole-module overrides with
sectioned, relocatable translation units driven by the `dsd` symbol and
relocation maps.

Small maintained functions are registered in `reasm/<version>/patches.json`.
Each entry names its owner module, runtime address, exact size, unique ELF input
section, source file, external symbol definitions, and expected original-byte
hash. The bootstrap builder injects the resulting fixed-size binary range.
`tools/relink_overlay.py` instead assembles the function as its own object and
links it together with sectioned fallback units at verified runtime addresses.
External definitions must agree with the DSD symbol maps and have a supporting
DSD relocation in the maintained unit.

Human-written symbolic assembly belongs here. Original ROM bytes, generated
`.word` dumps, extracted assets, and build outputs do not.
