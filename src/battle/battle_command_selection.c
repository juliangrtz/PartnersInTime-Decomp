#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/save_data.h>

enum BattleCommandSelectionConstant {
    BATTLE_INPUT_HELD_OFFSET = 0x102,
    BATTLE_INPUT_PRESSED_OFFSET = 0x104,
    BATTLE_INPUT_REPEAT_OFFSET = 0x106,
    BATTLE_INPUT_RIGHT = 0x10,
    BATTLE_INPUT_LEFT = 0x20,
    BATTLE_INPUT_UP = 0x40,
    BATTLE_INPUT_DOWN = 0x80,
    BATTLE_ACTIVE_ACTOR_ID_OFFSET = 0x20,
    BATTLE_SELECTED_COMMAND_OFFSET = 0x11A,
    BATTLE_SELECTED_ITEM_INDEX_OFFSET = 0x120,
    BATTLE_BROS_ITEM_STATE_OFFSET = 0x130,
    BATTLE_ITEM_TARGET_STATE_OFFSET = 0x134,
    BATTLE_BROS_ITEM_MASK_OFFSET = 0x5A8,
    BATTLE_COMMAND_WHEEL_INTENSITY_OFFSET = 0x6534,
    BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET = 0x6538,
    BATTLE_COMMAND_WHEEL_ENTRY_COUNT_OFFSET = 0x6560,
    BATTLE_COMMAND_WHEEL_ANGULAR_SPEED_OFFSET = 0x6562,
    BATTLE_COMMAND_WHEEL_INPUT_STATE_OFFSET = 0x6566,
    BATTLE_RUNTIME_FLAG_NO_RETREAT = 0x400,
    SAVE_PARTY_FORM_OFFSET = 0x558,
    BATTLE_COMMAND_WHEEL_MAX_ACCELERATION = 5
};

enum BattleCommandId {
    BATTLE_COMMAND_SLOT_1 = 1,
    BATTLE_COMMAND_SLOT_2 = 2,
    BATTLE_COMMAND_BROS_ITEM = 3,
    BATTLE_COMMAND_ITEM = 4,
    BATTLE_COMMAND_RETREAT = 5
};

enum BattleItemTargetMode {
    BATTLE_ITEM_TARGET_RESTORE_0 = 0,
    BATTLE_ITEM_TARGET_RESTORE_1 = 1,
    BATTLE_ITEM_TARGET_REVIVE = 2,
    BATTLE_ITEM_TARGET_STATUS = 3
};

typedef struct BattleCommandSelectionEntry {
    s16 angle;
    u8 unknown_02[6];
} BattleCommandSelectionEntry;

typedef struct BattleItemTargetState {
    u8 unknown_00[0x0C];
    u32 target_mode_flags;
} BattleItemTargetState;

int BattleCommandWheel_UpdateSelection(void) {
    int selected_index = *(s16 *)(
        gBattleContext + BATTLE_SELECTED_COMMAND_OFFSET);
    int entry_count = *(s16 *)(
        gBattleContext + BATTLE_COMMAND_WHEEL_ENTRY_COUNT_OFFSET);

    if (*(s16 *)(gBattleContext +
                 BATTLE_COMMAND_WHEEL_INTENSITY_OFFSET) < 1) {
        return 0;
    }
    if (entry_count <= 1) {
        return 0;
    }

    if ((*(u16 *)(gBattleContext + BATTLE_INPUT_HELD_OFFSET) &
         (BATTLE_INPUT_LEFT | BATTLE_INPUT_RIGHT)) != 0) {
        u16 input_state = *(u16 *)(
            gBattleContext + BATTLE_COMMAND_WHEEL_INPUT_STATE_OFFSET);
        unsigned int acceleration =
            (u32)(input_state << 23) >> 24;

        if (acceleration < BATTLE_COMMAND_WHEEL_MAX_ACCELERATION) {
            *(u16 *)(gBattleContext +
                     BATTLE_COMMAND_WHEEL_INPUT_STATE_OFFSET) =
                (input_state & 0xFE01) |
                (2 * (u8)(acceleration + 1));
        } else {
            *(u16 *)(gBattleContext +
                     BATTLE_COMMAND_WHEEL_INPUT_STATE_OFFSET) =
                (input_state & 0xFE01) | 10;
        }
    } else {
        u16 input_state = *(u16 *)(
            gBattleContext + BATTLE_COMMAND_WHEEL_INPUT_STATE_OFFSET);
        unsigned int acceleration =
            (u32)(input_state << 23) >> 24;

        if (acceleration != 0) {
            *(u16 *)(gBattleContext +
                     BATTLE_COMMAND_WHEEL_INPUT_STATE_OFFSET) =
                (input_state & 0xFE01) |
                (2 * (u8)(acceleration - 1));
        }
    }

    *(s16 *)(gBattleContext +
             BATTLE_COMMAND_WHEEL_ANGULAR_SPEED_OFFSET) =
        200 /
        (entry_count *
         (((u32)(*(u16 *)(
                       gBattleContext +
                       BATTLE_COMMAND_WHEEL_INPUT_STATE_OFFSET) << 23) >>
           24) + 3));

    {
        BattlePartyActor *actor =
            (BattlePartyActor *)BattleActor_GetPartySlot(
                *(u16 *)(gBattleContext +
                         BATTLE_ACTIVE_ACTOR_ID_OFFSET));

        if (*(u16 *)(gBattleContext +
                     BATTLE_COMMAND_WHEEL_INPUT_STATE_OFFSET) & 1) {
            return 0;
        }
        if (actor->state_flags.bits.target_selection_locked) {
            return 0;
        }
    }

    if (*(u16 *)(gBattleContext + BATTLE_INPUT_HELD_OFFSET) &
        BATTLE_INPUT_LEFT) {
        selected_index--;
    }
    if (*(u16 *)(gBattleContext + BATTLE_INPUT_HELD_OFFSET) &
        BATTLE_INPUT_RIGHT) {
        selected_index++;
    }

    while (selected_index < 0) {
        int i = 0;

        selected_index += entry_count;
        do {
            BattleCommandSelectionEntry *entries =
                (BattleCommandSelectionEntry *)(
                    gBattleContext +
                    BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET);

            entries[i].angle -= 256;
            i++;
        } while (i < entry_count);
    }
    while (selected_index >= entry_count) {
        int i = 0;

        selected_index -= entry_count;
        do {
            BattleCommandSelectionEntry *entries =
                (BattleCommandSelectionEntry *)(
                    gBattleContext +
                    BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET);

            entries[i].angle += 256;
            i++;
        } while (i < entry_count);
    }

    if (*(s16 *)(gBattleContext +
                 BATTLE_SELECTED_COMMAND_OFFSET) == selected_index) {
        return 0;
    }
    BattleSound_Play(8, 0, 0, 0);
    *(s16 *)(gBattleContext + BATTLE_SELECTED_COMMAND_OFFSET) =
        selected_index;
    return 1;
}

int BattleCommand_IsAvailable(int command_id) {
    BattleActor_GetPartySlot(
        *(u16 *)(gBattleContext + BATTLE_ACTIVE_ACTOR_ID_OFFSET));

    switch (command_id) {
    case BATTLE_COMMAND_SLOT_1:
        return 1;
    case BATTLE_COMMAND_SLOT_2:
        return 1;
    case BATTLE_COMMAND_BROS_ITEM:
        if (*(void **)(gBattleContext +
                       BATTLE_BROS_ITEM_STATE_OFFSET) == 0) {
            return 0;
        }
        return (*(u32 *)(gBattleContext +
                         BATTLE_BROS_ITEM_MASK_OFFSET) &
                (1 << *(s16 *)(gBattleContext +
                               BATTLE_SELECTED_ITEM_INDEX_OFFSET))) != 0;
    case BATTLE_COMMAND_ITEM: {
        BattleItemTargetState *target_state =
            *(BattleItemTargetState **)(
                gBattleContext + BATTLE_ITEM_TARGET_STATE_OFFSET);
        BattleActor *party[4];
        int target_mode;
        int i;

        if (target_state == 0) {
            return 0;
        }
        target_mode =
            (u32)(2 * target_state->target_mode_flags) >> 25;
        party[0] = BattleActor_GetPartySlot(BATTLE_ACTOR_MARIO);
        party[1] = BattleActor_GetPartySlot(BATTLE_ACTOR_LUIGI);
        party[2] = BattleActor_GetPartySlot(BATTLE_ACTOR_BABY_MARIO);
        party[3] = BattleActor_GetPartySlot(BATTLE_ACTOR_BABY_LUIGI);

        party[0]->flags &= ~BATTLE_ACTOR_FLAG_13;
        party[1]->flags &= ~BATTLE_ACTOR_FLAG_13;
        if (*(s16 *)(gSaveData + SAVE_PARTY_FORM_OFFSET) == 2 &&
            target_mode < 4) {
            party[2]->flags &= ~BATTLE_ACTOR_FLAG_13;
            party[3]->flags &= ~BATTLE_ACTOR_FLAG_13;
        } else {
            party[2]->flags |= BATTLE_ACTOR_FLAG_13;
            party[3]->flags |= BATTLE_ACTOR_FLAG_13;
        }

        if (target_mode == BATTLE_ITEM_TARGET_REVIVE) {
            if (party[0]->current_hp <= 0) {
                party[2]->flags |= BATTLE_ACTOR_FLAG_13;
            } else {
                party[0]->flags |= BATTLE_ACTOR_FLAG_13;
            }
            if (party[1]->current_hp <= 0) {
                party[3]->flags |= BATTLE_ACTOR_FLAG_13;
            } else {
                party[1]->flags |= BATTLE_ACTOR_FLAG_13;
            }
            if (party[2]->current_hp > 0) {
                party[2]->flags |= BATTLE_ACTOR_FLAG_13;
            }
            if (party[3]->current_hp > 0) {
                party[3]->flags |= BATTLE_ACTOR_FLAG_13;
            }
            if (*(s16 *)(gSaveData + SAVE_PARTY_FORM_OFFSET) == 2) {
                if (party[0]->transition_state > 0) {
                    party[2]->flags |= BATTLE_ACTOR_FLAG_13;
                }
                if (party[1]->transition_state > 0) {
                    party[3]->flags |= BATTLE_ACTOR_FLAG_13;
                }
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (party[i]->current_hp <= 0) {
                    party[i]->flags |= BATTLE_ACTOR_FLAG_13;
                }
            }
        }

        for (i = 0; i < 4; i++) {
            BattleActor *actor = party[i];

            if (target_mode <= BATTLE_ITEM_TARGET_RESTORE_1) {
                if (actor->current_hp == actor->max_hp) {
                    actor->flags |= BATTLE_ACTOR_FLAG_13;
                }
            } else if (target_mode == BATTLE_ITEM_TARGET_STATUS &&
                       actor->transition_state < 2 &&
                       actor->force_low_hp_animation == 0 &&
                       actor->status_channel_50 >= 0 &&
                       actor->status_channel_5c >= 0 &&
                       actor->status_channel_68 >= 0) {
                actor->flags |= BATTLE_ACTOR_FLAG_13;
            }
        }

        return (party[0]->flags & BATTLE_ACTOR_FLAG_13) == 0 ||
               (party[1]->flags & BATTLE_ACTOR_FLAG_13) == 0 ||
               (party[2]->flags & BATTLE_ACTOR_FLAG_13) == 0 ||
               (party[3]->flags & BATTLE_ACTOR_FLAG_13) == 0;
    }
    case BATTLE_COMMAND_RETREAT:
        if (*(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) &
            BATTLE_RUNTIME_FLAG_NO_RETREAT) {
            return 0;
        }
        return BattleActor_GetPartySlot(BATTLE_ACTOR_MARIO)->
                       transition_state <= 0 &&
               BattleActor_GetPartySlot(BATTLE_ACTOR_LUIGI)->
                       transition_state <= 0;
    default:
        return 0;
    }
}

int BattleMenu_UpdateVerticalSelection(int selected_index,
                                       int entry_count) {
    int next_index = selected_index;
    u16 pressed = *(u16 *)(gBattleContext +
                           BATTLE_INPUT_PRESSED_OFFSET);

    if ((pressed & BATTLE_INPUT_UP) != 0 && next_index <= 0) {
        next_index = entry_count - 1;
    } else if ((pressed & BATTLE_INPUT_DOWN) != 0 &&
               entry_count - 1 <= next_index) {
        next_index = 0;
    } else {
        u16 repeated = *(u16 *)(gBattleContext +
                                BATTLE_INPUT_REPEAT_OFFSET);

        if (repeated & BATTLE_INPUT_UP) {
            next_index--;
        }
        if (repeated & BATTLE_INPUT_DOWN) {
            next_index++;
        }
        if (next_index < 0) {
            next_index = 0;
        }
        if (entry_count - 1 < next_index) {
            next_index = entry_count - 1;
        }
    }

    if (selected_index != next_index) {
        BattleSound_Play(1, 0, 0, 0);
    }
    return next_index;
}
