#ifndef PIT_GAME_SPRITE_ANIMATION_H
#define PIT_GAME_SPRITE_ANIMATION_H
#include <nitro.h>
typedef struct GameSpriteAnimationTrack {
    struct GameSpriteAnimationTrack *next;
    const s16 *commands;
    s32 time;
    s16 speed;
    s16 x, y, z;
    union {
        u16 raw;
        struct { u16 duration : 14, loop : 1, paused : 1; } bits;
    } flags;
    u16 reserved16;
    s32 parameters[4];
    void (*finish)(struct GameSpriteAnimationTrack *track);
    struct GameSpriteAnimationTrack **owner;
} GameSpriteAnimationTrack;
typedef struct GameSpriteAnimation {
    s32 time;
    s32 fraction;
    u32 reserved08;
    s32 depth;
    const s16 *commands;
    GameSpriteAnimationTrack *storage;
    GameSpriteAnimationTrack *free_head;
    GameSpriteAnimationTrack *free_tail;
    GameSpriteAnimationTrack *active;
    void *buffer;
} GameSpriteAnimation;
typedef char GameSpriteAnimationTrack_SizeCheck[sizeof(GameSpriteAnimationTrack) == 48 ? 1 : -1];
typedef char GameSpriteAnimation_SizeCheck[sizeof(GameSpriteAnimation) == 40 ? 1 : -1];
GameSpriteAnimationTrack *GameSpriteAnimation_Allocate(GameSpriteAnimation *animation);
void GameSpriteAnimation_ReleaseOwner(GameSpriteAnimationTrack *track);
void GameSpriteAnimation_Scan(GameSpriteAnimation *animation, GameSpriteAnimationTrack *track);
void func_02016c0c(GameSpriteAnimation *, GameSpriteAnimationTrack *, const s16 *);
GameSpriteAnimation *GameSpriteAnimation_Initialize(GameSpriteAnimation *animation,
    void *storage, int tracks, int buffer_size);
void GameSpriteAnimation_Finalize(GameSpriteAnimation *animation);
GameSpriteAnimationTrack *GameSpriteAnimation_Start(GameSpriteAnimation *animation,
    const s16 *commands, int x, int y, int z, int speed);
void GameSpriteAnimation_Update(GameSpriteAnimation *animation);
u32 GameSpriteAnimation_PackColor(const s32 *rgb);
#endif
