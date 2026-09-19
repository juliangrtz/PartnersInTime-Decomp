/*
 * Queued row drawing (overlay 7, 0x020757C4-0x020758F0).
 *
 * Drawing a list row is deferred to a task, so a scroll queues one draw per row
 * rather than redrawing the whole list in one frame. Copying the marker is part
 * of the same step.
 */

#include "pause_scene_internal.h"
#include <game/pause_list_row.h>
#include <game/overlay007_party.h>

extern "C" {
void MIi_CpuCopy16(const void *, void *, u32);
void func_ov005_0206650c(PauseMenuElement *);
u8 func_ov007_02075400(Overlay7Party *);

void PauseItem_CopyMarker(Overlay7Party *party, u32 offset, int icon)
{
    if (icon < 0)
        return;
    u8 *destination = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN) + offset;
    MIi_CpuCopy16((u8 *)WORK.ownedc0 + 64 * icon, destination, 64);
}

void PauseList_DrawRowTask(PauseMenuElement *element)
{
    PauseItemRowDrawTask *task = (PauseItemRowDrawTask *)element;
    u32 offset = WORK.main_allocation.offset + 1088 * task->row;
    int item = task->item;
    PauseItem_DrawText((Overlay7Party *)data_ov007_0208e1e4, offset, 0, item, 4, 1, 1);
    int kind = func_ov007_02075400((Overlay7Party *)data_ov007_0208e1e4);
    int icon = PauseItem_GetMarkerId((Overlay7Party *)data_ov007_0208e1e4, kind, (u16)item, 0);
    PauseItem_CopyMarker((Overlay7Party *)data_ov007_0208e1e4, offset + 1024, icon);
    func_ov005_0206650c(element);
}

PauseMenuElement *PauseList_QueueRowDraw(int row, int item)
{
    PauseMenuElement *element = func_ov005_0206659c(PauseList_DrawRowTask, 11, 1);
    PauseItemRowDrawTask *task = (PauseItemRowDrawTask *)element;
    task->row = row;
    task->item = item;
    return element;
}
}
