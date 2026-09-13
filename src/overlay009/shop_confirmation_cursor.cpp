#include "shop_navigation_internal.h"

extern "C" {
void ShopConfirmationCursor_Draw(ShopRowTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    int y = task->cursor_y;
    /* The category byte also holds the Yes/No selection during confirmation. */
    model->animation_offset_x = task->cursor_x + 80 * ((s8 *)data_ov009_0207ea3c)[0x9f];
    model->animation_offset_y = y;
    func_ov005_02069084(model, 10);
}

void ShopConfirmationCursor_Create(void)
{
    void *resource = func_ov005_020698dc(0);
    ShopRowTask *task = func_ov005_0206659c(ShopConfirmationCursor_Draw, 3, 1);
    task->cursor_x = 86;
    task->cursor_y = 112;
    BattleModel *model = Overlay5ResourceA_Attach(task, 0, 1);
    func_ov005_02068908(model, 0, resource, 1, -1);
    model->set_primary_animation(0, 0, 1);
    model->flag_bits.unknown_00_01 = 1;
}

void ShopConfirmationCursor_Clear(void)
{
    /* Preserve the original indirect tail call to the task-group helper. */
    void (*clear)(int) = func_ov005_020663d8;
    clear(3);
}
}
