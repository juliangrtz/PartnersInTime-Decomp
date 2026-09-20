/* Body-volume overlap (overlay 0, 0x020AE5FC).
 * X/Y require positive overlap; the height interval also includes touching. */

#include <game/field_entity.h>

int FieldEntity_GetBodyOverlapMask(FieldRuntimeEntity *entity, FieldRuntimeEntity *other)
{
    int other_x = other->position_x;
    int entity_x = entity->position_x;
    int overlap = entity_x + entity->body_max_x - (other_x + other->body_min_x);
    int opposite = other_x + other->body_max_x - (entity_x + entity->body_min_x);
    int mask = 0;
    if (overlap > opposite)
        overlap = opposite;
    if (overlap > 0)
        mask = 1;
    {
        int other_y = other->position_y;
        int entity_y = entity->position_y;
        overlap = entity_y + entity->body_max_y - (other_y + other->body_min_y);
        opposite = other_y + other->body_max_y - (entity_y + entity->body_min_y);
        if (overlap > opposite)
            overlap = opposite;
        if (overlap > 0)
            mask |= 2;
    }
    {
        int other_z = other->position_z;
        int entity_z = entity->position_z;
        overlap = entity_z + entity->body_vertical_extent - other_z;
        opposite = other_z + other->body_vertical_extent - entity_z;
        if (overlap > opposite)
            overlap = opposite;
        if (overlap >= 0)
            mask |= 4;
    }
    return mask;
}
