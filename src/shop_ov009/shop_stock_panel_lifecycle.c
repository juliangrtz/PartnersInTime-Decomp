#include "shop_panels_internal.h"

ShopStockPanel *ShopStockPanel_Init(ShopStockPanel *panel)
{
    panel->unknown_00 = 0;
    ShopStockPanel_SetPosition(panel, 65536, 221184);
    return panel;
}

void ShopStockPanel_Destroy(ShopStockPanel *panel) {}
