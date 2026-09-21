/*
 * Smash Egg entry (overlay 15, 0x020C597C-0x020C5B4C).
 *
 * The per-frame entry update that runs while the attack is starting.
 */

#include "actor_internal.h"

void Overlay15Attack_UpdateEntry(struct Overlay10ActionActor *action)
{
    /* The common entry callback carries the action actor; this phase only
     * accesses its leading BattlePartyActor record. */
    BattlePartyActor *actor = (BattlePartyActor *)action;
    Overlay15AttackContext *context = data_ov002_020c0710;
    BattleSceneObject *primary = BattleSceneObject_GetById(40);
    BattleSceneObject *user;

    BattleSceneObject_GetById(42);
    user = actor->actor.scene_object;
    BattleSceneObject_GetById(actor->linked_object_id);
    switch (context->phase) {
    case 0:
        {
            BattleSceneObject *background;
            int duration;

            BattleEntity_BindResource(43, 55);
            BattleEntity_BindResource(44, 55);
            BattleEntity_BindResource(47, 55);
            BattleEntity_BindResource(48, 56);
            BattleSceneObject_SetAnimation(BattleSceneObject_GetById(48), 0, 1);
            background = BattleSceneObject_GetById(48);
            BattleSceneObject_AdjustPosition(background,
                -background->x, -background->y, 64 - background->z);
            duration = BattleSceneObject_StartVerticalArc(primary, 3, 1536, 80, 0);
            BattleSceneObject_MoveBy(primary, 2,
                user->x - primary->x, user->y - primary->y, 0, duration);
            BattleSceneObject_SetAnimation(BattleSceneObject_GetById(40), 1, 0);
            BattleEntity_BindResource(42, 52);
            BattleSceneObject_SetAnimation(BattleSceneObject_GetById(42), 1, 1);
            func_ov015_020c31dc(&context->pair);
            Overlay15Attack_ConfigureHitDescriptor();
            primary->effect_anchor_z = -16;
            context->timer = 0;
            ++context->phase;
        }
        break;
    case 1:
        if (context->timer >= 30) {
            context->actors[0].phase_flags.raw |= 0x40;
            Overlay15Attack_PreparePrimaryActor(&context->actors[1]);
            context->timer = 0;
            context->phase = context->timer;
            BattleAttack_SetCallback(actor, Overlay15Attack_UpdateSequence);
        } else {
            ++context->timer;
        }
        break;
    }
}
