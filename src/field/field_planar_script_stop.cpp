#include <game/field_entity_motion.h>
#include <game/field_linear.h>
#include <game/field_transform.h>

void FieldEntity2D_StopScript(FieldRuntimeEntity *entity) {
    entity->base.state_flag_bits.active = 0;
    entity->base.action_timer = 0;
    entity->base.cancel_linear_movement(0, 0);
    entity->base.cancel_orbit_movement(0, 0);
    FieldScaling_Stop(entity, 0, 0);
}
