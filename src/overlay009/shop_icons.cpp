extern "C" {
#include <game/overlay005_display.h>
}
#include <game/shop_panels.h>
#include "shop_price_internal.h"
extern "C" u8 ShopText_GetItemIcon(ShopText *renderer, u16 item)
{
    u8 result = 0;
    const ShopPriceRecord *record = ShopPrice_GetRecord(item);
    if (record)
        result = record->unknown_00[9];
    return result;
}

extern "C" {
extern u8 data_ov009_0207ea3c[];
void MIi_CpuCopy16(const void *, void *, u32);

void ShopText_CopyIcon(ShopText *renderer, DisplayEngine engine, int offset, int icon)
{
    u8 *destination = (u8 *)Overlay5Display_GetObjVram(engine) + offset;
    MIi_CpuCopy16(*(u8 **)(data_ov009_0207ea3c + 0x5c) + (icon << 6), destination, 128);
}
}
