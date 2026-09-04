#ifndef PIT_GAME_BATTLE_OBJECT_H
#define PIT_GAME_BATTLE_OBJECT_H

#include <nitro.h>

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

typedef BattleSceneResource BattleObjectDataLoadState;

typedef char BattleObjectDataLoadState_SizeCheck[
    sizeof(BattleObjectDataLoadState) == 0x30 ? 1 : -1];
typedef char BattleSceneResource_SizeCheck[
    sizeof(BattleSceneResource) == 0x30 ? 1 : -1];
typedef char BattleObjectResourceRequest_SizeCheck[
    sizeof(BattleObjectResourceRequest) == 0x2C ? 1 : -1];

BattleObjectDataLoadState *BattleObjectData_GetLoadState(int object_data_id);
int BattleObjectData_AllocateLoadBuffer(int object_data_id, u32 size);
int BattleObjectData_ConfigureLoad(int object_data_id, int resource_index,
                                   int compressed);
void BattleObjectData_CopyResource(BattleSceneResource *resource,
                                   BattleObjectDataLoadState *load_state);
int BattleObjectData_EnsureLoaded(u16 object_data_id, s32 resource_id);
int BattleObjectData_IsLoadPending(int object_data_id);
int BattleObjectData_ResetAll(void);
BattleObjectResourceRequest *BattleObjectData_ResolveSlot(u16 packed_id);

#endif
