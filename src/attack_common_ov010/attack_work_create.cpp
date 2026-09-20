/* Common Bros. Attack allocation and state snapshot, 0x020C2978-0x020C2A64. */
#include "attack_common_internal.h"

/* The caller supplies an action item. Native code tests bit 12 only;
 * preserve that lookup and its invalid-item null result. */
static inline const ActionItemRecord *ActionRecord(u16 action)
{
    if ((action & ITEM_ID_ACTION_TAG) != ITEM_ID_ACTION_TAG)
        return 0;
    return &gActionItemRecords[action & ITEM_ID_INDEX_MASK];
}

extern "C" {
Overlay10Work *Overlay10Attack_CreateWork(Overlay10ActionActor *user, int size, int mode)
{
    Overlay10Work *work;
    const ActionItemRecord *item;
    u16 action;
    data_ov002_020c0710 = (Overlay10Work *)GameHeap_Allocate(
        *(int *)(gBattleContext + 0xE154), size, 0, 1);
    func_0202cbd4(data_ov002_020c0710, 0, size);
    work = data_ov002_020c0710;
    work->user = user;
    /* Keep the native unsigned halfword copy, including the no-target value. */
    work->target = *(const u16 *)&user->party.actor.target_actor_id;
    work->enemy_count = Overlay10Enemy_CollectIds(work->enemy_ids, 1);
    Overlay10Enemy_Snapshot((Overlay10EnemyState *)work);
    action = user->action_item;
    item = ActionRecord(action);
    Overlay10Party_AverageStats(&work->combined_actor,
        data_ov010_020c2aa0[((const Overlay10TargetFlags *)&item->target_flags)->count_index]);
    ((Overlay10BattleFlags *)(gBattleContext + 0xD3A0))->mode = mode;
    return work;
}
}
