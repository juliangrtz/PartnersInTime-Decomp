/*
 * Bro Flower duration extension (overlay 14, 0x020C2AE0-0x020C2AF8).
 *
 * Extends the attack's remaining time, which a successful input does.
 */

#include "flower_internal.h"

extern "C" {
void Overlay14Attack_ExtendDuration()
{
    data_ov014_020c6124 += 120;
}
}
