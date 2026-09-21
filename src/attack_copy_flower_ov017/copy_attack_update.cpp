/* Copy Flowers rounds and return, overlay 17, 0x020C60BC-0x020C6298. */
#include "participant_internal.h"
extern "C" {
#include <game/random.h>
}
/* Full attack allocation; only fields used by this controller are named. */
struct CopyFlowerAttackWork {
    u8 unknown000[16];
    struct { u8 unknown0 : 1, stop : 1, unknown2 : 6; } flags;
    s8 phase;
    u8 unknown018[286];
    Overlay17Participant participants[8];
    Overlay17Participant *last, *head;
    int tuning, spawn_timer;
    u16 input_mask, pressed_mask;
    u8 unknown2692[40];
    GameTexturePalette display;
    Overlay17AttackObject attack_objects[40];
};
typedef char CopyFlowerAttackWorkSize[sizeof(CopyFlowerAttackWork) == 16356 ? 1 : -1];
extern "C" {
extern u8 *gBattleContext;
void func_ov017_020c58cc(Overlay17Participant *);
void BattleAttack_SetCallback(BattlePartyActor *, void (*)(BattlePartyActor *));

void CopyFlowerAttack_Update(BattlePartyActor *actor)
{
    CopyFlowerAttackWork *work = (CopyFlowerAttackWork *)data_ov002_020c0710;
    work->pressed_mask = *(u16 *)(gBattleContext + 260) & 0xc03;
    work->input_mask = work->pressed_mask;
    /* Participant updates can change the next link. Read it after each call. */
    for (Overlay17Participant *p = work->head; p; p = p->state.next)
        func_ov017_020c58cc(p);
    Overlay17Participant_RemoveIdleFromList();
    switch (work->phase) {
    case 0:
        if (work->flags.stop) {
            int active = 0;
            int i;
            /* Walk the flag field at the native participant stride. */
            Overlay17ParticipantFlags *flags = &work->participants[0].state.bits;
            for (i = 0; i < 8; ++i,
                 flags = (Overlay17ParticipantFlags *)((u8 *)flags + sizeof(Overlay17Participant))) {
                if (flags->phase)
                    ++active;
            }
            if (!active)
                work->phase = 1;
        } else if (work->spawn_timer <= 0) {
            int slot = Overlay17Participant_FindIdleSlot();
            if (slot != -1) {
                Overlay17Participant_BeginEntry(&work->participants[slot], Random_NextModulo(4));
                work->spawn_timer = Overlay17Attack_GetGroupTuningParameter3(work->tuning);
            }
        } else {
            --work->spawn_timer;
        }
        break;
    case 1:
        if (Overlay17Attack_UpdateObjectAnimations()) {
            Overlay17Attack_ConfigurePartyObjects();
            BattleSound_Play(223, 0, 0, 0);
            work->phase = 2;
        }
        break;
    case 2:
        if (Overlay17Attack_UpdatePartyObjectReadiness())
            work->phase = 3;
        break;
    case 3:
        Overlay17Attack_FinalizeDisplay(&work->display);
        {
            int i = 0;
            Overlay17Participant *p = work->participants;
            do {
                Overlay17Participant_ReleaseModel(p);
                ++p;
                ++i;
            } while (i < 8);
        }
        BattleAttack_SetCallback(actor, 0);
        data_ov002_020c0710 = 0;
        break;
    }
}
}
