#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>

enum BattleObjectTableOffset {
    BATTLE_FIELD_OBJECT_TABLE_OFFSET = 0x4CC8,
    BATTLE_PARTY_ACTOR_TABLE_OFFSET = 0x4FF8,
    BATTLE_ENEMY_ACTOR_TABLE_OFFSET = 0x6508,
    BATTLE_AUXILIARY_OBJECT_TABLE_OFFSET = 0x6528,
    BATTLE_OBJECT_LOAD_STATE_TABLE_OFFSET = 0xD3C8
};

BattleObjectDataLoadState *BattleObjectData_GetLoadState(int object_data_id) {
    BattleObjectDataLoadState *states =
        (BattleObjectDataLoadState *)(gBattleContext +
                                      BATTLE_OBJECT_LOAD_STATE_TABLE_OFFSET);

    return &states[object_data_id];
}

BattleSceneObject *BattleSceneObject_GetById(u32 object_id) {
    u16 slot;

    if (object_id < BATTLE_ACTOR_PARTY_FIRST) {
        return *(BattleSceneObject **)(gBattleContext +
                                       BATTLE_FIELD_OBJECT_TABLE_OFFSET +
                                       object_id * 4);
    }

    slot = (u16)(object_id - BATTLE_ACTOR_PARTY_FIRST);
    if (slot < BATTLE_ACTOR_PARTY_COUNT) {
        BattleActor *actor =
            *(BattleActor **)(gBattleContext +
                              BATTLE_PARTY_ACTOR_TABLE_OFFSET + slot * 4);
        return actor->scene_object;
    }

    slot = (u16)(object_id - BATTLE_ACTOR_ENEMY_FIRST);
    if (slot < BATTLE_ACTOR_ENEMY_COUNT) {
        BattleActor *actor =
            *(BattleActor **)(gBattleContext +
                              BATTLE_ENEMY_ACTOR_TABLE_OFFSET + slot * 4);
        return actor->scene_object;
    }

    slot = (u16)(object_id -
                 (BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT));
    return *(BattleSceneObject **)(gBattleContext +
                                   BATTLE_AUXILIARY_OBJECT_TABLE_OFFSET +
                                   slot * 4);
}

BattleActor *BattleActor_GetById(int actor_id) {
    u16 party_slot = actor_id - BATTLE_ACTOR_PARTY_FIRST;

    if (party_slot < BATTLE_ACTOR_PARTY_COUNT) {
        return *(BattleActor **)(gBattleContext +
                                 BATTLE_PARTY_ACTOR_TABLE_OFFSET +
                                 party_slot * 4);
    }
    return *(BattleActor **)(gBattleContext + BATTLE_ENEMY_ACTOR_TABLE_OFFSET +
                             (u16)(actor_id - BATTLE_ACTOR_ENEMY_FIRST) * 4);
}

BattleActor *BattleActor_GetPartySlot(u16 actor_id) {
    return *(BattleActor **)(gBattleContext + BATTLE_PARTY_ACTOR_TABLE_OFFSET +
                             (actor_id - BATTLE_ACTOR_PARTY_FIRST) * 4);
}

BattleActor *BattleActor_GetEnemySlot(int actor_id) {
    return *(BattleActor **)(gBattleContext + BATTLE_ENEMY_ACTOR_TABLE_OFFSET +
                             (actor_id - BATTLE_ACTOR_ENEMY_FIRST) * 4);
}
