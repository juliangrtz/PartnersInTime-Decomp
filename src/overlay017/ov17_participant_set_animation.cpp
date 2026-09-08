#include "participant_internal.h"

extern "C" {
void Overlay17Participant_BeginRemoval(Overlay17Participant *participant)
{
    BattleModelEffect_SpawnAttached(&participant->state.effect, 855, &participant->object, 0, 0, 0, 256);
    Overlay10Motion_StopAll(&participant->object);
    BattleSound_Play(0, 0, 0, 0);
    participant->state.bits.phase = 16;
}

void Overlay17Participant_SetAnimation(Overlay17Participant *participant, int command, int value, int render_state)
{
    BattleSceneObject *object = &participant->object;
    BattleSceneObject_GetActiveModel(object);
    u16 resource =
        (command & 0x100) ? (u16)((participant->state.bits.formation & 1) + 57) : (u16)(participant->state.bits.formation + 53);
    if (!object->resource || object->resource->object_data_id != resource) {
        object->resource = BattleObjectData_GetLoadState(resource);
        object->animation_variant_offset = 0;
    }
    u16 saved = object->render_state;
    BattleSceneObject_SetAnimationFromComponent(object, (u8)command, 0, participant->state.animation_component);
    func_ov002_02071750(object, value);
    func_0200940c(BattleSceneObject_GetActiveModel(object), 256);
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    if (command & 0x8000) {
        model->flags |= 0x400;
        model->flags &= ~0x800;
    } else if ((command & 0x4000) && (participant->state.bits.formation & 1)) {
        model->flags |= 0x400;
        model->flags &= ~0x800;
    } else {
        model->flags &= ~0x400;
        model->flags &= ~0x800;
    }
    if (render_state != -1)
        object->render_state = render_state;
    else
        object->render_state = saved;
}
}
