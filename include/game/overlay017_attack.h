#ifndef PARTNERS_IN_TIME_GAME_OVERLAY017_ATTACK_H
#define PARTNERS_IN_TIME_GAME_OVERLAY017_ATTACK_H

#include <nitro.h>

typedef struct Overlay17AttackObject Overlay17AttackObject;

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
void *Overlay17Attack_QueueDisplayInitialization(void);
void Overlay17Attack_InitializeDisplayPattern(void);
void Overlay17Attack_FinalizeDisplay(void);
int Overlay17Attack_ConfigureDisplay(int argument);
int Overlay17Attack_AreObjectStatesIdle(Overlay17AttackObject *objects,
                                        int object_count);
void Overlay17Attack_ConfigureObjectModel(
    Overlay17AttackObject *object, int animation_command,
    int model_value, int render_state,
    int synchronize_animation_layer, int flag_10);
void Overlay17Attack_StartHorizontalExit(Overlay17AttackObject *object,
                                         int motion_variant);
void Overlay17Attack_LaunchObject(Overlay17AttackObject *object,
                                  int destination_x, int destination_y,
                                  int start_x, int start_y, int start_z,
                                  int timing_parameter);

#ifdef __cplusplus
}
#endif

#endif
