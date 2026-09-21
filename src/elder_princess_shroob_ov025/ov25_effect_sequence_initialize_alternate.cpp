/*
 * Elder Princess Shroob: alternate effect sequence (overlay 25, 0x020C34C4-0x020C37E4).
 *
 * The alternate-renderer variant of an effect sequence, with the particle and
 * model-effect teardown it finishes through.
 */

#include "effect_task_internal.h"

extern "C" {
void Overlay25EffectSequence_WaitParticleTasks(Overlay25Task *task, BattleSceneObject *,
                                              Overlay25WorkPrefix *work)
{
    int i;
    for (i = 0; i < 6; ++i) {
        /* Index the shared context before applying the particle-count field offset. */
        if (((u32 *)(gBattleContext + i * sizeof(u32)))[27108 / sizeof(u32)] &&
            work->tasks[i + 1].update)
            return;
    }
    for (i = 0; i < 7; ++i)
        work->slots[i] = 0;
    work->sprite_effect->sprite_flags &= ~0x4000;
    task->update = Overlay25EffectSequence_FinishParticles;
}

void Overlay25EffectSequence_FinishParticles(Overlay25Task *task, BattleSceneObject *,
                                             Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *linked = BattleSceneObject_GetById(43);
    if (!work->sprite_effect->update_callback) {
        work->sprite_effect->update_callback = 0;
        work->sprite_effect = 0;
        parameters->effect->update_callback = 0;
        parameters->effect = 0;
        linked->effect_anchor_z -= 1024;
        BattlePosition pos;
        Overlay25Object_GetViewPosition(&pos, linked);
        linked->effect_anchor_z += 1024;
        BattleSpriteEffect_Spawn(532, pos.x, pos.y, pos.z, 256);
        work->model_effect = BattleModelEffect_Spawn(835, linked, 0, 0, 0, 256);
        task->update = Overlay25EffectSequence_FinishModelEffect;
        BattleSound_Play(309, 0, 0, 0);
    }
}

void Overlay25EffectSequence_FinishModelEffect(Overlay25Task *task, BattleSceneObject *,
                                               Overlay25WorkPrefix *work)
{
    BattleSceneObject *linked = BattleSceneObject_GetById(43);
    if (!work->model_effect->update_callback) {
        work->model_effect->update_callback = 0;
        BattleSceneObject_SetAnimation(linked, -1, -1);
        work->phase = 0;
        task->update = Overlay25Enemy_BeginReturn;
    }
}

void Overlay25EffectSequence_InitializeAlternate(Overlay25Task *task, BattleSceneObject *object,
                                                 Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleGlobalProperty_Set(12, 1);
    BattleObjectData_AllocateLoadBuffer(53, 16384);
    BattleObjectData_ConfigureLoad(53, 2532, 0);
    BattleObjectData_EnsureLoaded(53, 0xc100007e);
    BattleObjectData_AllocateLoadBuffer(54, 16384);
    BattleObjectData_ConfigureLoad(54, 2682, 0);
    BattleObjectData_EnsureLoaded(54, 0xc100009a);
    BattleActor_GetEnemySlot(object->actor_id)->damage_scale_q8 = 153;
    task->parameters.index = 0;
    parameters->parameter = 0;
    parameters->timer = 0;
    parameters->angle = 0;
    parameters->mode = 2;
    task->update = Overlay25EffectSequence_BeginPositioning;
}

void Overlay25EffectSequence_BeginSequentialLaunch(Overlay25Task *task, BattleSceneObject *,
                                                   Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters;
    /* Keep the native payload-base calculation; C alone folds it into task offsets. */
    asm { add parameters, task, #4 }
    parameters->timer = 0;
    task->parameters.index = -1;
    task->update = func_ov025_020c31a0;
}
}
