#include "pocket_chomp_internal.h"

extern "C" {
u32 PocketChomp_BeginEntry(PocketChomp *center, BattleSceneObject *target)
{
    BattleSceneObject *object = center->object;
    BattleEntity_BindResource(object->actor_id, 52);
    BattleSceneObject_SetAnimation(object, 1, -1);
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    model->animation_offset_x = 0;
    model->animation_offset_y = -8;
    object->flags.bits.unk_20_23 = 2;
    BattleSceneObject_AdjustPosition(object, target->x - object->x, target->y - object->y,
                                     target->z - object->z);
    int duration = BattleMotion_StartBallistic(object, 1, 0, 0, 1, -object->z, -96, 76, 1);
    int dy = 132 - object->y;
    int distance = FX_Sqrt((1444 + dy * dy) << 12) / 4096;
    BattleSceneObject_StartAcceleratedMotionForDuration(object, 0, 38, dy, 0, distance, duration, distance,
                                                        0);
    center->squash_frame = 0;
    u32 result = (center->flags & ~31) | POCKET_CHOMP_ENTRY;
    center->flags = result;
    return result;
}

u32 PocketChomp_BeginSmallBounce(PocketChomp *center)
{
    BattleSceneObject *object = center->object;
    func_ov002_02071f08(118, object, 0, 0, 0, 0, 256);
    BattleMotion_StartBallistic(object, 1, 0, 0, 1, 38, -96, 38, 0);
    BattleSound_Play(71, 0, 0, 0);
    center->timer = 8;
    u32 result = (center->flags & ~31) | POCKET_CHOMP_SMALL_BOUNCE;
    center->flags = result;
    return result;
}

void PocketChomp_Reveal(PocketChomp *center)
{
    BattleSceneObject *object = center->object;
    s16 offset[4];
    func_ov002_02071e40(13, object, 0, -8, 0, 256);
    func_ov002_02071f08(28, object, 0, 0, -8, 0, 256);
    if ((u32)Random_NextModulo(100) < 5)
        center->flags |= 0x400;
    else
        center->flags &= ~0x400;
    PocketChomp_SetAnimation(center, 2, 1);
    center->flags |= 0x20;
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    model->rotation_z = 0;
    model->animation_offset_x = 0;
    model->animation_offset_y = 0;
    PocketChomp_GetAnchorOffset(offset, center);
    BattleSceneObject *linked = center->linked_object;
    BattleSceneObject_AdjustPosition(linked, object->x + offset[0] - linked->x, object->y - linked->y,
                                     object->z - offset[1] + 22 - linked->z);
    PocketChomp_AttachObject(center, 0);
    func_ov002_020b8a58(center->object, 16);
    u16 sound = center->bits.rare_variant ? 104 : 236;
    BattleSound_Play(sound, 0, 0, 0);
    center->timer = 8;
    u32 flags = (center->flags & ~31) | POCKET_CHOMP_REVEAL;
    center->flags = flags;
}

u32 PocketChomp_BeginFall(PocketChomp *center)
{
    BattleMotion_StartBallistic(center->object, 1, 0, 0, 1, -center->object->z, -112, 0, 1);
    func_ov002_020b8a58(center->object, 56);
    u32 result = (center->flags & ~31) | POCKET_CHOMP_FALL;
    center->flags = result;
    return result;
}

void PocketChomp_BeginRun(PocketChomp *center)
{
    BattleSceneObject *object = center->object;
    PocketChompAttackWorkPrefix *work = data_ov002_020c0710;
    func_ov002_02071f08(396, object, 0, 0, 0, 0, 256);
    BattleScreenEffect_StartPrimary(1, 50, 0);
    int speed = Overlay18Attack_GetTravelSpeedQ8(work->progress);
    int dx = work->horizontal_offset - 144 - object->x;
    int distance = dx > 0 ? dx : -dx;
    int duration = _s32_div_f(distance << 8, speed);
    BattleSceneObject_MoveBy(object, 0, dx, 0, 0, duration);
    func_ov018_020c40b8(center, _s32_div_f(12288, speed), 24, 0);
    func_ov018_020c40b8(center, _s32_div_f(8192, speed), 42, 1);
    func_ov018_020c3ea8(center);
    func_ov002_020b8a58(object, 256);
    PocketChompAttack_MoveAdultsOffscreen();
    u32 flags = (center->flags & ~31) | POCKET_CHOMP_RUN;
    center->flags = flags;
}

void PocketChomp_UpdateSquash(PocketChomp *center)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(center->object);
    if (center->squash_frame == 65535)
        model->rotation_z += 2048;
    else {
        ++center->squash_frame;
        if (center->squash_frame < 9) {
            int scale = FX_SinCosTable_[2 * (((int)(center->squash_frame << 16) / 9) >> 4)] / 16 / 2;
            model->scale_x = scale + 256;
            model->scale_y = 256 - scale;
        } else {
            model->scale_x = 256;
            model->scale_y = 256;
            center->squash_frame = 65535;
        }
    }
}

int PocketChompAttack_MoveAdultsOffscreen(void)
{
    PocketChompAttackWorkPrefix *work = data_ov002_020c0710;
    int i;
    for (i = 0; i < 2; ++i) {
        BattleSceneObject *object = BattleSceneObject_GetById((u16)(i + 56));
        BattleSceneObject_SetAnimation(object, 4, -1);
        BattleModel *model = BattleSceneObject_GetActiveModel(object);
        model->flag_bits.facing_left = 1;
        model->flag_bits.flip_y = 0;
        int dx = -64 - object->x;
        int speed = Overlay18Attack_GetTravelSpeedQ8(work->progress);
        int distance = dx > 0 ? dx : -dx;
        int duration = _s32_div_f(distance << 8, speed);
        BattleSceneObject_MoveBy(object, 2, dx, 0, 0, duration);
    }
    return BattleSound_Play(61, 0, 0, 0);
}

int PocketChompAttack_AreAdultsIdle(void)
{
    int count = 0, i;
    for (i = 0; i < 2; ++i)
        if (!BattleSceneObject_IsAnimationChannelActive(BattleActor_GetPartySlot((u16)(i + 56))->scene_object,
                                                        2))
            ++count;
    return count == 2;
}

int PocketChompAttack_IsBackdropIdle(void)
{
    return !BattleSceneObject_IsAnimationActiveById(68, 2) && !BattleSceneObject_IsAnimationActiveById(69, 2);
}

void PocketChompAttack_ReturnBackdrop(void)
{
    PocketChompAttackWorkPrefix *work = data_ov002_020c0710;
    BattleSceneObject_MoveBy(BattleSceneObject_GetById(68), 2, -work->horizontal_offset, 0, 0, 32);
    BattleSceneObject_MoveBy(BattleSceneObject_GetById(69), 2, -work->horizontal_offset, 0, 0, 32);
}
}
