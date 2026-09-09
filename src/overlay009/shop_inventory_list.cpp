#include <game/shop_panels.h>
#include <game/save_data.h>
extern "C" {
void MI_CpuFill8(void *, int, u32);
void func_ov009_020741b4(ShopInventoryPanel *, u8);
void func_ov009_02070764(ShopInventoryPanel *);
void ShopInventoryPanel_RebuildList(ShopInventoryPanel *panel)
{
    int category;
    MI_CpuFill8(panel->base.inline_items, 0, sizeof(panel->base.inline_items));
    panel->base.count = 0;
    for (category = 0; category < 4; category++)
        func_ov009_020741b4(panel, (u8)category);
    panel->base.items = panel->base.inline_items;
    panel->base.use_inventory = 1;
    panel->base.first = ((s8 *)gSaveData)[1376];
    panel->base.selected.signed_value = ((s8 *)gSaveData)[1373];
    if (panel->base.first + panel->base.visible_rows >= panel->base.count) {
        if (panel->base.visible_rows <= panel->base.count) {
            panel->base.first = panel->base.count - panel->base.visible_rows;
        } else {
            panel->base.first = 0;
            if (panel->base.selected.signed_value >= panel->base.count)
                panel->base.selected.signed_value = panel->base.count - 1;
        }
    }
    panel->base.ring_first = 0;
    func_ov009_02070764(panel);
    if (!ShopItemPanel_GetCount((ShopItemPanel *)panel))
        ShopInventoryPanel_ShowEmptyMessage(panel);
    panel->base.unknown_334 = 0;
    panel->base.unknown_338 = 0;
}
}
