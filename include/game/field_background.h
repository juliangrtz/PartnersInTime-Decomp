#ifndef PIT_GAME_FIELD_BACKGROUND_H
#define PIT_GAME_FIELD_BACKGROUND_H
#include <nitro.h>

typedef struct FieldBackgroundPaletteSource {
    u32 size;
    const u16 *colors;
} FieldBackgroundPaletteSource;

typedef struct FieldBackground {
    void **vtable;
    u8 unknown_004[0x70];
    u16 *tilemaps[3];
    u16 *extended_palettes[3];
    u16 *palette;
    u16 *base_palette;
    u8 unknown_094[0x584];
    FieldBackgroundPaletteSource palette_sources[3];
    u8 unknown_630[0x152];
    u8 screen;
    u8 color256_layers;
    u8 heap;
    u8 unknown_785[7];
    union {
        u8 raw;
        struct { u8 animation : 2, unknown_02_03 : 2, upload_full_palette : 1, unknown_05_07 : 3; } bits;
    } palette_state;
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
#endif
