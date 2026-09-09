#ifndef PIT_OVERLAY013_TRAMPOLINE_INTERNAL_H
#define PIT_OVERLAY013_TRAMPOLINE_INTERNAL_H
#include "../overlay010/attack_common_internal.h"
#include <game/overlay013_attack.h>
extern "C" {
#include <game/battle_object.h>
#include <game/battle_impact_effect.h>
}
struct Overlay13Pair {
    BattleSceneObject *primary, *secondary;
    u8 unknown_08[24];
    int retreat_timer;
    int resource;
    const int *configuration;
    int load_offset;
    u16 actor_id;
    Overlay13AttackPairFlags flags;
};
struct Overlay13Work {
    union {
        Overlay10Work common;
        struct {
            u8 unknown_00[302];
            u16 user_resource;
        };
    };
    u16 partner_resource, background[4];
    u8 low_entry : 1, restoring : 1, unknown_13a : 6;
    u8 unknown_13b[2], phase, unknown_13e[2];
    Overlay13Pair pairs[4];
    u8 unknown_210[16];
    int input_value, input_limit, timer, tuning;
    s16 velocity, acceleration;
    int parameter, delay;
    u8 unknown_23c[4];
    BattleAITask *effect;
};
typedef char Overlay13WorkSize[sizeof(Overlay13Work) == 580 ? 1 : -1];
typedef char Overlay13PairSize[sizeof(Overlay13Pair) == 52 ? 1 : -1];
extern "C" {
void func_ov013_020c5210(Overlay10ActionActor *);
void func_ov013_020c4228(Overlay10ActionActor *);
void BattleFeedback_LoadResource();
void func_ov002_020b03b8(int, int, int, int);
void func_ov002_02071f08(int, BattleSceneObject *, int, int, int, int, int);
int BattleFeedback_IsEffectComplete();
void func_ov002_0206b5f0(BattleAITask *);
void func_ov002_0206bda4(BattleAITask *);
BattleImpactParticleTask *func_ov002_0206bdf8(BattleSceneObject *, void (*)(BattleAITask *));
void func_ov002_02072004(BattleSceneObject *, int, int, int, int);
void BattleSceneObject_SetAnimationMode01(BattleSceneObject *, int);
extern int data_ov013_020c5b30[][3], data_ov013_020c5b14, data_ov013_020c5b08, data_ov013_020c5b0c,
    data_ov013_020c5b00;

void Overlay13Attack_Initialize(Overlay10ActionActor *user);
void Overlay13Attack_ShowLandingEffect();
int Overlay13Attack_StartJump(BattleSceneObject *object, int animation, s8 facing, int velocity,
                              int acceleration);
void Overlay13Attack_RestoreBattle(Overlay10ActionActor *user);
int Overlay13Attack_UpdateArcEffects(BattleSceneObject *object, BattleSceneObject *copy,
                                     int update_animation);
int Overlay13Attack_UpdateBounce(BattleSceneObject *object, BattleSceneObject *copy, int landing_animation,
                                 int launch_animation, int falling_animation);
int Overlay13Attack_UpdateRetreat(BattleSceneObject *object, Overlay13Pair *pair);
}
#endif
