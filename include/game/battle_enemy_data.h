#ifndef PIT_GAME_BATTLE_ENEMY_DATA_H
#define PIT_GAME_BATTLE_ENEMY_DATA_H

#include <game/battle_task_queue.h>

typedef union BattleEnemyItemDrop {
    u32 packed;
    struct {
        u16 item_id;
        u16 chance_percent;
    } fields;
} BattleEnemyItemDrop;

typedef struct BattleEnemyStatRecord {
    u16 name_id;
    u16 packed_object_data_id;
    union {
        u16 flags_and_level;
        struct {
            u8 unknown_04;
            u8 level;
        };
    };
    u16 max_hp;
    u16 power;
    u16 defense;
    u16 speed;
    union {
        u16 traits;
        struct {
            u16 trait_class : 2;
            u16 unknown_trait_02 : 1;
            u16 unknown_traits_03_15 : 13;
        } trait_bits;
        struct {
            u16 unknown_resistance_00_07 : 8;
            u16 status_3_resistance : 2;
            u16 status_2_resistance : 2;
            u16 status_4_5_resistance : 2;
            u16 status_6_8_resistance : 2;
        } resistance_bits;
    };
    u16 unknown_10;
    u8 unknown_12[14];
    u16 experience;
    u16 coins;
    BattleEnemyItemDrop item_drop_1;
    BattleEnemyItemDrop item_drop_2;
} BattleEnemyStatRecord;

typedef union BattleEnemyDataPayload {
    struct {
        BattleEnemyStatRecord stats;
        u8 object_data[0x1FD4];
    } loaded;
    u8 bytes[0x2000];
} BattleEnemyDataPayload;

typedef struct BattleEnemyDataRequest {
    BattleEnemyStatRecord *stats;
    u8 *object_data;
    u8 unknown_08[2];
    u16 stat_index;
    BattleEnemyDataPayload payload;
} BattleEnemyDataRequest;

typedef char BattleEnemyStatRecord_SizeCheck[
    sizeof(BattleEnemyStatRecord) == 0x2C ? 1 : -1];
typedef char BattleEnemyItemDrop_SizeCheck[
    sizeof(BattleEnemyItemDrop) == 4 ? 1 : -1];
typedef char BattleEnemyDataPayload_SizeCheck[
    sizeof(BattleEnemyDataPayload) == 0x2000 ? 1 : -1];
typedef char BattleEnemyDataRequest_SizeCheck[
    sizeof(BattleEnemyDataRequest) == 0x200C ? 1 : -1];

BattleQueuedTask *BattleEnemyData_RequestLoad(BattleEnemyDataRequest *request,
                                              s32 stat_index);
BattleEnemyStatRecord *BattleEnemy_GetStats(int actor_id);

#endif
