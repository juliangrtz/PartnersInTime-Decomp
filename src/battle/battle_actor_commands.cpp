#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>

enum BattleScriptHandleType {
    BATTLE_SCRIPT_HANDLE_INDEX_MASK = 0x3FFF,
    BATTLE_SCRIPT_HANDLE_TYPE_MASK = 0xC000,
    BATTLE_SCRIPT_HANDLE_TYPE_0 = 0x4000,
    BATTLE_SCRIPT_HANDLE_TYPE_1 = 0x8000,
    BATTLE_SCRIPT_HANDLE_TYPE_2 = 0xC000
};

enum BattleActorCommandContextOffset {
    BATTLE_ENEMY_POSITION_RECORDS_OFFSET = 0xCAD8,
    BATTLE_ENEMY_POSITION_ORIGIN_X_OFFSET = 0xCB9C,
    BATTLE_ENEMY_POSITION_ORIGIN_Y_OFFSET = 0xCB9E,
    BATTLE_SCRIPT_HANDLES_0_OFFSET = 0xCDF8,
    BATTLE_SCRIPT_HANDLES_1_OFFSET = 0xCBF8,
    BATTLE_SCRIPT_HANDLES_2_OFFSET = 0xCCF8
};

typedef struct BattleEnemyPositionRecord {
    u16 unknown_00;
    u16 actor_id;
    s16 x;
    s16 anchor_y;
    s16 offset_y;
    s16 active;
    u8 unknown_0c[8];
} BattleEnemyPositionRecord;

typedef struct BattleEnemyActorCommandView {
    BattleActor actor;
    u8 unknown_070[0x22A];
    s16 removal_animation_id;
} BattleEnemyActorCommandView;

typedef struct BattleEnemySceneObjectCommandView {
    BattleSceneObject object;
    u8 unknown_0f8[0x0A];
    u8 removal_state_102;
    u8 removal_state_103;
} BattleEnemySceneObjectCommandView;

typedef char BattleEnemyPositionRecord_SizeCheck[
    sizeof(BattleEnemyPositionRecord) == 0x14 ? 1 : -1];

extern "C" int func_ov002_020a95cc(BattleSceneObject *object);

/* Metrowerks emits C++ functions in reverse source order. */
int BattleScriptHandle_IsActive(int handle) {
    int type = handle & BATTLE_SCRIPT_HANDLE_TYPE_MASK;
    int index = handle & BATTLE_SCRIPT_HANDLE_INDEX_MASK;

    switch (type) {
    case BATTLE_SCRIPT_HANDLE_TYPE_0:
        return *(void **)(gBattleContext + BATTLE_SCRIPT_HANDLES_0_OFFSET +
                          index * sizeof(void *)) != 0;
    case BATTLE_SCRIPT_HANDLE_TYPE_1:
        return *(void **)(gBattleContext + BATTLE_SCRIPT_HANDLES_1_OFFSET +
                          index * sizeof(void *)) != 0;
    case BATTLE_SCRIPT_HANDLE_TYPE_2:
        return *(void **)(gBattleContext + BATTLE_SCRIPT_HANDLES_2_OFFSET +
                          index * sizeof(void *)) != 0;
    }
    return 0;
}

int BattleSceneObject_ConfigureAnimationLayer(int object_id, int layer) {
    BattleSceneObject *object = BattleSceneObject_GetById(object_id);
    BattleModel *model = BattleSceneObject_GetActiveModel(object);

    return model->configure_animation_layer((s8)layer, -1, 1);
}

int BattleEnemy_Remove(int actor_id, int show_damage) {
    BattleEnemyActorCommandView *enemy =
        (BattleEnemyActorCommandView *)BattleActor_GetEnemySlot(actor_id);

    if (show_damage == 1 && enemy->actor.current_hp > 0) {
        BattlePosition position;
        BattleEnemyPositionRecord *record;

        BattlePosition_StoreViewRelative(
            &position,
            enemy->actor.scene_object->x,
            (s16)(enemy->actor.scene_object->y -
                  enemy->actor.scene_object->z),
            (s16)(enemy->actor.scene_object->effect_anchor_z +
                  16 * (256 - enemy->actor.scene_object->y)),
            enemy->actor.scene_object->flags.bits.use_raw_position,
            enemy->actor.scene_object->flags.bits.use_alternate_model);

        record = (BattleEnemyPositionRecord *)(
            gBattleContext + BATTLE_ENEMY_POSITION_RECORDS_OFFSET);
        if (record->active != 0) {
            do {
                if (record->actor_id == actor_id) {
                    s16 anchor_y = record->anchor_y;
                    s16 offset_y = record->offset_y;
                    s16 height = (s16)(16 * (256 - anchor_y));

                    position.x = record->x - *(s16 *)(
                        gBattleContext +
                        BATTLE_ENEMY_POSITION_ORIGIN_X_OFFSET);
                    position.y =
                        (s16)(anchor_y - offset_y) -
                        *(s16 *)(gBattleContext +
                                 BATTLE_ENEMY_POSITION_ORIGIN_Y_OFFSET);
                    if (height < 0) {
                        height = 0;
                    }
                    position.z = height;
                    break;
                }
                record++;
            } while (record->active != 0);
        }

        ((BattleEnemySceneObjectCommandView *)enemy->actor.scene_object)
            ->removal_state_102 = 0;
        ((BattleEnemySceneObjectCommandView *)enemy->actor.scene_object)
            ->removal_state_103 = 0;
        BattleSceneObject_SetAnimation(
            enemy->actor.scene_object, enemy->removal_animation_id, -1);
        BattleDamage_SpawnNumber(
            enemy->actor.pending_damage, position.x, position.y, 7, 0);
    }

    enemy->actor.current_hp = 0;
    enemy->actor.flags &= ~BATTLE_ACTOR_FLAG_KO;
    return func_ov002_020a95cc(enemy->actor.scene_object);
}

int BattleActor_IsAnyHitLocked(void) {
    if (((u32)(BattleActor_GetPartySlot(56)->flags << 22) >> 31) != 0) {
        return 1;
    }
    if (((u32)(BattleActor_GetPartySlot(57)->flags << 22) >> 31) != 0) {
        return 1;
    }
    if (((u32)(BattleActor_GetEnemySlot(60)->flags << 22) >> 31) != 0) {
        return 1;
    }
    if (((u32)(BattleActor_GetEnemySlot(61)->flags << 22) >> 31) != 0) {
        return 1;
    }
    if (((u32)(BattleActor_GetEnemySlot(62)->flags << 22) >> 31) != 0) {
        return 1;
    }
    if (((u32)(BattleActor_GetEnemySlot(63)->flags << 22) >> 31) != 0) {
        return 1;
    }
    if (((u32)(BattleActor_GetEnemySlot(64)->flags << 22) >> 31) != 0) {
        return 1;
    }
    return ((u32)(BattleActor_GetEnemySlot(65)->flags << 22) >> 31) != 0;
}
