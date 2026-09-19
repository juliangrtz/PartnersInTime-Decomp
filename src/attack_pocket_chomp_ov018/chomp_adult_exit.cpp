/*
 * Pocket Chomp adult exit (overlay 18, 0x020C6008-0x020C60BC).
 *
 * Begins the exit movement of an adult participant.
 */

#include "pocket_chomp_internal.h"

extern "C" {
u32 PocketChompAdult_BeginExit(PocketChompAdultMotion *adult) {
    PocketChompAttackWorkPrefix *work = data_ov002_020c0710;
    BattleSceneObject *object = adult->object;
    int exit_x;
    if (adult->bits.direction)
        exit_x = work->horizontal_offset - 336;
    else
        exit_x = work->horizontal_offset + 592;
    int delta = exit_x - object->x;
    int distance = delta > 0 ? delta : -delta;
    int speed = Overlay18Attack_GetMotionScaleQ8(work->progress);
    int duration = _s32_div_f(distance << 8, speed);
    BattleSceneObject_MoveBy(object, 1, delta, 0, 0, duration);
    func_ov018_020c2e50(adult, 33026);
    u32 result = (adult->flags & ~0x7c0) | 0x180;
    adult->flags = result;
    return result;
}
}
