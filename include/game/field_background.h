#ifndef PIT_GAME_FIELD_BACKGROUND_H
#define PIT_GAME_FIELD_BACKGROUND_H
#include <nitro.h>

typedef struct FieldBackgroundPaletteSource {
    u32 size;
    const u16 *colors;
} FieldBackgroundPaletteSource;

typedef struct FieldBackgroundResource {
    u32 size;
    const void *data;
} FieldBackgroundResource;

typedef struct FieldBackgroundPaletteEffect {
    s16 mode;
    u16 color;
    u16 extended_masks[3];
    u16 standard_mask;
    s32 duration;
    s32 time_q8;
    s16 speed_q8;
    u8 finish_mode;
    u8 disabled;
} FieldBackgroundPaletteEffect;

typedef struct FieldBackgroundTileFrame {
    u16 source_frame;
    u16 duration;
} FieldBackgroundTileFrame;

typedef struct FieldBackgroundTileResource {
    u32 layer : 2, mode : 2, destination_tile : 10, tile_count : 10, unknown_24_31 : 8;
    u16 unknown_04;
    u16 frame_count;
    FieldBackgroundTileFrame frames[1];
} FieldBackgroundTileResource;

typedef struct FieldBackgroundTileAnimation {
    const FieldBackgroundTileResource *resource;
    s32 time_q8;
    union {
        struct { u8 active; u8 unknown_09[3]; };
        struct { u32 unknown_00_07 : 8, frame : 16, unknown_24_31 : 8; } bits;
    };
} FieldBackgroundTileAnimation;

typedef struct FieldBackgroundBlendFrame {
    u16 coefficient_a : 5, coefficient_b : 5, unknown_10_15 : 6;
    u16 duration;
} FieldBackgroundBlendFrame;

typedef struct FieldBackgroundBlendAnimation {
    u16 count;
    u16 stop_at_end : 1, enabled : 1, planes_a : 5, planes_b : 5, unknown_12_15 : 4;
    FieldBackgroundBlendFrame frames[1];
} FieldBackgroundBlendAnimation;

typedef struct FieldBackgroundConfiguration {
    u32 unknown_00[2];
    u32 unknown_00_07 : 8;
    u32 first_x : 4, first_y : 4;
    u32 second_x : 4, second_y : 4;
    u32 third_x : 4, third_y : 4;
} FieldBackgroundConfiguration;

typedef struct FieldBackground FieldBackground;
typedef struct FieldBackgroundTransfers FieldBackgroundTransfers;
struct FieldBackground {
#ifdef __cplusplus
    /* Slots verified from the resident vtable at 0x02050d60. */
    virtual void unknown_00();
    virtual void unknown_04();
    virtual void unknown_08();
    virtual void unknown_0c();
    virtual void unknown_10();
    virtual void set_scroll(u8 layer, int x, int y);
    virtual void unknown_18();
    virtual void set_scroll_parameters(s16 x, s16 y);
    virtual void get_origin(s16 *x, s16 *y);
    virtual int is_ready();
    virtual const FieldBackgroundConfiguration *get_configuration();
    virtual const void *get_resource_650();
    virtual const void *get_resource_638();
    virtual void unknown_34();
    virtual void unknown_38();
    virtual void unknown_3c();
    virtual void unknown_40();
    virtual int restart_tile_animation(int index);
    virtual void update_tile_animations();
    virtual int stop_tile_animation(int index);
    virtual int start_tile_animation(int index);
    virtual const s16 *get_palette_animation(u8 *state, u16 **first, u16 **second, u16 **third);
    virtual void set_palette_animation(const s16 *table);
    virtual int has_palette_animation();
    virtual void initialize_palette_animation();
    virtual void start_palette_animation(int index, u8 flags);
    virtual void clear_palette_animation(int index);
    virtual void clear_all_palette_animations();
    virtual void pause_palette_animation(int index);
    virtual void resume_palette_animation(int index);
    virtual void restore_and_upload_palette();
    virtual void restore_palettes();
    virtual void update_palette_animations();
    virtual void allocate_palettes();
    virtual void upload_all_palettes();
    virtual void request_palette_upload();
    virtual int start_blend_animation(int index);
    virtual void update_blend_animation();
    virtual void *get_character_base(int block);
    virtual void get_layer_parameters(int layer, int *x, int *y);
    virtual int get_palette_span(u32 mask);
    virtual void set_palette_effects(FieldBackgroundPaletteEffect *effects, u8 count, int reset);
    virtual void configure_palette_effect(int index, u16 first_mask, u16 second_mask,
        u16 third_mask, u16 standard_mask, s16 mode, u16 color, int duration,
        u8 finish_mode, s16 speed, int time, u8 disabled);
    virtual void reset_palette_effect(u8 index);
    virtual u16 *edit_base_palette();
    virtual void upload_base_palette();
    virtual void update_palette_effects();
    virtual void reverse_palette_effect(int index);
    virtual int get_tile_bytes(int layer);
#else
    void **vtable;
#endif
    u8 unknown_004[0x24];
    u8 load_status;
    u8 unknown_029[3];
    FieldBackgroundTransfers *transfers;
    u32 resource_id;
    const u32 *tile_sources[16];
    u16 *tilemaps[3];
    u16 *extended_palettes[3];
    u16 *palette;
    u16 *base_palette;
    u32 unknown_094, unknown_098;
    const s16 *palette_components[3];
    FieldBackgroundPaletteEffect *palette_effects;
    void *tilemap_patches;
    u8 unknown_0b0[0x40];
    void *resource_data;
    u8 unknown_0f4[0x524];
    FieldBackgroundPaletteSource palette_sources[3];
    u8 unknown_630[4];
    const FieldBackgroundConfiguration *configuration;
    FieldBackgroundResource resource_638;
    u8 unknown_640[4];
    const s16 *palette_animation_table;
    u8 unknown_648[4];
    const u32 *blend_animation_table;
    FieldBackgroundResource resource_650;
    u8 unknown_658[0x10];
    FieldBackgroundTileAnimation tile_animations[16];
    const FieldBackgroundBlendAnimation *blend_animation;
    s32 blend_time_q8;
    u16 blend_frame;
    u16 unknown_732;
    s32 scroll_x[3], scroll_y[3];
    s32 origin_x, origin_y;
    u32 unknown_754;
    s32 *palette_animation_times;
    u8 *palette_animation_states;
    s32 palette_animation_speed;
    u16 unknown_764;
    s16 scroll_parameter_x, scroll_parameter_y;
    u16 map_width, map_height;
    u8 unknown_76e[0xe];
    u8 dirty_tilemaps[3];
    u8 layer_ids[3];
    u8 screen;
    u8 color256_layers;
    u8 heap;
    u8 unknown_785, unknown_786, unknown_787;
    u8 tile_animation_count;
    u8 unknown_789;
    u8 palette_animation_count;
    u8 palette_effect_count;
    union {
        u8 raw;
        struct { u8 animation : 2, effects : 2, upload_full_palette : 1, unknown_05_07 : 3; } bits;
    } palette_state;
    union {
        u8 raw;
        struct { u8 unknown_00_03 : 4, unknown_04 : 1, unknown_05_07 : 3; } bits;
    } flags_78d;
};

struct FieldBackgroundTransfers {
#ifdef __cplusplus
    /* Slots verified from the resident vtable at 0x02050ce4. */
    virtual void unknown_00();
    virtual void unknown_04();
    virtual void unknown_08();
    virtual void upload_dirty_tilemaps();
    virtual void upload_tilemap(u8 layer);
    virtual void upload_tilemap_edges(u8 flags, int x, int y, const u16 *source, void *destination);
    virtual void set_scroll(int layer, int x, int y);
    virtual void upload_palette();
    virtual void *get_tilemap_address(u8 layer);
    virtual void queue_transfer(const void *source, void *destination, u32 size);
    virtual void transfer_queued();
    virtual void queue_blend(int planes_a, int planes_b, int coefficient_a, int coefficient_b);
    virtual void apply_blend();
#else
    void **vtable;
#endif
    FieldBackground *background;
    u8 unknown_008[0x20];
    int state;
    const void *sources[16];
    void *destinations[16];
    u32 sizes[16];
    int planes_a, planes_b, coefficient_a, coefficient_b;
    u8 count;
    union {
        u8 dirty_flags;
        struct {
            u8 blend_dirty : 1;
            u8 palette_dirty : 1;
            u8 unknown_flags : 6;
        };
    };
    u8 unknown_0fe[2];
};

typedef char FieldBackgroundTransfers_SizeCheck[
    sizeof(FieldBackgroundTransfers) == 0x100 ? 1 : -1];
typedef char FieldBackground_SizeCheck[sizeof(FieldBackground) == 0x790 ? 1 : -1];
typedef char FieldBackgroundPaletteEffect_SizeCheck[
    sizeof(FieldBackgroundPaletteEffect) == 24 ? 1 : -1];
typedef char FieldBackgroundTileAnimation_SizeCheck[
    sizeof(FieldBackgroundTileAnimation) == 12 ? 1 : -1];
typedef char FieldBackgroundBlendFrame_SizeCheck[
    sizeof(FieldBackgroundBlendFrame) == 4 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void FieldBackground_ReversePaletteEffect(FieldBackground *background, int index);
void FieldBackground_UploadBasePalette(FieldBackground *background);
u16 *FieldBackground_EditBasePalette(FieldBackground *background);
void FieldBackground_ResetPaletteEffect(FieldBackground *background, int index);
void FieldBackground_ConfigurePaletteEffect(FieldBackground *background, int index,
    u16 first_mask, u16 second_mask, u16 third_mask, u16 standard_mask,
    s16 mode, u16 color, int duration, u8 finish_mode, s16 speed, int time, u8 disabled);
void FieldBackground_SetPaletteEffects(FieldBackground *background, FieldBackgroundPaletteEffect *effects,
                   u8 count, int reset);
int FieldBackground_GetPaletteSpan(FieldBackground *background, u32 mask);
void FieldBackground_GetLayerParameters(FieldBackground *background, int layer, int *x, int *y);
void *FieldBackground_GetCharacterBase(FieldBackground *background, int block);
void FieldBackground_UpdateBlendAnimation(FieldBackground *background);
void FieldBackground_RequestPaletteUpload(FieldBackground *background);
void FieldBackground_UpdatePaletteAnimations(FieldBackground *background);
void FieldBackground_RestoreAndUploadPalette(FieldBackground *background);
void FieldBackground_ResumePaletteAnimation(FieldBackground *background, int index);
void FieldBackground_PausePaletteAnimation(FieldBackground *background, int index);
void FieldBackground_ClearAllPaletteAnimations(FieldBackground *background);
void FieldBackground_ClearPaletteAnimation(FieldBackground *background, int index);
void FieldBackground_StartPaletteAnimation(FieldBackground *background, int index, u8 flags);
int FieldBackground_HasPaletteAnimation(FieldBackground *background);
void FieldBackground_InitializePaletteAnimation(FieldBackground *background);
void FieldBackground_SetPaletteAnimation(FieldBackground *background, const s16 *table);
const s16 *FieldBackground_GetPaletteAnimation(FieldBackground *background, u8 *state,
                        u16 **first, u16 **second, u16 **third);
int FieldBackground_StartTileAnimation(FieldBackground *background, int index);
int FieldBackground_StopTileAnimation(FieldBackground *background, int index);
void FieldBackground_UpdateTransfers(FieldBackgroundTransfers *transfers);
FieldBackgroundTransfers *FieldBackground_DeleteTransfers(FieldBackgroundTransfers *transfers);
FieldBackgroundTransfers *FieldBackground_DestroyTransfers(FieldBackgroundTransfers *transfers);
FieldBackgroundTransfers *FieldBackground_InitTransfers(FieldBackgroundTransfers *transfers,
    u32 priority, u32 unused, FieldBackground *background);
const void *FieldBackground_GetResource638(FieldBackground *background);
const void *FieldBackground_GetResource650(FieldBackground *background);
const FieldBackgroundConfiguration *FieldBackground_GetConfiguration(FieldBackground *background);
int FieldBackground_IsReady(FieldBackground *background);
void FieldBackground_GetOrigin(FieldBackground *background, s16 *x, s16 *y);
void FieldBackground_SetScrollParameters(FieldBackground *background, s16 x, s16 y);
FieldBackground *FieldBackground_Delete(FieldBackground *background);
FieldBackground *FieldBackground_Destroy(FieldBackground *background);
FieldBackground *FieldBackground_Init(FieldBackground *background, u32 priority, u32 unused, void *argument,
    u8 screen, u16 mode, int heap, int flags, u32 resource_id, s16 x, s16 y,
    u32 unknown_098, u32 unknown_094, u8 option);
int FieldBackground_GetTileBytes(FieldBackground *background, int layer);
void FieldBackground_ApplyBlend(FieldBackgroundTransfers *transfers);
void FieldBackground_QueueBlend(FieldBackgroundTransfers *transfers,
    int planes_a, int planes_b, int coefficient_a, int coefficient_b);
void FieldBackground_TransferQueued(FieldBackgroundTransfers *transfers);
void FieldBackground_QueueTransfer(FieldBackgroundTransfers *transfers,
    const void *source, void *destination, u32 size);
void *FieldBackground_GetTilemapAddress(FieldBackgroundTransfers *transfers, int layer);
void FieldBackground_UploadPalette(FieldBackgroundTransfers *transfers);
void FieldBackground_SetScroll(FieldBackgroundTransfers *transfers, int layer, int x, int y);
void FieldBackground_UploadTilemap(FieldBackgroundTransfers *transfers, int layer);
void FieldBackground_AllocatePalettes(FieldBackground *background);
void FieldBackground_UploadAllPalettes(FieldBackground *background);
void FieldBackground_RestorePalettes(FieldBackground *background);
#ifdef __cplusplus
}
#endif
#endif
