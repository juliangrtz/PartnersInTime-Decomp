#include <game/field_background.h>
extern void func_02036988(vu32 *, int, int, int, int);
extern void func_0203b578(int, const void *, void *, u32);
extern void DC_FlushRange(const void *, u32);
extern void func_020385d8(const void *, u32, u32);
extern void func_02038640(const void *, u32, u32);
extern void func_020382a8(const void *, u32, u32);
extern void func_020381d8(const void *, u32, u32);
extern void func_02038378(const void *, u32, u32);
extern void func_02038310(const void *, u32, u32);
extern void func_02038240(const void *, u32, u32);
extern void func_020383e0(const void *, u32, u32);


static inline void SetScroll(vu32 *registers, int x, int y)
{
    *registers = (x & 0x1ff) | ((y << 16) & 0x1ff0000);
}

void FieldBackground_SetScroll(FieldBackgroundTransfers *transfers, int layer, int x, int y)
{
    if (!transfers->background->screen) {
        switch (layer) {
        case 0: SetScroll((vu32 *)0x04000014, x, y); break;
        case 1: SetScroll((vu32 *)0x04000018, x, y); break;
        case 2: SetScroll((vu32 *)0x0400001c, x, y); break;
        }
    } else {
        switch (layer) {
        case 0: SetScroll((vu32 *)0x04001014, x, y); break;
        case 1: SetScroll((vu32 *)0x04001018, x, y); break;
        case 2: SetScroll((vu32 *)0x0400101c, x, y); break;
        }
    }
}

void FieldBackground_UploadPalette(FieldBackgroundTransfers *transfers)
{
    FieldBackground *background = transfers->background;
    DC_FlushRange(background->palette, 512);
    if (!background->screen)
        func_02038640(background->palette, 0, 512);
    else
        func_020385d8(background->palette, 0, 512);
    transfers->palette_dirty = 0;
}

void *FieldBackground_GetTilemapAddress(FieldBackgroundTransfers *transfers, int layer)
{
    if (transfers->background->screen)
        return (void *)(0x06205000 + (layer << 12));
    else
        return (void *)(0x06005000 + (layer << 12));
}

void FieldBackground_QueueTransfer(FieldBackgroundTransfers *transfers, const void *source,
                   void *destination, u32 size)
{
    int i;
    int count = transfers->count;
    for (i = 0; i < count; ++i) {
        if (transfers->sources[i] == source && transfers->destinations[i] == destination)
            return;
    }
    transfers->sources[count] = source;
    transfers->destinations[transfers->count] = destination;
    transfers->sizes[transfers->count] = size;
    ++transfers->count;
}

void FieldBackground_TransferQueued(FieldBackgroundTransfers *transfers)
{
    int i;
    for (i = 0; i < transfers->count; ++i) {
        func_0203b578(3, transfers->sources[i], transfers->destinations[i], transfers->sizes[i]);
        transfers->sources[i] = 0;
        transfers->destinations[i] = 0;
        transfers->sizes[i] = 0;
    }
    transfers->count = 0;
}

void FieldBackground_QueueBlend(FieldBackgroundTransfers *transfers, int planes_a, int planes_b,
                   int coefficient_a, int coefficient_b)
{
    transfers->planes_a = planes_a;
    transfers->planes_b = planes_b;
    transfers->coefficient_a = coefficient_a;
    transfers->coefficient_b = coefficient_b;
    transfers->blend_dirty = 1;
}

void FieldBackground_ApplyBlend(FieldBackgroundTransfers *transfers)
{
    if (!transfers->background->screen)
        func_02036988((vu32 *)0x04000050, transfers->planes_a, transfers->planes_b,
                  transfers->coefficient_a, transfers->coefficient_b);
    else
        func_02036988((vu32 *)0x04001050, transfers->planes_a, transfers->planes_b,
                  transfers->coefficient_a, transfers->coefficient_b);
    transfers->planes_a = 0;
    transfers->planes_b = 0;
    transfers->coefficient_a = 0;
    transfers->coefficient_b = 0;
    transfers->blend_dirty = 0;
}

int FieldBackground_GetLayerWidth(FieldBackground *background, int layer)
{
    return 32 * (((background->wide_layers >> layer) & 1) + 1);
}
