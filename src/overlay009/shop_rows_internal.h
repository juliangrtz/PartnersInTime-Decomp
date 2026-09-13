#ifndef PIT_SHOP_ROWS_INTERNAL_H
#define PIT_SHOP_ROWS_INTERNAL_H
#include <game/battle_scene.h>
#include <game/shop_list.h>
#include <game/shop_panels.h>
#include <game/shop_rows.h>

/* The first 64 bytes of the attached sprite, through its affine matrix. */
struct ShopRowSprite {
    u8 unknown_00[20];
    u32 attributes_0;
    union {
        u32 attributes_1;
        struct {
            u32 tile : 10, size : 2, palette : 4, unknown_16_31 : 16;
        } bits;
    };
    s32 x, y, offset_x, offset_y;
    s16 origin_x, origin_y;
    u8 unknown_30[8];
    s16 matrix[4];
};
typedef char ShopRowSpriteSizeCheck[sizeof(ShopRowSprite) == 64 ? 1 : -1];
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
