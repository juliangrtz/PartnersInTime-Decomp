#include <game/shop_panels.h>
extern "C" {
#include <game/heap.h>
extern u8 data_ov009_0207e224[];

ShopBuyPanel *ShopBuyPanel_Init(ShopBuyPanel *panel)
{
    ShopItemPanel_Init(&panel->base);
    panel->base.vtable = data_ov009_0207e224;
    for (int i = 0; i < 2; ++i) {
        panel->unknown_3a1[i] = 0;
        panel->unknown_3a3[i] = 0;
    }
    panel->saved_selection = 0;
    panel->base.visible_rows = 7;
    return panel;
}
ShopBuyPanel *ShopBuyPanel_Destroy(ShopBuyPanel *panel)
{
    panel->base.vtable = data_ov009_0207e224;
    ShopItemPanel_DestroyBase(&panel->base);
    return panel;
}
ShopBuyPanel *ShopBuyPanel_Delete(ShopBuyPanel *panel)
{
    panel->base.vtable = data_ov009_0207e224;
    ShopItemPanel_DestroyBase(&panel->base);
    GameHeap_Delete(panel);
    return panel;
}
}
