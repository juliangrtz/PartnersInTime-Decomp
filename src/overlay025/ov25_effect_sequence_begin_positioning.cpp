#include "effect_task_internal.h"

extern "C" {
void Overlay25Projectile_UpdateSpin(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    BattleSceneObject *object = BattleSceneObject_GetById((u16)(task->parameters.index + 44));
    BattleHitDescriptor_DisableByActor(object->actor_id);
    object->primary_model->rotation_z += 4096;
    if (!BattleSceneObject_IsAnimationChannelActive(object, 2))
        task->update = 0;
}

void Overlay25EffectSequence_Initialize(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleObjectData_AllocateLoadBuffer(52, 16384);
    BattleObjectData_ConfigureLoad(52, 2432, 0);
    BattleObjectData_GetLoadState(52)->flags.bits.allocated = 1;
    BattleObjectData_GetLoadState(52)->flags.raw &= ~0x08000000;
    BattleObjectData_EnsureLoaded(52, 0xc1000099);
    BattleObjectData_AllocateLoadBuffer(53, 16384);
    BattleObjectData_ConfigureLoad(53, 2532, 0);
    BattleObjectData_EnsureLoaded(53, 0xc100007e);
    BattleObjectData_AllocateLoadBuffer(54, 16384);
    BattleObjectData_ConfigureLoad(54, 2682, 0);
    BattleObjectData_EnsureLoaded(54, 0xc100009a);
    task->parameters.index = 0;
    parameters->parameter = 0;
    parameters->timer = 0;
    parameters->angle = 0;
    parameters->mode = 1;
    task->update = Overlay25EffectSequence_BeginPositioning;
}

void Overlay25EffectSequence_BeginPositioning(Overlay25Task *task, BattleSceneObject *object,
                                              Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    if (!BattleObjectData_IsLoadPending(54)) {
        BattleEntity_BindResource(43, 54);
        BattleSceneObject_SetAnimation(object, 7, -1);
        int dy = 128 - object->y, dx = 240 - object->x;
        Overlay25Object_StartDistance(dx, dy);
        BattleSceneObject_MoveBy(object, 2, dx, dy, 0, ((int)*(vu32 *)0x40002B4 << 8) / 256);
        parameters->sound = BattleSound_Play(24, 0, -1, 0);
        task->update = func_ov025_020c4084;
    }
}
}
