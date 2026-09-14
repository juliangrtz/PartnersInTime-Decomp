#include <game/battle_model_animation.h>
#include <game/battle_effect.h>
#include <game/battle_context.h>
typedef struct ModelSlotsView {
    u8 prefix[52472];
    GameMatrixAnimationTrack *animations[64];
    BattleAITask *effects[64];
} ModelSlotsView;
#define SLOTS ((ModelSlotsView *)gBattleContext)
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
