#include <game/battle_selection_animation.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>

int BattleTargetCursor_TriggerBounce(void) {
    if (((BattleTargetCursorAnimation *)(
             gBattleContext + BATTLE_TARGET_CURSOR_OFFSET + 0x0C))->
            bits.uniform_targets != 0) {
        *(s8 *)(gBattleContext + BATTLE_TARGET_CURSOR_OFFSET + 0x0C) =
            BATTLE_TARGET_CURSOR_BOUNCE_FRAMES;
    }
    return BattleSound_Play(3, 0, 0, 0);
}

void BattleTargetCursor_Update(BattleTargetCursorState *state) {
    if (state->visible != 0 ||
        state->animation.bytes.bounce_timer != 0) {
        if (state->animation.bits.uniform_targets != 0) {
            state->rotation_angle += 4;
        } else {
            state->rotation_angle = 0;
        }
    }
}
