#include <game/field_tasks.h>
#include <game/heap.h>
extern GameTaskVTable data_ov000_020c0dd0, data_ov000_020c0df8;
FieldGraphicsIrqTask *FieldGraphicsIrqTask_Init(FieldGraphicsIrqTask *work, u32 priority,
                                                u32 unused, void *argument) {
    GameIrqTask_Init(&work->task, priority, unused, argument);
    work->task.vtable = &data_ov000_020c0dd0;
    work->flags.pending = 0;
    return work;
}
