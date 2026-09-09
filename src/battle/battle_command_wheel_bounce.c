#include <game/battle_selection_animation.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>

int BattleCommandWheel_TriggerEntryBounce(int dismiss_after_bounce) {
    BattleCommandWheelEntry *entry =
        &((BattleCommandWheelEntry *)(
              gBattleContext + BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET))[
            *(s16 *)(gBattleContext + BATTLE_SELECTED_COMMAND_OFFSET)];

    if (entry->bounce_timer == 0) {
        (*(s16 *)(gBattleContext +
                  BATTLE_INTERFACE_ANIMATION_COUNT_OFFSET))++;
    }
    entry->bounce_timer = BATTLE_COMMAND_WHEEL_BOUNCE_FRAMES;
    entry->flags.bits.dismiss_after_bounce = dismiss_after_bounce != 0;
    return BattleSound_Play(9, 0, 0, 0);
}
