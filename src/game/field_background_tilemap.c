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

void FieldBackground_UploadTilemap(FieldBackgroundTransfers *transfers, int layer)
{
    FieldBackground *background = transfers->background;
    if (!background->screen) {
        switch (layer) {
        case 0: func_020383e0(background->tilemaps[0], 0, 4096); break;
        case 1: func_02038310(background->tilemaps[1], 0, 4096); break;
        case 2: func_02038240(background->tilemaps[2], 0, 4096); break;
        }
    } else {
        switch (layer) {
        case 0: func_02038378(background->tilemaps[0], 0, 4096); break;
        case 1: func_020382a8(background->tilemaps[1], 0, 4096); break;
        case 2: func_020381d8(background->tilemaps[2], 0, 4096); break;
        }
    }
}
