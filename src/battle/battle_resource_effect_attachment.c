#include <game/battle_ai.h>
#include <game/battle_effect.h>
BattleAITask *BattleModelEffect_SpawnFromResourceAttached(BattleAITask **owner, int resource,
                                                          int animation, int x, s16 y, s16 z,
                                                          int scale) {
    BattleAITask *task = BattleModelEffect_SpawnFromResource(resource, animation, x, y, z, scale);
    BattleTask_BindOwnerSlot(task, owner);
    return task;
}
