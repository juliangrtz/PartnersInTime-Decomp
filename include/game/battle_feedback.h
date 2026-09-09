#ifndef PIT_GAME_BATTLE_FEEDBACK_H
#define PIT_GAME_BATTLE_FEEDBACK_H
#include <game/battle_actor.h>
#include <game/battle_effect.h>

/* Attack-specific work objects share this prefix; their later fields vary. */
typedef struct BattleFeedbackOwnerPrefix {
    u8 unknown_00[12];
    BattleEffect *effect;
} BattleFeedbackOwnerPrefix;
typedef char BattleFeedbackOwnerPrefixSizeCheck[sizeof(BattleFeedbackOwnerPrefix) == 16 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void BattleParty_ApplyNonfatalDamageFeedback(BattlePartyActor *actor, int unused, BattleSceneObject *object);
int BattleActor_IsDamageImmune(u32 actor_id);
int BattleAnimation_GetDurationThroughFrame(const GameGraphicsResource *resource, int animation, int frame,
                                            int speed);
void BattleSceneObject_SetAnimationMode01(BattleSceneObject *object, int mode_zero);
int BattleParty_RestoreIdleAnimation(BattlePartyActor *actor, BattleSceneObject *object);
void BattleActor_SetStateFlag03(int actor_id);
int BattleActor_HasTrait1(int actor_id);
void BattleParty_PlayFormationSound(BattlePartyActor *actor, int adult_sound, int baby_sound);
int BattleFeedback_IsEffectComplete(void);
void BattleFeedback_SpawnVariant(BattleSceneObject *object, int offset_x, int offset_y, s16 count);
BattleEffect *BattleFeedback_SpawnAtObject(int effect_id, BattleSceneObject *object, int offset_x,
                                           int offset_y, int z);
int BattleFeedback_IsResourceLoadPending(void);
void BattleFeedback_LoadResource(void);
#ifdef __cplusplus
}
#endif
#endif
