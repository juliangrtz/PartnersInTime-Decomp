#include "save_menu_internal.h"
extern void func_ov005_02066358(MenuElement *, void (*)(MenuElement *), int);
extern void func_ov005_02069bcc(int, int, int, int);
extern void func_ov008_0206e310(MenuElement *);
#define WORK (*(SaveMenuEntryWorkPrefix *)data_ov008_02078290)
#define BUTTONS (*(GameInput *)data_0206032c)

void LoadMenu_UpdateSelection(MenuElement *task)
{
    switch (task->state) {
    case 0:
        WORK.menu_mode = 0;
        SaveMenuText_BuildDialog(data_ov008_0207828c, 4, 9);
        task->state = 100;
        break;
    case 100:
        if (!WORK.input_locked) {
            u16 pressed = BUTTONS.pressed;
            if (pressed & 0x401) {
                func_ov005_02069bcc(232, 0, 0, 128);
                WORK.previous_selection = 0;
                func_ov005_02066358(task, func_ov008_0206e310, 0);
            } else if (pressed & 0x802) {
                func_ov005_02069bcc(3, 0, 0, 128);
                func_ov005_02066358(task, LoadMenu_UpdateExit, 0);
                task->counter = 0;
                WORK.exit_mode = 1;
            } else {
                int initial_selection = WORK.selected_panel;
                u8 previous = initial_selection;
                if (pressed & 0x50) WORK.selected_panel = initial_selection - 1;
                if (pressed & 0xa0) ++WORK.selected_panel;
                if (WORK.selected_panel < 0) WORK.selected_panel = 1;
                if (WORK.selected_panel >= 2) WORK.selected_panel = 0;
                if (previous != WORK.selected_panel)
                    func_ov005_02069bcc(231, 0, 4, 128);
            }
        }
        break;
    }
}
