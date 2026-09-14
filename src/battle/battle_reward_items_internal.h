#ifndef PIT_BATTLE_REWARD_ITEMS_INTERNAL_H
#define PIT_BATTLE_REWARD_ITEMS_INTERNAL_H
#include <game/battle_reward_items.h>
#include <nitro.h>
typedef struct BattleRewardItemEntry {
    u16 item, unknown_02;
    u8 counts[3], unknown_07;
    u32 unknown_08;
} BattleRewardItemEntry;
typedef struct BattleRewardItemsWork {
    void (*update)(void), (*draw)(void);
    /* Embedded scene objects and text/layout state. */
    u8 unknown_008[1232];
    union {
        u32 raw;
        struct {
            u32 done : 1, ready : 1, load_phase : 4, phase : 4;
            u32 unknown10 : 12, item_count : 4, unknown26 : 6;
        } bits;
    } flags;
    int fade;
    BattleRewardItemEntry items[8];
} BattleRewardItemsWork;
typedef char BattleRewardItemsWorkSizeCheck[sizeof(BattleRewardItemsWork) == 1344 ? 1 : -1];
typedef char BattleRewardItemEntrySizeCheck[sizeof(BattleRewardItemEntry) == 12 ? 1 : -1];
extern BattleRewardItemsWork *data_ov002_020c06a4;
#endif
