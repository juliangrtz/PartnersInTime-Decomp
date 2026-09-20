/* Bro/Ice Flower projectile phases, overlay 14, 0x020C4464-0x020C4734. */
#include "flower_internal.h"
extern "C" {
void func_ov014_020c3dac(Overlay14Projectile *);
void func_ov014_020c3aa4(Overlay14Projectile *);
static inline u8 AnimationFinished(BattleModel *model)
{
    return model->flag_bits.unknown_09;
}

void Overlay14Projectile_Update(Overlay14Projectile *projectile)
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    BattleModel *model = BattleSceneObject_GetActiveModel(&projectile->object);
    switch (projectile->phase) {
    case 1:
        if (AnimationFinished(model)) {
            Overlay14Projectile_SelectAnimation(projectile, 0);
            projectile->phase = 2;
        }
        break;
    case 2:
        Overlay14Projectile_UpdateStopRequest(projectile);
        break;
    /* Finish the initial flight before choosing the bounce/return phase. */
    case 3:
    case 5:
        if (!BattleSceneObject_IsAnimationChannelActive(&projectile->object, 2)) {
            func_ov014_020c3dac(projectile);
            Overlay14Projectile_StartBounce(projectile);
            if (projectile->flags.level <= 1)
                projectile->phase = 8;
        }
        break;
    case 4:
    case 6:
        if (!BattleSceneObject_IsAnimationChannelActive(&projectile->object, 3))
            Overlay14Projectile_StartBounce(projectile);
        if (!BattleSceneObject_IsAnimationChannelActive(&projectile->object, 2)) {
            if (projectile->phase == 4)
                func_ov014_020c3aa4(projectile);
            Overlay14Projectile_Reset(projectile);
            projectile->phase = 0;
        }
        break;
    case 8:
        if (!BattleSceneObject_IsAnimationChannelActive(&projectile->object, 3)) {
            BattleSceneObject_StopMotionChannel(&projectile->object, 2, 0);
            if (work->variant) {
                BattleSpriteEffect_SpawnRelative(535, &projectile->object, 0, 0, 0, 256);
                BattleModelEffect_SpawnRelative(409, &projectile->object, 0, 0, 0, 0, 256);
            } else {
                BattleSpriteEffect_SpawnRelative(287, &projectile->object, 0, 0, 0, 256);
                BattleModelEffect_SpawnRelative(409, &projectile->object, 0, 0, 0, 0, 256);
            }
            Overlay14Projectile_BeginHide(projectile);
            BattleSound_Play(0, 0, 0, 0);
        }
        break;
    case 7:
        if (!BattleSceneObject_IsAnimationChannelActive(&projectile->object, 2)) {
            func_ov014_020c3aa4(projectile);
            Overlay14Projectile_Reset(projectile);
            projectile->phase = 0;
        }
        break;
    case 9:
        if (!BattleSceneObject_IsAnimationChannelActive(&projectile->object, 0)) {
            Overlay14Projectile_Reset(projectile);
            projectile->phase = 0;
        }
        break;
    }
}
}
