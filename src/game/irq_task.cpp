#include <game/task.h>

extern "C" u32 OS_DisableIrqMask(u32 mask);
extern "C" u32 OS_EnableIrqMask(u32 mask);

extern "C" GameTaskVTable data_02059b40;
extern "C" u8 data_02060d6c;
extern "C" GameIrqTask *data_02060d88;
extern "C" GameIrqTask *data_02060d7c;
extern "C" GameIrqTask *data_02060d80;
extern "C" void func_02029bb0(void *allocation);

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
    func_02029bb0(task);
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
