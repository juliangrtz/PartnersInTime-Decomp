#ifndef PIT_GAME_SAVE_DATA_H
#define PIT_GAME_SAVE_DATA_H

#include <nitro.h>

/* The saved game. gSaveData points at the live copy of the battery file, and
   most code indexes into it with byte offsets rather than through a single
   struct, because only parts of the layout have been recovered. The records
   below type the pieces that are understood. */

enum PartyMemberId {
    PARTY_MEMBER_MARIO = 0,
    PARTY_MEMBER_LUIGI = 1,
    PARTY_MEMBER_BABY_MARIO = 2,
    PARTY_MEMBER_BABY_LUIGI = 3,
    PARTY_MEMBER_COUNT = 4
};

/* Level and experience share one word: the level in the low byte, the total
   experience in the upper 24 bits. */
typedef union SavePartyExperience {
    struct {
        u32 level : 8;
        u32 value : 24;
    } fields;
    u32 packed;
} SavePartyExperience;

/* The next-level threshold shares its top byte with the member's equipped
   clothing, so the same word reads differently depending on what the caller
   wants; the `equipment` arm exists to make that reuse explicit. */
typedef union SavePartyExperienceToNextLevel {
    struct {
        u32 value : 24;
        u32 unknown : 8;
    } fields;
    struct {
        u8 experience_bytes[3];
        u8 clothing_id;
    } equipment;
    u32 packed;
} SavePartyExperienceToNextLevel;

/* One party member's saved stats, 0x24 bytes, stored as an array of four at
   SAVE_PARTY_MEMBERS_OFFSET. Each stat appears twice: base_ is the value the
   level grants, the unprefixed one includes equipment and status modifiers. */
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
    union {
        u8 unknown_020[4];
        struct {
            u8 badge_id;
            u8 unknown_021[3];
        };
    };
} SavePartyMember;

typedef char SavePartyMember_SizeCheck[
    sizeof(SavePartyMember) == 0x24 ? 1 : -1
];

/* One row of a level-up table: what the member gains at that level. There is
   one table per party member, indexed by the level being reached. */
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
