#include "shop_scene_internal.h"
extern "C" {
typedef struct ShopScrollState {
    u8 unknown[196];
    s16 sub, main;
} ShopScrollState;
typedef char ShopScrollStateSizeCheck[sizeof(ShopScrollState) == 200 ? 1 : -1];
extern ShopScrollState data_ov009_0207f23c;
extern int func_ov005_02065f00(void);
extern void func_ov005_0206651c(int, int), func_ov005_0206642c(int, int), func_ov005_02068f84(int),
    func_ov005_02065eec(void);
extern void MIi_CpuCopyFast(const void *, void *, u32);

#define WORK data_ov009_0207ea3c
#define SCROLL data_ov009_0207f23c
void ShopScene_UploadFrame(void)
{
    if (!func_ov005_02065f00())
        return;
    func_ov005_0206651c(11, 11);
    func_ov005_0206642c(11, 11);
    SCROLL.sub = -*(s32 *)((u8 *)&WORK + 0x8c8) / 4096;
    SCROLL.main = *(s32 *)((u8 *)&WORK + 0x8c8) / 4096;
    if (SCROLL.sub < 0)
        func_ov005_0206786c(1, 0, 0, 0, 255, SCROLL.sub + 192);
    else
        func_ov005_0206786c(1, 0, 0, SCROLL.sub, 255, 192);
    if (SCROLL.main < 0)
        func_ov005_0206786c(0, 0, 0, 0, 255, SCROLL.main + 192);
    else
        func_ov005_0206786c(0, 0, 0, SCROLL.main, 255, 192);
    if (!WORK.special_shop) {
        if (WORK.animated_background) {
            if (WORK.background_dirty) {
                void *destination = Overlay5DisplayBg_GetScreenVram16K(DISPLAY_ENGINE_SUB, 2);
                MIi_CpuCopyFast(WORK.owned_58[7], destination, 49152);
                WORK.background_dirty = 0;
            }
            WORK.wave_current = WORK.wave_pending;
        }
        Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_SUB, 1, -SCROLL.sub);
        Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_SUB, 2, 4096, 4096, 0, 0, 0, 0, -SCROLL.sub);
        Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_SUB, 3, 4096, 4096, 0, 0, 0, 0, -SCROLL.sub);
        Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_MAIN, 0, -SCROLL.main);
        Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_MAIN, 1, -SCROLL.main);
        Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_MAIN, 3, 4096, 4096, 0, 0, 0, 0, -SCROLL.main);
    } else {
        Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_SUB, 0, -SCROLL.sub);
        Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_SUB, 1, -SCROLL.sub);
        Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_SUB, 2, 4096, 4096, 0, 0, 0, 0, -SCROLL.sub);
        Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_SUB, 3, 4096, 4096, 0, 0, 0, 0, -SCROLL.sub);
        Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_MAIN, 0, -SCROLL.main);
        Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_MAIN, 1, -SCROLL.main);
        Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_MAIN, 3, 4096, 4096, 0, 0, 0, 0, -SCROLL.main);
    }
    func_ov005_02068f84(0);
    func_ov005_02068f84(1);
    GameSpritePalette_UploadScreen(0);
    GameSpritePalette_UploadScreen(1);
    GameDisplay_CopyOam(3);
    GameDisplay_ClearOam();
    func_ov005_02065eec();
}
}
