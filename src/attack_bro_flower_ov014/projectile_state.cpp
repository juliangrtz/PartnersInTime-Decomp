/*
 * Bro Flower projectile state (overlay 14, 0x020C4130-0x020C4464).
 *
 * Finds a free projectile slot, reports when they are all idle, and maps the
 * projectile's level onto the animation it plays - the Bro and Ice variants
 * share this code and differ by level.
 */

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

#include <game/overlay014_projectile.h>
extern "C" {
#include <game/battle_effect.h>
extern void BattleSceneObject_SetAnimationMode01(BattleSceneObject *, int);
void Overlay14Projectile_SetLevel(Overlay14Projectile *projectile, int level)
{
    u32 previous = projectile->flags.level;
    if ((previous == 2 && level == 2) || (previous == 3 && level == 3) || (previous == 1 && level == 1)) {
        Overlay14Projectile_SelectAnimation(projectile, 0);
        projectile->phase = 2;
    } else {
        if (previous != 2 && level == 2) {
            BattleSceneObject_SetAnimation(&projectile->object, 3, -1);
            projectile->object.flags.bits.unk_20_23 = 3;
            projectile->phase = 1;
        }
        if (projectile->flags.level != 3 && level == 3) {
            BattleSceneObject_SetAnimation(&projectile->object, 4, -1);
            projectile->object.flags.bits.unk_20_23 = 4;
            projectile->phase = 1;
        }
        if (level == 1) {
            BattleSceneObject_SetAnimation(&projectile->object, 1, -1);
            projectile->object.flags.bits.unk_20_23 = 2;
            projectile->phase = 1;
        }
        BattleSceneObject_SetAnimationMode01(&projectile->object, 0);
        projectile->flags.level = (u8)level;
    }
}

void Overlay14Projectile_SelectAnimation(Overlay14Projectile *projectile, int active)
{
    if (active) {
        switch (projectile->flags.level) {
        case 1:
            BattleSceneObject_SetAnimation(&projectile->object, 6, -1);
            projectile->object.flags.bits.unk_20_23 = 2;
            break;
        case 2:
            BattleSceneObject_SetAnimation(&projectile->object, 5, -1);
            projectile->object.flags.bits.unk_20_23 = 3;
            break;
        case 3:
            BattleSceneObject_SetAnimation(&projectile->object, 7, -1);
            projectile->object.flags.bits.unk_20_23 = 4;
            break;
        }
    } else {
        switch (projectile->flags.level) {
        case 1:
            BattleSceneObject_SetAnimation(&projectile->object, 1, -1);
            projectile->object.flags.bits.unk_20_23 = 2;
            break;
        case 2:
            BattleSceneObject_SetAnimation(&projectile->object, 0, -1);
            projectile->object.flags.bits.unk_20_23 = 3;
            break;
        case 3:
            BattleSceneObject_SetAnimation(&projectile->object, 2, -1);
            projectile->object.flags.bits.unk_20_23 = 4;
            break;
        }
    }
    BattleSceneObject_SetAnimationMode01(&projectile->object, 1);
}
}
