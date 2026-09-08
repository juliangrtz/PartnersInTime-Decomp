#include "trampoline_internal.h"

extern "C" {
void Overlay13Attack_RestoreBattle(Overlay10ActionActor *user)
{
    Overlay13Work *work = (Overlay13Work *)data_ov002_020c0710;
    if (!work->restoring) {
        BattleSceneObject_MoveTo(BattleSceneObject_GetById(68), 3, 0, 0, 0, 30);
        BattleSceneObject_MoveTo(BattleSceneObject_GetById(69), 3, 0, 0, 0, 30);
        work->restoring = 1;
    }
    if (BattleSceneObject_IsAnimationChannelActive(BattleSceneObject_GetById(68), 3))
        return;
    if (!func_ov002_020719a4())
        return;
    for (int i = 40; i < 56; ++i)
        BattleSceneObject_GetById((u16)i)->effect_anchor_z = 0;
    user->party.actor.scene_object->effect_anchor_z = 0;
    BattleSceneObject_GetById(user->party.linked_object_id)->effect_anchor_z = 0;
    func_ov002_020722ac(user, 0);
    *(u16 *)(gBattleContext + 54) = work->background[3];
    *(u16 *)(gBattleContext + 52) = work->background[2];
    *(u16 *)(gBattleContext + 50) = work->background[1];
    *(u16 *)(gBattleContext + 48) = work->background[0];
    GameHeap_Free(data_ov002_020c0710);
    data_ov002_020c0710 = 0;
}

int Overlay13Attack_StartJump(BattleSceneObject *object, int animation, s8 facing, int velocity,
                              int acceleration)
{
    Overlay13Work *work = (Overlay13Work *)data_ov002_020c0710;
    BattleSceneObject_SetAnimation(object, animation, 0);
    BattleModel *model = object->primary_model;
    model->flag_bits.facing_left = (u8)facing;
    model->flag_bits.flip_y = 0;
    int height = work->low_entry ? 0 : 16;
    int duration = BattleSceneObject_StartVerticalMotionTo(object, 3, velocity, acceleration, height, 1);
    BattleSceneObject_MoveTo(object, 2, 96, 140, height, duration);
    if (!object->effect_anchor_z)
        object->effect_anchor_z = -16;
    return 1;
}

int Overlay13Attack_UpdateBounce(BattleSceneObject *object, BattleSceneObject *copy, int landing_animation,
                                 int launch_animation, int falling_animation)
{
    Overlay13Work *work = (Overlay13Work *)data_ov002_020c0710;
    BattleSceneObject *trampoline = BattleSceneObject_GetById(40);
    if (BattleSceneObject_IsAnimationChannelActive(object, 3)) {
        if (falling_animation != object->primary_model->get_animation_id() &&
            object->z < object->motion_origin_z)
            object->primary_model->set_primary_animation((u8)falling_animation, 0, 1);
        return 0;
    } else if (trampoline->primary_model->get_animation_id() != 1) {
        if (landing_animation != -1) {
            BattleSceneObject_SetAnimation(object, landing_animation, 0);
            BattleSceneObject_SetStateFlags(object, 1, 0);
        }
        func_ov002_02072004(object, 0, object->actor_id < 44 ? 16 : 17, 0, 0);
        BattleSound_Play(57, 0, 0, 0);
        BattleSceneObject_SetAnimation(trampoline, 1, 0);
        BattleSceneObject_SetStateFlags(trampoline, 1, 0);
        BattleSceneObject_MoveBy(object, 2, 0, 0, -16, 8);
    } else if ((u8)trampoline->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(trampoline, 2, 0);
        BattleSceneObject_SetStateFlags(trampoline, 1, 0);
        BattleModel *model = object->primary_model;
        model->flag_bits.facing_left = 0;
        model->flag_bits.flip_y = 0;
        if (launch_animation != -1) {
            BattleSceneObject_SetAnimation(object, launch_animation, 0);
            BattleSceneObject_SetStateFlags(object, 1, 0);
        }
        int duration =
            BattleSceneObject_StartVerticalMotionTo(object, 3, work->velocity, work->acceleration, 470, 0);
        BattleSceneObject_MoveBy(object, 2, 64, 0, 0, duration);
        BattleSound_Play(40, 0, 0, 0);
        BattleSceneObject_SetAnimation(copy, 0, 1);
        object->primary_model->unknown_38(copy->alternate_model);
        BattleSceneObject_AdjustPosition(copy, -96 - copy->x, 96 - copy->y, -copy->z);
        *(u16 *)(gBattleContext + 54) = 0;
        *(u16 *)(gBattleContext + 52) = 63;
        *(u16 *)(gBattleContext + 50) = 6;
        *(u16 *)(gBattleContext + 48) = 12;
        return 1;
    }
    return 0;
}

int Overlay13Attack_UpdateArcEffects(BattleSceneObject *object, BattleSceneObject *copy, int update_animation)
{
    Overlay13Work *work = (Overlay13Work *)data_ov002_020c0710;
    u8 animation[2];
    s16 angle;
    unsigned region =
        (s16)Overlay13Attack_ClassifyVerticalAngle((BattlePosition *)&object->x, &angle, animation);
    if (update_animation) {
        u8 id = animation[0];
        if (id != object->primary_model->get_animation_id())
            object->primary_model->set_primary_animation(id, 0, 1);
        u8 copy_id = animation[0];
        if (copy_id != copy->alternate_model->get_animation_id())
            copy->alternate_model->set_primary_animation(copy_id, 0, 1);
    }
    switch (region) {
    case 0:
    case 1: {
        BattleModel *model = object->primary_model;
        if (!(u8)model->flag_bits.animation_active)
            BattleSceneObject_SetAnimation(object, model->get_animation_id(), -1);
        copy->x = -96;
        if (work->common.timer % 2 == 0) {
            BattleImpactParticleTask *effect =
                BattleImpactParticle_CreateFromObject(object, func_ov002_0206b5f0);
            BattleImpactParticlePayload *payload = &effect->data;
            payload->frame = 4;
            payload->render_bits.size = 12;
            work->effect = (BattleAITask *)effect;
        }
        break;
    }
    case 2: {
        BattleModel *model = copy->alternate_model;
        if (!(u8)model->flag_bits.animation_active)
            BattleSceneObject_SetAnimation(copy, model->get_animation_id(), -1);
        copy->x = object->x;
        copy->y = object->y;
        copy->z = object->y - angle;
        if (work->common.timer % 2 == 0) {
            BattleImpactParticleTask *effect = func_ov002_0206bdf8(copy, func_ov002_0206bda4);
            effect->data.frame = 4;
            work->effect = (BattleAITask *)effect;
        }
    }
    }
    if (!BattleSceneObject_IsAnimationChannelActive(object, 3)) {
        if (update_animation)
            object->effect_anchor_z = 0;
        return 1;
    }
    return 0;
}

int Overlay13Attack_UpdateRetreat(BattleSceneObject *object, Overlay13Pair *pair)
{
    BattleSceneObject *trampoline = BattleSceneObject_GetById(40);
    if (!BattleSceneObject_IsAnimationChannelActive(object, 3)) {
        BattleSceneObject_SetAnimation(object, 5, -1);
        // Variants 2 and 3 use baby animations; preserve the native 16-bit range test.
        const u16 first_baby_offset = (u16)-2;
        if ((u16)(pair->flags.bits.variant + first_baby_offset) <= 1)
            func_ov002_02071750(object, 1);
        else
            func_ov002_02071750(object, 0);
        if (trampoline->primary_model->get_animation_id() != 3) {
            func_ov002_02072004(object, 0, object->actor_id < 44 ? 14 : 15, 0, 0);
            BattleSceneObject_SetAnimation(object, 6, 0);
            int duration = BattleSceneObject_StartVerticalMotionTo(object, 3, 1280, -96, 0, 1);
            BattleSceneObject_MoveBy(object, 2, -32, 0, 0, duration);
            BattleSound_Play(52, 0, 0, 0);
            pair->retreat_timer = 0;
            BattleSceneObject_SetAnimation(trampoline, 3, 0);
            BattleSceneObject_SetStateFlags(trampoline, -1, 0);
            BattleSceneObject_MoveBy(trampoline, 2, -24, 0, 0, 16);
        } else if (object->x > -64) {
            if (!pair->retreat_timer) {
                func_ov002_02072004(object, 0, object->actor_id < 44 ? 14 : 15, 0, 0);
                if (object->x > -16)
                    BattleSound_Play(58, 0, 0, 0);
            }
            if (pair->retreat_timer < 2)
                ++pair->retreat_timer;
            else {
                BattleSceneObject_SetAnimation(object, 6, 0);
                int duration = BattleSceneObject_StartVerticalMotionTo(object, 3, 1024, -96, 0, 1);
                BattleSceneObject_MoveBy(object, 2, -32, 0, 0, duration);
                pair->retreat_timer = 0;
            }
        }
    }
    if (trampoline->primary_model->get_animation_id() == 3) {
        if (trampoline->primary_model->property_056 >= 6) {
            if (BattleSceneObject_IsAnimationChannelActive(trampoline, 3))
                BattleSceneObject_StopMotionChannel(trampoline, 3, 0);
            if (trampoline->primary_model->property_056 == 6)
                BattleSound_Play(57, 0, 0, 0);
        } else {
            if (!BattleSceneObject_IsAnimationChannelActive(trampoline, 3))
                BattleSceneObject_MoveBy(trampoline, 3, -24, 0, 0, 16);
        }
    }
    if (trampoline->x < -32) {
        if ((u8)trampoline->primary_model->flag_bits.animation_active)
            BattleSceneObject_SetAnimation(trampoline, -1, 0);
        if (object->x <= -64)
            return 1;
    }
    return 0;
}
}
