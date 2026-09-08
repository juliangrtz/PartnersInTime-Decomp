#include "pair_effects_internal.h"

void Overlay16Participant_StartPhaseEight(Overlay16Participant *participant)
{
    BattleSceneObject *object = participant->object;
    BattleEntity_BindResource(object->actor_id, (u16)(participant->variant + 58));
    BattleSceneObject_SetAnimationFromComponent(object, 1, 0, participant->component);
    func_ov002_02071750(object, 1);
    if (participant->variant <= 1) {
        BattleModel *model = BattleSceneObject_GetActiveModel(object);
        model->flags |= BATTLE_MODEL_FLAG_10;
        model->flags &= ~BATTLE_MODEL_FLAG_11;
    }
    BattleSound_Play(106, 0, 0, 0);
    participant->timer = 0;
    participant->timer2 = 32;
    participant->phase = 8;
}
