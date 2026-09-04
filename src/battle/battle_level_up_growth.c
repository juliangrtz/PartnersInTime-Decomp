#include <game/battle_ai.h>
#include <game/save_data.h>

enum BattleLevelUpGrowthOffset {
    BATTLE_LEVEL_UP_MEMBER_ID_OFFSET = 0xDA,
    BATTLE_LEVEL_UP_GROWTH_ACTIVE_OFFSET = 0xE4,
    BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET = 0xE6,
    BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET = 0xF0,
    BATTLE_LEVEL_UP_TASK_POOL_OFFSET = 0x8B44,
    SAVE_PARTY_MEMBERS_OFFSET = 0x3F8
};

enum BattleLevelUpGrowthConstant {
    BATTLE_LEVEL_UP_STAT_COUNT = 5,
    BATTLE_LEVEL_UP_ROW_INTERVAL = 3,
    BATTLE_LEVEL_UP_ROW_START_X = 0x2580
};

typedef struct BattleLevelUpRowTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    u8 phase;
    s8 intensity;
    u8 stat_index;
    u8 unknown_0f;
    s16 x;
} BattleLevelUpRowTask;

extern const PartyLevelGrowth *const
    gBattleLevelUpGrowthTables[PARTY_MEMBER_COUNT];

void BattleLevelUpGrowth_UpdateStatRow(BattleAITask *task);
void BattleLevelUpGrowth_UpdateRowSpawner(BattleAITask *task);

BattleAITask *BattleLevelUpGrowth_StartDisplay(void) {
    BattleAITask *task = BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext + BATTLE_LEVEL_UP_TASK_POOL_OFFSET),
        0);
    s16 member_id;
    const PartyLevelGrowth *growth;
    const PartyLevelGrowth *older_growth;

    task->callback = BattleLevelUpGrowth_UpdateRowSpawner;
    *(u16 *)&task->state = 0;

    member_id =
        *(s16 *)(gBattleContext + BATTLE_LEVEL_UP_MEMBER_ID_OFFSET);
    growth = gBattleLevelUpGrowthTables[member_id] +
             ((SavePartyMember *)(gSaveData + SAVE_PARTY_MEMBERS_OFFSET))
                 [member_id]
                     .experience.fields.level;
    older_growth = growth - 2;

    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_ACTIVE_OFFSET) = 1;
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 0) =
        (older_growth + 1)->max_hp - older_growth->max_hp;
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 0) =
        *(s16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 0);
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 2) =
        (older_growth + 1)->power - older_growth->power;
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 2) =
        *(s16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 2);
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 4) =
        (older_growth + 1)->defense - older_growth->defense;
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 4) =
        *(s16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 4);
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 6) =
        (older_growth + 1)->speed - older_growth->speed;
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 6) =
        *(s16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 6);
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 8) = 0;
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 8) =
        *(s16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 8);
    return task;
}

void BattleLevelUpGrowth_UpdateRowSpawner(BattleAITask *task) {
    s16 frame = *(s16 *)&task->state;
    int stat_index = frame / BATTLE_LEVEL_UP_ROW_INTERVAL;
    int remainder = frame % BATTLE_LEVEL_UP_ROW_INTERVAL;

    *(s16 *)&task->state = frame + 1;
    if (stat_index > BATTLE_LEVEL_UP_STAT_COUNT - 1) {
        task->callback = 0;
        return;
    }
    if (remainder == 0) {
        BattleLevelUpRowTask *row =
            (BattleLevelUpRowTask *)BattleTaskList_Insert(
                (BattleTaskPool *)(
                    gBattleContext + BATTLE_LEVEL_UP_TASK_POOL_OFFSET),
                0);
        u8 *row_state = (u8 *)row + 0xC;

        row->callback = BattleLevelUpGrowth_UpdateStatRow;
        row->phase = 1;
        row_state[1] = 0;
        row_state[2] = stat_index;
        *(u16 *)(row_state + 4) = BATTLE_LEVEL_UP_ROW_START_X;
    }
}
