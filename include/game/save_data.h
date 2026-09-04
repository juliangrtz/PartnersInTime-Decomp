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
        u32 unknown : 8;
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
    u8 unknown_000[0x18];
    SavePartyExperience experience;
    SavePartyExperienceToNextLevel experience_to_next_level;
    u8 unknown_020[4];
} SavePartyMember;

typedef char SavePartyMember_SizeCheck[
    sizeof(SavePartyMember) == 0x24 ? 1 : -1
];

typedef struct PartyLevelGrowth {
    u8 unknown_000[8];
    u16 experience_to_next_level;
    u8 unknown_00a[2];
} PartyLevelGrowth;

typedef char PartyLevelGrowth_SizeCheck[
    sizeof(PartyLevelGrowth) == 0x0C ? 1 : -1
];

extern u8 *gSaveData;
extern const PartyLevelGrowth gMarioLevelGrowth[];
extern const PartyLevelGrowth gLuigiLevelGrowth[];
extern const PartyLevelGrowth gBabyMarioLevelGrowth[];
extern const PartyLevelGrowth gBabyLuigiLevelGrowth[];

#endif
