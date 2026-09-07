#ifndef PIT_GAME_FIELD_BACKGROUND_H
#define PIT_GAME_FIELD_BACKGROUND_H
#include <nitro.h>

typedef struct FieldBackground {
    void **vtable;
    u8 unknown_004[0x70];
    u16 *tilemaps[3];
    u8 unknown_080[0x0c];
    u16 *palette;
    u8 unknown_090[0x6f2];
    u8 screen;
    u8 wide_layers;
} FieldBackground;

typedef struct FieldBackgroundTransfers {
    void **vtable;
    FieldBackground *background;
    u8 unknown_008[0x24];
    const void *sources[16];
    void *destinations[16];
    u32 sizes[16];
    int planes_a, planes_b, coefficient_a, coefficient_b;
    u8 count;
    u8 blend_dirty : 1;
    u8 palette_dirty : 1;
    u8 unknown_flags : 6;
    u8 unknown_0fe[2];
} FieldBackgroundTransfers;

typedef char FieldBackgroundTransfers_SizeCheck[
    sizeof(FieldBackgroundTransfers) == 0x100 ? 1 : -1];

int FieldBackground_GetLayerWidth(FieldBackground *background, int layer);
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
#endif
