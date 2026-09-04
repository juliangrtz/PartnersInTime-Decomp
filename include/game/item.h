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
    u8 unknown_00[6];
    u16 interface_resource_id;
    u8 unknown_08[12];
} UsableItemRecord;

typedef struct ActionItemRecord {
    u8 unknown_00[6];
    u16 interface_resource_id;
    u8 unknown_08[20];
} ActionItemRecord;

typedef char UsableItemRecord_SizeCheck[
    sizeof(UsableItemRecord) == 20 ? 1 : -1
];
typedef char ActionItemRecord_SizeCheck[
    sizeof(ActionItemRecord) == 28 ? 1 : -1
];

extern UsableItemRecord gItemRecords[];
extern ActionItemRecord gActionItemRecords[];

#endif
