/*
 * Field graphics IRQ teardown (overlay 0, 0x02065D40-0x02065D90).
 *
 * Destroy and Delete for the field's graphics VBlank task.
 */

#include <game/field_tasks.h>
#include <game/heap.h>
extern GameTaskVTable data_ov000_020c0dd0, data_ov000_020c0df8;
FieldGraphicsIrqTask *FieldGraphicsIrqTask_Delete(FieldGraphicsIrqTask *work) {
    work->task.vtable = &data_ov000_020c0dd0;
    GameIrqTask_DestroyBase(&work->task);
    GameHeap_Delete(work);
    return work;
}

FieldGraphicsIrqTask *FieldGraphicsIrqTask_Destroy(FieldGraphicsIrqTask *work) {
    work->task.vtable = &data_ov000_020c0dd0;
    GameIrqTask_DestroyBase(&work->task);
    return work;
}
