#ifndef PARTNERS_IN_TIME_GAME_OVERLAY012_ATTACK_STATE_H
#define PARTNERS_IN_TIME_GAME_OVERLAY012_ATTACK_STATE_H

#include <game/battle_scene.h>

typedef union Overlay12AttackFlags {
    u8 raw;
    struct {
        u8 state : 5;
        u8 formation_variant : 2;
        u8 unknown_7 : 1;
    } bits;
} Overlay12AttackFlags;

typedef struct Overlay12AttackState {
    BattleSceneObject *object;
    int timer;
    int counter;
    int animation_step;
    int unknown_10;
    u8 action_order[4];
    Overlay12AttackFlags flags;
    u8 unknown_19[3];
} Overlay12AttackState;

typedef union Overlay12ControllerFlags {
    u8 raw;
    struct {
        u8 mode : 2;
        u8 unknown_2_7 : 6;
    } bits;
} Overlay12ControllerFlags;

typedef struct Overlay12AttackController {
    BattleSceneObject *primary_object;
    BattleSceneObject *secondary_object;
    struct BattlePartyActor *actor;
    int argument;
    int unknown_10;
    u16 counter_14;
    u16 counter_16;
    Overlay12AttackFlags flags;
    Overlay12ControllerFlags controller_flags;
    u8 unknown_1a[2];
} Overlay12AttackController;

typedef char Overlay12AttackState_SizeCheck[
    sizeof(Overlay12AttackState) == 0x1C ? 1 : -1];
typedef char Overlay12AttackController_SizeCheck[
    sizeof(Overlay12AttackController) == 0x1C ? 1 : -1];

int Overlay12Attack_GetMotionParameterPair(Overlay12AttackState *state);
void Overlay12Attack_GetHomePosition(Overlay12AttackState *state,
                                     s16 *position_x, s16 *position_y);
int Overlay12Attack_IsIdle(Overlay12AttackState *state);
void Overlay12Attack_StartAnimationThree(Overlay12AttackState *state);
void Overlay12Attack_StartAnimationFour(Overlay12AttackState *state);
void Overlay12Attack_ShuffleActionOrder(Overlay12AttackState *state);
void Overlay12Attack_InitializeController(Overlay12AttackController *state,
                                          int mode,
                                          int formation_variant,
                                          BattleSceneObject *primary_object,
                                          BattleSceneObject *secondary_object,
                                          int argument);
int Overlay12Attack_CleanupController(Overlay12AttackController *state);

#endif
