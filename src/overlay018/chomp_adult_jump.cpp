#include "pocket_chomp_internal.h"

extern "C" {
u32 PocketChompAdult_BeginJump(PocketChompAdultMotion *adult) {
    PocketChompAttackWorkPrefix *work = data_ov002_020c0710;
    BattleSceneObject *object = adult->object;
    BattleSceneObject *target = BattleSceneObject_GetById(work->target_id);
    BattleMotionChannel *motion = BattleSceneObject_GetMotionChannel(object, 1);
    int duration = motion->duration - motion->elapsed_q8 / 256;
    int height = target->z + target->property_0fa;
    int extent =
        PocketChompAttack_ScaleTargetHeight(Overlay18Attack_GetLaunchHeight(work->progress));
    extent += height;
    BattleSceneObject_StartAcceleratedMotionForDuration(object, 2, 0, 0, 1, height, duration,
                                                        extent, 1);
    func_ov018_020c2e50(adult, 16385);
    adult->input_timer = duration;
    adult->timing.grade = 0;
    u32 flags = (adult->flags & ~0x7c0) | 0x80;
    adult->flags = flags;
    return flags;
}
}
