#ifndef PIT_GAME_BATTLE_SCENE_H
#define PIT_GAME_BATTLE_SCENE_H

#include <nitro.h>

typedef struct BattleSceneObject BattleSceneObject;
typedef struct BattleMotionChannel BattleMotionChannel;
typedef struct BattleModel BattleModel;

enum BattleModelFlag {
    BATTLE_MODEL_FLAG_10_SHIFT = 10,
    BATTLE_MODEL_FLAG_11_SHIFT = 11,
    BATTLE_MODEL_FLAG_10 = 1 << BATTLE_MODEL_FLAG_10_SHIFT,
    BATTLE_MODEL_FLAG_11 = 1 << BATTLE_MODEL_FLAG_11_SHIFT
};

typedef union BattleSceneFlags {
    u32 raw;
    struct {
        u32 state : 8;
        u32 unk_08_13 : 6;
        u32 use_alternate_model : 1;
        u32 unk_15_17 : 3;
        u32 independent_flag : 1;
        u32 unk_19_31 : 13;
    } bits;
} BattleSceneFlags;

struct BattleMotionChannel {
    u8 data[0x28];
};

struct BattleModel {
    u8 unk_000[0x7C];
    u32 flags;
};

struct BattleSceneObject {
    u8 unk_000[4];
    s16 x;
    s16 y;
    s16 z;
    s16 motion_origin_x;
    s16 motion_origin_y;
    s16 motion_origin_z;
    u8 unk_010[0x0C];
    BattleMotionChannel motion_channels[4];
    void *resource;
    BattleModel *primary_model;
    BattleModel *alternate_model;
    u8 unk_0c8[0x24];
    u16 actor_id;
    u16 linked_actor_id;
    u8 unk_0f0[4];
    BattleSceneFlags flags;
};

typedef char BattleMotionChannel_SizeCheck[
    sizeof(BattleMotionChannel) == 0x28 ? 1 : -1];
typedef char BattleSceneObject_SizeCheck[
    sizeof(BattleSceneObject) == 0xF8 ? 1 : -1];

void BattleSceneObject_SetStateFlags(BattleSceneObject *object, u8 state,
                                     int independent_flag);
void BattleSceneObject_SetModelFlag11ById(int object_id, int enabled);
void BattleSceneObject_SetModelFlag10(BattleSceneObject *object, int enabled);
void BattleSceneObject_SetModelFlag10ById(int object_id, int enabled);
BattleModel *BattleSceneObject_GetActiveModel(BattleSceneObject *object);
BattleModel *BattleSceneObject_GetActiveModelById(int object_id);
BattleMotionChannel *BattleSceneObject_GetMotionChannel(
    BattleSceneObject *object, int channel_index);
void BattleSceneObject_SnapshotPosition(BattleSceneObject *object);
BattleSceneObject *BattleSceneObject_GetById(int object_id);

#endif
