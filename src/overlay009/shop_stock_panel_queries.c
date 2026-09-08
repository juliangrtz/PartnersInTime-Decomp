#include "shop_panels_internal.h"

u8 ShopStockPanel_GetCategory(ShopStockPanel *panel)
{
    return panel->category;
}

u8 ShopStockPanel_GetCount(ShopStockPanel *panel)
{
    return panel->count;
}

u16 ShopStockPanel_GetRowQuantity(ShopStockPanel *panel, int row)
{
    return panel->quantities[panel->items[(panel->first + row) % panel->count] & 0xfff];
}

u16 ShopStockPanel_GetRowItem(ShopStockPanel *panel, int row)
{
    return panel->items[(panel->first + row) % panel->count];
}

void ShopStockPanel_SetPosition(ShopStockPanel *panel, s32 x, s32 y)
{
    panel->x = x;
    panel->y = y;
}

s32 ShopStockPanel_GetX(ShopStockPanel *panel)
{
    return panel->x;
}

s32 ShopStockPanel_GetY(ShopStockPanel *panel)
{
    return panel->y;
}

int ShopStockPanel_GetRingFirst(ShopStockPanel *panel)
{
    return panel->ring_first;
}
