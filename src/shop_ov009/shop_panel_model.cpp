/*
 * Shop panel model (overlay 9, 0x02075AB0-0x02075B84).
 *
 * Draws the model a shop panel is framed by.
 */

#include "shop_rows_internal.h"
extern "C" {
extern u8 data_ov009_0207ea3c[];
extern u8 data_ov009_0207f23c[];
}
struct PanelModelParent {
    u8 prefix[44];
    s32 x, y;
    s16 offset_x, offset_y;
    u8 tail[16];
};
struct PanelModelTask {
    u8 prefix[16];
    PanelModelParent *parent;
    u8 unknown[20];
    int screen;
    u8 tail[28];
};
typedef char ParentSize[sizeof(PanelModelParent) == 72 ? 1 : -1];
typedef char ModelTaskSize[sizeof(PanelModelTask) == 72 ? 1 : -1];
extern "C" void ShopPanel_DrawModel(PanelModelTask *task) {
    PanelModelParent *parent = task->parent;
    BattleModel *model = Overlay5ResourceA_Get((ShopRowTask *)task);
    int x = (parent->x + 16 * parent->offset_x) / 4096;
    int y = (parent->y + 16 * parent->offset_y) / 4096;
    int sub = task->screen;
    if (sub)
        y += 240;
    s16 scale = *(u16 *)(data_ov009_0207f23c + 0xb8) >> 4;
    model->scale_x = scale;
    model->scale_y = scale;
    model->animation_offset_x = x;
    model->animation_offset_y = y;
    if (data_ov009_0207ea3c[0x8b6])
        model->flag_bits.unknown_03 = 1;
    else
        model->flag_bits.unknown_03 = 0;
    if (!data_ov009_0207ea3c[0x8b7])
        func_ov005_02069084(model, 8);
    else
        func_ov005_02069084(model, 60);
}
