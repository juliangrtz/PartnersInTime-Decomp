#include <game/field_geometry.h>

extern const s16 data_ov000_020c0d08[16];

int FieldGeometry_GetDirectionVector(int direction, int length, int *x, int *y)
{
    if (direction >= 0 && direction <= 7) {
        int result;
        *x = length * data_ov000_020c0d08[direction * 2] / 4096;
        result = length * data_ov000_020c0d08[direction * 2 + 1] / 4096;
        *y = result;
        return result;
    }
    *x = 0;
    *y = 0;
    return 0;
}
