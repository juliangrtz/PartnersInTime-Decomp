#include "shop_panels_internal.h"

const u8 *ShopText_GetString(ShopText *renderer, int kind, u16 item, u8 quantity)
{
    const u8 *result = 0;
    switch (kind) {
    case 0:
        result = GameTextResources_GetEntry(data_ov009_0207e290[(u8)(((item & 0xf000) >> 12) - 1)],
                                            func_ov009_02074a74(renderer, item, quantity));
        break;
    case 1:
        result = GameTextResources_GetEntry(data_ov009_0207e294[(u8)(((item & 0xf000) >> 12) - 1)],
                                            func_ov009_02074950(renderer, item, 0, quantity));
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
