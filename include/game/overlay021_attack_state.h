#ifndef PARTNERS_IN_TIME_GAME_OVERLAY021_ATTACK_STATE_H
#define PARTNERS_IN_TIME_GAME_OVERLAY021_ATTACK_STATE_H

#include <game/battle_scene.h>

enum Overlay21AttackStateFlag {
    OVERLAY21_ATTACK_PRIMARY_PHASE_MASK = 0x1E0,
    OVERLAY21_ATTACK_SECONDARY_PHASE_MASK = 0x1E00,
    OVERLAY21_ATTACK_SECONDARY_PHASE_1 = 0x200,
    OVERLAY21_ATTACK_SECONDARY_PHASE_2 = 0x400
};

typedef struct Overlay21AttackConfig {
    u8 unknown_00[0x26];
    u16 phase_1_duration;
    u16 phase_2_duration;
} Overlay21AttackConfig;

typedef struct Overlay21AttackState {
    BattleSceneObject *object;
    void *unknown_04;
    Overlay21AttackConfig *config;
    u8 unknown_0c[4];
    int primary_timer;
    int secondary_timer;
    u32 flags;
} Overlay21AttackState;

#endif
