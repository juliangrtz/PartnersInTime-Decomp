#include "item_internal.h"

extern "C" {
void Overlay26Attack_LaunchItem(Overlay26ItemUser *user, int actor_id, int item_id, int velocity)
{
    BattleSceneObject_GetById(actor_id);
    BattleSceneObject *item = BattleSceneObject_GetById(item_id);
    BattlePartyActor *actor = (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
    s16 position[4];
    Overlay26Attack_GetActorPosition(position, actor, 1);
    int duration = BattleSceneObject_StartVerticalMotionTo(item, 3, velocity, -120, position[2], 1);
    BattleSceneObject_MoveTo(item, 2, position[0], position[1], position[2], duration);
    BattleSound_Play(117, 0, 0, 0);
}

void Overlay26Attack_LoadResource(u16 resource, int size, int resource_index, int resource_id, int compressed)
{
    BattleObjectData_AllocateLoadBuffer(resource, size);
    BattleObjectData_ConfigureLoad(resource, resource_index, compressed);
    BattleObjectData_EnsureLoaded(resource, resource_id);
}
}
