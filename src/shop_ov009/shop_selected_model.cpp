#include "shop_rows_internal.h"
extern "C" {
extern s8 data_ov009_0207ea3c[];
void func_ov005_0206650c(void *);
void ShopPanel_DrawSelectedModel(ShopRowTask *task) {
    BattleModel *model = Overlay5ResourceA_Get(task);
    if (task->part != data_ov009_0207ea3c[0x8c])
        func_ov005_0206650c(task);
    else
        func_ov005_02069084(model, 5);
}
}
