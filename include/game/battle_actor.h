#ifndef PIT_GAME_BATTLE_ACTOR_H
#define PIT_GAME_BATTLE_ACTOR_H

#include <nitro.h>

typedef struct BattleActor BattleActor;

struct BattleActor {
    const void *resource;
    s16 max_hp;
    s16 current_hp;
    s16 speed;
    s16 power;
    s16 defense;
    u8 unk_00e[0x10];
    s16 hit_state;
    u8 unk_020[4];
    u16 flags;
};

int BattleActor_IsHpAtMostQuarter(BattleActor *actor);
int BattleDamage_CalculateBase(int attacker_id, int defender_id, int scale_q8);
struct BattleSceneObject;
int BattleActor_ApplyDamage(struct BattleSceneObject *object, int damage);
BattleActor *BattleActor_GetEnemySlot(int actor_id);
BattleActor *BattleActor_GetPartySlot(int actor_id);
BattleActor *BattleActor_GetById(int actor_id);

#endif
