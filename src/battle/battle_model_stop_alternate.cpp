#include <game/battle_scene.h>
extern "C" void func_020093b4(BattleModel *, int);

void BattleModel_StopAlternate(BattleModel *model) {
    model->stop();
    if (model->palette)
        GameSpritePalette_Unlink(model->palette);
    model->unknown_1c();
    func_020093b4(model, 0);
}
