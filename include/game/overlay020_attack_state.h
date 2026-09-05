#ifndef PARTNERS_IN_TIME_GAME_OVERLAY020_ATTACK_STATE_H
#define PARTNERS_IN_TIME_GAME_OVERLAY020_ATTACK_STATE_H

#include <game/battle_actor.h>

typedef union Overlay20AttackFlags {
    u16 raw;
    struct {
        u16 state : 5;
        u16 formation_variant : 4;
        u16 mode : 2;
        u16 unknown_11_15 : 5;
    } bits;
} Overlay20AttackFlags;

typedef struct Overlay20AttackConfig {
    u8 unknown_00[0x3A];
    s16 arc_parameter_y;
    s16 arc_parameter_x;
    u8 unknown_3e[0x36];
} Overlay20AttackConfig;

typedef struct Overlay20AttackState {
    struct BattleSceneObject *primary_object;
    struct BattleSceneObject *secondary_object;
    BattlePartyActor *actor;
    int animation_argument;
    int unknown_10;
    int unknown_14;
    int motion_duration;
    Overlay20AttackFlags flags;
    u8 unknown_1e[2];
} Overlay20AttackState;

typedef char Overlay20AttackState_SizeCheck[
    sizeof(Overlay20AttackState) == 0x20 ? 1 : -1];
typedef char Overlay20AttackConfig_SizeCheck[
    sizeof(Overlay20AttackConfig) == 0x74 ? 1 : -1];

void Overlay20Attack_StopObjects(Overlay20AttackState *state);
void Overlay20Attack_Initialize(Overlay20AttackState *state,
                                BattlePartyActor *actor);
void Overlay20Attack_ResetActorPosition(Overlay20AttackState *state);
void Overlay20Attack_BeginFinish(Overlay20AttackState *state,
                                 Overlay20AttackState *other);
void Overlay20Attack_SetMode(Overlay20AttackState *state, int mode);
void Overlay20Attack_StartTargetArc(Overlay20AttackState *state);

#endif
