/*
 * Shop stock tables (overlay 9, 0x0207DDF4-0x0207E008).
 *
 * What a shop sells: the tier descriptor for the shop being visited, the item
 * pool it draws from, and the mapping from a menu category to the item class it
 * lists. The inventory accessors return the party's array for that class.
 */

#include <game/shop_stock.h>

extern const u32 gShop0TierDescriptors
    [SHOP_ITEM_CLASS_COUNT][SHOP_STOCK_TIER_COUNT];
extern const u32 gShop1TierDescriptors
    [SHOP_ITEM_CLASS_COUNT][SHOP_STOCK_TIER_COUNT];
extern const u32 gShop2TierDescriptors
    [SHOP_ITEM_CLASS_COUNT][SHOP_STOCK_TIER_COUNT];
extern const u32 gShop3TierDescriptors
    [SHOP_ITEM_CLASS_COUNT][SHOP_STOCK_TIER_COUNT];

extern const u16 gShop0ItemPool[];
extern const u16 gShop1ItemPool[];
extern const u16 gShop2ItemPool[];
extern const u16 gShop3ItemPool[];

/* Metrowerks emits C functions in reverse source order. */
const u32 *ShopStock_GetTierDescriptor(
    int shop_id, int item_class, int tier) {
    const u32 *descriptor = 0;

    switch (shop_id) {
    case 0:
        descriptor = &gShop0TierDescriptors[item_class][tier];
        break;
    case 1:
        descriptor = &gShop1TierDescriptors[item_class][tier];
        break;
    case 2:
        descriptor = &gShop2TierDescriptors[item_class][tier];
        break;
    case 3:
        descriptor = &gShop3TierDescriptors[item_class][tier];
        break;
    }
    return descriptor;
}

const u16 *ShopStock_GetItemPool(int shop_id) {
    const u16 *pool = 0;

    switch (shop_id) {
    case 0:
        pool = gShop0ItemPool;
        break;
    case 1:
        pool = gShop1ItemPool;
        break;
    case 2:
        pool = gShop2ItemPool;
        break;
    case 3:
        pool = gShop3ItemPool;
        break;
    }
    return pool;
}

int ShopStock_MapMenuCategoryToItemClass(
    int menu_category, int subcategory) {
    if (menu_category == SHOP_ITEM_CLASS_WEAR) {
        return SHOP_ITEM_CLASS_BADGE;
    }
    return (u8)(subcategory + 2 * menu_category);
}

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
