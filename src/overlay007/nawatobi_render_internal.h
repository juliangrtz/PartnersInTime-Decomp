#ifndef PIT_NAWATOBI_RENDER_INTERNAL_H
#define PIT_NAWATOBI_RENDER_INTERNAL_H

#include <game/overlay005_resource.h>
#include "nawatobi_rope_internal.h"

/* Position prefix of a participant followed on both screens. */
typedef struct NawatobiFollowerSource {
    u8 unknown_00[48];
    s32 x, y, z;
} NawatobiFollowerSource;

typedef struct NawatobiFollowerTask {
    u8 unknown_00[16];
    NawatobiFollowerSource *source;
    u8 unknown_14[20];
    int screen;
    u8 unknown_2c[28];
} NawatobiFollowerTask;

/* One rope segment's sprite task and its copy on the other screen. */
typedef struct NawatobiRopeSpriteTask {
    u8 unknown_00[16];
    struct NawatobiRopeSpriteTask *source;
    u8 unknown_14[20];
    u8 unknown_28, point_index;
    s16 draw_list;
    s32 x, y, z, angle, scale;
    u8 unknown_40[8];
} NawatobiRopeSpriteTask;

typedef char NawatobiFollowerSourceSizeCheck[
    sizeof(NawatobiFollowerSource) == 60 ? 1 : -1];
typedef char NawatobiFollowerTaskSizeCheck[
    sizeof(NawatobiFollowerTask) == 72 ? 1 : -1];
typedef char NawatobiRopeSpriteTaskSizeCheck[
    sizeof(NawatobiRopeSpriteTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
extern NawatobiRope data_ov007_020a6bd0;
void NawatobiFollower_Create(NawatobiFollowerSource *source, int screen);
void NawatobiFollower_Update(NawatobiFollowerTask *task);
void NawatobiRope_UpdateAndDraw(void);
void NawatobiRope_UpdateSubSprite(NawatobiRopeSpriteTask *task);
void func_ov007_0208c33c(int shadow);
int func_ov007_0208c2f8(int y, int priority);
Overlay5ObjectSprite *Overlay5ResourceB_Get(void *owner);
void func_ov005_020692c0(Overlay5ObjectSprite *, int, int, int);
void func_ov005_02069084(void *, int);
#ifdef __cplusplus
}
#endif

#endif
