#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>

extern u8 *gBattleSystem;

u32 func_0202c5a8(u8 *system, BattleTextureUploadRequest *request,
                  void *source, void *component);
u32 BattleArchive_GetEntrySize(u8 *system, void *source, u16 texture_id);
void BattleArchive_ReadAsync(u8 *system, BattleTextureUploadRequest *request,
                             int unknown_2, void *source, void *component,
                             int unknown_5);
void func_0202c510(u8 *system, BattleTextureUploadRequest *request,
                   void *destination);

void BattleObjectData_BeginTextureSetupTask(BattleQueuedTask *task) {
    BattleSceneResource *resource;

    ((BattleContext *)gBattleContext)->current_texture_upload =
        &((BattleContext *)gBattleContext)->texture_upload_requests[0];
    resource = (BattleSceneResource *)task->argument;
    ((BattleContext *)gBattleContext)->current_texture_upload->texture_id =
        ((BattleContext *)gBattleContext)->texture_body_id;
    ((BattleContext *)gBattleContext)->current_texture_upload->size =
        (func_0202c5a8(
             gBattleSystem,
             ((BattleContext *)gBattleContext)->current_texture_upload,
             ((BattleContext *)gBattleContext)
                 ->current_texture_upload->source,
             ((BattleContext *)gBattleContext)
                 ->current_texture_upload->component) + 3) & ~3;
    func_0202c510(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->current_texture_upload,
        resource->data);
    task->callback = BattleObjectData_SetupTailTextureTask;
}

void BattleObjectData_SetupTailTextureTask(BattleQueuedTask *task) {
    BattleSceneResource *resource =
        (BattleSceneResource *)task->argument;

    ++((BattleContext *)gBattleContext)->current_texture_upload;
    ((BattleContext *)gBattleContext)->current_texture_upload->texture_id =
        ((BattleContext *)gBattleContext)->texture_tail_id;
    ((BattleContext *)gBattleContext)->current_texture_upload->size =
        (BattleArchive_GetEntrySize(
             gBattleSystem,
             ((BattleContext *)gBattleContext)
                 ->current_texture_upload->source,
             ((BattleContext *)gBattleContext)
                 ->current_texture_upload->texture_id) + 3) & ~3;
    resource->component_04 = resource->data +
        (((BattleContext *)gBattleContext)
            ->current_texture_upload[-1].size & ~3);
    ((BattleContext *)gBattleContext)->current_texture_upload->destination =
        resource->component_04;
    BattleArchive_ReadAsync(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->current_texture_upload,
        0,
        ((BattleContext *)gBattleContext)->current_texture_upload->source,
        ((BattleContext *)gBattleContext)->current_texture_upload->component,
        0);

    if (((BattleContext *)gBattleContext)->texture_optional_id != 0xFFFF) {
        task->callback = BattleObjectData_SetupOptionalTextureTask;
    } else {
        task->callback = BattleObjectData_PrepareBodyDecodeTask;
    }
}

void BattleObjectData_SetupOptionalTextureTask(BattleQueuedTask *task) {
    BattleSceneResource *resource =
        (BattleSceneResource *)task->argument;

    ++((BattleContext *)gBattleContext)->current_texture_upload;
    ((BattleContext *)gBattleContext)->current_texture_upload->texture_id =
        ((BattleContext *)gBattleContext)->texture_optional_id;
    ((BattleContext *)gBattleContext)->current_texture_upload->size =
        (BattleArchive_GetEntrySize(
             gBattleSystem,
             ((BattleContext *)gBattleContext)
                 ->current_texture_upload->source,
             ((BattleContext *)gBattleContext)
                 ->current_texture_upload->texture_id) + 3) & ~3;
    resource->component_08 =
        (u8 *)((BattleContext *)gBattleContext)
            ->current_texture_upload[-1].destination +
        (((BattleContext *)gBattleContext)
            ->current_texture_upload[-1].size & ~3);
    ((BattleContext *)gBattleContext)->current_texture_upload->destination =
        resource->component_08;
    BattleArchive_ReadAsync(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->current_texture_upload,
        0,
        ((BattleContext *)gBattleContext)->current_texture_upload->source,
        ((BattleContext *)gBattleContext)->current_texture_upload->component,
        0);
    task->callback = BattleObjectData_PrepareBodyDecodeTask;
}
