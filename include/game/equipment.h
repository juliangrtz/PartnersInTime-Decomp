#ifndef PIT_GAME_EQUIPMENT_H
#define PIT_GAME_EQUIPMENT_H

/*
 * The stat bonuses an equipped item grants and their application to a member.
 */
#include <game/save_data.h>

void Equipment_GetBonuses(u16 equipment, s16 *hp, s16 *power, s16 *defense, s16 *speed, s16 *stache);
void Equipment_Apply(u8 party_member, u16 equipment);
#endif
