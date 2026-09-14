#include <game/battle_model_animation.h>
#include <game/battle_effect.h>
#include <game/battle_context.h>
typedef struct ModelSlotsView {
    u8 prefix[52472];
    GameMatrixAnimationTrack *animations[64];
    BattleAITask *effects[64];
} ModelSlotsView;
#define SLOTS ((ModelSlotsView *)gBattleContext)
int BattleModelAnimation_StartInFreeSlot(int animation, BattleModel *model, int x, s16 y, s16 z,
                                         int speed) {
    int slot;
    for (slot = 0;; ++slot) {
        if (slot == 64)
            return -1;
        if (!SLOTS->animations[slot])
            break;
    }
    BattleModelAnimation_StartAttached(&SLOTS->animations[slot], animation, model, x, y, z, speed);
    return slot;
}
