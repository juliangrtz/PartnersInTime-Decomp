/* Pocket Chomp entry, repeated attacks and return to the battle command state.
 * Overlay 18, 0x020C6A20-0x020C6D68. The two participants have separate
 * controllers; active pointers select which pair takes the next turn.
 */

#include "pocket_chomp_internal.h"
extern "C" {
#include <game/battle_feedback.h>
}

/* Complete 656-byte attack allocation. The common prefix is mostly opaque here. */
struct PocketChompAttackWork {
    u8 unknown00[16];
    struct { u8 unknown0 : 1, stop : 1, unknown2 : 6; } action_flags;
    s8 phase;
    u8 unknown18[240];
    u16 target_id;
    u8 unknown260[44];
    PocketChompAdultMotion adults[2];
    PocketChompSupport supports[2];
    PocketChomp center;
    PocketChompAdultMotion *active_adult;
    PocketChompSupport *active_support, *other_support;
    u16 blocked_target, horizontal_offset, contact_offset, progress;
    struct { u16 direction : 1, unknown : 15; } flags;
    u16 padding;
};
typedef char PocketChompAttackWorkSize[sizeof(PocketChompAttackWork) == 656 ? 1 : -1];

extern "C" {
void func_ov018_020c6484(PocketChompAdultMotion *);
void func_ov018_020c5214(PocketChompSupport *, PocketChomp *);
int Overlay10Enemy_SelectReactionTarget(void);
void func_ov018_020c3298(int, int);
void PocketChompAttack_Update(BattlePartyActor *);
void func_ov002_020722ac(BattlePartyActor *, void (*)(BattlePartyActor *));
int func_ov018_020c2dfc(void);

void PocketChompAttack_UpdateEntry(BattlePartyActor *user)
{
    PocketChompAttackWork *work = (PocketChompAttackWork *)data_ov002_020c0710;
    BattleSceneObject_GetById(user->linked_object_id);
    int i;
    PocketChompAdultMotion *adult = work->adults;
    PocketChompSupport *support = work->supports;
    for (i = 0; i < 2; ++i, ++adult, ++support) {
        func_ov018_020c6484(adult);
        func_ov018_020c5214(support, &work->center);
    }
    PocketChomp_Update(&work->center, work->adults, &work->supports[0], &work->supports[1]);
    switch (work->phase) {
    case 0: {
        work->contact_offset = 64;
        work->flags.direction = 0;
        BattleSceneObject *backdrop = BattleSceneObject_GetById(40);
        BattleSceneObject_SetAnimation(backdrop, -1, -1);
        PocketChomp_BeginEntry(&work->center, backdrop);
        BattleParty_RestoreIdleAnimation(user, user->actor.scene_object);
        work->phase = 1;
        break;
    }
    case 1:
        if (!work->center.bits.phase && PocketChompAttack_AreAdultsIdle()) {
            work->target_id = Overlay10Enemy_SelectReactionTarget();
            func_ov018_020c3298(0, 0);
            work->phase = 0;
            func_ov002_020722ac(user, PocketChompAttack_Update);
        }
        break;
    }
}

void PocketChompAttack_Update(BattlePartyActor *user)
{
    PocketChompAttackWork *work = (PocketChompAttackWork *)data_ov002_020c0710;
    int i = 0;
    PocketChompAdultMotion *adult = work->adults;
    do {
        func_ov018_020c6484(adult);
        ++i;
        ++adult;
    } while (i < 2);
    int j;
    PocketChompSupport *support = work->supports;
    for (j = 0; j < 2; ++j, ++support)
        func_ov018_020c5214(support, &work->center);
    PocketChomp_Update(&work->center, work->active_adult, work->active_support, work->other_support);
    switch (work->phase) {
    case 0:
        if (!work->active_adult->bits.phase && !work->center.bits.phase) {
            if (work->action_flags.stop) {
                work->phase = 1;
            } else if (func_ov018_020c2dfc()) {
                work->target_id = Overlay10Enemy_SelectReactionTarget();
                if (work->target_id) {
                    ++work->progress;
                    func_ov018_020c3298(-1, -1);
                } else {
                    work->phase = 1;
                }
            }
        }
        break;
    case 1:
        PocketChompAttack_ReturnBackdrop();
        PocketChompAdult_BeginEntry((Overlay18AttackObjectController *)&work->adults[0]);
        PocketChompAdult_BeginEntry((Overlay18AttackObjectController *)&work->adults[1]);
        work->phase = 2;
        break;
    case 2:
        if (PocketChompAttack_IsBackdropIdle() &&
            !work->adults[0].bits.phase && !work->adults[1].bits.phase) {
            int k;
            PocketChompAdultMotion *adult = work->adults;
            PocketChompSupport *support = work->supports;
            for (k = 0; k < 2; ++k, ++adult, ++support) {
                Overlay18Attack_ClearObjectAnchor((Overlay18AttackObjectController *)adult);
                PocketChompSupport_Hide(support);
                PocketChompSupport_ClearAnchor(support);
            }
            PocketChomp_Hide(&work->center);
            func_ov002_020722ac(user, 0);
            /* Detach the active workspace; this callback does not free it. */
            data_ov002_020c0710 = 0;
        }
        break;
    }
}
}
