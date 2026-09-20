/* Move a Mix Flower participant offscreen and enter the exit-wait phase.
 * Each side keeps its own facing flag; the object's Y is reloaded after the
 * resource and animation calls, which can replace its rendering state.
 */
#include "pair_effects_internal.h"

void Overlay16Participant_BeginExit(Overlay16Participant *participant)
{
    Overlay16AttackWork *work;
    BattleSceneObject *object = participant->object;
    BattleModel *model;
    work = data_ov002_020c0710;
    BattleEntity_BindResource(object->actor_id,
        (u16)((participant->variant & 1) ? 57 : 56));
    BattleSceneObject_SetAnimationFromComponent(object,
        participant->variant <= 1 ? 2 : 3, 0, participant->component);
    model = BattleSceneObject_GetActiveModel(object);
    if (participant->variant <= 1) {
        model->flags &= ~BATTLE_MODEL_FLAG_10;
        model->flags &= ~BATTLE_MODEL_FLAG_11;
        BattleSceneObject_MoveTo(object, 3, work->entry_offset_x + 288,
            participant->object->y, 0, 8);
    } else {
        model->flags |= BATTLE_MODEL_FLAG_10;
        model->flags &= ~BATTLE_MODEL_FLAG_11;
        BattleSceneObject_MoveTo(object, 3, work->entry_offset_x - 32,
            participant->object->y, 0, 8);
    }
    BattleSound_Play(61, 0, 0, 0);
    participant->phase = 9;
}
