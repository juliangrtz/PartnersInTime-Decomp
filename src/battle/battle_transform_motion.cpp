#include "battle_motion_effects_internal.h"

extern "C" {
void BattleMotion_StartTransform(BattleSceneObject *object, int channel, int sx0, int sy0, int angle0,
                                 int sx1, int sy1, int angle1, int duration, int mode, int angle)
{
    BattleTransformMotion *motion = (BattleTransformMotion *)BattleSceneObject_BeginMotionChannel(
        object, channel, duration, func_ov002_0206e89c);
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    motion->scale_x[0] = sx0 == 0x8000 ? model->scale_x : sx0;
    motion->scale_x[1] = sx1 == 0x8000 ? model->scale_x : sx1;
    motion->scale_y[0] = sy0 == 0x8000 ? model->scale_y : sy0;
    motion->scale_y[1] = sy1 == 0x8000 ? model->scale_y : sy1;
    motion->rotation[0] = angle0 == (int)0x80000000 ? model->rotation_z : angle0;
    motion->rotation[1] = angle1 == (int)0x80000000 ? model->rotation_z : angle1;
    motion->mode = (u8)mode;
    motion->angle = angle;
}
}
