#ifndef PIT_GAME_BATTLE_ACTOR_H
#define PIT_GAME_BATTLE_ACTOR_H

#include <nitro.h>

typedef struct BattleActor BattleActor;

struct BattleActor {
    const void *resource;
    s16 max_hp;
    s16 current_hp;
    u8 unk_008[0x16];
    s16 hit_state;
    u8 unk_020[4];
    u16 flags;
};

int BattleActor_IsHpAtMostQuarter(BattleActor *actor);
struct BattleSceneObject;
int BattleActor_ApplyDamage(struct BattleSceneObject *object, int damage);
BattleActor *BattleActor_GetEnemySlot(int actor_id);
BattleActor *BattleActor_GetPartySlot(int actor_id);
BattleActor *BattleActor_GetById(int actor_id);

#endif
