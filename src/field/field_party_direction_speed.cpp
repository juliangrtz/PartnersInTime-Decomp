/* Choose the party's directional speed, including airborne and slope adjustments. */
#include <game/field_party.h>
#include <game/field_geometry.h>
extern "C" {
fx32 func_ov000_020bb2dc(FieldPartyEntity *, int, const FieldNavigationSurface *);
fx32 FieldPartyEntity_GetDirectionVector(FieldPartyEntity *member, int direction, fx32 *x, fx32 *y)
{
    fx32 speed = member->entity.movement_speed;
    if (member->unknown_594.unknown_00) {
        if (!member->entity.relative_height || (!member->entity.unknown_134 && direction != -1)) {
            const FieldNavigationSurface *surface = member->entity.ground_surface;
            if (surface && !surface->bits.type) {
                fx32 z = member->entity.position_z;
                if (surface->vertices[0].z > z || surface->vertices[1].z > z ||
                    surface->vertices[2].z > z) {
                    speed = (fx32)(((s64)speed * func_ov000_020bb2dc(member, direction, surface) +
                                    2048) >>
                                   12);
                }
            }
        } else if (member->entity.field_state_flag_bits.vertical_motion_active) {
            speed = member->entity.unknown_134;
        }
    }
    member->entity.party_direction_speed = speed;
    return FieldGeometry_GetDirectionVector(direction, speed, x, y);
}
}
