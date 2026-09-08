#ifndef PIT_GAME_OVERLAY010_MOTION_H
#define PIT_GAME_OVERLAY010_MOTION_H
#include <game/battle_scene.h>
#include <game/battle_actor.h>
#include <game/overlay010_enemy_state.h>
#ifdef __cplusplus
extern "C" {
#endif
void Overlay10Motion_AdjustBounce(void *unused, BattleSceneObject *object, int *velocity, int *height);
int Overlay10Motion_ScaleHeight(BattleSceneObject *object, int height, int scale);
void Overlay10Motion_StopAll(BattleSceneObject *object);
int Overlay10Motion_DistanceXY(BattleSceneObject *first, BattleSceneObject *second, const s16 *first_offset,
                               const s16 *second_offset);
int Overlay10Motion_DistanceXYZ(BattleSceneObject *first, BattleSceneObject *second, const s16 *first_offset,
                                const s16 *second_offset);
void Overlay10Enemy_ApplyProjectedDamage(Overlay10EnemyState *state, int actor_id, int damage);
void Overlay10Enemy_AddScaleSteps(Overlay10EnemyState *state, int actor_id, int amount);
int Overlay10Enemy_SelectReactionTarget(void);
int Overlay10Enemy_SelectProjectedSurvivor(Overlay10EnemyState *state);
s16 *Overlay10Party_GetEffectPosition(s16 *position, BattlePartyActor *actor, int relative);
#ifdef __cplusplus
}
#endif
#endif
