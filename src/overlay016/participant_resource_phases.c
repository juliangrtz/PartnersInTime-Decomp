#include "pair_effects_internal.h"

void Overlay16Participant_StartPhaseOne(Overlay16Participant *participant)
{
    BattleSceneObject *object = participant->object;
    BattleEntity_BindResource(object->actor_id, (u16)((participant->variant & 1) + 56));
    BattleSceneObject_SetAnimationFromComponent(object, 0, 0, participant->component);
    func_ov002_02071750(object, 0);
    BattleSound_Play(41, 0, 0, 0);
    participant->phase = 1;
}

void Overlay16Participant_StartPhaseThree(Overlay16Participant *participant)
{
    BattleSceneObject *object = participant->object;
    BattleModel *model;
    BattleEntity_BindResource(object->actor_id, (u16)(participant->variant + 58));
    BattleSceneObject_SetAnimationFromComponent(object, 0, 0, participant->component);
    model = BattleSceneObject_GetActiveModel(object);
    model->flags |= BATTLE_MODEL_FLAG_10;
    model->flags &= ~BATTLE_MODEL_FLAG_11;
    func_ov002_02071750(object, 0);
    participant->phase = 3;
}
