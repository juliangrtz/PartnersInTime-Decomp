#include "flower_internal.h"

extern "C" {
void Overlay14Attack_InitializeEnemyEffects(int resource)
{
    Overlay14Work *work;
    BattleSceneResource *load;
    int i;
    BattleSceneObject *group, *object;
    int j;
    work = (Overlay14Work *)data_ov002_020c0710;
    load = (BattleSceneResource *)BattleObjectData_GetLoadState(resource);
    for (i = 0; i < 6; ++i) {
        object = work->effects[i];
        for (j = 0; j < 3; ++j, ++object) {
            object->resource = load;
            object->animation_variant_offset = 0;
            object->render_state = 0x7fff;
        }
    }
}

void Overlay14Attack_ReleaseEnemyEffects()
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    for (int i = 0; i < 6; ++i) {
        BattleSceneObject *object = work->effects[i];
        for (int j = 0; j < 3; ++j, ++object) {
            if (object->primary_model) {
                func_ov002_0206f1b8(object->primary_model);
                if (object->primary_model)
                    object->primary_model->unknown_14();
                object->primary_model = 0;
            }
        }
    }
}
}
