#include <game/field_transform.h>
static inline void SetScale(FieldRenderObject *renderer, s16 x, s16 y)
{
    renderer->scale_x = x;
    renderer->scale_y = y;
}
static inline void AddScale(FieldRenderObject *renderer, s16 x, s16 y)
{
    s16 old_x = renderer->scale_x;
    s16 old_y = renderer->scale_y;
    renderer->scale_x = old_x + x;
    renderer->scale_y = old_y + y;
}
extern "C" int FieldRotation_StartTimed(FieldRuntimeEntity *entity, int relative, u32 angle, int duration,
                                        int direction, u8 snap, FieldTransformController *controller)
{
    int old_angle = entity->render_object->rotation;
    int destination, distance;
    if (!controller)
        controller = &entity->transform_controller;
    if (!relative) {
        destination = angle & 0xFFFF;
        distance = direction > 0 ? destination - old_angle : old_angle - destination;
        if (distance < 0)
            distance += 0x10000;
        distance += angle & 0xFFFF0000;
    } else {
        distance = angle;
        destination = (old_angle + angle * direction) & 0xFFFF;
    }
    controller->bits.rotation_active = 1;
    controller->bits.rotation_paused = 0;
    controller->bits.rotation_fixed_duration = 1;
    controller->rotation_elapsed = 0;
    controller->start_rotation = old_angle;
    controller->destination_rotation = destination;
    controller->rotation_duration = duration;
    controller->bits.stop_rotation_at_destination = snap;
    controller->rotation_step = (distance * direction) / duration;
    controller->remaining_rotation = distance;
    return 1;
}
extern "C" void FieldRotation_Stop(FieldRuntimeEntity *entity, FieldTransformController *controller, int snap)
{
    if (!controller)
        controller = &entity->transform_controller;
    if (controller->bits.rotation_active) {
        controller->bits.rotation_active = 0;
        if (snap)
            entity->render_object->rotation = controller->destination_rotation;
    }
}
extern "C" void FieldTransform_Advance(FieldRuntimeEntity *entity, FieldTransformController *controller)
{
    if (!controller)
        controller = &entity->transform_controller;
    if (controller->scaling.scaling_active && !controller->bits.scaling_paused) {
        s16 step_x = controller->bits.scale_x_active ? controller->scale_x_step : 0;
        AddScale(entity->render_object, step_x,
                 controller->bits.scale_y_active ? controller->scale_y_step : 0);
        ++controller->scaling_elapsed;
    }
    if (controller->bits.rotation_active && !controller->bits.rotation_paused) {
        entity->render_object->rotation += controller->rotation_step;
        controller->remaining_rotation -= controller->rotation_speed;
        ++controller->rotation_elapsed;
    }
}
extern "C" int FieldTransform_CheckCompletion(FieldRuntimeEntity *entity,
                                              FieldTransformController *controller)
{
    if (!controller)
        controller = &entity->transform_controller;
    if (controller->scaling.scaling_active && !controller->bits.scaling_paused) {
        if (!controller->bits.scaling_fixed_duration) {
            if (controller->scaling.scaling_active) {
                int x = entity->render_object->scale_x;
                int y = entity->render_object->scale_y;
                if (controller->bits.scale_x_active) {
                    int destination = controller->destination_scale_x;
                    int start = controller->start_scale_x;
                    if ((start > destination && x <= destination) ||
                        (start < destination && x >= destination)) {
                        x = destination;
                        controller->bits.scale_x_active = 0;
                    }
                }
                if (controller->bits.scale_y_active) {
                    int destination = controller->destination_scale_y;
                    int start = controller->start_scale_y;
                    if ((start > destination && y <= destination) ||
                        (start < destination && y >= destination)) {
                        y = destination;
                        controller->bits.scale_y_active = 0;
                    }
                }
                SetScale(entity->render_object, x, y);
            }
        } else {
            if (controller->scaling_elapsed == controller->x.scale_x_duration) {
                SetScale(entity->render_object, controller->destination_scale_x,
                         entity->render_object->scale_y);
                controller->bits.scale_x_active = 0;
            }
            if (controller->scaling_elapsed == controller->y.scale_y_duration) {
                entity->render_object->scale_y = controller->destination_scale_y;
                controller->bits.scale_y_active = 0;
            }
        }
    }
    if (controller->bits.rotation_active && !controller->bits.rotation_paused &&
        controller->bits.stop_rotation_at_destination) {
        if (!controller->bits.rotation_fixed_duration) {
            if (controller->remaining_rotation <= 0) {
                entity->render_object->rotation = controller->destination_rotation;
                controller->bits.rotation_active = 0;
            }
        } else if (controller->rotation_elapsed == controller->rotation_duration) {
            entity->render_object->rotation = controller->destination_rotation;
            controller->bits.rotation_active = 0;
        }
    }
    return controller->state.active != 0;
}

extern "C" {

#include <game/field_entity.h>
#include <game/field_timed_renderer.h>
extern void func_0200940c(FieldRenderObject *, s16);
extern void func_020093b4(FieldRenderObject *, u8);
static inline void SetFlagPair(FieldRenderObject *renderer, u8 first, u8 second)
{
    renderer->state_flag_bits.unknown_10 = first;
    renderer->state_flag_bits.unknown_11 = second;
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
    entity->render_object->unknown_48(1);
}

}
