#include "save_menu_internal.h"

typedef struct StoredSaveMenuPartyMember {
    u16 max_hp, power, defense, speed, stache, current_hp;
    SavePartyExperience experience;
    u8 equipment, badge, unknown_12[2];
} StoredSaveMenuPartyMember;
typedef struct StoredMenuState {
    u32 flags;
    u8 unknown_004[940];
    StoredSaveMenuPartyMember party[4];
    u32 coins;
    u8 unknown_404[132];
    u32 play_time;
    u8 unknown_48c[2];
    u8 location, unknown_48f, unknown_490;
} StoredMenuState;
void SaveMenu_BuildLiveSummary(SaveMenuSummary *summary)
{
    int i;
    SaveMenuPartyMember *output;
    SaveMenuPartyMember *source;
    for (i = 0; i < 4; ++i)
        summary->present[i] = 1;
    if (!VM_ReadVariable(0x201B, 0, 0)) {
        if (VM_ReadVariable(0x2005, 0, 0)) {
            summary->present[1] = 0;
            summary->present[2] = 0;
            summary->present[3] = 0;
        }
        if (VM_ReadVariable(0x2006, 0, 0)) {
            summary->present[2] = 0;
            summary->present[3] = 0;
        }
        if (VM_ReadVariable(0x2007, 0, 0)) {
            summary->present[0] = 0;
            summary->present[1] = 0;
        }
    }
    output = summary->party;
    source = (SaveMenuPartyMember *)(gSaveData + 1016);
    for (i = 0; i < 4; ++i, ++source, ++output) {
        output->base_max_hp = source->base_max_hp;
        output->base_power = source->base_power;
        output->base_defense = source->base_defense;
        output->base_speed = source->base_speed;
        output->base_stache = source->base_stache;
        output->current_hp = source->current_hp;
        output->max_hp = source->max_hp;
        output->power = source->power;
        output->defense = source->defense;
        output->speed = source->speed;
        output->stache = source->stache;
        output->experience.fields.level = source->experience.fields.level;
        output->experience.fields.value = source->experience.fields.value;
        output->equipment = source->equipment;
        output->badge = source->badge;
    }
    summary->coins = *(u32 *)(gSaveData + 1160);
    summary->play_time = *(u32 *)(gSaveData + 1304);
    summary->unknown_9c = gSaveData[1302];
    summary->unknown_9d = gSaveData[1303];
    summary->location = gSaveData[1376];
}

void SaveMenu_BuildStoredSummary(int slot, SaveMenuSummary *summary)
{
    StoredMenuState *stored = (StoredMenuState *)SaveStorage_GetSlot(slot);
    int i;
    SaveMenuPartyMember *output;
    StoredSaveMenuPartyMember *source;
    for (i = 0; i < 4; ++i)
        summary->present[i] = 1;
    if (!((stored->flags >> 27) & 1)) {
        if ((stored->flags >> 5) & 1) {
            summary->present[1] = 0;
            summary->present[2] = 0;
            summary->present[3] = 0;
        }
        if ((stored->flags >> 6) & 1) {
            summary->present[2] = 0;
            summary->present[3] = 0;
        }
        if ((stored->flags >> 7) & 1) {
            summary->present[0] = 0;
            summary->present[1] = 0;
        }
    }
    output = summary->party;
    source = stored->party;
    for (i = 0; i < 4; ++i, ++source, ++output) {
        output->base_max_hp = source->max_hp;
        output->base_power = source->power;
        output->base_defense = source->defense;
        output->base_speed = source->speed;
        output->base_stache = source->stache;
        output->current_hp = source->current_hp;
        output->experience.fields.level = source->experience.fields.level;
        output->experience.fields.value = source->experience.fields.value;
        output->equipment = source->equipment;
        output->badge = source->badge;
    }
    output = summary->party;
    for (i = 0; i < 4; ++i, ++output) {
        s16 hp = 0, power = 0, defense = 0, speed = 0, stache = 0;
        Equipment_GetBonuses(output->equipment, &hp, &power, &defense, &speed, &stache);
        hp += output->base_max_hp;
        power += output->base_power;
        defense += output->base_defense;
        speed += output->base_speed;
        stache += output->base_stache;
        if (hp < 0)
            hp = 0;
        if (hp > 999)
            hp = 999;
        if (power < 0)
            power = 0;
        if (power > 999)
            power = 999;
        if (defense < 0)
            defense = 0;
        if (defense > 999)
            defense = 999;
        /* The original equipment path uses these asymmetric final limits. */
        if (speed < 0)
            speed = 0;
        if (stache > 999)
            stache = 999;
        output->max_hp = hp;
        output->power = power;
        output->defense = defense;
        output->speed = speed;
        output->stache = stache;
    }
    summary->coins = stored->coins;
    summary->play_time = stored->play_time;
    summary->unknown_9c = stored->unknown_490;
    summary->unknown_9d = stored->unknown_48f;
    summary->location = stored->location;
}
