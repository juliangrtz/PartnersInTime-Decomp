/*
 * Stock caption (overlay 9, 0x0207218C-0x020721D4).
 *
 * Draws one segment of the stock panel's caption.
 */

#include "shop_stock_rows_internal.h"

extern "C" void ShopStockCaption_DrawSegment(ShopStockRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    sprite->y = ShopStockPanel_GetY(STOCK_PANEL) - 73728 - data_ov009_0207ea3c.panel_offset_y;
    func_ov005_02069084(sprite, 40);
}
