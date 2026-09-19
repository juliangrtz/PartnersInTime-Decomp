#ifndef PIT_SHOP_STOCK_ROWS_INTERNAL_H
#define PIT_SHOP_STOCK_ROWS_INTERNAL_H
#include "shop_scene_internal.h"
#include "shop_sprite_internal.h"
#include <game/shop_stock_rows.h>
extern "C" {
#include <game/item.h>
ShopRowSprite *Overlay5ResourceB_Get(void *);
void func_ov005_02069084(void *, u8);
void func_ov005_020663d8(int);
void func_ov009_020721d4(int);
void func_ov009_02072080(void);
}
#define STOCK_PANEL ((ShopStockPanel *)data_ov009_0207ea28)
#endif
