#include <game/battle_scene.h>
#include <game/save_menu.h>
extern "C" {
void *func_ov005_020698dc(int);
SaveMenuTransferTask *func_ov005_0206659c(void (*)(SaveMenuCursorTask *), int, int);
BattleModel *Overlay5ResourceA_Attach(void *, BattleModel *, int);
void func_ov005_02068908(BattleModel *, int, void *, int, int);
extern u8 data_ov008_02078290[];
extern SaveMenuText *data_ov008_0207828c;
BattleModel *Overlay5ResourceA_Get(void *);
void func_ov005_02069084(void *, u8);
void func_ov005_020663d8(int);
void func_ov005_0206650c(void *);
void Overlay5Display_EnablePlanes(u8, int);
void Overlay5Display_DisablePlanes(u8, int);
void func_ov008_02076b38(SaveMenuTransferTask *);
/* Menu state prefix through selection and cursor visibility. */
struct EraseMenuControlState {
    u8 unknown_00[0x199];
    s8 selection;
    u8 unknown_19a;
    s8 hide_cursor;
};
typedef char EraseMenuControlStateSizeCheck[sizeof(EraseMenuControlState) == 0x19c ? 1 : -1];
#define MENU_CONTROL (*(EraseMenuControlState *)data_ov008_02078290)

void EraseMenu_DrawCursor(SaveMenuCursorTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    SaveMenuTransferTask *parent = task->parent;
    model->animation_offset_x = parent->arguments[MENU_CONTROL.selection] - 4;
    model->animation_offset_y = 124;
    if (MENU_CONTROL.hide_cursor == 0)
        func_ov005_02069084(model, 8);
}

void EraseMenu_CreateCursor(SaveMenuTransferTask *parent)
{
    void *resource = func_ov005_020698dc(0);
    SaveMenuCursorTask *task = (SaveMenuCursorTask *)func_ov005_0206659c(EraseMenu_DrawCursor, 1, 1);
    BattleModel *model;
    task->parent = parent;
    model = Overlay5ResourceA_Attach(task, 0, 1);
    func_ov005_02068908(model, 0, resource, 1, -1);
    model->set_primary_animation(0, 0, 1);
    model->flag_bits.unknown_00_01 = 1;
}

void EraseMenu_DisableConfirmationPlanes(SaveMenuCursorTask *task)
{
    Overlay5Display_DisablePlanes(0, 2);
    Overlay5Display_DisablePlanes(0, 4);
    func_ov005_0206650c(task);
}

void EraseMenu_HideConfirmation(void)
{
    func_ov005_020663d8(1);
    func_ov005_0206659c(EraseMenu_DisableConfirmationPlanes, 11, 1);
}

void EraseMenu_EnableConfirmationPlanes(SaveMenuCursorTask *task)
{
    Overlay5Display_EnablePlanes(0, 2);
    Overlay5Display_EnablePlanes(0, 4);
    Overlay5Display_EnablePlanes(0, 16);
    func_ov005_0206650c(task);
}

void EraseMenu_ShowConfirmation(SaveMenuTransferTask *parent, int entry)
{
    SaveMenuText_DrawBackground(data_ov008_0207828c, 0, 2, 0, (u16)entry, 0, 0);
    func_ov008_02076b38(parent);
    EraseMenu_CreateCursor(parent);
    MENU_CONTROL.hide_cursor = 0;
    MENU_CONTROL.selection = 1;
    func_ov005_0206659c(EraseMenu_EnableConfirmationPlanes, 11, 1);
}
}
