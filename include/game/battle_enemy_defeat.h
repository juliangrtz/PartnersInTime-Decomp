#ifndef PIT_GAME_BATTLE_ENEMY_DEFEAT_H
#define PIT_GAME_BATTLE_ENEMY_DEFEAT_H

struct BattleAITask;
struct BattleSceneObject;

#ifdef __cplusplus
extern "C" {
#endif

struct BattleAITask *BattleEnemy_StartDefeat(struct BattleSceneObject *object);

#ifdef __cplusplus
}
#endif

#endif
