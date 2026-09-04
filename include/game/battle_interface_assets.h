#ifndef PIT_GAME_BATTLE_INTERFACE_ASSETS_H
#define PIT_GAME_BATTLE_INTERFACE_ASSETS_H

#include <game/battle_task_queue.h>

BattleQueuedTask *BattleInterface_RequestLoad(void);
BattleQueuedTask *BattleInterface_RequestScreenLoad(int screen_index);
int BattleInterface_LoadItemName(int item_id);
int BattleInterface_LoadCommandMenu(int resource_id);
int BattleInterface_LoadTargetLabelResource(int resource_id);
int BattleInterface_LoadTargetName(u32 actor_id);

#endif
