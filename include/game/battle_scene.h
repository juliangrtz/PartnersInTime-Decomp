#ifndef PIT_GAME_BATTLE_SCENE_H
#define PIT_GAME_BATTLE_SCENE_H

#include <nitro.h>

typedef struct BattleSceneObject BattleSceneObject;
typedef struct BattleMotionChannel BattleMotionChannel;

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
    void *primary_model;
    void *alternate_model;
    u8 unk_0c8[0x24];
    u16 actor_id;
    u8 unk_0ee[6];
    BattleSceneFlags flags;
};

typedef char BattleMotionChannel_SizeCheck[
    sizeof(BattleMotionChannel) == 0x28 ? 1 : -1];

void BattleSceneObject_SetStateFlags(BattleSceneObject *object, u8 state,
                                     int independent_flag);
void *BattleSceneObject_GetActiveModel(BattleSceneObject *object);
BattleMotionChannel *BattleSceneObject_GetMotionChannel(
    BattleSceneObject *object, int channel_index);
void BattleSceneObject_SnapshotPosition(BattleSceneObject *object);

#endif
