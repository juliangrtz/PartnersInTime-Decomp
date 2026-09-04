#ifndef PIT_GAME_BATTLE_ACTOR_H
#define PIT_GAME_BATTLE_ACTOR_H

#include <nitro.h>

typedef struct BattleActor BattleActor;
struct BattleSceneObject;

enum BattleActorIdRange {
    BATTLE_ACTOR_PARTY_FIRST = 56,
    BATTLE_ACTOR_PARTY_COUNT = 4,
    BATTLE_ACTOR_ENEMY_FIRST = 60,
    BATTLE_ACTOR_ENEMY_COUNT = 8
};

enum BattleActorFlag {
    BATTLE_ACTOR_LEVEL_MASK = 0x007F,
    BATTLE_ACTOR_FLAG_KO = 0x0100
};

struct BattleActor {
    struct BattleSceneObject *scene_object;
    s16 max_hp;
    s16 current_hp;
    s16 speed;
    s16 power;
    s16 defense;
    u8 unk_00e[2];
    s16 base_speed;
    s16 base_power;
    s16 base_defense;
    u8 unk_016[8];
    s16 pending_damage;
    s16 target_actor_id;
    u8 unk_022[2];
    u16 flags;
    u8 unk_026[0x46];
    void *resource_slot;
};

typedef char BattleActor_SizeCheck[sizeof(BattleActor) == 0x70 ? 1 : -1];

int BattleActor_IsHpAtMostQuarter(BattleActor *actor);
int BattleActor_ApplyDamage(struct BattleSceneObject *object, int damage);
int BattleActor_CanReceiveStatus(BattleActor *actor);
BattleActor *BattleActor_GetEnemySlot(int actor_id);
BattleActor *BattleActor_GetPartySlot(int actor_id);
BattleActor *BattleActor_GetById(int actor_id);

#endif
