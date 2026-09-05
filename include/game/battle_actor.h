#ifndef PIT_GAME_BATTLE_ACTOR_H
#define PIT_GAME_BATTLE_ACTOR_H

#include <nitro.h>

typedef struct BattleActor BattleActor;
typedef struct BattleActorAnimationState BattleActorAnimationState;
typedef struct BattleStatusState BattleStatusState;
typedef struct BattleDamageNumberTask BattleDamageNumberTask;
struct BattleSceneObject;
struct BattleModel;

enum BattleActorIdRange {
    BATTLE_ACTOR_PARTY_FIRST = 56,
    BATTLE_ACTOR_PARTY_COUNT = 4,
    BATTLE_ACTOR_ENEMY_FIRST = 60,
    BATTLE_ACTOR_ENEMY_COUNT = 8
};

enum BattlePartyActorId {
    BATTLE_ACTOR_MARIO = BATTLE_ACTOR_PARTY_FIRST,
    BATTLE_ACTOR_LUIGI,
    BATTLE_ACTOR_BABY_MARIO,
    BATTLE_ACTOR_BABY_LUIGI
};

enum BattlePartyFormationIndex {
    BATTLE_FORMATION_MARIO = 0,
    BATTLE_FORMATION_LUIGI = 1,
    BATTLE_FORMATION_BABY_MARIO = 2,
    BATTLE_FORMATION_BABY_LUIGI = 3,
    BATTLE_FORMATION_MARIO_CARRYING = 4,
    BATTLE_FORMATION_LUIGI_CARRYING = 5,
    BATTLE_FORMATION_COUNT = 6
};

enum BattleActorFlag {
    BATTLE_ACTOR_LEVEL_MASK = 0x007F,
    BATTLE_ACTOR_FLAG_07 = 0x0080,
    BATTLE_ACTOR_FLAG_KO = 0x0100,
    BATTLE_ACTOR_FLAG_HIT_LOCK = 0x0200,
    BATTLE_ACTOR_FLAG_10 = 0x0400,
    BATTLE_ACTOR_FLAG_DAMAGE_NUMBER_PENDING = BATTLE_ACTOR_FLAG_10,
    BATTLE_ACTOR_FLAG_RESOURCE_BOUND = 0x0800,
    BATTLE_ACTOR_FLAG_12 = 0x1000,
    BATTLE_ACTOR_FLAG_13 = 0x2000,
    BATTLE_ACTOR_FLAG_14 = 0x4000
};

struct BattleStatusState {
    void *effect;
    s16 duration;
    s16 counter;
    s8 kind;
    u8 parameter_1;
    union {
        struct {
            u8 parameter_2;
            u8 unknown_0b;
        };
        u16 visual_flags;
    };
};

struct BattleActor {
    struct BattleSceneObject *scene_object;
    s16 max_hp;
    s16 current_hp;
    s16 speed;
    s16 power;
    s16 defense;
    s16 unk_00e;
    s16 base_speed;
    s16 base_power;
    s16 base_defense;
    s16 unk_016;
    s16 unk_018;
    s16 unk_01a;
    s16 unk_01c;
    s16 pending_damage;
    s16 target_actor_id;
    s16 damage_scale_q8;
    union {
        u16 flags;
        struct {
            u16 level : 7;
            u16 unknown_flag_07 : 1;
            u16 knocked_out : 1;
            u16 hit_locked : 1;
            u16 unknown_flags_10_12 : 3;
            u16 excluded_from_targeting : 1;
            u16 unknown_flags_14_15 : 2;
        } flag_bits;
        struct {
            u16 unknown_flags_00_13 : 14;
            u16 force_one_damage : 1;
            u16 unknown_flag_15 : 1;
        } damage_flag_bits;
        struct {
            u16 unknown_damage_number_00_09 : 10;
            u16 damage_number_pending : 1;
            u16 damage_number_alternate : 1;
            u16 unknown_damage_number_12_15 : 4;
        } damage_number_flag_bits;
    };
    u8 unk_026[6];
    BattleDamageNumberTask *damage_number_task;
    union {
        struct {
            BattleStatusState ailment_group_1;
            BattleStatusState ailment_group_2;
            BattleStatusState power_change;
            BattleStatusState defense_change;
            BattleStatusState speed_change;
        };
        struct {
            BattleActorAnimationState *animation_state;
            u8 unk_034[4];
            s8 transition_state;
            u8 unk_039[0x0B];
            s8 force_low_hp_animation;
            u8 unk_045[0x0B];
            s8 status_channel_50;
            u8 unk_051[0x0B];
            s8 status_channel_5c;
            u8 unk_05d[0x0B];
            s8 status_channel_68;
            u8 unk_069[3];
        };
    };
    void *resource_slot;
};

struct BattleActorAnimationState {
    u8 unknown_00[0x0C];
    struct BattleModel *model;
};

typedef union BattlePartyStateFlags {
    u16 raw;
    struct {
        u16 uniform_target_selection : 1;
        u16 target_selection_locked : 1;
        u16 unknown_02_11 : 10;
        u16 flag_12 : 1;
        u16 unknown_13_15 : 3;
    } bits;
    struct {
        u16 jump_input_enabled : 1;
        u16 airborne : 1;
        u16 carried_baby_phase : 2;
        u16 unknown_04_15 : 12;
    } command_bits;
    struct {
        u16 unknown_00_03 : 4;
        u16 power_special_disabled : 1;
        u16 unknown_05_15 : 11;
    } status_bits;
} BattlePartyStateFlags;

typedef struct BattlePartyActor {
    BattleActor actor;
    u8 unknown_070[4];
    BattlePartyStateFlags state_flags;
    u8 unknown_076[8];
    u16 formation_index;
    u16 linked_object_id;
} BattlePartyActor;

typedef struct BattleEnemyActor {
    BattleActor actor;
    u8 unknown_070[0x22A];
    u16 resource_initialized;
    union {
        u32 state_flags;
        struct {
            u32 flag_00 : 1;
            u32 flag_01 : 1;
            u32 flag_02 : 1;
            u32 flag_03 : 1;
            u32 flag_04 : 1;
            u32 flag_05 : 1;
            u32 traits : 2;
            u32 unknown_08_31 : 24;
        } state_flag_bits;
    };
} BattleEnemyActor;

typedef char BattleActor_SizeCheck[sizeof(BattleActor) == 0x70 ? 1 : -1];
typedef char BattleStatusState_SizeCheck[
    sizeof(BattleStatusState) == 0x0C ? 1 : -1
];
typedef char BattlePartyActor_SizeCheck[
    sizeof(BattlePartyActor) == 0x84 ? 1 : -1
];
typedef char BattleEnemyActor_SizeCheck[
    sizeof(BattleEnemyActor) == 0x2A0 ? 1 : -1
];

#ifdef __cplusplus
extern "C" {
#endif

int BattleActor_IsHpAtMostQuarter(BattleActor *actor);
int BattleActor_ApplyDamage(struct BattleSceneObject *object, int damage);
int BattleActor_CanReceiveStatus(BattleActor *actor);
int BattleActor_IsAnyHitLocked(void);
int BattleEnemy_Remove(int actor_id, int show_damage);
int BattleParty_ShowHealingEffect(BattleActor *actor, int amount);
u32 BattleParty_AddExperience(u32 member_id, u32 amount);
int BattleParty_StartFormationTransition(int next_state, u16 mario_target_x,
                                         u16 luigi_target_x);
BattleActor *BattleActor_GetEnemySlot(int actor_id);
BattleActor *BattleActor_GetPartySlot(int actor_id);
BattleActor *BattleActor_GetById(int actor_id);

#ifdef __cplusplus
}
#endif

#endif
