#include "shop_panels_internal.h"

void ShopText_CopyIcon(ShopText *renderer, DisplayEngine engine, int offset, int icon)
{
    u8 *destination = (u8 *)Overlay5Display_GetObjVram(engine) + offset;
    MIi_CpuCopy16(*(u8 **)(data_ov009_0207ea3c + 0x5c) + (icon << 6), destination, 128);
}
