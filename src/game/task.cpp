/*
 * Task scheduler (ARM9 resident, 0x02029DE4-0x0202A814).
 *
 * Both scheduler lists: the normal tasks stepped from the main loop and the IRQ
 * tasks stepped from VBlank. A task links itself in at Init and out at Destroy,
 * and the update pass restarts from the head whenever a task is still
 * incomplete, so a task added or removed mid-pass is handled correctly. See
 * include/game/task.h for what interval and counter mean.
 */

#include <game/task.h>

extern "C" GameTaskVTable data_02059b54;
extern "C" u8 data_02060d70;
extern "C" GameTask *data_02060d84;
extern "C" GameTask *data_02060d74;
extern "C" GameTask *data_02060d78;
extern "C" void GameHeap_Delete(void *allocation);

GameTask *GameTask_Init(GameTask *task, u32 priority, u32 unused, void *argument)
{
    GameTask *cursor;
    task->vtable = &data_02059b54;
    cursor = data_02060d84;
    if (!cursor) {
        data_02060d84 = task;
        task->previous = 0;
        task->next = 0;
    } else {
        for (;;) {
            if (cursor->priority > priority) {
                if (!cursor->previous) {
                    data_02060d84 = task;
                    task->previous = 0;
                } else {
                    cursor->previous->next = task;
                    task->previous = cursor->previous;
                }
                task->next = cursor;
                cursor->previous = task;
                break;
            }
            if (!cursor->next) {
                cursor->next = task;
                task->previous = cursor;
                task->next = 0;
                break;
            }
            cursor = cursor->next;
        }
    }
    task->argument = argument;
    task->status = 0;
    task->priority = priority;
    task->active = 1;
    task->processed = 1;
    task->interval = 1;
    task->counter = 0;
    task->update_count = 0;
    ++data_02060d70;
    return task;
}

static inline void GameTask_Unlink(GameTask *task)
{
    task->vtable = &data_02059b54;
    if (task->next) task->next->previous = task->previous;
    if (!task->previous) data_02060d84 = task->next;
    else task->previous->next = task->next;
    --data_02060d70;
    if (data_02060d74 == task) data_02060d74 = 0;
    if (data_02060d78 == task) data_02060d78 = task->next;
}

GameTask *GameTask_DestroyComplete(GameTask *task)
{
    GameTask_Unlink(task);
    return task;
}

GameTask *GameTask_Delete(GameTask *task)
{
    GameTask_Unlink(task);
    GameHeap_Delete(task);
    return task;
}

GameTask *GameTask_DestroyBase(GameTask *task)
{
    GameTask_Unlink(task);
    return task;
}

void GameTask_Disable(GameTask *task)
{
    task->active = 0;
    task->processed = 1;
}

void GameTask_Enable(GameTask *task)
{
    task->active = 1;
    task->processed = 1;
}

void GameTask_DisableNormal(void)
{
    GameTask *task;
    for (task = data_02060d84; task; task = task->next) {
        if (task->priority != 0 && task->priority != 255) {
            task->active = 0;
            task->processed = 1;
        }
    }
}

void GameTask_UpdateAll(void)
{
    int complete;
    GameTask *task;
    if (data_02060d84) {
        do {
            task = data_02060d84;
            complete = 1;
            while (task) {
                data_02060d78 = task->next;
                data_02060d74 = task;
                if (task->active && !task->processed) {
                    int interval = task->interval;
                    if (interval & 8) {
                        task->counter++;
                        interval = -interval;
                        if (task->counter >= interval) {
                            ((GameTaskDispatch *)task)->update();
                            task->counter = 0;
                        }
                        if (data_02060d74) {
                            task->update_count++;
                            task->processed = 1;
                        }
                    } else {
                        ((GameTaskDispatch *)task)->update();
                        if (data_02060d74) {
                            task->update_count++;
                            task->counter++;
                            if (task->counter < task->interval) complete = 0;
                            else task->processed = 1;
                        }
                    }
                }
                task = data_02060d78;
            }
        } while (!complete);
    }
    for (task = data_02060d84; task; task = task->next) {
        task->processed = 0;
        if (!(task->interval & 8)) task->counter = 0;
    }
}

void GameTask_InitSystem(void)
{
    data_02060d70 = 0;
    data_02060d84 = 0;
    data_02060d74 = 0;
    data_02060d78 = 0;
}

extern "C" u32 OS_DisableIrqMask(u32 mask);
extern "C" u32 OS_EnableIrqMask(u32 mask);

extern "C" GameTaskVTable data_02059b40;
extern "C" u8 data_02060d6c;
extern "C" GameIrqTask *data_02060d88;
extern "C" GameIrqTask *data_02060d7c;
extern "C" GameIrqTask *data_02060d80;

GameIrqTask *GameIrqTask_Init(GameIrqTask *task, u32 priority, u32 unused, void *argument)
{
    GameIrqTask *cursor;
    u32 mask;
    task->vtable = &data_02059b40;
    mask = OS_DisableIrqMask(1);
    cursor = data_02060d88;
    if (!cursor) {
        data_02060d88 = task;
        task->previous = 0;
        task->next = 0;
    } else {
        for (;;) {
            if (cursor->priority > priority) {
                if (!cursor->previous) {
                    data_02060d88 = task;
                    task->previous = 0;
                } else {
                    cursor->previous->next = task;
                    task->previous = cursor->previous;
                }
                task->next = cursor;
                cursor->previous = task;
                break;
            }
            if (!cursor->next) {
                cursor->next = task;
                task->previous = cursor;
                task->next = 0;
                break;
            }
            cursor = cursor->next;
        }
    }
    task->argument = argument;
    task->status = 0;
    task->priority = priority;
    task->active = 0;
    task->processed = 1;
    task->update_count = 0;
    ++data_02060d6c;
    OS_EnableIrqMask(mask);
    return task;
}

static inline void GameIrqTask_Unlink(GameIrqTask *task)
{
    task->vtable = &data_02059b40;
    if (task->next) task->next->previous = task->previous;
    if (!task->previous) data_02060d88 = task->next;
    else task->previous->next = task->next;
    --data_02060d6c;
    if (data_02060d7c == task) data_02060d7c = 0;
    if (data_02060d80 == task) data_02060d80 = task->next;
}

GameIrqTask *GameIrqTask_DestroyComplete(GameIrqTask *task)
{
    GameIrqTask_Unlink(task);
    return task;
}

GameIrqTask *GameIrqTask_Delete(GameIrqTask *task)
{
    GameIrqTask_Unlink(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *GameIrqTask_DestroyBase(GameIrqTask *task)
{
    GameIrqTask_Unlink(task);
    return task;
}

void GameIrqTask_Disable(GameIrqTask *task)
{
    u32 mask = OS_DisableIrqMask(1);
    task->active = 0;
    task->processed = 1;
    OS_EnableIrqMask(mask);
}

void GameIrqTask_Enable(GameIrqTask *task)
{
    u32 mask = OS_DisableIrqMask(1);
    task->active = 1;
    task->processed = 1;
    OS_EnableIrqMask(mask);
}

void GameIrqTask_DisableNormal(void)
{
    GameIrqTask *task;
    u32 mask = OS_DisableIrqMask(1);
    for (task = data_02060d88; task; task = task->next) {
        if (task->priority != 0 && task->priority != 255) {
            task->active = 0;
            task->processed = 1;
        }
    }
    OS_EnableIrqMask(mask);
}

void GameIrqTask_UpdateAll(void)
{
    GameIrqTask *task = data_02060d88;
    if (task) {
        while (task) {
            data_02060d80 = task->next;
            data_02060d7c = task;
            if (task->active && !task->processed) {
                ((GameTaskDispatch *)task)->update();
                if (data_02060d7c) {
                    task->update_count++;
                    task->processed = 1;
                }
            }
            task = data_02060d80;
        }
    }
    for (task = data_02060d88; task; task = task->next) task->processed = 0;
}

void GameIrqTask_InitSystem(void)
{
    u32 mask = OS_DisableIrqMask(1);
    data_02060d6c = 0;
    data_02060d88 = 0;
    data_02060d7c = 0;
    data_02060d80 = 0;
    OS_EnableIrqMask(mask);
}

static inline void GameIrqTask_DeleteVirtual(GameIrqTask *task)
{
    if (task) ((GameTaskDispatch *)task)->delete_task();
}

void GameIrqTask_DeleteSafe(GameIrqTask *task)
{
    if (task) {
        u32 mask = OS_DisableIrqMask(1);
        GameIrqTask_DeleteVirtual(task);
        OS_EnableIrqMask(mask);
    }
}
