#ifndef PIT_GAME_BATTLE_ENEMY_DATA_H
#define PIT_GAME_BATTLE_ENEMY_DATA_H

#include <game/battle_task_queue.h>

typedef struct BattleEnemyStatRecord {
    u16 name_id;
    u16 packed_object_data_id;
    u8 unknown_04;
    u8 level;
    u16 max_hp;
    u16 power;
    u16 defense;
    u16 speed;
    union {
        u16 traits;
        struct {
            u16 trait_class : 2;
            u16 unknown_traits_02_15 : 14;
        } trait_bits;
    };
    u16 unknown_10;
    u8 unknown_12[14];
    u16 experience;
    u16 coins;
    u32 item_drop_1;
    u32 item_drop_2;
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
typedef char BattleEnemyDataPayload_SizeCheck[
    sizeof(BattleEnemyDataPayload) == 0x2000 ? 1 : -1];
typedef char BattleEnemyDataRequest_SizeCheck[
    sizeof(BattleEnemyDataRequest) == 0x200C ? 1 : -1];

BattleQueuedTask *BattleEnemyData_RequestLoad(BattleEnemyDataRequest *request,
                                              s32 stat_index);
BattleEnemyStatRecord *BattleEnemy_GetStats(int actor_id);

#endif
