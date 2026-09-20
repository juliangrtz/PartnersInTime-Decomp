/* Smash Eggs attack sequence and return, overlay 15, 0x020C5570-0x020C597C. */
#include "actor_internal.h"
extern "C" {
#include <game/inventory.h>
void func_ov015_020c4a68(Overlay15AttackModelController *, Overlay15AttackModelController *);
void func_ov015_020c37b0(Overlay15AttackObjectPairState *);
/* Preserve the engine's absolute-position wrapper and its per-axis deltas. */
static inline void SetPosition(BattleSceneObject *object, int x, int y, int z)
{
    BattleSceneObject_AdjustPosition(object, x - object->x, y - object->y, z - object->z);
}
void Overlay15Attack_UpdateSequence(BattlePartyActor *actor)
{
    Overlay15AttackContext *context = data_ov002_020c0710;
    BattleSceneObject *objects[2];
    BattleSceneObject *even, *odd;
    int i;
    for (i = 0; i < 2; ++i) {
        objects[i] = context->actors[i].primary->object;
        BattleSceneObject_GetActiveModel(objects[i]);
    }
    if (context->actors[0].primary->formation_index & 1) {
        odd = context->actors[0].primary->object;
        even = context->actors[1].primary->object;
    } else {
        even = context->actors[0].primary->object;
        odd = context->actors[1].primary->object;
    }
    switch (context->phase) {
    case 0:
        func_ov015_020c4a68(&context->actors[0], &context->actors[1]);
        func_ov015_020c4a68(&context->actors[1], &context->actors[0]);
        func_ov015_020c37b0(&context->pair);
        if (context->flag_bits.finished && Overlay15Attack_IsIdle(&context->actors[0]) &&
            Overlay15Attack_IsIdle(&context->actors[1]) &&
            Overlay15Attack_IsLinkedPhase((Overlay15AttackPhaseState *)&context->pair)) {
            context->timer = 0;
            context->phase = 1;
        }
        break;
    case 1:
        {
            BattleModel *model;
            BattleSceneObject_MoveTo(BattleSceneObject_GetById(68), 3, 0, 0, 0, 24);
            BattleSceneObject_MoveTo(BattleSceneObject_GetById(69), 3, 0, 0, 0, 24);
            for (int j = 0; j < 2; ++j) {
                BattleEntity_BindResource(context->actors[j].primary->object->actor_id,
                    context->actors[j].secondary_resource_id);
            }
            BattleSceneObject_SetAnimation(even, 4, -1);
            model = BattleSceneObject_GetActiveModel(even);
            model->flags &= ~0x400;
            model->flags &= ~0x800;
            BattleSceneObject_SetAnimation(odd, 4, -1);
            model = BattleSceneObject_GetActiveModel(odd);
            model->flags |= 0x400;
            model->flags &= ~0x800;
            BattleSceneObject_MoveTo(even, 3, BattleSceneObject_GetById(68)->x+288, 112, 0, 24);
            BattleSceneObject_MoveTo(odd, 3, 46, 152, 0, 12);
            BattleSceneObject_SetAnimationMode01(even, 1);
            BattleSceneObject_SetAnimationMode01(odd, 1);
            context->phase = 2;
        }
        /* Fall through into movement polling on the same update. */
    case 2:
        if (!BattleSceneObject_IsAnimationChannelActive(even, 3)) {
            int x = even->x;
            if (x > 256) {
                SetPosition(even, -32, even->y, 0);
                BattleSceneObject_MoveTo(even, 3, 56, 112, 0, 24);
            } else {
                BattleSceneObject_SetAnimation(even, 0, -1);
            }
        }
        if (BattleSceneObject_GetActiveModel(odd)->get_animation_id() == 4 &&
            !BattleSceneObject_IsAnimationChannelActive(odd, 3)) {
            BattleSceneObject_SetAnimation(odd, 0, -1);
            BattleModel *model = BattleSceneObject_GetActiveModel(odd);
            model->flags &= ~0x400;
            model->flags &= ~0x800;
        }
        if (!BattleSceneObject_IsAnimationChannelActive(even, 3) &&
            !BattleSceneObject_IsAnimationChannelActive(odd, 3)) {
            context->phase = 3;
        }
        break;
    case 3:
        if (context->item) {
            GameInventory_Add(context->item->item_id, 1);
        }
        for (int j = 0; j < 2; ++j) {
            Overlay15Attack_RestoreActors(&context->actors[j]);
        }
        Overlay15Attack_HideAttackObjects(&context->pair);
        BattleSceneObject_SetAnimation(BattleSceneObject_GetById(43), -1, -1);
        BattleSceneObject_SetAnimation(BattleSceneObject_GetById(47), -1, -1);
        func_ov002_020722ac(actor, 0);
        data_ov002_020c0710 = 0;
        break;
    }
}
}
