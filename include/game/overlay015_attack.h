#ifndef PARTNERS_IN_TIME_GAME_OVERLAY015_ATTACK_H
#define PARTNERS_IN_TIME_GAME_OVERLAY015_ATTACK_H

#include <game/battle_hit.h>
#include <game/battle_scene.h>

typedef struct Overlay15AttackModelOwner {
    BattleSceneObject *object;
    u8 unknown_04[0x7A];
    u16 flags;
} Overlay15AttackModelOwner;

typedef union Overlay15AttackPhaseFlags {
    u8 raw;
    struct {
        u8 phase : 4;
        u8 unknown_4_7 : 4;
    } bits;
} Overlay15AttackPhaseFlags;

typedef struct Overlay15AttackHitMotion {
    BattleSceneObject *object;
    u8 unknown_04[0xC];
    s32 window_end_q8;
    s32 window_start_q8;
} Overlay15AttackHitMotion;

typedef struct Overlay15AttackModelController {
    Overlay15AttackModelOwner *primary;
    u8 unknown_04[4];
    u16 secondary_resource_id;
    u16 primary_resource_id;
    s16 hit_timer;
    u8 unknown_0e[2];
    s8 hit_active;
    Overlay15AttackPhaseFlags phase_flags;
    u8 unknown_12[0xA];
    Overlay15AttackHitMotion hit_motion;
} Overlay15AttackModelController;

typedef struct Overlay15AttackPhaseState {
    u8 unknown_00[0x16];
    Overlay15AttackPhaseFlags flags;
} Overlay15AttackPhaseState;

typedef struct Overlay15AttackObjectPairState {
    BattleSceneObject *primary;
    BattleSceneObject *secondary;
    u8 unknown_08[0xE];
    u8 flags;
    u8 phase;
    u16 timer;
    u16 rotation;
} Overlay15AttackObjectPairState;

typedef char Overlay15AttackModelOwner_SizeCheck[
    sizeof(Overlay15AttackModelOwner) == 0x80 ? 1 : -1];
typedef char Overlay15AttackModelController_SizeCheck[
    sizeof(Overlay15AttackModelController) == 0x34 ? 1 : -1];
typedef char Overlay15AttackHitMotion_SizeCheck[
    sizeof(Overlay15AttackHitMotion) == 0x18 ? 1 : -1];
typedef char Overlay15AttackObjectPairState_SizeCheck[
    sizeof(Overlay15AttackObjectPairState) == 0x1C ? 1 : -1];

void Overlay15Attack_ConfigureModelFlags(
    Overlay15AttackModelController *state);
int Overlay15Attack_ClampContextIndex(int offset);
void Overlay15Attack_EmptyHitCallback(BattleHitRecord *record);
void Overlay15Attack_ConfigureHitDescriptor(void);
int Overlay15Attack_IsLinkedPhase(Overlay15AttackPhaseState *state);
void Overlay15Attack_InitializeObjectPair(
    Overlay15AttackObjectPairState *state);
void Overlay15Attack_PreparePrimaryActor(
    Overlay15AttackModelController *state);
void Overlay15Attack_PrepareSecondaryActor(
    Overlay15AttackModelController *state);

#endif
