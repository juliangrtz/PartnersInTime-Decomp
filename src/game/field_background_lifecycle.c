#include <game/field_background.h>
#include <game/task.h>
#include <game/input.h>
#include <game/heap.h>
extern void *data_02050d60[];
extern GameDisplayWork data_0206032c;

FieldBackground *FieldBackground_Init(FieldBackground *background, u32 priority, u32 unused, void *argument,
    u8 screen, u16 mode, int heap, int flags, u32 resource_id, s16 x, s16 y,
    u32 unknown_098, u32 unknown_094, u8 option)
{
    int i;
    FieldBackgroundTransfers *transfers;
    GameTask_Init((GameTask *)background, priority, unused, argument);
    background->vtable = data_02050d60;
    if (data_0206032c.display_mode == 1) background->screen = 0;
    else background->screen = screen;
    background->unknown_764 = mode;
    background->resource_id = resource_id;
    background->load_status = 0;
    background->heap = heap;
    background->unknown_785 = flags;
    background->scroll_parameter_x = x;
    background->scroll_parameter_y = y;
    background->scroll_x[0] = 0;
    background->scroll_x[1] = 0;
    background->scroll_x[2] = 0;
    background->scroll_y[0] = 0;
    background->scroll_y[1] = 0;
    background->scroll_y[2] = 0;
    background->origin_x = 0;
    background->origin_y = 0;
    background->unknown_754 = 0;
    background->unknown_098 = unknown_098;
    background->unknown_094 = unknown_094;
    background->unknown_787 = 0;
    background->palette_state.bits.animation = 0;
    background->palette_state.bits.effects = 0;
    background->palette_state.bits.upload_full_palette = 0;
    background->palette_effect_count = 0;
    background->palette_animation_count = 0;
    background->palette_animation_speed = 256;
    background->tilemap_patches = 0;
    background->tilemaps[0] = 0;
    background->tilemaps[1] = 0;
    background->tilemaps[2] = 0;
    background->extended_palettes[0] = 0;
    background->extended_palettes[1] = 0;
    background->extended_palettes[2] = 0;
    background->palette = 0;
    for (i = 0; i < 16; ++i) background->tile_sources[i] = 0;
    background->palette_effects = 0;
    background->flags_78d.bits.unknown_04 = option;
    transfers = GameHeap_New(sizeof(FieldBackgroundTransfers), background->heap, 0, 0);
    if (transfers) transfers = FieldBackground_InitTransfers(transfers, 8, 0, background);
    background->transfers = transfers;
    return background;
}

FieldBackground *FieldBackground_Destroy(FieldBackground *background)
{
    int i, j;
    background->vtable = data_02050d60;
    GameIrqTask_DeleteSafe((GameIrqTask *)background->transfers);
    for (i = 15; i >= 0; --i) {
        const u32 *source = background->tile_sources[i];
        if (source) {
            GameHeap_Free((void *)source);
            for (j = 0; j < i; ++j) {
                if (source == background->tile_sources[j]) background->tile_sources[j] = 0;
            }
        }
    }
    if (background->palette_animation_states) GameHeap_DeleteArray(background->palette_animation_states);
    if (background->palette_animation_times) GameHeap_DeleteArray(background->palette_animation_times);
    if (background->tilemap_patches) GameHeap_DeleteArray(background->tilemap_patches);
    GameHeap_DeleteArray(background->base_palette);
    if (background->extended_palettes[2]) GameHeap_DeleteArray(background->extended_palettes[2]);
    if (background->extended_palettes[1]) GameHeap_DeleteArray(background->extended_palettes[1]);
    if (background->extended_palettes[0]) GameHeap_DeleteArray(background->extended_palettes[0]);
    GameHeap_DeleteArray(background->palette);
    GameHeap_Free(background->resource_data);
    GameHeap_DeleteArray(background->tilemaps[2]);
    GameHeap_DeleteArray(background->tilemaps[1]);
    GameHeap_DeleteArray(background->tilemaps[0]);
    GameTask_DestroyBase((GameTask *)background);
    return background;
}

FieldBackground *FieldBackground_Delete(FieldBackground *background)
{
    int i, j;
    background->vtable = data_02050d60;
    GameIrqTask_DeleteSafe((GameIrqTask *)background->transfers);
    for (i = 15; i >= 0; --i) {
        const u32 *source = background->tile_sources[i];
        if (source) {
            GameHeap_Free((void *)source);
            for (j = 0; j < i; ++j) {
                if (source == background->tile_sources[j]) background->tile_sources[j] = 0;
            }
        }
    }
    if (background->palette_animation_states) GameHeap_DeleteArray(background->palette_animation_states);
    if (background->palette_animation_times) GameHeap_DeleteArray(background->palette_animation_times);
    if (background->tilemap_patches) GameHeap_DeleteArray(background->tilemap_patches);
    GameHeap_DeleteArray(background->base_palette);
    if (background->extended_palettes[2]) GameHeap_DeleteArray(background->extended_palettes[2]);
    if (background->extended_palettes[1]) GameHeap_DeleteArray(background->extended_palettes[1]);
    if (background->extended_palettes[0]) GameHeap_DeleteArray(background->extended_palettes[0]);
    GameHeap_DeleteArray(background->palette);
    GameHeap_Free(background->resource_data);
    GameHeap_DeleteArray(background->tilemaps[2]);
    GameHeap_DeleteArray(background->tilemaps[1]);
    GameHeap_DeleteArray(background->tilemaps[0]);
    GameTask_DestroyBase((GameTask *)background);
    GameHeap_Delete(background);
    return background;
}
