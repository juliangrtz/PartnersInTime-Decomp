#ifndef PARTNERS_IN_TIME_GAME_OVERLAY017_ATTACK_H
#define PARTNERS_IN_TIME_GAME_OVERLAY017_ATTACK_H

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif

u16 Overlay17Attack_LookupPairTuning(int value, int parameter_index);
u16 Overlay17Attack_LookupGroupTuning(int value, int parameter_index);
u16 Overlay17Attack_GetPairTuningParameter2(int value);
u16 Overlay17Attack_GetPairTuningParameter1(int value);
u16 Overlay17Attack_GetGroupTuningParameter4(int value);
u16 Overlay17Attack_GetGroupTuningParameter3(int value);
u16 Overlay17Attack_GetGroupTuningParameter2(int value);
u16 Overlay17Attack_GetGroupTuningParameter1(int value);
int Overlay17Attack_UpdatePartyObjectReadiness(void);
int Overlay17Attack_ConfigurePartyObjects(void);
int Overlay17Attack_UpdateObjectAnimations(void);

#ifdef __cplusplus
}
#endif

#endif
