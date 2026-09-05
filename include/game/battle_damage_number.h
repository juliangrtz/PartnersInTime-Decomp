#ifndef PIT_GAME_BATTLE_DAMAGE_NUMBER_H
#define PIT_GAME_BATTLE_DAMAGE_NUMBER_H

#include <game/battle_ai.h>
#include <game/battle_effect.h>

typedef struct BattleDamageNumberEffect BattleDamageNumberEffect;
typedef struct BattleDamageNumberPayload BattleDamageNumberPayload;

struct BattleDamageNumberEffect {
    u8 unknown_00[4];
    void (*update_callback)(BattleEffect *effect);
    u8 unknown_08[0x0C];
    s16 x;
    s16 y;
    u8 unknown_18[2];
    union {
        u16 flags;
        struct {
            u16 unknown_00_02 : 3;
            u16 active : 1;
            u16 complete : 1;
            u16 unknown_05_15 : 11;
        } flag_bits;
    };
    int value;
};

struct BattleDamageNumberPayload {
    BattleDamageNumberEffect *effect;
    u16 actor_id;
    u16 padding_06;
};

struct BattleDamageNumberTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    union {
        BattleDamageNumberPayload data;
        struct {
            BattleDamageNumberEffect *effect;
            u16 actor_id;
            u16 padding_12;
        };
    };
};

typedef char BattleDamageNumberEffect_KnownSizeCheck
    [sizeof(BattleDamageNumberEffect) == 0x20 ? 1 : -1];
typedef char BattleDamageNumberTask_SizeCheck
    [sizeof(BattleDamageNumberTask) == 0x14 ? 1 : -1];
typedef char BattleDamageNumberPayload_SizeCheck
    [sizeof(BattleDamageNumberPayload) == 8 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

void BattleDamageNumber_CleanupAttached(BattleAITask *task);
void BattleDamageNumber_UpdateAttached(BattleAITask *task);
void BattleDamageNumber_MergeAttached(BattleAITask *task);

#ifdef __cplusplus
}
#endif

#endif
