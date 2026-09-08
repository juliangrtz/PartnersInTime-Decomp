#include "participant_internal.h"

extern "C" {
int Overlay17Participant_GetTravelDuration(Overlay17Participant *participant, int height)
{
    int difference = 80 - height;
    int scale = Overlay17Attack_GetGroupTuningParameter4(participant->state.tuning);
    int duration = Overlay17Attack_GetGroupTuningParameter2(participant->state.tuning);
    return duration + (scale ? duration * difference / scale : 0);
}
}
