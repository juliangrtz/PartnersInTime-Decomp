#include "battle_motion_effects_internal.h"

extern "C" {
int BattleMotion_StartPursuit(BattleSceneObject *object, int channel, int storage_channel,
                              BattleSceneObject *target, int speed)
{
    BattlePursuitMotion *motion =
        (BattlePursuitMotion *)BattleSceneObject_BeginMotionChannel(object, channel, 0, func_ov002_0206ea08);
    motion->target = target;
    motion->speed = speed;
    motion->elapsed = 0;
    int *position = (int *)BattleSceneObject_BeginMotionChannel(object, storage_channel, 0, 0);
    motion->position = position;
    position[0] = object->x << 12;
    position[1] = object->y << 12;
    position[2] = object->z << 12;
    return 0;
}
}
