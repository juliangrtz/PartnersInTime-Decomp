#ifndef PARTNERS_IN_TIME_GAME_OVERLAY014_ATTACK_H
#define PARTNERS_IN_TIME_GAME_OVERLAY014_ATTACK_H

#include <game/battle_scene.h>

typedef struct Overlay14AttackObjectState {
    BattleSceneObject *object;
    void *effect;
    u16 timer;
    u8 state;
    u8 padding_0b;
} Overlay14AttackObjectState;

typedef struct Overlay14AttackTarget {
    BattleSceneObject *object;
    u8 unknown_04[0x7A];
    u16 offset_table_index;
} Overlay14AttackTarget;

typedef struct Overlay14AttackTargetReference {
    Overlay14AttackTarget *target;
} Overlay14AttackTargetReference;

typedef char Overlay14AttackObjectState_SizeCheck[
    sizeof(Overlay14AttackObjectState) == 0xC ? 1 : -1];
typedef char Overlay14AttackTarget_SizeCheck[
    sizeof(Overlay14AttackTarget) == 0x80 ? 1 : -1];

int Overlay14Attack_LaunchAtTarget(
    Overlay14AttackObjectState *state,
    Overlay14AttackTargetReference *target_reference);
int Overlay14Attack_BeginTargetMove(
    Overlay14AttackObjectState *state,
    Overlay14AttackTargetReference *target_reference, int duration);
int Overlay14Attack_BeginHide(Overlay14AttackObjectState *state);
int Overlay14Attack_FinishWithEffects(Overlay14AttackObjectState *state);
int Overlay14Attack_StartApproach(Overlay14AttackObjectState *state);
int Overlay14Attack_IsIdle(Overlay14AttackObjectState *state);
int Overlay14Attack_Update(Overlay14AttackObjectState *state);
void Overlay14Attack_Stop(Overlay14AttackObjectState *state);
void Overlay14Attack_Initialize(Overlay14AttackObjectState *state,
                                BattleSceneObject *object,
                                s16 effect_anchor_z);

#endif
