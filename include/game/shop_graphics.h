#ifndef PIT_GAME_SHOP_GRAPHICS_H
#define PIT_GAME_SHOP_GRAPHICS_H
#include <game/shop_rows.h>

#ifdef __cplusplus
extern "C" {
#endif
void ShopPartyDisplay_DrawLabel(ShopRowTask *task);
void ShopBitmap_DrawPlaceholder(int x, int y);
void ShopBitmap_DrawValue(int x, int y, u32 value);
void ShopNumber_DrawPlaceholder(int tile, u32 x);
const u8 *ShopGlyph_Expand4bpp(const u8 *source, u8 *destination, int count);
#ifdef __cplusplus
}
#endif
#endif
