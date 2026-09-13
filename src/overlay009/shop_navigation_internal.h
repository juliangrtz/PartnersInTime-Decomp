#ifndef PIT_SHOP_NAVIGATION_INTERNAL_H
#define PIT_SHOP_NAVIGATION_INTERNAL_H
#include "shop_rows_internal.h"
#include <game/shop_navigation.h>
extern "C" {
extern ShopItemPanel *data_ov009_0207ea20;
extern u8 data_ov009_0207ea3c[];
void *func_ov005_020698dc(int);
ShopRowTask *func_ov005_0206659c(void (*)(ShopRowTask *), int, int);
BattleModel *Overlay5ResourceA_Attach(ShopRowTask *, BattleModel *, int);
void func_ov005_02068908(BattleModel *, int, void *, int, int);
void func_ov005_020663d8(int);
}
#endif
