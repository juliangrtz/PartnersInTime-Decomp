#ifndef PIT_GAME_SPRITE_ANIMATOR_H
#define PIT_GAME_SPRITE_ANIMATOR_H
#include <game/sprite_output.h>
#include <game/heap.h>

typedef struct GameSpriteAnimationFrame { u16 first; u16 duration : 9, flags : 7; u16 count, reserved; } GameSpriteAnimationFrame;
typedef struct GameSpriteAnimationRange { u16 first, end; } GameSpriteAnimationRange;
typedef struct GameSpriteAnimationFile {
    struct { u8 reserved0 : 4, tile_boundary : 2, reserved6 : 2; } flags0;
    struct { u8 reserved0 : 4, color256 : 1, reserved5 : 3; } flags1;
    u8 reserved[10];
    u16 animation_count, frame_count, group_count, object_count;
    GameSpriteAnimationRange animations[1];
} GameSpriteAnimationFile;
typedef struct GameSpriteAnimationObject {
    u16 attr0 : 14, shape : 2;
    u16 attr1 : 14, size : 2;
    u16 tile : 10, attr2 : 6;
    u16 mapped_tile : 10, attr3 : 6;
    u16 reserved[2];
} GameSpriteAnimationObject;

typedef struct GameSpriteAnimator GameSpriteAnimator;
typedef struct GameSpriteAnimation GameSpriteAnimation;
struct GameSpriteAnimation {
    GameSpriteAnimation *previous, *next;
    union {
        u32 raw;
        struct {
            u32 screen : 1, animated : 1;
            s32 x : 9, y : 9;
            u32 palette : 4, priority : 2, linked : 1, mode : 2, mosaic : 1, paused : 1, reserved : 1;
        } bits;
    } flags;
    u16 affine;
    u16 reserved;
    union {
        struct { u8 first, end, index, frame; s32 remaining; } animation;
        struct { const GameOamEntry *entries; u8 count, reserved[3]; } direct;
    } data;
};
typedef struct GameSpriteAnimationList {
    GameSpriteAnimation *head_marker, *first, *last, *tail_marker;
} GameSpriteAnimationList;
struct GameSpriteAnimator {
    void (**vtable)(GameSpriteAnimator *, GameSpriteAnimation *);
    GameSpriteAnimationList list;
    union {
        u32 raw;
        struct { u8 count[2]; u16 flags; } bytes;
        struct { u32 main_count : 8, sub_count : 8, main_allocated : 1, sub_allocated : 1, main_loaded : 1, sub_loaded : 1, reserved : 12; } bits;
    } state;
    GameSpriteAllocation allocation[2];
    GameOamEntry *oam[2];
    int reverse;
    GameSpriteAnimationFile *file;
    const void *tiles;
    u32 tile_size;
    const void *main_objects;
    void *sub_objects;
    GameSpriteAnimationFrame *frames;
};

GameSpriteAnimator *GameSpriteAnimator_Destroy(GameSpriteAnimator *animator);
GameSpriteAnimator *GameSpriteAnimator_Construct(GameSpriteAnimator *animator);
GameSpriteAnimator *GameSpriteAnimator_Init(GameSpriteAnimator *animator);
void GameSpriteAnimator_LoadMain(GameSpriteAnimator *animator);
void GameSpriteAnimator_LoadSub(GameSpriteAnimator *animator);
void GameSpriteAnimator_LoadBoth(GameSpriteAnimator *animator);
void GameSpriteAnimator_Add(GameSpriteAnimator *animator, int screen, int animation,
    u16 x, u16 y, u8 priority, u8 mode, u8 palette, u8 mosaic, GameSpriteAnimation *entry);
void GameSpriteAnimator_Update(GameSpriteAnimator *animator);
void GameSpriteAnimator_DrawDirect(GameSpriteAnimator *animator, GameSpriteAnimation *entry);
void GameSpriteAnimator_UploadTiles(GameSpriteAnimator *animator, int screen,
    GameSpriteAllocation *allocation, const void *tiles, u32 size,
    const GameSpriteAnimationFile *file, const GameSpriteAnimationObject *object);
void GameSpriteAnimator_RemapTiles(GameSpriteAnimator *animator, int screen);
#endif
