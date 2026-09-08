#include "attack_common_internal.h"

extern "C" {
int Overlay10Party_HasBadgeTwo()
{
    Overlay10BadgeView *save =
        (Overlay10BadgeView *)(gSaveData +
                               36 * data_ov010_020c2aa4[data_ov002_020c0710->user->party.formation_index]);
    return (save->badge | 0x3000) == 0x3002;
}
}
