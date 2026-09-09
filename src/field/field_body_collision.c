#include <game/field_roaming.h>
extern s32 FX_Atan2(fx32, fx32);
static inline void UpdateBodyAngles(FieldRuntimeEntity *entity)
{
    entity->body_center_y = (entity->body_min_y + entity->body_max_y) / 2;
    entity->body_corner_angles[0] = FX_Atan2(entity->body_min_x, entity->body_center_y);
    entity->body_corner_angles[1] = FX_Atan2(entity->body_min_x, -entity->body_center_y);
    entity->body_corner_angles[2] = FX_Atan2(entity->body_max_x, -entity->body_center_y);
    entity->body_corner_angles[3] = FX_Atan2(entity->body_max_x, entity->body_center_y);
}
void FieldEntity_LoadBodyBounds(FieldRuntimeEntity *entity)
{
    if (entity->bounds_resource) {
        int animation = entity->animation_id;
        const FieldInteractionBounds *bounds;
        int x, y, z, width, height;
        if (animation >= entity->bounds_animation_count)
            animation = 0;
        entity->body_bounds_index = entity->animation_bounds[animation].body_index;
        bounds = entity->body_bounds + entity->body_bounds_index;
        x = bounds->minimum_x;
        y = bounds->maximum_y;
        z = bounds->vertical_extent;
        height = bounds->height;
        width = bounds->width;
        entity->body_min_x = x << 12;
        entity->body_max_x = entity->body_min_x + (width << 12);
        entity->body_max_y = y << 12;
        entity->body_min_y = entity->body_max_y - (height << 12);
        entity->body_vertical_extent = z << 12;
        UpdateBodyAngles(entity);
    } else {
        entity->body_min_x = -32768;
        entity->body_max_x = entity->body_min_x + 65536;
        entity->body_max_y = 0;
        entity->body_min_y = entity->body_max_y - 32768;
        entity->body_vertical_extent = 131072;
        UpdateBodyAngles(entity);
    }
}
void FieldEntity_UpdateCollisionFlagGroups(FieldRuntimeEntity *entity, int flag0, int flag23, int flag1,
                                           int flag6, int flag4)
{
    if (flag0 != -1)
        entity->collision_state_flag_bits.unknown_00 = flag0;
    if (flag23 != -1) {
        entity->collision_state_flag_bits.unknown_02 = flag23;
        entity->collision_state_flag_bits.unknown_03 = flag23;
    }
    if (flag1 != -1)
        entity->collision_state_flag_bits.unknown_01 = flag1;
    if (flag6 != -1)
        entity->collision_state_flag_bits.unknown_06 = flag6;
    if (flag4 != -1)
        entity->collision_state_flag_bits.unknown_04 = flag4;
}
void FieldEntity_SetCollisionFlagGroups(FieldRuntimeEntity *entity, u8 mask, u8 enabled)
{
    if (mask & 1)
        entity->collision_state_flag_bits.unknown_00 = enabled;
    if (mask & 2) {
        entity->collision_state_flag_bits.unknown_02 = enabled;
        entity->collision_state_flag_bits.unknown_03 = enabled;
    }
    if (mask & 4)
        entity->collision_state_flag_bits.unknown_01 = enabled;
    if (mask & 8)
        entity->collision_state_flag_bits.unknown_06 = enabled;
    if (mask & 16)
        entity->collision_state_flag_bits.unknown_04 = enabled;
}
void FieldEntity_SelectBodyContact(FieldRuntimeEntity *entity, FieldRuntimeEntity *contact, u8 mask)
{
    if (!entity->unknown_500 ||
        (contact->field_state_flag_bits.contact_mode == 0 && entity->unknown_504 != contact) ||
        (contact->field_state_flag_bits.contact_mode == 4 &&
         (entity->unknown_504 != contact ||
          entity->locomotion_category == entity->previous_locomotion_category))) {
        entity->unknown_3a0_bits.contact_mask_b = mask;
        entity->unknown_500 = contact;
    }
}
