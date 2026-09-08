#include "item_internal.h"

extern "C" {
void Overlay26Attack_GetWorldPosition(const s8 (*offsets)[3], s16 *position, int formation,
                                      BattleSceneObject *object, int relative)
{
    int index = formation;
    if ((u32)(formation - 2) <= 1 && object->actor_id >= 58) {
        index = (formation & 1) + 6;
        object = BattleSceneObject_GetById((u16)((index & 1) + 56));
    }
    const s8 *offset = offsets[index];
    position[0] = offset[0];
    position[1] = offset[1];
    position[2] = offset[2];
    if (relative) {
        position[0] += object->x;
        position[1] += object->y;
        position[2] += object->z;
    }
}

void Overlay26Attack_GetViewPosition(const s8 (*offsets)[3], s16 *position, int formation,
                                     BattleSceneObject *object, int relative)
{
    int index = formation;
    if ((u32)(formation - 2) <= 1 && object->actor_id >= 58) {
        index = (formation & 1) + 6;
        object = BattleSceneObject_GetById((u16)((index & 1) + 56));
    }
    const s8 *offset = offsets[index];
    position[0] = offset[0];
    position[1] = offset[1];
    position[2] = offset[2];
    if (relative) {
        BattlePosition view;
        BattlePosition_StoreViewRelative(&view, object->x, (s16)(object->y - object->z),
                                         (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
                                         object->flags.bits.use_raw_position,
                                         object->flags.bits.use_alternate_model);
        position[0] += view.x;
        position[1] += view.y;
        position[2] += view.z;
    }
}

void Overlay26Attack_GetActorPosition(s16 *position, BattlePartyActor *actor, int relative)
{
    int formation = actor->formation_index, state;
    if ((u32)(formation - 2) <= 1 && actor->actor.scene_object->actor_id >= 58)
        state = Overlay26Attack_GetTrackedObjectState((u16)((formation & 1) + 56));
    else
        state = Overlay26Attack_GetTrackedObjectState(actor->actor.scene_object->actor_id);
    const s8(*offsets)[3];
    switch (state) {
    case 1:
        offsets = data_ov026_020c63d8;
        break;
    case 2:
        offsets = data_ov026_020c6420;
        break;
    case 3:
        offsets = data_ov026_020c6468;
        break;
    default:
        offsets = data_ov026_020c6498;
        break;
    }
    Overlay26Attack_GetWorldPosition(offsets, position, actor->formation_index, actor->actor.scene_object,
                                     relative);
}

int Overlay26Attack_GetPhasePosition(s16 *position, int phase, BattlePartyActor *actor, int relative)
{
    Overlay26Attack_GetWorldPosition(data_ov026_020c6520[phase], position, actor->formation_index,
                                     actor->actor.scene_object, relative);
    return data_ov026_020c64e0[phase][actor->formation_index];
}
}
