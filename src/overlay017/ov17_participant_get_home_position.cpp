#include "participant_internal.h"

extern "C" {
void Overlay17Participant_GetHomePosition(Overlay17Participant *participant, s16 *x, s16 *y)
{
    BattleActor *actor = BattleActor_GetById((u16)((participant->state.bits.formation & 1) + 56));
    s16 px = actor->unk_018, py = actor->unk_01a;
    if (participant->state.bits.formation >= 2)
        px -= 16;
    *x = px;
    *y = py;
}
}
