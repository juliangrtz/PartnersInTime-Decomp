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
    BATTLE_ACTOR_FLAG_KO = 0x0100,
    BATTLE_ACTOR_FLAG_HIT_LOCK = 0x0200
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
    union {
        u16 flags;
        struct {
            u16 level : 7;
            u16 unknown_flag_07 : 1;
            u16 knocked_out : 1;
            u16 hit_locked : 1;
            u16 unknown_flags_10_15 : 6;
        } flag_bits;
    };
    u8 unk_026[0x12];
    s8 transition_state;
    u8 unk_039[0x33];
    void *resource_slot;
};

typedef struct BattlePartyActor {
    BattleActor actor;
    u8 unknown_070[0x0E];
    u16 formation_index;
    u16 linked_object_id;
} BattlePartyActor;

typedef char BattleActor_SizeCheck[sizeof(BattleActor) == 0x70 ? 1 : -1];
typedef char BattlePartyActor_SizeCheck[
    sizeof(BattlePartyActor) == 0x84 ? 1 : -1
];

int BattleActor_IsHpAtMostQuarter(BattleActor *actor);
int BattleActor_ApplyDamage(struct BattleSceneObject *object, int damage);
int BattleActor_CanReceiveStatus(BattleActor *actor);
int BattleParty_ShowHealingEffect(BattleActor *actor, int amount);
u32 BattleParty_AddExperience(u32 member_id, u32 amount);
int BattleParty_StartFormationTransition(int next_state, u16 mario_target_x,
                                         u16 luigi_target_x);
BattleActor *BattleActor_GetEnemySlot(int actor_id);
BattleActor *BattleActor_GetPartySlot(u16 actor_id);
BattleActor *BattleActor_GetById(int actor_id);

#endif
