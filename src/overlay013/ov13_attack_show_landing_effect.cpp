#include "trampoline_internal.h"

extern "C" {
void Overlay13Attack_Initialize(Overlay10ActionActor *user)
{
    Overlay13Work *work = (Overlay13Work *)func_ov010_020c2978(user, sizeof(Overlay13Work), 0);
    BattleObjectData_AllocateLoadBuffer(52, 4096);
    BattleObjectData_AllocateLoadBuffer(53, 32768);
    BattleObjectData_AllocateLoadBuffer(54, 32768);
    BattleObjectData_AllocateLoadBuffer(55, 20480);
    BattleObjectData_AllocateLoadBuffer(56, 20480);
    BattleObjectData_AllocateLoadBuffer(57, 30720);
    BattleObjectData_AllocateLoadBuffer(58, 30720);
    BattleObjectData_ConfigureLoad(52, 2432, 1);
    BattleObjectData_ConfigureLoad(53, 2438, 1);
    BattleObjectData_ConfigureLoad(54, 2502, 1);
    BattleObjectData_ConfigureLoad(55, 2694, 1);
    BattleObjectData_ConfigureLoad(56, 2758, 1);
    BattleObjectData_ConfigureLoad(57, 0, 1);
    BattleObjectData_ConfigureLoad(58, 0, 1);
    BattleObjectData_EnsureLoaded(52, 0xc0000006);
    BattleObjectData_EnsureLoaded(53, 0xc000000c);
    BattleObjectData_EnsureLoaded(54, 0xc000000e);
    BattleObjectData_EnsureLoaded(55, 0xc0000019);
    BattleObjectData_EnsureLoaded(56, 0xc000001a);
    BattleObjectData_EnsureLoaded(57, 0xc0000043);
    BattleObjectData_EnsureLoaded(58, 0xc0000044);
    BattleFeedback_LoadResource();
    BattleEntity_BindResource(41, 4);
    work->background[2] = *(u16 *)(gBattleContext + 52);
    work->background[3] = *(u16 *)(gBattleContext + 54);
    work->background[0] = *(u16 *)(gBattleContext + 48);
    work->background[1] = *(u16 *)(gBattleContext + 50);
    work->common.target = *(u16 *)&user->party.actor.target_actor_id;
    work->user_resource = user->party.actor.scene_object->resource->object_data_id;
    work->partner_resource =
        BattleSceneObject_GetById(user->party.linked_object_id)->resource->object_data_id;
    if (Overlay10Party_HasBadgeTwo())
        Overlay13Attack_AdvanceMotionTuning();
    func_ov002_020b03b8(0, 0, 48, 80);
    Overlay10Attack_BeginEntry(user, func_ov013_020c5210);
    work->common.background_x = 48;
    work->common.background_y = 0;
    work->common.background_duration = 90;
    work->common.background_phase = 2;
}

void Overlay13Attack_ShowLandingEffect()
{
    BattleSceneObject *object = BattleSceneObject_GetById(40);
    if (object->motion_origin_z && !object->z) {
        func_ov002_02071f08(419, object, 0, 0, 0, 0, 256);
        BattleSound_Play(57, 0, 0, 0);
    }
}
}
