#ifndef PIT_GAME_BATTLE_DAMAGE_REACTION_H
#define PIT_GAME_BATTLE_DAMAGE_REACTION_H

#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>

typedef struct BattleDamageReactionEffect BattleDamageReactionEffect;
typedef struct BattleDamageReactionPayload BattleDamageReactionPayload;

struct BattleDamageReactionEffect {
    u8 unknown_00[0x34];
    BattleEffect **owner_slot;
};

struct BattleDamageReactionPayload {
    s16 launch_count;
    u16 actor_id;
    BattleDamageReactionEffect *effect;
    s16 launch_phase;
    u16 padding_16;
};

struct BattleDamageReactionTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleDamageReactionPayload data;
};

typedef char BattleDamageReactionEffect_KnownSizeCheck
    [sizeof(BattleDamageReactionEffect) == 0x38 ? 1 : -1];
typedef char BattleDamageReactionTask_SizeCheck
    [sizeof(BattleDamageReactionTask) == 0x18 ? 1 : -1];
typedef char BattleDamageReactionPayload_SizeCheck
    [sizeof(BattleDamageReactionPayload) == 0x0C ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

void BattleDamage_StartPartyLaunchReaction(BattleActor *actor);
void BattleDamage_UpdatePartyLaunchReaction(BattleAITask *task);
void BattleDamage_StartPartyReaction(BattleSceneObject *object);
void BattleDamage_UpdatePartyReaction(BattleAITask *task);
void BattleDamage_StartEffectReaction(BattleSceneObject *object, int effect_id);
void BattleDamage_UpdateEffectReaction(BattleAITask *task);
void BattleDamage_StartEnemyReaction(BattleSceneObject *object, int effect_id);
void BattleDamage_UpdateEnemyReaction(BattleAITask *task);

#ifdef __cplusplus
}
#endif

#endif
