#include "pause_scene_internal.h"

struct PauseExitTask {
    u8 unknown_00[32];
    s32 phase, brightness;
    u8 unknown_28[32];
};
typedef char PauseExitTaskSize[sizeof(PauseExitTask) == 72 ? 1 : -1];

extern "C" void func_ov005_0206650c(void *);

extern "C" void PauseScene_FadeOutTask(PauseExitTask *task)
{
    switch (task->phase) {
    case 0:
        task->brightness = 0;
        ++task->phase;
        break;
    case 1:
        --task->brightness;
        func_ov005_0206781c(2, task->brightness);
        if (task->brightness == -16) {
            data_ov007_0208e1e0->phase = 6;
            func_ov005_0206650c(task);
        }
        break;
    }
}

extern "C" void PauseScene_PrepareExitTask(PauseExitTask *task)
{
    switch (task->phase) {
    case 0:
        /* Preserve the native signed -1 access and the existing raw byte view. */
        *(s8 *)&WORK.unknown110 = -1;
        func_ov005_020671e4(ARCHIVE);
        ++task->phase;
        break;
    case 1:
        PauseScene_RequestExit(data_ov007_0208e1e0, 1);
        break;
    }
}
