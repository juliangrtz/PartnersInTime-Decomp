#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/item.h>
#include <game/save_data.h>

enum BattleInterfaceResourceOffset {
    BATTLE_ACTIVE_PARTY_ACTOR_ID_OFFSET = 0x20,
    BATTLE_COMMAND_MODE_A_OFFSET = 0x65EE,
    BATTLE_COMMAND_MODE_B_OFFSET = 0x65F0,
    BATTLE_COMMAND_MENU_RESOURCE_OFFSET = 0x6642,
    BATTLE_TARGET_CURSOR_LEFT_OFFSET = 0x6696,
    BATTLE_TARGET_CURSOR_RIGHT_OFFSET = 0x6698,
    BATTLE_TARGET_LABEL_LAYER_OFFSET = 0x669C,
    BATTLE_TARGET_LABEL_STATE_OFFSET = 0x66DC,
    BATTLE_ITEM_NAME_LAYER_OFFSET = 0x66EC,
    BATTLE_ITEM_NAME_STATE_OFFSET = 0x672C,
    SAVE_LANGUAGE_OFFSET = 0x515
};

extern void *func_ov002_02076b00(int archive_id, u16 resource_id);
extern int BattleInterfaceLayer_SetResource(
    BattleInterfaceLayer *layer, void *resource, int unknown_2, int unknown_3,
    int unknown_4, int unknown_5);

int BattleInterface_LoadTargetLabelResource(int resource_id) {
    int result = 0;
    void *resource = func_ov002_02076b00(0, resource_id);

    if (resource != 0) {
        *(u32 *)(gBattleContext + BATTLE_TARGET_LABEL_STATE_OFFSET) = 0;
        result = BattleInterfaceLayer_SetResource(
            (BattleInterfaceLayer *)(gBattleContext +
                                     BATTLE_TARGET_LABEL_LAYER_OFFSET),
            resource, 3, 3,
            *(s16 *)(gBattleContext + BATTLE_TARGET_CURSOR_LEFT_OFFSET) ==
                *(s16 *)(gBattleContext + BATTLE_TARGET_CURSOR_RIGHT_OFFSET),
            1);
    }
    return result;
}

int BattleInterface_LoadCommandMenu(int resource_id) {
    int selected_resource = resource_id;

    if (resource_id == -1) {
        BattlePartyActor *actor = (BattlePartyActor *)BattleActor_GetPartySlot(
            *(u16 *)(gBattleContext + BATTLE_ACTIVE_PARTY_ACTOR_ID_OFFSET));
        u16 formation_index = actor->formation_index;

        if (*(s16 *)(gBattleContext + BATTLE_COMMAND_MODE_B_OFFSET) != 0 ||
            *(s16 *)(gBattleContext + BATTLE_COMMAND_MODE_A_OFFSET) != 0) {
            if (formation_index == 2) {
                selected_resource = 6;
            } else if (formation_index == 3) {
                selected_resource = 7;
            } else if (*(u16 *)(gBattleContext +
                                BATTLE_ACTIVE_PARTY_ACTOR_ID_OFFSET) == 57) {
                selected_resource = 5;
            } else {
                selected_resource = 4;
            }
        } else if (formation_index == 2) {
            selected_resource = 2;
        } else if (formation_index == 3) {
            selected_resource = 3;
        } else {
            selected_resource =
                *(u16 *)(gBattleContext +
                         BATTLE_ACTIVE_PARTY_ACTOR_ID_OFFSET) == 57;
        }
    }

    if (func_ov002_02076b00(0, selected_resource) != 0) {
        *(u16 *)(gBattleContext + BATTLE_COMMAND_MENU_RESOURCE_OFFSET) =
            selected_resource + 1;
    }
    return 0;
}

int BattleInterface_LoadItemName(int item_id) {
    int result = 0;
    void *resource = 0;
    int language_variant = 0;

    switch (item_id & ITEM_ID_TAG_MASK) {
    case ITEM_ID_ACTION_TAG: {
        ActionItemRecord *item;

        if ((item_id & ITEM_ID_ACTION_TAG) != ITEM_ID_ACTION_TAG) {
            item = 0;
        } else {
            item = &gActionItemRecords[item_id & ITEM_ID_INDEX_MASK];
        }
        resource = func_ov002_02076b00(5, item->interface_resource_id);
        break;
    }
    case ITEM_ID_USABLE_TAG: {
        UsableItemRecord *item;

        if ((item_id & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG) {
            item = 0;
        } else {
            item = &gItemRecords[item_id & ITEM_ID_INDEX_MASK];
        }
        resource = func_ov002_02076b00(8, item->interface_resource_id);
        if (gSaveData[SAVE_LANGUAGE_OFFSET] == 0) {
            language_variant = 8;
        }
        if (gSaveData[SAVE_LANGUAGE_OFFSET] == 1) {
            language_variant = 8;
        }
        break;
    }
    }

    if (resource != 0) {
        *(u32 *)(gBattleContext + BATTLE_ITEM_NAME_STATE_OFFSET) = 0;
        result = BattleInterfaceLayer_SetResource(
            (BattleInterfaceLayer *)(gBattleContext +
                                     BATTLE_ITEM_NAME_LAYER_OFFSET),
            resource, 0, language_variant, 1, 1);
    }
    return result;
}
