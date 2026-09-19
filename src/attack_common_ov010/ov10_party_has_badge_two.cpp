/*
 * Badge check (overlay 10, 0x020C13BC-0x020C1410).
 *
 * Whether a party member has the badge whose effect the attack overlays query.
 */

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
