#include "actor_internal.h"

int Overlay15Attack_SelectRandomEnemy(void)
{
    u16 targets[8];
    int count = 0;
    int i;
    for (i = 0; i < 6; ++i) {
        if (Overlay10Enemy_IsSelectable((u16)(i + 60))) {
            targets[count] = i + 60;
            ++count;
        }
    }
    if (count) {
        return targets[Random_NextModulo(count)];
    }
    return 0;
}

void Overlay15Attack_ApplyBadgeMotionAdjustments(void)
{
    u32 i;
    for (i = 0; i < 30; ++i) {
        data_ov015_020c5fdc[i][0] += data_ov015_020c6054[i][0];
        data_ov015_020c5fdc[i][1] += data_ov015_020c6054[i][1];
    }
}
