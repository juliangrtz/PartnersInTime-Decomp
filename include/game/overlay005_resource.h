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
typedef struct Overlay5Sprite {
    u8 unknown_00[20];
    u32 attributes_0, attributes_1;
    int x, y, target_x, target_y;
    s16 center_x, center_y;
    u8 unknown_30[8];
    s16 matrix[4];
} Overlay5Sprite;
typedef char Overlay5Sprite_SizeCheck[sizeof(Overlay5Sprite) == 64 ? 1 : -1];

#endif
