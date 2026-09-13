#include "save_menu_internal.h"
#include <game/audio.h>
#define WORK (*(SaveMenuEntryWorkPrefix *)data_ov008_02078290)

void LoadMenu_UpdateExit(MenuElement *task)
{
    switch (task->state) {
    case 0:
        if (task->counter) {
            --task->counter;
        } else {
            GameAudio_StopMusic(16);
            task->counter = 0;
            ++task->state;
        }
        break;
    case 1:
        ++task->counter;
        if (!WORK.exit_mode)
            func_ov005_0206781c(2, -task->counter);
        else
            func_ov005_0206781c(2, task->counter);
        if (task->counter >= 16) {
            GameAudio_StopEffects();
            switch (WORK.exit_mode) {
            case 0: data_ov008_02078284->phase = 1; break;
            case 1: data_ov008_02078284->phase = 2; break;
            }
            ++task->state;
        }
        break;
    }
}
