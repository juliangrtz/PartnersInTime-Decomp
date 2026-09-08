extern "C" {
#include <nitro/fx.h>
extern u16 FX_Atan2Idx(fx32, fx32);
extern const u8 data_ov000_020c0b04[];
}
#include <game/field_entity_motion.h>
extern "C" {

void FieldEntity2D_UpdateLocomotionState(FieldRuntimeEntity *entity)
{
    if (entity->base.property_00a_bits.property_00a_flag_00) {
        if (entity->frame_delta_x || entity->frame_delta_y)
            entity->locomotion_state = 1;
        else
            entity->locomotion_state = 0;
        entity->base.map_locomotion_state();
    }
}

void FieldEntity2D_MapLocomotionState(FieldRuntimeEntity *entity)
{
    entity->locomotion_category = data_ov000_020c0b04[entity->locomotion_state];
}

void FieldEntity2D_AccumulateMotion(FieldRuntimeEntity *entity, fx32 x, fx32 y)
{
    entity->frame_delta_x += x;
    entity->frame_delta_y += y;
    if (entity->frame_delta_x || entity->frame_delta_y) {
        entity->movement_direction = FX_Atan2Idx(entity->frame_delta_y, entity->frame_delta_x) + 0x4000;
        if (entity->saved_presentation_flag_bits.behavior_mode >= 3 && (x || y)) {
            entity->base_state_flag_bits.facing_direction =
                (u32)(u16)(FX_Atan2Idx(entity->frame_delta_y, entity->frame_delta_x) + 0x4000) >> 13;
        }
    }
}
}
