#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
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
typedef struct BattleTargetSelectionEntry {
    s16 packed_command;
    u8 unknown_02[6];
} BattleTargetSelectionEntry;

typedef char BattleTargetSelectionEntry_SizeCheck[
    sizeof(BattleTargetSelectionEntry) == 8 ? 1 : -1
];

int BattleInterface_LoadTargetName(u32 actor_id) {
    int result;
    int resource_id;
    void *resource;
    u8 *context;

    if (actor_id == 0) {
        int selection_index = *(s16 *)(gBattleContext + 0x11A);
        BattleTargetSelectionEntry *entry =
            (BattleTargetSelectionEntry *)(gBattleContext + 0x653E) +
            selection_index;
        int command = entry->packed_command << 17 >> 17;

        switch (command) {
        case 0:
            return 0;
        case 1:
            resource_id = 9;
            break;
        case 2:
            resource_id = 9;
            break;
        case 3:
            resource_id = 10;
            break;
        case 4:
            resource_id = 10;
            break;
        case 5:
            resource_id = 11;
            break;
        case 6:
            resource_id = 11;
            break;
        case 7:
            resource_id = 13;
            break;
        case 8:
            resource_id = 13;
            break;
        case 9:
            resource_id = 12;
            break;
        case 10:
            resource_id = 12;
            break;
        case 11:
            resource_id = 8;
            break;
        case 12:
            return 0;
        case 13:
        case 14:
            resource_id = 14;
            break;
        case 15:
        case 16:
            resource_id = 15;
            break;
        case 17:
        case 18:
            resource_id = 16;
            break;
        case 19:
        case 20:
            resource_id = 17;
            break;
        case 21:
        case 22:
            resource_id = 20;
            break;
        }
        resource = BattleText_GetEntry(0, resource_id);
    } else {
        if (actor_id < BATTLE_ACTOR_PARTY_FIRST) {
            resource_id = -1;
        } else if (actor_id < BATTLE_ACTOR_ENEMY_FIRST) {
            resource_id = -1;
        } else if (actor_id < BATTLE_ACTOR_ENEMY_FIRST) {
            /* Preserved from the original actor-range dispatch. */
            resource_id = -1;
        } else if (actor_id <
                   BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT) {
            resource_id = BattleEnemy_GetStats(actor_id)->name_id;
        }
        resource = BattleText_GetEntry(1, resource_id);
    }

    result = 0;
    if (resource != 0) {
        *(u32 *)(gBattleContext + BATTLE_TARGET_LABEL_STATE_OFFSET) = 0;
        context = gBattleContext;
        result = BattleInterfaceLayer_SetResource(
            (BattleInterfaceLayer *)(context +
                                     BATTLE_TARGET_LABEL_LAYER_OFFSET),
            resource, 3, 3,
            *(s16 *)(context + BATTLE_TARGET_CURSOR_LEFT_OFFSET) ==
                *(s16 *)(context + BATTLE_TARGET_CURSOR_RIGHT_OFFSET),
            1);
    }
    return result;
}

int BattleInterface_LoadTargetLabelResource(int resource_id) {
    int result = 0;
    void *resource = BattleText_GetEntry(0, resource_id);

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

    if (BattleText_GetEntry(0, selected_resource) != 0) {
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
        resource = BattleText_GetEntry(5, item->interface_resource_id);
        break;
    }
    case ITEM_ID_USABLE_TAG: {
        UsableItemRecord *item;

        if ((item_id & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG) {
            item = 0;
        } else {
            item = &gItemRecords[item_id & ITEM_ID_INDEX_MASK];
        }
        resource = BattleText_GetEntry(8, item->interface_resource_id);
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

int BattleTexture_EncodeWidth(int pixel_width) {
    switch (pixel_width) {
    case 8:
        return 0;
    case 16:
        return 1;
    case 32:
        return 2;
    case 64:
        return 3;
    case 128:
        return 4;
    case 256:
        return 5;
    case 512:
        return 6;
    case 1024:
        return 7;
    default:
        return 0;
    }
}

int BattleTexture_EncodeHeight(int pixel_height) {
    switch (pixel_height) {
    case 8:
        return 0;
    case 16:
        return 1;
    case 32:
        return 2;
    case 64:
        return 3;
    case 128:
        return 4;
    case 256:
        return 5;
    case 512:
        return 6;
    case 1024:
        return 7;
    default:
        return 0;
    }
}

#include <game/battle_interface_text.h>
#define REG32(address) (*(volatile u32 *)(address))
static inline void TextTexCoord(int s, int t) {
    REG32(0x04000488) = (u16)(s16)(s >> 8) | ((u16)(s16)(t >> 8) << 16);
}
static inline void TextVertex(s16 x, s16 y, s16 z) {
    REG32(0x0400048c) = (u16)x | ((u16)y << 16);
    REG32(0x0400048c) = (u16)z;
}
/* Advances the tile buffer for text wider than its visible panel. */
void func_ov002_02092390(BattleInterfaceLayerState *state);
int BattleTexture_EncodeHeight(int), BattleTexture_EncodeWidth(int);
int _s32_div_f(int, int);
void BattleInterface_DrawText(BattleSpriteTransform *transform, BattleInterfaceLayerState *state) {
    BattleInterfaceLayer *layer = &state->layer;
    int columns, rows, height;
    u32 texture;
    int x, y, width, draw_height;
    if (!transform)
        return;
    if (layer->flags.bits.state_15)
        func_ov002_02092390(state);
    columns = layer->layout.bits.row_tiles_minus_one + 1;
    /* Keep the native division call before reading the texture allocation. */
    rows = _s32_div_f(layer->allocation_size, 32 * columns);
    texture = layer->vram_offset;
    height = BattleTexture_EncodeHeight(8 * rows);
    REG32(0x040004a8) = (texture >> 3) | 0x4c000000 |
                        (BattleTexture_EncodeWidth(8 * columns) << 20) | (height << 23) |
                        0x20030000;
    REG32(0x040004ac) = 0;
    REG32(0x04000500) = 1;
    x = layer->x;
    y = layer->y;
    width = (s16)layer->width;
    draw_height = (s16)layer->height;
    TextTexCoord((s16)state->transition_progress << 4, 0);
    TextVertex(x << 8, y << 8, -2);
    TextTexCoord((s16)state->transition_progress << 4, draw_height << 12);
    TextVertex(x << 8, (y + draw_height) << 8, -2);
    TextTexCoord(((s16)state->transition_progress << 4) + (width << 12), draw_height << 12);
    TextVertex((x + width) << 8, (y + draw_height) << 8, -2);
    TextTexCoord(((s16)state->transition_progress << 4) + (width << 12), 0);
    TextVertex((x + width) << 8, y << 8, -2);
    REG32(0x04000504) = 0;
}
