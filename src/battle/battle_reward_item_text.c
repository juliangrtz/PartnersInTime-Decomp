/* Reward item names (overlay 2, 0x0206C6E4-0x0206C848).
 * The four item classes use different localized name tables. A count greater
 * than one selects the following entry, which holds the plural form.
 * Callers must supply a valid tagged item; the native default dereferences null.
 */
#include "battle_reward_items_internal.h"
#include <game/item.h>
#include <game/battle_effect.h>
extern u8 data_02050290[], data_020505c4[];
static inline const u8 *ActionRecord(int item)
{
    if ((item & ITEM_ID_ACTION_TAG) != ITEM_ID_ACTION_TAG)
        return 0;
    return (const u8 *)&gActionItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const u8 *UsableRecord(int item)
{
    if ((item & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG)
        return 0;
    return (const u8 *)&gItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const u8 *BadgeRecord(int item)
{
    if ((item & ITEM_ID_BADGE_TAG) != ITEM_ID_BADGE_TAG)
        return 0;
    return &data_02050290[20 * (item & ITEM_ID_INDEX_MASK)];
}
static inline const u8 *ClothingRecord(int item)
{
    if ((item & ITEM_ID_CLOTHING_TAG) != ITEM_ID_CLOTHING_TAG)
        return 0;
    return &data_020505c4[28 * (item & ITEM_ID_INDEX_MASK)];
}
void *BattleRewardItems_GetItemName(int item, int count)
{
    int table, plural;
    const u8 *record;
    switch (item & ITEM_ID_TAG_MASK) {
    case ITEM_ID_ACTION_TAG:
        table = 2;
        break;
    case ITEM_ID_USABLE_TAG:
        table = 6;
        break;
    case ITEM_ID_BADGE_TAG:
        table = 9;
        break;
    case ITEM_ID_CLOTHING_TAG:
        table = 11;
        break;
    }
    switch (item & ITEM_ID_TAG_MASK) {
    case ITEM_ID_ACTION_TAG:
        record = ActionRecord(item);
        break;
    case ITEM_ID_USABLE_TAG:
        record = UsableRecord(item);
        break;
    case ITEM_ID_BADGE_TAG:
        record = BadgeRecord(item);
        break;
    case ITEM_ID_CLOTHING_TAG:
        record = ClothingRecord(item);
        break;
    default:
        record = 0;
    }
    plural = 0;
    if (count > 1)
        plural = 1;
    return BattleText_GetEntry((u16)table, (u16)(*(const u16 *)(record + 2) + plural));
}
