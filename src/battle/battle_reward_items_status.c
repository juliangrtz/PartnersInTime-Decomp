/*
 * Reward items close (overlay 2, 0x0206CE30-0x0206CED4).
 *
 * Begins closing the reward item display.
 */

#include "battle_reward_items_internal.h"
#include <game/battle_context.h>
int BattleRewardItems_IsReady(void) { return data_ov002_020c06a4->flags.bits.ready; }
void BattleRewardItems_BeginClose(void) {
    if (data_ov002_020c06a4->flags.bits.item_count) {
        *(s16 *)(gBattleContext + 54) = 16;
        *(s16 *)(gBattleContext + 52) = 47;
        *(s16 *)(gBattleContext + 50) = 16;
        *(s16 *)(gBattleContext + 48) = 0;
        data_ov002_020c06a4->fade = 16;
        data_ov002_020c06a4->flags.bits.phase = 7;
    }
}
