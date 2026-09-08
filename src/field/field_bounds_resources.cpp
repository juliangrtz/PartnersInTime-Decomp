#include <game/field_presentation.h>
#include <game/field_timed_renderer.h>
#include <game/sprite_output.h>
extern "C" {
void FieldEntity_StopRenderer(FieldRuntimeEntity *entity)
{
    if (entity->saved_presentation_flag_bits.unknown_06) {
        entity->render_object->stop();
        entity->render_object->unknown_1c();
        if (!entity->saved_presentation_flag_bits.resource_set)
            GameSpriteAllocation_Compact((u8)entity->base.property_00a_bits.property_00a_unknown_01, 0, 0);
        entity->saved_presentation_flag_bits.unknown_06 = 0;
    }
}

void FieldEntity_SetResourceAnimation(FieldRuntimeEntity *entity, int resource_animation, int animation,
                                      int reset_control, u8 update_bounds)
{
    if (reset_control)
        FieldTimedRenderer_ResetAnimationControl((FieldAnimationRenderer *)entity->render_object);
    entity->render_object->set_animation(resource_animation, (s16)animation, 1);
    entity->animation_id = resource_animation;
    if (update_bounds)
        entity->base.update_bounds();
}

void FieldEntity2D_BindBoundsResource(FieldRuntimeEntity *entity, const u16 *resource)
{
    entity->bounds_resource = resource;
    if (resource) {
        entity->interaction_bounds = (const FieldInteractionBounds *)((const u8 *)resource + resource[2] + 1);
        entity->animation_bounds =
            (const FieldAnimationBoundsIndex *)((const u8 *)resource + resource[5] + 1);
        entity->bounds_animation_count = *((const u8 *)resource + resource[5]);
    } else {
        entity->interaction_bounds = 0;
        entity->animation_bounds = 0;
        entity->bounds_animation_count = 0;
    }
    entity->base.update_bounds();
}
}
