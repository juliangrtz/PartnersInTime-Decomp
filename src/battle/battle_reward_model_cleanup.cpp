/* Release the twelve reserved scene models before reward display. */
#include <game/battle_scene.h>

extern "C" {
#include <game/sprite_output.h>
}

/* Keep the deleting virtual call's null guard, as in the native helper. */
static inline void DeleteModel(BattleModel *model)
{
    if (model) {
        model->unknown_14();
    }
}

extern "C" void BattleRewardItems_ReleaseSceneModels(void)
{
    BattleSceneObject *object = BattleSceneObject_GetById(16);

    for (int i = 0; i < 12; ++i, ++object) {
        if (object->alternate_model) {
            object->alternate_model->stop();
            GameSpritePalette_Unlink((GameSpritePalette *)object->unk_0cc);
            /* Read the owning slot again after the virtual stop call. */
            if (object->alternate_model) {
                DeleteModel(object->alternate_model);
                object->alternate_model = 0;
            }
        }
        if (object->primary_model) {
            object->primary_model->stop();
            if (object->primary_model) {
                DeleteModel(object->primary_model);
                object->primary_model = 0;
            }
        }
    }
}
