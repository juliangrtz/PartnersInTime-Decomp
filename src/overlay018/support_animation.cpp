#include "pocket_chomp_internal.h"

extern "C" {
u32 PocketChompSupport_BeginSwing(PocketChompSupport *support)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(support->object);
    func_ov018_020c2e50(support, 17926);
    func_0200940c(model, 256);
    u32 result = (support->flags & ~0x7c0) | (POCKET_CHOMP_SUPPORT_SWING << 6);
    support->flags = result;
    return result;
}

void PocketChompSupport_SpawnImpact(PocketChompSupport *support)
{
    BattleSceneObject *object = support->object;
    if (object) {
        func_ov002_02071e40(8, object, 0, 0, 0, 256);
        func_ov002_02071f08(11, object, 0, 0, 0, 0, 256);
        BattleSound_Play(51, 0, 0, 0);
    }
}

void PocketChompSupport_SetLinkedAnimation(PocketChompSupport *support, int animation)
{
    if (support->object) {
        BattleSceneObject *object = support->linked_object;
        BattleEntity_BindResource(object->actor_id, 52);
        BattleSceneObject_SetAnimation(object, animation, -1);
        BattleModel *model = BattleSceneObject_GetActiveModel(object);
        if (model) {
            model->flag_bits.facing_left = (u8)support->bits.direction;
            model->flag_bits.flip_y = 0;
        }
    }
}

void PocketChomp_UpdateLinkedAnimation(BattleSceneObject *object, PocketChomp *center)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    if (model) {
        BattleSceneObject *target = center->object;
        if (object->y > object->motion_origin_y && model->get_animation_id() != 10) {
            model->set_animation(10, 0);
            func_ov002_020b8a68(target, 0, 2, 2);
        }
    }
}

int PocketChompSupport_UpdateParticles(PocketChompSupport *support)
{
    int result = --support->particle_timer;
    if (result <= 0) {
        func_ov002_02071f08(data_ov018_020c7180[support->bits.direction], support->linked_object, 0, 0, 0, 0,
                            256);
        support->particle_timer = 4;
        return 4;
    }
    return result;
}

void PocketChomp_Initialize(PocketChomp *center, BattleSceneObject *linked)
{
    center->object = BattleSceneObject_GetById(42);
    center->linked_object = linked;
    center->timer = 0;
    center->bits.phase = 0;
    Overlay10Party_AverageStats(center->party_snapshot, 4);
}

void PocketChomp_Hide(PocketChomp *center)
{
    BattleSceneObject_SetAnimation(center->object, -1, -1);
    center->object->effect_anchor_z = 0;
}
}
