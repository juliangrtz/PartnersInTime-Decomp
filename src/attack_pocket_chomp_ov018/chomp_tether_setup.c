#include <game/pocket_chomp.h>

void func_ov018_020c2d34(BattleSceneObject *object, BattleMotionChannel *channel);

PocketChompTether *PocketChomp_StartTether(BattleSceneObject *object, int channel, BattleSceneObject *target,
                                           int source_x, int source_z, int target_x, int target_z)
{
    PocketChompTether *tether =
        (PocketChompTether *)BattleSceneObject_BeginMotionChannel(object, channel, 0, func_ov018_020c2d34);
    tether->target = target;
    tether->source_x = source_x;
    tether->source_z = source_z;
    tether->target_x = target_x;
    tether->target_z = target_z;
    return tether;
}

int PocketChompSupport_GetClampedDistance(PocketChompSupport *support, PocketChomp *center)
{
    int distance = support->object->x - center->object->x;
    if (support->bits.direction == 1)
        distance *= -1;
    if (distance < 24)
        distance = 24;
    if (distance > 192)
        distance = 192;
    return distance;
}
