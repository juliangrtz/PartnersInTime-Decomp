#include "pocket_chomp_internal.h"

extern "C" {
u32 PocketChomp_BeginExit(PocketChomp *center)
{
    BattleSceneObject *object = center->object;
    BattleSceneObject_StopMotionChannel(object, 2, 0);
    BattleSceneObject_StopMotionChannel(object, 0, 0);
    BattleSceneObject_StopMotionChannel(object, 1, 0);
    center->bits.launch_pending = 0;
    center->timer = 0;
    int dx = data_ov002_020c0710->horizontal_offset + 128 - object->x;
    int dy = 272 - object->y;
    int duration = BattleMotion_StartBallistic(object, 1, 0, 0, 1, -144 - object->z, -96, 104, 1);
    BattleSceneObject_MoveBy(object, 0, dx, dy, 0, duration);
    PocketChomp_SetAnimation(center, 3, 0);
    center->bits.animation_locked = 1;
    func_ov002_020b8ab0(object, 0, -2, 8);
    u32 result = (center->flags & ~31) | POCKET_CHOMP_EXIT;
    center->flags = result;
    return result;
}

void PocketChomp_BeginExitWithSupport(PocketChomp *center, PocketChompSupport *support)
{
    PocketChomp_BeginExit(center);
    func_ov018_020c4c70(support, center);
}
}
