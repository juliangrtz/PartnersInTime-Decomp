#ifndef PIT_NAWATOBI_INTERNAL_H
#define PIT_NAWATOBI_INTERNAL_H
extern "C" {
#include <nitro.h>
#include <game/debug_console.h>
}
#include "pause_scene_internal.h"
struct FourResources {
    u32 entries[4][2];
};
struct ThreeResources {
    u32 entries[3][2];
};
struct ConsolePalette {
    u16 colors[16];
};
struct NawatobiMenuTask {
    u8 unknown_00[32];
    int phase, timer;
    u8 unknown_28[32];
};
/* The global scene script reads its level argument from shared variable zero. */
struct NawatobiScriptVariablesPrefix {
    u8 unknown_0000[0x37ec];
    u32 variables[32];
};
enum NawatobiStateOffset {
    NAWATOBI_STATE_LEVEL = 0,
    NAWATOBI_STATE_SELECTED_LEVEL = 1,
    NAWATOBI_STATE_MENU_INITIALIZED = 2,
    NAWATOBI_STATE_SCRIPT_RUNNING = 3
};
enum NawatobiLevelPhase {
    NAWATOBI_LEVEL_START,
    NAWATOBI_LEVEL_WAIT_SCRIPT,
    NAWATOBI_LEVEL_RETURN_DELAY
};
extern "C" {
void NawatobiMenu_Draw(void);
void NawatobiLevel_Update(NawatobiMenuTask *task);
extern const char data_ov007_0208e19c[], data_ov007_0208e1b4[];
extern const char data_ov007_0208e1b8[], data_ov007_0208e1c4[];
extern GameTaskVTable data_ov007_0208e0d0, data_ov007_0208e0bc;
extern NawatobiSceneTask *data_ov007_020a6bbc;
extern u8 *data_ov007_020a6bc0, *data_ov007_020a6bc4, *data_ov007_020a6bc8;
extern u8 data_ov007_020a6bcc[1680];
extern FourResources data_ov007_0208e12c;
extern ThreeResources data_ov007_0208e0f4;
extern ConsolePalette data_ov007_0208e14c;
extern const void *data_ov007_0208e074[3];
extern void *Overlay5Archive_ReadEntry(Overlay5Archive *, int, u16, u32 *, int);
extern void func_ov005_02067468(Overlay5Archive *, u8, const void *, int);
extern Overlay5Asset *func_ov005_02066a7c(Overlay5Archive *, u16, u16, const void *, u8, int);
extern void func_ov005_020698c8(u32, Overlay5Asset *);
extern void func_02007ebc(GameSpritePalette *, int, int, int, int, int, int, int, int, int);
extern void MIi_CpuClear16(u16, void *, u32), MIi_CpuClearFast(u32, void *, u32),
    func_0203b7a0(u32, void *, u32), func_0203b7b4(const void *, void *, u32);
extern void func_ov007_0208c5bc(PauseMenuElement *), func_ov007_0208b420(void);
extern GameIrqTask *NawatobiSceneIrq_Init(GameIrqTask *, u32, u32, void *);
extern int func_ov005_02065f00(void), func_ov005_02065edc(void);
extern void func_ov005_0206651c(int, int), func_ov005_0206642c(int, int), func_ov005_02068f84(int),
    func_ov005_02065eec(void);
extern void func_ov005_0206903c(int), func_ov005_02068fe0(int), func_ov005_02068ee0(int, void *, u8 *, u8 *);
extern void func_02009598(void *, int, int);
static inline void ClearFast(void *p, u32 size)
{
    volatile u32 zero = 0;
    MIi_CpuClearFast(zero, p, size);
}
static inline void Clear16(void *p, u32 size)
{
    volatile u16 zero = 0;
    MIi_CpuClear16(zero, p, size);
}
static inline void DeleteBuffer(void *p)
{
    if (p)
        GameHeap_DeleteArray(p);
}
#define DISPLAY data_ov005_0206a180
#define CONSOLE(screen) (*(GameConsole **)((u8 *)&DISPLAY + 32 + 4 * (screen)))
}
typedef char FourResourcesSizeCheck[sizeof(FourResources) == 32 ? 1 : -1];
typedef char ThreeResourcesSizeCheck[sizeof(ThreeResources) == 24 ? 1 : -1];
typedef char ConsolePaletteSizeCheck[sizeof(ConsolePalette) == 32 ? 1 : -1];
typedef char NawatobiMenuTaskSizeCheck[sizeof(NawatobiMenuTask) == 72 ? 1 : -1];
typedef char NawatobiScriptVariablesPrefixSizeCheck[
    sizeof(NawatobiScriptVariablesPrefix) == 0x386c ? 1 : -1];
#endif
