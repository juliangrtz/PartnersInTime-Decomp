#ifndef PIT_GAME_GRAPHICS_RESOURCE_H
#define PIT_GAME_GRAPHICS_RESOURCE_H
#include <nitro.h>

typedef struct GameGraphicsResource {
    union { u16 raw; struct { u16 reserved0 : 4, alternate_boundary : 3, normal_boundary : 3,
        texture_format : 3, reserved13 : 1, color256 : 1, reserved15 : 1; } bits; } flags;
    u16 extra_count;
    u8 reserved04[8];
    u16 animation_count, frame_count, group_count, object_count;
    u8 reserved14[4];
} GameGraphicsResource;

typedef struct GameGraphicsRange { u16 first, end; } GameGraphicsRange;
typedef struct GameGraphicsObject {
    u16 attributes0 : 14, shape : 2;
    u16 attributes1 : 14, size : 2;
    u16 image;
    u16 reserved[3];
} GameGraphicsObject;

typedef char GameGraphicsResourceSizeCheck[sizeof(GameGraphicsResource) == 24 ? 1 : -1];
typedef char GameGraphicsObjectSizeCheck[sizeof(GameGraphicsObject) == 12 ? 1 : -1];

const void *GameGraphics_GetSection(const GameGraphicsResource *resource, int section);
u8 GameGraphics_GetTextureFormat(const GameGraphicsResource *resource);
void GameGraphics_BuildTextureOffsets(u16 *output, const GameGraphicsResource *resource);
void GameGraphics_BuildGroupTextureOffsets(u16 *output, const GameGraphicsResource *resource, int group);
#endif
