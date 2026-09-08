#include "pair_effects_internal.h"

void Overlay16PairEffect_Initialize(Overlay16PairEffect *effect, int primary_resource, int secondary_resource)
{
    if (primary_resource) {
        effect->primary.resource = (BattleSceneResource *)(gBattleContext + 54216 + 48 * primary_resource);
        effect->secondary.resource =
            (BattleSceneResource *)(gBattleContext + 54216 + 48 * secondary_resource);
        effect->secondary.animation_variant_offset = 0;
        effect->primary.animation_variant_offset = effect->secondary.animation_variant_offset;
        effect->secondary.render_state = 32767;
        effect->primary.render_state = effect->secondary.render_state;
    }
    effect->control.bits.phase = 0;
    effect->control.bits.unknown5 = 0;
    effect->control.bits.kind = 0;
}

void Overlay16PairEffect_DestroyModels(Overlay16PairEffect *effect)
{
    if (effect->primary.primary_model) {
        func_ov002_0206f1b8(effect->primary.primary_model);
        if (effect->primary.primary_model)
            effect->primary.primary_model->vtable->unknown_014(effect->primary.primary_model);
        effect->primary.primary_model = 0;
    }
    if (effect->secondary.alternate_model) {
        func_ov002_0206f384(effect->secondary.alternate_model);
        if (effect->secondary.alternate_model)
            effect->secondary.alternate_model->vtable->unknown_014(effect->secondary.alternate_model);
        effect->secondary.alternate_model = 0;
    }
}

void Overlay16PairEffect_Reset(Overlay16PairEffect *effect)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(&effect->primary);
    if (model)
        BattleModel_SetAlpha(model, 31, 0);
    Overlay16PairEffect_SetAnimation(effect, -1, 1);
    Overlay10Motion_StopAll(&effect->primary);
    Overlay16PairEffect_Initialize(effect, 0, 0);
}
