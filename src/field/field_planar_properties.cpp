/* Script-facing 2D properties: movement, presentation and renderer state.
 * The speed query prefers an active linear controller, then an active orbit. */
#include <game/field_entity_properties.h>
extern "C" int FieldEntity2D_GetProperty(FieldRuntimeEntity *entity, int property)
{
    switch (property) {
    case FIELD_PROPERTY_ENABLED:
    case FIELD_PROPERTY_INTERACTION_STATE:
        return FieldEntity_GetInteractionState(&entity->base, property);
    case FIELD_PROPERTY_PLANAR_MOTION:
        return entity->frame_delta_x || entity->frame_delta_y;
    case FIELD_PROPERTY_ANIMATION_ENABLED:
        return entity->base_state_flag_bits.animation_wait_enabled;
    case FIELD_PROPERTY_OUTSIDE_SCREEN:
        return entity->base_state_flag_bits.outside_screen;
    case FIELD_PROPERTY_RENDER_LAYER:
        return entity->render_object->sort_key_bits.layer;
    case FIELD_PROPERTY_POSITION_X:
        return entity->position_x / 4096;
    case FIELD_PROPERTY_POSITION_Y:
        return entity->position_y / 4096;
    case FIELD_PROPERTY_FACING:
        return entity->base_state_flag_bits.facing_direction;
    case FIELD_PROPERTY_SCREEN_X:
        return entity->screen_x;
    case FIELD_PROPERTY_SCREEN_Y:
        return entity->screen_y;
    case FIELD_PROPERTY_MOVEMENT_SPEED:
        if (entity->linear_controller.bits.active_axes)
            return entity->linear_controller.timing.speed;
        if (entity->orbit_controller.bits.active)
            return entity->orbit_controller.speed;
        return entity->movement_speed;
    case FIELD_PROPERTY_RESOURCE_INDEX:
        return entity->resource_index;
    case FIELD_PROPERTY_RESOURCE_ANIMATION:
        return entity->render_object->resource_animation;
    case FIELD_PROPERTY_ANIMATION_ID:
        return entity->render_object->animation_id;
    case FIELD_PROPERTY_ANIMATION_SPEED:
        return entity->render_object->animation_speed;
    default:
        return 0;
    }
}
