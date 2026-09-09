#include <game/battle_context.h>
#include <game/battle_scene.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>

extern u8 *gBattleSystem;

u32 ArchiveIO_PrepareCompressedRead(u8 *system, BattleTextureUploadRequest *request,
                  void *source, void *component);
void func_0202cbd4(void *destination, int value, u32 size);

void BattleObjectData_PrepareBodyDecodeTask(BattleQueuedTask *task) {
    BattleSceneResource *resource =
        (BattleSceneResource *)task->argument;
    s32 component_size;
    u8 *stream_tail;
    u32 total_size;

    resource->component_0c =
        (u8 *)((BattleContext *)gBattleContext)
            ->current_texture_upload->destination +
        (((BattleContext *)gBattleContext)
             ->current_texture_upload->size & ~3);
    resource->component_10 =
        (u8 *)resource->component_0c +
        ((BattleRenderModel_GetTextureConversionSize(resource->data,
                        resource->flags.bits.copy_flag == 0) + 3) /
            4) * 4;
    resource->component_14 =
        (u8 *)resource->component_10 +
        ((BattleModel_GetScreenTextureConversionSize(1, !resource->flags.bits.copy_flag,
                        resource->data) + 3) / 4) * 4;

    ++((BattleContext *)gBattleContext)->current_texture_upload;
    ((BattleContext *)gBattleContext)->current_texture_upload->texture_id =
        ((BattleContext *)gBattleContext)->texture_body_id + 1;
    ((BattleContext *)gBattleContext)->current_texture_upload->size =
        (ArchiveIO_PrepareCompressedRead(
             gBattleSystem,
             ((BattleContext *)gBattleContext)->current_texture_upload,
             ((BattleContext *)gBattleContext)
                 ->current_texture_upload->source,
             ((BattleContext *)gBattleContext)
                 ->current_texture_upload->component) + 3) & ~3;

    component_size = (u8 *)resource->component_14 - resource->data;
    total_size =
        ((((BattleContext *)gBattleContext)
              ->current_texture_upload->size + 3) & ~3) +
        component_size;
    if (resource->allocation_size >= total_size) {
        component_size = total_size;
    }
    stream_tail = resource->data + ((component_size + 3) / 4) * 4;

    if (resource->allocation_size -
            (u32)(stream_tail - resource->data) >
        100) {
        func_0202cbd4(stream_tail, 0, 100);
        resource->stream_state = stream_tail;
        *(u8 **)resource->stream_state = stream_tail + 100;
    } else {
        resource->stream_state = 0;
    }

    if (resource->flags.bits.load_pending) {
        GameGraphics_InitOffsetDecoder(
            &((BattleContext *)gBattleContext)->texture_decode,
            0, 1, 1, (u16 *)resource->component_0c, (const GameGraphicsResource *)resource->data);
        resource->flags.bits.upload_complete = 0;
        resource->flags.bits.copy_flag = 0;
    } else {
        if (resource->allocation_size < total_size) {
            resource->component_14 = gBattleContext + 0x37608;
        }
        GameGraphics_InitOffsetDecoder(
            &((BattleContext *)gBattleContext)->texture_decode,
            0, 1, !resource->flags.bits.upload_complete,
            (u16 *)resource->component_0c, (const GameGraphicsResource *)resource->data);
    }
    task->callback = BattleObjectData_WaitBodyDecodeTask;
}
