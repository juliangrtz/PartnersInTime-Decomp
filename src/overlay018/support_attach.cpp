#include "pocket_chomp_internal.h"

extern "C" {
void PocketChompSupport_Attach(PocketChompSupport *support, PocketChomp *center, int direction)
{
    if (support->object) {
        BattleSceneObject *target = center->object;
        BattleSceneObject *linked = support->linked_object;
        support->bits.direction = direction;
        int sign = direction ? -1 : 1;
        BattleSceneObject_AdjustPosition(linked, target->x - linked->x, target->y + 2 - linked->y,
                                         target->z - linked->z);
        BattleSceneObject *object = support->object;
        BattleSceneObject_AdjustPosition(object, linked->x + 4 * sign - object->x, linked->y + 1 - object->y,
                                         linked->z + 9 - object->z);
        BattleMotion_StartFollower(linked, support->object, 3);
        func_ov018_020c2e50(support, 17152);
        support->object->effect_anchor_z = -48;
        PocketChompSupport_SetLinkedAnimation(support, 9);
        linked->effect_anchor_z = -48;
        func_ov002_020b8a68(target, 12 * sign, 1, 9);
        support->action_flags.unknown_00 = 0;
        u32 result = (support->flags & ~0x7c0) | (POCKET_CHOMP_SUPPORT_READY << 6);
        support->flags = result;
    }
}

void PocketChompSupport_StopLinkedMotion(PocketChompSupport *support)
{
    if (support->object) {
        BattleSceneObject_StopMotionChannel(support->linked_object, 3, 0);
        PocketChompSupport_SetLinkedAnimation(support, -1);
        PocketChompSupport_Hide(support);
    }
}
}
