#ifndef PIT_SHOP_ROWS_INTERNAL_H
#define PIT_SHOP_ROWS_INTERNAL_H

/*
 * The rows the shop's panels are drawn from.
 */
#include <game/battle_scene.h>
#include <game/shop_list.h>
#include <game/shop_panels.h>
#include <game/shop_rows.h>

#include "shop_sprite_internal.h"

class ShopRowPanelInterface
{
  public:
    virtual void destroy();
    virtual void deleteInstance();
    virtual u32 get_row_value(u16 row);
    virtual u32 get_row_adjustment(u16 row);
    virtual u32 get_row_quantity(u16 row, int mode);
};
extern "C" {
extern ShopItemPanel *data_ov009_0207ea34;
extern ShopList *data_ov009_0207ea2c;
extern u8 data_ov005_0206a180[];
BattleModel *Overlay5ResourceA_Get(ShopRowTask *);
ShopRowSprite *Overlay5ResourceB_Get(void *);
void func_ov005_020692c0(ShopRowSprite *, u16, u16, u16);
void func_ov005_02069084(void *, u8);
void func_ov009_0207c8f0(int, int, u32, int);
}
#endif
