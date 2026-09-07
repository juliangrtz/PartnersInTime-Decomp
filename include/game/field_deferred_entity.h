#ifndef PIT_GAME_FIELD_DEFERRED_ENTITY_H
#define PIT_GAME_FIELD_DEFERRED_ENTITY_H

#include <game/field_entity.h>

typedef struct FieldDeferredEntity {
    FieldRuntimeEntity runtime;
    u8 unknown_514[12];
    union {
        u32 raw;
        struct {
            u32 unknown_00 : 1, unknown_01 : 1, unknown_02 : 1, unknown_03 : 1;
            u32 phase : 2, pause_frames : 16, unknown_22 : 1, unknown_23_31 : 9;
        };
    } control;
} FieldDeferredEntity;
typedef char FieldDeferredEntity_SizeCheck[sizeof(FieldDeferredEntity) == 0x524 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
FieldDeferredEntity *FieldDeferredEntity_Init(FieldDeferredEntity *entity, const void *descriptor,
    const void *spawn_record, int argument_3, const void *argument_4, u8 argument_5);
FieldDeferredEntity *FieldDeferredEntity_InitEmpty(FieldDeferredEntity *entity);
FieldDeferredEntity *FieldDeferredEntity_DestroyBase(FieldDeferredEntity *entity);
FieldDeferredEntity *FieldDeferredEntity_Delete(FieldDeferredEntity *entity);
FieldDeferredEntity *FieldDeferredEntity_CopyState(FieldDeferredEntity *entity, const FieldDeferredEntity *source);
void FieldDeferredEntity_PauseScript(FieldDeferredEntity *entity);
void FieldDeferredEntity_StopScript(FieldDeferredEntity *entity, int deferred);
void FieldDeferredEntity_ResumeScript(FieldDeferredEntity *entity);
void FieldDeferredEntity_UpdateBlink(FieldDeferredEntity *entity);
void FieldDeferredEntity_BeginTimedPause(FieldDeferredEntity *entity);
void FieldDeferredEntity_UpdateTimedPause(FieldDeferredEntity *entity);
#ifdef __cplusplus
}
#endif

#endif
