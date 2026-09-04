#include <game/battle_effect.h>
#include <hardware.h>

u32 BattleSceneObject_SetBattleAnimationById(u32 object_id,
                                             int animation_id,
                                             int model_flag) {
    BattleSceneObject *object = BattleSceneObject_GetById(object_id);

    return BattleSceneObject_SetBattleAnimation(object, animation_id,
                                                model_flag);
}

u32 BattleSceneObject_SetBattleAnimation(BattleSceneObject *object,
                                         int animation_id, int model_flag) {
    BattleModel *model;
    u32 flags;

    BattleSceneObject_SetAnimation(object, animation_id, -1);
    BattleSceneObject_SetModelFlag10(object, model_flag);

    model = object->primary_model;
    flags = (model->flags & ~BATTLE_MODEL_ANIMATION_MODE_MASK) |
            BATTLE_MODEL_ANIMATION_MODE_BATTLE;
    model->flags = flags;
    return flags;
}

void BattlePosition_StoreBattleRelative(BattlePosition *position,
                                        int x, s16 anchor_y,
                                        s16 offset_y, s16 offset_z,
                                        int use_raw_position,
                                        int view_index) {
    BattlePosition_StoreViewRelative(
        position,
        x,
        (s16)(anchor_y - offset_y),
        (s16)(offset_z + 16 * (256 - anchor_y)),
        use_raw_position,
        view_index);
}

u32 BattleMath_StartSqrt(u32 value) {
    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = value;
    return value;
}

u32 BattleMath_WaitForSqrtResult(void) {
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    return *rSQRT_RESULT;
}
