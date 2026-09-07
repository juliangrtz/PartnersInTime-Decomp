#ifndef PIT_GAME_SAVE_DATA_H
#define PIT_GAME_SAVE_DATA_H

#include <nitro.h>

enum PartyMemberId {
    PARTY_MEMBER_MARIO = 0,
    PARTY_MEMBER_LUIGI = 1,
    PARTY_MEMBER_BABY_MARIO = 2,
    PARTY_MEMBER_BABY_LUIGI = 3,
    PARTY_MEMBER_COUNT = 4
};

typedef union SavePartyExperience {
    struct {
        u32 level : 8;
        u32 value : 24;
    } fields;
    u32 packed;
} SavePartyExperience;

typedef union SavePartyExperienceToNextLevel {
    struct {
        u32 value : 24;
        u32 unknown : 8;
    } fields;
    u32 packed;
} SavePartyExperienceToNextLevel;

typedef struct SavePartyMember {
    u16 member_id;
    u16 base_max_hp;
    u16 base_power;
    u16 base_defense;
    u16 base_speed;
    u16 base_stache;
    u16 max_hp;
    u16 current_hp;
    u16 power;
    u16 defense;
    u16 speed;
    u16 stache;
    SavePartyExperience experience;
    SavePartyExperienceToNextLevel experience_to_next_level;
    u8 unknown_020[4];
} SavePartyMember;

typedef char SavePartyMember_SizeCheck[
    sizeof(SavePartyMember) == 0x24 ? 1 : -1
];

typedef struct PartyLevelGrowth {
    u16 max_hp;
    u16 power;
    u16 defense;
    u16 speed;
    u16 experience_to_next_level;
    u16 stache;
} PartyLevelGrowth;

typedef char PartyLevelGrowth_SizeCheck[
    sizeof(PartyLevelGrowth) == 0x0C ? 1 : -1
];

/* Prefix of the live save context containing the language and loaded text
   tables. The table pointers are runtime data, not serialized file offsets. */
typedef struct SaveLocalizedResourceState {
    u8 unknown_000[0x515];
    u8 language;
    u8 unknown_516[10];
    const u8 *tables[14];
} SaveLocalizedResourceState;
typedef char SaveLocalizedResourceState_SizeCheck[
    sizeof(SaveLocalizedResourceState) == 0x558 ? 1 : -1];

extern u8 *gSaveData;
extern const PartyLevelGrowth gMarioLevelGrowth[];
extern const PartyLevelGrowth gLuigiLevelGrowth[];
extern const PartyLevelGrowth gBabyMarioLevelGrowth[];
extern const PartyLevelGrowth gBabyLuigiLevelGrowth[];

#endif
