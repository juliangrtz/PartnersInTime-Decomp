/* Mix Flower participant animation and effect positioning when its kind changes.
 * Entering kinds 2 and above uses a short approach motion; subsequent changes
 * use the holding pose. Kinds below two restore the idle pose and orbit motion.
 */
#include "pair_effects_internal.h"
extern "C" {
void func_0200940c(BattleModel *, s16);
void func_ov002_0206e64c(BattleSceneObject *, int, int, int, int, int, int, int, int, u16);

void Overlay16Participant_ChangeEffectKind(Overlay16Participant *participant, int kind)
{
    BattleSceneObject *actor = participant->object;
    BattleModel *model = BattleSceneObject_GetActiveModel(actor);
    BattleSceneObject *effect;
    const s8 *offset;
    if (participant->effect->control.bits.kind <= 1 && kind >= 2) {
        if (model->get_animation_id() != 3) {
            BattleSceneObject_SetAnimation(actor, 2, -1);
            BattleSceneObject_SetAnimationMode01(actor, 0);
            func_0200940c(model, 256);
            offset = (const s8 *)&gOverlay16AttackTargetOffsets[participant->variant] + 5;
            effect = &participant->effect->primary;
            BattleSceneObject_AdjustPosition(effect, actor->x + offset[0] - effect->x,
                                             actor->y + 2 - effect->y, offset[1] - effect->z);
            BattleSceneObject_MoveTo(&participant->effect->primary, 2, actor->x + offset[2], actor->y,
                                     offset[3], offset[4]);
            participant->phase = 11;
        }
    } else if (kind >= 2) {
        if (model->get_animation_id() != 3) {
            BattleSceneObject_SetAnimation(actor, 3, -1);
            BattleSceneObject_SetAnimationMode01(actor, 1);
            func_0200940c(model, 256);
            offset = (const s8 *)&gOverlay16AttackTargetOffsets[participant->variant] + 5;
            effect = &participant->effect->primary;
            BattleSceneObject_AdjustPosition(effect, actor->x + offset[2] - effect->x,
                                             actor->y + 2 - effect->y, offset[3] - effect->z);
            participant->phase = 12;
        }
    } else {
        BattleSceneObject_SetAnimation(actor, 0, -1);
        BattleSceneObject_SetAnimationMode01(actor, 1);
        effect = &participant->effect->primary;
        offset = (const s8 *)&gOverlay16AttackTargetOffsets[participant->variant];
        BattleSceneObject_AdjustPosition(effect, actor->x + offset[0] - effect->x,
                                         actor->y + 2 - effect->y, offset[1] - effect->z);
        /* Preserve the unsigned halfword conversion of the last orbit parameter. */
        func_ov002_0206e64c(&participant->effect->primary, 2, offset[0], 0, offset[1], offset[2], 0,
                            offset[3], 0x4000, (u16)offset[4]);
        participant->phase = 12;
    }
    /* Refresh the model even when animation 3 kept the participant phase. */
    func_0200940c(model, 256);
    participant->timer2 = 0;
    Overlay16PairEffect_ChangeKind(participant->effect, kind);
}
}
