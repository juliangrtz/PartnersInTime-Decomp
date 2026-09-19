/*
 * Resource model effects (overlay 2, 0x020AC740-0x020AC820).
 *
 * Spawns a model effect from a scene resource, attached or in a free slot.
 */

#include <game/battle_ai.h>
#include <game/battle_effect.h>
BattleAITask *BattleModelEffect_SpawnFromResourceAttached(BattleAITask **owner, int resource,
                                                          int animation, int x, s16 y, s16 z,
                                                          int scale) {
    BattleAITask *task = BattleModelEffect_SpawnFromResource(resource, animation, x, y, z, scale);
    BattleTask_BindOwnerSlot(task, owner);
    return task;
}

#include "battle_model_slots_internal.h"
int BattleModelEffect_SpawnFromResourceInFreeSlot(int resource, int animation, int x, s16 y, s16 z,
                                                  int scale) {
    int slot;
    for (slot = 0;; ++slot) {
        if (slot == 64)
            return -1;
        if (!SLOTS->effects[slot])
            break;
    }
    BattleModelEffect_SpawnFromResourceAttached(&SLOTS->effects[slot], resource, animation, x, y, z,
                                                scale);
    return slot;
}
