#ifndef PIT_SHOP_TEXT_TASKS_INTERNAL_H
#define PIT_SHOP_TEXT_TASKS_INTERNAL_H
#include <game/shop_panels.h>
#include <game/shop_text_tasks.h>
extern ShopText *data_ov009_0207ea30;
void *func_ov005_0206659c(void (*callback)(void *), int group, int append);
void func_ov005_0206650c(void *task);
void func_ov009_0207459c(ShopText *renderer, DisplayEngine engine, int offset,
    int kind, u16 item, int columns, int rows, int row_stride, u8 color,
    u8 centered, u8 quantity);

#endif
