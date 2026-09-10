#ifndef PIT_GAME_TITLE_SPRITE_SEQUENCE_H
#define PIT_GAME_TITLE_SPRITE_SEQUENCE_H
#include <game/title_effects.h>
#include <game/sprite_animation.h>

typedef struct TitleSpriteHeader {
    void *unknown_00;
    s32 elapsed, duration, x, y, depth;
    void (*update)(void *);
    void (*draw)(void *);
    u32 unknown_20;
    u8 state, unknown_25[3];
} TitleSpriteHeader;

typedef struct TitleSequenceActor {
    TitleSpriteHeader header;
    TitleTextureResource texture;
    const TitleSpriteLayout *layout;
    const s16 *commands[2];
    GameSpriteAnimationTrack *tracks[2];
    GameSpriteAnimation *animation;
    GameSpriteAnimationTrack track_storage[4];
    u8 animation_buffer[1024];
    s32 start_scale, current_scale;
    u16 angle;
    s8 alpha;
    u8 unknown_54f;
    s32 timing_scale;
} TitleSequenceActor;

typedef struct TitleSequenceSprite {
    TitleSpriteHeader header;
    TitleTextureResource *texture;
    const TitleSpriteLayout *layout;
    s32 acceleration, velocity, vertical_offset;
    s32 unknown_3c, unknown_40;
    s32 start_x, start_y, target_x, target_y;
    s32 movement_frames, hold_frames;
    s32 scale_x, scale_y;
    u32 final_depth;
    s8 alpha;
    u8 unknown_69[3];
} TitleSequenceSprite;

typedef struct TitleRotatingSprite {
    TitleSequenceSprite sprite;
    s32 angle;
} TitleRotatingSprite;

typedef struct TitleFadeSprite {
    TitleSpriteHeader header;
    TitleTextureResource *texture;
    const TitleSpriteLayout *layout;
    s8 alpha;
    u8 unknown_31[3];
} TitleFadeSprite;

typedef struct TitleSpriteSequence {
    TitleSpriteHeader header;
    TitleTextureResource textures[2];
    s32 elapsed, active_elapsed;
    TitleSequenceActor actor;
    TitleSequenceSprite sprites[2];
    TitleRotatingSprite rotating;
    TitleFadeSprite fade;
} TitleSpriteSequence;

typedef char TitleSpriteHeader_SizeCheck[sizeof(TitleSpriteHeader) == 40 ? 1 : -1];
typedef char TitleSequenceActor_SizeCheck[sizeof(TitleSequenceActor) == 1364 ? 1 : -1];
typedef char TitleSequenceSprite_SizeCheck[sizeof(TitleSequenceSprite) == 108 ? 1 : -1];
typedef char TitleRotatingSprite_SizeCheck[sizeof(TitleRotatingSprite) == 112 ? 1 : -1];
typedef char TitleFadeSprite_SizeCheck[sizeof(TitleFadeSprite) == 52 ? 1 : -1];
typedef char TitleSpriteSequence_SizeCheck[sizeof(TitleSpriteSequence) == 1928 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void TitleSequenceSprite_Update(TitleSequenceSprite *work);
void TitleRotatingSprite_Update(TitleRotatingSprite *rotating);
void TitleSequenceActor_Update(TitleSequenceActor *work);
void TitleSequenceActor_Init(TitleSequenceActor *work);
void TitleSequenceActor_Release(TitleSequenceActor *work);
void TitleSequenceActor_Start(TitleSequenceActor *work, int x, int y, int timing_scale);
void TitleSequenceActor_Stop(TitleSequenceActor *work);
int TitleSequenceActor_IsHolding(TitleSequenceActor *work);
void TitleSequenceSprite_StartEntry(TitleSequenceSprite *work);
void TitleSequenceSprite_QueueEntry(TitleSequenceSprite *work, int delay, int movement_frames,
                                    int hold_frames, int start_x, int start_y, int target_x, int target_y,
                                    int unknown_3c, int unknown_40);
void TitleSequenceSprite_StartMove(TitleSequenceSprite *work, int start_x, int start_y, int target_x,
                                   int target_y, int duration);
int TitleSequenceSprite_StartSquash(TitleSequenceSprite *work, int duration);
void TitleSequenceActor_Draw(TitleSequenceActor *work);
void TitleSequenceSprite_Draw(TitleSequenceSprite *work);
void TitleRotatingSprite_Draw(TitleRotatingSprite *work);
void TitleFadeSprite_Draw(TitleFadeSprite *work);
void TitleRotatingSprite_Init(TitleRotatingSprite *work, TitleTextureResource *texture);
void TitleFadeSprite_Update(TitleFadeSprite *work);
void TitleFadeSprite_Init(TitleFadeSprite *work, TitleTextureResource *texture);
void TitleFadeSprite_StartFadeIn(TitleFadeSprite *work);
void TitleFadeSprite_Show(TitleFadeSprite *work);
void TitleSpriteSequence_Update(TitleSpriteSequence *work);
void TitleSpriteSequence_Draw(TitleSpriteSequence *work);
void TitleSpriteSequence_Init(TitleSpriteSequence *work);
void TitleSpriteSequence_Release(TitleSpriteSequence *work);
void TitleSpriteSequence_Start(TitleSpriteSequence *work);
void TitleSpriteSequence_Finish(TitleSpriteSequence *work);
void TitleSequenceSprite_Init(TitleSequenceSprite *work, TitleTextureResource *texture, int index, int depth,
                              int final_depth);
int TitleSequenceSprite_IsIdle(TitleSequenceSprite *work);
int TitleSequenceSprite_IsSquashing(TitleSequenceSprite *work);
void TitleRotatingSprite_StartArc(TitleRotatingSprite *work, int duration, int hold_frames, int start_x,
                                  int start_y, int target_x, int target_y, int unknown_3c, int unknown_40,
                                  int state);
void TitleRotatingSprite_StartVerticalMove(TitleRotatingSprite *work, int distance);
void TitleRotatingSprite_StartSquash(TitleRotatingSprite *work, int duration);
int TitleRotatingSprite_IsIdle(TitleRotatingSprite *work);
#ifdef __cplusplus
}
#endif
#endif
