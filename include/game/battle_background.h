#ifndef PIT_GAME_BATTLE_BACKGROUND_H
#define PIT_GAME_BATTLE_BACKGROUND_H

#include <game/battle_context.h>
#include <game/battle_task_queue.h>

BattleQueuedTask *BattleBackground_RequestLoad(void);
void BattleBackground_LoadTask(BattleQueuedTask *task);
void BattleBackground_FinishLoadTask(BattleQueuedTask *task);

BattleQueuedTask *BattleBackground_RequestToggle(void);
int BattleBackground_ToggleTask(BattleQueuedTask *task);

BattleQueuedTask *BattleFieldAssets_RequestReload(void);
void BattleFieldAssets_ReloadTask(BattleQueuedTask *task);
void BattleFieldAssets_FinishReloadTask(BattleQueuedTask *task);

#endif
