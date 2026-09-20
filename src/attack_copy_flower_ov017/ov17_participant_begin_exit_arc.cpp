/* Copy Flower exit arc (overlay 17, 0x020C52F4-0x020C5434).
 * Moves right on a vertical arc; even formations play the animation backward. */

#include "participant_internal.h"
extern "C" int func_02009224(BattleModel *, int);

extern "C" void Overlay17Participant_BeginExitArc(Overlay17Participant *participant)
{
    BattleSceneObject_GetById(participant->state.target);
    int duration = Overlay17Attack_GetGroupTuningParameter2(participant->state.tuning);
    int y = participant->object.y;
    BattleSceneObject_StartAcceleratedMotionForDuration(&participant->object,
        3, 0, 0, 1, 0, duration, participant->state.height - participant->object.z, 1);
    BattleSceneObject_MoveBy(&participant->object, 2, 352 - participant->object.x,
        y - participant->object.y, 0, duration);
    Overlay17Participant_SetAnimation(participant, 0x400c, 0, -1);
    if (!(participant->state.bits.formation & 1)) {
        BattleModel *model = BattleSceneObject_GetActiveModel(&participant->object);
        int animation = model->get_animation_id();
        int length = func_02009224(model, -1);
        model->set_animation(animation, (s16)(length - 1));
        func_0200940c(model, (s16)-model->effect_scale);
    }
    participant->state.bits.phase = 7;
}
