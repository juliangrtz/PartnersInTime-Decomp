#include "shop_rows_internal.h"
class ShopInventoryPanelInterface : public ShopRowPanelInterface
{
  public:
    virtual void unknown_0c();
    virtual u32 get_row_quantity(u16 row, int mode);
};
extern "C" {
extern ShopItemPanel *data_ov009_0207ea24;
int func_ov009_02073e90(ShopItemPanel *, int);
void ShopInventoryRow_Update(ShopInventoryRowTask *task)
{
    int first = ShopItemPanel_GetRingFirst(data_ov009_0207ea24);
    int count = ShopItemPanel_GetVisibleRows(data_ov009_0207ea24);
    int slot = (first + task->row) % count;
    int old_value;
    task->x = task->offset_x + ShopItemPanel_GetX(data_ov009_0207ea24);
    task->y = task->offset_y + ShopItemPanel_GetY(data_ov009_0207ea24);
    task->priority = 60;
    task->tile = (36 * slot + 76) / 4;
    task->quantity = ((ShopInventoryPanelInterface *)data_ov009_0207ea24)->get_row_quantity(task->row, 0);
    old_value = task->value;
    task->value = ((ShopInventoryPanelInterface *)data_ov009_0207ea24)->get_row_value(task->row);
    if (old_value != task->value)
        func_ov009_0207c8f0(16 * task->row + 520, 30, task->value, 4);
    if (func_ov009_02073e90(data_ov009_0207ea24, task->row))
        task->palette = 0;
    else
        task->palette = 1;
}
void ShopInventoryRow_DrawTextSegment(ShopInventoryRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    ShopInventoryRowTask *row = task->parent;
    sprite->bits.tile = row->tile + 8 * task->part / 4;
    sprite->bits.palette = (data_ov005_0206a180[0x71] + row->palette) & 15;
    sprite->x = row->x;
    sprite->y = row->y;
    func_ov005_02069084(sprite, (u8)row->priority);
}
void ShopInventoryRow_DrawInsetSprite(ShopInventoryRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    ShopInventoryRowTask *row = task->parent;
    sprite->bits.tile = row->tile + 8;
    sprite->x = row->x;
    sprite->y = row->y;
    func_ov005_02069084(sprite, (u8)row->priority);
}
void ShopInventoryRow_DrawQuantityDigit(ShopInventoryRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    int divisor = task->part;
    ShopInventoryRowTask *row = task->parent;
    u8 digit = (row->quantity % (10 * divisor)) / divisor;
    if (digit || divisor == 1) {
        sprite->bits.tile = digit + 174;
        sprite->bits.palette = (data_ov005_0206a180[0x71] + row->palette) & 15;
        sprite->x = row->x;
        sprite->y = row->y;
        func_ov005_02069084(sprite, (u8)row->priority);
    }
}
void ShopInventoryRow_DrawValueSprite(ShopInventoryRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    ShopInventoryRowTask *row = task->parent;
    sprite->x = row->x;
    sprite->y = row->y;
    func_ov005_02069084(sprite, (u8)row->priority);
}
}
