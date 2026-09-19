#include "shop_stock_rows_internal.h"
extern "C" {
#include <game/shop_stock.h>
void func_ov009_02072edc(ShopStockPanel *);
}

extern "C" void ShopStockPanel_RebuildList(ShopStockPanel *panel, int selection)
{
    panel->category = ShopStock_MapMenuCategoryToItemClass(
        data_ov009_0207ea3c.animated_background, selection);
    u16 tag;
    switch (panel->category) {
    case SHOP_ITEM_CLASS_USABLE_ITEM:
        panel->quantities = (s8 *)(gSaveData + 1166);
        panel->item_limit = 14;
        tag = ITEM_ID_USABLE_TAG;
        break;
    case SHOP_ITEM_CLASS_ACTION_ITEM:
        panel->quantities = (s8 *)(gSaveData + 1182);
        panel->item_limit = 11;
        tag = ITEM_ID_ACTION_TAG;
        break;
    case SHOP_ITEM_CLASS_WEAR:
        panel->quantities = (s8 *)(gSaveData + 1198);
        panel->item_limit = 33;
        tag = ITEM_ID_CLOTHING_TAG;
        break;
    case SHOP_ITEM_CLASS_BADGE:
        panel->quantities = (s8 *)(gSaveData + 1248);
        panel->item_limit = 41;
        tag = ITEM_ID_BADGE_TAG;
        break;
    }
    /* The native clear covers 99 entries; preserve the final array slot. */
    MI_CpuFill8(panel->items, 0, 198);
    panel->count = 0;
    for (int index = 0; index < panel->item_limit; ++index) {
        if (panel->quantities[index])
            panel->items[panel->count++] = tag | index;
    }
    panel->filter_mode = 0;
    panel->filter_item = 0;
    panel->first = 0;
    panel->ring_first = 0;
    func_ov009_02072edc(panel);
}
