#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_BeginLoadAndMove(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleGlobalProperty_Set(12, 0);
    BattleActor_GetEnemySlot(object->actor_id)->damage_scale_q8 = 153;
    BattleObjectData_AllocateLoadBuffer(52, 16384);
    BattleObjectData_ConfigureLoad(52, 2432, 0);
    BattleObjectData_EnsureLoaded(52, 0xc1000097);
    BattleObjectData_AllocateLoadBuffer(53, 16384);
    BattleObjectData_ConfigureLoad(53, 2532, 0);
    func_ov002_02091fd8(53, 0xc100009b);
    BattleSceneObject_SetAnimation(object, 7, -1);
    int dx = 240 - object->x, dy = 128 - object->y;
    Overlay25Object_StartDistance(dx, dy);
    BattleSceneObject_MoveBy(object, 2, dx, dy, 0, ((int)*(vu32 *)0x40002B4 << 8) / 256);
    task->update = func_ov025_020cbbb0;
    parameters->travel_sound = BattleSound_Play(24, 0, -1, 0);
}
}
