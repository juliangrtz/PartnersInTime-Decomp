/* Bring the lead actor and optional support into position for a Shell attack.
 * The landing effect runs once while the support animation catches up. Once
 * both are ready, install the attack updater and capture the projectile bounds.
 */
#include "shell_internal.h"
#include "../attack_common_ov010/attack_work.h"
/* Bit 2 of the shared work flags records the one-time landing effect. */
struct ShellEntryFlags {
    u8 prefix[16];
    u8 unknown0 : 2, landing_effect : 1, unknown3 : 5;
};
extern "C" {
extern s8 data_ov011_020c5f28[][2], data_ov011_020c5f29[][2];
void func_ov011_020c54b4(Overlay10ActionActor *);
void func_ov002_020722ac(Overlay10ActionActor *, void (*)(Overlay10ActionActor *));

static inline void SetModelOffset(BattleSceneObject *object, s16 x, s16 y)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    model->animation_offset_x = x;
    model->animation_offset_y = y;
}

void Overlay11Attack_UpdateEntry(BattlePartyActor *user)
{
    Overlay11AttackWork *work = data_ov002_020c0710;
    Overlay10Work *common = (Overlay10Work *)work;
    ShellEntryFlags *flags = (ShellEntryFlags *)work;
    Overlay11ControllerConfig *config = work->controllers[0].config;
    BattleSceneObject *jumper = BattleSceneObject_GetById(40);
    BattleSceneObject *actor = work->controllers[0].actor->actor.scene_object;
    *(u16 *)(gBattleContext + 52096) |= 8;
    switch (common->phase) {
    case 0: {
        int duration = BattleSceneObject_StartVerticalArc(jumper, 2, 1152, 20, 0);
        BattleSceneObject_MoveTo(jumper, 1, config->home_x + 12, config->home_y, 0, duration);
        if (work->support.object) {
            BattleEntity_BindResource(actor->actor_id, work->controllers[0].actor_resource_id);
            BattleSceneObject_SetAnimation(actor, 39, -1);
            BattleModel *model = BattleSceneObject_GetActiveModel(actor);
            model->flags = (model->flags & ~0xf000) | 0x1000;
            work->support.duration = duration;
            common->phase = 1;
        } else common->phase = 3;
        break;
    }
    case 1:
        if ((u8)BattleSceneObject_GetActiveModel(actor)->flag_bits.unknown_09) {
            int side = work->support.formation & 1;
            BattleEntity_BindResource(work->support.object->actor_id, work->controllers[0].actor_resource_id);
            BattleSceneObject *support = work->support.object;
            BattleSceneObject_AdjustPosition(support,
                actor->x - data_ov011_020c5f28[side][0] - support->x,
                actor->y - support->y,
                actor->z + data_ov011_020c5f29[side][0] - support->z);
            Overlay11Support_StartIntercept(&work->support, work->support.duration, 0, 0, 41);
            /* These writable table entries are reloaded after StartIntercept. */
            SetModelOffset(work->support.object,
                data_ov011_020c5f28[side][0], data_ov011_020c5f29[side][0]);
            BattleEntity_BindResource(actor->actor_id, work->controllers[0].actor_resource_id);
            BattleSceneObject_SetAnimation(actor, 40, -1);
            BattleModel *model = BattleSceneObject_GetActiveModel(actor);
            model->flags &= ~0xf000;
            common->phase = 2;
        }
        break;
    case 2:
        Overlay11Support_Update(&work->support, &work->projectile);
        if (work->support.state == 1) common->phase = 3;
        break;
    case 3:
        if (!BattleSceneObject_IsAnimationChannelActive(jumper, 2)) {
            common->phase = 0;
            flags->landing_effect = 0;
            work->controllers[0].flag_5 = 1;
            BattleSceneObject_SetAnimation(work->controllers[0].actor->actor.scene_object, 0, -1);
            BattleCollision_GetBounds((BattleCollisionBounds *)&work->projectile.collision_x, 40, 0, 0);
            func_ov002_020722ac((Overlay10ActionActor *)user, func_ov011_020c54b4);
        }
        return;
    }
    if (!(u8)flags->landing_effect && !BattleSceneObject_IsAnimationChannelActive(jumper, 2)) {
        BattleModelEffect_SpawnRelative(415, jumper, 0, 0, 0, 0, 256);
        flags->landing_effect = 1;
    }
}
}
