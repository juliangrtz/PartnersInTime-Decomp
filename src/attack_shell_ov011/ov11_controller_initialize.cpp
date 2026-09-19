#include "shell_internal.h"

extern "C" {
void Overlay11Controller_Initialize(Overlay11Controller *controller, int actor_id, BattlePartyActor *user,
                                    int load_id, u16 saved_id, Overlay11ControllerConfig *rows, int side)
{
    BattlePartyActor *actor = (BattlePartyActor *)BattleActor_GetById(actor_id);
    u32 formation = actor->formation_index;
    if (formation >= 4)
        controller->formation = (u8)(formation & 1);
    else
        controller->formation = (u8)formation;
    controller->actor = actor;
    controller->target = BattleActor_GetById((u16)user->actor.target_actor_id);
    controller->config = &rows[actor->formation_index];
    GameSprite_CopyBytes(controller->config, &controller->offset_x, 4);
    controller->button = data_ov002_020be704[controller->formation];
    controller->load_id = load_id;
    controller->actor_resource_id = actor->actor.scene_object->resource->object_data_id;
    controller->saved_resource_id = saved_id;
    BattleCollision_GetBounds((BattleCollisionBounds *)controller->bounds,
                              actor->actor.scene_object->actor_id, 0, 0);
    *(u16 *)&actor->actor.target_actor_id = *(u16 *)&user->actor.target_actor_id;
    controller->side = (u8)side;
    actor->actor.scene_object->effect_anchor_z = -8;
}
}
