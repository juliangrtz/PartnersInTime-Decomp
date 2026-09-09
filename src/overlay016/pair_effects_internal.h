#ifndef PIT_OVERLAY016_PAIR_EFFECTS_INTERNAL_H
#define PIT_OVERLAY016_PAIR_EFFECTS_INTERNAL_H
#include <game/battle_scene.h>
#include <game/overlay010_motion.h>
#include <game/battle_effect.h>
#include <game/battle_actor.h>
#include <game/battle_impact_effect.h>
#include <game/overlay016_attack.h>
#include "attack_state.h"

typedef union Overlay16PairControl {
    u16 raw;
    struct {
        u8 phase : 5, unknown5 : 2, unknown7 : 1;
        u8 kind;
    } bits;
} Overlay16PairControl;

typedef struct Overlay16PairEffect {
    BattleSceneObject primary, secondary;
    Overlay16PairControl control;
    u16 unknown522;
} Overlay16PairEffect;
typedef struct Overlay16PairController {
    BattleSceneObject *primary, *secondary;
    u8 unknown8[8];
    int unknown16, position_z_q12, unknown24;
    u16 radius;
    u8 phase : 5, unknown30 : 3;
    u8 unknown31;
} Overlay16PairController;
typedef struct Overlay16Participant {
    BattleSceneObject *object;
    Overlay16PairEffect *effect;
    BattleEffect *attached_effect;
    u16 unknown12;
    s16 timer;
    int component;
    s16 timer2;
    u8 phase : 5, variant : 2, unknown_flag7 : 1;
    u8 unknown23;
} Overlay16Participant;

typedef struct Overlay16LinkedObject {
    BattleSceneObject *object;
    union {
        u32 flags;
        struct {
            u32 phase : 4, unknown4 : 28;
        } bits;
    };
} Overlay16LinkedObject;
typedef struct Overlay16AttackWork {
    u8 unknown0[16];
    u8 unknown_flag0 : 1, flag_1 : 1, unknown_flags2 : 6;
    s8 phase;
    u8 unknown18[286];
    Overlay16Participant participants[4];
    Overlay16PairEffect effects[32];
    Overlay16PairController controller;
    Overlay16LinkedObject linked;
    Overlay16AttackObjectState variants[4];
    BattlePartyActor *adults[2];
    u8 unknown17264[8];
    s16 center_x, center_y, entry_offset_x, unknown17278;
    int pass_timer;
    Overlay16Participant *active, *next;
} Overlay16AttackWork;

#ifdef __cplusplus
extern "C" {
#endif
extern Overlay16AttackWork *data_ov002_020c0710;
extern u8 *gBattleContext;
extern int data_ov016_020c7400[][2];
void func_ov002_0206b5c0(BattleImpactParticlePayload *);
void Overlay16Attack_UpdateEntryObject(Overlay16AttackObjectState *state);
void Overlay16Particle_UpdateShrink(BattleAITask *task);
void Overlay16LinkedObject_Update(Overlay16LinkedObject *state);
void Overlay16LinkedObject_Hide(Overlay16LinkedObject *state);
void Overlay16LinkedObject_Initialize(Overlay16LinkedObject *state);
void Overlay16Attack_ScheduleParticipant(Overlay16Participant *participant);
int Overlay16PairController_IsWithinRadius(Overlay16PairController *controller, BattleSceneObject *object);
int Overlay16PairController_GetMotionStep(Overlay16PairController *controller);
void Overlay16PairController_UpdateScale(Overlay16PairController *controller);
void Overlay16Participant_EmitTrail(Overlay16Participant *participant);
void Overlay16Particle_UpdateGrow(BattleAITask *task);
void Overlay16Participant_PositionEffect(Overlay16Participant *participant);

void BattleSceneObject_SetAnimationMode01(BattleSceneObject *, int);
void func_ov002_02071938(BattleSceneObject *, BattleSceneObject *, int);
void func_ov002_0206f1b8(BattleModel *);
void func_ov002_0206f384(BattleModel *);
int BattleMotion_StartPursuit(BattleSceneObject *, int, int, BattleSceneObject *, int);

void Overlay16PairEffect_Initialize(Overlay16PairEffect *effect, int primary_resource,
                                    int secondary_resource);
void Overlay16PairEffect_DestroyModels(Overlay16PairEffect *effect);
void Overlay16PairEffect_Reset(Overlay16PairEffect *effect);
void Overlay16PairEffect_ResetIfRequested(Overlay16PairEffect *effect);
void Overlay16PairEffect_ChangeKind(Overlay16PairEffect *effect, int kind);
void Overlay16PairEffect_ApplyKindAnimation(Overlay16PairEffect *effect, int active);
void Overlay16PairEffect_StartPursuit(Overlay16PairEffect *effect, BattleSceneObject **target);
void Overlay16PairEffect_SetAnimation(Overlay16PairEffect *effect, int animation, int argument);
void Overlay16PairEffect_BeginFinish(Overlay16PairEffect *effect);
int Overlay16PairEffect_FindFree(void);
int Overlay16PairEffect_AreAllIdle(Overlay16PairEffect *effect, int count);
void Overlay16PairEffect_BeginKind(Overlay16PairEffect *effect, u8 kind);
void Overlay16PairController_Initialize(Overlay16PairController *controller, int x, int y);
void Overlay16PairController_Hide(Overlay16PairController *controller);
void Overlay16Participant_Initialize(Overlay16Participant *participant, BattleSceneObject *object,
                                     int variant, int component);
void Overlay16Participant_Hide(Overlay16Participant *participant);
int Overlay16Participant_IsIdle(Overlay16Participant *participant);
void Overlay16Participant_StartPhaseOne(Overlay16Participant *participant);
void Overlay16Participant_StartPhaseThree(Overlay16Participant *participant);
void Overlay16Participant_StartPhaseEight(Overlay16Participant *participant);
#ifdef __cplusplus
}
#endif
typedef char Overlay16PairControlSize[sizeof(Overlay16PairControl) == 2 ? 1 : -1];
typedef char Overlay16PairEffectSize[sizeof(Overlay16PairEffect) == 524 ? 1 : -1];
typedef char Overlay16PairControllerSize[sizeof(Overlay16PairController) == 32 ? 1 : -1];
typedef char Overlay16AttackWorkSize[sizeof(Overlay16AttackWork) == 17292 ? 1 : -1];
typedef char Overlay16LinkedObjectSize[sizeof(Overlay16LinkedObject) == 8 ? 1 : -1];
typedef char Overlay16ParticipantSize[sizeof(Overlay16Participant) == 24 ? 1 : -1];

static inline int ParticleFinished(BattleAITask *particle)
{
    return particle && particle->callback == 0;
}
#endif
