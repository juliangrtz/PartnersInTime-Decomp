#ifndef PIT_GAME_TEXTURE_ALLOCATION_H
#define PIT_GAME_TEXTURE_ALLOCATION_H
#include <game/sprite_output.h>

typedef struct GameTextureAllocation GameTextureAllocation;
struct GameTextureAllocation {
    u32 offset, size;
    GameTextureAllocation *previous, *next;
    u32 resource;
    union {
        u32 state;
        struct { union { u8 raw; } flags; u8 end, reserved[2]; };
    };
};

typedef struct GameTexturePalette GameTexturePalette;
struct GameTexturePalette {
    u32 offset;
    GameTexturePalette *previous, *next;
    u32 resource;
    const void *data;
    u16 size;
    union { u16 state; struct { u8 flags, kind; }; };
};

typedef char GameTextureAllocationSizeCheck[sizeof(GameTextureAllocation) == 24 ? 1 : -1];
typedef char GameTexturePaletteSizeCheck[sizeof(GameTexturePalette) == 24 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

extern GameTextureAllocation *data_0205a8c0, *data_0205a8b4;
extern GameTexturePalette *data_0205a8b0, *data_0205a8bc;
extern u8 *data_0205a8c4;
extern u32 data_0205a8b8;
extern u32 func_020354f4(void), func_020354dc(void);

int GameTextureAllocation_Allocate(GameTextureAllocation *allocation, int mode, u32 size,
                                   int shared, u32 resource, int unused, u8 dirty, u32 offset);
void GameTextureAllocation_Unlink(GameTextureAllocation *allocation);
int GameTexturePalette_Allocate(GameTexturePalette *palette, int mode, u8 kind, u16 colors,
                                u8 shared, u8 dirty, u8 slot, const void *data, u32 resource, u32 offset);
void GameTexturePalette_Unlink(GameTexturePalette *palette);
void GameTexturePalette_MarkDirty(GameTexturePalette *palette, const void *source);
void *GameTexturePalette_GetBuffer(const GameTexturePalette *palette);
void GameTexturePalette_Upload(const u32 *banks);
void GameTexturePalette_SetBuffer(void *buffer, u32 size);
void GameTexturePalette_ReleaseBuffer(int release);

#ifdef __cplusplus
}
#endif

#endif
