#include "item_internal.h"

extern "C" {
void Overlay26Attack_Initialize(Overlay26ItemUser *user)
{
    gOverlay26AttackState = (Overlay26AttackState *)GameHeap_New(sizeof(Overlay26AttackState),
                                                                 *(int *)(gBattleContext + 57684), 0, 0);
    func_0202cbd4(gOverlay26AttackState, 0, sizeof(Overlay26AttackState));
    for (int i = 0; i < OVERLAY26_TRACKED_OBJECT_COUNT; ++i) {
        BattlePartyActor *actor =
            (BattlePartyActor *)BattleActor_GetPartySlot((u16)(i + OVERLAY26_FIRST_TRACKED_OBJECT_ID));
        gOverlay26AttackState->tracked_objects[i].state = Overlay26Attack_GetActionState(actor);
        if (actor->actor.scene_object->primary_model) {
            gOverlay26AttackState->tracked_objects[i].flag_bits.restore_actor = 1;
            gOverlay26AttackState->tracked_objects[i].object_data_id =
                actor->actor.scene_object->resource->object_data_id;
            gOverlay26AttackState->tracked_objects[i].object_id = actor->actor.scene_object->actor_id;
        }
    }
    Overlay26Attack_PrepareResources(user);
    user->callback = Overlay26Attack_UpdateEntry;
}
}
