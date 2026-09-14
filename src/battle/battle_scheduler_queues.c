#include "battle_scheduler_internal.h"
#include <game/task.h>

u32 func_02035818(void);
u32 func_02035804(void);
void func_02035c00(int banks);
void func_02035b0c(int banks);

void BattleScheduler_VBlank(void) {
    BattleSchedulerNode *node;

    ++gBattleSystem->vblank_count;
    if (!gBattleSystem->flags.bits.vblank_ready) {
        return;
    }
    gBattleSystem->flags.bits.vblank_ready = 0;
    gBattleSystem->flags.bits.active_task = 0;
    gBattleSystem->flags.raw |= 4;
    gBattleSystem->texture_banks = func_02035818();
    gBattleSystem->palette_banks = func_02035804();
    {
        int end = gBattleSystem->before_tail;
        int index = gBattleSystem->before_head;

        gBattleSystem->before_head = end;
        while (index != end) {
            BattleTransferTask *task = &gBattleSystem->before_mapping[index];
            BattleTransferCallback callback = task->callback;

            if (callback) {
                callback(task);
            }
            if (++index == 32) {
                index = 0;
            }
        }
    }
    func_02035c00(gBattleSystem->texture_banks);
    func_02035b0c(gBattleSystem->palette_banks);
    {
        int end = gBattleSystem->after_tail;
        int index = gBattleSystem->after_head;

        gBattleSystem->after_head = end;
        while (index != end) {
            BattleTransferTask *task = &gBattleSystem->after_mapping[index];
            BattleTransferCallback callback = task->callback;

            if (callback) {
                callback(task);
            }
            if (++index == 32) {
                index = 0;
            }
        }
    }
    for (node = gBattleSystem->first; node; node = node->next) {
        if (node->vblank) {
            node->vblank();
        }
    }
    gBattleSystem->flags.raw &= ~4;
}

BattleSchedulerNode *BattleSchedulerNode_Insert(BattleSchedulerNode *node, int priority) {
    BattleSchedulerNode *previous = 0;
    BattleSchedulerNode *next;

    for (next = gBattleSystem->first;
         next && next->priority <= priority; next = next->next) {
        previous = next;
    }
    node->update = BattleScheduler_Idle;
    node->vblank = 0;
    node->priority = priority;
    node->next = next;
    if (previous) {
        previous->next = node;
    } else {
        gBattleSystem->first = node;
    }
    GameTask_Enable((GameTask *)gBattleSystem);
    GameIrqTask_Enable((GameIrqTask *)gBattleSystem->irq_task);
    return node;
}

BattleSchedulerNode *BattleSchedulerNode_UnlinkForCleanup(BattleSchedulerNode *node) {
    BattleSchedulerNode *previous = gBattleSystem->first;

    /* The native head case clears the list, including any following nodes. */
    if (previous == node) {
        gBattleSystem->first = 0;
    } else {
        for (;;) {
            if (previous->next == node) {
                previous->next = node->next;
                return node;
            }
            previous = previous->next;
        }
    }
    return node;
}

BattleSchedulerNode *BattleSchedulerNode_Unlink(BattleSchedulerNode *node) {
    BattleSchedulerNode *previous = gBattleSystem->first;

    /* The native head case clears the list, including any following nodes. */
    if (previous == node) {
        gBattleSystem->first = 0;
    } else {
        for (;;) {
            if (previous->next == node) {
                previous->next = node->next;
                return node;
            }
            previous = previous->next;
        }
    }
    return node;
}

void BattleScheduler_Idle(void) {
}

BattleQueuedTask *BattleTaskQueue_Enqueue(BattleQueuedTaskCallback callback,
                                        void *argument) {
    int tail = gBattleSystem->task_tail;
    BattleQueuedTask *task = &gBattleSystem->tasks[tail];
    int next = 0;

    task->callback = callback;
    task->state = 0;
    task->timer = 0;
    task->argument = argument;
    if (tail < 31) {
        next = tail + 1;
    }
    gBattleSystem->task_tail = next;
    return task;
}

void BattleTaskQueue_Promote(BattleQueuedTask *task) {
    BattleQueuedTask *active = &gBattleSystem->active_task;

    active->callback = task->callback;
    active->state = task->state;
    active->timer = task->timer;
    active->argument = task->argument;
    task->callback = 0;
    gBattleSystem->flags.bits.active_task = 1;
}

BattleTransferTask *BattleTransfer_EnqueueBeforeMapping(BattleTransferCallback callback,
                                      u32 argument_1, u32 argument_2,
                                      u32 argument_3) {
    BattleTransferTask *task;
    int tail;

    if (gBattleSystem->flags.bits.inside_vblank) {
        task = gBattleSystem->deferred_before_mapping;
        while (task->callback) {
            ++task;
        }
        task->callback = callback;
        task->argument_1 = argument_1;
        task->argument_2 = argument_2;
        task->argument_3 = argument_3;
        return task;
    }
    tail = gBattleSystem->before_tail;
    task = &gBattleSystem->before_mapping[tail];
    task->callback = callback;
    task->argument_1 = argument_1;
    task->argument_2 = argument_2;
    task->argument_3 = argument_3;
    gBattleSystem->before_tail = tail < 31 ? tail + 1 : 0;
    return task;
}

BattleTransferTask *BattleTransfer_EnqueueAfterMapping(BattleTransferCallback callback,
                                      u32 argument_1, u32 argument_2,
                                      u32 argument_3) {
    BattleTransferTask *task;
    int tail;

    if (gBattleSystem->flags.bits.inside_vblank) {
        task = gBattleSystem->deferred_after_mapping;
        while (task->callback) {
            ++task;
        }
        task->callback = callback;
        task->argument_1 = argument_1;
        task->argument_2 = argument_2;
        task->argument_3 = argument_3;
        return task;
    }
    tail = gBattleSystem->after_tail;
    task = &gBattleSystem->after_mapping[tail];
    task->callback = callback;
    task->argument_1 = argument_1;
    task->argument_2 = argument_2;
    task->argument_3 = argument_3;
    gBattleSystem->after_tail = tail < 31 ? tail + 1 : 0;
    return task;
}

int BattleScheduler_ElapsedScanlines(int start) {
    int scanline = *(vu16 *)0x04000006;
    int frames = gBattleSystem->vblank_count;

    if (scanline >= 192) {
        scanline -= 263;
    }
    return scanline + 263 * frames - start;
}
