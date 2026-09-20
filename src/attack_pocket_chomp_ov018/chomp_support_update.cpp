/* Support attack timing, swings, impact and hiding.
 * Overlay 18, 0x020C5214-0x020C54A0.
 */

#include "pocket_chomp_internal.h"
extern "C" {
extern u8 *gBattleContext;
extern u16 data_ov002_020be704[];
int func_ov018_020c513c(PocketChompSupport *);
void func_ov018_020c4f3c(PocketChompSupport *);

void PocketChompSupport_Update(PocketChompSupport *support, PocketChomp *center)
{
    PocketChompAttackWorkPrefix *work;
    BattleModel *model;
    u16 pressed;
    BattleSceneObject *object = support->object;
    if (object) {
        work = data_ov002_020c0710;
        model = BattleSceneObject_GetActiveModel(object);
        pressed = *(u16 *)(gBattleContext + 260) & 0xc03;
        switch (support->bits.phase) {
        case 1:
            PocketChompSupport_UpdateParticles(support);
            if (!work->flags16.stop && (pressed & data_ov002_020be704[support->bits.mode]))
                PocketChompSupport_BeginAttack(support);
            break;
        case 2:
            PocketChompSupport_UpdateParticles(support);
            --support->timer;
            if (func_ov018_020c513c(support))
                func_ov018_020c4f3c(support);
            if (support->timer <= 0) {
                func_ov018_020c2e50(support, 17152);
                func_0200940c(model, 256);
                support->timer = 0;
                u32 flags = (support->flags & ~0x7c0) | 0x40;
                support->flags = flags;
            }
            break;
        case 3:
            if (!work->flags16.stop && (pressed & data_ov002_020be704[support->bits.mode]))
                PocketChompSupport_BeginSwing(support);
            break;
        case 4:
            if (!support->action_flags.unknown_00 && model->property_056 > 1) {
                support->action_flags.unknown_00 = 1;
                PocketChompSupport_SpawnImpact(support);
                if (work->flags16.stop)
                    PocketChomp_UpdateVisibility(center);
                else
                    PocketChomp_Boost(center);
            }
            if ((u8)model->flag_bits.unknown_09) {
                func_ov018_020c2e50(support, 1024);
                support->action_flags.unknown_00 = 0;
                u32 flags = (support->flags & ~0x7c0) | 0xc0;
                support->flags = flags;
            }
            break;
        case 5:
            if (!BattleSceneObject_IsAnimationChannelActive(object, 2))
                PocketChompSupport_Hide(support);
            break;
        }
    }
}
}
