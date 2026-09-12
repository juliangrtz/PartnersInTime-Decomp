#include "save_write_effects_internal.h"
#include <game/audio.h>

void SaveMenu_UpdateExit(SaveMenuExitTask *task)
{
    switch (task->phase) {
    case 0:
        if (task->timer) {
            --task->timer;
        } else {
            if (task->quit_to_title) GameAudio_StopMusic(16);
            task->timer = 0;
            ++task->phase;
        }
        break;
    case 1:
        ++task->timer;
        if (!task->quit_to_title)
            func_ov005_0206781c(2, -task->timer);
        else
            func_ov005_0206781c(2, task->timer);
        if (task->timer >= 16) {
            if (task->quit_to_title) {
                GameAudio_StopEffects();
                data_ov008_02078280->phase = 2;
            } else {
                data_ov008_02078280->phase = 1;
            }
            ++task->phase;
        }
        break;
    }
}
