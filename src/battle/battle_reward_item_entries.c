/* Reward-list entries (overlay 2, 0x0206DE30-0x0206DFD0).
 * Each item has separate regular, action-item and guaranteed reward counts.
 * Initialization uses categories 1..3; lookup uses count indices 0..2 or -1
 * for any category. Callers supply a valid tagged item and an entry within
 * the eight-slot list. The native routines do not validate those inputs.
 */
#include "battle_reward_items_internal.h"
#include <game/item.h>
extern u8 data_02050290[], data_020505c4[];
static inline const u8 *ActionRecord(u16 item)
{
    if ((item & ITEM_ID_ACTION_TAG) != ITEM_ID_ACTION_TAG)
        return 0;
    return (const u8 *)&gActionItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const u8 *UsableRecord(u16 item)
{
    if ((item & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG)
        return 0;
    return (const u8 *)&gItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const u8 *BadgeRecord(u16 item)
{
    if ((item & ITEM_ID_BADGE_TAG) != ITEM_ID_BADGE_TAG)
        return 0;
    return &data_02050290[20 * (item & ITEM_ID_INDEX_MASK)];
}
static inline const u8 *ClothingRecord(u16 item)
{
    if ((item & ITEM_ID_CLOTHING_TAG) != ITEM_ID_CLOTHING_TAG)
        return 0;
    return &data_020505c4[28 * (item & ITEM_ID_INDEX_MASK)];
}
static inline const u8 *GetItemRecord(u16 item)
{
    const u8 *record = 0;
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
    return record;
}
int BattleRewardItems_FindEntry(BattleRewardItemEntry *entries, int count, int item, int category)
{
    int i = 0;
    if (count > 0) {
        u8 *selected = (u8 *)entries + category;
        do {
            if (item == entries->item) {
                if (category == -1)
                    return i;
                if (selected[4])
                    return i;
            }
            ++i;
            ++entries;
            selected += sizeof(*entries);
        } while (i < count);
    }
    return -1;
}
void BattleRewardItems_InitEntry(BattleRewardItemEntry *entries, int index, int item, int count,
                                 int category)
{
    u8 *counts = entries[0].counts;
    entries[index].item = (u16)item;
    /* The native word clear also resets the byte after the three counts. */
    *(u32 *)(counts + sizeof(*entries) * index) = 0;
    (counts + sizeof(*entries) * index)[category - 1] = count;
    /* List selection later compares this halfword from the item record. */
    entries[index].record_value = *(const u16 *)(GetItemRecord((u16)item) + 12);
}
