/*
 * Buying panel (overlay 9, 0x020710B8-0x02071AF0).
 *
 * The list of items the shop sells: the price of a row after discounts, whether
 * the player can afford it, whether it is new, and the sprites and models each
 * row is drawn from. Show and Hide run the panel's open and close animations.
 */

#include "shop_rows_internal.h"
#include "shop_price_internal.h"
#include <game/save_data.h>
#include <game/shop_scene.h>

extern "C" {
extern u8 data_ov009_0207ea3c[];
extern ShopSceneTask *data_ov009_0207ea38;
int func_ov009_0207ddac(u16);
u32 func_ov009_0206d0bc(ShopSceneTask *, u8);
void func_ov009_02071e24(ShopBuyPanel *, u8);
void func_ov009_0207d92c(ShopList *, int, u16, int, u8);
void func_ov009_02070cfc(int);
void func_ov009_02070978(int);
void func_ov005_020663d8(int);
}

extern "C" {
u8 ShopBuyPanel_GetCategory(ShopBuyPanel *panel)
{
    return panel->category;
}
u32 ShopBuyPanel_GetRowPrice(ShopBuyPanel *panel, u16 row)
{
    u32 value = 0;
    const ShopPriceRecord *record = ShopPrice_GetRecord(ShopItemPanel_GetRowItem(&panel->base, row));
    if (record) {
        int price = record->price;
        if (data_ov009_0207ea3c[0x98] == 2) {
            value = 5 * (100 * price / 500u);
        } else {
            value = price * (100 - func_ov009_0206d0bc(data_ov009_0207ea38, record->bonus_class)) / 100;
            if (!value && record->price)
                value = 1;
        }
    }
    return value;
}
u32 ShopBuyPanel_GetPriceDiscount(ShopBuyPanel *panel, u16 row)
{
    if (data_ov009_0207ea3c[0x98] == 2)
        return 0;
    const ShopPriceRecord *record = ShopPrice_GetRecord(ShopItemPanel_GetRowItem(&panel->base, row));
    if (record)
        return record->price - ((ShopRowPanelInterface *)panel)->get_row_value(row);
    return 0;
}
s8 ShopBuyPanel_GetRowQuantity(ShopBuyPanel *panel, u16 row, u32 mode)
{
    return ShopInventory_GetCount(ShopItemPanel_GetRowItem(&panel->base, row), mode);
}
int ShopBuyPanel_CanBuyRow(ShopBuyPanel *panel, u16 row)
{
    u32 price = ((ShopRowPanelInterface *)panel)->get_row_value(row);
    u16 item = ShopItemPanel_GetRowItem(&panel->base, row);
    u16 maximum = func_ov009_0207ddac(item);
    if (ShopInventory_GetCount(item, 2) >= maximum)
        return 0;
    if (data_ov009_0207ea3c[0x98] == 2) {
        if (*(u16 *)(gSaveData + 1164) < price)
            return 0;
    } else if (*(u32 *)(gSaveData + 1160) < price) {
        return 0;
    }
    return 1;
}
int ShopBuyPanel_IsRowNew(ShopBuyPanel *panel, int row)
{
    return panel->new_items[(panel->base.first + row) % panel->base.count];
}

void ShopBuyPanel_Show(ShopBuyPanel *panel, u8 selection)
{
    int row;
    if (panel->base.enabled)
        ShopBuyPanel_Hide(panel);
    panel->base.enabled = 1;
    func_ov009_02071e24(panel, selection);
    func_ov009_0207d92c(data_ov009_0207ea2c, panel->base.visible_rows, 20, 1, 0);
    for (row = 0; row < panel->base.visible_rows; ++row) {
        if (panel->base.count < (u32)panel->base.visible_rows &&
            panel->base.first + row >= panel->base.count)
            break;
        func_ov009_02070cfc(row);
    }
    /* The native byte arithmetic wraps category zero to 255. */
    if ((u8)(data_ov009_0207ea3c[0x98] + 255) <= 1)
        func_ov009_02070978(panel->category);
}
void ShopBuyPanel_Hide(ShopBuyPanel *panel)
{
    if (panel->base.enabled) {
        (gSaveData + panel->saved_selection)[1374] = panel->base.first;
        (gSaveData + panel->saved_selection)[1371] = panel->base.selected.signed_value;
    }
    panel->base.enabled = 0;
    func_ov005_020663d8(2);
}
}

extern "C" {
static inline void SetModelPosition(BattleModel *model, int x, int y)
{
    model->animation_offset_x = x;
    model->animation_offset_y = y;
}

void ShopRow_Update(ShopRowTask *task)
{
    int first = ShopItemPanel_GetRingFirst(data_ov009_0207ea34);
    int row = task->row;
    int slot = (first + row) % ShopItemPanel_GetVisibleRows(data_ov009_0207ea34);
    int old_value;
    task->x = task->offset_x + ShopList_GetRowX(data_ov009_0207ea2c, (u8)row);
    task->y = task->offset_y + ShopList_GetRowY(data_ov009_0207ea2c, (u8)task->row);
    task->priority = ShopList_GetRowPriority(data_ov009_0207ea2c, (u8)task->row);
    task->scale = ShopList_GetScale(data_ov009_0207ea2c);
    task->tile = (36 * slot + 76) / 4;
    old_value = task->value;
    task->value = ((ShopRowPanelInterface *)data_ov009_0207ea34)->get_row_value(task->row);
    if (old_value != task->value)
        func_ov009_0207c8f0(16 * task->row + 520, 30, task->value, 4);
    if (ShopBuyPanel_CanBuyRow((ShopBuyPanel *)data_ov009_0207ea34, task->row))
        task->palette = 0;
    else
        task->palette = 1;
}
void ShopRow_DrawTextSegment(ShopRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    int part = task->part;
    ShopRowTask *row = task->parent;
    int y, top, bottom;
    sprite->bits.tile = row->tile + 8 * part / 4;
    sprite->bits.palette = (data_ov005_0206a180[0x71] + row->palette) & 15;
    sprite->x = row->x;
    sprite->y = row->y;
    if (row->scale > 409) {
        func_ov005_020692c0(sprite, 256, row->scale >> 4, 0);
        y = (sprite->y + sprite->offset_y) / 4096 - sprite->origin_y;
        ShopItemPanel_GetVector334(data_ov009_0207ea34, &top, &bottom);
        if (top >= y || bottom <= y + 12)
            func_ov005_02069084(sprite, (u8)row->priority);
    }
}
void ShopRow_DrawInsetSprite(ShopRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    ShopRowTask *row = task->parent;
    int y, top, bottom;
    sprite->bits.tile = row->tile + 8;
    sprite->x = row->x;
    sprite->y = row->y;
    if (row->scale > 409) {
        func_ov005_020692c0(sprite, 256, row->scale >> 4, 0);
        y = (sprite->y + sprite->offset_y) / 4096 - sprite->origin_y;
        ShopItemPanel_GetVector334(data_ov009_0207ea34, &top, &bottom);
        if (top >= y || bottom <= y + 8)
            func_ov005_02069084(sprite, (u8)row->priority);
    }
}
void ShopRow_DrawValueSprite(ShopRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    ShopRowTask *row = task->parent;
    sprite->x = row->x;
    sprite->y = row->y;
    if (row->scale > 409) {
        func_ov005_020692c0(sprite, 256, row->scale >> 4, 0);
        func_ov005_02069084(sprite, (u8)row->priority);
    }
}
void ShopRow_DrawAnchorModel(ShopRowTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    int x = ShopList_GetPointX(data_ov009_0207ea2c, 3) / 4096 - 48;
    int y = ShopList_GetPointY(data_ov009_0207ea2c, 3) / 4096 + 8;
    int scale = ShopList_GetScale(data_ov009_0207ea2c);
    if (scale > 409) {
        model->scale_x = 256;
        model->scale_y = scale / 16;
        model->animation_offset_x = x;
        model->animation_offset_y = y;
        func_ov005_02069084(model, 5);
    }
}
void ShopRow_DrawRowModel(ShopRowTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    ShopRowTask *row = task->parent;
    if (ShopBuyPanel_IsRowNew((ShopBuyPanel *)data_ov009_0207ea34, row->row)) {
        u32 scale = row->scale;
        if (scale > 409) {
            model->scale_x = 256;
            model->scale_y = scale >> 4;
            SetModelPosition(model, row->x / 4096, row->y / 4096);
            func_ov005_02069084(model, (u8)row->priority);
        }
    }
}
}
