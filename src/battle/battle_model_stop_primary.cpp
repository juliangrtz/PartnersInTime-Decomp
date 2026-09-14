#include <game/battle_scene.h>
extern "C" void func_020093b4(BattleModel *, int);

void BattleModel_StopPrimary(BattleModel *model) {
    model->stop();
    model->unknown_c0();
    func_020093b4(model, 0);
}
