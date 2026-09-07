#include <game/field_background.h>
#include <game/task.h>
#include <game/input.h>
extern "C" {
#include <game/heap.h>
extern GameTaskVTable data_02050ce4;
extern GameFrameTiming data_02060b2c;
extern void DC_FlushRange(const void *, u32);

void FieldBackground_ReversePaletteEffect(FieldBackground *background, int index)
{
    FieldBackgroundPaletteEffect *effect = &background->palette_effects[index];
    effect->time_q8 = (effect->duration << 8) - effect->time_q8;
    effect->mode = -effect->mode;
}

FieldBackgroundTransfers *FieldBackground_InitTransfers(FieldBackgroundTransfers *transfers,
    u32 priority, u32 unused, FieldBackground *background)
{
    GameIrqTask_Init((GameIrqTask *)transfers, priority, unused, background);
    ((GameIrqTask *)transfers)->vtable = &data_02050ce4;
    transfers->state = 0;
    for (int i = 0; i < 16; ++i) {
        transfers->sources[i] = 0;
        transfers->destinations[i] = 0;
    }
    transfers->count = 0;
    transfers->planes_a = 0;
    transfers->planes_b = 0;
    transfers->coefficient_a = 0;
    transfers->coefficient_b = 0;
    transfers->blend_dirty = 0;
    transfers->palette_dirty = 0;
    return transfers;
}

FieldBackgroundTransfers *FieldBackground_DestroyTransfers(FieldBackgroundTransfers *transfers)
{
    ((GameIrqTask *)transfers)->vtable = &data_02050ce4;
    GameIrqTask_DestroyBase((GameIrqTask *)transfers);
    return transfers;
}

FieldBackgroundTransfers *FieldBackground_DeleteTransfers(FieldBackgroundTransfers *transfers)
{
    ((GameIrqTask *)transfers)->vtable = &data_02050ce4;
    GameIrqTask_DestroyBase((GameIrqTask *)transfers);
    GameHeap_Delete(transfers);
    return transfers;
}

void FieldBackground_UpdateTransfers(FieldBackgroundTransfers *transfers)
{
    FieldBackground *background = transfers->background;
    if (data_02060b2c.bits.waiting_vblank) {
        switch (transfers->state) {
        case 0:
            transfers->set_scroll(0, 0, 0);
            transfers->set_scroll(1, 0, 0);
            transfers->set_scroll(2, 0, 0);
            ++transfers->state;
            break;
        case 1:
            if (background->is_ready()) transfers->state = 255;
            break;
        case 255:
            if (background->dirty_tilemaps[0] || background->dirty_tilemaps[1] || background->dirty_tilemaps[2]) {
                DC_FlushRange(background->tilemaps[0], 4096);
                DC_FlushRange(background->tilemaps[1], 4096);
                DC_FlushRange(background->tilemaps[2], 4096);
                transfers->upload_dirty_tilemaps();
            }
            transfers->set_scroll(0, background->scroll_x[0] >> 8, background->scroll_y[0] >> 8);
            transfers->set_scroll(1, background->scroll_x[1] >> 8, background->scroll_y[1] >> 8);
            transfers->set_scroll(2, background->scroll_x[2] >> 8, background->scroll_y[2] >> 8);
            if (transfers->palette_dirty == 1) {
                transfers->upload_palette();
            } else {
                if (background->palette_state.bits.animation == 1 || background->palette_state.bits.effects == 1
                    || transfers->palette_dirty == 2) {
                    background->upload_all_palettes();
                    if (transfers->palette_dirty == 2) transfers->palette_dirty = 0;
                }
            }
            if (transfers->count) transfers->transfer_queued();
            if (transfers->blend_dirty == 1) transfers->apply_blend();
            break;
        }
    }
}
}
