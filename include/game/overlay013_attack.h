#ifndef PARTNERS_IN_TIME_GAME_OVERLAY013_ATTACK_H
#define PARTNERS_IN_TIME_GAME_OVERLAY013_ATTACK_H

#include <game/battle_scene.h>

struct BattlePosition;

typedef union Overlay13AttackPairFlags {
    u16 raw;
    struct {
        u16 state : 5;
        u16 variant : 2;
        u16 unknown_7_15 : 9;
    } bits;
} Overlay13AttackPairFlags;

typedef struct Overlay13AttackPairState {
    BattleSceneObject *primary_object;
    BattleSceneObject *secondary_object;
    u8 unknown_08[0x24];
    int animation_component;
    u16 unknown_30;
    Overlay13AttackPairFlags flags;
} Overlay13AttackPairState;

typedef char Overlay13AttackPairState_SizeCheck[
    sizeof(Overlay13AttackPairState) == 0x34 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

void Overlay13Attack_AdvanceMotionTuning(void);
void Overlay13Attack_StartRetreat(BattleSceneObject *object);
void Overlay13Attack_AlignObjectPair(Overlay13AttackPairState *state);
void Overlay13Attack_PrepareObjectPair(Overlay13AttackPairState *state);
int Overlay13Attack_ClassifyVerticalAngle(struct BattlePosition *position,
                                          s16 *angle, u8 *animation_id);
int Overlay13Attack_UpdateAnimation(BattleSceneObject *object,
                                    int animation_id);
int Overlay13Attack_StartVerticalEntry(BattleSceneObject *object,
                                       int animation_id, int variant);

#ifdef __cplusplus
}
#endif

#endif
