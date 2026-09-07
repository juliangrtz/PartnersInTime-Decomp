#ifndef PIT_GAME_FIELD_TIMED_RENDERER_H
#define PIT_GAME_FIELD_TIMED_RENDERER_H

#include <game/field_entity.h>

typedef struct FieldTimedRenderer {
    FieldRenderObject base;
    struct { u16 unknown_00_03 : 4, finished : 1; s16 delay : 11; } control;
    u16 unknown_13a;
    union {
        u32 raw;
        struct { u32 unknown_00_14 : 15, expired : 1, unknown_16_31 : 16; } bits;
        struct { u16 flags, remaining; } halves;
    } state;
} FieldTimedRenderer;

typedef char FieldTimedRenderer_SizeCheck[sizeof(FieldTimedRenderer) == 0x140 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void FieldTimedRenderer_Update(FieldTimedRenderer *model);
void FieldTimedRenderer_ResetAnimationControl(FieldTimedRenderer *model);
void FieldTimedRenderer_SetAnimationDelay(FieldTimedRenderer *model, int delay);
void FieldTimedRenderer_RestartAnimation(FieldTimedRenderer *model);
#ifdef __cplusplus
}
#endif

#endif
