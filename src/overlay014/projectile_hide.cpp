#include <game/overlay014_projectile.h>
extern "C" {
#include <game/battle_effect.h>
extern u8 *data_ov002_020c0710;
extern int func_ov002_0206f0bc(BattleSceneObject *, int, int, int, int, int);
extern int func_ov002_0206e7e4(BattleSceneObject *, int, int, int, int, int, int, int, int, int, int);
void Overlay14Projectile_BeginHide(Overlay14Projectile *projectile)
{
    func_ov002_0206f0bc(&projectile->object, 0, 31, 0, 8, 0);
    func_ov002_0206e7e4(&projectile->object, 1, 256, 256, 0, 64, 512, 0, 8, 0, 0);
    projectile->phase = 9;
}

void Overlay14Projectile_StartBounce(Overlay14Projectile *projectile)
{
    BattleSceneObject_StartVerticalMotionTo(&projectile->object, 3, 2048, -512, 0, 1);
    BattleSound_Play(0, 0, 0, 0);
}
}
