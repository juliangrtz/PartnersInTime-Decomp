#include "participant_internal.h"

extern "C" {
void Overlay17Participant_BeginEntry(Overlay17Participant *participant, int formation)
{
    u16 selected = formation;
    participant->state.tuning = data_ov002_020c0710->tuning;
    participant->state.counter = 0;
    participant->state.bounce = 0;
    participant->state.duration = 0;
    participant->state.target = 0;
    participant->state.bits.active = 0;
    participant->state.bits.formation = selected;
    participant->state.bits.input = 0;
    BattleSound_Play(0, 0, 0, 0);
    int duration = Overlay17Attack_GetGroupTuningParameter1(participant->state.tuning);
    BattleSceneObject_AdjustPosition(&participant->object, -48 - participant->object.x, 132 - participant->object.y, -participant->object.z);
    BattleSceneObject_MoveTo(&participant->object, 2, 64, 132, 0, duration);
    Overlay17Participant_SetAnimation(participant, participant->state.bits.formation >= 2 ? 259 : 258, 1, -1);
    participant->state.bits.phase = 1;
    Overlay17Participant_AppendToList(participant);
}
}
