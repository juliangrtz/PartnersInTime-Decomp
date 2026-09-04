#include <game/battle_context.h>
#include <game/item.h>
#include <game/save_data.h>

enum BattleItemSelectionConstant {
    BATTLE_SELECTED_ACTION_ITEM_ID_OFFSET = 0x114,
    BATTLE_SELECTED_USABLE_ITEM_ID_OFFSET = 0x116,
    BATTLE_SELECTED_ACTION_ITEM_INDEX_OFFSET = 0x120,
    BATTLE_SELECTED_USABLE_ITEM_INDEX_OFFSET = 0x124,
    BATTLE_SELECTED_ACTION_ITEM_OFFSET = 0x130,
    BATTLE_SELECTED_USABLE_ITEM_OFFSET = 0x134,
    BATTLE_USABLE_ITEM_LIST_OFFSET = 0x572,
    BATTLE_ACTION_ITEM_LIST_OFFSET = 0x58E,
    BATTLE_USABLE_ITEM_COUNT_OFFSET = 0x5A4,
    BATTLE_ACTION_ITEM_COUNT_OFFSET = 0x5A6,
    BATTLE_COMMAND_WHEEL_ENTRY_FLAGS_OFFSET = 0x653E,
    BATTLE_USABLE_ITEM_COUNT = 14,
    BATTLE_ACTION_ITEM_COUNT = 11,
    SAVE_USABLE_ITEM_COUNTS_OFFSET = 0x48E,
    SAVE_ACTION_ITEM_COUNTS_OFFSET = 0x49E
};

int BattleItemList_RebuildActionItems(void) {
    int item_id = 0;

    *(u16 *)(gBattleContext + BATTLE_ACTION_ITEM_COUNT_OFFSET) = 0;
    do {
        if (*(s8 *)(gSaveData + SAVE_ACTION_ITEM_COUNTS_OFFSET +
                    item_id) != 0) {
            *(u16 *)(gBattleContext + BATTLE_ACTION_ITEM_LIST_OFFSET +
                     2 * *(u16 *)(gBattleContext +
                                  BATTLE_ACTION_ITEM_COUNT_OFFSET)) =
                item_id | ITEM_ID_ACTION_TAG;
            (*(u16 *)(gBattleContext +
                      BATTLE_ACTION_ITEM_COUNT_OFFSET))++;
        }
        item_id++;
    } while (item_id < BATTLE_ACTION_ITEM_COUNT);
    return item_id;
}

int BattleItemList_RebuildUsableItems(void) {
    int item_id = 0;

    *(u16 *)(gBattleContext + BATTLE_USABLE_ITEM_COUNT_OFFSET) = 0;
    do {
        if (*(s8 *)(gSaveData + SAVE_USABLE_ITEM_COUNTS_OFFSET +
                    item_id) != 0) {
            *(u16 *)(gBattleContext + BATTLE_USABLE_ITEM_LIST_OFFSET +
                     2 * *(u16 *)(gBattleContext +
                                  BATTLE_USABLE_ITEM_COUNT_OFFSET)) =
                item_id | ITEM_ID_USABLE_TAG;
            (*(u16 *)(gBattleContext +
                      BATTLE_USABLE_ITEM_COUNT_OFFSET))++;
        }
        item_id++;
    } while (item_id < BATTLE_USABLE_ITEM_COUNT);
    return item_id;
}

int BattleItemSelection_SelectActionIndex(int selected_index) {
    u16 item_id;
    int result;

    *(u16 *)(gBattleContext +
             BATTLE_SELECTED_ACTION_ITEM_INDEX_OFFSET) = selected_index;
    *(u16 *)(gBattleContext + BATTLE_SELECTED_ACTION_ITEM_ID_OFFSET) =
        *(u16 *)(gBattleContext + BATTLE_ACTION_ITEM_LIST_OFFSET +
                 2 * selected_index);
    item_id = *(u16 *)(
        gBattleContext + BATTLE_SELECTED_ACTION_ITEM_ID_OFFSET);
    if ((item_id & ITEM_ID_ACTION_TAG) != ITEM_ID_ACTION_TAG) {
        result = 0;
    } else {
        result = (int)&gActionItemRecords[
            item_id & ITEM_ID_INDEX_MASK];
    }
    *(int *)(gBattleContext +
             BATTLE_SELECTED_ACTION_ITEM_OFFSET) = result;
    return result;
}

int BattleItemSelection_SelectUsableIndex(int selected_index) {
    u16 item_id;
    int result;

    *(u16 *)(gBattleContext +
             BATTLE_SELECTED_USABLE_ITEM_INDEX_OFFSET) = selected_index;
    *(u16 *)(gBattleContext + BATTLE_SELECTED_USABLE_ITEM_ID_OFFSET) =
        *(u16 *)(gBattleContext + BATTLE_USABLE_ITEM_LIST_OFFSET +
                 2 * selected_index);
    item_id = *(u16 *)(
        gBattleContext + BATTLE_SELECTED_USABLE_ITEM_ID_OFFSET);
    if ((item_id & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG) {
        result = 0;
    } else {
        result = (int)&gItemRecords[
            item_id & ITEM_ID_INDEX_MASK];
    }
    *(int *)(gBattleContext +
             BATTLE_SELECTED_USABLE_ITEM_OFFSET) = result;
    return result;
}

int BattleItemSelection_SelectActionId(int item_id) {
    int i;

    for (i = 0;; i++) {
        if (i == *(u16 *)(gBattleContext +
                          BATTLE_ACTION_ITEM_COUNT_OFFSET)) {
            return BattleItemSelection_SelectActionIndex(0);
        }
        if (item_id == ((u16 *)(
                gBattleContext + BATTLE_ACTION_ITEM_LIST_OFFSET))[i]) {
            return BattleItemSelection_SelectActionIndex((u16)i);
        }
    }
}

int BattleItemSelection_SelectUsableId(int item_id) {
    int i;

    for (i = 0;; i++) {
        if (i == *(u16 *)(gBattleContext +
                          BATTLE_USABLE_ITEM_COUNT_OFFSET)) {
            return BattleItemSelection_SelectUsableIndex(0);
        }
        if (item_id == ((u16 *)(
                gBattleContext + BATTLE_USABLE_ITEM_LIST_OFFSET))[i]) {
            return BattleItemSelection_SelectUsableIndex((u16)i);
        }
    }
}

int BattleCommandWheel_GetEntryCommand(int entry_index,
                                       int include_disabled) {
    int icon_id =
        *(s16 *)(gBattleContext +
                 BATTLE_COMMAND_WHEEL_ENTRY_FLAGS_OFFSET +
                 8 * entry_index) << 17 >> 17;

    if (include_disabled != 0) {
        return (icon_id + 1) / 2;
    }
    if ((icon_id & 1) != 0) {
        return (icon_id + 1) / 2;
    }
    return 0;
}
