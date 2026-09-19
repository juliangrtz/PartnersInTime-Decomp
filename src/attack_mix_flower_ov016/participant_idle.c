/*
 * Mix Flower participant idle test (overlay 16, 0x020C5C38-0x020C5C50).
 *
 * Whether a participant has finished its part of the attack.
 */

#include "pair_effects_internal.h"

int Overlay16Participant_IsIdle(Overlay16Participant *participant)
{
    return participant->phase == 0;
}
