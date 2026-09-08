#include "motion_internal.h"

extern "C" {
void Overlay12Motion_UpdateDampedArc(BattleSceneObject *object, BattleMotionChannel *channel)
{
    int frame = channel->elapsed_q8 / 256;
    Overlay12ArcParameters *arc = (Overlay12ArcParameters *)channel->parameters;
    if (frame == channel->duration) {
        int end = arc->end;
        int start = arc->start;
        int delta = end - start;
        if (delta > 270)
            object->z = (int)((double)(start + 270) + 179.2 * (double)(delta - 270) / 256.0);
        else
            object->z = end;
    } else {
        int height = frame * (arc->velocity + arc->acceleration * frame) / 4096;
        if (height > 270)
            height = (int)(270.0 + 179.2 * (double)(height - 270) / 256.0);
        object->z = arc->start + height;
    }
}

void Overlay12Attack_ShowHit(int actor_id, Overlay12AttackController *controller, int unused, int bonus)
{
    Overlay12AttackWork *work = data_ov002_020c0710;
    BattleActor_GetById(actor_id);
    BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
    int offset_y = object->property_0ff;
    int offset_z = object->property_0fa;
    int popup_x = object->property_0fe;
    int popup_y = offset_y - offset_z;
    BattlePosition position;
    BattlePosition_StoreViewRelative(
        &position, (s16)(object->x + popup_x),
        (s16)((s16)(object->y + offset_y) - (s16)(object->z + offset_z)),
        (s16)(object->effect_anchor_z + 16 * (256 - (s16)(object->y + offset_y))),
        object->flags.bits.use_raw_position, object->flags.bits.use_alternate_model);
    BattleActor_GetById(data_ov012_020c5a34[controller->flags.bits.formation_variant]);
    if (((Overlay12ControllerOptions *)&controller->controller_flags)->result == 1) {
        if (bonus) {
            BattleEffect_SetVariant(6);
            BattleSpriteEffect_Spawn(286, position.x, position.y, position.z, 256);
            BattleModelEffect_Spawn(406, 0, position.x, position.y, position.z, 256);
            Overlay10Enemy_PlayHitReaction(actor_id, 407);
            BattleSound_Play(49, 0, 0, 0);
        } else {
            BattleEffect_SetVariant(1);
            BattleSpriteEffect_Spawn(286, position.x, position.y, position.z, 256);
            BattleModelEffect_Spawn(406, 0, position.x, position.y, position.z, 256);
            Overlay10Enemy_PlayHitReaction(actor_id, 407);
            BattleSound_Play(56, 0, 0, 0);
        }
        BattleScreenEffect_StartSecondary(5, 1, 0);
        Overlay10Enemy_AddScaleSteps((Overlay10EnemyState *)work, actor_id, 1);
    } else {
        BattleEffect_SetVariant(-1);
        BattleModelEffect_Spawn(418, 0, position.x, position.y, position.z, 256);
        Overlay10Enemy_PlayHitReaction(actor_id, 408);
        BattleSound_Play(60, 0, 0, 0);
    }
    func_ov002_020719e4(object, popup_x, popup_y, 1);
    if (!Overlay10Enemy_IsSelectable(actor_id))
        work->flags |= 2;
}
}
