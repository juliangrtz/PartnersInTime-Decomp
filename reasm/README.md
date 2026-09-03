# Maintained reassembly sources

The generated raw module sources live under ignored `build/reassembly/` paths.
Do not copy those complete dumps into Git.

During the fixed-layout bootstrap, a maintained whole-module source named
`reasm/<version>/modules/<module>.s` overrides its generated counterpart. This
is intentionally temporary. Stage 1 will replace whole-module overrides with
sectioned, relocatable translation units driven by the `dsd` symbol and
relocation maps.

Small maintained fixed-layout functions are registered in
`reasm/<version>/patches.json`. Each entry names its owner module, runtime
address, exact size, source file, and expected original-byte hash. The builder
assembles these sources over the corresponding generated range and rejects
size, overlap, or base-ROM mismatches. This is the promotion path used while
the fully relocatable Stage-1 linker is being developed.

Human-written symbolic assembly belongs here. Original ROM bytes, generated
`.word` dumps, extracted assets, and build outputs do not.
