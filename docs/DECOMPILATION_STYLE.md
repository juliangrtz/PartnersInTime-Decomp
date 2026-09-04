# Decompilation source policy

The project aims for readable, editable C that also reproduces the verified
European ROM exactly. Neither requirement is optional.

## Source organization

- Keep related functions from one contiguous original code region in one
  subsystem module. Do not create a permanent source file for every function.
- A small temporary file is acceptable while surrounding code is still a raw
  or symbolic assembly gap. Merge it into the subsystem module when the gap is
  translated; do not introduce linker tricks solely to make unrelated address
  ranges appear to be one compilation unit.
- Order functions as they occur in the original object whenever that order is
  known. Headers own shared structures, enums, flags, and public declarations.

## Readability and matching

- Prefer structured `if`, `switch`, and loop constructs. Use `goto` only when a
  structured equivalent cannot reproduce the original code, and document the
  matching constraint next to it.
- Replace confirmed numeric values with domain names. Keep unknown fields and
  flags explicitly named `unknown` rather than assigning speculative meaning.
- Add compile-time size checks to every sufficiently reconstructed structure.
- Treat decompiler output as a starting hypothesis, not source code. Simplify
  casts, temporary variables, and control flow, then run the matching check.
- Every C change must pass `ninja check`; milestone changes must also produce a
  native relink with zero differing bytes and pass the unit tests.

## Evidence levels

Names and comments should distinguish three levels of knowledge:

1. **Confirmed:** established by data flow, multiple callers, data formats, or
   a runtime observation. This may use a gameplay-facing name.
2. **Inferred:** strongly suggested but not yet observed. Record the evidence
   in the relevant research map and avoid over-specific public names.
3. **Unknown:** retain an offset, bit number, or neutral role name and add it to
   the runtime-probe backlog when its meaning affects modding.

## Runtime evidence workflow

Runtime probes should answer one narrow question at a time. Record the ROM
hash, battle/enemy, save state, breakpoint, inputs, before/after memory, and the
observed on-screen event. Useful first probes are:

| Question | Breakpoint or data | Observation |
|---|---|---|
| Which party actor ID maps to each character form? | `BattleActor_GetById` at `0x02076F64` | ID, actor pointer, scene object, visible character |
| What do active-model bits 10 and 11 control? | `0x020A3310` and `0x020A3348` | Caller, requested value, model flags, visible render change |
| Which hit kinds map to damage/reaction behavior? | `BattleHitDescriptor_Configure` at `0x0209E10C` | source, target, low six flag bits, resulting animation/effect |
| How are final damage modifiers applied? | `BattleDamage_CalculateBase` at `0x0209BF38` and `BattleDamage_DispatchHit` at `0x0209DE8C` | registers, stats, intermediate and final damage |
| What does save field `+0x558` represent? | `BattleActor_CanReceiveStatus` at `0x02076BD4` | field value, party composition, baby-status eligibility |

Overlay 2 is loaded at its fixed runtime addresses during battle.
`gBattleContext` is the pointer stored at `0x020C0718`; actor HP is the signed
halfword at actor offset `+0x06`. Begin with read-only breakpoints and memory
logging. Only after a hypothesis is reproducible should it change a public C
name or structure field.

For scripted emulator sessions, [`py-desmume`](https://pypi.org/project/py-desmume/)
is the preferred first automation candidate. Keep it out of the mandatory build
dependencies; introduce a pinned optional probe environment only when the first
repeatable RAM experiment is implemented.
