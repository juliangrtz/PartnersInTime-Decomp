/* Update the Shell attack's support character: catch the shell, respond to
 * its button, time the temporary boost, and return to the finishing actor.
 * Animation, sound and effect state are reloaded where helper calls can change
 * them. The cached model is the one selected before this update's state switch.
 */
#include "shell_internal.h"
struct ShellFinishView {
    u8 prefix[16];
    u8 unknown0 : 1, finished : 1, unknown2 : 6;
};
extern "C" {
extern s16 data_ov011_020c5f30[], data_ov011_020c5f38[], data_ov011_020c5f3c[];
extern int data_ov011_020c5f4c[], data_ov011_020c5f5c[];
int BattleMotion_StartFollower(BattleSceneObject *, BattleSceneObject *, int);

void Overlay11Support_Update(Overlay11Support *support, Overlay11Projectile *projectile)
{
    Overlay11AttackWork *work;
    BattleSceneObject *shell;
    BattleModel *model;
    BattleSceneObject *object = support->object;
    if (object) {
        work = data_ov002_020c0710;
        shell = projectile->object;
        model = BattleSceneObject_GetActiveModel(object);
        u16 buttons = *(u16 *)(gBattleContext + 260) & 0xc03;
        switch (support->state) {
        case 1: // Holding the shell; wait for the support button.
            if (buttons & support->button) {
                BattleSceneObject_SetAnimation(object, model->get_animation_id() + 2, -1);
                BattleSceneObject_SetAnimation(shell, BattleSceneObject_GetActiveModel(shell)->get_animation_id() + 2, -1);
                func_0200940c(BattleSceneObject_GetActiveModel(shell), 256);
                func_0200940c(model, 256);
                Overlay11Effects_SpawnAttached(data_ov011_020c5f4c[work->variant], shell);
                BattleModelEffect_SpawnRelative(data_ov011_020c5f5c[work->variant], 0, shell, 0, 0, 0, 256);
                support->effect_handle = BattleSound_Play(220, 0, 9999, 0) + 1;
                support->active = 0;
                support->state = 2;
            }
            break;
        case 2: // Emit the boost effects until its timer expires.
            if (support->active < support->speed_q8) {
                ++support->active;
                int variant = work->variant;
                if (!(support->active % data_ov011_020c5f3c[variant]))
                    Overlay11Effects_SpawnAttached(data_ov011_020c5f4c[variant], shell);
                variant = work->variant;
                if (!(support->active % data_ov011_020c5f30[variant]))
                    BattleModelEffect_SpawnRelative(data_ov011_020c5f5c[variant], 0, shell, 0, 0, 0, 256);
            } else {
                BattleSceneObject_SetAnimation(object, support->rest_animation, -1);
                BattleSceneObject_SetAnimation(shell, support->rest_animation, -1);
                if (support->effect_handle) {
                    BattleSound_Stop(support->effect_handle - 1);
                    support->effect_handle = 0;
                }
                func_0200940c(BattleSceneObject_GetActiveModel(shell), data_ov011_020c5f38[work->variant]);
                func_0200940c(model, data_ov011_020c5f38[work->variant]);
                support->active = 0;
                support->state = 1;
            }
            break;
        case 3: // Finish the launch-away motion before waiting or retreating.
            if (!BattleSceneObject_IsAnimationChannelActive(object, 3) && model->get_animation_id() != 4)
                BattleSceneObject_SetAnimation(support->object, 4, -1);
            if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
                if ((u8)((ShellFinishView *)work)->finished) func_ov011_020c2f38(support);
                else if (support->delay) support->state = 4;
                else support->state = 0;
            }
            break;
        case 4: // Delay before becoming idle.
            --support->delay;
            if (support->delay <= 0) support->state = 0;
            break;
        case 5: // A scheduled intercept starts when its delay elapses.
            --support->delay;
            if (support->delay <= 0)
                Overlay11Support_StartIntercept(support, support->duration, support->reposition, 0, support->animation);
            break;
        case 6: // Catch completed: attach the projectile to the support.
            if (!BattleSceneObject_IsAnimationChannelActive(object, 3)) {
                BattleSceneObject_StopMotionChannel(object, 2, 0);
                BattleEntity_BindResource(object->actor_id, 55);
                BattleSceneObject_SetAnimation(object, 0, -1);
                func_0200940c(model, data_ov011_020c5f38[work->variant]);
                work->projectile.caught_side = (u8)(support->side + 1);
                BattleSceneObject_AdjustPosition(object, shell->x - object->x,
                    shell->y - object->y, work->particle_config->height - object->z);
                BattleMotion_StartFollower(shell, object, 3);
                support->state = 1;
            }
            break;
        case 7: // Return to the actor and start its finish animation.
            if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
                if (!work->controllers[support->side].state) {
                    BattleSceneObject_SetAnimation(object, -1, -1);
                    Overlay11Attack_BeginFinishAnimation((Overlay11AttackController *)&work->controllers[support->side]);
                    support->state = 0;
                } else if (model->get_animation_id()) {
                    BattleEntity_BindResource(object->actor_id, 56);
                    BattleSceneObject_SetAnimationFromComponent(object, 3, 0, 2596);
                    model->flags |= 0x200;
                }
            }
            break;
        }
    }
}
}
