#include <game/field_tasks.h>
#include <game/heap.h>
extern GameTaskVTable data_ov000_020c0dd0, data_ov000_020c0df8;
FieldFrameTask *FieldFrameTask_Init(FieldFrameTask *work, u32 priority, u32 unused,
                                    void *argument) {
    GameTask_Init(&work->task, priority, unused, argument);
    work->task.vtable = &data_ov000_020c0df8;
    return work;
}

FieldFrameTask *FieldFrameTask_Destroy(FieldFrameTask *work) {
    work->task.vtable = &data_ov000_020c0df8;
    GameTask_DestroyBase(&work->task);
    return work;
}

FieldFrameTask *FieldFrameTask_Delete(FieldFrameTask *work) {
    work->task.vtable = &data_ov000_020c0df8;
    GameTask_DestroyBase(&work->task);
    GameHeap_Delete(work);
    return work;
}
