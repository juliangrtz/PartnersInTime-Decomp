/* Toggle the primary renderer and its eligible shadow together. The entity
 * keeps the requested state even when no primary renderer is allocated. */
#include <game/field_presentation.h>
void func_020093b4(FieldRenderObject *, int);
void FieldEntity_SetRendererVisibility(FieldRuntimeEntity *entity, int enabled)
{
    entity->base_state_flag_bits.animation_wait_enabled = enabled;
    /* Preserve the native primary-renderer guard around both renderer calls. */
    if (entity->render_object) {
        func_020093b4(entity->render_object, enabled);
        if (entity->auxiliary_render_object) {
            if (entity->field_state_flag_bits.shadow_enabled &&
                (entity->relative_height > 0 || entity->field_state_flag_bits.unknown_16))
                func_020093b4(entity->auxiliary_render_object, enabled);
            else
                func_020093b4(entity->auxiliary_render_object, 0);
        }
    }
}
