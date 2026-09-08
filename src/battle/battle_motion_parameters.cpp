#include "battle_motion_effects_internal.h"

extern "C" {
void BattleMotion_UpdateScaleWave(BattleSceneObject *object, BattleMotionChannel *channel)
{
    s16 *amplitude = channel->parameters;
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    int frame, duration;
    duration = channel->duration;
    frame = channel->elapsed_q8 / 256;
    if (frame < duration) {
        int wave = FX_SinCosTable_[2 * ((frame << 16) / duration >> 4)] / 16;
        wave = wave * *amplitude / 256;
        model->scale_x = 256 + wave;
        model->scale_y = 256 - wave;
    } else {
        model->scale_x = 256;
        model->scale_y = 256;
    }
}

void BattleMotion_StartScaleWave(BattleSceneObject *object, int channel, int duration, int amplitude)
{
    *BattleSceneObject_BeginMotionChannel(object, channel, duration, BattleMotion_UpdateScaleWave) =
        amplitude;
}

void BattleMotion_SetRangeParameters(s16 *parameters, s16 value, s16 start, s16 end)
{
    parameters[0] = value;
    parameters[1] = end;
    parameters[2] = end - start;
}
}
