/*
 * Camera origin (overlay 0, 0x02073860-0x02073880).
 *
 * Offsets the origin the camera position is measured from.
 */

#include <game/field_area.h>
void FieldArea_OffsetCameraOrigin(FieldAreaContext *area, fx32 x, fx32 y) {
    area->unknown_2424[0] += x;
    area->unknown_2424[1] += y;
}
