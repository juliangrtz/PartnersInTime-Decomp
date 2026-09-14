#include <game/field_area_motion.h>
#define LAYER(a, i) ((a)->layer_motion[i])
void FieldArea_StopLayerMotion(FieldAreaContext *area, int layer) {
    if (LAYER(area, layer).flags.axes.active_axes)
        LAYER(area, layer).flags.raw &= ~3;
}
