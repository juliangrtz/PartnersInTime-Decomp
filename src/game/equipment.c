#include <game/equipment.h>

extern const s16 data_020505d4[][14];
extern const s16 data_020505d6[][14];
extern const s16 data_020505d8[][14];
extern const s16 data_020505da[][14];
extern const s16 data_020505dc[][14];

void Equipment_GetBonuses(u16 equipment, s16 *hp, s16 *power, s16 *defense, s16 *speed, s16 *stache)
{
    u16 index = equipment & 0x0fff;
    *hp = data_020505d4[index][0];
    *power = data_020505d6[index][0];
    *defense = data_020505d8[index][0];
    *speed = data_020505da[index][0];
    *stache = data_020505dc[index][0];
}

void Equipment_Apply(u8 party_member, u16 equipment)
{
    int offset = party_member * 36;
    SavePartyMember *member = (SavePartyMember *)(gSaveData + 1016 + offset);
    u16 kind = equipment & 0xf000;
    u16 index = equipment & 0x0fff;
    s16 hp = 0;
    s16 power = 0;
    s16 defense = 0;
    s16 speed = 0;
    s16 stache = 0;
    switch (kind) {
    case 0x4000:
        Equipment_GetBonuses(index, &hp, &power, &defense, &speed, &stache);
        hp += member->base_max_hp;
        power += member->base_power;
        defense += member->base_defense;
        speed += member->base_speed;
        stache += member->base_stache;
        if (hp < 0) hp = 0;
        if (hp > 999) hp = 999;
        if (power < 0) power = 0;
        if (power > 999) power = 999;
        if (defense < 0) defense = 0;
        if (defense > 999) defense = 999;
        /* These asymmetric limits follow the original equipment path. */
        if (speed < 0) speed = 0;
        if (stache > 999) stache = 999;
        member->max_hp = hp;
        member->power = power;
        member->defense = defense;
        member->speed = speed;
        member->stache = stache;
        if (member->max_hp < member->current_hp) member->current_hp = member->max_hp;
        (gSaveData + offset)[1047] = index;
        break;
    case 0x3000:
        (gSaveData + offset)[1048] = index;
        break;
    }
}
