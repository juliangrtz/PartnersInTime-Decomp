extern "C" {
#include <game/battle_feedback.h>
#include <game/battle_context.h>
extern BattleFeedbackOwnerPrefix *data_ov002_020c0710;
extern int data_ov002_020be918[];
}

enum BattleFeedbackContextOffset {
    BATTLE_FEEDBACK_COUNT_OFFSET = 0x13a,
    BATTLE_FEEDBACK_VARIANT_OFFSET = 0xcb7a,
    BATTLE_FEEDBACK_VIEW_X_OFFSET = 0xcb9c,
    BATTLE_FEEDBACK_VIEW_Y_OFFSET = 0xcb9e
};

void BattleFeedback_LoadResource(void)
{
    BattleObjectData_AllocateLoadBuffer(67, 6144);
    BattleObjectData_ConfigureLoad(67, 2944, 0);
    func_ov002_02091fd8(67, 0xc200001f);
}

int BattleFeedback_IsResourceLoadPending(void)
{
    return BattleObjectData_IsLoadPending(67);
}

BattleEffect *BattleFeedback_SpawnAtObject(int effect_id, BattleSceneObject *object, int offset_x,
                                           int offset_y, int z)
{
    if (effect_id == -1)
        return 0;
    BattlePosition position;
    BattlePosition_StoreViewRelative(
        &position, (s16)(object->x + (s16)offset_x), (s16)(object->y - (s16)(object->z + (s16)-offset_y)),
        (s16)(object->effect_anchor_z + 16 * (256 - object->y)), object->flags.bits.use_raw_position,
        object->flags.bits.use_alternate_model);
    position.x += *(s16 *)(gBattleContext + BATTLE_FEEDBACK_VIEW_X_OFFSET);
    position.y += *(s16 *)(gBattleContext + BATTLE_FEEDBACK_VIEW_Y_OFFSET);
    position.z = z;
    if (position.x > *(s16 *)(gBattleContext + BATTLE_FEEDBACK_VIEW_X_OFFSET) + 224)
        position.x = *(s16 *)(gBattleContext + BATTLE_FEEDBACK_VIEW_X_OFFSET) + 224;
    if (position.y > *(s16 *)(gBattleContext + BATTLE_FEEDBACK_VIEW_Y_OFFSET) + 172)
        position.y = *(s16 *)(gBattleContext + BATTLE_FEEDBACK_VIEW_Y_OFFSET) + 172;
    BattleEffect *effect = BattleModelEffect_Spawn(effect_id, 0, position.x, position.y, position.z, 256);
    if (effect) {
        effect->user_value = (effect->user_value & 0xffff0000) | 0x43;
        if (data_ov002_020c0710)
            data_ov002_020c0710->effect = effect;
    }
    return effect;
}

void BattleFeedback_SpawnVariant(BattleSceneObject *object, int offset_x, int offset_y, s16 count)
{
    int index;
    switch (*(s16 *)(gBattleContext + BATTLE_FEEDBACK_VARIANT_OFFSET)) {
    case -1:
        index = 0;
        break;
    case 0:
        index = 1;
        count = 0;
        break;
    case 1:
        index = 2;
        break;
    case 2:
        index = 3;
        break;
    case 3:
        index = 4;
        break;
    case 4:
        index = 5;
        break;
    case 5:
        index = 6;
        break;
    case 6:
        index = 7;
        break;
    default:
        index = 0;
        break;
    }
    *(u16 *)(gBattleContext + BATTLE_FEEDBACK_COUNT_OFFSET) += count;
    BattleFeedback_SpawnAtObject(data_ov002_020be918[index], object, offset_x + 28, offset_y - 26, 8);
}

int BattleFeedback_IsEffectComplete(void)
{
    if (!data_ov002_020c0710 || !data_ov002_020c0710->effect)
        return 1;
    return data_ov002_020c0710->effect->complete ? 1 : 0;
}
