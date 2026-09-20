/* Reward count labels (overlay 2, 0x0206CB7C-0x0206CC08).
 * Cap the displayed total at 99 and prepare a space-padded, two-digit label.
 * The effect pointer and unused digit byte retain their previous values.
 */
#include "battle_reward_items_internal.h"
#include <game/format.h>
void BattleRewardCounter_Initialize(BattleRewardCounter *counter, int item, unsigned count, int y,
                                    int row)
{
    u16 amount;
    if (count > 99)
        count = 99;
    amount = (u16)count;
    counter->item = item;
    counter->count = amount;
    counter->text = BattleRewardItems_GetItemName(item, amount);
    counter->x = 0;
    counter->y = y;
    GameFormat_Decimal(counter->digits, 2, 0, amount);
    counter->delay = 2;
    counter->flags.flag0 = 0;
    counter->flags.flag1 = 0;
    counter->flags.flag2 = 1;
    counter->row = row;
}
