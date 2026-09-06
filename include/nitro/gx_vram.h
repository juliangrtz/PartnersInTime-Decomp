#ifndef NITRO_GX_VRAM_H
#define NITRO_GX_VRAM_H

#include <nitro.h>

typedef struct GxVramState {
    u16 lcdc;
    u16 bg;
    u16 obj;
    u16 arm7;
    u16 texture;
    u16 texture_palette;
    u16 clear_image;
    u16 bg_ext_palette;
    u16 obj_ext_palette;
    u16 sub_bg;
    u16 sub_obj;
    u16 sub_bg_ext_palette;
    u16 sub_obj_ext_palette;
} GxVramState;

typedef char GxVramState_SizeCheck[sizeof(GxVramState) == 0x1A ? 1 : -1];

extern GxVramState data_02062c50;

#endif
