#include "pocket_chomp_internal.h"

extern "C" {
void PocketChompSupport_BeginAttack(PocketChompSupport *support)
{
    if (support->object) {
        func_ov018_020c2e50(support, 769);
        BattleModel *model = BattleSceneObject_GetActiveModel(support->object);
        func_0200940c(model, 256);
        func_ov018_020c2c14(220, support->object);
        support->timer = 24;
        u32 result = (support->flags & ~0x7c0) | (POCKET_CHOMP_SUPPORT_ATTACK << 6);
        support->flags = result;
    }
}
}
