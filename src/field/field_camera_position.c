/*
 * Camera position (overlay 0, 0x020736A4-0x02073740).
 *
 * Places the camera directly or centres it on an entity.
 */

#include <game/field_area.h>
#include <game/field_entity.h>
void FieldArea_SetCameraPosition(FieldAreaContext *area, fx32 x, fx32 y) {
    int limit = area->unknown_243c[2];
    if (x < 0)
        x = 0;
    if (x > limit)
        x = limit;
    limit = area->unknown_243c[3];
    if (y < 0)
        y = 0;
    if (y > limit)
        y = limit;
    area->camera_x = x;
    area->camera_y = y;
}

void FieldArea_CenterCameraOnEntity(FieldAreaContext *area, FieldRuntimeEntity *entity, fx32 x,
                                    fx32 y) {
    FieldArea_SetCameraPosition(area, entity->position_x + x - 0x80000,
                                y + (entity->position_y - entity->position_z) - 0x88000);
    area->unknown_2468[0] = entity->position_x;
    area->unknown_2468[1] = entity->position_y - entity->position_z;
}
