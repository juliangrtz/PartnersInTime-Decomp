/*
 * Copy Flower travel timing (overlay 17, 0x020C2B44-0x020C2B8C).
 *
 * How long a participant takes to reach its destination.
 */

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
