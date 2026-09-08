#include <nitro/fx.h>
#include <game/battle_scene.h>
#include <game/battle_actor.h>
#include <game/battle_enemy_data.h>
#include <game/overlay010_enemy_state.h>

void Overlay10Motion_StopAll(BattleSceneObject *object)
{
    int i;
    for (i = 0; i <= 3; ++i)
        BattleSceneObject_StopMotionChannel(object, i, 0);
}
