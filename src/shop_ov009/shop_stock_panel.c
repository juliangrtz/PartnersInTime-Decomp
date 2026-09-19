#include "shop_text_tasks_internal.h"

void ShopStockText_Update(ShopStockTextTask *task)
{
    int item = task->item;
    int offset = 32 * (36 * task->row + 180);
    u8 icon;
    func_ov009_0207459c(data_ov009_0207ea30, DISPLAY_ENGINE_SUB, offset,
        0, (u16)item, 4, 1, 32, 0, 0, (u8)task->quantity);
    icon = ShopText_GetItemIcon(data_ov009_0207ea30, (u16)item);
    ShopText_CopyIcon(data_ov009_0207ea30, DISPLAY_ENGINE_SUB, offset + 1024, icon);
    func_ov005_0206650c(task);
}
ShopStockTextTask *ShopStockText_Queue(int row, int item, int quantity)
{
    ShopStockTextTask *task = func_ov005_0206659c((void (*)(void *))ShopStockText_Update, 11, 1);
    task->row = row;
    task->item = item;
    task->quantity = quantity;
    return task;
}

u8 ShopStockPanel_GetCategory(ShopStockPanel *panel)
{
    return panel->category;
}

u8 ShopStockPanel_GetCount(ShopStockPanel *panel)
{
    return panel->count;
}

u16 ShopStockPanel_GetRowQuantity(ShopStockPanel *panel, int row)
{
    return panel->quantities[panel->items[(panel->first + row) % panel->count] & 0xfff];
}

u16 ShopStockPanel_GetRowItem(ShopStockPanel *panel, int row)
{
    return panel->items[(panel->first + row) % panel->count];
}

void ShopStockPanel_SetPosition(ShopStockPanel *panel, s32 x, s32 y)
{
    panel->x = x;
    panel->y = y;
}

s32 ShopStockPanel_GetX(ShopStockPanel *panel)
{
    return panel->x;
}

s32 ShopStockPanel_GetY(ShopStockPanel *panel)
{
    return panel->y;
}

int ShopStockPanel_GetRingFirst(ShopStockPanel *panel)
{
    return panel->ring_first;
}
