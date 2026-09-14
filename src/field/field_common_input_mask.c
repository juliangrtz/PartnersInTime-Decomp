#include <game/field_area.h>
int FieldArea_GetCommonInputMask(FieldAreaContext *area) {
    int mask = 65535, i;
    for (i = 0; i < 2; i++)
        mask &= area->input_masks[i];
    return mask;
}
