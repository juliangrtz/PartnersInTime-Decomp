/*
 * Item panel lifecycle (overlay 9, 0x020708F0-0x02070978).
 *
 * Init, Destroy and Delete for the shared item panel the buying and selling
 * lists are built on.
 */

#include <game/shop_panels.h>
extern "C" {
#include <game/heap.h>
extern u8 data_ov009_0207e1dc[];

ShopItemPanel *ShopItemPanel_Init(ShopItemPanel *panel)
{
    panel->vtable = data_ov009_0207e1dc;
    panel->enabled = 0;
    panel->first = 0;
    panel->selected.signed_value = 0;
    panel->unknown_07 = 4;
    ShopItemPanel_SetPosition(panel, 393216, 81920);
    return panel;
}
ShopItemPanel *ShopItemPanel_Destroy(ShopItemPanel *panel)
{
    panel->vtable = data_ov009_0207e1dc;
    return panel;
}
ShopItemPanel *ShopItemPanel_Delete(ShopItemPanel *panel)
{
    panel->vtable = data_ov009_0207e1dc;
    GameHeap_Delete(panel);
    return panel;
}
ShopItemPanel *ShopItemPanel_DestroyBase(ShopItemPanel *panel)
{
    panel->vtable = data_ov009_0207e1dc;
    return panel;
}
}
