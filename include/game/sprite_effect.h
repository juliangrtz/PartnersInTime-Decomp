#ifndef PIT_GAME_SPRITE_EFFECT_H
#define PIT_GAME_SPRITE_EFFECT_H
#include <game/heap.h>

typedef struct GameSpriteEffect GameSpriteEffect;
struct GameSpriteEffect {
    GameSpriteEffect *previous, *next;
    u8 state[40];
};
typedef struct GameSpriteEffectList {
    GameSpriteEffect *head_marker, *first, *last, *tail_marker;
} GameSpriteEffectList;
typedef struct GameSpriteEffectPool {
    GameSpriteEffect *entries;
    union { u32 raw; struct { u16 low, high; } words; } count;
    GameSpriteEffectList free_list, active_list;
} GameSpriteEffectPool;
GameSpriteEffectPool *GameSpriteEffectPool_Destroy(GameSpriteEffectPool *pool);
GameSpriteEffectPool *GameSpriteEffectPool_Init(GameSpriteEffectPool *pool, u32 count);
#endif
