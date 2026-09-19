#include <game/overlay014_projectile.h>
extern "C" {
#include <game/battle_effect.h>
extern u8 *data_ov002_020c0710;
extern s8 data_ov014_020c6180[][4], data_ov014_020c6204[];
extern void BattleSceneObject_SetAnimationMode01(BattleSceneObject *, int);
extern void func_0200940c(BattleModel *, s16);
extern void Overlay10Motion_StopAll(BattleSceneObject *);
extern void func_ov002_0206e64c(BattleSceneObject *, int, int, int, int, int, int, int, int, u16);
int Overlay14Participant_IsIdle(Overlay14Participant *participant)
{
    return participant->phase == 0;
}

void Overlay14Participant_SetProjectileMode(Overlay14Participant *participant, int mode)
{
    BattleSceneObject *actor = participant->actor->actor.scene_object;
    BattleModel *model = BattleSceneObject_GetActiveModel(actor);
    BattleSceneObject *projectile;
    const s8 *offset;
    if (participant->projectile->flags.level <= 1 && mode == 2) {
        if (model->get_animation_id() != 3) {
            BattleSceneObject_SetAnimation(actor, 2, -1);
            BattleSceneObject_SetAnimationMode01(actor, 0);
            func_0200940c(model, 256);
            offset = data_ov014_020c6204 + 10 * participant->actor->formation_index + 5;
            projectile = &participant->projectile->object;
            BattleSceneObject_AdjustPosition(projectile, actor->x + offset[0] - projectile->x,
                                             actor->y + 2 - projectile->y, offset[1] - projectile->z);
            BattleSceneObject_MoveTo(&participant->projectile->object, 2, actor->x + offset[2], actor->y,
                                     offset[3], offset[4]);
            participant->phase = 4;
        }
    } else if (mode == 2) {
        if (model->get_animation_id() != 3) {
            BattleSceneObject_SetAnimation(actor, 3, -1);
            BattleSceneObject_SetAnimationMode01(actor, 1);
            func_0200940c(model, 256);
            offset = data_ov014_020c6204 + 10 * participant->actor->formation_index + 5;
            projectile = &participant->projectile->object;
            BattleSceneObject_AdjustPosition(projectile, actor->x + offset[2] - projectile->x,
                                             actor->y + 2 - projectile->y, offset[3] - projectile->z);
            participant->phase = 3;
        }
    } else if (model->get_animation_id()) {
        BattleSceneObject_SetAnimation(actor, 0, -1);
        BattleSceneObject_SetAnimationMode01(actor, 1);
        projectile = &participant->projectile->object;
        offset = data_ov014_020c6204 + 10 * participant->actor->formation_index;
        BattleSceneObject_AdjustPosition(projectile, actor->x + offset[0] - projectile->x,
                                         actor->y + 2 - projectile->y, offset[1] - projectile->z);
        func_ov002_0206e64c(&participant->projectile->object, 2, offset[0], 0, offset[1], offset[2], 0,
                            offset[3], 0x4000, (u16)offset[4]);
        participant->phase = 3;
    }
    func_0200940c(model, 256);
    participant->timer = 0;
    Overlay14Projectile_SetLevel(participant->projectile, mode);
}

void Overlay14Participant_PositionProjectile(Overlay14Participant *participant)
{
    BattleSceneObject *actor = participant->actor->actor.scene_object;
    const s8 *offset = data_ov014_020c6204 + 10 * participant->actor->formation_index + 5;
    Overlay10Motion_StopAll(&participant->projectile->object);
    BattleSceneObject *projectile = &participant->projectile->object;
    BattleSceneObject_AdjustPosition(projectile, actor->x + offset[2] - projectile->x,
                                     actor->y + 2 - projectile->y, offset[3] - projectile->z);
}
}
