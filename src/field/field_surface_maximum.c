/* Navigation surface height bound (overlay 0, 0x020AA7E4-0x020AA830).
 * Type-zero surfaces use the highest of their three defining vertices. Other
 * types use the first height. The entity argument belongs to the shared calling
 * convention and is unused here; a fourth vertex is never consulted.
 */

#include <game/field_navigation.h>
fx32 FieldNavigation_GetMaximumHeight(const FieldRuntimeEntity *entity, const FieldNavigationSurface *surface)
{
    if (surface->bits.type == 0) {
        fx32 second = surface->vertices[1].z;
        fx32 first = surface->vertices[0].z;
        if (first >= second) {
            fx32 third = surface->vertices[2].z;
            if (first < third) first = third;
            return first;
        } else {
            fx32 third = surface->vertices[2].z;
            if (second < third) second = third;
            return second;
        }
    }
    return surface->vertices[0].z;
}
