#ifndef PARTNERS_IN_TIME_OVERLAY016_ATTACK_H
#define PARTNERS_IN_TIME_OVERLAY016_ATTACK_H

#include <nitro.h>

typedef struct Overlay16AttackObjectState Overlay16AttackObjectState;

#ifdef __cplusplus
extern "C" {
#endif

void Overlay16Attack_AdvanceMotionTable(void);
void *Overlay16Attack_GetMotionRecord(u32 packed_index);
int Overlay16Attack_IsObjectIdle(Overlay16AttackObjectState *state);
int Overlay16Attack_IsObjectMotionComplete(
    Overlay16AttackObjectState *state);

#ifdef __cplusplus
}
#endif

#endif
