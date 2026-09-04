#ifndef PIT_GAME_BATTLE_SCENE_H
#define PIT_GAME_BATTLE_SCENE_H

#include <nitro.h>

typedef struct BattleSceneObject BattleSceneObject;

struct BattleSceneObject {
    u8 unk_000[0xEC];
    u16 actor_id;
};

#endif
