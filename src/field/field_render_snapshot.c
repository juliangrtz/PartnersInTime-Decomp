#include <game/field_entity.h>
#include <game/field_timed_renderer.h>
extern void func_0200940c(FieldRenderObject *, s16);
extern void func_020093b4(FieldRenderObject *, u8);
static inline void SetFlagPair(FieldRenderObject *renderer, u8 first, u8 second)
{
    renderer->state_flag_bits.unknown_10 = first;
    renderer->state_flag_bits.unknown_11 = second;
}
static inline void SetScale(FieldRenderObject *renderer, s16 x, s16 y)
{
    renderer->scale_x = x;
    renderer->scale_y = y;
}
void FieldEntity_SaveRenderSnapshot(FieldRuntimeEntity *entity, FieldRenderSnapshot *snapshot)
{
    if (!entity->render_object)
        return;
    if (!snapshot)
        snapshot = &entity->render_snapshot;
    if (!entity->saved_presentation_flag_bits.unknown_06) {
        snapshot->flags.valid = 0;
        return;
    }
    snapshot->flags.valid = 1;
    snapshot->animation_speed = entity->render_object->animation_speed;
    snapshot->scale_x = entity->render_object->scale_x;
    snapshot->scale_y = entity->render_object->scale_y;
    snapshot->rotation = entity->render_object->rotation;
    snapshot->flags.semitransparent = entity->render_object->state_flag_bits.semitransparent;
    snapshot->flags.render_05_07 = entity->render_object->state_flag_bits.unknown_05_07;
    snapshot->flags.render_10 = entity->render_object->state_flag_bits.unknown_10;
    snapshot->flags.render_11 = entity->render_object->state_flag_bits.unknown_11;
    snapshot->flags.behavior_state = entity->render_object->state_flag_bits.behavior_state;
    snapshot->sort_key = entity->render_object->sort_key;
    snapshot->overlap_priorities[0] = entity->render_object->overlap_priority_bytes[0];
    snapshot->overlap_priorities[1] = entity->render_object->overlap_priority_bytes[1];
    snapshot->overlap_priorities[2] = entity->render_object->overlap_priority_bytes[2];
    snapshot->overlap_priorities[3] = entity->render_object->overlap_priority_bytes[3];
    snapshot->animation_control.finished = ((FieldAnimationRenderer *)entity->render_object)->control.finished;
    snapshot->animation_control.loops_remaining = ((FieldAnimationRenderer *)entity->render_object)->control.loops_remaining;
    snapshot->unknown_08 = entity->render_object->unknown_064;
    snapshot->animation_id = entity->render_object->animation_id;
    snapshot->unknown_14 = (u8)entity->render_object->unknown_060;
    snapshot->flags.render_02 = (u8)entity->render_object->state_flag_bits.unknown_02;
    snapshot->flags.animation_active = (u8)entity->render_object->state_flag_bits.animation_active;
    snapshot->flags.animation_suppressed = (u8)entity->render_object->state_flag_bits.animation_suppressed;
}

void FieldEntity_RestoreRenderSnapshot(FieldRuntimeEntity *entity, const FieldRenderSnapshot *snapshot)
{
    if (!snapshot)
        snapshot = &entity->render_snapshot;
    if (!snapshot->flags.valid)
        return;
    if (!entity->render_object)
        return;
    func_0200940c(entity->render_object, snapshot->animation_speed);
    entity->render_object->unknown_060 = (u8)snapshot->unknown_14;
    entity->render_object->state_flag_bits.semitransparent = snapshot->flags.semitransparent;
    func_020093b4(entity->render_object, snapshot->flags.animation_active);
    entity->render_object->state_flag_bits.animation_suppressed = (u8)snapshot->flags.animation_suppressed;
    SetFlagPair(entity->render_object, snapshot->flags.render_10, snapshot->flags.render_11);
    SetScale(entity->render_object, snapshot->scale_x, snapshot->scale_y);
    entity->render_object->rotation = snapshot->rotation;
    entity->render_object->state_flag_bits.unknown_05_07 = (u8)snapshot->flags.render_05_07;
    entity->render_object->sort_key = snapshot->sort_key;
    entity->render_object->overlap_priority_bytes[0] = snapshot->overlap_priorities[0];
    entity->render_object->overlap_priority_bytes[1] = snapshot->overlap_priorities[1];
    entity->render_object->overlap_priority_bytes[2] = snapshot->overlap_priorities[2];
    entity->render_object->overlap_priority_bytes[3] = snapshot->overlap_priorities[3];
    ((FieldAnimationRenderer *)entity->render_object)->control.finished = snapshot->animation_control.finished;
    ((FieldAnimationRenderer *)entity->render_object)->control.loops_remaining = snapshot->animation_control.loops_remaining;
    entity->render_object->state_flag_bits.behavior_state = (u8)snapshot->flags.behavior_state;
    entity->render_object->unknown_064 = snapshot->unknown_08;
    entity->render_object->animation_id = snapshot->animation_id;
    entity->render_object->state_flag_bits.unknown_02 = snapshot->flags.render_02;
    entity->render_object->vtable->unknown_48(entity->render_object, 1);
}
