#include "effect_task_internal.h"
extern "C" {
void func_ov025_020c5da4(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
static inline void StartDistance3(int dx, int dy, int dz)
{
    *(vu16 *)0x40002b0 = 0;
    *(vu32 *)0x40002b8 = dx * dx + dy * dy + dz * dz;
    while (*(vu16 *)0x40002b0 & 0x8000) {
    }
}
void Overlay25Enemy_WaitReturn(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *work)
{
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        work->phase = 0;
        task->update = Overlay25Enemy_BeginReturn;
    }
}

void Overlay25Enemy_LoadProjectiles(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleGlobalProperty_Set(12, 1);
    BattleObjectData_AllocateLoadBuffer(52, 16384);
    BattleObjectData_ConfigureLoad(52, 2432, 0);
    BattleObjectData_GetLoadState(52)->flags.bits.allocated = 1;
    BattleObjectData_GetLoadState(52)->flags.raw &= ~0x08000000;
    BattleObjectData_EnsureLoaded(52, 0xc1000099);
    BattleObjectData_AllocateLoadBuffer(53, 16384);
    BattleObjectData_ConfigureLoad(53, 2532, 0);
    BattleObjectData_EnsureLoaded(53, 0xc100007e);
    BattleActor_GetEnemySlot(object->actor_id)->damage_scale_q8 = 128;
    task->parameters.index = 0;
    parameters->parameter = 0;
    parameters->timer = 0;
    parameters->angle = 0;
    parameters->mode = 0;
    task->update = Overlay25Enemy_PositionLoadedProjectiles;
}
void Overlay25Enemy_PositionLoadedProjectiles(Overlay25Task *task, BattleSceneObject *,
                                              Overlay25WorkPrefix *work)
{
    if (BattleObjectData_IsLoadPending(53))
        return;
    BattleEntity_BindResource(40, 52);
    work->slots[0] = 40;
    work->secondary_slots[0] = 41;
    work->slot_depth_offsets[0] = 32;
    for (int i = 0; i < 6; ++i) {
        if (OVERLAY25_PROJECTILE_TARGETS[i]) {
            BattleSceneObject *projectile = BattleSceneObject_GetById((u16)(i + 44));
            BattleSceneObject *target = BattleSceneObject_GetById((u16)OVERLAY25_PROJECTILE_TARGETS[i]);
            BattleEntity_BindResource(projectile->actor_id, 53);
            BattleSceneObject_AdjustPosition(projectile, ((i + 2) << 8) / 10 - projectile->x,
                                             96 - projectile->y, -projectile->z);
            BattleSceneObject_SetAnimation(projectile, 0, -1);
            work->slots[i + 1] = i + 44;
            work->secondary_slots[i + 1] = i + 50;
            work->slot_depth_offsets[i + 1] = 32;
            projectile->primary_model->scale_x = 0;
            projectile->primary_model->scale_y = 0;
            int dx = target->x - projectile->x;
            int dy = target->y - projectile->y;
            int dz = target->z - projectile->z + 224;
            StartDistance3(dx, dy, dz);
            int distance = *(vu32 *)0x40002b4;
            BattleSceneObject_StartMotionWithPeakDistance(projectile, 2, dx, dy, dz, distance, distance, 2048,
                                                          1);
        }
    }
    BattleSound_Play(279, 0, 0, 0);
    task->update = func_ov025_020c5da4;
}
}
