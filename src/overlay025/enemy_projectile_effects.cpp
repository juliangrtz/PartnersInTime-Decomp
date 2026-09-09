#include "effect_task_internal.h"
#include <game/battle_impact_effect.h>
extern "C" {
void func_ov025_020c5374(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
void func_ov002_0206c1e4(int, int, BattleModel *, int);

void Overlay25Enemy_BeginProjectileEffects(Overlay25Task *task, BattleSceneObject *object,
                                           Overlay25WorkPrefix *)
{
    BattleSceneObject *linked = BattleSceneObject_GetById(40);
    BattleSound_Play(97, 20, 0, 0);
    BattleSceneObject_SetAnimation(object, 25, -1);
    BattleSceneObject_AdjustPosition(linked, object->x - 20 - linked->x, object->y + 4 - linked->y,
                                     object->z + 40 - object->property_103 - linked->z);
    BattlePosition pos, origin;
    Overlay25Object_GetViewPosition(&pos, linked);
    Overlay25Object_GetViewPosition(&origin, object);
    BattleSpriteEffect_Spawn(525, pos.x, pos.y, pos.z, 256);
    BattleModelEffect_Spawn(821, object, pos.x - origin.x, pos.y - origin.y, pos.z - origin.z, 256);
    task->update = Overlay25Enemy_WaitProjectileEffectAnimation;
}

void Overlay25Enemy_WaitProjectileEffectAnimation(Overlay25Task *task, BattleSceneObject *object,
                                                  Overlay25WorkPrefix *work)
{
    BattleSceneObject *linked = BattleSceneObject_GetById(40);
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 26, -1);
        BattleSceneObject_SetAnimation(linked, 0, -1);
        linked->property_103 = -8;
        BattleSceneObject_AdjustPosition(linked, object->x - 20 - linked->x, object->y + 4 - linked->y,
                                         object->z + 40 - object->property_103 - linked->z);
        BattlePosition pos;
        Overlay25Object_GetViewPosition(&pos, linked);
        BattleSpriteEffect_Spawn(526, pos.x, pos.y, pos.z, 256);
        BattleModelEffect_SpawnAttached(&work->model_effect, 822, linked, 0, 0, 0, 256);
        task->update = Overlay25Enemy_ReleaseProjectileEffects;
        BattleSound_Play(96, 0, 0, 0);
    }
}

void Overlay25Enemy_ReleaseProjectileEffects(Overlay25Task *task, BattleSceneObject *object,
                                             Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *linked = BattleSceneObject_GetById(40);
    if (!work->model_effect) {
        BattleSound_Play(123, 0, 0, 0);
        BattleSceneObject_SetAnimation(object, 27, -1);
        parameters->index = -1;
        BattlePosition pos, origin;
        Overlay25Object_GetViewPosition(&pos, linked);
        Overlay25Object_GetViewPosition(&origin, object);
        BattleSpriteEffect_Spawn(527, pos.x, pos.y, pos.z, 256);
        BattleModelEffect_Spawn(823, object, pos.x - origin.x, pos.y - origin.y, pos.z - origin.z, 256);
        BattleImpactEmitter_Start(40, 0, 4096, 1, 1, 16, 16, 64, 64, 0);
        BattleImpactEmitter_Start(41, 0, 4096, 1, 1, 8, 8, 64, 64, 0);
        BattleEntity_BindResource(42, 52);
        BattleSceneObject_SetAnimation(BattleSceneObject_GetById(42), 0, 1);
        BattleSceneObject *projectile = BattleSceneObject_GetById(42);
        BattleSceneObject_AdjustPosition(projectile, -64 - projectile->x, -64 - projectile->y,
                                         -projectile->z);
        func_ov002_0206c1e4(0, -1, BattleSceneObject_GetActiveModel(BattleSceneObject_GetById(42)), -1);
        work->tasks[7].update = Overlay25Enemy_ResetAnimation;
        task->update = func_ov025_020c5374;
    }
}

void Overlay25Enemy_ResetAnimation(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    BattleSceneObject_GetById(40);
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 0, -1);
        task->update = 0;
    }
}
}
