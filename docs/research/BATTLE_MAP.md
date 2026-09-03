# Battle reverse-engineering map (European ARMP build)

This map was recovered from the verified European ROM with SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Addresses combine resident ARM9
and runtime overlay 2.

## Navigation anchors

| Address | Working name | Purpose |
|---:|---|---|
| `020726B0` | `BattleTaskQueue_Enqueue` | Appends a callback/argument pair to the 32-entry battle task queue |
| `02073068` | `BattleMain_Update` | Per-frame battle update and central turn-state owner |
| `02076B0C` | `BattlePosition_StoreViewRelative` | Stores coordinates raw or relative to one of two battle-view offsets |
| `02076F44` | `BattleActor_GetPartySlot` | Resolves party IDs 56-59 through the battle context |
| `02076F24` | `BattleActor_GetEnemySlot` | Resolves enemy IDs 60-67 through the battle context |
| `02076F64` | `BattleActor_GetById` | Resolves party IDs 56-59 or enemy IDs 60+ to actor pointers |
| `02076BD4` | `BattleActor_CanReceiveStatus` | Rejects dead, unloaded, or inactive-form status targets |
| `02076FB4` | `BattleSceneObject_GetById` | Resolves field, party, enemy, and auxiliary visual-object IDs |
| `02091198` | `BattleSceneObject_SetAnimation` | Selects, creates, stops, or starts a scene-object animation |
| `02091A18` | `BattleSceneObject_IsAnimationChannelActive` | Tests one of four per-object animation slots |
| `02091A58` | `BattleSceneObject_IsAnimationActiveById` | Resolves an object ID and tests its requested animation slot |
| `0207FE2C` | `BattleTurnState_Update` | Turn selection, actions, reactions, victory, and exit |
| `02079950` | `BattleAI_DispatchOpcode` | Executes loaded `BAI_*.dat` battle bytecode |
| `0208ED90` | `BattleScript_GetProperty` | Reads actor, object, and global properties |
| `0208FB6C` | `BattleScript_SetProperty` | Writes properties; cases 16-20 are actor stats |
| `02071C84` | `BattleDamage_CalculateAttack` | General POW/DEF/level damage calculation |
| `0209BF38` | `BattleDamage_CalculateBase` | Compact actor-ID-based damage calculation |
| `0209BFA0` | `BattleDamage_CalculateByObject` | Resolves scene objects and selects damage modes/equipment |
| `0209BCCC` | `BattleDamage_ApplyEquipmentModifiers` | Applies attacker/defender equipment multipliers |
| `0209D694` | `BattleActor_ApplyDamage` | Subtracts HP, clamps at zero, and marks knockout |
| `0209CD9C` | `BattleDamage_SpawnNumber` | Creates free or actor-attached damage-number effects |
| `0209D718` | `BattleDamage_ApplyToEnemy` | Enemy damage, animation, popup, sound, effects |
| `0209D9DC` | `BattleDamage_ApplyToParty` | Party damage, animation, popup, sound, effects |
| `0209DE8C` | `BattleDamage_DispatchHit` | Routes queued hit records to enemy or party handling |
| `0209DFF4` | `BattleDamage_ReflectQueuedHits` | Reverses queued source/target pairs and rebuilds payloads |
| `0209DFDC` | `BattleEffect_SetVariant` | Sets the signed effect variant at battle-context offset `+0xCB7A` |
| `0209E10C` | `BattleHitDescriptor_Configure` | Configures a source/target hit before queue expansion |
| `0209E20C` | `BattleHitQueue_Update` | Dispatches old hits and compiles active descriptors into the next queue |
| `0209E918` | `BattleCollision_GetBounds` | Resolves party, special-object, or resource collision bounds |
| `0209EBAC` | `BattleHitDescriptor_GetByActorId` | Resolves per-actor 16-byte hit descriptors |
| `0209EBFC` | `BattleCollision_TestObjects` | Tests all source/target bounds and returns a hit position |
| `0209EF3C` | `BattleCollision_TestVolumes` | Swept six-axis overlap and time-of-impact calculation |
| `020A3370` | `BattleSceneObject_GetActiveModel` | Selects a scene object's primary or alternate model pointer |
| `020A50D4` | `BattleTaskList_Update` | Runs live callbacks and recycles stopped tasks |
| `020A519C` | `BattleTask_BindOwnerSlot` | Binds a task handle to its owning object and returns the displaced task |
| `020A51F8` | `BattleTaskList_Insert` | Allocates if needed and prepends a task to an active list |
| `020A5254` | `BattleTaskPool_Init` | Builds an aligned fixed-payload task free list |
| `020A90F4` | `BattleParty_UpdateKnockout` | Completes party knockout and linked-character transitions |
| `020A9280` | `BattleParty_StartKnockout` | Starts party knockout state, animation, sounds, and task |
| `020ACB44` | `BattleModelEffect_SpawnAttached` | Creates a model effect bound to an owner slot |
| `020ACB88` | `BattleModelEffect_Spawn` | Creates a positioned model effect from its resource table |
| `020ACBF0` | `BattleSpriteEffect_SpawnInFreeSlot` | Creates a sprite effect in the first free tracked slot |
| `020ACCB8` | `BattleSpriteEffect_Spawn` | Creates a sprite effect at view-adjusted coordinates |
| `0209C464` | `BattleStatus_TryApply` | Ailments and POW/DEF/SPD percentage changes |
| `0209C278` | `BattleStatus_ClearEffect` | Clears an effect and restores a base stat |
| `02076584` | `BattleItemEffect_Apply` | Healing, revival-style HP updates, status items |
| `020768A4` | `BattleItemEffect_ApplyBadgeBoost` | Applies equipped 150/200-percent healing multipliers |
| `02018F48` | `ItemEffect_CalculateValue` | Resident item-record value calculation and HP clamps |
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
| `+7E` | `party_member` | Identifies linked party members during revival handling |

Actor IDs 56-59 are party slots. IDs 60-67 are enemy slots. Do not confuse
battle actors with visual scene objects, whose offsets `+04/+06/+08` are
coordinates rather than HP or stats. `BattleSceneObject_GetById` resolves IDs
below 56 through the field-object table, 56-59 through party slots, 60-67
through enemy slots, and IDs from 68 through the auxiliary table. The active
model helper then selects scene-object pointer `+0xC0` or `+0xC4` from flag bit
14 at `+0xF4`. `BattlePosition_StoreViewRelative` accepts a raw-coordinate
bypass flag and otherwise subtracts either context offset pair
`+0xCB9C/+0xCB9E` or `+0xCBA0/+0xCBA2`; its stored depth is clamped at zero.
Both actor resolvers, the compact base
damage calculation, and the damage/KO updater are now maintained symbolic
assembly in `reasm/eur/battle/` and byte-match the European overlay.

`BattleSceneObject_SetAnimation` is the common animation path used by damage
and reaction code. It validates the bound resource, classifies scene-object
IDs as party or enemy, creates a model on demand, stops negative animation
requests, and starts the selected animation through the model vtable. Party
idle selection is adjusted for three actor status values, a separate party
flag, and the low-HP predicate. It also remaps animations for linked party
forms, preserves model flags across resource replacement, and calls the battle
scene's model-change callback when the active model changes.

## Battle task lists

Battle task pools begin with active-list and free-list pointers, followed by
nodes whose 12-byte header contains `next`, `callback`, and an optional pointer
to the owner's task slot. `BattleTaskPool_Init` links a fixed number of nodes
whose payload size is rounded to four bytes. `BattleTaskList_Insert` allocates
from that free list when the caller does not provide a node and prepends the
result to the active list.

`BattleTaskList_Update` calls every non-null callback once, detects owner slots
that no longer refer to their task, and recycles stopped nodes. Binding a task
stores both sides of the owner relationship and returns any displaced task.
`BattleTask_Release` clears an active callback for deferred removal, but can
immediately return a newly allocated, not-yet-inserted node to its pool. The
separate raw-node take/return pair serves callers that use the same free-list
layout without task callbacks.

The two effect families share the same ownership pattern but use separate
resource tables and constructors. Model effects store signed X/Y/Z halfwords,
a Q8-scale argument shifted into their internal fixed-point field, an optional
object pointer, and an owner slot at `+0x34`. Sprite effects add the current
view origin to X/Y before construction and use owner slot `+0x2C`. A convenience
allocator scans the 64 pointers at context offset `+0xCBF8`, binds the first
free slot, and returns its index or `-1` when all slots are occupied.

## Damage and status behavior

The compact base formula at `BattleDamage_CalculateBase` is:

```c
damage = ((level * power * scale_q8) / defense + 128) / 256;
damage = min(damage, 999);
```

The maintained function resolves both actor IDs, extracts the attacker's level
from the low seven flag bits, reads signed POW/DEF fields, calls the signed
division helper, reproduces the original rounding, and caps the result at 999.

The maintained `BattleDamage_CalculateAttack` path performs the corresponding
full calculation for live actors. It resolves an implicit defender when needed,
maps attack categories to Q8 multiplier columns, optionally applies a second
multiplier table, clamps the intermediate and final values, scales by the
battle-wide percentage, and honors the defender's forced-one-damage flag.

`BattleDamage_CalculateByObject` is now maintained in full. It maps both scene
objects through their linked actor IDs, rejects invalid IDs and the defender
immunity flag, selects Q8 scales `0x126`, `0x10C`, or `0x100` from the active
battle mode, calls the appropriate maintained damage path, applies a
140-percent equipment bonus for effect `0x301B`, and caps the result at 999.

`BattleDamage_ApplyEquipmentModifiers` first calculates base damage, then
rounds each equipment stage as `(damage * percent + 50) / 100`. Its recovered
effect table is:

| Effect | Side | Rule |
|---:|---|---|
| `3015` | defender | 25% damage |
| `301F` | defender / attacker | 50% damage |
| `3020` | defender / attacker | 150% damage |
| `3014` | attacker | 250% while HP is at most 25% |
| `300D` | attacker | 250% when the caller condition is active |
| `3016` | attacker | 150% damage |
| `301D` | attacker | `max(100, 300 - 8 * eligible_item_count)` percent |

The HP-at-most-quarter predicate is maintained separately and compares
`current_hp * 100 <= max_hp * 25` without division.

The maintained `BattleStatus_TryApply(actor, status_id, duration,
magnitude_percent, chance_percent)` handles both ailments and temporary stat
changes. For enemy IDs 60-67 it reads four two-bit resistance fields from
enemy-record offset `+0E`; their values leave chance unchanged, double it,
halve it, or reject the effect. A random value in `[0, 99]` then decides the
application. Party targets additionally pass through two equipment-effect
special cases.

The caller-side `BattleActor_CanReceiveStatus` gate additionally requires
positive HP. Enemy targets must have a loaded resource slot; the two adult
party IDs are accepted directly, while the baby IDs require save-state field
`+0x558` to equal 2.

On success it initializes the corresponding 12-byte effect state, preserves
the requested duration, emits the original sound/effect event, and returns the
status ID. Failed applications return zero. For stat changes it calculates:

```c
current_stat = clamp((base_stat * (100 + magnitude) + 50) / 100, 0, 999);
```

The stat IDs are:

- status 6 changes power from `base_power`;
- status 7 changes defense from `base_defense`;
- status 8 changes speed from `base_speed`.

`BattleStatus_ClearEffect` is maintained symbolic assembly: it verifies the
requested status is active, removes the associated battle effect, clears its
state, and restores base POW, DEF, or SPD for status IDs 6-8. The maintained
`BattleStatus_ClearAll` wrapper explicitly invokes it for IDs 1 through 8.

`BattleItemEffect_Apply` decodes the effect type from each 20-byte resident
item record. Types 0-1 add a fixed HP amount, type 2 restores a percentage of
maximum HP, type 3 cures ailments and negative stat changes, and types 4-6
apply POW, DEF, or SPD changes with a battle-configured duration. The adjacent
badge helper scales the actual HP delta to 150 or 200 percent and clamps it to
maximum HP. These two overlay functions and the resident
`ItemEffect_CalculateValue` dependency are all maintained exact assembly.

Property IDs 16-20 directly expose current HP, max HP, POW, DEF, and SPD.

`BattleDamage_ApplyToEnemy` is maintained end to end. It rejects already
defeated targets, clamps incoming damage to 1-999, calls the HP/KO primitive,
selects the enemy hit reaction, preserves special-object state, computes the
world/screen position for the damage number, honors the enemy record's popup
suppression bit, and emits the impact variants selected by hit kinds `0x11`
and `0x13`.

`BattleDamage_SpawnNumber` is the shared popup constructor used by both target
paths and several attack scripts. It applies the battle-wide X/Y popup offsets,
stores the displayed value in the effect, and can either use a caller-selected
effect or create the actor-attached `0x369` variant with its own task lifecycle.
The maintained lifecycle merges overlapping values through effects `0x36A` and
`0x36B`, selects final actor-state variants `0x36C` or `0x36D`, and releases the
task pointer at actor offset `+0x2C` only after the last effect has disappeared.

`BattleDamage_ApplyToParty` is maintained too. It clamps damage, clears status
1, handles the special nonlethal hit kind, selects one of six party reaction
paths, generates Mario/Luigi-specific popup metadata, and optionally decodes a
post-hit status record into status ID, chance, magnitude, and the battle-wide
duration before calling `BattleStatus_TryApply`.

The maintained `BattleDamage_DispatchHit` is the common entry above those two
paths. Its 20-byte hit record supplies target ID, hit coordinates, pending hit
kind, and the optional party status payload. It accepts party IDs 56-59 and
enemy IDs 60-67, resolves the corresponding scene object and actor, computes
relative offsets, then dispatches to the exact target-specific implementation.
The adjacent maintained party/enemy reaction starters allocate or reuse the
per-actor task at `actor + 0x28`, set actor flag `0x200` while the reaction is
active, and install their original update callbacks. The party pre-hit hook
also consumes its one-shot flag and clears POW status 6 when equipment effect
`0x3024` is active.
Their two maintained per-frame callbacks now close the lifecycle: wait for the
animation completion flags, choose party KO or idle behavior, restore ordinary
enemy impact offsets, clear the task callback, and release actor flag `0x200`.
The alternate maintained effect-reaction pair uses the same `actor + 0x28`
ownership and hit-lock flag, but waits for an attached effect or invalidated
resource rather than selecting a full party/enemy animation sequence.
The maintained party launch-reaction pair owns the remaining state machine in
this region: it moves a party object through the screen boundary, switches its
resource animation, emits the three-stage impact burst, restores or retires
the actor according to HP, and finally releases the same hit-lock flag.

If HP reached zero, `BattleParty_StartKnockout` clears all eight statuses,
starts animation 13, sets actor and battle-global locks, selects one of six
form-specific sound pairs, and installs `BattleParty_UpdateKnockout`. The
update callback waits for the model flags, releases ordinary party actors, or
for linked forms moves the paired scene object and chains into the appropriate
Mario/Luigi follow-up load callback.

The hit queue at battle-context offset `0xCAD8` contains up to eight packed
20-byte records. `BattleDamage_ReflectQueuedHits` stops at the first inactive
entry, finds records targeting the requested actor, recalculates damage for the
reversed pairing, swaps source and target, and reloads the reflected attack's
status ID, chance, and magnitude.

Before expansion into that queue, each active attacker owns a 16-byte hit
descriptor in the battle-context table at `+0xC8F4`. The maintained configure,
lookup, status-payload, and disable helpers expose its linked-list pointer,
callback, source/target IDs, six-bit hit kind, seven-bit status ID, signed
chance/magnitude bytes, and the active-list head at context offset `+0xCAD4`.
The maintained `BattleHitQueue_Update` is the complete bridge between both
representations. Each update first invokes callbacks for the previous queue,
then walks and unlinks active descriptors, expands party and enemy wildcard
targets, collision-tests candidates, copies status/callback payloads, removes
conflicting chains and duplicate targets, calculates pending damage, handles
negative/reversed damage, and leaves at most eight records for the next pass.
`BattleCollision_GetBounds` supplies the queue compiler with six signed
halfwords. It contains explicit body-size presets for the six party-member
forms and object IDs 8-9, while ordinary battle objects obtain their bounds
from the bound resource and receive the original coordinate-axis conversion.
The maintained object test iterates every available source/target bound pair,
transforms both objects' bounds into battle-scene coordinates, and passes the
four resulting volumes plus the output position to the low-level overlap test.
That maintained low-level test rejects separated or nonconverging axes, derives
fixed-point entry/exit times across all six faces, rejects misses, returns a
six-bit impact-axis mask, and interpolates the three-dimensional hit position.

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
