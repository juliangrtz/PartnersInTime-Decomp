#ifndef PIT_GAME_SPRITE_OUTPUT_H
#define PIT_GAME_SPRITE_OUTPUT_H
#include <nitro.h>

typedef struct GameOamEntry { u16 attributes[3]; u16 affine; } GameOamEntry;
typedef struct GameOamSortRecord {
    u32 key;
    union { u32 raw; struct { u8 first, count, reserved[2]; } bytes; } range;
} GameOamSortRecord;
typedef union GameSpriteFlags {
    u8 raw;
    struct { u8 screen : 2; u8 shared : 1; u8 dirty : 1; u8 buffered : 1; u8 linked : 1; u8 reserved : 2; } bits;
} GameSpriteFlags;
typedef struct GameSpritePalette GameSpritePalette;
struct GameSpritePalette {
    GameSpritePalette *previous, *next;
    const void *data;
    u32 resource;
    u8 first, bank, count;
    GameSpriteFlags flags;
};
typedef struct GameSpriteAllocation GameSpriteAllocation;
struct GameSpriteAllocation {
    u32 offset, size;
    GameSpriteAllocation *previous, *next;
    u32 unknown10;
    GameSpriteFlags flags;
    u8 end, reserved[2];
};

extern void *data_0205a054;
extern u8 data_0205a058[2], data_0205a05c[2], data_0205a060[2], data_0205a064[2];
extern GameOamEntry *data_0205a068;
extern GameSpriteAllocation *data_0205a06c[2], *data_0205a074[2];
extern GameSpritePalette *data_0205a07c[2], *data_0205a084[2];
extern u8 *data_0205a08c[2], *data_0205a094[2];
extern GameOamSortRecord data_0205a09c[2][128];

void GameOam_CopyState(void *destination);
void GameOam_CopyAffine(const GameOamEntry *source, GameOamEntry *destination);
void GameOam_BuildSorted(int screen, const GameOamEntry *source, GameOamEntry *destination);
void GameOam_SortGroups(int screen);
void GameOam_AddGroup(int screen, u32 key, int count, u8 affine_count);
void GameOam_SetReserved(int screen, int count, u8 affine_count, int group_count);
void GameSpritePalette_Upload(int screen, const u8 *extended, const u8 *standard);
void GameSpritePalette_UploadScreen(int screen);
void GameSpritePalette_MarkDirty(GameSpritePalette *palette, const void *source);
void *GameSpritePalette_GetBuffer(const GameSpritePalette *palette);
void GameSpritePalette_Unlink(GameSpritePalette *palette);
void GameSpriteAllocation_Unlink(GameSpriteAllocation *allocation);

/* These copies choose their direction from the relative buffer addresses. */
void func_0202cc58(const void *source, void *destination, u32 size);
void func_0202cd2c(const void *source, void *destination, u32 size);
static inline void GameSprite_CopyBytes(const void *source, void *destination, u32 size)
{
    if ((u32)source < (u32)destination) func_0202cd2c(source, destination, size);
    else func_0202cc58(source, destination, size);
}

#endif
