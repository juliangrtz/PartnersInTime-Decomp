#include "effect_task_internal.h"

extern "C" {
void func_ov025_020cb664(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);

void Overlay25Enemy_BeginPause(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 9, -1);
        parameters->index = 30;
        task->update = Overlay25Enemy_UpdatePause;
    }
}

void Overlay25Enemy_UpdatePause(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    int timer = task->parameters.index;
    if (timer > 0)
        parameters->index = timer - 1;
    if (parameters->index <= 0) {
        BattleSceneObject_SetAnimation(object, 10, -1);
        task->update = Overlay25Enemy_BeginProjectilePhase;
    }
}

void Overlay25Enemy_BeginProjectilePhase(Overlay25Task *task, BattleSceneObject *object,
                                         Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 11, -1);
        parameters->index = 0;
        parameters->parameter = 0;
        task->update = Overlay25Enemy_EmitProjectile;
    }
}

void Overlay25Enemy_EmitProjectile(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *work)
{
    int timer = task->parameters.index;
    Overlay25Parameters *parameters = &task->parameters;
    if (timer > 0)
        parameters->index = timer - 1;
    if (parameters->index <= 0) {
        BattleSound_Play(80, 0, 0, 0);
        BattlePosition pos;
        BattlePosition_StoreViewRelative(
            &pos, (s16)(object->x - 20), (s16)((s16)(object->y + 4) - (s16)(object->z + 68)),
            (s16)(object->effect_anchor_z + 16 * (256 - (s16)(object->y + 4))),
            object->flags.bits.use_raw_position, object->flags.bits.use_alternate_model);
        BattleSpriteEffect_Spawn(509, pos.x, pos.y, pos.z, 256);
        BattleEffect *effect = BattleModelEffect_Spawn(795, object, -20, -64, -64, 256);
        effect->user_value = (effect->user_value & 0xffff0000) | 53;
        parameters->index = 54;
        Overlay25Task *next = &work->tasks[parameters->parameter + 1];
        next->update = func_ov025_020cb664;
        Overlay25Parameters *projectile = &next->parameters;
        projectile->timer = (OVERLAY25_PROJECTILE_COUNTS[0] & (1 << parameters->parameter)) ? 57 : 56;
        projectile->index = 0;
        projectile->parameter = parameters->parameter++;
        if (parameters->parameter >= OVERLAY25_PROJECTILE_COUNTS[1])
            task->update = Overlay25Enemy_WaitProjectiles;
    }
}

void Overlay25Enemy_WaitProjectiles(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *work)
{
    for (int i = 1; i < 7; ++i)
        if (work->tasks[i].update)
            return;
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 12, -1);
        task->update = Overlay25Enemy_WaitReturnVariant;
    }
}

void Overlay25Enemy_WaitReturnVariant(Overlay25Task *task, BattleSceneObject *object,
                                      Overlay25WorkPrefix *work)
{
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        work->phase = 0;
        task->update = Overlay25Enemy_BeginReturn;
    }
}
}
