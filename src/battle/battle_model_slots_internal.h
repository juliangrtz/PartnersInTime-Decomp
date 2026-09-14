#ifndef PIT_BATTLE_MODEL_SLOTS_INTERNAL_H
#define PIT_BATTLE_MODEL_SLOTS_INTERNAL_H
#include <game/battle_model_animation.h>
#include <game/battle_effect.h>
#include <game/battle_context.h>
typedef struct ModelSlotsView {
    u8 prefix[52472];
    GameMatrixAnimationTrack *animations[64];
    BattleAITask *effects[64];
} ModelSlotsView;
#define SLOTS ((ModelSlotsView *)gBattleContext)
#endif
