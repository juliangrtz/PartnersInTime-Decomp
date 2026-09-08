extern "C" {
#include <nitro/fx.h>
extern u16 FX_Atan2Idx(fx32, fx32);
extern const u8 data_ov000_020c0b04[];
}
#include <game/field_entity_motion.h>
extern "C" {

void FieldEntity3D_UpdateLocomotionState(FieldRuntimeEntity *entity)
{
    if (entity->base.property_00a_bits.property_00a_flag_00) {
        if (entity->field_state_flag_bits.vertical_motion_active) {
            if (entity->position_z >= entity->previous_position_z)
                entity->locomotion_state = 2;
            else
                entity->locomotion_state = 3;
        } else {
            if (entity->frame_delta_x || entity->frame_delta_y)
                entity->locomotion_state = 1;
            else
                entity->locomotion_state = 0;
        }
        entity->base.map_locomotion_state();
    }
}

void FieldEntity3D_MapLocomotionState(FieldRuntimeEntity *entity)
{
    entity->locomotion_category = data_ov000_020c0b04[entity->locomotion_state];
    if (entity->locomotion_category == 1 && entity->field_state_flag_bits.unknown_07)
        entity->locomotion_category = 0;
}

void FieldEntity3D_AccumulateMotion(FieldRuntimeEntity *entity, fx32 x, fx32 y, fx32 z)
{
    entity->frame_delta_z += z;
    FieldEntity2D_AccumulateMotion(entity, x, y);
}

}
