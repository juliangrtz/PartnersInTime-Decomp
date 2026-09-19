#include "title_animation_internal.h"

extern "C" {
u32 GX_ResetBankForTex(void);
u32 GX_ResetBankForBG(void);
u32 GX_ResetBankForSubBG(void);
u32 GX_ResetBankForSubOBJ(void);
u32 GX_ResetBankForTexPltt(void);
u32 GX_ResetBankForBGExtPltt(void);
u32 GX_ResetBankForSubBGExtPltt(void);
u32 GX_ResetBankForSubOBJExtPltt(void);
void func_02036988(vu32 *, int, int, int, int);
}

static inline void DeleteTask(GamePaletteEffectController *task)
{
    if (task)
        ((GameTaskDispatch *)task)->delete_task();
}

extern "C" void TitleAnimation_ReleaseRenderState(TitleAnimationController *work)
{
    if (work->palette_effects[0]) {
        DeleteTask(work->palette_effects[0]);
        work->palette_effects[0] = 0;
    }
    if (work->palette_effects[1]) {
        DeleteTask(work->palette_effects[1]);
        work->palette_effects[1] = 0;
    }
    GX_ResetBankForTex();
    GX_ResetBankForBG();
    GX_ResetBankForSubBG();
    GX_ResetBankForSubOBJ();
    GX_ResetBankForTexPltt();
    GX_ResetBankForBGExtPltt();
    GX_ResetBankForSubBGExtPltt();
    GX_ResetBankForSubOBJExtPltt();
    func_02036988((vu32 *)0x04000050, 0, 63, 8, 8);
    func_02036988((vu32 *)0x04001050, 0, 63, 8, 8);
    *(vu32 *)0x04000000 &= ~0xe000;
    *(vu32 *)0x04001000 &= ~0xe000;
}
