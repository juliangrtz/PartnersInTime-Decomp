/* Preserve the previous culling result; blinking and explicit visibility flags
 * can suppress automatic renderer toggles. */
#include <game/field_presentation.h>
extern "C" void func_020093b4(FieldRenderObject *, int);
extern "C" void FieldEntity_UpdateScreenVisibility(FieldRuntimeEntity *entity)
{
    entity->base_state_flag_bits.previous_outside_screen =
        entity->base_state_flag_bits.outside_screen;
    entity->base_state_flag_bits.outside_screen = 0;
    if (entity->base.property_00a_bits.property_00a_flag_00) {
        if (entity->base.is_outside_screen()) {
            entity->base_state_flag_bits.outside_screen = 1;
            if (entity->base_state_flag_bits.animation_wait_enabled && entity->render_object &&
                entity->render_object->state_flag_bits.animation_active &&
                !entity->base_state_flag_bits.blink_mode &&
                !entity->base_state_flag_bits.unknown_09 &&
                !entity->base.visibility_bits.unknown_07)
                func_020093b4(entity->render_object, 0);
        } else {
            entity->base_state_flag_bits.outside_screen = 0;
            if (entity->base_state_flag_bits.animation_wait_enabled && entity->render_object &&
                !entity->render_object->state_flag_bits.animation_active &&
                !entity->base_state_flag_bits.blink_mode &&
                !entity->base.visibility_bits.unknown_07)
                func_020093b4(entity->render_object, 1);
        }
    }
}
