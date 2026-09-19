#include "shop_scene_internal.h"
extern "C" {
#include <game/shop_graphics.h>

extern const u16 data_ov009_0207e2c0[4][2], data_ov009_0207e2d0[4][2];
void MIi_CpuClear16(u32 value, void *destination, u32 size);
void func_ov009_0207c558(int x, int y, int width, int height);
void func_ov009_0207c100(u8 *destination, const u8 *source, int x, int y,
    int columns, int rows, u32 destination_stride, int source_columns, u8 palette);
void func_ov009_0207c2ac(int x, int y, const void *source, int columns, int rows, int palette);

#define WORK data_ov009_0207ea3c
#define TEXT_SCRATCH ((u8 *)WORK.owned_84[0])
#define BITMAP_PIXELS ((u8 *)WORK.owned_58[7])
#define PARTY_ICONS ((u8 *)WORK.owned_58[8])

void ShopPartyBitmap_DrawItemName(int member, u16 item)
{
    GameTextToken token;
    /* The native halfword clear reads its value from a stack slot. */
    volatile u16 clear;
    GameText text;
    int x, y;

    GameText_Init(&text, (const u32 *const *)WORK.font_slots, TEXT_SCRATCH,
        0, 0, 0, 1, 3, 3, 6, 12, 0, 255, 0);
    clear = 0;
    MIi_CpuClear16(clear, TEXT_SCRATCH, 832);
    text.text = ShopText_GetString((ShopText *)data_ov009_0207ea30, 0, item, 1);
    text.cursor.bits.x = 0;
    text.cursor.bits.y = 0;
    for (;;) {
        if (!*text.text || (*text.text == 255 && !text.text[1]))
            break;
        token = GameText_Next(&text, 0, 0);
    }
    x = data_ov009_0207e2c0[member][0];
    y = data_ov009_0207e2c0[member][1];
    func_ov009_0207c558(x - 52, y, 104, 12);
    func_ov009_0207c100(BITMAP_PIXELS, TEXT_SCRATCH, x - text.cursor.bits.x / 2,
        y, 13, 2, 256, 13, 16);
    func_ov009_0207c2ac(data_ov009_0207e2d0[member][0],
        data_ov009_0207e2d0[member][1], PARTY_ICONS + 384 * member, 2, 3, 80);
    WORK.background_dirty = 1;
}
}
