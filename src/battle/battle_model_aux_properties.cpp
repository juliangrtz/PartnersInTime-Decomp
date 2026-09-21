/* Additional active-model properties used by common battle script commands. */
#include <game/battle_scene.h>
extern "C" {
void BattleSceneObject_SetAuxModelProperty(BattleSceneObject *object, int property, int value, int unused)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    switch (property) {
    case 0: {
        /* Resolve the other object before reacquiring the current model. */
        BattleModel *other = BattleSceneObject_GetActiveModel(BattleSceneObject_GetById((u16)value));
        BattleSceneObject_GetActiveModel(object)->unknown_38(other);
        break;
    }
    case 1: break;
    case 2: model->flag_bits.animation_mode = (u8)value; break;
    }
}
int BattleSceneObject_GetAuxModelProperty(BattleSceneObject *object, int property, int element, int unused)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    switch (property) {
    case 0: break;
    case 1: return model->animation_data[element].end_frame - model->animation_data[element].start_frame;
    case 2: return model->flag_bits.animation_mode;
    }
    return 0;
}
}
