#ifndef PIT_SHOP_GRAPHICS_INTERNAL_H
#define PIT_SHOP_GRAPHICS_INTERNAL_H
#include <game/shop_graphics.h>

#ifdef __cplusplus
extern "C" {
#endif
extern u8 data_ov009_0207ea3c[];
void func_ov009_0207c558(int x, int y, int width, int height);
void func_ov009_0207c464(int x, int y, const u8 *source, int width,
                       int height, int palette_offset, int transparent);
const u8 *func_ov009_0207caec(const u8 *source, u8 *destination,
                            int width, int height, int stride);
const u8 *func_ov009_0207cbf0(const u8 *source, u8 *destination,
                            int width, int height, int stride);
#ifdef __cplusplus
}
#endif

/* Resource pointers in the shared ShopSceneWork allocation. */
#define SHOP_PACKED_GLYPHS (*(u8 **)(data_ov009_0207ea3c + 0x64))
#define SHOP_BITMAP_GLYPHS (*(const u8 **)(data_ov009_0207ea3c + 0x68))
#define SHOP_NUMBER_SCRATCH (*(u8 **)(data_ov009_0207ea3c + 0x84))
#endif
