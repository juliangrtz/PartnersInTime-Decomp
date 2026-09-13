#include "save_write_effects_internal.h"
#include <game/audio.h>
#include <game/save_menu_write.h>
#include <game/save_state_transfer.h>

extern SaveMenuBufferHeader data_0205e32c;
void func_ov008_02071400(int);
void func_ov008_0206b598(MenuElement *);
void func_ov005_02066358(MenuElement *, void (*)(MenuElement *), int);

#define ENTRY_WORK (*(SaveMenuEntryWorkPrefix *)data_ov008_02078290)
#define BUTTONS (*(GameInput *)data_0206032c)
#define LIVE_SAVE ((SaveLiveTransferView *)gSaveData)

void SaveMenu_UpdateEntry(MenuElement *task)
{
    switch (task->state) {
    case 0:
        ENTRY_WORK.unknown_190 = 0;
        ENTRY_WORK.previous_selection = 0;
        ENTRY_WORK.selection = 0;
        task->counter = 0;
        ++task->state;
        break;
    case 1:
        if (!task->counter) {
            if (data_0205e32c.settings.occupied_slots & (1 << LIVE_SAVE->slot_select.bits.slot))
                SaveMenuText_PreparePanel(0, ENTRY_WORK.summaries[0].unknown_9d);
            else
                SaveMenuText_PreparePanel(0, 0);
            ++task->counter;
        } else {
            SaveMenuText_PreparePanel(1, ENTRY_WORK.summaries[1].unknown_9d);
            ++task->state;
        }
        break;
    case 2:
        SaveMenu_CreateModels();
        SaveMenuText_BuildDialog(data_ov008_0207828c, 0, 0);
        func_ov008_02071400(ENTRY_WORK.selected_panel);
        task->counter = 0;
        ++task->state;
        break;
    case 3:
        ++task->counter;
        func_ov005_0206781c(2, task->counter - 16);
        if (task->counter >= 16)
            func_ov005_02066358(task, SaveMenu_UpdateSelection, 0);
        break;
    }
}

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

void SaveMenu_UpdateSelection(MenuElement *task)
{
    switch (task->state) {
    case 0:
        ENTRY_WORK.unknown_190 = 0;
        ENTRY_WORK.selection = ENTRY_WORK.previous_selection;
        SaveMenuText_BuildDialog(data_ov008_0207828c, 0, 0);
        task->state = 100;
        break;
    case 100:
        if (!ENTRY_WORK.input_locked) {
            u16 pressed = BUTTONS.pressed;
            int action = 0;
            if (pressed & 0x401) {
                if (ENTRY_WORK.selection == 2) action = -1;
                else action = 1;
            }
            if (pressed & 0x802) action = -1;
            if (action > 0) {
                func_ov005_02069bcc(232, 0, 0, 128);
                func_ov005_02066358(task, func_ov008_0206b598, 0);
            } else if (action < 0) {
                func_ov005_02069bcc(3, 0, 0, 128);
                task->counter = 0;
                task->arguments[0] = 0;
                func_ov005_02066358(task, (void (*)(MenuElement *))SaveMenu_UpdateExit, 0);
            } else {
                u16 repeated = BUTTONS.repeated;
                int initial_selection = ENTRY_WORK.selection;
                u8 previous = initial_selection;
                if (repeated & 0x40) ENTRY_WORK.selection = initial_selection - 1;
                if (repeated & 0x80) ++ENTRY_WORK.selection;
                if (ENTRY_WORK.selection < 0) {
                    if (pressed & 0x40) ENTRY_WORK.selection = 2;
                    else ENTRY_WORK.selection = 0;
                }
                if (ENTRY_WORK.selection > 2) {
                    if (pressed & 0x80) ENTRY_WORK.selection = 0;
                    else ENTRY_WORK.selection = 2;
                }
                if (previous != ENTRY_WORK.selection) func_ov005_02069bcc(231, 0, 4, 128);
            }
        }
        break;
    }
}
