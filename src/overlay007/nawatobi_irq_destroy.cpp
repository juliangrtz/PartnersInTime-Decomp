#include "nawatobi_internal.h"

extern "C" {
GameIrqTask *NawatobiSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov007_0208e0bc;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}
GameIrqTask *NawatobiSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov007_0208e0bc;
    GameIrqTask_DestroyBase(task);
    return task;
}
}
