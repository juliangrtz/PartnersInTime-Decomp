#ifndef PIT_GAME_BATTLE_CONTEXT_H
#define PIT_GAME_BATTLE_CONTEXT_H

#include <game/battle_ai.h>
#include <game/battle_object.h>

enum BattleContextOffset {
    BATTLE_CONTEXT_BACKGROUND_ID_OFFSET = 0x3A,
    BATTLE_RUNTIME_STATE_OFFSET = 0xD000,
    BATTLE_RUNTIME_FLAGS_OFFSET = 0xD3A0
};

enum BattleContextConstant {
    BATTLE_COMMON_ASSET_COUNT = 21
};

enum BattleRuntimeFlag {
    BATTLE_RUNTIME_FLAG_COMMON_ASSET_LOAD_PENDING = 1 << 7,
    BATTLE_RUNTIME_FLAG_ASSET_LOAD_PENDING = 1 << 8,
    BATTLE_RUNTIME_FLAG_ALTERNATE_FORMATION = 1 << 16,
    BATTLE_RUNTIME_FLAG_BACKGROUND_BUFFER = 1 << 19,
    BATTLE_RUNTIME_FLAG_BACKGROUND_FADED_OUT = 1 << 20
};

typedef union BattleRuntimeFlags {
    u32 raw;
    struct {
        u32 unknown_00_07 : 8;
        u32 asset_load_pending : 1;
        u32 unknown_09_15 : 7;
        u32 alternate_formation : 1;
        u32 unknown_17_18 : 2;
        u32 background_buffer : 1;
        u32 background_faded_out : 1;
        u32 unknown_21_31 : 11;
    } bits;
} BattleRuntimeFlags;

typedef struct BattleArchiveReadRequest {
    u8 unknown_000[4];
    const u8 *archive_cursor;
    u8 unknown_008[4];
    u8 *destination;
    const void *descriptor;
    u32 read_size;
    u8 unknown_018[0x0C];
    u16 entry_index;
    u16 padding_026;
} BattleArchiveReadRequest;

typedef struct BattleCommonAssetArchive {
    const u8 *source;
    u32 unknown_004;
    u32 header[1];
} BattleCommonAssetArchive;

typedef struct BattleInterfaceLayer {
    u8 unknown_00[0x32];
    u16 allocation_size;
    s16 x;
    s16 y;
    u16 width;
    u16 height;
    u32 vram_offset;
} BattleInterfaceLayer;

typedef struct BattleTextureUploadRequest {
    u8 unknown_00[4];
    void *source;
    u8 unknown_08[4];
    void *destination;
    void *component;
    u32 size;
    u8 unknown_18[0x0C];
    u16 texture_id;
    u8 unknown_26[0x1A];
} BattleTextureUploadRequest;

typedef struct BattleTextureDecodeState {
    u8 unknown_00[0x24];
} BattleTextureDecodeState;

typedef struct BattleRuntimeState {
    u8 unknown_000[0x3A0];
    BattleRuntimeFlags flags;
    u8 unknown_3a4[0xDA4];
    u32 heap_id;
    u8 unknown_114c[0x0C];
    u32 resource_heap_id;
    u8 unknown_115c[4];
    BattleCommonAssetArchive common_assets;
} BattleRuntimeState;

typedef struct BattleContext {
    u8 unknown_0000[0x3A];
    u16 background_id;
    u8 unknown_003c[8];
    void *screen_assets[2];
    u8 unknown_004c[0xF4];
    BattleArchiveReadRequest asset_read;
    u8 unknown_0168[0x18];
    BattleTextureUploadRequest texture_upload_requests[4];
    BattleTextureUploadRequest *current_texture_upload;
    u8 unknown_0284[0x2C0];
    BattleObjectResourceRequest *active_object_resource;
    BattleTextureDecodeState texture_decode;
    u16 texture_body_id;
    u16 texture_tail_id;
    u16 texture_optional_id;
    u8 unknown_0572[0x6082];
    BattleInterfaceLayer interface_layer_0;
    u8 unknown_6634[0x14];
    BattleInterfaceLayer interface_layer_1;
    u8 unknown_6688[0x14];
    BattleInterfaceLayer interface_layer_2;
    u8 unknown_66dc[0x10];
    BattleInterfaceLayer interface_layer_3;
    u8 unknown_672c[0x68];
    BattleResourceModel *resource_models[72];
    BattleResourceStream resource_stream;
    u8 unknown_68c8[0x2C];
    void *interface_assets[2];
    void *common_asset_pointers[BATTLE_COMMON_ASSET_COUNT];
    void *common_asset_end;
    u8 unknown_6954[0x66AC];
    BattleRuntimeState runtime;
} BattleContext;

typedef char BattleArchiveReadRequest_SizeCheck[
    sizeof(BattleArchiveReadRequest) == 0x28 ? 1 : -1
];
typedef char BattleInterfaceLayer_SizeCheck[
    sizeof(BattleInterfaceLayer) == 0x40 ? 1 : -1
];
typedef char BattleTextureUploadRequest_SizeCheck[
    sizeof(BattleTextureUploadRequest) == 0x40 ? 1 : -1
];
typedef char BattleTextureDecodeState_SizeCheck[
    sizeof(BattleTextureDecodeState) == 0x24 ? 1 : -1
];
typedef char BattleRuntimeState_SizeCheck[
    sizeof(BattleRuntimeState) == 0x116C ? 1 : -1
];
typedef char BattleContext_KnownPrefixSizeCheck[
    sizeof(BattleContext) == 0xE16C ? 1 : -1
];

static inline BattleRuntimeState *BattleContext_GetRuntimeState(void) {
    return (BattleRuntimeState *)(gBattleContext + BATTLE_RUNTIME_STATE_OFFSET);
}

#endif
