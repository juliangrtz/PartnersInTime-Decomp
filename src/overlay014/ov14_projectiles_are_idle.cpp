#include "flower_internal.h"

extern "C" {
int Overlay14Attack_FindFreeProjectile()
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    int i;
    for (i = 0; i < 32; ++i, work = (Overlay14Work *)((u8 *)work + 268)) {
        if (!work->projectiles[0].phase)
            return i;
    }
    return -1;
}

int Overlay14Projectiles_AreIdle(Overlay14Projectile *projectiles, int count)
{
    for (int i = 0; i < count; ++i) {
        if (projectiles[i].phase)
            return 0;
        if (BattleSceneObject_IsAnimationChannelActive(&projectiles[i].object, 3))
            return 0;
        if (BattleSceneObject_IsAnimationChannelActive(&projectiles[i].object, 2))
            return 0;
    }
    return 1;
}
}
