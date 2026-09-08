#include "pair_effects_internal.h"

int Overlay16PairController_GetMotionStep(Overlay16PairController *controller)
{
    int i;
    int radius = controller->radius;
    int value = gOverlay16MotionTable[19].y;
    i = 0;
    do {
        if (radius <= gOverlay16MotionTable[i].x)
            return data_ov016_020c7400[i][0];
    } while ((unsigned)++i < 20);
    return value;
}

int Overlay16PairController_IsWithinRadius(Overlay16PairController *controller, BattleSceneObject *object)
{
    s16 first[3], second[3];
    BattleSceneObject *primary = controller->primary;
    BattlePosition_StoreViewRelative((BattlePosition *)first, primary->x, (s16)(primary->y - primary->z),
                                     (s16)(primary->effect_anchor_z + 16 * (256 - primary->y)),
                                     primary->flags.bits.use_raw_position,
                                     primary->flags.bits.use_alternate_model);
    BattlePosition_StoreViewRelative((BattlePosition *)second, object->x, (s16)(object->y - object->z),
                                     (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
                                     object->flags.bits.use_raw_position,
                                     object->flags.bits.use_alternate_model);
    {
        int dx = first[0] - second[0];
        int dy = first[1] - second[1];
        return FX_Sqrt((dx * dx + dy * dy) << 12) < (controller->radius << 12);
    }
}
