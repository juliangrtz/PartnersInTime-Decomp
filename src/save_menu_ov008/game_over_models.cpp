#include <game/battle_scene.h>
#include <game/save_menu.h>
extern "C" {
void *func_ov005_020698dc(int);
SaveMenuTransferTask *func_ov005_0206659c(void (*)(SaveMenuCursorTask *), int, int);
BattleModel *Overlay5ResourceA_Attach(void *, BattleModel *, int);
void func_ov005_02068908(BattleModel *, int, void *, int, int);
void Overlay5ResourceA_ApplySelector(SaveMenuCursorTask *);
void func_ov008_02070688(SaveMenuCursorTask *);

void GameOverMenu_CreateModels(int show_cursor)
{
    void *resource = func_ov005_020698dc(1);
    SaveMenuTransferTask *task = func_ov005_0206659c(Overlay5ResourceA_ApplySelector, 2, 1);
    BattleModel *model;
    task->arguments[0] = 60;
    model = Overlay5ResourceA_Attach(task, 0, 1);
    func_ov005_02068908(model, 0, resource, 0, -1);
    model->set_primary_animation(0, 0, 1);
    model->flag_bits.unknown_00_01 = 2;
    model->animation_offset_x = 48;
    model->animation_offset_y = 24;
    if (show_cursor) {
        resource = func_ov005_020698dc(0);
        task = func_ov005_0206659c(func_ov008_02070688, 2, 1);
        model = Overlay5ResourceA_Attach(task, 0, 1);
        func_ov005_02068908(model, 0, resource, 1, -1);
        model->set_primary_animation(0, 0, 1);
        model->flag_bits.unknown_00_01 = 2;
    }
}
}
