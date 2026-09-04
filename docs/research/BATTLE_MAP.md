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
| `02076EE8` | `BattleEnemy_GetStats` | Returns the loaded stat record bound through an enemy actor's resource slot |
| `02076F64` | `BattleActor_GetById` | Resolves party IDs 56-59 or enemy IDs 60+ to actor pointers |
| `02076BD4` | `BattleActor_CanReceiveStatus` | Rejects dead, unloaded, or inactive-form status targets |
| `02076FB4` | `BattleSceneObject_GetById` | Resolves field, party, enemy, and auxiliary visual-object IDs |
| `02077058` | `BattleObjectData_GetLoadState` | Resolves a 48-byte object-resource load state |
| `02087B88` | `BattleMath_WaitForSqrtResult` | Waits for and reads the DS square-root coprocessor result |
| `02087BAC` | `BattleMath_StartSqrt` | Starts a 32-bit DS hardware square root |
| `02087BCC` | `BattlePosition_StoreBattleRelative` | Converts a battle anchor and offsets through the active view |
| `02087C14` | `BattleSceneObject_SetBattleAnimation` | Applies the standard battle animation and model flags to an object |
| `02087C58` | `BattleSceneObject_SetBattleAnimationById` | Object-ID wrapper for standard battle animation setup |
| `02087CE4` | `BattleFieldAssets_RequestReload` | Queues the current map's field-asset reload when battle exits |
| `02087D98` | `BattleBackground_RequestToggle` | Queues a swap of the double-buffered 3D battle background |
| `02087E5C` | `BattleBackground_RequestLoad` | Queues a battle-background load into the inactive buffer |
| `02087F98` | `BattleCommonAssets_LoadEntriesTask` | Relocates the battle archive table and loads its localized/common entries |
| `0208848C` | `BattleCommonAssets_RequestLoad` | Starts the asynchronous common battle-asset load pipeline |
| `020884E8` | `BattleInterface_InitLayersTask` | Initializes four battle-interface layers and their fixed VRAM regions |
| `02088A34` | `BattleInterface_RequestLoad` | Starts the shared/localized battle-interface asset pipeline |
| `02088BC4` | `BattleInterface_RequestScreenLoad` | Loads one of the two selectable battle-screen resources |
| `02088CAC` | `BattlePartyScript_RequestLoad` | Resolves and asynchronously loads a packed party-script resource |
| `02088E44` | `BattleActionScript_RequestLoad` | Marks an action pending and queues its battle-AI script load |
| `02091118` | `BattleSceneObject_SetAnimationFromComponent` | Temporarily selects one packed resource component and starts its animation |
| `02091198` | `BattleSceneObject_SetAnimation` | Selects, creates, stops, or starts a scene-object animation |
| `02091A18` | `BattleSceneObject_IsAnimationChannelActive` | Tests one of four per-object animation slots |
| `02091A58` | `BattleSceneObject_IsAnimationActiveById` | Resolves an object ID and tests its requested animation slot |
| `02091A90` | `BattleSceneObject_ApplyMovement` | Dispatches one of seven scene-object movement modes |
| `02091C20` | `BattleEntity_BindResource` | Binds an ordinary visual resource or initializes an enemy actor from loaded stats |
| `02091EDC` | `BattleObjectData_IsLoadPending` | Tests pending state for ordinary and large enemy data slots |
| `02091F68` | `func_ov002_02091f68` | Validates an ordinary slot and queues its alternate archive-load path |
| `02091FD8` | `func_ov002_02091fd8` | Validates an ordinary slot and queues a load with flag 29 set |
| `02092048` | `BattleObjectData_EnsureLoaded` | Suppresses duplicate loads and routes ordinary versus enemy data requests |
| `0209210C` | `BattleObjectData_ConfigureLoad` | Stores the packed resource index and configurable flag 28 |
| `02092184` | `BattleObjectData_AllocateLoadBuffer` | Selects a slot heap, allocates its payload, and initializes stream state |
| `020927A0` | `BattleTexture_EncodeHeight` | Encodes an 8-1024-pixel T dimension for `G3_TEXIMAGE_PARAM` |
| `0209285C` | `BattleTexture_EncodeWidth` | Encodes an 8-1024-pixel S dimension for `G3_TEXIMAGE_PARAM` |
| `02092918` | `BattleInterface_LoadItemName` | Resolves typed ordinary/Bros. item IDs and refreshes the localized item-name layer |
| `02092A2C` | `BattleInterface_LoadCommandMenu` | Selects a command-menu resource from the active party member and formation mode |
| `02092B04` | `BattleInterface_LoadTargetLabelResource` | Loads a target-label resource and refreshes its interface layer |
| `02092B8C` | `BattleInterface_LoadTargetName` | Maps battle-menu states or enemy `name_id` values to target-label resources |
| `02093C00` | `BattleInterface_ProcessResourceTask` | Streams rows from the selected interface resource and completes its layer upload |
| `02093D30` | `BattleInterface_ApplyResourceTask` | Returns the pooled request, configures the layer, and advances to row processing |
| `02093EA8` | `BattleInterfaceLayer_SetResource` | Suppresses duplicate resources and resets the layer transition state |
| `02093F30` | `BattleInterfaceLayer_QueueResource` | Packs layout/render flags into a pooled asynchronous layer request |
| `0209401C` | `BattleLevelUpBonus_MapPhaseToValue` | Maps the eight-position bonus wheel to a clamped 1-6 stat increase |
| `020940E0` | `BattleLevelUpBonus_UpdateRisingObject` | Moves the selected-bonus object toward its result position and retires its callback |
| `02094174` | `BattleLevelUpBonus_UpdateAppliedStats` | Counts the selected bonus into HP, POW, DEF, SPEED, or STACHE and commits the active stats |
| `02094408` | `BattleLevelUpBonus_StartApplying` | Transitions the stopped wheel into its delayed point-application task |
| `02094FB0` | `BattleLevelUpScreen_UpdateEntrance` | Moves the level-up panel toward its initial screen position and hands off to the next phase |
| `0209513C` | `BattleLevelUpScreen_UpdateController` | Drives the level-up screen's main presentation task |
| `02095648` | `BattleLevelUpScreen_Start` | Creates the level-up screen tasks, binds its six visual objects, and initializes layout coordinates |
| `02095928` | `BattleLevelUpGrowth_UpdateStatRow` | Animates one level-up stat row and draws its old value plus growth delta |
| `02095C34` | `BattleLevelUpGrowth_UpdateRowSpawner` | Starts the five stat rows at three-frame intervals |
| `02095CE0` | `BattleLevelUpGrowth_StartDisplay` | Computes cumulative-table growth deltas and starts the row sequence |
| `02095F6C` | `BattleResults_DrawPartyExperienceRow` | Draws one party member's current experience and points remaining to the next level |
| `020960AC` | `BattleResults_DrawScreen` | Draws the post-battle party experience, earned experience, earned coins, and coin total |
| `02096878` | `BattleResults_UpdateCounters` | Updates result-screen fade intensity and synchronizes its animated EXP/coin counters with save data |
| `02096990` | `BattlePartyIndicator_Update` | Fades and animates a party indicator as the active adult/baby group or actor availability changes |
| `02096BD4` | `BattleTargetOverlay_Draw` | Draws the focused target cursor and the additional eligible-target markers for enemy or party selection |
| `0207FE2C` | `BattleTurnState_Update` | Turn selection, actions, reactions, victory, and exit |
| `02079320` | `BattleVM_WriteVariable` | Writes target IDs and battle-wide script variables in namespace `0x4000` |
| `020793D8` | `BattleVM_ReadVariable` | Reads battle owner/target IDs, masks, and shared script variables |
| `0207959C` | `BattleActor_SelectRandomStatusTarget` | Uniformly selects one valid status target from an actor-ID range |
| `02079624` | `BattleActor_FindMostDamagedEnemy` | Finds the loaded enemy with the greatest missing HP above a threshold |
| `020796BC` | `BattleActor_FindLowestHpEnemy` | Finds the loaded enemy with the lowest HP at or below a threshold |
| `02079740` | `BattleActor_FindHighestHpEnemy` | Finds the loaded enemy with the highest HP at or above a threshold |
| `020797C4` | `BattleAI_GetStateById` | Resolves party slots and all four typed Battle-AI state families |
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
| `0208DEC0` | `BattleAIArchives_Load` | Allocates and opens all fourteen battle-AI archives |
| `0208DFD0` | `BattleAI_Initialize` | Initializes the shared VM, archive requests, and AI task pools |
| `0208E034` | `BattleSpecialHandle_ClearTask` | Clears the special-handle owner slot after its task completes |
| `0208E048` | `BattleSpecialHandle_ReloadTask` | Waits for the active resource before requesting a special-handle reload |
| `0208E080` | `BattleSpecialHandle_QueueReload` | Queues the special-handle reload task |
| `0208E098` | `BattleActor_IsHitLocked` | Tests an actor's hit-lock flag |
| `0208E0C4` | `BattleAI_UpdateControlMask` | Sets or clears one bit in the shared AI control mask |
| `0208E10C` | `BattleActor_IsAnyHitLocked` | Tests both active party slots and all six enemy hit locks |
| `0208E238` | `BattleEnemy_Remove` | Removes an enemy with optional animation and damage-number feedback |
| `0208E3BC` | `BattleSceneObject_ConfigureAnimationLayer` | Configures one active-model animation layer through its virtual interface |
| `0208E3F0` | `BattleScriptHandle_IsActive` | Tests one entry in any of the three typed battle-handle families |
| `0207F080` | `BattleReward_ClearCounterEffects` | Stops and detaches the post-battle coin/experience counter effects |
| `0207F100` | `BattleReward_AdvanceCounterEffect` | Replaces a completed counter sprite while preserving its owner slot |
| `0207F17C` | `BattleReward_EnsureCounterEffect` | Creates the positioned coin or experience tally effect when absent |
| `0207F3A0` | `BattleParty_ShowHealingEffect` | Shows the party healing sprite, model, number, and sound feedback |
| `0207F5A0` | `BattleParty_UpdateFormationTransition` | Advances the paired-party animation, resource swap, and return motion task |
| `0207F920` | `BattleParty_StartFormationTransition` | Selects and loads an adult party formation resource and starts its transition |
| `0207FC78` | `BattleParty_AddExperience` | Adds capped experience, crosses level thresholds, and updates the remaining requirement |
| `0208ED90` | `BattleScript_GetProperty` | Reads actor, object, and global properties |
| `0208FB6C` | `BattleScript_SetProperty` | Writes actor stats, HP, positions, model/animation flags, and scene channels |
| `02090C18` | `BattleSceneObject_SwapSlots` | Exchanges field/actor scene bindings and repairs their object IDs |
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
| `020A3C5C` | `BattleSceneObject_StartAcceleratedMotionForDuration` | Starts normalized acceleration over an explicit duration and speed range |
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
| `020890B4` | `BattleObjectData_RebuildNextComponentTask` | Serializes one model component and finalizes the destination resource |
| `020891D8` | `BattleObjectData_BeginRebuildTask` | Allocates a model and prepares its destination component table |
| `02089300` | `BattleObjectData_QueueLoadAndMarkPending` | Queues a resource ID and marks its destination load pending |
| `02089320` | `BattleObjectData_CopyResource` | Copies a loaded resource, rebases its internal pointers, and schedules upload |
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
| `+7E` | `formation_index` | Selects party-form effects and one of six adult visual resources |
| `+80` | `linked_object_id` | Resolves the paired adult/baby scene object during transitions |

`BattleActor_IsHpAtMostQuarter` and the contiguous actor, scene-object, and
object-load-state lookup layer are maintained as byte-identical C. The shared
`BattleActor` declaration now
types the confirmed `max_hp` and `current_hp` fields at `+0x04/+0x06`, giving
later damage, healing, and KO translations a common high-level representation.
`BattleActor_CanReceiveStatus` is matching C too: defeated actors are rejected,
enemies require a bound resource slot, adults are always eligible, and the two
baby slots depend on save-state halfword `+0x558` being 2.
The byte-identical reward/effect unit creates and tears down the coin and
experience counter visuals during the post-battle tally. Its healing helper
uses a party-form offset table to place sprite, model, and number effects,
attaches the positive-heal sparkle to the appropriate adult, and plays sound
333. The reconstructed `BattlePosition` record is eight bytes; scene-object
offset `+0xEA` is its signed effect-height anchor, distinct from `actor_id` at
`+0xEC`.
`BattleParty_AddExperience` at `0x0207FC78` is matching C. It exposes the four
36-byte save-party records, their packed 24-bit total/remaining-experience
fields, the four 12-byte level-growth tables, the 999,999 experience cap, and
the level-100 cap. The save record's now-typed prefix contains the character's
base HP, POW, DEF, SPEED, and STACHE at `+0x02` through `+0x0A`, followed by the
active max-HP/current-HP and four active combat stats at `+0x0C` through
`+0x16`.

The byte-matching level-up bonus unit at `0x0209401C` maps the eight animated
wheel phases to the displayed 1-6 reward, moves the stopped result object, and
applies one selected stat point every eight ticks. It increments the selected
base stat in the current 36-byte save-party record, then copies all five base
stats into their active counterparts after the result delay. This identifies
the low seven bits of the wheel's selector as HP, POW, DEF, SPEED, and STACHE
indices `0` through `4`. `BattleLevelUpBonus_StartApplying` is semantically
named but remains on the reference object because its otherwise equivalent C
translation differs by one compiler-generated address instruction.

The matching level-up display unit at `0x02095648`-`0x02095E00` reads the
current level from the low byte of the save member's packed experience word.
It selects the character-specific cumulative growth table, subtracts the
preceding two level rows to obtain HP/POW/DEF/SPEED gains, sets the STACHE gain
to zero, and spawns all five result rows three frames apart. Its matching row
callback at `0x02095928` animates and draws each old stat value and growth
delta.

The byte-identical target-interface renderer at `0x02096BD4` uses the model's
virtual render methods, explaining why this source unit is C++ while its
neighboring state callback remains ordinary C-style code. It positions a
focus cursor on the selected actor, draws secondary markers over the other
eligible enemies or party members, and supports a uniform-marker mode that
includes the active actor. Actor flag bit 13 excludes an actor from these
target markers.

The neighboring formation pair at
`0x0207F5A0`-`0x0207FC78` has a readable semantic C translation: it selects one
of twelve adult-party resources, stages object-data slot 52, coordinates the
adult/baby animation pair, and schedules the movement callback. Its functions
currently match 88.84 and 87.38 percent respectively, so exact builds continue
to link their original delinked reference object.

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
The matching utility wrapper at `0x02087BCC` converts a battle anchor into that
view-relative coordinate system. The adjacent animation wrappers select the
standard model mode, while two leaf functions encapsulate the asynchronous DS
square-root registers.
The next matching task group owns double-buffered background changes. It loads
the configured background ID from context offset `+0x3A`, uses runtime flag bit
19 as the inactive-buffer selector, fades/toggles the resident scene, and uses
bit 8 as the pending marker. The neighboring battle-exit task rebuilds the
field asset set from the signed current-map ID at save-state offset `+0x55A`.
The following six-function common-asset pipeline is matching C too. Its
asynchronous tasks open the archive referenced at context offset `+0xE160`,
read an eight-byte header followed by its offset table, relocate every table
word to the selected archive member, and stream entries into a contiguous
buffer. The loader fills twenty-one pointers at `+0x68FC`; slot zero uses a
resident fallback, slots 1-13 select language-specific entries from save byte
`+0x515`, and slots 14-20 use shared entries except for the final localized
one. Runtime flag bit 7 marks this pipeline pending. The exact content types of
the individual slots remain to be named from their consumers.
The next eight matching functions load the battle interface itself. They open
one common archive, select entry 3 or 8 from save language byte `+0x515`, load
shared entry 4, and initialize four 2D-layer records at context offsets
`+0x65F4`, `+0x6648`, `+0x669C`, and `+0x66EC`. Those records expose allocation
size, position, dimensions, and fixed VRAM offsets `0x38000` through `0x3CC00`.
A sibling two-request pipeline selects one of two archive descriptors and
stores its loaded screen resource at context `+0x44` or `+0x48`. The allocations
use the battle runtime's heap selector at `+0xE148`.
The adjacent six-function script pipeline resolves packed object-data IDs into
the shared 44-byte runtime descriptor, rounds archive reads to four-byte
boundaries, and streams party and action BAI data into context buffers
`+0x35608` and `+0x25608`. Completion starts fixed party VM slot 4 or the
requesting action's actor VM and selects the script stream from the payload's
leading offset. Action flag bit 2 records the pending load. Map `0x2028` also
passes through a pre-load synchronization path whose exact gameplay meaning
still needs runtime confirmation.
All actor resolvers, the compact base-damage calculation, and the damage/KO
updater are now maintained byte-identical C.
The state-bit setter, active-model selectors, model-bit setters, position
snapshot, and fixed-stride motion-channel accessor are byte-identical C as
well. The guarded direct and object-ID animation-channel activity queries also
match. Their common scene-object
type records positions at `+0x04..+0x0E`, four `0x28`-byte motion channels at
`+0x1C`, models at `+0xC0/+0xC4`, actor ID `+0xEC`, and flags at `+0xF4`.

`BattleSceneObject_SetAnimation` is the byte-matching common animation path
used by damage and reaction code. It validates the bound resource, classifies
scene-object IDs as party or enemy, creates a model on demand, stops negative
animation requests, and starts the selected animation through the original
C++ model vtable. Party idle selection is adjusted for three actor status
values, a separate party flag, and the low-HP predicate. It also offsets linked
party formation animations in 44-frame groups, preserves the low five model
animation-state bits across resource replacement, and calls the battle scene's
model-change observer when the active model changes. The preceding component
wrapper switches the resource's five-bit component index only for the duration
of that call, then restores it without disturbing the other packed flags.

`BattleSceneObject_ApplyMovement` is the common script-facing movement
dispatcher. Mode 0 adjusts an object to absolute coordinates while respecting
an already active motion-list entry. Modes 1 and 2 select relative and absolute
linear interpolation. Modes 3 and 4 select the two vertical ballistic solvers,
mode 5 keeps an offset relative to another scene object, and mode 6 configures
an accelerated motion channel. The dispatcher validates the 70-entry scene
object index before resolving it.

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
when it returns completion code 1 or 2. All four update callbacks and the
common sorted find-or-insert helper are now byte-identical C in one cohesive
source unit.
When pause bit 1 requests ordering, `BattleAI_TryClearOrderWait` scans both
active task lists. It ignores the same actor, empty scripts, and bit-0-disabled
states, then compares the signed order field at `+0xB4` with an actor-ID tie
break. It clears the wait bit and permits `VM_Run` only after every earlier
live task has passed. Its behavior is named and documented, but the function
remains symbolic assembly until readable C reproduces its original register
allocation exactly.
`BattleAI_StopScriptById` is now byte-identical C for the inverse routing
layer. IDs 1-4 clear the four
party VM pointers at context offsets `+0x6A64`, `+0x6B1C`, `+0x6BD4`, and
`+0x6C8C`; high nibbles `0x1000` through `0x4000` select the four task pools.
The matching C sorted-list helper stops at IDs greater than the target, clears
the found task's state script pointer, and releases the task node.
The `0x4000` family uses one fixed 192-byte state per battle object. Starting
over can instead queue a continuation at state `+0xB8`, with its order and
tie-break fields at `+0xBC/+0xBE`; VM completion code 2 promotes those values
and immediately runs the continuation. Its start and continuation setup are
now byte-identical C. The `0x3000` family uses the same VM
header embedded at enemy-actor offset `+0x1E0`, but completes without the
continuation loop; its start path is matching C too.
`BattleScriptState_GetByObjectId` maps valid object IDs into
the table at context offset `+0x6D44` with a `0xC0`-byte stride.
The same namespace is now closed at the resident/overlay boundary:
`BattleVM_ReadVariable` and `BattleVM_WriteVariable` are byte-identical C for
variables `0x4000` through `0x402F`. They expose the current state's low
12-bit owner actor ID, its high-nibble task family, the owner's target actor,
three battle-context masks, and 32 shared signed integers at context offset
`+0x69E4`. Context fields whose gameplay meaning is not yet proven retain
offset-based names rather than speculative labels.
The five adjacent dispatcher helpers are matching C as one cohesive unit.
Three scan the six ordinary enemy slots for highest HP, lowest HP, or greatest
missing HP while rejecting unloaded actors. A fourth uses reservoir sampling
to select a valid status target uniformly from a requested party/enemy range;
Baby Mario and Baby Luigi are deliberately excluded. `BattleAI_GetStateById`
maps fixed party IDs 1-4 and typed IDs `0x1000`-`0x4000` to their action,
reaction, auxiliary, or object-script VM state.
`BattleAI_HandleVmResult`, all four fixed party VM starters, both enemy
action/reaction start wrappers, the VM state initializer, and
`BattleScriptState_GetByObjectId` are the first overlay-2 battle functions
promoted from matching assembly into byte-identical C. Their shared header
records the confirmed `BattleAITask` size (`0x14`) and the relevant fields of
the `0xC0`-byte VM state. DSD delinks cohesive, contiguous source modules and
verifies the rebuilt overlay against the original.
The generic battle-task pool is matching C as well. It covers active callback
iteration, owner-slot validation, stopped-node recycling, free-list
initialization/allocation/return, active-list insertion, owner-slot binding,
and safe task release. `BattleAITask` is its 12-byte generic header followed by
the eight-byte AI-specific payload already described above.
`BattleAI_StartScriptById` is now byte-identical C for the common public router.
IDs 1-4 select four
fixed party VM states, while typed IDs currently route `0x1000` and `0x2000`
to enemy action and reaction starters. The fixed-state initializer clears
184 bytes, derives the executable stream from the first script-block halfword,
and records the slot ID at `+0xB0`; slot 2 deliberately has a no-op starter and
is serviced by its dedicated update helper. The matching C
`BattleAI_UpdateAll` runs all four fixed states, then action, reaction,
auxiliary, and object task lists. Matching pool initialization reserves eight
action, eight reaction, eight auxiliary, and 40 object-script task nodes, each
with an eight-byte payload.
The separate AI-system initializer at `0x0208DEC0`-`0x0208E10C` is matching C
too. It configures the shared `ScriptVm` at context offset `+0x6954` with
`BattleAI_DispatchOpcode` and the 260-entry command-descriptor table, allocates
and opens all fourteen BAI archives into typed resource requests, and resets
the four task pools. Its adjacent helpers implement the asynchronous
special-handle reload chain, test actor flag `0x200`, and update the shared
16-bit control mask at context offset `+0x10C`.
The following four script-command helpers are byte-identical C/C++. They scan
the two active party actors and all six enemy actors for hit-lock flag `0x200`,
remove an enemy with optional position-corrected damage feedback, invoke the
active model's animation-layer method at virtual slot `0x88`, and resolve the
`0x4000`, `0x8000`, and `0xC000` battle-handle families. The enemy-removal
path also confirms a 20-byte position record at context offset `+0xCAD8` and
its view origin at `+0xCB9C/+0xCB9E`.
`BattleGlobalProperty_Get` at `0x0208E494` is byte-identical C for battle-AI
opcode `0x46`. Its 34-value namespace exposes the active party and target
actors, save-backed map state, background ID and fade progress, shared masks,
and individual runtime flags. Unknown properties retain numeric names pending
script analysis or runtime observation. The paired 1,304-byte
`BattleGlobalProperty_Set` is maintained as structured C and reproduces every
opcode and side effect, including hit-descriptor invalidation and background
reloads; it remains on the reference object because MWCC selects different
registers for eleven instructions in the final fade-toggle case.
`BattleScript_GetProperty` at `0x0208ED90` is also byte-identical C. Its
134-entry switch is the actor/object counterpart used by opcodes `0x4D` and
`0xDE`: it exposes the core HP/POW/DEF/SPD fields, world and view-relative
positions, animation/model flags, object-data IDs, hit availability, enemy
resource state, party formation state, and several transition channels. The
checked-in BAI corpus calls this reader 7,548 times. Properties whose precise
gameplay meaning is still ambiguous retain field-offset names; this keeps the
ABI editable without turning guesses into permanent structure names.
The paired 4,268-byte `BattleScript_SetProperty` at `0x0208FB6C` is
byte-identical, linked C as well. It implements all writable members of the
same 134-value ABI, including core HP/POW/DEF/SPD changes, position and model
state, hit flags, party formation fields, enemy-private flags, and the scene
operation channels. Opcode `0x4E` invokes it 6,042 times in the checked-in BAI
corpus. Setter-only operations retain neutral numeric names until their callees
or observed in-game effects prove stronger semantics.
`BattleSceneObject_SwapSlots` at `0x02090C18` is byte-identical C for the
following opcode `0x41` helper. Its ID-range dispatch exchanges field-object
pointers or actor-owned scene-object pointers between the low and high slot
banks, rewrites both `actor_id`/`linked_actor_id` fields, and transfers actor
flag bit 7 when two party or two enemy slots themselves are exchanged.
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
| `00` | 2 | localized enemy-name ID |
| `02` | 2 | packed object-data/AI resource ID |
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

`BattleEnemyData_LoadStatRecord` is linked byte-identical C. It multiplies
the selected enemy index by 44, queues exactly that byte range from the primary
battle resource, and installs `BattleEnemyData_LoadObjectData` as the next load
callback. `BattleEnemyData_RequestLoad` initializes that request and submits it
through the matching `BattleTaskQueue_Enqueue` path. The object-data callback
uses the packed-ID resolver and queues the following 8,148-byte payload; its
matching fixup callback exposes typed stat and object-data pointers and applies
the payload's leading relative offset. The complete request occupies 0x200C
bytes, including its 12-byte header and 0x2000-byte private payload buffer.
`BattleObjectData_CopyResource` copies an existing 48-byte resource descriptor
and its allocation into another load slot. It selects a forward or backward
memory copy for overlap safety, excludes unfinished stream-writer bytes,
rebases the five component pointers against the destination buffer, preserves
slot-owned object/allocation fields, and transfers only flags 27 and 28. A
nonzero resource index whose upload-complete flag is clear is then queued for
resource upload.
`BattleObjectData_QueueLoadAndMarkPending` is the small forced-load entry used
by the ordinary-slot request path: it forwards both arguments to the common
queue loader, sets descriptor flag 29, and returns the queued task so the
caller can retain its object-data slot ID.
The adjacent rebuild callbacks turn a loaded source descriptor into the
destination's runtime component layout. The setup callback replaces the
destination slot's 440-byte polymorphic model, configures it from the packed
resource index, reads the component count from model metadata, and reserves a
leading pointer table in the destination buffer. The update callback writes one
component per task tick through the shared stream state at context `+0x68B4`.
After the final component it clears flag 30 on both descriptors and, when at
least 101 bytes remain, clears a 100-byte tail workspace and seeds its cursor.
The following object-texture pipeline is now linked matching C from
`0x020894D0` through `0x020897A0` and from `0x020899F0` through `0x02089C78`.
Its setup callbacks initialize up to three 64-byte archive requests for the
body, tail, and optional texture IDs, align every measured transfer to four
bytes, and place the later payloads directly after the preceding result. The
decode callbacks process 48 body units followed by 16 tail units and then
queue the final transfer into component `+0x14`. Completion schedules the
sprite-side upload for unshared resources and the texture-side upload for
nonzero resource slots that have not completed; either path raises the battle
system's pending-transfer bit. The texture uploader maps the resource index to
the context's bank table in 64-byte units, while the sprite uploader delegates
to the renderer path that ultimately targets DS OBJ VRAM.

`BattleObjectData_PrepareBodyDecodeTask` at `0x020897A0` is maintained as a
structured C draft beside this unit. Its component layout, allocation bounds,
100-byte stream workspace, and pending-load behavior are recovered; it remains
on the reference object because MWCC orders two equivalent conditional moves
differently. This localized compiler match is the only known code difference.
The contiguous resource-control block from `0x02091C20` through `0x020922DC`
is linked byte-identical C. `BattleEntity_BindResource`'s enemy branch
copies max/current HP, base/current POW, DEF, and SPD, installs level/trait
bits, and clears transient actor flags; its party branch binds the corresponding
scene-object resource record. The following six functions expose ordinary and
enemy pending-state checks, guarded queue entry points, duplicate suppression,
packed load configuration, four heap groups, the slot-51 allocation override,
and initialization of the 100-byte streaming workspace.

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
