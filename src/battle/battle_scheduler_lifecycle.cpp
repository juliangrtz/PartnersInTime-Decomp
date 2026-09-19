/*
 * Battle scheduler lifecycle (overlay 2, 0x02072D90-0x02073068).
 *
 * Init, Destroy and Delete for the battle scheduler and its IRQ task.
 */

#include "battle_scheduler_internal.h"
#include <game/task.h>
extern "C" {
#include <game/archive_io.h>
#include <game/heap.h>
void GameTexture_ResetLists(void);
void func_02009094(int screen);
void func_0202cbd4(void *destination, int value, u32 size);
extern GameTaskVTable data_ov002_020be978;
extern GameTaskVTable data_ov002_020be98c;
extern char data_ov002_020be998[];
GameIrqTask *BattleSchedulerIrq_Init(GameIrqTask *task, u32 priority, u32 unused);
}

extern "C" BattleScheduler *BattleScheduler_Init(BattleScheduler *system, u32 priority, u32 unused)
{
    ArchiveCompressedIO_InitComplete((ArchiveCompressedIO *)system, priority, unused, 0, -32897);
    ((GameTask *)system)->vtable = &data_ov002_020be978;
    GameTask_Disable((GameTask *)system);
    GameTexture_ResetLists();
    func_02009094(0);
    func_02009094(1);
    func_0202cbd4(&system->unknown_440, 0,
        (u8 *)&system->padding_dfc - (u8 *)&system->unknown_440);
    gBattleSystem = system;
    GameIrqTask *irq = (GameIrqTask *)GameHeap_New(40, 0, data_ov002_020be998, 0);
    if (irq)
        irq = BattleSchedulerIrq_Init(irq, 8, (u32)data_ov002_020be998);
    gBattleSystem->irq_task = irq;
    return system;
}

extern "C" BattleScheduler *BattleScheduler_Destroy(BattleScheduler *system)
{
    ((GameTask *)system)->vtable = &data_ov002_020be978;
    GameIrqTask_DeleteSafe((GameIrqTask *)gBattleSystem->irq_task);
    /* Native cleanup reads the retained payload link after returning the heap block. */
    for (BattleSchedulerNode *node = gBattleSystem->first; node; node = node->next) {
        if (node) {
            BattleSchedulerNode_UnlinkForCleanup(node);
            GameHeap_Delete(node);
        }
    }
    if (gBattleSystem->buffer_a)
        GameHeap_Delete(gBattleSystem->buffer_a);
    if (gBattleSystem->buffer_b)
        GameHeap_Delete(gBattleSystem->buffer_b);
    gBattleSystem = 0;
    GameTexture_ResetLists();
    func_02009094(0);
    func_02009094(1);
    ArchiveCompressedIO_DestroyBase((ArchiveCompressedIO *)system);
    return system;
}

extern "C" BattleScheduler *BattleScheduler_Delete(BattleScheduler *system)
{
    ((GameTask *)system)->vtable = &data_ov002_020be978;
    GameIrqTask_DeleteSafe((GameIrqTask *)gBattleSystem->irq_task);
    /* Native cleanup reads the retained payload link after returning the heap block. */
    for (BattleSchedulerNode *node = gBattleSystem->first; node; node = node->next) {
        if (node) {
            BattleSchedulerNode_UnlinkForCleanup(node);
            GameHeap_Delete(node);
        }
    }
    if (gBattleSystem->buffer_a)
        GameHeap_Delete(gBattleSystem->buffer_a);
    if (gBattleSystem->buffer_b)
        GameHeap_Delete(gBattleSystem->buffer_b);
    gBattleSystem = 0;
    GameTexture_ResetLists();
    func_02009094(0);
    func_02009094(1);
    ArchiveCompressedIO_DestroyBase((ArchiveCompressedIO *)system);
    GameHeap_Delete(system);
    return system;
}

extern "C" GameIrqTask *BattleSchedulerIrq_Init(GameIrqTask *task, u32 priority, u32 unused)
{
    GameIrqTask_Init(task, priority, unused, gBattleSystem);
    task->vtable = &data_ov002_020be98c;
    return task;
}

extern "C" GameIrqTask *BattleSchedulerIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov002_020be98c;
    GameIrqTask_DestroyBase(task);
    return task;
}

extern "C" GameIrqTask *BattleSchedulerIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov002_020be98c;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}
