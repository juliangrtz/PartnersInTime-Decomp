#ifndef PIT_GAME_MATRIX_ANIMATION_H
#define PIT_GAME_MATRIX_ANIMATION_H
#include <nitro.h>
#include <nitro/fx_mtx.h>
typedef int (*GameAnimationEvaluator)(const s16 *command, int time);
typedef struct GameMatrixAnimationTrack {
    struct GameMatrixAnimationTrack *next;
    const s16 *commands;
    s32 current_time;
    s32 previous_time;
    s16 duration;
    s16 speed;
    u8 reserved14[6];
    union {
        u16 raw;
        struct { u16 loop : 1, paused : 1, separate_sequences : 1, retain : 1, finished : 1, reserved : 11; } bits;
    } flags;
    s32 parameters[3];
    void (*draw)(int object, MtxFx44 *matrix, struct GameMatrixAnimationTrack *track);
    void *context;
    void (*finish)(struct GameMatrixAnimationTrack *track);
    struct GameMatrixAnimationTrack **owner;
} GameMatrixAnimationTrack;
typedef struct GameMatrixAnimation {
    GameMatrixAnimationTrack *storage;
    GameMatrixAnimationTrack *free_head;
    GameMatrixAnimationTrack *free_tail;
    GameMatrixAnimationTrack *active;
    MtxFx44 *matrices;
    MtxFx44 *matrix;
    GameMatrixAnimationTrack *resource;
    GameAnimationEvaluator evaluate;
    s32 parameters[16];
    s32 time;
} GameMatrixAnimation;

int GameAnimation_EvaluateLinear(const s16 *, int);
int GameAnimation_EvaluateStep(const s16 *, int);
int GameAnimation_EvaluateSmooth(const s16 *, int);
int GameAnimation_EvaluateLoopSmooth(const s16 *, int);
int GameAnimation_EvaluateChanged(const s16 *, int);
void GameMatrixAnimation_Execute(GameMatrixAnimation *, const s16 *, int);
typedef char GameMatrixAnimationTrack_SizeCheck[sizeof(GameMatrixAnimationTrack) == 56 ? 1 : -1];
typedef char GameMatrixAnimation_SizeCheck[sizeof(GameMatrixAnimation) == 100 ? 1 : -1];
GameMatrixAnimationTrack *GameMatrixAnimation_Allocate(GameMatrixAnimation *);
void GameMatrixAnimation_ReleaseOwner(GameMatrixAnimationTrack *);
void GameMatrixAnimation_Update(GameMatrixAnimation *animation);
GameMatrixAnimationTrack *GameMatrixAnimation_Start(GameMatrixAnimation *animation,
    const s16 *commands,
    void (*draw)(int, MtxFx44 *, GameMatrixAnimationTrack *), int speed);
void GameMatrixAnimation_Finalize(void);
GameMatrixAnimation *GameMatrixAnimation_Initialize(GameMatrixAnimation *animation,
    void *storage, int tracks, int matrices);
#endif
