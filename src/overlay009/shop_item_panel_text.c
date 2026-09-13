#include "shop_text_tasks_internal.h"

void ShopItemText_Update(ShopItemTextTask *task)
{
    int item = task->item;
    int offset = 32 * (36 * task->row + 76);
    u8 icon;
    func_ov009_0207459c(data_ov009_0207ea30, DISPLAY_ENGINE_MAIN, offset,
        0, (u16)item, 4, 1, 32, (u8)task->color, 0, (u8)task->quantity);
    icon = ShopText_GetItemIcon(data_ov009_0207ea30, (u16)item);
    ShopText_CopyIcon(data_ov009_0207ea30, DISPLAY_ENGINE_MAIN, offset + 1024, icon);
    func_ov005_0206650c(task);
}
ShopItemTextTask *ShopItemText_Queue(int row, int item, int color, int quantity)
{
    ShopItemTextTask *task = func_ov005_0206659c((void (*)(void *))ShopItemText_Update, 11, 1);
    task->row = row;
    task->item = item;
    task->color = color;
    task->quantity = quantity;
    return task;
}
