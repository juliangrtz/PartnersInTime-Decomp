#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_EmitTravelEffects(Overlay25Task *task, BattleSceneObject *object,
                                      Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    if (++parameters->index > 8) {
        parameters->index = 0;
        BattlePosition pos;
        Overlay25Object_GetViewPosition(&pos, object);
        BattleModelEffect_Spawn(796, 0, pos.x, pos.y, pos.z, 256);
    }
    if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
        if (parameters->travel_sound != -1) {
            func_ov002_02076178(parameters->travel_sound);
            parameters->travel_sound = -1;
        }
        if (object->primary_model->flag_bits.panel_animation_trigger) {
            BattleSound_Play(0, 0, 0, 0);
            BattleSceneObject_SetAnimation(object, 18, -1);
            BattlePosition pos;
            Overlay25Object_GetViewPosition(&pos, object);
            BattleSpriteEffect_Spawn(522, pos.x, pos.y, pos.z, 256);
            BattleModelEffect_SpawnAttached(&work->model_effect, 818, object, 0, 0, 0, 256);
            task->update = Overlay25Enemy_WaitAttachedEffect;
        }
    }
}

void Overlay25Enemy_WaitAttachedEffect(Overlay25Task *task, BattleSceneObject *object,
                                       Overlay25WorkPrefix *work)
{
    if (!work->model_effect) {
        BattleSceneObject_SetAnimation(object, 22, -1);
        task->update = func_ov025_020c6c4c;
    }
}
}
