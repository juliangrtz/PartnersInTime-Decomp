/*
 * Battle transition start (overlay 2, 0x020B5FDC-0x020B60C4).
 *
 * Begins the battle's entry and exit transitions.
 */

#include <game/battle_transition.h>
#include <game/battle_context.h>
#include <game/battle_entry.h>
#include <game/battle_display_capture.h>
#include <game/save_data.h>
extern void (*data_ov002_020c03a4[])(BattleAITask *);

BattleAITask *BattleTransition_BeginEntry(void) {
    BattleAITask *task;
    *(u32 *)(gBattleContext + 54176) |= 0x40;
    task = BattleTaskList_Insert((BattleTaskPool *)(gBattleContext + 35652), 0);
    /* Keep the full-width clear mask before storing the encounter halfword. */
    *(u16 *)(gSaveData + 1376) = (*(u16 *)(gSaveData + 1376) & ~0x1e00) | 0x200;
    data_ov002_020c03a4[((BattleEntrySaveView *)(gSaveData + 0x558))->flags.transition_kind](task);
    return task;
}

BattleAITask *BattleTransition_BeginExit(void) {
    BattleAITask *task;
    *(u32 *)(gBattleContext + 54176) |= 0x40;
    task = BattleTaskList_Insert((BattleTaskPool *)(gBattleContext + 35652), 0);
    BattleDisplayCapture_QueueConfigure(0);
    task->callback = BattleTransition_UpdateExitWait;
    return task;
}
