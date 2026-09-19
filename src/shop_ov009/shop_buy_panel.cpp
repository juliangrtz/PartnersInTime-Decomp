#include "shop_rows_internal.h"
#include "shop_price_internal.h"
#include <game/save_data.h>
#include <game/shop_scene.h>

extern "C" {
extern u8 data_ov009_0207ea3c[];
extern ShopSceneTask *data_ov009_0207ea38;
int func_ov009_0207ddac(u16);
u32 func_ov009_0206d0bc(ShopSceneTask *, u8);
void func_ov009_02071e24(ShopBuyPanel *, u8);
void func_ov009_0207d92c(ShopList *, int, u16, int, u8);
void func_ov009_02070cfc(int);
void func_ov009_02070978(int);
void func_ov005_020663d8(int);
}

extern "C" {
u8 ShopBuyPanel_GetCategory(ShopBuyPanel *panel)
{
    return panel->category;
}
u32 ShopBuyPanel_GetRowPrice(ShopBuyPanel *panel, u16 row)
{
    u32 value = 0;
    const ShopPriceRecord *record = ShopPrice_GetRecord(ShopItemPanel_GetRowItem(&panel->base, row));
    if (record) {
        int price = record->price;
        if (data_ov009_0207ea3c[0x98] == 2) {
            value = 5 * (100 * price / 500u);
        } else {
            value = price * (100 - func_ov009_0206d0bc(data_ov009_0207ea38, record->bonus_class)) / 100;
            if (!value && record->price)
                value = 1;
        }
    }
    return value;
}
u32 ShopBuyPanel_GetPriceDiscount(ShopBuyPanel *panel, u16 row)
{
    if (data_ov009_0207ea3c[0x98] == 2)
        return 0;
    const ShopPriceRecord *record = ShopPrice_GetRecord(ShopItemPanel_GetRowItem(&panel->base, row));
    if (record)
        return record->price - ((ShopRowPanelInterface *)panel)->get_row_value(row);
    return 0;
}
s8 ShopBuyPanel_GetRowQuantity(ShopBuyPanel *panel, u16 row, u32 mode)
{
    return ShopInventory_GetCount(ShopItemPanel_GetRowItem(&panel->base, row), mode);
}
int ShopBuyPanel_CanBuyRow(ShopBuyPanel *panel, u16 row)
{
    u32 price = ((ShopRowPanelInterface *)panel)->get_row_value(row);
    u16 item = ShopItemPanel_GetRowItem(&panel->base, row);
    u16 maximum = func_ov009_0207ddac(item);
    if (ShopInventory_GetCount(item, 2) >= maximum)
        return 0;
    if (data_ov009_0207ea3c[0x98] == 2) {
        if (*(u16 *)(gSaveData + 1164) < price)
            return 0;
    } else if (*(u32 *)(gSaveData + 1160) < price) {
        return 0;
    }
    return 1;
}
int ShopBuyPanel_IsRowNew(ShopBuyPanel *panel, int row)
{
    return panel->new_items[(panel->base.first + row) % panel->base.count];
}

void ShopBuyPanel_Show(ShopBuyPanel *panel, u8 selection)
{
    int row;
    if (panel->base.enabled)
        ShopBuyPanel_Hide(panel);
    panel->base.enabled = 1;
    func_ov009_02071e24(panel, selection);
    func_ov009_0207d92c(data_ov009_0207ea2c, panel->base.visible_rows, 20, 1, 0);
    for (row = 0; row < panel->base.visible_rows; ++row) {
        if (panel->base.count < (u32)panel->base.visible_rows &&
            panel->base.first + row >= panel->base.count)
            break;
        func_ov009_02070cfc(row);
    }
    /* The native byte arithmetic wraps category zero to 255. */
    if ((u8)(data_ov009_0207ea3c[0x98] + 255) <= 1)
        func_ov009_02070978(panel->category);
}
void ShopBuyPanel_Hide(ShopBuyPanel *panel)
{
    if (panel->base.enabled) {
        (gSaveData + panel->saved_selection)[1374] = panel->base.first;
        (gSaveData + panel->saved_selection)[1371] = panel->base.selected.signed_value;
    }
    panel->base.enabled = 0;
    func_ov005_020663d8(2);
}
}
