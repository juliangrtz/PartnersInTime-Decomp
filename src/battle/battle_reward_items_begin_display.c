/* Start the reward display (overlay 2, 0x0206D6E0-0x0206D740).
 * Empty lists are immediately ready; populated lists enter draw phase zero.
 */
#include "battle_reward_items_internal.h"
void func_ov002_0206d270(void);
void BattleRewardItems_BeginDisplay(void)
{
    if (data_ov002_020c06a4->flags.bits.item_count == 0) {
        data_ov002_020c06a4->flags.bits.ready = 1;
        data_ov002_020c06a4->draw = 0;
    } else {
        data_ov002_020c06a4->flags.bits.ready = 0;
        data_ov002_020c06a4->flags.bits.phase = 0;
        data_ov002_020c06a4->draw = func_ov002_0206d270;
    }
}
