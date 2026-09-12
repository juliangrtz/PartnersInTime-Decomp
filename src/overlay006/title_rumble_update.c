#include "title_sequence_internal.h"
#include <game/title_animation.h>
#include <game/audio.h>
#include <game/input.h>
#include <game/rumble.h>
extern GameInput data_0206032c;
extern const s16 FX_SinCosTable_[];
extern u8 data_0205a00c;
void func_ov006_020739f0(void *, int, int, int);

void TitleRumblePrompt_Update(void *element)
{
    TitleSequenceRumblePrompt *work = element;
    switch (work->menu.header.state) {
    case TITLE_RUMBLE_INACTIVE:
        break;
    case TITLE_RUMBLE_ENTERING:
        ++work->menu.header.elapsed;
        if (work->menu.header.elapsed < 8) {
            work->menu.header.x = -256 * FX_SinCosTable_[2 * (((work->menu.header.elapsed << 14) / 8) >> 4)] + (384 << 12);
        } else {
            work->menu.header.x = 128 << 12;
            if (work->menu.selection == 0)
                GameRumble_PlayTimed(0, 20);
            work->menu.header.state = TITLE_RUMBLE_ACTIVE;
        }
        work->menu.header.y = 316 << 12;
        break;
    case TITLE_RUMBLE_ACTIVE: {
        int direction, close;
        if (work->menu.input_disabled)
            break;
        direction = 0;
        close = 0;
        if (data_0206032c.pressed & 0x409) {
            work->selection_override = work->menu.selection;
            GameAudio_PlayEffectDelayed(232, 0, -1);
            close = 1;
        } else if (data_0206032c.pressed & 0x806) {
            GameAudio_PlayEffectDelayed(234, 0, -1);
            close = 1;
        } else if (data_0206032c.pressed & 0x40) {
            direction = -1;
        } else if (data_0206032c.pressed & 0x80) {
            direction = 1;
        }
        if (close) {
            TitleAnimation_ResetIdleTimer();
            func_ov006_020739f0(work->menu.cursor, work->return_cursor_x, work->return_cursor_y, 8);
            work->menu.header.elapsed = 0;
            work->menu.header.state = TITLE_RUMBLE_EXITING;
        }
        if (direction) {
            TitleAnimation_ResetIdleTimer();
            TitleMenuBase_MoveSelection(&work->menu, direction, 4, TITLE_RUMBLE_WAIT_CURSOR);
            if (work->menu.selection == 0)
                GameRumble_PlayTimed(0, 20);
            else if (data_0205a00c)
                GameRumble_Stop();
        }
        break;
    }
    case TITLE_RUMBLE_WAIT_CURSOR:
        if (TitleElement_IsInactive(work->menu.cursor))
            work->menu.header.state = TITLE_RUMBLE_ACTIVE;
        break;
    case TITLE_RUMBLE_EXITING:
        ++work->menu.header.elapsed;
        if (work->menu.header.elapsed < 8) {
            work->menu.header.x = -256 * FX_SinCosTable_[2 * (((work->menu.header.elapsed << 14) / 8) >> 4)] + (128 << 12);
        } else {
            work->menu.header.x = -128 * 4096;
            work->menu.header.pass_flags = 255;
            work->menu.header.state = TITLE_RUMBLE_INACTIVE;
        }
        work->menu.header.y = 316 << 12;
        break;
    }
}
