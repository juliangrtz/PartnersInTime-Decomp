#include "battle_model_slots_internal.h"
int BattleAlternateModelEffect_SpawnInFreeSlot(int resource, int animation, int x, s16 y, s16 z,
                                               int scale) {
    int slot;
    for (slot = 0;; ++slot) {
        if (slot == 64)
            return -1;
        if (!SLOTS->effects[slot])
            break;
    }
    BattleAlternateModelEffect_SpawnAttached(&SLOTS->effects[slot], resource, animation, x, y, z,
                                             scale);
    return slot;
}
