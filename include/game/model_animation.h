#ifndef PIT_GAME_MODEL_ANIMATION_H
#define PIT_GAME_MODEL_ANIMATION_H

#include <game/matrix_animation.h>
#include <game/battle_scene.h>

typedef struct GameModelAnimationContext {
    struct GameModelAnimationContext *next;
    union {
        u32 unknown_04[5];
        struct {
            s32 offset_x, offset_y, offset_z;
            /* -1 inherits model bytes 0x134..0x137; their role is unresolved. */
            s32 property_134;
            struct { u32 property_134 : 2, unknown_02_31 : 30; } flags;
        };
    };
    BattleModel *models[4];
} GameModelAnimationContext;

typedef struct GameModelAnimation {
    GameMatrixAnimation *animation;
    GameModelAnimationContext *storage;
    GameModelAnimationContext *free_head;
    GameModelAnimationContext *free_tail;
    GameModelAnimationContext *active;
    void (*prepare)(BattleModel *, BattleModel *, GameModelAnimationContext *, MtxFx44 *);
} GameModelAnimation;

typedef char GameModelAnimationContext_SizeCheck[sizeof(GameModelAnimationContext) == 40 ? 1 : -1];
typedef char GameModelAnimation_SizeCheck[sizeof(GameModelAnimation) == 24 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
GameModelAnimationContext *GameModelAnimation_AllocateContext(GameModelAnimation *pool);
GameModelAnimation *GameModelAnimation_Initialize(GameModelAnimation *pool,
    GameMatrixAnimation *animation, GameModelAnimationContext *storage, int count,
    void (*prepare)(BattleModel *, BattleModel *, GameModelAnimationContext *, MtxFx44 *));
GameModelAnimation *GameModelAnimation_Finalize(GameModelAnimation *pool);
GameMatrixAnimationTrack *GameModelAnimation_Start(GameModelAnimation *pool,
    const s16 *commands, BattleModel *const *models, int count,
    void (*draw)(int, MtxFx44 *, GameMatrixAnimationTrack *), int speed);
void GameModelAnimation_ReleaseContext(GameMatrixAnimationTrack *track);
#ifdef __cplusplus
}
#endif

#endif
