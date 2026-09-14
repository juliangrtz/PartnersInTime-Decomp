#include <game/field_entity_motion.h>
#include <game/field_linear.h>
#include <game/field_transform.h>

void FieldEntity3D_StopScript(FieldRuntimeEntity *entity) {
    entity->base.state_flag_bits.active = 0;
    entity->base.action_timer = 0;
    entity->roaming_state.active = 0;
    entity->roaming_state.moving = 0;
    entity->base.cancel_linear_movement(0, 0);
    FieldVertical_Stop(entity);
    FieldScaling_Stop(entity, 0, 0);
}

void FieldEntity3D_PauseScript(FieldRuntimeEntity *entity) {
    entity->field_state_flag_bits.vertical_motion_paused = 1;
    entity->base.state_flag_bits.flag_02 = 1;
    entity->roaming_state.paused = 1;
    FieldLinear_Pause(entity, 0);
    FieldScaling_Pause(entity, 0);
}

void FieldEntity3D_ResumeScript(FieldRuntimeEntity *entity) {
    entity->field_state_flag_bits.vertical_motion_paused = 0;
    entity->base.state_flag_bits.flag_02 = 0;
    entity->roaming_state.paused = 0;
    FieldLinear_Resume(entity, 0);
    FieldScaling_Resume(entity, 0);
}
