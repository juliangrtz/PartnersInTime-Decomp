#include "title_menu_internal.h"

void TitleMenuBase_MoveSelection(TitleMenuBase *work, int direction, int frames, int next_state)
{
    if (work->minimum != work->maximum) {
        work->selection += direction;
        if (work->selection > work->maximum)
            work->selection = work->minimum;
        if (work->selection < work->minimum)
            work->selection = work->maximum;
        TitleMenuCursor_Move(work->cursor, work->items[work->selection].x << 12,
                            work->items[work->selection].y << 12, frames);
        work->header.state = next_state;
    }
}
void TitleMenuBase_Init(TitleMenuBase *work, u32 resource, void *cursor)
{
    TitleModel_Load(work, resource, 2, 0, 1);
    work->cursor = cursor;
}
