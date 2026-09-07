#include <game/field_presentation.h>
#include <game/field_timed_renderer.h>
extern "C" {

void FieldEntity_SetBehaviorMode(FieldEntity *base, int mode)
{
    FieldRuntimeEntity *entity = (FieldRuntimeEntity *)base;
    entity->saved_presentation_flag_bits.behavior_mode = mode;
    if (entity->saved_presentation_flag_bits.behavior_mode >= 3) {
        entity->unknown_178 = entity->base_state_flag_bits.facing_direction << 13;
        if (entity->saved_presentation_flag_bits.behavior_mode == 3)
            entity->render_object->set_animation((u8)entity->animation_id, 0, 1);
    }
    FieldTimedRenderer_ResetAnimationControl((FieldAnimationRenderer *)entity->render_object);
}
}
