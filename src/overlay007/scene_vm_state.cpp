#include "scene_vm_state_internal.h"
#include <game/item.h>

extern "C" {

void SceneInput_SetDisabledMask(int mask)
{
    SCENE_MENU.disabled_input = mask;
}

void SceneInput_SetAuxiliaryMask(int mask)
{
    SCENE_MENU.auxiliary_input = mask;
}

void SceneInput_SetRejectedMask(int mask)
{
    SCENE_MENU.rejected_input = mask;
}

void SceneInput_ClearRejectedMask(void)
{
    SceneInput_SetRejectedMask(0);
}

int SceneMenu_GetSelectionIndex(void)
{
    int result = 0;
    switch (data_ov007_0208e1e0->phase) {
    case 2:
        result = (u16)SCENE_MENU.menu;
        break;
    case 3:
        switch (SCENE_MENU.menu) {
        case 0:
            if (data_ov007_0208e1e4->unknown_00)
                result = func_ov007_02075408(data_ov007_0208e1e4);
            break;
        case 1:
            result = gSceneMenuSubmenus[SCENE_MENU.menu];
            break;
        case 3:
            if (data_ov007_0208e1e4->unknown_00)
                result = func_ov007_02075408(data_ov007_0208e1e4);
            break;
        }
        break;
    case 4:
        if (SCENE_MENU.menu == 1) {
            if (data_ov007_0208e1e4->unknown_00)
                result = func_ov007_02075408(data_ov007_0208e1e4);
        }
        break;
    }
    return result;
}

int SceneMenu_GetSelectedItem(void)
{
    u16 result = 0;
    if (data_ov007_0208e1e4->unknown_00) {
        int slot = func_ov007_02075408(data_ov007_0208e1e4);
        result = func_ov007_02075324(data_ov007_0208e1e4, slot);
        switch (func_ov007_02075400(data_ov007_0208e1e4)) {
        case 0:
            result |= ITEM_ID_USABLE_TAG;
            break;
        case 2:
            result |= ITEM_ID_CLOTHING_TAG;
            break;
        case 3:
            result |= ITEM_ID_BADGE_TAG;
            break;
        case 4:
            result |= ITEM_ID_ACTION_TAG;
            break;
        }
    }
    return result;
}

int SceneMenu_GetSelectedMember(void)
{
    return (u16)SCENE_MENU.member;
}

int SceneVM_GetMenuValue(int property)
{
    switch (property) {
    case 0:
        return SceneMenu_GetSelectionIndex();
    case 1:
        return SceneMenu_GetSelectedItem();
    case 2:
        return SceneMenu_GetSelectedMember();
    }
    return 0;
}

int SceneVM_GetSaveValue(int property, int member)
{
    switch (property) {
    case SCENE_SAVE_BASE_MAX_HP:
        return SCENE_SAVE_VALUES.members[member].base_max_hp;
    case SCENE_SAVE_BASE_POWER:
        return SCENE_SAVE_VALUES.members[member].base_power;
    case SCENE_SAVE_BASE_DEFENSE:
        return SCENE_SAVE_VALUES.members[member].base_defense;
    case SCENE_SAVE_BASE_SPEED:
        return SCENE_SAVE_VALUES.members[member].base_speed;
    case SCENE_SAVE_BASE_STACHE:
        return SCENE_SAVE_VALUES.members[member].base_stache;
    case SCENE_SAVE_MAX_HP:
        return SCENE_SAVE_VALUES.members[member].max_hp;
    case SCENE_SAVE_CURRENT_HP:
        return SCENE_SAVE_VALUES.members[member].current_hp;
    case SCENE_SAVE_POWER:
        return SCENE_SAVE_VALUES.members[member].power;
    case SCENE_SAVE_DEFENSE:
        return SCENE_SAVE_VALUES.members[member].defense;
    case SCENE_SAVE_SPEED:
        return SCENE_SAVE_VALUES.members[member].speed;
    case SCENE_SAVE_STACHE:
        return SCENE_SAVE_VALUES.members[member].stache;
    case SCENE_SAVE_LEVEL:
        return SCENE_SAVE_VALUES.members[member].experience.fields.level;
    case SCENE_SAVE_EXPERIENCE:
        return SCENE_SAVE_VALUES.members[member].experience.fields.value;
    case SCENE_SAVE_EXPERIENCE_TO_NEXT_LEVEL:
        return SCENE_SAVE_VALUES.members[member].experience_to_next_level.fields.value;
    case SCENE_SAVE_CLOTHING:
        return SCENE_SAVE_VALUES.members[(u8)member].experience_to_next_level.equipment.clothing_id;
    case SCENE_SAVE_BADGE:
        return SCENE_SAVE_VALUES.members[(u8)member].badge_id;
    case SCENE_SAVE_COINS:
        return SCENE_SAVE_VALUES.coins;
    case SCENE_SAVE_BEANS:
        return SCENE_SAVE_VALUES.beans;
    }
    return 0;
}
}
