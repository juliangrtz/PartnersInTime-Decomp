#include "title_sequence_internal.h"
#include <game/heap.h>
void func_ov006_0207380c(void *);

void TitleRumblePrompt_Open(TitleSequenceRumblePrompt *work)
{
    work->return_cursor_x = work->menu.cursor->header.x;
    work->return_cursor_y = work->menu.cursor->header.y;
    TitleMenuBase_MoveSelection(&work->menu, 0, 8, TITLE_RUMBLE_ENTERING);
    work->menu.header.x = 384 << 12;
    work->menu.header.y = 316 << 12;
    work->menu.header.elapsed = 0;
    work->menu.header.pass_flags = 0;
    work->menu.header.state = TITLE_RUMBLE_ENTERING;
}
void TitleRumblePrompt_Release(TitleSequenceRumblePrompt *work)
{
    func_ov006_0207380c(work);
    GameSpriteAllocation_Unlink(&work->text_tiles);
    GameSpritePalette_Unlink(&work->text_palette);
    if (work->palette_data) {
        GameHeap_Free(work->palette_data);
        work->palette_data = 0;
    }
}
