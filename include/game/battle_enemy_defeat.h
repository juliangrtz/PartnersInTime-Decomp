#ifndef PIT_GAME_BATTLE_ENEMY_DEFEAT_H
#define PIT_GAME_BATTLE_ENEMY_DEFEAT_H

#include <game/battle_ai.h>

struct BattleAITask;
struct BattleSceneObject;

typedef struct BattleEnemyDefeatPayload {
    struct BattleSceneObject *object;
    s32 timer;
} BattleEnemyDefeatPayload;

typedef struct BattleEnemyDefeatTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleEnemyDefeatPayload data;
} BattleEnemyDefeatTask;

typedef char BattleEnemyDefeatPayload_SizeCheck[sizeof(BattleEnemyDefeatPayload) == 8 ? 1 : -1];
typedef char BattleEnemyDefeatTask_SizeCheck[sizeof(BattleEnemyDefeatTask) == 0x14 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

void BattleEnemy_UpdateDefeatRemoval(BattleAITask *task);
void BattleEnemy_UpdateDelayedDefeatRemoval(BattleAITask *task);
struct BattleAITask *BattleEnemy_StartDefeat(struct BattleSceneObject *object);

#ifdef __cplusplus
}
#endif

#endif
