/*
 * 3D frame state (overlay 0, 0x020B4920-0x020B4EDC).
 *
 * A 3D entity's per-frame state: the navigation surfaces it is standing on, the
 * locomotion state and its mapping to an animation, and the accumulated motion.
 */

#include <game/field_entity_motion.h>
#include <game/field_linear.h>
#include <game/field_orbit.h>
#include <game/field_roaming.h>
#include <game/field_transform.h>

#define FRAME_MINIMUM(a, b) ((a) <= (b) ? (a) : (b))
#define FRAME_MAXIMUM(a, b) ((a) >= (b) ? (a) : (b))
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

extern "C" {
#include <nitro/fx.h>
extern u16 FX_Atan2Idx(fx32, fx32);
extern const u8 data_ov000_020c0b04[];
}
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

extern "C" {
/* Advance controllers, carry motion from the previous supporting entity,
 * and bound the navigation search using both old and new positions. */
void FieldEntity3D_UpdateMotion(FieldRuntimeEntity *entity, FieldRuntimeEntity **entities)
{
    if (!entity->saved_presentation_flag_bits.unknown_31) {
        entity->base.unknown_78();
        if (entity->base.property_00a_bits.property_00a_flag_00) {
            FieldRoaming_Update(entity);
            entity->base.update_linear_movement(0);
            entity->base.update_orbit_movement(0);
            if (entity->field_state_flag_bits.vertical_motion_active)
                FieldVertical_Update(entity);
            else {
                entity->position_z += entity->frame_delta_z;
                entity->relative_height += entity->frame_delta_z;
            }
            entity->position_x += entity->frame_delta_x;
            entity->position_y += entity->frame_delta_y;
            if (entity->previous_support_entity_index != -1) {
                FieldRuntimeEntity *support = entities[entity->previous_support_entity_index];
                entity->position_x += support->position_x - support->previous_position_x;
                entity->position_y += support->position_y - support->previous_position_y;
                fx32 delta_z = support->position_z - support->previous_position_z;
                if (delta_z > 0 || entity->field_state_flag_bits.track_ground) {
                    entity->position_z += delta_z;
                    entity->relative_height = support->relative_height + support->body_vertical_extent;
                }
                fx32 clearance = support->position_z + support->body_vertical_extent
                                 - entity->position_z;
                if (clearance > 0)
                    entity->position_z += clearance;
            }
            entity->base.check_linear_completion(0);
            entity->base.advance_orbit_frame(0);
            FieldNavigationSurface *surface = entity->navigation_surfaces;
            if (surface) {
                fx32 minimum_x = FRAME_MINIMUM(entity->position_x, entity->previous_position_x);
                fx32 limit = entity->navigation_min_x + minimum_x - 0x80000
                             - entity->locomotion.starting_speed;
                while (!surface->bits.end && surface->sort_x < limit)
                    ++surface;
            }
            entity->navigation_cursor = surface;
            entity->navigation_scan_limit = entity->navigation_max_x
                + FRAME_MAXIMUM(entity->position_x, entity->previous_position_x) + 0x10000;
            entity->swept_min_x = entity->navigation_min_x
                + FRAME_MINIMUM(entity->position_x, entity->previous_position_x)
                - entity->locomotion.starting_speed;
            entity->swept_min_y = entity->navigation_min_y
                + FRAME_MINIMUM(entity->position_y, entity->previous_position_y)
                - entity->locomotion.starting_speed;
            entity->swept_max_x = entity->locomotion.starting_speed
                + (entity->navigation_max_x
                   + FRAME_MAXIMUM(entity->position_x, entity->previous_position_x));
            entity->swept_max_y = entity->locomotion.starting_speed
                + (entity->navigation_max_y
                   + FRAME_MAXIMUM(entity->position_y, entity->previous_position_y));
        }
        if (entity->base_state_flag_bits.animation_wait_enabled) {
            FieldTransform_Advance(entity, 0);
            FieldTransform_CheckCompletion(entity, 0);
        }
    }
}

/* Finish contact-dependent updates after the spatial pass. */
void FieldEntity3D_FinishMotion(FieldRuntimeEntity *entity)
{
    if (entity->runtime_flag_bits.unknown_22) {
        entity->base.unknown_9c();
        FieldEntity3D_UpdateSupportClearance(entity);
        entity->runtime_flag_bits.unknown_22 = 0;
    }
    entity->base.update_locomotion_state();
    FieldLinear3D_CheckStopMasks(entity, 0);
    FieldOrbit3D_CheckStopMasks(entity, 0);
    FieldEntity3D_UpdateBounceShake(entity);
}
}
