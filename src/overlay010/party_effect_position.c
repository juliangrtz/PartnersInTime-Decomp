#include <nitro/fx.h>
#include <game/battle_scene.h>
#include <game/battle_actor.h>
#include <game/battle_enemy_data.h>
#include <game/overlay010_enemy_state.h>
extern s8 data_ov010_020c2a70[];

s16 *Overlay10Party_GetEffectPosition(s16 *position, BattlePartyActor *actor, int relative)
{
    const s8 *offset = data_ov010_020c2a70 + 3 * actor->formation_index;
    position[0] = offset[0];
    position[1] = offset[1];
    position[2] = offset[2];
    if (relative) {
        position[0] += actor->actor.scene_object->x;
        position[1] += actor->actor.scene_object->y;
        position[2] += actor->actor.scene_object->z;
    }
    return position;
}
