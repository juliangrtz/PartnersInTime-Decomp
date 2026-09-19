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

#include <game/field_display.h>
#include <game/input.h>
#include <game/sprite_output.h>
extern GameOamEntry data_02060340[], data_02060740[];
void FieldDisplay_FinishFrame(void) {
    GameOam_SortGroups(0);
    GameOam_SortGroups(1);
    GameOam_BuildSorted(0, data_02060340, data_02060340);
    GameOam_BuildSorted(1, data_02060740, data_02060740);
    GameFrame_WaitVBlank();
    GameOam_SetReserved(0, 0, 0, 0);
    GameOam_SetReserved(1, 0, 0, 0);
    GameInput_Read();
}

FieldGraphicsIrqTask *FieldGraphicsIrqTask_Init(FieldGraphicsIrqTask *work, u32 priority,
                                                u32 unused, void *argument) {
    GameIrqTask_Init(&work->task, priority, unused, argument);
    work->task.vtable = &data_ov000_020c0dd0;
    work->flags.pending = 0;
    return work;
}
