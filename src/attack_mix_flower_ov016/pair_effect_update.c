/* Advance paired Mix Flower effects through animation, convergence and reset. */
#include "pair_effects_internal.h"
void Overlay16PairEffect_Update(Overlay16PairEffect *effect, Overlay16PairController *controller)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(&effect->primary);
    BattleSceneObject_GetActiveModel(&effect->secondary);
    switch (effect->control.bits.phase) {
    case 2:
    case 3:
        Overlay16PairEffect_ResetIfRequested(effect);
        break;
    case 1:
        Overlay16PairEffect_ResetIfRequested(effect);
        if ((u8)model->flag_bits.unknown_09) {
            Overlay16PairEffect_ApplyKindAnimation(effect, 0);
            effect->control.bits.phase = 2;
        }
        break;
    case 4:
        switch (controller->phase) {
        case 0: {
            BattlePosition position;
            BattlePosition_StoreViewRelative(&position, effect->primary.x,
                (s16)(effect->primary.y - effect->primary.z),
                (s16)(effect->primary.effect_anchor_z + 16 * (256 - effect->primary.y)),
                effect->primary.flags.bits.use_raw_position,
                effect->primary.flags.bits.use_alternate_model);
            if (position.y + 268 <= 108) {
                Overlay16PairController_StartAnimation(controller);
                Overlay16PairEffect_Reset(effect);
                effect->control.bits.phase = 0;
            }
            break;
        }
        case 1:
        case 2:
        case 3:
            if (Overlay16PairController_IsWithinRadius(controller, &effect->primary)) {
                func_ov016_020c34d0(controller, effect->control.bits.kind >= 2);
                func_ov016_020c4b54(effect, controller);
                Overlay16PairEffect_Reset(effect);
                effect->control.bits.phase = 0;
            }
            break;
        case 4:
        case 5:
            if (!BattleSceneObject_IsAnimationChannelActive(&effect->primary, 3))
                Overlay16PairEffect_BeginFinish(effect);
            break;
        }
        break;
    case 5:
        Overlay16PairEffect_Reset(effect);
        effect->control.bits.phase = 0;
        break;
    case 6:
        Overlay16PairEffect_Reset(effect);
        effect->control.bits.phase = 0;
        break;
    }
    func_ov002_02071938(&effect->secondary, &effect->primary, 76);
}
