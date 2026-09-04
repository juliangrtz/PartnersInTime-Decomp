#ifndef PIT_GAME_BATTLE_OBJECT_H
#define PIT_GAME_BATTLE_OBJECT_H

#include <nitro.h>

typedef struct BattleResourceStream {
    u8 *cursor;
    u32 *word_cursor;
    u8 *start;
    u32 capacity;
    u32 pending_word;
} BattleResourceStream;

typedef struct BattleResourceModel BattleResourceModel;

typedef struct BattleResourceModelVTable {
    u8 unknown_00[0x14];
    void (*destroy)(BattleResourceModel *model);
    u8 unknown_18[0x10];
    void (*finish)(BattleResourceModel *model);
    u8 unknown_2c[0xA0];
    void (*write_component)(BattleResourceModel *model,
                            BattleResourceStream *stream);
} BattleResourceModelVTable;

#ifdef __cplusplus
struct BattleResourceModel {
    virtual void unknown_00();
    virtual void unknown_04();
    virtual void unknown_08();
    virtual void unknown_0c();
    virtual void unknown_10();
    virtual void destroy();
    virtual void unknown_18();
    virtual void unknown_1c();
    virtual void unknown_20();
    virtual void unknown_24();
    virtual void finish();
    virtual void unknown_2c();
    virtual void unknown_30();
    virtual void unknown_34();
    virtual void unknown_38();
    virtual void unknown_3c();
    virtual void unknown_40();
    virtual void unknown_44();
    virtual void unknown_48();
    virtual void unknown_4c();
    virtual void unknown_50();
    virtual void unknown_54();
    virtual void unknown_58();
    virtual void unknown_5c();
    virtual void unknown_60();
    virtual void unknown_64();
    virtual void unknown_68();
    virtual void unknown_6c();
    virtual void unknown_70();
    virtual void unknown_74();
    virtual void unknown_78();
    virtual void unknown_7c();
    virtual void unknown_80();
    virtual void unknown_84();
    virtual void unknown_88();
    virtual void unknown_8c();
    virtual void unknown_90();
    virtual void unknown_94();
    virtual void unknown_98();
    virtual void unknown_9c();
    virtual void unknown_a0();
    virtual void unknown_a4();
    virtual void unknown_a8();
    virtual void unknown_ac();
    virtual void unknown_b0();
    virtual void unknown_b4();
    virtual void unknown_b8();
    virtual void unknown_bc();
    virtual void unknown_c0();
    virtual void unknown_c4();
    virtual void unknown_c8();
    virtual void write_component(BattleResourceStream *stream);
    u8 unknown_004[0x34];
    u8 *metadata;
    u8 unknown_03c[0x18];
    s16 component_index;
    u8 unknown_056[0x162];
};
#else
struct BattleResourceModel {
    BattleResourceModelVTable *vtable;
    u8 unknown_004[0x34];
    u8 *metadata;
    u8 unknown_03c[0x18];
    s16 component_index;
    u8 unknown_056[0x162];
};
#endif

typedef struct BattleSceneResource {
    u8 *data;
    void *component_04;
    void *component_08;
    void *component_0c;
    void *component_10;
    void *component_14;
    u32 unknown_18;
    void *stream_state;
    u16 object_data_id;
    u16 unknown_22;
    s32 resource_id;
    u32 allocation_size;
    union {
        u32 raw;
        struct {
            u32 resource_index : 27;
            u32 copy_flag : 1;
            u32 upload_complete : 1;
            u32 load_pending : 1;
            u32 processing : 1;
            u32 allocated : 1;
        } bits;
    } flags;
} BattleSceneResource;

typedef struct BattleObjectResourceRequest {
    u8 unknown_00[4];
    const u8 *archive_cursor;
    u8 unknown_08[4];
    u8 *destination;
    const void *descriptor;
    u32 read_size;
    u8 unknown_18[0x0C];
    u16 entry_index;
    u16 status;
    u8 unknown_28[4];
} BattleObjectResourceRequest;

typedef struct BattleObjectUploadTask {
    int (*callback)(struct BattleObjectUploadTask *task);
    BattleSceneResource *resource;
} BattleObjectUploadTask;

typedef BattleSceneResource BattleObjectDataLoadState;
struct BattleQueuedTask;

typedef char BattleObjectDataLoadState_SizeCheck[
    sizeof(BattleObjectDataLoadState) == 0x30 ? 1 : -1];
typedef char BattleSceneResource_SizeCheck[
    sizeof(BattleSceneResource) == 0x30 ? 1 : -1];
typedef char BattleResourceStream_SizeCheck[
    sizeof(BattleResourceStream) == 0x14 ? 1 : -1];
typedef char BattleResourceModel_SizeCheck[
    sizeof(BattleResourceModel) == 0x1B8 ? 1 : -1];
typedef char BattleObjectResourceRequest_SizeCheck[
    sizeof(BattleObjectResourceRequest) == 0x2C ? 1 : -1];
typedef char BattleObjectUploadTask_SizeCheck[
    sizeof(BattleObjectUploadTask) == 8 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

BattleObjectDataLoadState *BattleObjectData_GetLoadState(int object_data_id);
int BattleObjectData_AllocateLoadBuffer(int object_data_id, u32 size);
int BattleObjectData_ConfigureLoad(int object_data_id, int resource_index,
                                   int compressed);
struct BattleQueuedTask *BattleObjectData_QueueLoad(
    BattleObjectDataLoadState *load_state, s32 resource_id);
struct BattleQueuedTask *BattleObjectData_QueueLoadAndMarkPending(
    BattleObjectDataLoadState *load_state, s32 resource_id);
void BattleObjectData_BeginRebuildTask(struct BattleQueuedTask *task);
void BattleObjectData_RebuildNextComponentTask(struct BattleQueuedTask *task);
int BattleObjectData_UploadSpriteTask(BattleObjectUploadTask *task);
int BattleObjectData_UploadTextureTask(BattleObjectUploadTask *task);
void BattleObjectData_CompleteUploadTask(struct BattleQueuedTask *task);
void BattleObjectData_QueueFinalUploadTask(struct BattleQueuedTask *task);
void BattleObjectData_WaitTailDecodeTask(struct BattleQueuedTask *task);
void BattleObjectData_BeginTailDecodeTask(struct BattleQueuedTask *task);
void BattleObjectData_WaitBodyDecodeTask(struct BattleQueuedTask *task);
void BattleObjectData_PrepareBodyDecodeTask(struct BattleQueuedTask *task);
void BattleObjectData_SetupOptionalTextureTask(struct BattleQueuedTask *task);
void BattleObjectData_SetupTailTextureTask(struct BattleQueuedTask *task);
void BattleObjectData_BeginTextureSetupTask(struct BattleQueuedTask *task);
void BattleObjectData_CopyResource(BattleSceneResource *resource,
                                   BattleObjectDataLoadState *load_state);
int BattleObjectData_EnsureLoaded(u16 object_data_id, s32 resource_id);
int BattleObjectData_IsLoadPending(int object_data_id);
int BattleObjectData_ResetAll(void);
BattleObjectResourceRequest *BattleObjectData_ResolveSlot(u16 packed_id);

#ifdef __cplusplus
}
#endif

#endif
