#include "battle_motion_effects_internal.h"

extern "C" {
int BattleMotion_StartSineMove(BattleSceneObject *object, int channel, int x, int y, int z, int start,
                               int end, int duration, int absolute)
{
    BattleSineMotion *motion = (BattleSineMotion *)BattleSceneObject_BeginMotionChannel(
        object, channel, duration, func_ov002_0206ecfc);
    motion->angle_start = start;
    motion->angle_end = end;
    if (absolute) {
        motion->offset[0] = x - object->x;
        motion->offset[1] = y - object->y;
        motion->offset[2] = z - object->z;
    } else {
        motion->offset[0] = x;
        motion->offset[1] = y;
        motion->offset[2] = z;
    }
    return duration;
}

void BattleMotion_UpdateFollower(BattleSceneObject *object, BattleMotionChannel *channel)
{
    BattleFollowMotion *motion = (BattleFollowMotion *)channel->parameters;
    motion->object->motion_origin_x = motion->object->x;
    motion->object->motion_origin_y = motion->object->y;
    motion->object->motion_origin_z = motion->object->z;
    motion->object->motion_target_x = object->x - motion->offset[0];
    motion->object->x = motion->object->motion_target_x;
    motion->object->motion_target_y = object->y - motion->offset[1];
    motion->object->y = motion->object->motion_target_y;
    motion->object->motion_target_z = object->z - motion->offset[2];
    motion->object->z = motion->object->motion_target_z;
}

int BattleMotion_StartFollower(BattleSceneObject *object, BattleSceneObject *follower, int channel)
{
    BattleFollowMotion *motion = (BattleFollowMotion *)BattleSceneObject_BeginMotionChannel(
        object, channel, 0, BattleMotion_UpdateFollower);
    motion->offset[0] = object->x - follower->x;
    motion->offset[1] = object->y - follower->y;
    motion->offset[2] = object->z - follower->z;
    motion->object = follower;
    return 0;
}
}
