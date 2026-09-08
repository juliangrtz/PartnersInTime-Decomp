#include "effect_task_internal.h"

extern "C" {
void Overlay25EffectSequence_PositionEffect(Overlay25Task *task, BattleSceneObject *object,
                                            Overlay25WorkPrefix *work)
{
    BattleSceneObject *linked = BattleSceneObject_GetById(43);
    if (!work->model_effect) {
        BattleSceneObject_SetAnimation(object, 0, -1);
        BattleSceneObject_AdjustPosition(linked, 140 - linked->x, 128 - linked->y, -linked->z);
        BattleSceneObject_SetAnimation(linked, 0, -1);
        BattlePosition pos;
        Overlay25Object_GetViewPosition(&pos, linked);
        BattleSpriteEffect_Spawn(529, pos.x, pos.y, pos.z, 256);
        BattleModelEffect_SpawnAttached(&work->model_effect, 832, linked, 0, 0, 0, 256);
        linked->effect_anchor_z += 1024;
        task->update = Overlay25EffectSequence_StartModel;
        BattleSound_Play(309, 0, 0, 0);
    }
}

void Overlay25EffectSequence_StartModel(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *linked = BattleSceneObject_GetById(43);
    if (!work->model_effect) {
        parameters->effect = BattleModelEffect_Spawn(834, linked, 0, 0, 0, 256);
        parameters->effect->attached_script_flag = 1;
        if (parameters->mode == 1)
            task->update = func_ov025_020c5ea8;
        if (parameters->mode == 2)
            task->update = Overlay25EffectSequence_StartSprite;
    }
}

void Overlay25EffectSequence_StartSprite(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *work)
{
    BattleSceneObject *linked = BattleSceneObject_GetById(43);
    BattleModel *model = BattleSceneObject_GetActiveModel(BattleSceneObject_GetById(43));
    model->configure_animation_layer(0, 0, 1);
    model->animation_layer_states[0] = 0;
    task->parameters.timer = 0;
    linked->effect_anchor_z -= 1024;
    BattlePosition pos;
    Overlay25Object_GetViewPosition(&pos, linked);
    linked->effect_anchor_z += 1024;
    work->sprite_effect = BattleSpriteEffect_Spawn(530, pos.x, pos.y, pos.z, 256);
    task->update = Overlay25EffectSequence_AdvanceSprite;
    BattleSound_Play(394, 0, 0, 0);
}

void Overlay25EffectSequence_AdvanceSprite(Overlay25Task *task, BattleSceneObject *,
                                           Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *linked = BattleSceneObject_GetById(43);
    if (!work->sprite_effect->update_callback) {
        linked->effect_anchor_z -= 1024;
        BattlePosition pos;
        Overlay25Object_GetViewPosition(&pos, linked);
        linked->effect_anchor_z += 1024;
        work->sprite_effect = BattleSpriteEffect_Spawn(531, pos.x, pos.y, pos.z, 256);
        work->sprite_effect->sprite_flags |= 0x4000;
        if (parameters->mode == 1) {
            parameters->timer = 100;
            task->update = func_ov025_020c3a08;
        }
        if (parameters->mode == 2)
            task->update = func_ov025_020c34c4;
    }
}
}
