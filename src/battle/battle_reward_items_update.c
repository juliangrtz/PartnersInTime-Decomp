/*
 * Reward items update (overlay 2, 0x0206CC08-0x0206CC68).
 *
 * Steps the reward item display.
 */

#include "battle_reward_items_internal.h"
void BattleRewardItems_Update(void) {
    if (!data_ov002_020c06a4)
        return;
    if (data_ov002_020c06a4->update)
        data_ov002_020c06a4->update();
    /* A callback can change the shared controller or its next callback. */
    if (data_ov002_020c06a4->draw)
        data_ov002_020c06a4->draw();
}
