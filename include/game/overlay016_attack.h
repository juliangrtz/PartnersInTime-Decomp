#ifndef PARTNERS_IN_TIME_OVERLAY016_ATTACK_H
#define PARTNERS_IN_TIME_OVERLAY016_ATTACK_H

#include <nitro.h>

typedef struct Overlay16AttackObjectState Overlay16AttackObjectState;
typedef struct Overlay16AttackTargetReference Overlay16AttackTargetReference;

#ifdef __cplusplus
extern "C" {
#endif

void Overlay16Attack_AdvanceMotionTable(void);
void *Overlay16Attack_GetMotionRecord(u32 packed_index);
int Overlay16Attack_IsObjectIdle(Overlay16AttackObjectState *state);
int Overlay16Attack_IsObjectMotionComplete(
    Overlay16AttackObjectState *state);
void Overlay16Attack_ResetObjectState(Overlay16AttackObjectState *state);
void Overlay16Attack_InitializeObjectState(
    Overlay16AttackObjectState *state, int variant);
void Overlay16Attack_LaunchObjectAtReference(
    Overlay16AttackObjectState *state,
    Overlay16AttackTargetReference *reference);
void Overlay16Attack_BeginObjectPhaseThree(
    Overlay16AttackObjectState *state);
void Overlay16Attack_BeginObjectPhaseTwo(
    Overlay16AttackObjectState *state);
void Overlay16Attack_BeginObjectTargetMotion(
    Overlay16AttackObjectState *state,
    Overlay16AttackTargetReference *reference, int duration);

#ifdef __cplusplus
}
#endif

#endif
