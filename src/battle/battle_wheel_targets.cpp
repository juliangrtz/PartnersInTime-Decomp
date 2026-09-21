/*
 * Populate item-target icons from the available party members and their current
 * formations. Keep the previous target when possible, then try the active actor,
 * its baby partner and finally the first entry. Icon 11 is the cancel entry.
 * Entry writes preserve the dismiss bit and untouched animation fields.
 */
extern "C" {
#include <game/item.h>
#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_selection_animation.h>
}
enum PartyTargetMenuOffset {
    PARTY_TARGET_ACTOR = 0x118,
    PARTY_TARGET_INDEX = 0x11A,
    PARTY_TARGET_ITEM = 0x134,
    PARTY_TARGET_FLAGS = 0x653E,
    PARTY_TARGET_COUNT = 0x6560
};

#define ENTRY(i) (*(BattleCommandWheelEntry *)(gBattleContext + \
    BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET + 8 * (i)))
#define FLAGS(i) (*(BattleCommandWheelEntryFlags *)(gBattleContext + \
    PARTY_TARGET_FLAGS + 8 * (i)))
#define HALF(offset) (*(u16 *)(gBattleContext + (offset)))
#define COUNT (*(s16 *)(gBattleContext + PARTY_TARGET_COUNT))

extern "C" void BattleCommandWheel_BuildPartyTargets(int preferred_actor,
                                                    int target_mode) {
    int count = 0;

    if (target_mode == 1 || target_mode >= 4) {
        for (int i = 0; i < 4; ++i) {
            if (!BattleActor_GetPartySlot((u16)(i + 56))->flag_bits.excluded_from_targeting) {
                ENTRY(0).angle = 0;
                FLAGS(0).bits.icon_id = 21;
                COUNT = ++count;
                break;
            }
        }
    } else {
        for (int i = 0; i < 2; ++i) {
            BattlePartyActor *actor = (BattlePartyActor *)
                BattleActor_GetPartySlot((u16)(i + 56));
            if (!actor->actor.flag_bits.excluded_from_targeting) {
                ENTRY(count).angle = 0;
                switch (actor->formation_index) {
                case 0:
                case 4:
                    FLAGS(count++).bits.icon_id = 13;
                    break;
                case 1:
                case 5:
                    FLAGS(count++).bits.icon_id = 15;
                    break;
                case 2:
                    FLAGS(count++).bits.icon_id = 17;
                    break;
                case 3:
                    FLAGS(count++).bits.icon_id = 19;
                    break;
                }
            }
            if (!BattleActor_GetPartySlot((u16)(i + 58))->flag_bits.excluded_from_targeting) {
                ENTRY(count).angle = 0;
                switch (actor->formation_index) {
                case 4:
                    FLAGS(count++).bits.icon_id = 17;
                    break;
                case 5:
                    FLAGS(count++).bits.icon_id = 19;
                    break;
                }
                if ((*(UsableItemRecord **)(gBattleContext +
                      PARTY_TARGET_ITEM))->effect_kind == 2) {
                    switch (actor->formation_index) {
                    case 2:
                        FLAGS(count++).bits.icon_id = 13;
                        break;
                    case 3:
                        FLAGS(count++).bits.icon_id = 15;
                        break;
                    }
                }
            }
        }
    }
    ENTRY(count).angle = 0;
    FLAGS(count).bits.icon_id = 11;
    COUNT = count + 1;
    for (int i = 0; i < COUNT; ++i) {
        if (preferred_actor == BattleCommandWheel_GetEntryActor(i, 0)) {
            HALF(PARTY_TARGET_INDEX) = i;
            HALF(PARTY_TARGET_ACTOR) = preferred_actor;
            return;
        }
    }
    if (HALF(BATTLE_ACTIVE_ACTOR_ID_OFFSET) >= 56 &&
        HALF(BATTLE_ACTIVE_ACTOR_ID_OFFSET) < 60) {
        if (preferred_actor != HALF(BATTLE_ACTIVE_ACTOR_ID_OFFSET)) {
            preferred_actor = HALF(BATTLE_ACTIVE_ACTOR_ID_OFFSET);
            for (int i = 0; i < COUNT; ++i) {
                if (preferred_actor == BattleCommandWheel_GetEntryActor(i, 0)) {
                    HALF(PARTY_TARGET_INDEX) = i;
                    HALF(PARTY_TARGET_ACTOR) = preferred_actor;
                    return;
                }
            }
        }
        for (int i = 0; i < COUNT; ++i) {
            if (preferred_actor + 2 == BattleCommandWheel_GetEntryActor(i, 0)) {
                HALF(PARTY_TARGET_INDEX) = i;
                HALF(PARTY_TARGET_ACTOR) = preferred_actor + 2;
                return;
            }
        }
    }
    HALF(PARTY_TARGET_INDEX) = 0;
    HALF(PARTY_TARGET_ACTOR) = BattleCommandWheel_GetEntryActor(0, 0);
}
