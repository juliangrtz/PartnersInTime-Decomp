#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>

extern u8 *gBattleSystem;

int func_ov002_02068134(void *component);
int func_0200b374(u32 destination, const void *resource,
                  const void *component, int component_index,
                  int end_index);
int func_020096b0(BattleTextureDecodeState *state, int count);
void func_02009d24(BattleTextureDecodeState *state, int texture_id,
                   int unknown_2, int copy_flag, void *component_10,
                   u8 *data);
void func_0202c510(u8 *system, BattleTextureUploadRequest *request,
                   void *destination);
void *func_ov002_02072508(int (*callback)(BattleObjectUploadTask *task),
                          BattleSceneResource *resource, int unknown_2,
                          int unknown_3);
void *func_ov002_020725a4(int (*callback)(BattleObjectUploadTask *task),
                          BattleSceneResource *resource, int unknown_2,
                          int unknown_3);

void BattleObjectData_WaitBodyDecodeTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;

    if (func_020096b0(&context->texture_decode, 48) != 0) {
        task->callback = BattleObjectData_BeginTailDecodeTask;
    }
}

void BattleObjectData_BeginTailDecodeTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;
    BattleSceneResource *resource =
        (BattleSceneResource *)task->argument;

    func_02009d24(&context->texture_decode, 1, 0,
                  !resource->flags.bits.copy_flag,
                  resource->component_10, resource->data);
    task->callback = BattleObjectData_WaitTailDecodeTask;
}

void BattleObjectData_WaitTailDecodeTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;

    if (func_020096b0(&context->texture_decode, 16) != 0) {
        task->callback = BattleObjectData_QueueFinalUploadTask;
    }
}

void BattleObjectData_QueueFinalUploadTask(BattleQueuedTask *task) {
    BattleSceneResource *resource =
        (BattleSceneResource *)task->argument;

    ((BattleContext *)gBattleContext)
        ->current_texture_upload->destination = resource->component_14;
    func_0202c510(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->current_texture_upload,
                  resource->component_14);
    task->callback = BattleObjectData_CompleteUploadTask;
}

void BattleObjectData_CompleteUploadTask(BattleQueuedTask *task) {
    BattleSceneResource *resource =
        (BattleSceneResource *)task->argument;

    if (resource->flags.bits.load_pending) {
        task->callback = BattleObjectData_BeginRebuildTask;
    } else {
        resource->flags.bits.processing = 0;
        task->callback = 0;
    }

    if ((resource->flags.bits.resource_index == 0 ||
         resource->flags.bits.allocated) &&
        !resource->flags.bits.copy_flag) {
        func_ov002_02072508(BattleObjectData_UploadSpriteTask,
                            resource, 0, 0);
        if (resource->component_14 == gBattleContext + 0x37608) {
            *(u16 *)(gBattleSystem + 0xDF6) =
                (*(u16 *)(gBattleSystem + 0xDF6) & ~1) | 1;
        }
    }

    if (resource->flags.bits.resource_index != 0 &&
        !resource->flags.bits.upload_complete) {
        func_ov002_020725a4(BattleObjectData_UploadTextureTask,
                            resource, 0, 0);
        *(u16 *)(gBattleSystem + 0xDF6) =
            (*(u16 *)(gBattleSystem + 0xDF6) & ~1) | 1;
    }
}

int BattleObjectData_UploadTextureTask(BattleObjectUploadTask *task) {
    BattleSceneResource *resource = task->resource;
    s32 resource_offset =
        (resource->flags.bits.resource_index - 1) << 6;

    return func_0200b374(
        *(u32 *)(gBattleContext + 0x64 + (resource_offset >> 17) * 4) +
            (resource_offset & 0x1FFFF),
        resource->data, resource->component_14, -1, -1);
}

int BattleObjectData_UploadSpriteTask(BattleObjectUploadTask *task) {
    return func_ov002_02068134(task->resource);
}
