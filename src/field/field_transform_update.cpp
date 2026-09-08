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
