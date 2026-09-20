/* Bro/Ice Flower participant phases, overlay 14, 0x020C53D4-0x020C5724. */
#include "flower_internal.h"
extern "C" {
extern u8 *gBattleContext;
extern u16 data_ov002_020be704[];
int func_ov014_020c4c10(Overlay14Participant *, int, Overlay14Participant *);
static inline u8 AnimationFinished(BattleModel *model)
{
    return model->flag_bits.unknown_09;
}

void Overlay14Participant_Update(Overlay14Participant *participant, Overlay14Participant *other)
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    BattleModel *model = BattleSceneObject_GetActiveModel(participant->actor->actor.scene_object);
    int formation = participant->actor->formation_index;
    int side = formation & 1;
    u16 keys = *(u16 *)(gBattleContext + 260) & 0xc03;
    switch (participant->phase) {
    case 1:
        if (participant->timer > 12)
            Overlay14Participant_UpdateParticles(participant);
        if (--participant->timer <= 0)
            participant->phase = 0;
        break;
    case 2:
        participant->phase = 0;
        break;
    /* Held projectile: automatic handoff or primary/secondary throw input. */
    case 3:
    case 4:
        if (participant->phase == 4 && AnimationFinished(model))
            Overlay14Participant_SetProjectileMode(participant, 2);
        if (((Overlay14AttackFlags *)&work->common.flags)->stop && participant->phase == 3) {
            Overlay14Projectile_BeginHide(participant->projectile);
            participant->phase = 0;
            return;
        }
        {
            int automatic = 0;
            if (participant->flags.side == 0 && participant->flags.flag1) {
                if (participant->phase == 4)
                    return;
                if (participant->timer < 2) {
                    ++participant->timer;
                    return;
                }
                automatic = 1;
            } else if (participant->flags.side == 1 && participant->flags.flag1) {
                return;
            }
            if (!participant->effect) {
                if ((keys & data_ov002_020be704[formation]) || automatic) {
                    participant->flags.flag1 = 0;
                    other->flags.flag1 = 0;
                    Overlay14Participant_BeginPrimaryThrow(participant);
                } else if (!participant->flags.flag1 && formation >= 4) {
                    if (keys & data_ov002_020be704[side + 2])
                        Overlay14Participant_BeginSecondaryThrow(participant);
                }
            }
        }
        break;
    /* Launch once at the animation marker, then prepare the next projectile. */
    case 5:
    case 7:
        if (participant->animation_frame != -1 &&
            (model->property_056 >= participant->animation_frame || AnimationFinished(model))) {
            int launched = 0;
            switch (participant->phase) {
            case 5: launched = func_ov014_020c4c10(participant, 0, other); break;
            case 7: launched = func_ov014_020c4c10(participant, 1, other); break;
            }
            if (launched) {
                participant->projectile = 0;
                participant->animation_frame = -1;
            }
        }
        if (AnimationFinished(model)) {
            switch (participant->phase) {
            case 5: Overlay14Participant_PrepareProjectile(participant, -1); break;
            case 7: Overlay14Participant_PrepareProjectile(participant, -1); break;
            }
        }
        break;
    case 6:
    case 8:
        if (--participant->timer <= 0)
            Overlay14Participant_PrepareProjectile(participant, -1);
        break;
    }
}
}
