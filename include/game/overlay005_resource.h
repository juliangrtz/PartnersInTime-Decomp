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

#endif
