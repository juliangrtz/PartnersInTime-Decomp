#ifndef PIT_GAME_BATTLE_COMMON_ASSETS_H
#define PIT_GAME_BATTLE_COMMON_ASSETS_H

/*
 * Requests the graphics every battle needs regardless of the encounter.
 */

#include <game/battle_task_queue.h>

BattleQueuedTask *BattleCommonAssets_RequestLoad(int archive_index);

#endif
