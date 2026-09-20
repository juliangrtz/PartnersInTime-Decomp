/*
 * Object upload (overlay 2, 0x02089300-0x020897A0).
 *
 * The tail of the object load chain: waiting out the decodes, queueing the final
 * upload and copying the resource into place. Each function is one frame of the
 * chain and installs its successor.
 */

#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>

extern u8 *gBattleSystem;

int func_0200b374(u32 destination, const void *resource,
                  const void *component, int component_index,
                  int end_index);
int func_020096b0(BattleTextureDecodeState *state, int count);
void ArchiveIO_QueueCompressedRead(u8 *system, BattleTextureUploadRequest *request,
                   void *destination);
void *BattleTransfer_EnqueueAfterMapping(int (*callback)(BattleObjectUploadTask *task),
                          BattleSceneResource *resource, int unknown_2,
                          int unknown_3);
void *BattleTransfer_EnqueueBeforeMapping(int (*callback)(BattleObjectUploadTask *task),
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

    GameGraphics_InitOffsetDecoder(&context->texture_decode, 1, 0,
                  !resource->flags.bits.copy_flag,
                  (u16 *)resource->component_10, (const GameGraphicsResource *)resource->data);
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
    ArchiveIO_QueueCompressedRead(
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
        BattleTransfer_EnqueueAfterMapping(BattleObjectData_UploadSpriteTask,
                            resource, 0, 0);
        if (resource->component_14 == gBattleContext + 0x37608) {
            *(u16 *)(gBattleSystem + 0xDF6) =
                (*(u16 *)(gBattleSystem + 0xDF6) & ~1) | 1;
        }
    }

    if (resource->flags.bits.resource_index != 0 &&
        !resource->flags.bits.upload_complete) {
        BattleTransfer_EnqueueBeforeMapping(BattleObjectData_UploadTextureTask,
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
    return BattleObjectData_UploadSprites(task->resource);
}

void func_0202cc58(const void *source, void *destination, u32 size);
void func_0202cd2c(const void *source, void *destination, u32 size);
void *BattleTransfer_EnqueueBeforeMapping(int (*callback)(BattleObjectUploadTask *task),
                          BattleSceneResource *resource, int unknown_2,
                          int unknown_3);

void BattleObjectData_CopyResource(BattleSceneResource *resource,
                                   BattleObjectDataLoadState *load_state) {
    u32 copy_size = resource->allocation_size;

    if (resource->stream_state != 0) {
        copy_size -= *(u8 **)resource->stream_state -
                     (u8 *)resource->stream_state;
    }

    if (resource->data < load_state->data) {
        func_0202cd2c(resource->data, load_state->data, copy_size);
    } else {
        func_0202cc58(resource->data, load_state->data, copy_size);
    }

    load_state->component_04 = resource->component_04 == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_04 -
                   (u32)resource->data);
    load_state->component_08 = resource->component_08 == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_08 -
                   (u32)resource->data);
    load_state->component_0c = resource->component_0c == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_0c -
                   (u32)resource->data);
    load_state->component_10 = resource->component_10 == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_10 -
                   (u32)resource->data);
    load_state->component_14 = resource->component_14 == 0
        ? 0
        : (void *)((u32)load_state->data + (u32)resource->component_14 -
                   (u32)resource->data);
    load_state->texture_set = resource->texture_set;
    load_state->stream_state = 0;
    load_state->texture_variant = resource->texture_variant;
    load_state->resource_id = resource->resource_id;
    load_state->flags.bits.copy_flag = resource->flags.bits.copy_flag;
    load_state->flags.bits.upload_complete =
        resource->flags.bits.upload_complete;

    if (load_state->flags.bits.resource_index == 0 ||
        load_state->flags.bits.upload_complete) {
        return;
    }

    BattleTransfer_EnqueueBeforeMapping(BattleObjectData_UploadTextureTask,
                        load_state, 0, 0);
    *(u16 *)(gBattleSystem + 3574) =
        (*(u16 *)(gBattleSystem + 3574) & ~1) | 1;
}

BattleQueuedTask *BattleObjectData_QueueLoadAndMarkPending(
        BattleObjectDataLoadState *load_state, s32 resource_id) {
    BattleQueuedTask *task =
        BattleObjectData_QueueLoad(load_state, resource_id);

    load_state->flags.raw |= 1 << 29;
    return task;
}
