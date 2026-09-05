#ifndef PARTNERS_IN_TIME_GAME_OVERLAY015_ATTACK_H
#define PARTNERS_IN_TIME_GAME_OVERLAY015_ATTACK_H

#include <game/battle_hit.h>
#include <game/battle_scene.h>

typedef struct Overlay15AttackModelOwner {
    BattleSceneObject *object;
    u8 unknown_04[0x7A];
    u16 flags;
} Overlay15AttackModelOwner;

typedef struct Overlay15AttackModelController {
    Overlay15AttackModelOwner *primary;
    u8 unknown_04[0x18];
    BattleSceneObject *secondary;
} Overlay15AttackModelController;

typedef union Overlay15AttackPhaseFlags {
    u8 raw;
    struct {
        u8 phase : 4;
        u8 unknown_4_7 : 4;
    } bits;
} Overlay15AttackPhaseFlags;

typedef struct Overlay15AttackPhaseState {
    u8 unknown_00[0x16];
    Overlay15AttackPhaseFlags flags;
} Overlay15AttackPhaseState;

typedef char Overlay15AttackModelOwner_SizeCheck[
    sizeof(Overlay15AttackModelOwner) == 0x80 ? 1 : -1];
typedef char Overlay15AttackModelController_SizeCheck[
    sizeof(Overlay15AttackModelController) == 0x20 ? 1 : -1];

void Overlay15Attack_ConfigureModelFlags(
    Overlay15AttackModelController *state);
int Overlay15Attack_ClampContextIndex(int offset);
void Overlay15Attack_EmptyHitCallback(BattleHitRecord *record);
void Overlay15Attack_ConfigureHitDescriptor(void);
int Overlay15Attack_IsLinkedPhase(Overlay15AttackPhaseState *state);

#endif
