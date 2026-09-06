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
| 0 | 366,712 B | Field/world code; the recovered entity object has two lifecycle variants and action/interaction state | `0x020A2BBC-0x020A2D04` (10 functions, 328 B) | Active; the complete 23,492-byte field VM dispatcher is structured WIP C for all 290 slots `0x033..0x154` (one 26,436-byte function with no remaining code-helper symbols); IDA confirms that the outer table's shared nested command-family switches are the next layout target |
| 2 | 362,436 B | Battle engine, battle BAI extension, combat UI, effects, transitions, and results | Many maintained units; see `BATTLE_MAP.md` | Active; the complete 19,168-byte battle-specific VM dispatcher is structured WIP C for all 182 slots (one 17,736-byte function, 40.13% fuzzy, no remaining helper symbols), while linked matching work continues in coherent clusters |
| 5 | 16,760 B | Shared resource owners plus Nintendo DS 2D display addressing, affine transforms, display-plane control, VRAM-bank reset, palette transfer, tile, and tilemap loading helpers are proven; whole-overlay role remains open | `0x02066224-0x02066358` and `0x02067AAC-0x02068594` (52 functions, 3,100 B) | Active quick-win target |
| 6 | 66,492 B | Title/staff-roll scene family; embedded `clTitleML2` and `clStaffRoll` controllers, title/menu resources, localized staff strings, and the ending field handoff to `func_ov006_0207ab08` are statically proven | None | Role proven; split the title and credits controller clusters before reconstructing leaf helpers |
| 7 | 142,264 B | Scene/object subsystem: a 210-entry script-VM descriptor table, its command handler, two global script slots, 56 object slots, movement/animation commands, and the `0x7000` variable extension are statically proven | `0x02083B1C-0x02083C20`, `0x02083EF8-0x0208435C`, `0x020844E0-0x02084784`, `0x020848F8-0x02084FBC`, and `0x020850D4-0x02085168` (27 functions, 3,940 B) | VM scheduler plus manager initialization, archive ownership, object/task cleanup, transition control, and sprite following are matching; the complete dispatcher is structured WIP C at 83.89% fuzzy similarity, alongside the intervening renderer, secondary-archive selector, sprite factory, and 964-byte controller |
| 8 | 54,068 B | Save/load/game-over scene family: embedded `clSaveMenu`, `clLoadMenu`, and `clOverMenu` controllers; resident field result 5 enters the save path at `func_ov008_0206ca4c`, while result 7 enters the game-over path at `func_ov008_02070e14`. The save scene snapshots party stats, inventory, options, and the scripted save-location byte, and its confirmation path invokes the resident save writer | None | Three scene roles proven; isolate their controllers before reconstructing leaf helpers |
| 9 | 78,984 B | Shop/menu overlay; recovered code maps menu categories to the four item classes and selects the item pool or 4-by-6 tier descriptor table for each of four shops | `0x0207DF1C-0x0207E008` (3 functions, 236 B) | Active quick-win target; shop-stock access is reconstructed and agrees with the editable-data layout |
| 10 | 6,820 B | Six-slot enemy selection/snapshot logic, enemy-stat trait filtering, Q8 scaling, and threshold selection | `0x020C2340-0x020C259C` (6 functions, 604 B) | Active; high-value small overlay |
| 11 | 13,372 B | Battle projectile/special-attack code; recovered helpers configure object 40 from level-scaled tables, calculate animation timing, test actor contact, clamp motion, and restore the actor resource | `0x020C4968-0x020C4C2C` (7 functions, 708 B) | Active; shell-like behavior is strongly indicated, but retain neutral item naming pending runtime confirmation |
| 12 | 12,032 B | Battle special-attack code; recovered state tracks party formation, animation timing, a shuffled four-member action order, and paired scene-object cleanup | `0x020C2FCC-0x020C31E8` and `0x020C4FA8-0x020C50C0` (8 functions, 820 B) | Active; exact item identity still requires runtime confirmation |
| 13 | 12,316 B | Battle special-attack code; recovered helpers prepare and align a paired scene-object setup, advance motion tuning, classify vertical angles, synchronize model animation, and start entry/retreat motion | `0x020C2AE0-0x020C2CD4` and `0x020C3620-0x020C37E0` (7 functions, 948 B) | Active; exact Bros. item identity still requires runtime confirmation |
| 14 | 13,860 B | Battle special-attack code; a recovered attack-object state machine controls approach and target motion, animation waits, hide/finish transitions, effects, sound, and cleanup | `0x020C3700-0x020C3AA4` (9 functions, 932 B) | Active; continue around callers while retaining neutral item naming |
| 15 | 13,164 B | Battle attack sequence | 8 functions / 668 B matching C | Actor selection, hit setup, model flags, object-pair initialization, and actor preparation reconstructed |
| 16 | 18,324 B | Battle attack code with a 20-entry motion table, resource-backed 20-byte motion records, and four variant attack objects that bind resource 53 before moving toward target-relative offsets | `0x020C2AE0-0x020C2BBC` and `0x020C2F1C-0x020C3168` (10 functions, 808 B) | Active quick-win target; motion-table access, object initialization/reset, phase changes, and target launch are reconstructed while the exact Bros. item identity remains open |
| 17 | 16,784 B | Battle attack sequence with threshold-driven tuning tables, a four-object party/support setup, and display initialization | `0x020C2C94-0x020C30D0`, `0x020C3280-0x020C3748` (19 functions, 2,308 B) | Tuning lookup, party resource swap/readiness, object setup, display startup, model/resource animation configuration, horizontal exits, and accelerated object launches reconstructed; exact item identity remains open |
| 18 | 18,036 B | Battle attack code with ten-row progress thresholds for input windows, alternate-actor probability, launch height, duration, distance, speed, and Q8 motion scaling; a four-variant object controller also tracks resource indices, modes, runtime flags, and per-variant anchor offsets | `0x020C30F0-0x020C3298` and `0x020C6918-0x020C6A20` (12 functions, 688 B) | Active quick-win target; the tuning-table access and object-controller lifecycle are reconstructed, while the exact Bros. item identity remains open |
| 20 | 9,204 B | Battle special-attack code; recovered state initializes object 40, resets the acting party member, controls finish/mode transitions, and launches a target-relative vertical arc | `0x020C34A4-0x020C3724` (6 functions, 640 B) | Active; continue around the central animation dispatcher |
| 21 | 5,280 B | Recovered code configures a battle scene-object animation/model and maintains two packed attack phases/timers | `0x020C2AE0-0x020C2BCC` (5 functions, 236 B) | Active; smallest real overlay |
| 25 | 38,024 B | Unknown | None | Untriaged |
| 26 | 14,452 B | Battle attack code; its shared state tracks all four party objects, async work, one resource load, cleanup/restoration, and adult/baby pairing | `0x020C2AE0-0x020C2D38`, `0x020C34A8-0x020C3570` (8 functions, 800 B) | Active quick-win target; state teardown, readiness gates, tracked-object phases, guarded resource/animation setup, and partner-target validation reconstructed |

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
