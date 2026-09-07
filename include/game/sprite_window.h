#ifndef PIT_GAME_SPRITE_WINDOW_H
#define PIT_GAME_SPRITE_WINDOW_H
#include <game/window.h>

/* A sprite window is an ordinary window whose pixels are presented through OAM
   objects instead of a background tilemap. Every slot owns one direct-mode
   sprite animation entry plus the object tiles it was allocated. */
#define GAME_SPRITE_WINDOW_OBJECTS 12

/* The hardware object view used while a window's objects are being built. */
typedef struct GameOamObject {
    union {
        u32 raw;
        struct { u32 reserved0 : 14, shape : 2, x : 9, reserved25 : 5, size : 2; } bits;
        struct { u8 y, reserved1[3]; } bytes;
    } attributes;
    union {
        u32 raw;
        struct { u32 tile : 10, reserved10 : 22; } bits;
    } tile;
} GameOamObject;

typedef struct GameSpriteWindowSlot {
    GameSpriteAnimation entry;
    u8 active, window, reserved1a[2];
    GameSpriteAllocation allocation;
} GameSpriteWindowSlot;

typedef struct GameSpriteWindowManager {
    GameWindowManager base;
    GameSpriteWindowSlot *slots;
    GameOamEntry *oam;
    union {
        u32 raw;
        struct { u32 main_palette : 4, sub_palette : 4, count : 8, reserved16 : 16; } bits;
        struct { u8 reserved0, count, reserved2[2]; } bytes;
    } state;
} GameSpriteWindowManager;

static inline GameSpriteWindowSlot *GameSpriteWindow_Slot(GameSpriteWindowManager *manager,
                                                          const GameWindow *window)
{
    return &manager->slots[window->sprite_slot];
}

typedef char GameOamObjectSizeCheck[sizeof(GameOamObject) == 8 ? 1 : -1];
typedef char GameWindowManagerSizeCheck[sizeof(GameWindowManager) == 3908 ? 1 : -1];
typedef char GameSpriteWindowSlotSizeCheck[sizeof(GameSpriteWindowSlot) == 52 ? 1 : -1];
typedef char GameSpriteWindowManagerSizeCheck[sizeof(GameSpriteWindowManager) == 3920 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void GameSpriteWindow_Close(GameSpriteWindowManager *manager, s16 index);
void GameSpriteWindow_Upload(GameSpriteWindowManager *manager, GameWindow *window);
void GameSpriteWindow_UploadDirty(GameSpriteWindowManager *manager);
GameOamObject *GameSpriteWindow_ApplyTileOffsets(GameSpriteWindowManager *manager, GameWindow *window);
u16 GameSpriteWindow_BuildObjects(GameSpriteWindowManager *manager, GameWindow *window);
s16 GameSpriteWindow_Open(GameSpriteWindowManager *manager, u32 size, s16 requested_index);
void GameSpriteWindow_ReleaseSlots(GameSpriteWindowManager *manager);
void GameSpriteWindow_AllocateSlots(GameSpriteWindowManager *manager, int heap, int count);
GameSpriteWindowManager *GameSpriteWindow_Delete(GameSpriteWindowManager *manager);
GameSpriteWindowManager *GameSpriteWindow_Destroy(GameSpriteWindowManager *manager);
#ifdef __cplusplus
}
#endif
#endif
