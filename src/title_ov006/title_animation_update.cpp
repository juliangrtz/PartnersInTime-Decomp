#include "title_animation_internal.h"
extern "C" {
#include <game/input.h>
#include <game/sprite_output.h>
#include <game/session.h>
extern TitleAnimationController *data_ov006_0207c4e4;
extern GameSessionTask *data_02059ffc;
extern GameOamEntry data_02060740[128];
/* Overlay IDs are absolute linker symbols. */
extern int OVERLAY_5_ID, OVERLAY_6_ID;
void func_02037108(void);
void func_020090f8(int);
void func_02009598(GameOamEntry *, int, u8);
}

class TitleRenderDispatch {
public:
    virtual void unknown_00();
    virtual void unknown_04();
    virtual void pass_08();
    virtual void pass_0c();
};
static inline void DeleteTask(void *task)
{
    if (task)
        ((GameTaskDispatch *)task)->delete_task();
}

extern "C" {
void TitleAnimation_Update(TitleAnimationController *work)
{
    GameInput_Read();
    GameOam_SetReserved(1, 0, 0, 0);
    func_02037108();
    *(vu32 *)0x040004c0 = 0x42107fff;
    *(vu32 *)0x040004c4 = 0;
    *(vu32 *)0x040004cc = 0x7fff;
    *(vu32 *)0x040004c8 = 678250;
    switch (work->exit_state) {
    case 0:
        TitleAnimation_UpdateSequence();
        break;
    case 1:
        if (data_ov006_0207c4e4) {
            DeleteTask(data_ov006_0207c4e4);
            data_ov006_0207c4e4 = 0;
        }
        ArchiveIO_UnloadOverlay((u32)&OVERLAY_6_ID, 0);
        ArchiveIO_UnloadOverlay((u32)&OVERLAY_5_ID, 0);
        /* The native sequence reads exit_kind after the deleting callback. */
        GameSessionTask_RequestStatePhase2(data_02059ffc, !work->exit_kind ? 8 : 2);
        return;
    }
    func_020090f8(0);
    func_020090f8(1);
    TitleAnimation_DrawElements();
    func_02009598(data_02060740, 0, data_0205a060[1]);
    GameOam_SortGroups(1);
    GameOam_BuildSorted(1, data_02060740, data_02060740);
    ((TitleRenderDispatch *)work->palette_effects[0])->pass_0c();
    ((TitleRenderDispatch *)work->palette_effects[1])->pass_08();
    *(vu32 *)0x04000540 = 1;
}
}
