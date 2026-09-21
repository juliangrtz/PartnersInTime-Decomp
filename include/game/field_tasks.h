#ifndef PIT_GAME_FIELD_TASKS_H
#define PIT_GAME_FIELD_TASKS_H
#include <game/task.h>
/* Allocated by FieldSystem_Init: 44 bytes for the IRQ task, 40 for frame finalization. */
typedef struct FieldGraphicsIrqTask {
    GameIrqTask task;
    u8 unknown18[16];
    struct {
        u32 pending : 1, unknown : 31;
    } flags;
} FieldGraphicsIrqTask;
typedef struct FieldFrameTask {
    GameTask task;
    u8 unknown18[16];
} FieldFrameTask;
typedef char FieldGraphicsIrqTask_SizeCheck[sizeof(FieldGraphicsIrqTask) == 44 ? 1 : -1];
typedef char FieldFrameTask_SizeCheck[sizeof(FieldFrameTask) == 40 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void FieldGraphicsIrqTask_Update(FieldGraphicsIrqTask *task);
FieldGraphicsIrqTask *FieldGraphicsIrqTask_Init(FieldGraphicsIrqTask *, u32, u32, void *);
FieldGraphicsIrqTask *FieldGraphicsIrqTask_Destroy(FieldGraphicsIrqTask *);
FieldGraphicsIrqTask *FieldGraphicsIrqTask_Delete(FieldGraphicsIrqTask *);
FieldFrameTask *FieldFrameTask_Init(FieldFrameTask *, u32, u32, void *);
FieldFrameTask *FieldFrameTask_Destroy(FieldFrameTask *);
FieldFrameTask *FieldFrameTask_Delete(FieldFrameTask *);
#ifdef __cplusplus
}
#endif
#endif
