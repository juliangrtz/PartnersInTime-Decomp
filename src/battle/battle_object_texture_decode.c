#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>

extern u8 *gBattleSystem;

u32 func_0202c5a8(u8 *system, BattleTextureUploadRequest *request,
                  void *source, void *component);
u32 func_0200f844(const void *resource, u32 copy_flag);
u32 func_0200c300(int texture_id, int copy_flag, const void *resource);
void func_0202cbd4(void *destination, int value, u32 size);
void func_02009d24(BattleTextureDecodeState *state, int texture_id,
                   int unknown_2, int copy_flag, void *component_10,
                   u8 *data);

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
        ((func_0200f844(resource->data,
                        resource->flags.bits.copy_flag == 0) + 3) /
            4) * 4;
    resource->component_14 =
        (u8 *)resource->component_10 +
        ((func_0200c300(1, !resource->flags.bits.copy_flag,
                        resource->data) + 3) / 4) * 4;

    ++((BattleContext *)gBattleContext)->current_texture_upload;
    ((BattleContext *)gBattleContext)->current_texture_upload->texture_id =
        ((BattleContext *)gBattleContext)->texture_body_id + 1;
    ((BattleContext *)gBattleContext)->current_texture_upload->size =
        (func_0202c5a8(
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
        func_02009d24(
            &((BattleContext *)gBattleContext)->texture_decode,
            0, 1, 1, resource->component_0c, resource->data);
        resource->flags.bits.upload_complete = 0;
        resource->flags.bits.copy_flag = 0;
    } else {
        if (resource->allocation_size < total_size) {
            resource->component_14 = gBattleContext + 0x37608;
        }
        func_02009d24(
            &((BattleContext *)gBattleContext)->texture_decode,
            0, 1, !resource->flags.bits.upload_complete,
            resource->component_0c, resource->data);
    }
    task->callback = BattleObjectData_WaitBodyDecodeTask;
}
