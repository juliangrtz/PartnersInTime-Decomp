#include "shop_stock_rows_internal.h"

extern "C" void ShopStockPanel_Show(ShopStockPanel *panel, int selection)
{
    if (panel->enabled) ShopStockPanel_Hide(panel);
    panel->enabled = 1;
    func_ov009_02073098(panel, selection);
    for (int row = 0; row < 9; ++row) {
        if (panel->count < 9 && panel->first + row >= panel->count) break;
        func_ov009_020721d4(row);
    }
    func_ov009_02072080();
}

extern "C" void ShopStockPanel_Hide(ShopStockPanel *panel)
{
    void (*clear)(int) = func_ov005_020663d8;
    panel->enabled = 0;
    clear(1);
}

extern "C" void ShopStockRow_Update(ShopStockRowTask *task)
{
    int first = ShopStockPanel_GetRingFirst(STOCK_PANEL);
    int row = task->row;
    int slot = (first + row) % 9;
    task->x = task->offset_x + ShopStockPanel_GetX(STOCK_PANEL);
    task->y = task->offset_y + ShopStockPanel_GetY(STOCK_PANEL);
    task->y -= data_ov009_0207ea3c.panel_offset_y;
    task->tile = (36 * slot + 180) / 4;
    task->quantity = ShopStockPanel_GetRowQuantity(STOCK_PANEL, (u8)task->row);
    task->palette = 0;
    int mode = STOCK_PANEL->filter_mode;
    if (mode) {
        u16 selected = STOCK_PANEL->filter_item;
        u16 item = ShopStockPanel_GetRowItem(STOCK_PANEL, (u8)task->row);
        if (mode == 1) {
            if (selected != item) task->palette = 1;
        } else {
            const UsableItemRecord *selected_record = &gItemRecords[selected & ITEM_ID_INDEX_MASK];
            const UsableItemRecord *item_record = &gItemRecords[item & ITEM_ID_INDEX_MASK];
            if (selected_record->effect_flags << 1 >> 25 != item_record->effect_flags << 1 >> 25)
                task->palette = 1;
        }
    }
}

extern "C" void ShopStockRow_DrawTextSegment(ShopStockRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    int part = task->part;
    ShopStockRowTask *row = task->parent;
    sprite->bits.tile = row->tile + 8 * part / 4;
    sprite->bits.palette = (data_ov005_0206a180[0x85] + row->palette) & 15;
    sprite->x = row->x;
    sprite->y = row->y;
    func_ov005_02069084(sprite, (u8)row->priority);
}

extern "C" void ShopStockRow_DrawIcon(ShopStockRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    ShopStockRowTask *row = task->parent;
    sprite->bits.tile = row->tile + 8;
    sprite->x = row->x;
    sprite->y = row->y;
    func_ov005_02069084(sprite, (u8)row->priority);
}

extern "C" void ShopStockRow_DrawQuantityDigit(ShopStockRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    int place = task->part;
    ShopStockRowTask *row = task->parent;
    u8 digit = (row->quantity % (10 * place)) / place;
    if (digit || place == 1) {
        sprite->bits.tile = digit;
        sprite->bits.palette = (data_ov005_0206a180[0x85] + row->palette) & 15;
        sprite->x = row->x;
        sprite->y = row->y;
        func_ov005_02069084(sprite, (u8)row->priority);
    }
}
