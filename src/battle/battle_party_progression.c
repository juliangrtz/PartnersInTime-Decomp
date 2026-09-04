#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/save_data.h>

enum BattlePartyProgressionOffset {
    BATTLE_PARTY_LEVELS_OFFSET = 0xD2,
    SAVE_PARTY_MEMBERS_OFFSET = 0x3F8
};

enum BattlePartyProgressionLimit {
    PARTY_MAX_LEVEL = 100,
    PARTY_MAX_EXPERIENCE = 999999
};

typedef struct BattlePartyProgressContext {
    u8 unknown_000[BATTLE_PARTY_LEVELS_OFFSET];
    s16 levels[PARTY_MEMBER_COUNT];
} BattlePartyProgressContext;

u32 BattleParty_AddExperience(u32 member_id, u32 amount) {
    SavePartyMember *member;
    u32 experience_to_next_level;
    u32 experience;
    int remaining;
    int level;

    switch (member_id) {
    case PARTY_MEMBER_MARIO:
        member_id = PARTY_MEMBER_MARIO;
        break;
    case PARTY_MEMBER_LUIGI:
        member_id = PARTY_MEMBER_LUIGI;
        break;
    case PARTY_MEMBER_BABY_MARIO:
        member_id = PARTY_MEMBER_BABY_MARIO;
        break;
    case PARTY_MEMBER_BABY_LUIGI:
        member_id = PARTY_MEMBER_BABY_LUIGI;
        break;
    }

    member = (SavePartyMember *)(gSaveData + SAVE_PARTY_MEMBERS_OFFSET +
                                 sizeof(SavePartyMember) * member_id);
    experience_to_next_level = member->experience_to_next_level.fields.value;
    experience = member->experience.fields.value;

    if (PARTY_MAX_EXPERIENCE - amount < experience) {
        amount = PARTY_MAX_EXPERIENCE - experience;
    }
    member->experience.fields.value = amount + (member->experience.packed >> 8);
    remaining = experience_to_next_level - amount;

    level = ((BattlePartyProgressContext *)gBattleContext)->levels[member_id];
    if (level < PARTY_MAX_LEVEL) {
        do {
            if (remaining > 0) {
                u32 packed;

                ((BattlePartyProgressContext *)gBattleContext)
                    ->levels[member_id] = level;
                packed = member->experience_to_next_level.packed & 0xFF000000;
                packed |= remaining & 0x00FFFFFF;
                member->experience_to_next_level.packed = packed;
                return packed;
            }

            switch (member_id) {
            case PARTY_MEMBER_MARIO:
                remaining +=
                    gMarioLevelGrowth[level++].experience_to_next_level;
                break;
            case PARTY_MEMBER_LUIGI:
                remaining +=
                    gLuigiLevelGrowth[level++].experience_to_next_level;
                break;
            case PARTY_MEMBER_BABY_MARIO:
                remaining +=
                    gBabyMarioLevelGrowth[level++].experience_to_next_level;
                break;
            case PARTY_MEMBER_BABY_LUIGI:
                remaining +=
                    gBabyLuigiLevelGrowth[level++].experience_to_next_level;
                break;
            }
        } while (level < PARTY_MAX_LEVEL);

        {
            u32 packed;

            ((BattlePartyProgressContext *)gBattleContext)->levels[member_id] =
                PARTY_MAX_LEVEL;
            packed = member->experience_to_next_level.packed & 0xFF000000;
            member->experience_to_next_level.packed = packed;
            return packed;
        }
    }

    return member_id;
}
