#include "participant_internal.h"

extern "C" {
void Overlay17Participant_ResumeArc(Overlay17Participant *participant)
{
    BattleMotionChannel *motion = BattleSceneObject_GetMotionChannel(&participant->object, 3);
    func_ov017_020c5434(participant, 0, motion->elapsed_q8 / 256);
    participant->state.bits.phase = 6;
}

void Overlay17Participant_BeginRecoveryAnimation(Overlay17Participant *participant)
{
    Overlay17Participant_SetAnimation(participant, 0x400a, 0, -1);
    participant->state.bits.phase = 12;
}

void Overlay17Participant_BeginExit(Overlay17Participant *participant)
{
    Overlay17Participant_SetAnimation(participant, participant->state.bits.formation >= 2 ? 259 : 258, 1, -1);
    BattleSceneObject_MoveBy(&participant->object, 2, 320 - participant->object.x, 0, 0, ((320 - participant->object.x) << 8) / 1024);
    participant->state.bits.phase = 13;
}
}
