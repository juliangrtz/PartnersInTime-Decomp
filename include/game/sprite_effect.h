#ifndef PIT_GAME_SPRITE_EFFECT_H
#define PIT_GAME_SPRITE_EFFECT_H
#include <game/heap.h>

typedef struct GameSpriteEffectFrame {
    u8 kind, reserved;
    s16 scale_x, scale_y, offset_x, offset_y;
    u16 duration;
} GameSpriteEffectFrame;
typedef union GameSpriteEffectSize {
    u16 raw;
    struct { u8 width, height; } bytes;
} GameSpriteEffectSize;
typedef struct GameSpriteEffect GameSpriteEffect;
struct GameSpriteEffect {
    GameSpriteEffect *previous, *next;
    const GameSpriteEffectFrame *frame;
    void (*update)(GameSpriteEffect *effect);
    u32 unknown10;
    const u32 *image;
    void *destination;
    u8 x, y, animation, remaining;
    GameSpriteEffectSize drawn_size;
    u8 palette, image_index;
    GameSpriteEffectSize base_size;
    u16 unknown26;
    s16 origin_x, origin_y;
    union {
        u32 raw;
        struct { u16 id, flags; } words;
        struct { u32 id : 16, mode : 4, reserved20 : 2, paused : 1, stop_at_end : 1, reserved24 : 8; } bits;
    } state;
};
typedef struct GameSpriteEffectList {
    GameSpriteEffect *head_marker, *first, *last, *tail_marker;
} GameSpriteEffectList;
typedef struct GameSpriteEffectPool {
    GameSpriteEffect *entries;
    union { u32 raw; struct { u16 low, high; } words; } count;
    GameSpriteEffectList free_list, active_list;
} GameSpriteEffectPool;
typedef char GameSpriteEffectSizeCheck[(sizeof(GameSpriteEffect) == 48) ? 1 : -1];
typedef char GameSpriteEffectFrameSizeCheck[(sizeof(GameSpriteEffectFrame) == 12) ? 1 : -1];
typedef char GameSpriteEffectPoolSizeCheck[(sizeof(GameSpriteEffectPool) == 40) ? 1 : -1];

GameSpriteEffectPool *GameSpriteEffectPool_Destroy(GameSpriteEffectPool *pool);
GameSpriteEffectPool *GameSpriteEffectPool_Init(GameSpriteEffectPool *pool, u32 count);
void GameSpriteEffect_CopySize(GameSpriteEffectSize *destination, const GameSpriteEffectSize *source);
void GameSpriteEffectPool_Pause(GameSpriteEffectPool *pool, int id, int mode);
void GameSpriteEffectPool_Resume(GameSpriteEffectPool *pool, int id);
void GameSpriteEffectPool_Release(GameSpriteEffectPool *pool, int mode, int id);
u32 GameSpriteEffectPool_Activate(GameSpriteEffectPool *pool, GameSpriteEffect *effect);
void GameSpriteEffectPool_Update(GameSpriteEffectPool *pool);
void GameSpriteEffect_ScaleSize(s32 scale_x, s32 scale_y, GameSpriteEffectSize *size);
#endif
