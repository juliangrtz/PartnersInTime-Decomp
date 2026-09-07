#ifndef PIT_GAME_INVENTORY_H
#define PIT_GAME_INVENTORY_H
#include <game/save_data.h>
int GameInventory_GetCount(u16 item);
int GameInventory_Add(u16 item, int amount);
void GameParty_Initialize(int preset);
#endif
