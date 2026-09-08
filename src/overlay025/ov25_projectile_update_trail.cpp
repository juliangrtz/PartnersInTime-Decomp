#include "effect_task_internal.h"

extern "C" {
void Overlay25Projectile_WaitSpin(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *object = BattleSceneObject_GetById(40);
    object->primary_model->rotation_z += 4096;
    if (!BattleSceneObject_IsAnimationActiveById(40, 2)) {
        if (parameters->mode == 0) {
            parameters->index = -1;
            parameters->timer = 120;
            task->update = func_ov025_020c4fd8;
        }
        if (parameters->mode == 1) {
            parameters->index = -1;
            task->update = Overlay25EffectSequence_StartSprite;
        }
    }
}

void Overlay25Projectile_UpdateTrail(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject_GetById(40);
    if (++parameters->timer > 12) {
        parameters->timer = 0;
        BattleModel *model =
            BattleSceneObject_GetActiveModel(BattleSceneObject_GetById((u16)(parameters->index + 50)));
        func_ov002_0206c148((u16)parameters->parameter, model, 0, 0, 0, 256);
    }
}
}
