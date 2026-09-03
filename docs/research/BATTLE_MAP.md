# Battle reverse-engineering map (European ARMP build)

This map was recovered from the verified European ROM with SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Addresses combine resident ARM9
and runtime overlay 2.

## Navigation anchors

| Address | Working name | Purpose |
|---:|---|---|
| `020726B0` | `BattleTaskQueue_Enqueue` | Appends a callback/argument pair to the 32-entry battle task queue |
| `02073068` | `BattleMain_Update` | Per-frame battle update and central turn-state owner |
| `02076F44` | `BattleActor_GetPartySlot` | Resolves party IDs 56-59 through the battle context |
| `02076F64` | `BattleActor_GetById` | Resolves party IDs 56-59 or enemy IDs 60+ to actor pointers |
| `0207FE2C` | `BattleTurnState_Update` | Turn selection, actions, reactions, victory, and exit |
| `02079950` | `BattleAI_DispatchOpcode` | Executes loaded `BAI_*.dat` battle bytecode |
| `0208ED90` | `BattleScript_GetProperty` | Reads actor, object, and global properties |
| `0208FB6C` | `BattleScript_SetProperty` | Writes properties; cases 16-20 are actor stats |
| `02071C84` | `BattleDamage_CalculateAttack` | General POW/DEF/level damage calculation |
| `0209BF38` | `BattleDamage_CalculateBase` | Compact actor-ID-based damage calculation |
| `0209D694` | `BattleActor_ApplyDamage` | Subtracts HP, clamps at zero, and marks knockout |
| `0209D718` | `BattleDamage_ApplyToEnemy` | Enemy damage, animation, popup, sound, effects |
| `0209D9DC` | `BattleDamage_ApplyToParty` | Party damage, animation, popup, sound, effects |
| `0209C464` | `BattleStatus_TryApply` | Ailments and POW/DEF/SPD percentage changes |
| `0209C278` | `BattleStatus_ClearEffect` | Clears an effect and restores a base stat |
| `02076584` | `BattleItemEffect_Apply` | Healing, revival-style HP updates, status items |
| `0208908C` | `BattleEnemyData_RequestLoad` | Initializes and queues one enemy-data request |
| `0209234C` | `BattleObjectData_ResolveSlot` | Decodes a packed object ID into a 44-byte runtime descriptor |

## Battle actor layout

| Offset | Working field | Evidence |
|---:|---|---|
| `+04` | `max_hp` | Loaded from enemy record `+06`; healing clamp target |
| `+06` | `current_hp` | Initialized from max HP; damage subtracts from it |
| `+08` | `speed` | Loaded from enemy record `+0C`; status 8 changes it |
| `+0A` | `power` | Loaded from enemy record `+08`; damage input |
| `+0C` | `defense` | Loaded from enemy record `+0A`; damage divisor |
| `+10` | `base_speed` | Restored when status 8 ends |
| `+12` | `base_power` | Restored when status 6 ends |
| `+14` | `base_defense` | Restored when status 7 ends |
| `+20` | `target_actor_id` | Resolves an implicit defender |
| `+24` | `flags` | Low seven bits contain level; bit `0x100` is KO |
| `+30` | `ailment_1` | Status IDs 1-3 |
| `+3C` | `ailment_2` | Status IDs 4-5 |
| `+48` | `power_change` | Signed percentage for status 6 |
| `+54` | `defense_change` | Signed percentage for status 7 |
| `+60` | `speed_change` | Signed percentage for status 8 |
| `+6C` | `resource_slot` | Leads to the loaded enemy stat record |

Actor IDs 56-59 are party slots. IDs 60-67 are enemy slots. Do not confuse
battle actors with visual scene objects, whose offsets `+04/+06/+08` are
coordinates rather than HP or stats. Both actor resolvers, the compact base
damage calculation, and the damage/KO updater are now maintained symbolic
assembly in `reasm/eur/battle/` and byte-match the European overlay.

## Damage and status behavior

The compact base formula at `BattleDamage_CalculateBase` is:

```c
damage = ((level * power * scale_q8) / defense + 128) / 256;
damage = min(damage, 999);
```

The maintained function resolves both actor IDs, extracts the attacker's level
from the low seven flag bits, reads signed POW/DEF fields, calls the signed
division helper, reproduces the original rounding, and caps the result at 999.

`BattleStatus_TryApply(actor, status_id, duration, magnitude_percent,
chance_percent)` handles both ailments and temporary stat changes:

- status 6 changes power from `base_power`;
- status 7 changes defense from `base_defense`;
- status 8 changes speed from `base_speed`.

`BattleStatus_ClearAll` is maintained symbolic assembly and explicitly invokes
the per-effect reset for status IDs 1 through 8.

Property IDs 16-20 directly expose current HP, max HP, POW, DEF, and SPD.

## Enemy stat records

`/BData/BDataMon.dat` is 4,312 bytes: 98 records of 44 bytes.

| Offset | Size | Working field |
|---:|---:|---|
| `00` | 2 | object-data ID |
| `02` | 2 | partially understood flags/AI-related ID |
| `05` | 1 | level |
| `06` | 2 | max HP |
| `08` | 2 | POW |
| `0A` | 2 | DEF |
| `0C` | 2 | SPD |
| `0E` | 2 | partially understood traits/resistance flags |
| `20` | 2 | experience |
| `22` | 2 | coins |
| `24` | 4 | item drop 1 |
| `28` | 4 | item drop 2 |

`BattleEnemyData_LoadStatRecord` is maintained symbolic assembly. It multiplies
the selected enemy index by 44, queues exactly that byte range from the primary
battle resource, and installs `BattleEnemyData_LoadObjectData` as the next load
callback. `BattleEnemyData_RequestLoad` initializes that request and submits it
through the maintained `BattleTaskQueue_Enqueue` path. The object-data callback
uses the maintained packed-ID resolver and queues the variable-sized payload
after the stat record; its maintained fixup callback converts embedded offsets
to RAM pointers. `BattleEntity_BindResource` is maintained too: its enemy branch
copies max/current HP, base/current POW, DEF, and SPD, installs level/trait
bits, and clears transient actor flags; its party branch binds the corresponding
scene-object resource record.

## Large native dispatchers

`BattleAI_DispatchOpcode` is one real `0x4AE0`-byte function.
`BattleTurnState_Update` is one real `0x7D5C`-byte function. They are genuine
large switches, not accidental merged functions. Reconstruct their leaf
operations and state layouts before attempting to translate either dispatcher
as one unit.
