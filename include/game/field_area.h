#ifndef PIT_GAME_FIELD_AREA_H
#define PIT_GAME_FIELD_AREA_H

#include <game/field_script_manager.h>
#include <game/field_bounds_records.h>
#include <game/field_navigation.h>

typedef struct FieldVariablePlacement FieldVariablePlacement;
struct FieldPaletteCrossfade;

typedef struct FieldAreaContext {
    const void *vtable;
    void *owner;
    u8 unknown_0008[34];
    s8 unknown_2a;
    u8 unknown_002b[8913];
    const FieldQuadRegionRecord *quad_records;
    u8 unknown_2300[188];
    const FieldNavigationResource *navigation_resource;
    const FieldNavigationRecord *navigation_records;
    FieldNavigationSurface *navigation_surfaces;
    u32 navigation_surface_count;
    const FieldBoundaryRecord *boundary_records;
    FieldBoundaryRuntime *boundaries;
    u32 boundary_count;
    const FieldPairedBoundsRecord *paired_records;
    u8 unknown_23dc[16];
    void *archive;
    u16 unknown_23f0, unknown_23f2;
    u16 room_id;
    struct {
        u8 main : 4, temporary : 4;
    } heaps;
    u8 unknown_23f7[1];
    struct {
        u16 screen : 1, party_active : 1, mode : 2, unknown_04_05 : 2, unknown_06 : 1;
        u16 unknown_07_12 : 6, unknown_13 : 1, unknown_14 : 1, unknown_15 : 1;
    } flags;
    s8 area_value;
    s8 unknown_23fb;
    s16 unknown_23fc;
    s8 unknown_23fe;
    u8 unknown_23ff[1];
    s16 unknown_2400;
    struct {
        u8 unknown_00 : 1, unknown_01 : 1;
        s8 unknown_02_07 : 6;
    } unknown_2402;
    u8 unknown_2403;
    u16 unknown_2404, unknown_2406;
    s16 unknown_2408, brightness, unknown_240c;
    u16 unknown_240e;
    struct {
        u32 unknown_00 : 1, unknown_01 : 1, unknown_02_07 : 6, unknown_08_13 : 6;
        u32 unknown_14_18 : 5, unknown_19_23 : 5, unknown_24_31 : 8;
    } unknown_2410;
    u16 unknown_2414, unknown_2416;
    s16 unknown_2418[6];
    s32 unknown_2424[4];
    s32 camera_x, camera_y;
    s32 unknown_243c[4];
    s16 unknown_244c[4];
    s32 unknown_2454[4];
    s16 unknown_2464[2];
    s32 unknown_2468[2];
    s32 unknown_2470[6];
    struct {
        u16 unknown_00 : 1, unknown_01 : 1, unknown_02_15 : 14;
    } unknown_2488;
    u16 unknown_248a;
    s32 unknown_248c[4];
    struct {
        u32 unknown_00 : 1, unknown_01 : 1, unknown_02_09 : 8;
        u32 unknown_10_19 : 10, unknown_20_29 : 10, unknown_30_31 : 2;
    } unknown_249c;
    s32 unknown_24a0[8];
    u32 unknown_24c0;
    u16 input_masks[2];
    u8 unknown_24c8[16];
    struct {
        u16 unknown_00_13 : 14, visible : 1, moving : 1;
    } hud;
    s16 unknown_24da, unknown_24dc;
    u8 unknown_24de[10];
    u32 paired_count;
    FieldPairedBoundsRuntime *paired_bounds;
    union { u32 unknown_24f0; u32 quad_region_count; };
    union {
        u8 unknown_24f4[8];
        struct { FieldQuadRegion *quad_regions; const FieldVariablePlacement *variable_records; };
    };
    void *auxiliary;
    FieldNavigationChangeManager *navigation_changes;
    u8 unknown_2504[8], unknown_250c[44], unknown_2538[40];
    void *party_order;
    void *unknown_2564;
    void *unknown_2568;
    void *party;
    FieldScriptManager scripts;
    u8 unknown_29d4[4];
    struct FieldRuntimeEntity *entities[36];
    void *shared_resources;
    void *unknown_2a6c;
    void *unknown_2a70;
    u8 unknown_2a74[180];
    u8 effect_owners[8];
    union {
        u16 state;
        struct {
            u16 unknown_00_01 : 2, unknown_02_15 : 14;
        } state_bits;
    };
    union { u8 unknown_2b32[2]; struct { u8 entity_count, variable_count; }; };
    s8 special_resources[6];
    u8 unknown_2b3a[34], unknown_2b5c[52], unknown_2b90[52], unknown_2bc4[8];
    struct FieldPaletteCrossfade *palette_crossfade;
} FieldAreaContext;
typedef char FieldAreaContext_SizeCheck[sizeof(FieldAreaContext) == 11216 ? 1 : -1];
typedef struct FieldAreaSnapshot {
    u16 room_id;
    struct {
        u16 party_active : 1, mode : 2, unknown_03 : 1;
        s16 area_value : 8;
        u16 unknown_12_14 : 3, hud_visible : 1;
    } flags;
    s32 camera_x, camera_y;
    u32 unknown_0c;
} FieldAreaSnapshot;
typedef char FieldAreaSnapshot_SizeCheck[sizeof(FieldAreaSnapshot) == 16 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void FieldArea_InitializeQuadRegions(FieldAreaContext *field);
void FieldArea_CreateVariableEntities(FieldAreaContext *field);
FieldAreaContext *FieldArea_CopyState(FieldAreaContext *field, const FieldAreaContext *source);
void FieldArea_UpdateGraphics(FieldAreaContext *field);
FieldAreaContext *FieldArea_Construct(FieldAreaContext *field);
FieldAreaContext *FieldArea_Destroy(FieldAreaContext *field);
void FieldArea_CaptureSnapshot(const FieldAreaContext *field, FieldAreaSnapshot *snapshot);
#ifdef __cplusplus
}
#endif
#endif
