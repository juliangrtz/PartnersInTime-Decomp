#include "save_menu_internal.h"
#include <game/audio.h>
#include <game/save_menu_write.h>
#include <game/save_state_transfer.h>
extern SaveMenuBufferHeader data_0205e32c;
extern void func_ov005_02066358(MenuElement *, void (*)(MenuElement *), int);
extern void func_ov005_02069bcc(int, int, int, int);
extern void func_ov008_02071400(int);
#define WORK (*(SaveMenuEntryWorkPrefix *)data_ov008_02078290)
#define BUTTONS (*(GameInput *)data_0206032c)
#define LIVE_SAVE ((SaveLiveTransferView *)gSaveData)

void GameOverMenu_UpdateEntry(MenuElement *element)
{
    GameOverMenuTask *task = (GameOverMenuTask *)element;
    switch (task->state) {
    case 0:
        WORK.menu_mode = 0;
        WORK.previous_selection = 0;
        WORK.selection = 0;
        ++task->state;
        break;
    case 1:
        if (data_0205e32c.settings.occupied_slots & (1 << LIVE_SAVE->slot_select.bits.slot))
            SaveMenuText_PreparePanel(0, WORK.summaries[0].unknown_9d);
        else
            SaveMenuText_PreparePanel(0, 0);
        ++task->state;
        break;
    case 2:
        if (!WORK.summaries[0].available && !LIVE_SAVE->unknown_516) {
            SaveMenuText_BuildDialog(data_ov008_0207828c, 4, 33);
            GameOverMenu_CreateModels(0);
        } else {
            SaveMenuText_BuildDialog(data_ov008_0207828c, 2, 0);
            GameOverMenu_CreateModels(1);
        }
        func_ov008_02071400(WORK.selected_panel);
        GameAudio_PlayMusic(32, 0, 3072);
        task->counter = 0;
        ++task->state;
        break;
    case 3:
        func_ov005_0206781c(2, ++task->counter - 16);
        if (task->counter >= 16) {
            if (WORK.summaries[0].available || LIVE_SAVE->unknown_516)
                func_ov005_02066358(element, GameOverMenu_UpdateSelection, 0);
            else
                ++task->state;
        }
        break;
    case 4:
        if (BUTTONS.pressed & 0x401) {
            task->counter = 0;
            task->quit_to_title = 1;
            func_ov005_02066358(element, GameOverMenu_UpdateExit, 0);
        }
        break;
    }
}

void GameOverMenu_UpdateExit(MenuElement *element)
{
    GameOverMenuTask *task = (GameOverMenuTask *)element;
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
        if (!task->quit_to_title)
            func_ov005_0206781c(2, -task->counter);
        else
            func_ov005_0206781c(2, task->counter);
        if (task->counter >= 16) {
            GameAudio_StopEffects();
            if (!task->quit_to_title)
                data_ov008_02078288->phase = 1;
            else
                data_ov008_02078288->phase = 2;
            ++task->state;
        }
        break;
    }
}

void GameOverMenu_UpdateSelection(MenuElement *element)
{
    GameOverMenuTask *task = (GameOverMenuTask *)element;
    switch (task->state) {
    case 0:
        task->choice_count = 0;
        if (WORK.summaries[0].available) ++task->choice_count;
        if (LIVE_SAVE->unknown_516) ++task->choice_count;
        task->state = 100;
        break;
    case 100:
        if (!WORK.input_locked) {
            u16 pressed = BUTTONS.pressed;
            if (pressed & 0x401) {
                func_ov005_02069bcc(232, 0, 0, 128);
                if (!WORK.selection && WORK.summaries[0].available) {
                    LIVE_SAVE->slot_select.raw &= ~8u;
                    LIVE_SAVE->unknown_561[0] = 0;
                    SaveState_UnpackSlot();
                    task->counter = 0;
                    task->quit_to_title = 0;
                    func_ov005_02066358(element, GameOverMenu_UpdateExit, 0);
                } else {
                    LIVE_SAVE->slot_select.raw &= ~8u;
                    LIVE_SAVE->unknown_561[0] = 1;
                    task->counter = 0;
                    task->quit_to_title = 0;
                    func_ov005_02066358(element, GameOverMenu_UpdateExit, 0);
                }
            } else {
                int initial_selection = WORK.selection;
                u8 previous = initial_selection;
                if (pressed & 0x40) WORK.selection = initial_selection - 1;
                if (pressed & 0x80) ++WORK.selection;
                if (WORK.selection < 0) WORK.selection = task->choice_count - 1;
                if (WORK.selection >= task->choice_count) WORK.selection = 0;
                if (previous != WORK.selection)
                    func_ov005_02069bcc(231, 0, 4, 128);
            }
        }
        break;
    }
}
