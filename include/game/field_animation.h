#ifndef PIT_GAME_FIELD_ANIMATION_H
#define PIT_GAME_FIELD_ANIMATION_H

#include <game/model_animation.h>
#include <game/sprite_animation.h>
#include <game/battle_scene.h>

typedef struct FieldModelAnimation {
    u8 matrix_storage[0x3C00];
    GameModelAnimationContext contexts[32];
    GameMatrixAnimation *matrix_animation;
    GameModelAnimation *model_animation;
    GameMatrixAnimationTrack *tracks[2];
} FieldModelAnimation;

typedef struct FieldSpriteAnimationSystem {
    u8 unknown_000[0x3AC];
    ModelRenderDescriptor *render_descriptor;
} FieldSpriteAnimationSystem;

typedef struct FieldSpriteAnimation {
    GameSpriteAnimationTrack track_storage[128];
    u8 buffer[4096];
    struct {
        u16 active : 1, owns_blend : 1, unknown_02_15 : 14;
    } flags;
    u16 unknown_2802;
    GameSpriteAnimation *animation;
    FieldSpriteAnimationSystem *field_system;
    GameSpriteAnimationTrack *tracks[2];
    MtxFx44 projection;
} FieldSpriteAnimation;

typedef char FieldModelAnimation_SizeCheck[sizeof(FieldModelAnimation) == 0x4110 ? 1 : -1];
typedef char FieldSpriteAnimationSystem_SizeCheck[sizeof(FieldSpriteAnimationSystem) == 0x3B0 ? 1 : -1];
typedef char FieldSpriteAnimation_SizeCheck[sizeof(FieldSpriteAnimation) == 0x2854 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void FieldModelAnimation_PrepareModel(FieldRenderObject *model, FieldRenderObject *parent,
                                      GameModelAnimationContext *context, MtxFx44 *matrix);
void FieldModelAnimation_Update(FieldModelAnimation *state);
FieldModelAnimation *FieldModelAnimation_Init(FieldModelAnimation *state);
FieldModelAnimation *FieldModelAnimation_Destroy(FieldModelAnimation *state);
void FieldModelAnimation_SetModels(FieldModelAnimation *state, FieldRenderObject **models,
                                   FieldRenderObject *first, FieldRenderObject *second,
                                   FieldRenderObject *third, FieldRenderObject *fourth);
GameMatrixAnimationTrack *FieldModelAnimation_Start(FieldModelAnimation *state, int index,
                                                    const s16 *commands, FieldRenderObject **models,
                                                    FieldRenderObject *first, FieldRenderObject *second,
                                                    FieldRenderObject *third, FieldRenderObject *fourth,
                                                    s16 x, s16 y, s16 z, int overlap_priority, int speed);
FieldSpriteAnimation *FieldSpriteAnimation_Init(FieldSpriteAnimation *state, void *field_system);
FieldSpriteAnimation *FieldSpriteAnimation_Destroy(FieldSpriteAnimation *state);
void FieldSpriteAnimation_Stop(FieldSpriteAnimation *state);
void FieldSpriteAnimation_CancelTrack(FieldSpriteAnimation *state, int index);
GameSpriteAnimationTrack *FieldSpriteAnimation_Start(FieldSpriteAnimation *state, int index,
                                                     const s16 *commands, int x, s16 y, s16 speed);
#ifdef __cplusplus
}
#endif

#endif
