#include <game/overlay014_projectile.h>
extern "C" {
#include <game/battle_effect.h>
extern u8 *data_ov002_020c0710;
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
