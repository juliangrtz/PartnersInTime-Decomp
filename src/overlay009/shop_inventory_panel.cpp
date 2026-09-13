#include "shop_rows_internal.h"
#include <game/save_data.h>
#include "shop_price_internal.h"

extern "C" {
extern u8 data_ov009_0207ea3c[];
struct ShopEmptyMessages {
    u16 entries[4];
};
typedef char ShopEmptyMessagesSizeCheck[sizeof(ShopEmptyMessages) == 8 ? 1 : -1];
extern ShopEmptyMessages data_ov009_0207e238;
void func_ov005_020663d8(int);
void func_ov005_02069bcc(int, int, int, int);
void func_ov009_02073538(int);
void func_ov009_02073228(int);
void func_ov009_02070764(ShopInventoryPanel *);
void func_ov009_0207a844(int, u16);
}
extern "C" {
u32 ShopInventoryPanel_GetPriceBonus(ShopInventoryPanel *panel, u16 row)
{
    const ShopPriceRecord *record = ShopPrice_GetRecord(ShopItemPanel_GetRowItem((ShopItemPanel *)panel, row));
    if (record) {
        int base = 25 * record->price / 100;
        if (!base && record->price)
            base = 1;
        return ((ShopRowPanelInterface *)panel)->get_row_value(row) - base;
    }
    return 0;
}
s8 ShopInventoryPanel_GetRowQuantity(ShopInventoryPanel *panel, u16 row, u32 mode)
{
    return ShopInventory_GetCount(ShopItemPanel_GetRowItem((ShopItemPanel *)panel, row), mode);
}
int ShopInventoryPanel_CanSellRow(ShopInventoryPanel *panel, u16 row)
{
    return (u16)ShopInventory_GetCount(ShopItemPanel_GetRowItem((ShopItemPanel *)panel, row), 1) != 0;
}
void ShopInventoryPanel_ShowEmptyMessage(ShopInventoryPanel *panel)
{
    ShopEmptyMessages messages = data_ov009_0207e238;
    func_ov009_0207a844(2, messages.entries[data_ov009_0207ea3c[0x97]]);
}
int ShopInventoryPanel_GetCategory(ShopInventoryPanel *panel, int item)
{
    switch (item & ITEM_ID_TAG_MASK) {
    case ITEM_ID_USABLE_TAG:
        return 0;
    case ITEM_ID_ACTION_TAG:
        return 1;
    case ITEM_ID_CLOTHING_TAG:
        return 2;
    case ITEM_ID_BADGE_TAG:
        return 3;
    default:
        return 0;
    }
}
void ShopInventoryPanel_CycleCategory(ShopInventoryPanel *panel, s8 delta)
{
    u16 item =
        ShopItemPanel_GetRowItem((ShopItemPanel *)panel, ShopItemPanel_GetSelection((ShopItemPanel *)panel));
    s8 category = ShopInventoryPanel_GetCategory(panel, item);
    int previous = category;
    do {
        category = (s8)(category + delta);
        if (category >= 4)
            category = 0;
        if (category < 0)
            category = 3;
    } while (panel->category_first[category] == -1);
    if (previous != category) {
        func_ov005_02069bcc(231, 0, 4, 128);
        if (panel->base.count <= panel->base.visible_rows) {
            panel->base.selected.signed_value = panel->category_first[category];
        } else {
            panel->base.first = panel->category_first[category];
            panel->base.selected.signed_value = 0;
            if (panel->base.first + panel->base.visible_rows >= panel->base.count) {
                int last_first = panel->base.count - panel->base.visible_rows;
                panel->base.selected.signed_value = panel->base.first - last_first;
                panel->base.first = last_first;
            }
            panel->base.ring_first = 0;
            func_ov009_02070764(panel);
        }
    }
}
void ShopInventoryPanel_Show(ShopInventoryPanel *panel)
{
    int row;
    if (panel->base.enabled)
        ShopInventoryPanel_Hide(panel, 0);
    panel->base.enabled = 1;
    ShopInventoryPanel_RebuildList(panel);
    for (row = 0; row < panel->base.visible_rows; row++) {
        if (panel->base.count < (u32)panel->base.visible_rows && panel->base.first + row >= panel->base.count)
            break;
        func_ov009_02073538(row);
    }
    func_ov009_02073228(0);
    func_ov009_02073228(1);
}
void ShopInventoryPanel_Hide(ShopInventoryPanel *panel, int adjust)
{
    panel->base.enabled = 0;
    if (adjust) {
        int first = panel->base.first;
        u16 index = (first + panel->base.selected.signed_value) % panel->base.count;
        if (first > index)
            --panel->base.first;
    }
    gSaveData[1376] = panel->base.first;
    gSaveData[1373] = panel->base.selected.signed_value;
    func_ov005_020663d8(2);
}
}
class ShopInventoryPanelInterface : public ShopRowPanelInterface
{
  public:
    virtual void unknown_0c();
    virtual u32 get_row_quantity(u16 row, int mode);
};
extern "C" {
extern ShopItemPanel *data_ov009_0207ea24;
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
    if (ShopInventoryPanel_CanSellRow((ShopInventoryPanel *)data_ov009_0207ea24, task->row))
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
