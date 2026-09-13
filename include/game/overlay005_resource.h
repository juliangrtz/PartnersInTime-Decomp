#ifndef PARTNERS_IN_TIME_GAME_OVERLAY005_RESOURCE_H
#define PARTNERS_IN_TIME_GAME_OVERLAY005_RESOURCE_H

#include <nitro.h>

typedef struct Overlay5ResourceOwner {
    u8 unknown_00[0x18];
    void (*release_callback)(struct Overlay5ResourceOwner *owner);
    void *resource;
    u8 unknown_20[8];
    u32 selector;
} Overlay5ResourceOwner;

/* Shared 64-byte sprite used by overlay-5 draw lists. */
typedef struct Overlay5ObjectSprite {
    union {
        u8 unknown_00[20];
        struct {
            u8 unknown_00_before_screen[16];
            u8 screen;
            u8 unknown_11[3];
        };
    };
    u32 attributes_0;
    union {
        u32 attributes_1;
        struct {
            u32 tile : 10, priority : 2, palette : 4, reserved : 16;
        } attributes_1_bits;
    };
    int x, y, target_x, target_y;
    s16 center_x, center_y;
    u8 unknown_30[8];
    s16 matrix[4];
} Overlay5ObjectSprite;
typedef char Overlay5ObjectSprite_SizeCheck[sizeof(Overlay5ObjectSprite) == 64 ? 1 : -1];

#endif
