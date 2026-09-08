#include "participant_internal.h"

extern "C" {
void Overlay17Participant_ForwardVariantControl(Overlay17Participant *participant, int adult_value, int baby_value)
{
    int baby = participant->state.bits.formation == 2 || participant->state.bits.formation == 3;
    func_ov002_02072004(&participant->object, 0, baby ? baby_value : adult_value, 0, 0);
}

void Overlay17Participant_ReturnOthers(Overlay17Participant *participant, int return_others)
{
    Overlay17Participant_SetAnimation(participant, participant->state.bits.formation >= 2 ? 259 : 258, 1, -1);
    BattleSceneObject_GetActiveModel(&participant->object)->flags |= 0x200;
    BattleModel *model = BattleSceneObject_GetActiveModel(&participant->object);
    model->flags &= ~0x400;
    model->flags &= ~0x800;
    if (return_others)
        func_ov017_020c30e4(participant->state.bits.formation, participant->state.bits.index);
    participant->state.bits.phase = 0;
}
}
