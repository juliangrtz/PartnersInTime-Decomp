extern "C" {
#include <game/save_state_transfer.h>
#include <game/save_storage.h>
#include <game/equipment.h>
#include <game/sprite_output.h>
void MI_CpuCopy8(const void *, void *, u32);
}

enum { SAVE_PERSISTENT_BYTES = 944 };
#define LIVE ((SaveLiveTransferView *)gSaveData)
extern "C" void SaveState_UnpackSlot(void)
{
    const PartyLevelGrowth *growth;
    SaveStorageSlot *slot = SaveStorage_GetSlot(LIVE->slot_select.bits.slot);
    MI_CpuCopy8(slot, LIVE->persistent, SAVE_PERSISTENT_BYTES);
    SaveStoredTail *tail = (SaveStoredTail *)((u8 *)slot + SAVE_PERSISTENT_BYTES);
    SavePartyMember *output = LIVE->party;
    SaveStoredParty *source = tail->party;
    for (int i = 0; i < 4; i++, source++, output++) {
        output->base_max_hp = source->base_max_hp;
        output->base_power = source->base_power;
        output->base_defense = source->base_defense;
        output->base_speed = source->base_speed;
        output->base_stache = source->base_stache;
        output->current_hp = source->current_hp;
        output->experience.fields.level = source->experience.fields.level;
        output->experience.fields.value = source->experience.fields.value;
        output->experience_to_next_level.equipment.clothing_id = source->clothing_id;
        output->badge_id = source->badge_id;
    }
    LIVE->party[0].member_id = 0;
    LIVE->party[1].member_id = 1;
    LIVE->party[2].member_id = 2;
    LIVE->party[3].member_id = 3;
    {
        int i = 0, level_index;
        const PartyLevelGrowth *level;
        SavePartyMember *party = LIVE->party;
        for (; i < 4; i++, party++) {
            switch (party->member_id) {
            case PARTY_MEMBER_MARIO:
                growth = gMarioLevelGrowth;
                break;
            case PARTY_MEMBER_LUIGI:
                growth = gLuigiLevelGrowth;
                break;
            case PARTY_MEMBER_BABY_MARIO:
                growth = gBabyMarioLevelGrowth;
                break;
            case PARTY_MEMBER_BABY_LUIGI:
                growth = gBabyLuigiLevelGrowth;
                break;
            }
            party->experience_to_next_level.fields.value = 0;
            if (party->experience.fields.level < 100) {
                level_index = 0;
                if (level_index < (int)party->experience.fields.level) {
                    level = growth;
                    do {
                        party->experience_to_next_level.fields.value += level->experience_to_next_level;
                        level++;
                    } while (++level_index < party->experience.fields.level);
                }
                u32 experience = party->experience.fields.value;
                if (party->experience_to_next_level.fields.value > experience)
                    party->experience_to_next_level.fields.value -= experience;
                else
                    party->experience_to_next_level.fields.value = 1;
            }
        }
    }
    int party_index = 0;
    SavePartyMember *party = LIVE->party;
    for (; party_index < 4; party_index++, party++) {
        s16 hp = 0, power = 0, defense = 0, speed = 0, stache = 0;
        Equipment_GetBonuses(party->experience_to_next_level.equipment.clothing_id, &hp, &power, &defense,
                             &speed, &stache);
        hp += party->base_max_hp;
        power += party->base_power;
        defense += party->base_defense;
        speed += party->base_speed;
        stache += party->base_stache;
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
        /* Preserve the original asymmetric speed and stache limits. */
        if (speed < 0)
            speed = 0;
        if (stache > 999)
            stache = 999;
        party->max_hp = hp;
        party->power = power;
        party->defense = defense;
        party->speed = speed;
        party->stache = stache;
    }
    LIVE->coins = tail->coins;
    LIVE->play_time = tail->play_time;
    LIVE->unknown_516 = tail->unknown_e0;
    LIVE->unknown_517 = tail->unknown_df;
    LIVE->unknown_48c = tail->unknown_dc;
    for (int i = 0; i < 14; i++) {
        LIVE->items[i] = tail->items[i];
        if (LIVE->items[i] > 99)
            LIVE->items[i] = 99;
    }
    for (int i = 0; i < 11; i++) {
        LIVE->bros_items[i] = tail->bros_items[i];
        if (LIVE->bros_items[i] > 99)
            LIVE->bros_items[i] = 99;
    }
    for (int i = 0; i < 33; i++) {
        LIVE->clothing[i] = tail->clothing[i];
        if (LIVE->clothing[i] > 9)
            LIVE->clothing[i] = 9;
    }
    for (int i = 0; i < 41; i++) {
        LIVE->badges[i] = tail->badges[i];
        if (LIVE->badges[i] > 9)
            LIVE->badges[i] = 9;
    }
}
extern "C" void SaveState_PackSlot(void)
{
    SaveStorageSlot *slot = SaveStorage_GetSlot(LIVE->slot_select.bits.slot);
    MI_CpuCopy8(LIVE->persistent, slot, SAVE_PERSISTENT_BYTES);
    SaveStoredTail *tail = (SaveStoredTail *)((u8 *)slot + SAVE_PERSISTENT_BYTES);
    SavePartyMember *source = LIVE->party;
    SaveStoredParty *output = tail->party;
    for (int i = 0; i < 4; i++, source++, output++) {
        output->base_max_hp = source->base_max_hp;
        output->base_power = source->base_power;
        output->base_defense = source->base_defense;
        output->base_speed = source->base_speed;
        output->base_stache = source->base_stache;
        output->current_hp = source->current_hp;
        output->experience.fields.level = source->experience.fields.level;
        output->experience.fields.value = source->experience.fields.value;
        output->clothing_id = source->experience_to_next_level.equipment.clothing_id;
        output->badge_id = source->badge_id;
    }
    tail->coins = LIVE->coins;
    tail->play_time = LIVE->play_time;
    tail->unknown_e0 = LIVE->unknown_516;
    tail->unknown_df = LIVE->unknown_517;
    tail->location = LIVE->location;
    tail->unknown_dc = LIVE->unknown_48c;
    for (int i = 0; i < 14; i++)
        tail->items[i] = LIVE->items[i];
    for (int i = 0; i < 11; i++)
        tail->bros_items[i] = LIVE->bros_items[i];
    for (int i = 0; i < 33; i++)
        tail->clothing[i] = LIVE->clothing[i];
    for (int i = 0; i < 41; i++)
        tail->badges[i] = LIVE->badges[i];
    GameOam_CopyState(SaveStorage_GetSlotExtra(LIVE->slot_select.bits.slot));
}
