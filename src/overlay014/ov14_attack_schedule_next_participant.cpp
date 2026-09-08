#include "flower_internal.h"

extern "C" {
int Overlay14Attack_PickTarget(int filter)
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    int count = 0;
    u16 targets[6];
    if (filter) {
        for (int i = 0; i < 6; ++i) {
            if (Overlay10Enemy_IsSelectable((u16)(i + 60)) && work->common.current_hp[i] > 0 &&
                BattleEnemy_GetStats((u16)(i + 60))->trait_bits.unknown_trait_02)
                targets[count++] = i + 60;
        }
        if (count)
            return targets[Random_NextModulo(count)];
    }
    for (int i = 0; i < 6; ++i) {
        if (Overlay10Enemy_IsSelectable((u16)(i + 60)) && work->common.current_hp[i] > 0)
            targets[count++] = i + 60;
    }
    return count ? targets[Random_NextModulo(count)] : 0;
}

void Overlay14Attack_ScheduleNextParticipant(Overlay14Participant *participant)
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    work->next = participant;
    work->active = 0;
    work->delay = 8;
    Overlay14Attack_BeginTargetMove(&work->attack, (Overlay14AttackTargetReference *)participant, 8);
}
}
