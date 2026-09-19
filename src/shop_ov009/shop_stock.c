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
