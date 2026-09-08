#include "pocket_chomp_internal.h"

extern "C" {
void PocketChomp_SetAnimation(PocketChomp *center, int animation, s8 facing)
{
    if (!center->bits.animation_locked) {
        BattleSceneObject *object = center->object;
        int selected = animation;
        u16 actor_id = object->actor_id;
        if (center->bits.rare_variant)
            selected += 3;
        BattleSceneRenderOverride *render = object->render_override;
        BattleEntity_BindResource(actor_id, 52);
        BattleSceneObject_SetAnimation(object, selected, -1);
        object->flags.bits.unk_20_23 = 6;
        BattleModel *model = BattleSceneObject_GetActiveModel(object);
        model->flag_bits.facing_left = (u8)facing;
        model->flag_bits.flip_y = 0;
        object->render_override = render;
    }
}

int PocketChomp_PositionAtSide(PocketChomp *center, int target_id, int direction)
{
    PocketChompAttackWorkPrefix *work = data_ov002_020c0710;
    BattleSceneObject *target = BattleSceneObject_GetById(target_id);
    center->bits.direction = direction;
    int x = direction ? work->horizontal_offset + 320 : work->horizontal_offset - 64;
    BattleSceneObject *object = center->object;
    BattleSceneObject_AdjustPosition(object, x - object->x, target->y + 2 - object->y, -object->z);
    return PocketChomp_AttachObject(center, 1);
}
}
