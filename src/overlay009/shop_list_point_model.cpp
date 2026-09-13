#include "shop_list_effects_internal.h"

extern "C" void ShopListPoint_DrawModel(ShopListPointTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get((ShopRowTask *)task);
    int x = ShopList_GetPointX(data_ov009_0207ea2c, (u8)task->point) / 4096;
    int y = ShopList_GetPointY(data_ov009_0207ea2c, (u8)task->point) / 4096;
    int offset = 0;
    if (task->point == data_ov009_0207ea2c->point_count - 1 &&
        (data_ov009_0207ea2c->entering || ShopList_GetPhase(data_ov009_0207ea2c)))
        offset -= 2;
    model->animation_offset_x = x;
    model->animation_offset_y = y;
    func_ov005_02069084(model, (u8)(task->priority + offset));
}
