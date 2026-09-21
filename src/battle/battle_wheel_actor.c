/* Icon pairs encode an enabled/disabled entry. Formation 2/3 exchanges the
 * adult and baby represented by the corresponding pair of actor icons. */
#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_selection_animation.h>

int BattleCommandWheel_GetEntryActor(int entry_index, int enabled_only) {
    BattlePartyActor *mario = (BattlePartyActor *)
        BattleActor_GetPartySlot(BATTLE_ACTOR_MARIO);
    BattlePartyActor *luigi = (BattlePartyActor *)
        BattleActor_GetPartySlot(BATTLE_ACTOR_LUIGI);
    if (enabled_only) {
        switch (((BattleCommandWheelEntry *)(
            gBattleContext + 8 * entry_index +
            BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET))->flags.bits.icon_id) {
        case 13:
            return mario->formation_index != 2
                ? BATTLE_ACTOR_MARIO : BATTLE_ACTOR_BABY_MARIO;
        case 15:
            return luigi->formation_index != 3
                ? BATTLE_ACTOR_LUIGI : BATTLE_ACTOR_BABY_LUIGI;
        case 17:
            return mario->formation_index == 2
                ? BATTLE_ACTOR_MARIO : BATTLE_ACTOR_BABY_MARIO;
        case 19:
            return luigi->formation_index == 3
                ? BATTLE_ACTOR_LUIGI : BATTLE_ACTOR_BABY_LUIGI;
        case 11:
            return 0;
        case 21:
            return BATTLE_ACTOR_MARIO;
        default:
            return -1;
        }
    } else {
        switch (((BattleCommandWheelEntry *)(
            gBattleContext + 8 * entry_index +
            BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET))->flags.bits.icon_id) {
        case 13:
        case 14:
            return mario->formation_index != 2
                ? BATTLE_ACTOR_MARIO : BATTLE_ACTOR_BABY_MARIO;
        case 15:
        case 16:
            return luigi->formation_index != 3
                ? BATTLE_ACTOR_LUIGI : BATTLE_ACTOR_BABY_LUIGI;
        case 17:
        case 18:
            return mario->formation_index == 2
                ? BATTLE_ACTOR_MARIO : BATTLE_ACTOR_BABY_MARIO;
        case 19:
        case 20:
            return luigi->formation_index == 3
                ? BATTLE_ACTOR_LUIGI : BATTLE_ACTOR_BABY_LUIGI;
        case 11:
            return 0;
        case 21:
        case 22:
            return BATTLE_ACTOR_MARIO;
        default:
            return -1;
        }
    }
}
