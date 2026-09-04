# Battle-AI opcode catalog (European ARMP build)

This generated navigation index contains no ROM bytes. It was derived from
the verified European ROM with SHA-1 `ba4ec2f99b4f2e0047601552bccf00aa73e28701` by
`tools/analyze_battle_ai_dispatch.py`.

`BattleAI_DispatchOpcode` spans `0x4AE0` bytes at
`0x02079950`. Its ARM jump table at
`0x02079978` covers 182 opcodes
from `0x33` through `0xE8`.
Block ends are the next unique case entry and are navigation boundaries,
not independently proven function boundaries. Record offsets are direct
word loads observed from the command-record register (`r5`).

| Opcode | Handler | Aliases | Record offsets | Direct calls |
|---:|---:|---|---|---|
| `0x33` | `0x02079C50` | — | `0x8`, `0xC` | `func_ov002_02092184` |
| `0x34` | `0x02079C7C` | — | `0x8`, `0xC`, `0x10` | `func_ov002_0209210c` |
| `0x35` | `0x02079CB4` | — | `0x8`, `0xC`, `0x10` | `BattleObjectData_EnsureLoaded` |
| `0x36` | `0x02079CE8` | — | `0x8`, `0xC`, `0x10` | `func_ov002_020ad3c4` |
| `0x37` | `0x02079D38` | — | `0x8`, `0xC`, `0x10` | `func_ov002_02091fd8` |
| `0x38` | `0x02079D6C` | — | `0x8`, `0xC` | `func_ov002_02091f68` |
| `0x39` | `0x02079D90` | — | `0x8` | `BattleObjectData_IsLoadPending` |
| `0x3A` | `0x02079DF8` | — | `0x8` | `BattleObjectData_GetLoadState`, `VM_WriteVariable` |
| `0x3B` | `0x02079E30` | — | `0x8`, `0xC` | `BattleEntity_BindResource` |
| `0x3C` | `0x02079E5C` | — | — | `func_ov002_02076c74` |
| `0x3D` | `0x02079E70` | — | `0x8`, `0xC`, `0x10` | `BattleSceneObject_GetById`, `BattleSceneObject_GetActiveModel`, `BattleSceneObject_SetAnimation` |
| `0x3E` | `0x02079EC0` | — | `0x8` | `BattleAI_StartScriptById` |
| `0x3F` | `0x02079EE0` | — | `0x8`, `0xC` | `BattleAI_StopScriptById` |
| `0x40` | `0x0207A064` | — | `0x8` | — |
| `0x41` | `0x0207A094` | — | `0x8`, `0xC` | `func_ov002_02090c18` |
| `0x42` | `0x0207A0C0` | — | `0x8` | — |
| `0x43` | `0x0207A0FC` | — | `0x8` | — |
| `0x44` | `0x0207A124` | — | `0x8` | — |
| `0x45` | `0x0207A148` | — | — | — |
| `0x46` | `0x0207A164` | — | `0x8` | `func_ov002_0208e494`, `VM_WriteVariable` |
| `0x47` | `0x0207A198` | — | `0x8`, `0xC` | `func_ov002_0208e878` |
| `0x48` | `0x0207A1BC` | — | `0x8`, `0xC` | `BattleActor_FindHighestHpEnemy`, `BattleActor_FindLowestHpEnemy`, `BattleActor_FindMostDamagedEnemy`, `VM_WriteVariable` |
| `0x49` | `0x0207A234` | — | — | — |
| `0x4A` | `0x0207A244` | — | — | `VM_WriteVariable` |
| `0x4B` | `0x0207A274` | — | `0x8` | `func_ov002_0208bd88`, `VM_WriteVariable` |
| `0x4C` | `0x0207A2A0` | — | — | — |
| `0x4D` | `0x0207A2B0` | — | `0x8`, `0xC` | `BattleScript_GetProperty`, `VM_WriteVariable` |
| `0x4E` | `0x0207A314` | — | `0x8`, `0xC`, `0x10` | `BattleScript_SetProperty` |
| `0x4F` | `0x0207A2F0` | — | `0x10`, `0x14` | — |
| `0x50` | `0x0207A344` | — | `0x8`, `0xC`, `0x10` | `BattleSceneObject_GetById`, `BattlePosition_StoreViewRelative` |
| `0x51` | `0x0207A444` | — | — | — |
| `0x52` | `0x0207A454` | — | `0x8`, `0xC`, `0x10`, `0x14` | `func_ov002_02091a90`, `BattleActor_GetById` |
| `0x53` | `0x0207A508` | — | `0x8`, `0xC` | `BattleSceneObject_GetById`, `BattlePosition_StoreViewRelative`, `VM_WriteVariable` |
| `0x54` | `0x0207A5E4` | — | `0x8`, `0xC` | `BattleActor_GetEnemySlot`, `func_ov002_02076ee8`, `VM_WriteVariable` |
| `0x55` | `0x0207AA50` | — | `0x8`, `0xC`, `0x10` | `BattleActor_GetEnemySlot`, `func_ov002_02076ee8` |
| `0x56` | `0x0207AE2C` | — | `0x8` | `BattleSceneObject_GetById` |
| `0x57` | `0x0207AEAC` | — | `0x8`, `0xC` | `BattleSceneObject_GetById`, `BattleSceneObject_GetActiveModel` |
| `0x58` | `0x0207AF08` | — | `0x8`, `0xC` | `BattleSceneObject_GetById`, `BattleSceneObject_GetActiveModel`, `func_ov002_0208e3bc` |
| `0x59` | `0x0207AF94` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleSceneObject_GetById`, `BattleSceneObject_GetActiveModel` |
| `0x5A` | `0x0207B028` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleSceneObject_GetById`, `func_ov002_020a2f74` |
| `0x5B` | `0x0207B070` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24` | `BattleSceneObject_GetById`, `BattleSceneObject_AdjustPosition`, `BattleSceneObject_MoveBy`, `BattleSceneObject_MoveTo`, `func_ov002_020a3b2c` |
| `0x5C` | `0x0207B2D8` | — | `0x8`, `0xC`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24`, `0x28`, `0x2C` | `BattleSceneObject_GetById`, `func_ov000_020a3928` |
| `0x5D` | `0x0207B338` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24` | `BattleSceneObject_GetById`, `FX_Sqrt`, `_s32_div_f`, `BattleSceneObject_MoveBy`, `BattleSceneObject_MoveTo`, `func_ov002_020a3b2c` |
| `0x5E` | `0x0207B514` | — | — | `func_ov002_020789ec` |
| `0x5F` | `0x0207B52C` | — | — | `func_ov002_020787f0` |
| `0x60` | `0x0207B544` | — | — | `func_ov002_020789ec` |
| `0x61` | `0x0207B55C` | — | — | `func_ov002_020787f0` |
| `0x62` | `0x0207B574` | — | — | `func_ov002_020786d0` |
| `0x63` | `0x0207B58C` | — | — | `func_ov002_02078580` |
| `0x64` | `0x0207B5A4` | — | — | `func_ov002_02078460` |
| `0x65` | `0x0207B5BC` | — | — | `func_ov002_020786d0` |
| `0x66` | `0x0207B5D4` | — | — | `func_ov002_02078580` |
| `0x67` | `0x0207B5EC` | — | — | `func_ov002_02078460` |
| `0x68` | `0x0207B604` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24`, `0x28` | `BattleSceneObject_GetById`, `func_ov002_020a3ebc` |
| `0x69` | `0x0207B68C` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24`, `0x28` | `BattleSceneObject_GetById`, `func_ov002_020a3dd8` |
| `0x6A` | `0x0207B714` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24`, `0x28` | `BattleSceneObject_GetById`, `func_ov002_020a3c5c` |
| `0x6B` | `0x0207B77C` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24`, `0x28` | `BattleSceneObject_GetById`, `func_ov002_020be478` |
| `0x6C` | `0x0207B7E4` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24`, `0x28`, `0x2C`, `0x30` | `BattleSceneObject_GetById`, `func_ov002_020be3e8` |
| `0x6D` | `0x0207B8AC` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24`, `0x28`, `0x2C` | `BattleSceneObject_GetById`, `func_ov002_020a3810` |
| `0x6E` | `0x0207B954` | — | `0x8`, `0xC` | `BattleSceneObject_IsAnimationActiveById` |
| `0x6F` | `0x0207B9C8` | — | `0x8`, `0xC` | `BattleSceneObject_GetById`, `BattleSceneObject_StopMotionChannel` |
| `0x70` | `0x0207B9FC` | — | `0x8` | `BattleSceneObject_GetById` |
| `0x71` | `0x0207B1E0` | — | `0x8`, `0xC`, `0x14`, `0x18`, `0x1C`, `0x20` | `BattleSceneObject_GetById`, `BattleSceneObject_BeginMotionChannel`, `.L_020724b0` |
| `0x72` | `0x0207B288` | — | `0x8`, `0xC` | `BattleSceneObject_GetById`, `VM_WriteVariable` |
| `0x73` | `0x0207BA34` | — | `0x8`, `0xC`, `0x10` | `BattleActor_SelectRandomStatusTarget`, `BattleAI_StartActorAuxScript` |
| `0x74` | `0x0207BAC4` | — | `0x8`, `0xC` | — |
| `0x75` | `0x0207BBC8` | — | `0x8` | `BattleActor_GetById`, `BattleSceneObject_GetById`, `BattleSceneObject_GetActiveModel` |
| `0x76` | `0x0207BC30` | — | `0x8` | `BattleActor_GetById`, `BattleSceneObject_GetById`, `BattleSceneObject_GetActiveModel` |
| `0x77` | `0x0207BC98` | — | — | — |
| `0x78` | `0x0207BCA8` | — | `0x8`, `0xC` | `func_ov002_0208e238` |
| `0x79` | `0x0207BCD4` | — | — | — |
| `0x7A` | `0x0207BCE4` | — | — | — |
| `0x7B` | `0x0207BCF4` | — | — | — |
| `0x7C` | `0x0207BD04` | — | — | — |
| `0x7D` | `0x0207BD14` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleSceneObject_GetById`, `BattleDamage_ApplyToParty`, `BattleActor_ApplyDamage`, `BattlePosition_StoreViewRelative`, `BattleDamage_SpawnNumber` |
| `0x7E` | `0x0207BE14` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleActor_GetById`, `BattleModelEffect_Spawn` |
| `0x7F` | `0x0207BF44` | — | — | `func_ov002_0208e10c` |
| `0x80` | `0x0207BFA0` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleDamage_ApplyEquipmentModifiers`, `VM_WriteVariable` |
| `0x81` | `0x0207BEB0` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleActor_GetById`, `BattleStatus_TryApply`, `VM_WriteVariable` |
| `0x82` | `0x0207BF08` | — | `0x8`, `0xC` | `BattleActor_GetById`, `BattleStatus_ClearAll`, `BattleStatus_ClearEffect` |
| `0x83` | `0x0207C000` | — | `0x8`, `0xC` | `BattleActor_GetById` |
| `0x84` | `0x0207C04C` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C` | `BattleHitDescriptor_Configure`, `BattleHitDescriptor_SetStatus` |
| `0x85` | `0x0207C0A0` | — | `0x8` | `BattleDamage_ReflectQueuedHits` |
| `0x86` | `0x0207C0C0` | — | `0x8`, `0xC` | `VM_WriteVariable` |
| `0x87` | `0x0207C17C` | `0x88`, `0x89`, `0x8A` | `0x8` | `VM_WriteVariable` |
| `0x88` | `0x0207C17C` | `0x87`, `0x89`, `0x8A` | `0x8` | `VM_WriteVariable` |
| `0x89` | `0x0207C17C` | `0x87`, `0x88`, `0x8A` | `0x8` | `VM_WriteVariable` |
| `0x8A` | `0x0207C17C` | `0x87`, `0x88`, `0x89` | `0x8` | `VM_WriteVariable` |
| `0x8B` | `0x0207C208` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C` | `BattleActor_GetPartySlot`, `BattleDamage_CalculateByObject`, `BattleDamage_ApplyToParty`, `BattleDamage_StartPartyLaunchReaction`, `BattlePosition_StoreViewRelative`, `BattleModelEffect_Spawn`, `BattleSpriteEffect_Spawn`, `func_ov002_02077e78` |
| `0x8C` | `0x0207C380` | — | `0x8` | `func_ov002_02076c38` |
| `0x8D` | `0x0207C38C` | — | `0x8`, `0xC` | `BattleDamage_CalculateByObject`, `VM_WriteVariable` |
| `0x8E` | `0x0207C3CC` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20` | `func_ov002_020ac904` |
| `0x8F` | `0x0207C490` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20` | `BattleSceneObject_GetById`, `BattlePosition_StoreViewRelative`, `func_ov002_020ac904` |
| `0x90` | `0x0207C580` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20` | `BattleSceneObject_GetById`, `BattlePosition_StoreViewRelative`, `func_ov002_020ac740`, `VM_WriteVariable` |
| `0x91` | `0x0207C688` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20` | `BattleSceneObject_GetById`, `BattlePosition_StoreViewRelative`, `func_ov002_020ac740`, `VM_WriteVariable` |
| `0x92` | `0x0207C794` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C` | `BattleSpriteEffect_Spawn` |
| `0x93` | `0x0207C848` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C` | `BattleSceneObject_GetById`, `BattlePosition_StoreViewRelative`, `BattleSpriteEffect_Spawn` |
| `0x94` | `0x0207C928` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C` | `BattleSpriteEffect_SpawnInFreeSlot`, `VM_WriteVariable` |
| `0x95` | `0x0207C9F4` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C` | `BattleSceneObject_GetById`, `BattlePosition_StoreViewRelative`, `BattleSpriteEffect_SpawnInFreeSlot`, `VM_WriteVariable` |
| `0x96` | `0x0207CAF0` | — | `0x8`, `0xC` | — |
| `0x97` | `0x0207CB4C` | — | — | `func_ov002_02079090` |
| `0x98` | `0x0207CB64` | — | — | `func_ov002_02079090` |
| `0x99` | `0x0207CB7C` | — | — | `func_ov002_02079090` |
| `0x9A` | `0x0207CB94` | — | — | `func_ov002_02079090` |
| `0x9B` | `0x0207CBAC` | — | — | — |
| `0x9C` | `0x0207CBBC` | — | — | — |
| `0x9D` | `0x0207CBCC` | — | `0x8`, `0xC` | — |
| `0x9E` | `0x0207CC94` | — | `0x8` | — |
| `0x9F` | `0x0207CD14` | — | `0x8` | `func_ov002_0208e3f0` |
| `0xA0` | `0x0207CD7C` | — | `0x8` | `func_ov002_0208e3f0`, `VM_WriteVariable` |
| `0xA1` | `0x0207CDB0` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24`, `0x28`, `0x2C`, `0x30`, `0x34`, `0x38`, `0x3C` | `func_ov002_020a53e0` |
| `0xA2` | `0x0207CE94` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24` | `func_ov002_020aa38c` |
| `0xA3` | `0x0207CEEC` | — | `0x8` | `BattleSceneObject_GetById` |
| `0xA4` | `0x0207CF18` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24`, `0x28`, `0x2C`, `0x30` | `func_ov002_020bccc4` |
| `0xA5` | `0x0207CFD4` | — | — | `func_ov002_020bca68` |
| `0xA6` | `0x0207CFE8` | — | — | `func_ov002_020bca44` |
| `0xA7` | `0x0207CFFC` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20` | `func_ov002_020af97c` |
| `0xA8` | `0x0207D04C` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20` | `func_ov002_020af97c`, `BattleTask_BindOwnerSlot`, `VM_WriteVariable` |
| `0xA9` | `0x0207D0EC` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleSceneObject_GetById`, `func_ov002_020ae940` |
| `0xAA` | `0x0207D11C` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleSceneObject_GetById`, `func_ov002_020ae940`, `BattleTask_BindOwnerSlot`, `VM_WriteVariable` |
| `0xAB` | `0x0207D1B0` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C` | `func_ov002_020ae9c0`, `VM_WriteVariable` |
| `0xAC` | `0x0207D1F8` | — | `0x8` | `BattleSceneObject_GetById`, `func_ov002_020bba60` |
| `0xAD` | `0x0207D21C` | — | `0x8`, `0xC`, `0x10` | `BattleSceneObject_GetById`, `func_ov002_020bb644` |
| `0xAE` | `0x0207D250` | — | `0x8` | `BattleSceneObject_GetById`, `func_ov002_020bb274`, `VM_WriteVariable` |
| `0xAF` | `0x0207D288` | — | `0x8`, `0xC`, `0x10` | `BattleSceneObject_GetById`, `func_ov002_020bbb5c` |
| `0xB0` | `0x0207D2C0` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20` | `BattleSceneObject_GetById`, `func_ov002_020bb00c` |
| `0xB1` | `0x0207D320` | — | `0x8`, `0xC` | `BattleSceneObject_GetById`, `func_ov002_020badd8`, `VM_WriteVariable` |
| `0xB2` | `0x0207D35C` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C`, `0x20`, `0x24` | `BattleSceneObject_GetById`, `func_ov002_020ba210` |
| `0xB3` | `0x0207D3C4` | — | `0x8` | `BattleSceneObject_GetById`, `func_ov002_020b9978` |
| `0xB4` | `0x0207D3E8` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18` | `BattleSceneObject_GetById`, `func_ov002_020b9854` |
| `0xB5` | `0x0207D420` | — | `0x8`, `0xC`, `0x10` | `BattleSceneObject_GetById`, `func_ov002_020b9818` |
| `0xB6` | `0x0207D44C` | — | `0x8`, `0xC`, `0x10` | `BattleSceneObject_GetById`, `func_ov002_020b97d0` |
| `0xB7` | `0x0207D478` | — | `0x8` | `BattleSceneObject_GetById`, `func_ov002_020b97a8`, `VM_WriteVariable` |
| `0xB8` | `0x0207D4B0` | — | — | `func_ov002_02078e38` |
| `0xB9` | `0x0207D4C8` | — | — | `func_ov002_02078e38` |
| `0xBA` | `0x0207D4E0` | — | — | `func_ov002_02078e38` |
| `0xBB` | `0x0207D4F8` | — | — | `func_ov002_02078e38` |
| `0xBC` | `0x0207D510` | — | `0x8`, `0xC`, `0x10`, `0x14` | `func_ov002_0207905c`, `func_ov002_0206c1e4` |
| `0xBD` | `0x0207D578` | — | — | `func_ov002_02078bb4` |
| `0xBE` | `0x0207D590` | — | — | `func_ov002_02078bb4` |
| `0xBF` | `0x0207D5A8` | — | — | `func_ov002_02078bb4` |
| `0xC0` | `0x0207D5C0` | — | — | `func_ov002_02078bb4` |
| `0xC1` | `0x0207D5D8` | — | `0x8`, `0xC`, `0x10`, `0x14` | `func_ov002_020b1138`, `BattleTask_BindOwnerSlot`, `VM_WriteVariable` |
| `0xC2` | `0x0207D648` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18` | `func_ov002_020b1430` |
| `0xC3` | `0x0207D690` | — | — | — |
| `0xC4` | `0x0207D6A0` | — | — | — |
| `0xC5` | `0x0207D6B0` | — | `0x8`, `0xC` | — |
| `0xC6` | `0x0207D6DC` | — | `0x8`, `0xC` | — |
| `0xC7` | `0x0207D708` | — | `0x8`, `0xC` | `func_02019174`, `VM_WriteVariable`, `func_ov002_0209add8`, `func_ov002_0209ad50` |
| `0xC8` | `0x0207D758` | — | `0x8` | `func_0201904c`, `VM_WriteVariable` |
| `0xC9` | `0x0207D78C` | — | `0x8` | `VM_WriteVariable` |
| `0xCA` | `0x0207D748` | — | — | — |
| `0xCB` | `0x0207D898` | — | `0x8`, `0xC` | `func_ov002_0208e0c4` |
| `0xCC` | `0x0207D8FC` | — | `0x8`, `0xC` | `func_ov002_0208e0c4` |
| `0xCD` | `0x0207D928` | — | `0x8` | — |
| `0xCE` | `0x0207D94C` | — | `0x8` | — |
| `0xCF` | `0x0207D970` | — | — | — |
| `0xD0` | `0x0207D980` | — | — | — |
| `0xD1` | `0x0207D990` | — | `0x8`, `0xC`, `0x10` | `BattleAI_StartObjectScript` |
| `0xD2` | `0x0207D9D4` | — | `0x8`, `0xC`, `0x10` | `BattleAI_StartObjectScript` |
| `0xD3` | `0x0207DA10` | — | `0x8`, `0xC` | `BattleScriptState_GetByObjectId` |
| `0xD4` | `0x0207DA90` | — | `0x8`, `0xC`, `0x10` | `BattleAI_StartObjectScript` |
| `0xD5` | `0x0207DACC` | — | `0x8` | `BattleScriptState_GetByObjectId` |
| `0xD6` | `0x0207DB38` | — | `0x8` | — |
| `0xD7` | `0x0207DC68` | — | `0x8` | `BattleScriptState_GetByObjectId` |
| `0xD8` | `0x0207DC90` | — | `0x8` | `BattleScriptState_GetByObjectId` |
| `0xD9` | `0x0207DD98` | — | `0x8` | `BattleScriptState_GetByObjectId` |
| `0xDA` | `0x0207DDC8` | — | `0x8` | — |
| `0xDB` | `0x0207DE94` | — | `0x8` | `BattleScriptState_GetByObjectId` |
| `0xDC` | `0x0207DEC0` | — | `0x8` | — |
| `0xDD` | `0x0207DF84` | — | `0x8`, `0xC`, `0x10`, `0x14`, `0x18`, `0x1C` | — |
| `0xDE` | `0x0207E0CC` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleScript_GetProperty` |
| `0xDF` | `0x0207E11C` | — | `0x8`, `0xC`, `0x10` | `func_ov002_0208e098` |
| `0xE0` | `0x0207E15C` | — | — | — |
| `0xE1` | `0x0207E16C` | — | `0x8`, `0xC`, `0x10` | `BattleAI_GetStateById` |
| `0xE2` | `0x0207E1BC` | — | `0x8`, `0xC`, `0x10`, `0x14` | `BattleSceneObject_IsAnimationActiveById` |
| `0xE3` | `0x0207E208` | — | `0x8`, `0xC`, `0x10` | `BattleSound_Play`, `VM_WriteVariable` |
| `0xE4` | `0x0207E264` | — | `0x8`, `0xC` | `BattleSound_Play` |
| `0xE5` | `0x0207E298` | — | `0x8` | `func_ov002_02076178` |
| `0xE6` | `0x0207E2C4` | — | — | `func_ov002_02076178` |
| `0xE7` | `0x0207E308` | — | `0x8` | — |
| `0xE8` | `0x0207E394` | — | `0x8`, `0xC`, `0x10`, `0x14` | `func_ov002_020b0fc0`, `func_ov002_020698d4` |
