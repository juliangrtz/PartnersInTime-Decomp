#ifndef PIT_GAME_FIELD_AREA_H
#define PIT_GAME_FIELD_AREA_H

#include <game/field_script_manager.h>

typedef struct FieldAreaContext {
    const void *vtable;
    void *owner;
    u8 unknown_0008[34];
    s8 unknown_2a;
    u8 unknown_002b[9153];
    void *archive;
    u8 unknown_23f0[4];
    u16 room_id;
    struct {
        u8 main : 4, temporary : 4;
    } heaps;
    u8 unknown_23f7[1];
    struct {
        u16 screen : 1, party_active : 1, mode : 2, unknown_04_05 : 2, unknown_06_14 : 9, unknown_15 : 1;
    } flags;
    s8 area_value;
    s8 unknown_23fb;
    s16 unknown_23fc;
    s8 unknown_23fe;
    u8 unknown_23ff[1];
    s16 unknown_2400;
    u8 unknown_2402[8];
    s16 brightness;
    u8 unknown_240c[40];
    s32 camera_x, camera_y;
    u8 unknown_243c[52];
    s32 unknown_2470[6];
    u8 unknown_2488[56];
    u32 unknown_24c0;
    u16 input_masks[2];
    u8 unknown_24c8[16];
    struct {
        u16 unknown_00_13 : 14, visible : 1, moving : 1;
    } hud;
    u8 unknown_24da[34];
    void *auxiliary;
    u8 unknown_2500[96];
    void *party_order;
    void *unknown_2564;
    void *unknown_2568;
    void *party;
    FieldScriptManager scripts;
    u8 unknown_29d4[148];
    void *shared_resources;
    void *unknown_2a6c;
    void *unknown_2a70;
    u8 unknown_2a74[180];
    u8 effect_owners[8];
    u16 state;
    u8 unknown_2b32[2];
    s8 special_resources[6];
    u8 unknown_2b3a[150];
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
FieldAreaContext *FieldArea_Construct(FieldAreaContext *field);
FieldAreaContext *FieldArea_Destroy(FieldAreaContext *field);
void FieldArea_CaptureSnapshot(const FieldAreaContext *field, FieldAreaSnapshot *snapshot);
#ifdef __cplusplus
}
#endif
#endif
