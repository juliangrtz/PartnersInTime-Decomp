#include <game/battle_actor.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>

int BattleParty_SpawnLaunchImpact(BattleActor *base_actor) {
    BattlePartyActor *actor = (BattlePartyActor *)base_actor;
    s16 position[6];
    u8 animation_id = 16;

    if ((u16)(actor->formation_index + (u16)-2) <= 1) {
        animation_id = 17;
    }
    BattlePosition_StoreViewRelative(
        (BattlePosition *)position, actor->actor.scene_object->x,
        (s16)(actor->actor.scene_object->y - actor->actor.scene_object->z),
        (s16)(actor->actor.scene_object->effect_anchor_z +
              16 * (256 - actor->actor.scene_object->y)),
        actor->actor.scene_object->flags.bits.use_raw_position,
        actor->actor.scene_object->flags.bits.use_alternate_model);
    BattleModelEffect_SpawnFromResource(19, animation_id, position[0],
                                        position[1], position[2], 256);
    return BattleSound_Play(57, 0, 0, 0);
}
