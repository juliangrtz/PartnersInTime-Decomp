#include "pause_scene_internal.h"
#include <game/pause_background.h>
extern "C" void func_ov005_0206650c(PauseBackgroundTask *);
static inline PauseBackgroundTask *CreateBackgroundTask(void (*callback)(PauseBackgroundTask *), int group)
{
    return (PauseBackgroundTask *)func_ov005_0206659c((void (*)(PauseMenuElement *))callback, group, 1);
}
extern "C" void DC_FlushRange(const void *, u32);

extern "C" void PauseBackground_LoadPalette(PauseBackgroundTask *task)
{
    DC_FlushRange(task->buffer, 512);
    Overlay5Display_BeginLoadBgExtPalette(DISPLAY_ENGINE_MAIN);
    Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_MAIN, task->buffer, (task->selection << 9) + 16384, 512);
    Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_MAIN, task->buffer, (task->selection << 9) + 24576, 512);
    Overlay5Display_EndLoadBgExtPalette(DISPLAY_ENGINE_MAIN);
    if (task->buffer) {
        GameHeap_DeleteArray(task->buffer);
        task->buffer = 0;
    }
    func_ov005_0206650c(task);
}
extern "C" PauseBackgroundTask *PauseBackground_QueuePalette(void *palette, int bank)
{
    PauseBackgroundTask *task = CreateBackgroundTask(PauseBackground_LoadPalette, 11);
    task->buffer = palette;
    task->selection = bank;
    return task;
}
