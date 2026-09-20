/* Adjust following distance using the absolute height difference between two
 * samples on a triangular navigation surface. Direction selects the sample
 * offset and coefficient; other surface types retain the unit Q12 scale.
 */
#include <game/field_party.h>
#include <game/field_navigation.h>
extern "C" {
extern const s16 data_ov000_020c0be0[], data_ov000_020c0b90[], data_ov000_020c0b80[];
fx32 FieldParty_GetSlopeDistanceScale(FieldPartyEntity *member, int direction,
                                      const FieldNavigationSurface *surface)
{
    fx32 result = 4096;
    if (surface && !surface->bits.type) {
        fx32 x = (surface->vertices[0].x + surface->vertices[1].x + surface->vertices[2].x) / 3;
        fx32 y = (surface->vertices[0].y + surface->vertices[1].y + surface->vertices[2].y) / 3;
        fx32 previous_height, previous_lower, next_height, next_lower;
        func_ov000_020bdc88(surface, x, y, &previous_height, &previous_lower, 1);
        func_ov000_020bdc88(surface, x + data_ov000_020c0be0[2 * direction],
                            y + data_ov000_020c0be0[2 * direction + 1], &next_height, &next_lower,
                            1);
        int scale = surface->bits.slope_axis == 1 ? data_ov000_020c0b90[direction]
                                                  : data_ov000_020c0b80[direction];
        fx32 difference = next_height - previous_height;
        if (difference < 0)
            difference = -difference;
        result -= (fx32)(((s64)difference * scale + 2048) >> 12);
    }
    return result;
}
}
