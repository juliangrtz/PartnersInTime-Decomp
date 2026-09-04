#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_archive.h>
#include <game/battle_context.h>
#include <game/battle_task_queue.h>

extern void *gBattleSystem;
extern const void *gBattleAIArchivePaths[BATTLE_AI_ARCHIVE_COUNT];
extern const u32 data_ov002_020bf2c8[];

extern const u8 data_ov002_020bec0c[];
extern const u8 data_ov002_020bec94[];
extern const u8 data_ov002_020bec84[];
extern const u8 data_ov002_020bec30[];
extern const u8 data_ov002_020bed28[];
extern const u8 data_ov002_020bed3c[];
extern const u8 data_ov002_020bed50[];
extern const u8 data_ov002_020bed64[];
extern const u8 data_ov002_020bed78[];
extern const u8 data_ov002_020becc4[];
extern const u8 data_ov002_020becd8[];
extern const u8 data_ov002_020becec[];
extern const u8 data_ov002_020bed14[];
extern const u8 data_ov002_020bed00[];

void *func_02029928(u32 heap_id, u32 size, const void *allocator,
                    int use_default);
int func_0202b734(void *system, int handle, int mode);
void func_ov002_0208ddd0(BattleQueuedTask *task);

void BattleSpecialHandle_ClearTask(BattleQueuedTask *task);
void BattleSpecialHandle_ReloadTask(BattleQueuedTask *task);
void BattleAIArchives_Load(BattleQueuedTask *task);

void BattleAI_UpdateControlMask(int mask, int enabled) {
    if (enabled) {
        ((BattleContext *)gBattleContext)->ai_control_mask |= mask;
    } else {
        ((BattleContext *)gBattleContext)->ai_control_mask &= ~mask;
    }
}

int BattleActor_IsHitLocked(int actor_id) {
    return ((u32)(BattleActor_GetById(actor_id)->flags << 22) >> 31) != 0;
}

void BattleSpecialHandle_QueueReload(void *handle_slot) {
    BattleTaskQueue_Enqueue(BattleSpecialHandle_ReloadTask, handle_slot);
}

void BattleSpecialHandle_ReloadTask(BattleQueuedTask *task) {
    void **handle_slot = (void **)task->argument;

    func_0202b734(gBattleSystem, (int)*handle_slot, 0);
    task->callback = BattleSpecialHandle_ClearTask;
}

void BattleSpecialHandle_ClearTask(BattleQueuedTask *task) {
    void **handle_slot = (void **)task->argument;

    *handle_slot = 0;
    task->callback = 0;
}

void BattleAI_Initialize(void) {
    ScriptVm *vm = &((BattleContext *)gBattleContext)->battle_ai_vm;

    vm->unknown_00 = 0;
    vm->unknown_04 = 0;
    vm->command_descriptors = data_ov002_020bf2c8;
    vm->command_handler = BattleAI_DispatchOpcode;
    BattleTaskQueue_Enqueue(BattleAIArchives_Load, 0);
    BattleAI_TaskPoolsInit();
}

void BattleAIArchives_Load(BattleQueuedTask *task) {
    const void *allocators[BATTLE_AI_ARCHIVE_COUNT] = {
        data_ov002_020bec0c,
        data_ov002_020bec94,
        data_ov002_020bec84,
        data_ov002_020bec30,
        data_ov002_020bed28,
        data_ov002_020bed3c,
        data_ov002_020bed50,
        data_ov002_020bed64,
        data_ov002_020bed78,
        data_ov002_020becc4,
        data_ov002_020becd8,
        data_ov002_020becec,
        data_ov002_020bed14,
        data_ov002_020bed00
    };
    u32 archive_index = 0;
    u32 request_offset = 0;

    do {
        s32 size;

        ((BattleContext *)gBattleContext)->active_object_resource =
            (BattleObjectResourceRequest *)
                ((u8 *)((BattleContext *)gBattleContext)->ai_archives +
                 request_offset);
        ((BattleContext *)gBattleContext)
            ->active_object_resource->descriptor =
            gBattleAIArchivePaths[archive_index];
        size = BattleArchive_GetDescriptorSize(
            gBattleSystem,
            ((BattleContext *)gBattleContext)
                ->active_object_resource->descriptor);
        if (size > 0) {
            ((BattleContext *)gBattleContext)
                ->active_object_resource->archive_cursor =
                func_02029928(
                    ((BattleContext *)gBattleContext)->runtime.heap_id,
                    size, allocators[archive_index], 1);
            BattleArchive_Open(
                gBattleSystem,
                ((BattleContext *)gBattleContext)
                    ->active_object_resource->archive_cursor,
                size,
                ((BattleContext *)gBattleContext)
                    ->active_object_resource->descriptor);
        }
        archive_index++;
        request_offset += sizeof(BattleObjectResourceRequest);
    } while (archive_index < BATTLE_AI_ARCHIVE_COUNT);

    task->callback = func_ov002_0208ddd0;
}
