#include <game/field_entity_lifecycle.h>
#include <game/field_deferred_entity.h>
#include <game/field_blink.h>
extern "C" {
#include <game/heap.h>

extern FieldEntityVTable data_ov000_020c13e0;
extern const s8 data_ov000_020c0c9c[];
extern const s8 data_ov000_020c0cc0[];
extern void func_ov000_020a7a80(FieldRuntimeEntity *, const FieldRuntimeEntity *);
extern void func_ov000_020b5e84(FieldRuntimeEntity *, int, const FieldSpawnRecord *, int, int, u8);
extern void func_ov000_020b4990(FieldRuntimeEntity *);
extern void func_0202cbd4(void *, int, u32);

FieldDeferredEntity *FieldDeferredEntity_Init(FieldDeferredEntity *entity, int entity_index,
    const FieldSpawnRecord *spawn_record, int argument_3, int resource_set, u8 argument_5)
{
    func_ov000_020b5e84(&entity->runtime, entity_index, spawn_record, argument_3, resource_set, argument_5);
    *(FieldEntityVTable **)entity = &data_ov000_020c13e0;
    func_0202cbd4(&entity->control, 0, sizeof(entity->control));
    if (spawn_record && entity->runtime.unknown_3d4 == -1)
        entity->runtime.unknown_3d4 = 0;
    entity->control.unknown_00 = 1;
    entity->control.unknown_01 = 0;
    entity->control.unknown_02 = 0;
    entity->control.unknown_03 = 0;
    return entity;
}

FieldDeferredEntity *FieldDeferredEntity_InitEmpty(FieldDeferredEntity *entity)
{
    FieldEntity3D_Init(&entity->runtime);
    *(FieldEntityVTable **)entity = &data_ov000_020c13e0;
    func_0202cbd4((u8 *)entity + 4, 0, sizeof(*entity) - 4);
    return entity;
}

FieldDeferredEntity *FieldDeferredEntity_DestroyBase(FieldDeferredEntity *entity)
{
    *(FieldEntityVTable **)entity = &data_ov000_020c13e0;
    FieldEntity3D_DestroyBase(&entity->runtime);
    return entity;
}

FieldDeferredEntity *FieldDeferredEntity_Delete(FieldDeferredEntity *entity)
{
    *(FieldEntityVTable **)entity = &data_ov000_020c13e0;
    FieldEntity3D_DestroyBase(&entity->runtime);
    GameHeap_Delete(entity);
    return entity;
}

void FieldDeferredEntity_UpdateTimedPause(FieldDeferredEntity *entity)
{
    func_ov000_020b4990(&entity->runtime);
    if (entity->control.pause_frames) {
        entity->control.pause_frames = entity->control.pause_frames - 1;
        if (!entity->control.pause_frames)
            FieldDeferredEntity_ResumeScript(entity);
    }
}

void FieldDeferredEntity_BeginTimedPause(FieldDeferredEntity *entity)
{
    entity->runtime.base.start_blink(2, data_ov000_020c0c9c, 34, 0, 0);
    entity->runtime.base.property_00a_bits.property_00a_flag_00 = 0;
    entity->runtime.base.pause_script();
    /* Three seconds at the game's 60 Hz update rate. */
    entity->control.pause_frames = 180;
    entity->control.unknown_22 = (entity->runtime.unknown_3a0_bits.unknown_20_25 & 5) != 0;
}

void FieldDeferredEntity_ResumeScript(FieldDeferredEntity *entity)
{
    entity->runtime.base.property_00a_bits.property_00a_flag_00 = 1;
    entity->runtime.base.resume_script();
}

void FieldDeferredEntity_StopScript(FieldDeferredEntity *entity, int deferred)
{
    entity->runtime.base.property_00a_bits.property_00a_flag_00 = 0;
    entity->runtime.base.stop_script();
    if (entity->runtime.base_state_flag_bits.animation_wait_enabled) {
        if (deferred) {
            entity->runtime.base.start_blink(3, 0, 0, 0, 0);
            entity->runtime.saved_presentation_flag_bits.behavior_mode = 0;
            entity->runtime.render_object->state_flags |= FIELD_RENDER_ANIMATION_SUPPRESSED;
            entity->control.phase = 2;
        } else {
            entity->runtime.base.set_visible(0);
        }
    }
}

void FieldDeferredEntity_PauseScript(FieldDeferredEntity *entity)
{
    entity->runtime.base.property_00a_bits.property_00a_flag_00 = 0;
    if (entity->runtime.base_state_flag_bits.animation_wait_enabled) {
        entity->runtime.base.start_blink(2, data_ov000_020c0cc0, 72, 0, 0);
        entity->runtime.base.pause_script();
        entity->control.phase = 1;
    } else {
        entity->runtime.base.stop_script();
    }
}

void FieldDeferredEntity_UpdateBlink(FieldDeferredEntity *entity)
{
    if (entity->runtime.base_state_flag_bits.blink_mode) {
        FieldBlink_Update(&entity->runtime);
        if (!entity->runtime.base_state_flag_bits.blink_mode) {
            if (entity->control.phase) {
                switch (entity->control.phase) {
                case 1: entity->runtime.base.resume_script(); break;
                case 2: break;
                }
                entity->control.phase = 0;
            }
        }
    }
}

FieldDeferredEntity *FieldDeferredEntity_CopyState(FieldDeferredEntity *entity, const FieldDeferredEntity *source)
{
    entity->control.unknown_00 = source->control.unknown_00;
    entity->control.unknown_01 = source->control.unknown_01;
    entity->control.unknown_02 = source->control.unknown_02;
    entity->control.unknown_03 = source->control.unknown_03;
    entity->control.phase = source->control.phase;
    entity->control.pause_frames = source->control.pause_frames;
    entity->control.unknown_22 = source->control.unknown_22;
    func_ov000_020a7a80(&entity->runtime, &source->runtime);
    return entity;
}
}
