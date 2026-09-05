#ifndef PIT_GAME_BATTLE_DAMAGE_H
#define PIT_GAME_BATTLE_DAMAGE_H

#include <nitro.h>

struct BattleActor;
struct BattleSceneObject;

int BattleDamage_CalculateBase(int attacker_id, int defender_id, int scale_q8);
int BattleDamage_ApplyEquipmentModifiers(u32 attacker_id, u32 defender_id,
                                         int scale_q8,
                                         int condition_active);
int BattleDamage_CalculateByObject(u32 attacker_object_id,
                                   u32 defender_object_id);
void BattleDamage_ReflectQueuedHits(int target_actor_id);
void BattleEffect_SetVariant(int variant);
void BattleDamage_StartPartyLaunchReaction(struct BattleActor *actor);
void BattleDamage_StartPartyReaction(struct BattleSceneObject *object);
void BattleDamage_StartEffectReaction(struct BattleSceneObject *object,
                                      int effect_id);
void BattleDamage_StartEnemyReaction(struct BattleSceneObject *object,
                                     int effect_id);

#endif
