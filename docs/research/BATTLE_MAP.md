# Battle reverse-engineering map (European ARMP build)

This map was recovered from the verified European ROM with SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`. Addresses combine resident ARM9
and runtime overlay 2.

## Navigation anchors

| Address | Working name | Purpose |
|---:|---|---|
| `020726B0` | `BattleTaskQueue_Enqueue` | Appends a callback/argument pair to the 32-entry battle task queue |
| `02065E30` | `BattleScreenEffect_StartSecondary` | Starts or defers a secondary screen-effect preset |
| `02065EC4` | `BattleScreenEffect_StartPrimary` | Starts or defers the primary screen-effect presets used by hits and KO |
| `02065F58` | `BattleScreenEffect_UpdateDelayed` | Counts down and dispatches a deferred screen effect |
| `02065FCC` | `BattleScreenEffect_FindFreeDelaySlot` | Finds one of four deferred-effect records |
| `02073068` | `BattleMain_Update` | Per-frame battle update and central turn-state owner |
| `02076B0C` | `BattlePosition_StoreViewRelative` | Stores coordinates raw or relative to one of two battle-view offsets |
| `02076F44` | `BattleActor_GetPartySlot` | Resolves party IDs 56-59 through the battle context |
| `02076F24` | `BattleActor_GetEnemySlot` | Resolves enemy IDs 60-67 through the battle context |
| `02076F64` | `BattleActor_GetById` | Resolves party IDs 56-59 or enemy IDs 60+ to actor pointers |
| `02076BD4` | `BattleActor_CanReceiveStatus` | Rejects dead, unloaded, or inactive-form status targets |
| `02076FB4` | `BattleSceneObject_GetById` | Resolves field, party, enemy, and auxiliary visual-object IDs |
| `02077058` | `BattleObjectData_GetLoadState` | Resolves a 48-byte object-resource load state |
| `02091198` | `BattleSceneObject_SetAnimation` | Selects, creates, stops, or starts a scene-object animation |
| `02091A18` | `BattleSceneObject_IsAnimationChannelActive` | Tests one of four per-object animation slots |
| `02091A58` | `BattleSceneObject_IsAnimationActiveById` | Resolves an object ID and tests its requested animation slot |
| `02091EDC` | `BattleObjectData_IsLoadPending` | Tests pending state for ordinary and large enemy data slots |
| `0207FE2C` | `BattleTurnState_Update` | Turn selection, actions, reactions, victory, and exit |
| `02079950` | `BattleAI_DispatchOpcode` | Executes loaded `BAI_*.dat` battle bytecode |
| `0207E928` | `BattleAI_UpdateReactionTask` | Runs one enemy reaction VM task to completion |
| `0207E9C0` | `BattleAI_UpdateActionTask` | Runs one enemy action VM task to completion |
| `0207EA58` | `BattleAITask_GetOrInsert` | Finds or inserts a task in actor-ID order |
| `0207E684` | `BattleAI_TryClearOrderWait` | Resumes a paused VM state once no earlier action or reaction task blocks it |
| `0207E430` | `BattleAITask_StopById` | Stops one sorted task and invalidates its attached VM state |
| `0207E4F8` | `BattleAI_StopScriptById` | Decodes party or typed task IDs and routes script cancellation |
| `0207E7A0` | `BattleAI_HandleVmResult` | Completes a task or activates its saved continuation script |
| `0207E820` | `BattleAI_UpdateChainedTask` | Runs an object VM and any continuation activated in the same frame |
| `0207E8C8` | `BattleAI_UpdateAuxTask` | Runs one actor-embedded auxiliary VM task |
| `0207ECA8` | `BattleAI_StartReactionScript` | Starts an enemy reaction state with mode `0x2000` |
| `0207ECC0` | `BattleAI_StartActionScript` | Starts an enemy action state with mode `0x1000` |
| `0207EB14` | `BattleAI_StartObjectScript` | Starts or queues a `0x4000`-family object script |
| `0207EC1C` | `BattleAI_StartActorAuxScript` | Starts a `0x3000`-family actor auxiliary script |
| `0207ED70` | `BattleAI_StartScriptTask` | Initializes and attaches an enemy AI VM state |
| `0207EE1C` | `BattleAI_InitStateFromScriptBlock` | Resets a VM state and selects the stream encoded by a script block |
| `0207EE54` | `BattleAI_StartScriptById` | Routes fixed party slots and typed IDs to their script starters |
| `0207EF10` | `BattleAI_UpdateAll` | Runs all fixed party VMs and all four typed task lists |
| `0207F01C` | `BattleAI_TaskPoolsInit` | Initializes the four battle-AI task pools |
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
| `020681E8` | `BattleModel_GetAnimationBounds` | Decodes collision bounds from the current or temporarily selected animation frame |
| `0209E918` | `BattleCollision_GetBounds` | Resolves party, special-object, or resource collision bounds |
| `0209EBAC` | `BattleHitDescriptor_GetByActorId` | Resolves per-actor 16-byte hit descriptors |
| `0209EBFC` | `BattleCollision_TestObjects` | Tests all source/target bounds and returns a hit position |
| `0209EF3C` | `BattleCollision_TestVolumes` | Swept six-axis overlap and time-of-impact calculation |
| `020A3370` | `BattleSceneObject_GetActiveModel` | Selects a scene object's primary or alternate model pointer |
| `020A3310` | `BattleSceneObject_SetModelFlag11ById` | Resolves the active model and updates its currently unknown bit 11 |
| `020A3338` | `BattleSceneObject_SetModelFlag10` | Object-pointer wrapper for the active model's currently unknown bit 10 |
| `020A3348` | `BattleSceneObject_SetModelFlag10ById` | Resolves the active model and updates its currently unknown bit 10 |
| `020A3388` | `BattleSceneObject_GetActiveModelById` | Resolves an object ID and returns its primary or alternate model |
| `020A3F9C` | `BattleMotion_StartBallistic` | Derives launch velocity with the DS square-root unit and starts motion |
| `020A411C` | `BattleSceneObject_StartAcceleratedMotion` | Normalizes a path and solves constant or accelerated duration |
| `020A43D8` | `BattleSceneObject_UpdateAcceleratedMotion` | Advances normalized acceleration and applies the terminal correction |
| `020A483C` | `BattleSceneObject_MoveTo` | Moves immediately or interpolates toward absolute coordinates |
| `020A48AC` | `BattleSceneObject_UpdateMoveTo` | Advances absolute-target interpolation by one frame |
| `020A4934` | `BattleSceneObject_MoveBy` | Applies a position delta immediately or over a duration |
| `020A4A4C` | `BattleSceneObject_UpdateTravelDistance` | Averages the frame displacement into the object's travel metric |
| `020A4ADC` | `BattleSceneObject_SnapshotPosition` | Copies live coordinates into the stored motion origin |
| `020A4AF8` | `BattleSceneObject_MoveByImmediate` | Applies a delta and synchronizes both stored targets |
| `020A4B50` | `BattleSceneObject_AddPositionDelta` | Offsets live and target coordinates during active motion |
| `020A4B9C` | `BattleSceneObject_AdjustPosition` | Selects active-motion or immediate coordinate adjustment |
| `020A4BF4` | `BattleSceneMotion_UpdateAll` | Runs every active object's four motion callback channels |
| `020A4E08` | `BattleSceneObject_BeginMotionChannel` | Links an object and initializes one motion callback channel |
| `020A4EB0` | `BattleSceneObject_UnlinkMotion` | Preserves the final target and removes an object from the motion list |
| `020A4F18` | `BattleSceneObject_StopMotionChannel` | Stops a channel and reconciles accumulated coordinate deltas |
| `020A50C4` | `BattleSceneObject_GetMotionChannel` | Resolves one of the fixed-size per-object motion channels |
| `020A50D4` | `BattleTaskList_Update` | Runs live callbacks and recycles stopped tasks |
| `020A519C` | `BattleTask_BindOwnerSlot` | Binds a task handle to its owning object and returns the displaced task |
| `020A51F8` | `BattleTaskList_Insert` | Allocates if needed and prepends a task to an active list |
| `020A5254` | `BattleTaskPool_Init` | Builds an aligned fixed-payload task free list |
| `020A90F4` | `BattleParty_UpdateKnockout` | Completes party knockout and linked-character transitions |
| `020A87F4` | `BattleParty_UpdateLuigiReloadFinish` | Rebinds Luigi/Baby Luigi and completes linked KO recovery |
| `020A8990` | `BattleParty_UpdateLuigiReloadRequestBaby` | Requests Baby Luigi after the battle transition state |
| `020A89EC` | `BattleParty_UpdateLuigiReloadWaitRetreat` | Transfers locks after Luigi leaves the battle view |
| `020A8AA4` | `BattleParty_UpdateLuigiReloadStartRetreat` | Starts Luigi's timed off-screen retreat |
| `020A8B80` | `BattleParty_UpdateLuigiReloadWaitAnimation` | Advances Luigi's KO recovery after the rebound animation is ready |
| `020A8BEC` | `BattleParty_UpdateLuigiReloadWaitResource` | Waits for and binds Luigi's rebound resource |
| `020A8C74` | `BattleParty_UpdateMarioReloadFinish` | Rebinds Mario/Baby Mario and completes linked KO recovery |
| `020A8E10` | `BattleParty_UpdateMarioReloadRequestBaby` | Requests Baby Mario after the battle transition state |
| `020A8E6C` | `BattleParty_UpdateMarioReloadWaitRetreat` | Transfers locks after Mario leaves the battle view |
| `020A8F24` | `BattleParty_UpdateMarioReloadStartRetreat` | Starts Mario's timed off-screen retreat |
| `020A8FEC` | `BattleParty_UpdateMarioReloadWaitAnimation` | Advances Mario's KO recovery after the rebound animation is ready |
| `020A906C` | `BattleParty_UpdateMarioReloadWaitResource` | Waits for and binds Mario's rebound resource |
| `020A9280` | `BattleParty_StartKnockout` | Starts party knockout state, animation, sounds, and task |
| `020A9C18` | `BattleParty_SpawnLaunchImpact` | Emits the form-specific launch impact effect and sound |
| `020ACB44` | `BattleModelEffect_SpawnAttached` | Creates a model effect bound to an owner slot |
| `020ACB88` | `BattleModelEffect_Spawn` | Creates a positioned model effect from its resource table |
| `020ACBF0` | `BattleSpriteEffect_SpawnInFreeSlot` | Creates a sprite effect in the first free tracked slot |
| `020ACCB8` | `BattleSpriteEffect_Spawn` | Creates a sprite effect at view-adjusted coordinates |
| `0209C464` | `BattleStatus_TryApply` | Ailments and POW/DEF/SPD percentage changes |
| `0209C278` | `BattleStatus_ClearEffect` | Clears an effect and restores a base stat |
| `020A8320` | `BattleStatus_StopActorEffect` | Stops the actor-local timer or stat delta for one status ID |
| `02076584` | `BattleItemEffect_Apply` | Healing, revival-style HP updates, status items |
| `020768A4` | `BattleItemEffect_ApplyBadgeBoost` | Applies equipped 150/200-percent healing multipliers |
| `02018F48` | `ItemEffect_CalculateValue` | Resident item-record value calculation and HP clamps |
| `0208908C` | `BattleEnemyData_RequestLoad` | Initializes and queues one enemy-data request |
| `02089EEC` | `BattleObjectData_QueueLoad` | Resets an object-data state and queues its asynchronous loader |
| `02092048` | `BattleObjectData_EnsureLoaded` | Routes ordinary and enemy resource requests while suppressing duplicates |
| `020922DC` | `BattleScriptState_GetByObjectId` | Resolves an object ID to its fixed 192-byte VM state |
| `0209234C` | `BattleObjectData_ResolveSlot` | Decodes a packed object ID into a 44-byte runtime descriptor |
| `020A32F4` | `BattleSceneObject_SetStateFlags` | Sets the low state byte and independent bit-18 flag |

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

`BattleActor_IsHpAtMostQuarter` and the contiguous actor, scene-object, and
object-load-state lookup layer are maintained as byte-identical C. The shared
`BattleActor` declaration now
types the confirmed `max_hp` and `current_hp` fields at `+0x04/+0x06`, giving
later damage, healing, and KO translations a common high-level representation.
`BattleActor_CanReceiveStatus` is matching C too: defeated actors are rejected,
enemies require a bound resource slot, adults are always eligible, and the two
baby slots depend on save-state halfword `+0x558` being 2.

Actor IDs 56-59 are party slots. IDs 60-67 are enemy slots. Do not confuse
battle actors with visual scene objects, whose offsets `+04/+06/+08` are
coordinates rather than HP or stats. `BattleSceneObject_GetById` resolves IDs
below 56 through the field-object table, 56-59 through party slots, 60-67
through enemy slots, and IDs from 68 through the auxiliary table. The
object-data load-state resolver indexes opaque 48-byte records at battle-context
offset `+0xD3C8`. The active
model helper then selects scene-object pointer `+0xC0` or `+0xC4` from flag bit
14 at `+0xF4`. `BattlePosition_StoreViewRelative` accepts a raw-coordinate
bypass flag and otherwise subtracts either context offset pair
`+0xCB9C/+0xCB9E` or `+0xCBA0/+0xCBA2`; its stored depth is clamped at zero.
All actor resolvers, the compact base-damage calculation, and the damage/KO
updater are now maintained byte-identical C.
The state-bit setter, active-model selectors, model-bit setters, position
snapshot, and fixed-stride motion-channel accessor are byte-identical C as
well. Their common scene-object
type records positions at `+0x04..+0x0E`, four `0x28`-byte motion channels at
`+0x1C`, models at `+0xC0/+0xC4`, actor ID `+0xEC`, and flags at `+0xF4`.

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

The matching C function resolves both actor IDs, extracts the attacker's level
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
Those follow-ups now expose the whole asynchronous boundary explicitly. They
wait for resource slots 5 or 6, bind them to scene objects 56 or 57, play the
rebound animation, interpolate an off-screen retreat on motion channel 3,
transfer the actor lock to Baby Mario or Baby Luigi, and wait for battle state
`0x200A`. The final stage loads resource slot 0 or 1, copies the adult's
position to the baby scene object, rebinds both actors, and clears the task.
If the adult's equipped-effect byte resolves to `0x3024`, it also reapplies
status 6 at magnitude 40 and clears the actor's one-shot byte at `+0x51`.

The launch reaction's movement helper either updates both live and base
coordinates immediately or installs a fixed-point per-frame interpolation
callback. The adjacent absolute-target helper performs the same immediate or
timed choice, but each update interpolates the remaining distance toward its
stored X/Y/Z target. Its ballistic companion derives the signed initial velocity from
displacement and acceleration with the DS square-root registers at
`0x040002B0`-`0x040002B8`. The final impact helper chooses effect variant
`0x10` or `0x11` from the party form, converts through the maintained view
offset helper, creates effect family `0x13`, and plays sound `0x39`.
Scene objects with active motion are kept in the intrusive list rooted at
`gBattleMotionObjectList`. Each object owns fixed-size `0x28`-byte motion
channels beginning at offset `+0x1C`; starting a channel replaces an existing
callback, clears its transient accumulators, stores its duration, and returns
the channel payload at `+0x18`. List insertion/channel initialization and
unlinking are byte-identical C; the more branch-sensitive channel-stop routine
remains maintained symbolic assembly. Coordinate adjustment preserves interpolation
targets for listed objects, while unlisted objects receive an immediate move.
The absolute/relative move constructors, their fixed-point update callbacks,
travel-distance smoothing, snapshot, immediate move, active-motion delta, and
list-aware adjustment helpers are byte-identical C. Their shared scene-object
layout now names the intrusive next pointer, three stored target coordinates,
motion channels, and smoothed travel distance.
The common per-frame updater snapshots each object's coordinates, advances its
four callbacks, clamps timed channels, transfers deferred deltas between
overlapping channels, removes idle objects, and updates a smoothed travel
distance using the DS square-root unit.

The accelerated-motion primitive first normalizes a requested X/Y/Z vector
with the DS square-root unit. With zero acceleration it derives duration from
distance and speed; otherwise it solves the fixed-point quadratic, selects the
requested positive root, and installs the normalized direction plus speed and
acceleration in the channel payload. Its update callback integrates that
scalar along all three axes and applies a separate final-frame correction so
rounding cannot leave the object short of its requested displacement.

The hit queue at battle-context offset `0xCAD8` contains up to eight packed
20-byte records. `BattleDamage_ReflectQueuedHits` stops at the first inactive
entry, finds records targeting the requested actor, recalculates damage for the
reversed pairing, swaps source and target, and reloads the reflected attack's
status ID, chance, and magnitude. The reflector and effect-variant setter are
now byte-identical C, backed by a size-checked `BattleHitRecord`; actor offset
`+0x1E` is correspondingly named `pending_damage`.

Before expansion into that queue, each active attacker owns a 16-byte hit
descriptor in the battle-context table at `+0xC8F4`. The maintained configure,
lookup, status-payload, and disable helpers expose its linked-list pointer,
callback, source/target IDs, six-bit hit kind, seven-bit status ID, signed
chance/magnitude bytes, and the active-list head at context offset `+0xCAD4`.
The lookup, status-payload, direct-disable, actor-ID-disable, and full configure
paths are now byte-identical C with a size-checked `BattleHitDescriptor`.
The maintained `BattleHitQueue_Update` is the complete bridge between both
representations. Each update first invokes callbacks for the previous queue,
then walks and unlinks active descriptors, expands party and enemy wildcard
targets, collision-tests candidates, copies status/callback payloads, removes
conflicting chains and duplicate targets, calculates pending damage, handles
negative/reversed damage, and leaves at most eight records for the next pass.
When both the current actor and a computed damage target are enemies, the queue
compiler starts the target's reaction script. Action and reaction modes use
separate task pools and separate 184-byte actor-local VM states at actor offsets
`+0x70` and `+0x128`. Tasks remain sorted by actor ID; their update callbacks
honor the VM pause bits at state `+0xB2`, call `VM_Run`, and clear themselves
when it returns completion code 1 or 2.
When pause bit 1 requests ordering, `BattleAI_TryClearOrderWait` scans both
active task lists. It ignores the same actor, empty scripts, and bit-0-disabled
states, then compares the signed order field at `+0xB4` with an actor-ID tie
break. It clears the wait bit and permits `VM_Run` only after every earlier
live task has passed.
`BattleAI_StopScriptById` is the inverse routing layer. IDs 1-4 clear the four
party VM pointers at context offsets `+0x6A64`, `+0x6B1C`, `+0x6BD4`, and
`+0x6C8C`; high nibbles `0x1000` through `0x4000` select the four task pools.
The sorted-list helper stops at IDs greater than the target, clears the found
task's state script pointer, and releases the task node.
The `0x4000` family uses one fixed 192-byte state per battle object. Starting
over can instead queue a continuation at state `+0xB8`, with its order and
tie-break fields at `+0xBC/+0xBE`; VM completion code 2 promotes those values
and immediately runs the continuation. The `0x3000` family uses the same VM
header embedded at enemy-actor offset `+0x1E0`, but completes without the
continuation loop. `BattleScriptState_GetByObjectId` maps valid object IDs into
the table at context offset `+0x6D44` with a `0xC0`-byte stride.
`BattleAI_HandleVmResult`, all four fixed party VM starters, both enemy
action/reaction start wrappers, the VM state initializer, and
`BattleScriptState_GetByObjectId` are the first overlay-2 battle functions
promoted from matching assembly into byte-identical C. Their shared header
records the confirmed `BattleAITask` size (`0x14`) and the relevant fields of
the `0xC0`-byte VM state. DSD delinks cohesive, contiguous source modules and
verifies the rebuilt overlay against the original.
The generic battle-task pool is matching C as well. It covers free-list
initialization/allocation/return, active-list insertion, owner-slot binding, and
safe task release. `BattleAITask` is its 12-byte generic header followed by the
eight-byte AI-specific payload already described above.
`BattleAI_StartScriptById` is the common public router. IDs 1-4 select four
fixed party VM states, while typed IDs currently route `0x1000` and `0x2000`
to enemy action and reaction starters. The fixed-state initializer clears
184 bytes, derives the executable stream from the first script-block halfword,
and records the slot ID at `+0xB0`; slot 2 deliberately has a no-op starter and
is serviced by its dedicated update helper. `BattleAI_UpdateAll` runs all four
fixed states, then action, reaction, auxiliary, and object task lists. Pool
initialization reserves eight action, eight reaction, eight auxiliary, and 40
object-script task nodes, each with an eight-byte payload.
`BattleCollision_GetBounds` supplies the queue compiler with six signed
halfwords. It contains explicit body-size presets for the six party-member
forms and object IDs 8-9, while ordinary battle objects obtain their bounds
from the bound resource and receive the original coordinate-axis conversion.
The resource-backed path now continues through
`BattleModel_GetAnimationBounds`. That function can advance or rewind the live
model by a signed frame offset, decode the chosen frame's packed signed origin
and extent metadata, optionally scale centered bounds through the model table,
and then restore the model state. Invalid or missing frame metadata produces
the original fallback rectangle `[-16, -32, 16, 0]`.
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
as one unit. The complete 182-entry range, handler addresses, aliases,
observed command-record fields, and direct calls are indexed in
[`BATTLE_AI_OPCODES.md`](BATTLE_AI_OPCODES.md). Regenerate it from a matching
ROM with:

```powershell
python tools\analyze_battle_ai_dispatch.py `
  --rom 'C:\path\to\your\PiT.nds' --version eur `
  --format markdown --output docs\research\BATTLE_AI_OPCODES.md
```
