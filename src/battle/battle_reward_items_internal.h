#ifndef PIT_BATTLE_REWARD_ITEMS_INTERNAL_H
#define PIT_BATTLE_REWARD_ITEMS_INTERNAL_H
#include <game/battle_reward_items.h>
#include <nitro.h>
typedef struct BattleRewardCounter {
    void *effect;
    u16 item, count;
    void *text;
    s16 x, y;
    u8 digits[4];
    s16 delay;
    struct { u8 done : 1, drawing_count : 1, flag2 : 1, effect_started : 1, rest : 4; } flags;
    u8 row;
} BattleRewardCounter;
typedef char BattleRewardCounterSizeCheck[sizeof(BattleRewardCounter) == 24 ? 1 : -1];

typedef struct BattleRewardItemEntry {
    u16 item, unknown_02;
    u8 counts[3], unknown_07;
    union { u32 unknown_08; u32 record_value; };
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
int BattleRewardCounter_Update(BattleRewardCounter *counter, int unused, int accelerate);
void BattleRewardItems_BeginDisplay(void);
void *BattleRewardItems_GetItemName(int item, int count);
void BattleRewardCounter_Initialize(BattleRewardCounter *counter, int item, unsigned count, int y, int row);
void BattleRewardItems_InitEntry(BattleRewardItemEntry *entries, int index, int item, int count, int category);
int BattleRewardItems_FindEntry(BattleRewardItemEntry *entries, int count, int item, int category);

extern BattleRewardItemsWork *data_ov002_020c06a4;
#endif
