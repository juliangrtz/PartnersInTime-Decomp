/*
 * 2D entity script control (overlay 0, 0x020A3854-0x020A3928).
 *
 * Stop, pause and resume for the script attached to a 2D entity.
 */

#include <game/field_entity_motion.h>
#include <game/field_linear.h>
#include <game/field_orbit.h>
#include <game/field_transform.h>

void FieldEntity2D_StopScript(FieldRuntimeEntity *entity) {
    entity->base.state_flag_bits.active = 0;
    entity->base.action_timer = 0;
    entity->base.cancel_linear_movement(0, 0);
    entity->base.cancel_orbit_movement(0, 0);
    FieldScaling_Stop(entity, 0, 0);
}

void FieldEntity2D_PauseScript(FieldRuntimeEntity *entity) {
    entity->base.state_flag_bits.flag_02 = 1;
    FieldLinear_Pause(entity, 0);
    FieldOrbit_Pause(entity, 0);
    FieldScaling_Pause(entity, 0);
}

void FieldEntity2D_ResumeScript(FieldRuntimeEntity *entity) {
    entity->base.state_flag_bits.flag_02 = 0;
    FieldLinear_Resume(entity, 0);
    FieldOrbit_Resume(entity, 0);
    FieldScaling_Resume(entity, 0);
}
