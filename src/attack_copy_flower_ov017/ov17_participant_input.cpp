/* Accept only the formation's exact key mask; simultaneous extra keys do not
 * count. */
#include "participant_internal.h"
extern "C" u16 data_ov002_020be704[];

extern "C" void Overlay17Participant_ConsumeInput(Overlay17Participant *participant)
{
    if (participant->state.bits.input)
        return;
    Overlay17BattleStateView *work = data_ov002_020c0710;
    if (work->pressed_mask != data_ov002_020be704[participant->state.bits.formation])
        return;
    participant->state.bits.input = 1;
    work->input_mask &= ~data_ov002_020be704[participant->state.bits.formation];
}
