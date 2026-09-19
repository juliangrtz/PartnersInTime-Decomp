#ifndef PIT_GAME_FIELD_EFFECT_ANIMATION_H
#define PIT_GAME_FIELD_EFFECT_ANIMATION_H

/* Script-started field effects share room-owned command buffers. Model effects
 * bind up to four entity renderers; sprite effects use an explicit track slot. */
#include <game/field_animation.h>
#include <game/field_area.h>
#include <game/field_resources.h>
typedef char FieldAnimationBuffersOffset
    [(u32) & ((FieldResourceContext *)0)->animation_buffers == 0x22DC ? 1 : -1];
typedef char FieldAnimationSpecialOffset
    [(u32) & ((FieldResourceContext *)0)->special_animations == 0x22E4 ? 1 : -1];
typedef char
    FieldAnimationIdsOffset[(u32) & ((FieldResourceContext *)0)->animation_ids == 0x2330 ? 1 : -1];
typedef char FieldAnimationCountsOffset
    [(u32) & ((FieldResourceContext *)0)->animation_counts == 0x2358 ? 1 : -1];
typedef char FieldAnimationModelsOffset[(u32) & ((FieldAreaContext *)0)->animation_models == 0x2B18
                                            ? 1
                                            : -1];

#ifdef __cplusplus
extern "C"
{
#endif
    int FieldArea_IsSpriteAnimationActive(FieldAreaContext *area, int index);

    GameSpriteAnimationTrack *FieldArea_StartSpriteAnimation(FieldAreaContext *area, int slot,
                                                             int set, int index, s16 x, s16 y,
                                                             s16 speed);

    int FieldArea_IsModelAnimationActive(FieldAreaContext *area);

    GameMatrixAnimationTrack *FieldArea_StartModelAnimation(FieldAreaContext *area, int set,
                                                            int index, int first, int second,
                                                            int third, int fourth, s16 x, s16 y,
                                                            s16 z, int overlap_priority, s16 speed);

    int FieldResources_FinishAnimationReads(FieldResourceContext *area);

    void FieldResources_LoadAnimations(FieldResourceContext *area);
#ifdef __cplusplus
}
#endif
#endif
