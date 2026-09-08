#include "pair_effects_internal.h"

int Overlay16Participant_IsIdle(Overlay16Participant *participant)
{
    return participant->phase == 0;
}
