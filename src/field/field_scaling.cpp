#include <game/field_transform.h>
extern "C" void func_020093b4(FieldRenderObject *, u8);
static inline void SetScale(FieldRenderObject *renderer, s16 x, s16 y)
{
    renderer->scale_x = x;
    renderer->scale_y = y;
}
extern "C" void FieldEntity_DisableAnimation(FieldEntity *base)
{
    func_020093b4(((FieldRuntimeEntity *)base)->render_object, 0);
}
extern "C" void FieldEntity_EnableAnimation(FieldEntity *base)
{
    func_020093b4(((FieldRuntimeEntity *)base)->render_object, 1);
}
extern "C" int FieldScaling_Start(FieldRuntimeEntity *entity, int relative, int x, int y, int speed_x,
                                  int speed_y, FieldTransformController *controller)
{
    int old_x = entity->render_object->scale_x;
    int old_y = entity->render_object->scale_y;
    if (!controller)
        controller = &entity->transform_controller;
    if (relative == 1) {
        x = (s16)(x + old_x);
        y = (s16)(y + old_y);
    }
    controller->bits.scale_x_active = x != old_x;
    controller->bits.scale_y_active = y != old_y;
    if (controller->bits.scale_x_active || controller->bits.scale_y_active) {
        controller->bits.scaling_paused = 0;
        controller->bits.scaling_fixed_duration = 0;
        controller->scaling_elapsed = 0;
        controller->start_scale_x = old_x;
        controller->start_scale_y = old_y;
        controller->destination_scale_x = x;
        controller->destination_scale_y = y;
        if (controller->bits.scale_x_active) {
            if ((x > old_x && speed_x < 0) || (x < old_x && speed_x > 0))
                speed_x = (s16)-speed_x;
            controller->x.scale_x_speed = speed_x;
        } else {
            controller->x.scale_x_speed = 0;
        }
        if (controller->bits.scale_y_active) {
            if ((y > old_y && speed_y < 0) || (y < old_y && speed_y > 0))
                speed_y = (s16)-speed_y;
            controller->y.scale_y_speed = speed_y;
        } else {
            controller->y.scale_y_speed = 0;
        }
        controller->scale_x_step = controller->x.scale_x_speed;
        controller->scale_y_step = controller->y.scale_y_speed;
        return 1;
    }
    return 0;
}
extern "C" int FieldScaling_StartTimed(FieldRuntimeEntity *entity, int relative, int x, int y, int duration_x,
                                       int duration_y, FieldTransformController *controller)
{
    int old_x = entity->render_object->scale_x;
    int old_y = entity->render_object->scale_y;
    if (!controller)
        controller = &entity->transform_controller;
    if (relative == 1) {
        x = (s16)(x + old_x);
        y = (s16)(y + old_y);
    }
    controller->bits.scale_x_active = 1;
    controller->bits.scale_y_active = 1;
    controller->bits.scaling_paused = 0;
    controller->bits.scaling_fixed_duration = 1;
    controller->scaling_elapsed = 0;
    controller->start_scale_x = old_x;
    controller->start_scale_y = old_y;
    controller->destination_scale_x = x;
    controller->destination_scale_y = y;
    controller->x.scale_x_duration = duration_x;
    controller->y.scale_y_duration = duration_y;
    controller->scale_x_step = (x - old_x) / duration_x;
    controller->scale_y_step = (y - old_y) / duration_y;
    return 1;
}
extern "C" void FieldScaling_Stop(FieldRuntimeEntity *entity, FieldTransformController *controller, int snap)
{
    if (!controller)
        controller = &entity->transform_controller;
    if (controller->scaling.scaling_active) {
        controller->scaling.scaling_active = 0;
        if (snap)
            SetScale(entity->render_object, controller->destination_scale_x, controller->destination_scale_y);
    }
}
extern "C" void FieldScaling_Pause(FieldRuntimeEntity *entity, FieldTransformController *controller)
{
    if (!controller)
        controller = &entity->transform_controller;
    if (controller->scaling.scaling_active)
        controller->bits.scaling_paused = 1;
}
extern "C" void FieldScaling_Resume(FieldRuntimeEntity *entity, FieldTransformController *controller)
{
    if (!controller)
        controller = &entity->transform_controller;
    if (controller->scaling.scaling_active)
        controller->bits.scaling_paused = 0;
}
