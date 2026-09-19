#include "pair_effects_internal.h"

void Overlay16PairEffect_ResetIfRequested(Overlay16PairEffect *effect)
{
    if (data_ov002_020c0710->flag_1)
        Overlay16PairEffect_Reset(effect);
}
