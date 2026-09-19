/*
 * Mix Flower pair effect reset (overlay 16, 0x020C4B18-0x020C4B54).
 *
 * Resets a paired effect if a reset has been requested for it.
 */

#include "pair_effects_internal.h"

void Overlay16PairEffect_ResetIfRequested(Overlay16PairEffect *effect)
{
    if (data_ov002_020c0710->flag_1)
        Overlay16PairEffect_Reset(effect);
}
