#include <game/field_party.h>

extern "C" {

int FieldPartyEntity_TestType3Volumes02(FieldPartyEntity *member)
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
                if (!surface->attribute_bits.unknown_00 && surface->attribute_bits.unknown_02) {
                    fx32 z = member->entity.position_z;
                    if (z <= surface->vertices[0].z &&
                        z + member->entity.navigation_vertical_extent >= surface->vertices[0].lower_z) {
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
}
