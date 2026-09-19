#include "shop_list_internal.h"

s8 *ShopInventory_GetArray(u16 item)
{
    /* Unknown categories retain the original integer-to-pointer fallback. */
    s8 *inventory = (s8 *)item;
    switch (item & ITEM_ID_TAG_MASK) {
    case ITEM_ID_ACTION_TAG:
        inventory = (s8 *)gSaveData + 1182;
        break;
    case ITEM_ID_USABLE_TAG:
        inventory = (s8 *)gSaveData + 1166;
        break;
    case ITEM_ID_BADGE_TAG:
        inventory = (s8 *)gSaveData + 1248;
        break;
    case ITEM_ID_CLOTHING_TAG:
        inventory = (s8 *)gSaveData + 1198;
        break;
    }
    return inventory;
}

s8 ShopInventory_GetCount(u16 item, u32 mode)
{
    s8 *inventory = ShopInventory_GetArray(item);
    s8 count;
    u32 index;
    if (!inventory)
        return 0;
    index = item & ITEM_ID_INDEX_MASK;
    count = inventory[index];
    if ((item & ITEM_ID_TAG_MASK) == ITEM_ID_CLOTHING_TAG || (item & ITEM_ID_TAG_MASK) == ITEM_ID_BADGE_TAG) {
        if (mode < 2) {
            count -= func_ov009_0207dbc0(item);
            if (mode)
                count -= func_ov009_0207dc58(item);
            if (count < 0)
                count = 0;
        }
        if (index == 0)
            count = 0;
    }
    return count;
}
