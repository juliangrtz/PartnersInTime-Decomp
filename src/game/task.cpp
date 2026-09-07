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
