#ifndef PARTNERS_IN_TIME_GAME_SHOP_STOCK_H
#define PARTNERS_IN_TIME_GAME_SHOP_STOCK_H

#include <nitro.h>

enum ShopItemClass {
    SHOP_ITEM_CLASS_USABLE_ITEM = 0,
    SHOP_ITEM_CLASS_ACTION_ITEM = 1,
    SHOP_ITEM_CLASS_WEAR = 2,
    SHOP_ITEM_CLASS_BADGE = 3,
    SHOP_ITEM_CLASS_COUNT = 4,
};

enum {
    SHOP_STOCK_TIER_COUNT = 6,
};

#ifdef __cplusplus
extern "C" {
#endif

int ShopStock_MapMenuCategoryToItemClass(
    int menu_category, int subcategory);
const u16 *ShopStock_GetItemPool(int shop_id);
const u32 *ShopStock_GetTierDescriptor(
    int shop_id, int item_class, int tier);

#ifdef __cplusplus
}
#endif

#endif
