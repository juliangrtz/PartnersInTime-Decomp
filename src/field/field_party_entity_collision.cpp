#include <game/field_party.h>

extern "C" {

int FieldPartyEntity_TestType3Volumes03(FieldPartyEntity *member)
{
    if (member->entity.navigation_surfaces) {
        FieldNavigationSurface *surface;
        fx32 x = member->entity.position_x;
        fx32 min_x = x + member->entity.navigation_min_x;
        fx32 max_x = x + member->entity.navigation_max_x;
        fx32 y = member->entity.position_y;
        fx32 min_y = y + member->entity.navigation_min_y;
        fx32 max_y = y + member->entity.navigation_max_y;
        for (surface = member->entity.navigation_cursor;
             !surface->bits.end && surface->vertices[0].x < member->entity.navigation_scan_limit; ++surface) {
            if (surface->bits.type == 3) {
                if (surface->attribute_bits.unknown_00 && !surface->attribute_bits.unknown_03) {
                    fx32 z = member->entity.position_z;
                    if (z >= surface->vertices[0].lower_z &&
                        z + member->entity.navigation_vertical_extent <= surface->vertices[0].z) {
                        fx32 surface_max_x = surface->vertices[2].x;
                        fx32 surface_min_y = surface->vertices[0].y;
                        fx32 surface_max_y = surface->vertices[2].y;
                        fx32 surface_min_x = surface->vertices[0].x;
                        if (surface_min_x <= min_x && surface_max_x >= max_x && surface_min_y <= min_y &&
                            surface_max_y >= max_y)
                            return 0;
                        if (((surface_min_x <= min_x && surface_max_x > min_x) ||
                             (surface_min_x < max_x && surface_max_x >= max_x)) &&
                            ((surface_min_y >= min_y && surface_min_y < max_y) ||
                             (surface_max_y > min_y && surface_max_y <= max_y)))
                            return 0;
                        if (((surface_min_y <= min_y && surface_max_y > min_y) ||
                             (surface_min_y < max_y && surface_max_y >= max_y)) &&
                            ((surface_min_x >= min_x && surface_min_x < max_x) ||
                             (surface_max_x > min_x && surface_max_x <= max_x)))
                            return 0;
                    }
                }
            }
        }
    }
    return 1;
}

void FieldPartyEntity_SetCollisionResponseChannels(FieldPartyEntity *member, int first, int second, int third,
                                                   int fourth, int fifth)
{
    if (first != -1)
        member->entity.collision_state_flag_bits.unknown_00 = first;
    if (second != -1) {
        member->entity.collision_state_flag_bits.unknown_02 = second;
        member->entity.collision_state_flag_bits.unknown_03 = second;
    }
    if (third != -1)
        member->entity.collision_state_flag_bits.unknown_01 = third;
    if (fourth != -1) {
        member->entity.collision_state_flag_bits.unknown_06 = fourth;
        member->entity.collision_state_flag_bits.unknown_07 = fourth;
    }
    if (fifth != -1) {
        member->entity.collision_state_flag_bits.unknown_04 = fifth;
        member->entity.collision_state_flag_bits.unknown_05 = fifth;
    }
}

void FieldPartyEntity_SetCollisionResponseChannelsMasked(FieldPartyEntity *member, u8 mask, u8 enabled)
{
    if (mask & 1)
        member->entity.collision_state_flag_bits.unknown_00 = enabled;
    if (mask & 2) {
        member->entity.collision_state_flag_bits.unknown_02 = enabled;
        member->entity.collision_state_flag_bits.unknown_03 = enabled;
    }
    if (mask & 4)
        member->entity.collision_state_flag_bits.unknown_01 = enabled;
    if (mask & 8) {
        member->entity.collision_state_flag_bits.unknown_06 = enabled;
        member->entity.collision_state_flag_bits.unknown_07 = enabled;
    }
    if (mask & 16) {
        member->entity.collision_state_flag_bits.unknown_04 = enabled;
        member->entity.collision_state_flag_bits.unknown_05 = enabled;
    }
}
}
