#include <game/inventory.h>
#include <game/equipment.h>
extern void func_0202cbd4(void *, int, u32);
extern const s16 data_02055034[];
extern const s16 data_0205502c[];
extern const s16 data_02054fdc[];
extern const s16 data_0205500c[];
extern const s16 data_02054fec[];
extern const s16 data_02055014[];
extern const s16 data_02054fe4[];
extern const s16 data_02054fc4[];
extern const s16 data_02055044[];
extern const s16 data_0205503c[];
extern const s16 data_02054ffc[];
extern const s16 data_02054fd4[];
extern const s16 data_02055004[];
extern const s16 data_02055024[];
extern const s16 data_02054fcc[];
extern const s16 data_0205501c[];
extern const s16 data_02054fbc[];
extern const s16 data_02054ff4[];

void GameParty_Initialize(int preset) {
    int i;
    SavePartyMember *member;
    const PartyLevelGrowth *growth;
    func_0202cbd4(gSaveData + 1016, 0, 284);
    for (i = 0; i < 4; ++i) {
        int trousers = 0;
        int badge = 0;
        int level;
        switch (i) {
        case 0:
            member = (SavePartyMember *)(gSaveData + 1016);
            member->member_id = 0;
            member->experience.fields.level = data_02055034[preset];
            growth = gMarioLevelGrowth;
            trousers = data_02054fec[preset];
            badge = data_02055044[preset];
            break;
        case 1:
            member = (SavePartyMember *)(gSaveData + 1052);
            member->member_id = 1;
            member->experience.fields.level = data_0205502c[preset];
            growth = gLuigiLevelGrowth;
            trousers = data_02055014[preset];
            badge = data_0205503c[preset];
            break;
        case 2:
            member = (SavePartyMember *)(gSaveData + 1088);
            member->member_id = 2;
            member->experience.fields.level = data_02054fdc[preset];
            growth = gBabyMarioLevelGrowth;
            trousers = data_02054fe4[preset];
            badge = data_02054ffc[preset];
            break;
        case 3:
            member = (SavePartyMember *)(gSaveData + 1124);
            member->member_id = 3;
            member->experience.fields.level = data_0205500c[preset];
            growth = gBabyLuigiLevelGrowth;
            trousers = data_02054fc4[preset];
            badge = data_02054fd4[preset];
            break;
        }
        member->experience.fields.value = 0;
        for (level = 1; level < member->experience.fields.level; ++level) {
            member->experience.fields.value += growth[level - 1].experience_to_next_level;
        }
        if (member->experience.fields.level)
            growth += member->experience.fields.level - 1;
        member->base_max_hp = growth->max_hp + (((int)member->experience.fields.level - 1) << 7) / 256;
        member->base_power = growth->power + (((int)member->experience.fields.level - 1) << 7) / 256;
        member->base_defense = growth->defense + (((int)member->experience.fields.level - 1) << 7) / 256;
        member->base_speed = growth->speed + (((int)member->experience.fields.level - 1) << 7) / 256;
        member->base_stache = growth->stache;
        member->experience_to_next_level.fields.value = growth->experience_to_next_level;
        member->max_hp = member->base_max_hp;
        member->power = member->base_power;
        member->defense = member->base_defense;
        member->speed = member->base_speed;
        member->stache = member->base_stache;
        GameInventory_Add((u16)trousers, 1);
        GameInventory_Add((u16)badge, 1);
        Equipment_Apply((u8)i, (u16)trousers);
        Equipment_Apply((u8)i, (u16)badge);
        member->current_hp = member->max_hp;
    }
    *(u32 *)(gSaveData + 1160) = 0;
    *(u16 *)(gSaveData + 1164) = 0;
    gSaveData[1166] = data_02055004[preset];
    gSaveData[1170] = data_02055024[preset];
    gSaveData[1173] = data_02054fcc[preset];
    gSaveData[1182] = data_0205501c[preset];
    gSaveData[1184] = data_02054fbc[preset];
    gSaveData[1188] = data_02054ff4[preset];
}
