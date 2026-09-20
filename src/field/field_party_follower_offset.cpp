/* Compute a point behind the member, shortening the offset on sloped ground. */
#include <game/field_party.h>
#include <game/field_geometry.h>
extern "C" fx32 FieldParty_GetFollowerOffset(FieldPartyController *party, FieldPartyEntity *member,
                                             fx32 distance, fx32 *x, fx32 *y)
{
    fx32 adjusted_distance = distance;
    const FieldNavigationSurface *surface = member->entity.ground_surface;
    if (surface && !surface->bits.type) {
        if (surface->vertices[0].z > member->entity.position_z ||
            surface->vertices[1].z > member->entity.position_z ||
            surface->vertices[2].z > member->entity.position_z)
            adjusted_distance =
                (fx32)(((s64)adjusted_distance *
                            FieldParty_GetSlopeDistanceScale(
                                member, member->entity.base_state_flag_bits.facing_direction,
                                surface) +
                        2048) >>
                       12);
    }
    return FieldGeometry_GetDirectionVector(
        member->entity.base_state_flag_bits.facing_direction ^ 4, adjusted_distance, x, y);
}
