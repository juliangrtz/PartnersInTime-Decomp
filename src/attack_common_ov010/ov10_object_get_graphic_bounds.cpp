#include "attack_common_internal.h"

extern "C" {
void Overlay10Object_GetGraphicBounds(s16 *bounds, BattleSceneObject *object)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    if (!model) {
        for (int i = 0; i < 6; ++i)
            bounds[i] = 0;
    } else {
        func_ov010_020c1024(bounds, (const GameGraphicsResource *)model->resource,
                            object->animation_variant_offset);
    }
}
}
