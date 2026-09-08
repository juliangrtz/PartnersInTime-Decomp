#include "battle_motion_effects_internal.h"

extern "C" {
void BattleMotion_StartShrink(BattleSceneObject *object, int channel, int duration, int delay, int mode)
{
    if (duration > 0 && delay >= 0) {
        BattleShrinkMotion *motion = (BattleShrinkMotion *)BattleSceneObject_BeginMotionChannel(
            object, channel, duration + delay, func_ov002_0206efa0);
        BattleModel *model = BattleSceneObject_GetActiveModel(object);
        motion->delay = delay;
        motion->duration = duration;
        motion->bits.mode = (u8)mode;
        motion->bits.alpha = (u8)model->animation_state_bits.state;
        motion->bits.unknown = 0;
        motion->unknown5 = 0;
        motion->phase = 0;
    }
}

void BattleMotion_UpdateAppear(BattleSceneObject *object, BattleMotionChannel *channel)
{
    int frame, duration;
    duration = channel->duration;
    frame = channel->elapsed_q8 / 256;
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    if (frame != duration) {
        int third = duration / 3;
        if (frame <= third) {
            int factor = 3 * (FX_SinCosTable_[2 * ((frame << 14) / third >> 4)] >> 4) / 2;
            model->scale_x = 512 - factor;
            model->scale_y = 128 + factor;
        } else if (frame <= 2 * duration / 3) {
            int factor = FX_SinCosTable_[2 * (((frame - third) << 14) / third >> 4)] >> 4;
            model->scale_y = 384 - factor;
            model->scale_x = 128 + (factor >> 1);
        } else {
            model->scale_x = 256;
            model->scale_y = 256;
        }
    }
}

void BattleMotion_StartAppear(BattleSceneObject *object, int channel, int duration)
{
    BattleSceneObject_BeginMotionChannel(object, channel, duration, BattleMotion_UpdateAppear);
    BattleMotion_UpdateAppear(object, &object->motion_channels[channel]);
}
}
