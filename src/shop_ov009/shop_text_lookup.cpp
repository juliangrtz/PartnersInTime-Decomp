extern "C" {
#include <game/shop_panels.h>
#include <game/save_data.h>
}
#include "shop_price_internal.h"
extern "C" {
extern u8 data_ov009_0207e28c[], data_ov009_0207e290[], data_ov009_0207e294[];
extern u8 data_ov009_0207ea3c[];
u16 ShopText_GetNameIndex(ShopText *renderer, u16 item, int quantity)
{
    u32 result = 0;
    const ShopPriceRecord *record = ShopPrice_GetRecord(item);
    if (record)
        result = ((const u16 *)record)[1];
    if (quantity != 1)
        ++result;
    return result;
}
u16 ShopText_GetDescriptionIndex(ShopText *renderer, u16 item, int unused, int quantity)
{
    u32 result = 0;
    const ShopPriceRecord *record = ShopPrice_GetRecord(item);
    if (record) {
        if (data_ov009_0207e28c[(u16)(((item & ITEM_ID_TAG_MASK) >> 12) - 1)])
            result = ((const u16 *)record)[3];
        else
            result = ((const u16 *)record)[2];
    }
    return result;
}


const u8 *ShopText_GetString(ShopText *renderer, int kind, u16 item, u8 quantity)
{
    const u8 *result = 0;
    switch (kind) {
    case 0:
        result = (const u8 *)GameTextResources_GetEntry(data_ov009_0207e290[(u8)(((item & 0xf000) >> 12) - 1)],
                                            ShopText_GetNameIndex(renderer, item, quantity));
        break;
    case 1:
        result = (const u8 *)GameTextResources_GetEntry(data_ov009_0207e294[(u8)(((item & 0xf000) >> 12) - 1)],
                                            ShopText_GetDescriptionIndex(renderer, item, 0, quantity));
        break;
    case 2: {
        u8 *strings = *(u8 **)(data_ov009_0207ea3c + 0x58);
        strings += ((u32 *)strings)[gSaveData[1301]] & ~3;
        result = strings + ((u32 *)strings)[item];
        break;
    }
    }
    return result;
}

u16 ShopText_MeasureLine(ShopText *renderer, int kind, u16 item, u8 quantity)
{
    renderer->text.text = ShopText_GetString(renderer, kind, item, quantity);
    renderer->text.cursor.bits.x = 0;
    renderer->text.cursor.bits.y = 0;
    return GameText_MeasureLine(&renderer->text);
}
}
