#ifndef PIT_GAME_ITEM_H
#define PIT_GAME_ITEM_H

#include <nitro.h>

enum ItemIdTag {
    ITEM_ID_ACTION_TAG = 0x1000,
    ITEM_ID_USABLE_TAG = 0x2000,
    ITEM_ID_BADGE_TAG = 0x3000,
    ITEM_ID_CLOTHING_TAG = 0x4000,
    ITEM_ID_TAG_MASK = 0xF000,
    ITEM_ID_INDEX_MASK = 0x0FFF
};

typedef struct UsableItemRecord {
    union {
        u8 unknown_00[6];
        struct { u16 unknown_00_word, name_id, unknown_04; };
    };
    union { u16 interface_resource_id, description_id; };
    u8 unknown_08[4];
    union {
        u32 effect_flags;
        struct { u32 unknown_effect_flags : 24, effect_kind : 7, unknown_effect_flag_31 : 1; };
    };
    union { u8 unknown_10[4]; struct { u16 amount, unknown_12; }; };
} UsableItemRecord;

typedef union ActionItemTargetFlags {
    u8 raw;
    struct {
        u8 allows_uniform_targeting : 1;
        u8 unknown_01_07 : 7;
    } bits;
} ActionItemTargetFlags;

typedef struct ActionItemRecord {
    union {
        u8 unknown_00[6];
        struct { u16 unknown_00_word, name_id, unknown_04; };
    };
    union { u16 interface_resource_id, description_id; };
    u8 unknown_08[8];
    ActionItemTargetFlags target_flags;
    u8 unknown_11[11];
} ActionItemRecord;

typedef struct BadgeItemRecord {
    u16 unknown_00, name_id, description_id;
    u8 unknown_06[14];
} BadgeItemRecord;

typedef char BadgeItemRecord_SizeCheck[sizeof(BadgeItemRecord) == 20 ? 1 : -1];

typedef char UsableItemRecord_SizeCheck[
    sizeof(UsableItemRecord) == 20 ? 1 : -1
];
typedef char ActionItemRecord_SizeCheck[
    sizeof(ActionItemRecord) == 28 ? 1 : -1
];

extern UsableItemRecord gItemRecords[];
extern ActionItemRecord gActionItemRecords[];

#ifdef __cplusplus
extern "C" {
#endif
int ItemEffect_CalculateValue(u16 item, int current, int maximum, int unused);
#ifdef __cplusplus
}
#endif

#endif
