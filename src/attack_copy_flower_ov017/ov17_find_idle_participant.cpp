/* Inspect each slot's phase field at the native participant stride. */
#include "participant_internal.h"
// Fold nested member offsets before advancing the strided flag pointer.
#define MEMBER_OFFSET(type, member) ((unsigned long)&(((type *)0)->member))

extern "C" int Overlay17Participant_FindIdleSlot()
{
    enum {
        FIRST_FLAGS_OFFSET = MEMBER_OFFSET(Overlay17BattleStateView, attack_states) +
                             MEMBER_OFFSET(Overlay17Participant, state) +
                             MEMBER_OFFSET(Overlay17ParticipantState, bits)
    };
    const Overlay17ParticipantFlags *flags =
        (const Overlay17ParticipantFlags *)((const u8 *)data_ov002_020c0710 + FIRST_FLAGS_OFFSET);
    for (int index = 0; index < OVERLAY17_ATTACK_STATE_COUNT; ++index)
    {
        if (!flags->phase)
            return index;
        flags = (const Overlay17ParticipantFlags *)((const u8 *)flags + sizeof(Overlay17Participant));
    }
    return -1;
}

#undef MEMBER_OFFSET
