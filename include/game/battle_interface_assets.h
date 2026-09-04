#ifndef PIT_GAME_BATTLE_INTERFACE_ASSETS_H
#define PIT_GAME_BATTLE_INTERFACE_ASSETS_H

#include <game/battle_task_queue.h>

BattleQueuedTask *BattleInterface_RequestLoad(void);
BattleQueuedTask *BattleInterface_RequestScreenLoad(int screen_index);

#endif
