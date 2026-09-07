#ifndef PIT_GAME_EQUIPMENT_H
#define PIT_GAME_EQUIPMENT_H
#include <game/save_data.h>

void Equipment_GetBonuses(u16 equipment, s16 *hp, s16 *power, s16 *defense, s16 *speed, s16 *stache);
void Equipment_Apply(u8 party_member, u16 equipment);
#endif
