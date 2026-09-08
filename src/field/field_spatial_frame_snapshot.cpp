#include <game/field_entity_motion.h>
extern "C" {

void FieldEntity3D_SetNavigationSurfaces(FieldRuntimeEntity *entity, FieldNavigationSurface *surfaces,
                                         const void *resource)
{
    entity->navigation_surfaces = surfaces;
    entity->navigation_resource = resource;
}

void FieldEntity3D_BeginFrame(FieldRuntimeEntity *entity)
{
    FieldEntity2D_BeginFrame(entity);
    entity->previous_position_z = entity->position_z;
    entity->previous_relative_height = entity->relative_height;
    entity->previous_support_clearance = entity->support_clearance;
    entity->field_state_flag_bits.previous_vertical_motion_active =
        entity->field_state_flag_bits.vertical_motion_active;
    entity->unknown_3a0_bits.previous_contact_active = entity->unknown_3a0_bits.contact_active;
    entity->unknown_3a0_bits.contact_active = 0;
    entity->unknown_3a0_bits.previous_contact_mask_a = entity->unknown_3a0_bits.contact_mask_a;
    entity->unknown_3a0_bits.contact_mask_a = 0;
    entity->unknown_3a0_bits.previous_contact_mask_b = entity->unknown_3a0_bits.contact_mask_b;
    entity->unknown_3a0_bits.contact_mask_b = 0;
    entity->unknown_3ac = entity->unknown_3a8;
    entity->unknown_3a8 = 0;
    entity->unknown_4f4 = entity->unknown_4f0;
    entity->unknown_4f0 = 0;
    entity->previous_support_entity = entity->support_entity;
    entity->support_entity = 0;
    entity->unknown_504 = entity->unknown_500;
    entity->unknown_500 = 0;
    entity->previous_support_entity_index = entity->support_entity_index;
    entity->support_entity_index = -1;
    entity->runtime_flag_bits.previous_contact_mask_b = entity->runtime_flag_bits.contact_mask_b;
    entity->runtime_flag_bits.contact_mask_b = 0;
    entity->unknown_50c = entity->unknown_508;
    entity->unknown_508 = 0;
    entity->previous_frame_delta_z = entity->frame_delta_z;
    entity->frame_delta_z = 0;
    entity->unknown_3d2 = entity->unknown_3d0;
}
}
