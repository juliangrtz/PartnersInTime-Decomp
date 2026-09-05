# ARM9 overlay map

This is the navigation and triage map for the European executable overlays.
It separates facts proven by code/data references from working hypotheses. An
unknown row is deliberately left unknown; an overlay number alone does not
identify a gameplay subsystem.

The progress byte counts are generated in `docs/progress.json`. Function ranges
listed below are linked, byte-matching C. The generic IDA importer accepts any
real overlay ID and derives its section boundaries and function map from DSD:

```powershell
& 'C:\Program Files\IDA Essential 9.2\idat.exe' '-A' '-c' `
  '-TBinary file' '-pARM' '-b2065D4' '-i2065D40' `
  '-Stools/ida/import_overlay.py 5' '-Lbuild/ida/ov005.log' `
  '-obuild/ida/PiT_eur_ov005.i64' 'build/eur/build/arm9_ov005.bin'
```

The raw-loader base and input image differ by overlay; see
`tools/ida/README.md` before substituting another ID.

## Real code overlays

| Overlay | Mapped code | Proven role or evidence | Matching high-level ranges | Triage state |
|---:|---:|---|---|---|
| 0 | 366,712 B | Field/world code; the recovered entity object has two lifecycle variants and action/interaction state | `0x020A2BBC-0x020A2D04` (10 functions, 328 B) | Active; large dispatcher-heavy overlay, take isolated object helpers first |
| 2 | 362,436 B | Battle engine, battle BAI extension, combat UI, effects, transitions, and results | Many maintained units; see `BATTLE_MAP.md` | Active; continue coherent clusters alongside other overlays |
| 5 | 16,760 B | Shared resource owners and Nintendo DS 2D display/BG address helpers are proven; whole-overlay role remains open | `0x02066224-0x02066358` and `0x02067C70-0x02067F84` (28 functions, 1,096 B) | Active quick-win target |
| 6 | 66,492 B | Unknown | None | Untriaged |
| 7 | 142,264 B | Unknown | None | Untriaged; split into call-graph clusters before choosing source units |
| 8 | 54,068 B | Unknown | None | Untriaged |
| 9 | 78,984 B | Contains four shop datasets used by the editable-data project | None | Data mapped; code triage pending |
| 10 | 6,820 B | Six-slot enemy selection/snapshot logic, enemy-stat trait filtering, Q8 scaling, and threshold selection | `0x020C2340-0x020C259C` (6 functions, 604 B) | Active; high-value small overlay |
| 11 | 13,372 B | Unknown | None | Priority quick-win target |
| 12 | 12,032 B | Battle special-attack code; recovered state tracks party formation, animation timing, a shuffled four-member action order, and paired scene-object cleanup | `0x020C2FCC-0x020C31E8` and `0x020C4FA8-0x020C50C0` (8 functions, 820 B) | Active; exact item identity still requires runtime confirmation |
| 13 | 12,316 B | Unknown | None | Priority quick-win target |
| 14 | 13,860 B | Unknown | None | Priority quick-win target |
| 15 | 13,164 B | Unknown | None | Priority quick-win target |
| 16 | 18,324 B | Unknown | None | Untriaged |
| 17 | 16,784 B | Unknown | None | Untriaged |
| 18 | 18,036 B | Unknown | None | Untriaged |
| 20 | 9,204 B | Battle special-attack code; recovered state initializes object 40, resets the acting party member, controls finish/mode transitions, and launches a target-relative vertical arc | `0x020C34A4-0x020C3724` (6 functions, 640 B) | Active; continue around the central animation dispatcher |
| 21 | 5,280 B | Recovered code configures a battle scene-object animation/model and maintains two packed attack phases/timers | `0x020C2AE0-0x020C2BCC` (5 functions, 236 B) | Active; smallest real overlay |
| 25 | 38,024 B | Unknown | None | Untriaged |
| 26 | 14,452 B | Unknown | None | Priority quick-win target |

## Placeholder entries

Overlay IDs `1`, `3`, `4`, `19`, `22`, `23`, `24`, and `27-36` share the same
verified 32-byte placeholder image. They contain no DSD `kind:code` range and
therefore do not contribute to the decompilation denominator. They need build
and overlay-table preservation, not invented source functions.

## Triage checklist

For each real overlay:

1. import or refresh its IDA database;
2. rank leaf functions by size, call count, and structure overlap;
3. inspect callers and constants before assigning semantic names;
4. group adjacent functions by domain rather than creating one file per
   function;
5. compile in original function order, verify every symbol with `objdiff`, and
   only then link the unit;
6. update this map when a role is proven or a new high-level range lands.

Runtime observation is preferred when static evidence cannot distinguish two
plausible meanings. Until then, neutral names are part of the correctness
standard.
