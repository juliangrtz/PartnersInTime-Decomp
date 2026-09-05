#ifndef PIT_GAME_BATTLE_CONTEXT_H
#define PIT_GAME_BATTLE_CONTEXT_H

#include <game/battle_ai.h>
#include <game/battle_enemy_data.h>
#include <game/battle_object.h>

struct BattleActor;

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
    BATTLE_RUNTIME_FLAG_PARTY_KNOCKOUT = 1 << 9,
    BATTLE_RUNTIME_FLAG_ALTERNATE_FORMATION = 1 << 16,
    BATTLE_RUNTIME_FLAG_BACKGROUND_BUFFER = 1 << 19,
    BATTLE_RUNTIME_FLAG_BACKGROUND_FADED_OUT = 1 << 20
};

typedef union BattleRuntimeFlags {
    u32 raw;
    struct {
        u32 global_property_23 : 1;
        u32 global_property_14 : 1;
        u32 global_property_29 : 1;
        u32 unknown_03_07 : 5;
        u32 asset_load_pending : 1;
        u32 unknown_09 : 1;
        u32 global_property_19 : 1;
        u32 unknown_11 : 1;
        u32 global_property_20 : 1;
        u32 damage_number_merge_pending : 1;
        u32 coin_particle_render_pending : 1;
        u32 status_visuals_active : 1;
        u32 alternate_formation : 1;
        u32 global_property_28 : 1;
        u32 unknown_18 : 1;
        u32 background_buffer : 1;
        u32 background_faded_out : 1;
        u32 unknown_21 : 1;
        u32 fast_forward_effects : 1;
        u32 unknown_23_31 : 9;
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

typedef union BattleInterfaceLayoutParameters {
    u32 raw;
    struct {
        u32 unknown_00_03 : 4;
        u32 row_offset : 5;
        u32 row_tiles_minus_one : 7;
        u32 unknown_16_31 : 16;
    } bits;
} BattleInterfaceLayoutParameters;

typedef union BattleInterfaceRenderParameters {
    u32 raw;
    struct {
        u32 unknown_00_19 : 20;
        u32 mode_a : 4;
        u32 mode_b : 4;
        u32 mode_c : 4;
    } bits;
} BattleInterfaceRenderParameters;

typedef union BattleInterfaceLayerFlags {
    u16 raw;
    struct {
        u16 row_bytes : 12;
        u16 resource_ready : 1;
        u16 notify_on_complete : 1;
        u16 clear_before_upload : 1;
        u16 state_15 : 1;
    } bits;
} BattleInterfaceLayerFlags;

typedef struct BattleInterfaceLayer {
    void *pixel_buffer;
    const u8 *resource_cursor;
    BattleInterfaceRenderParameters render;
    BattleInterfaceLayoutParameters layout;
    u8 unknown_10[4];
    void *asset_table;
    u8 unknown_18[0x18];
    BattleInterfaceLayerFlags flags;
    u16 allocation_size;
    s16 x;
    s16 y;
    u16 width;
    u16 height;
    u32 vram_offset;
} BattleInterfaceLayer;

typedef struct BattleInterfaceLayerState {
    BattleInterfaceLayer layer;
    const void *resource;
    u16 transition_progress;
    u8 transition_state;
    u8 transition_step;
} BattleInterfaceLayerState;

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
    void *object_texture_sources[3];
    BattleObjectTextureCatalog *object_texture_catalogs[3];
    u8 unknown_3bc[0x0C];
    BattleObjectDataLoadState object_data_load_states[72];
    union {
        u32 object_data_heap_ids[4];
        struct {
            u32 heap_id;
            u32 unknown_heap_ids[3];
        };
    };
    u32 resource_heap_id;
    u8 unknown_115c[4];
    BattleCommonAssetArchive common_assets;
} BattleRuntimeState;

typedef struct BattleContext {
    u8 unknown_0000[0x3A];
    u16 background_id;
    u8 unknown_003c[8];
    void *screen_assets[2];
    u8 unknown_004c[0xC0];
    u16 ai_control_mask;
    u8 unknown_010e[0x1E];
    s16 status_duration;
    u8 unknown_012e[0x12];
    BattleArchiveReadRequest asset_read;
    u8 unknown_0168[0x18];
    BattleTextureUploadRequest texture_upload_requests[4];
    BattleTextureUploadRequest *current_texture_upload;
    BattleObjectResourceRequest ai_archives[14];
    u8 unknown_04ec[0x58];
    BattleObjectResourceRequest *active_object_resource;
    BattleTextureDecodeState texture_decode;
    u16 texture_body_id;
    u16 texture_tail_id;
    u16 texture_optional_id;
    u8 unknown_0572[0x5F96];
    struct BattleActor *enemy_actors[8];
    u8 unknown_6528[0xCC];
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
    ScriptVm battle_ai_vm;
    u8 unknown_6964[0x669C];
    BattleRuntimeState runtime;
    u8 unknown_e16c[0x743C];
    BattleEnemyDataRequest enemy_data_requests[8];
    u8 unknown_25608[0x12000];
    BattleObjectFallbackStorage object_fallback;
} BattleContext;

typedef char BattleArchiveReadRequest_SizeCheck[
    sizeof(BattleArchiveReadRequest) == 0x28 ? 1 : -1
];
typedef char BattleInterfaceLayer_SizeCheck[
    sizeof(BattleInterfaceLayer) == 0x40 ? 1 : -1
];
typedef char BattleInterfaceLayerState_SizeCheck[
    sizeof(BattleInterfaceLayerState) == 0x48 ? 1 : -1
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
    sizeof(BattleContext) == 0x3F608 ? 1 : -1
];

static inline BattleRuntimeState *BattleContext_GetRuntimeState(void) {
    return (BattleRuntimeState *)(gBattleContext + BATTLE_RUNTIME_STATE_OFFSET);
}

#endif
