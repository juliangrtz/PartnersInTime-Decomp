#include "shop_navigation_internal.h"
extern "C" {
void ShopPanelNavigation_DrawCursor(ShopRowTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    if (ShopItemPanel_GetCount(data_ov009_0207ea20)) {
        s16 selection = ShopItemPanel_GetSelection(data_ov009_0207ea20);
        int x = ShopItemPanel_GetX(data_ov009_0207ea20) / 4096 - 2;
        int y = ShopItemPanel_GetY(data_ov009_0207ea20);
        model->animation_offset_x = x;
        model->animation_offset_y = (selection << 4) + y / 4096 + 6;
        func_ov005_02069084(model, 8);
    }
}
void ShopPanelNavigation_DrawArrow(ShopRowTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    if (ShopItemPanel_CanScroll(data_ov009_0207ea20)) {
        s16 offset;
        int y;
        if (task->part)
            offset = (s8)(16 * ShopItemPanel_GetVisibleRows(data_ov009_0207ea20) + 4);
        else
            offset = -4;
        y = ShopItemPanel_GetY(data_ov009_0207ea20);
        model->animation_offset_x = 160;
        model->animation_offset_y = offset + y / 4096;
        if (!task->part) {
            if (!ShopItemPanel_IsWindowAtEdge(data_ov009_0207ea20, 0))
                model->set_primary_animation(1, -1, 1);
            else {
                if (!ShopItemPanel_IsSelectionAtEdge(data_ov009_0207ea20, 0)) {
                    model->animation_offset_x = -128;
                    model->animation_offset_y = -128;
                }
                model->set_primary_animation(9, -1, 1);
            }
        } else {
            if (!ShopItemPanel_IsWindowAtEdge(data_ov009_0207ea20, 1))
                model->set_primary_animation(2, -1, 1);
            else {
                if (!ShopItemPanel_IsSelectionAtEdge(data_ov009_0207ea20, 1)) {
                    model->animation_offset_x = -128;
                    model->animation_offset_y = -128;
                }
                model->set_primary_animation(10, -1, 1);
            }
        }
        func_ov005_02069084(model, 8);
    }
}
void ShopPanelNavigation_Create(void)
{
    void *resource = func_ov005_020698dc(0);
    ShopRowTask *task = func_ov005_0206659c(ShopPanelNavigation_DrawCursor, 3, 1);
    BattleModel *model = Overlay5ResourceA_Attach(task, 0, 1);
    func_ov005_02068908(model, 0, resource, 1, -1);
    model->set_primary_animation(0, 0, 1);
    model->flag_bits.unknown_00_01 = 1;
    resource = func_ov005_020698dc(0);
    if (ShopItemPanel_CanScroll(data_ov009_0207ea20)) {
        task = func_ov005_0206659c(ShopPanelNavigation_DrawArrow, 3, 1);
        task->part = 0;
        model = Overlay5ResourceA_Attach(task, 0, 1);
        func_ov005_02068908(model, 0, resource, 1, -1);
        model->set_primary_animation(1, 0, 1);
        model->flag_bits.unknown_00_01 = 1;
        task = func_ov005_0206659c(ShopPanelNavigation_DrawArrow, 3, 1);
        task->part = 1;
        model = Overlay5ResourceA_Attach(task, 0, 1);
        func_ov005_02068908(model, 0, resource, 1, -1);
        model->flag_bits.unknown_00_01 = 1;
        model->set_primary_animation(2, 0, 1);
    }
}
void ShopPanelNavigation_Clear(void)
{
    func_ov005_020663d8(3);
    func_ov005_020663d8(5);
}
}
