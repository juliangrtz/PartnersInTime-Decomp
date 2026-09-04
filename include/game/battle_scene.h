#ifndef PIT_GAME_BATTLE_SCENE_H
#define PIT_GAME_BATTLE_SCENE_H

#include <nitro.h>

typedef struct BattleSceneObject BattleSceneObject;
typedef struct BattleMotionChannel BattleMotionChannel;
typedef struct BattleModel BattleModel;
typedef struct BattleSceneResource BattleSceneResource;
typedef void (*BattleMotionCallback)(BattleSceneObject *object,
                                     BattleMotionChannel *channel);

enum BattleModelFlag {
    BATTLE_MODEL_FLAG_10_SHIFT = 10,
    BATTLE_MODEL_FLAG_11_SHIFT = 11,
    BATTLE_MODEL_FLAG_10 = 1 << BATTLE_MODEL_FLAG_10_SHIFT,
    BATTLE_MODEL_FLAG_11 = 1 << BATTLE_MODEL_FLAG_11_SHIFT
};

enum {
    BATTLE_MOTION_CHANNEL_COUNT = 4
};

typedef union BattleSceneFlags {
    u32 raw;
    struct {
        u32 state : 8;
        u32 unk_08_13 : 6;
        u32 use_alternate_model : 1;
        u32 unk_15_16 : 2;
        u32 use_raw_position : 1;
        u32 independent_flag : 1;
        u32 unk_19_31 : 13;
    } bits;
} BattleSceneFlags;

struct BattleMotionChannel {
    BattleMotionCallback callback;
    s32 elapsed_q8;
    s16 duration;
    u16 has_deferred_delta;
    s16 deferred_delta_x;
    s16 deferred_delta_y;
    s16 deferred_delta_z;
    s16 frame_delta_x;
    s16 frame_delta_y;
    s16 frame_delta_z;
    s16 parameters[8];
};

struct BattleModel {
    u8 unk_000[0x54];
    s16 animation_id;
    u8 unk_056[0x26];
    u32 flags;
};

struct BattleSceneResource {
    u8 unknown_000[0x20];
    u16 object_data_id;
    u8 unknown_022[2];
    s32 resource_id;
    u32 allocation_size;
};

struct BattleSceneObject {
    BattleSceneObject *motion_next;
    s16 x;
    s16 y;
    s16 z;
    s16 motion_origin_x;
    s16 motion_origin_y;
    s16 motion_origin_z;
    s16 motion_target_x;
    s16 motion_target_y;
    s16 motion_target_z;
    s16 unk_016;
    s16 smoothed_travel_distance;
    u16 unk_01a;
    BattleMotionChannel motion_channels[BATTLE_MOTION_CHANNEL_COUNT];
    BattleSceneResource *resource;
    BattleModel *primary_model;
    BattleModel *alternate_model;
    u8 unk_0c8[0x18];
    u32 transition_flags;
    u8 unk_0e4[6];
    s16 effect_anchor_z;
    u16 actor_id;
    u16 linked_actor_id;
    u8 unk_0f0[4];
    BattleSceneFlags flags;
};

typedef char BattleMotionChannel_SizeCheck[
    sizeof(BattleMotionChannel) == 0x28 ? 1 : -1];
typedef char BattleSceneObject_SizeCheck[
    sizeof(BattleSceneObject) == 0xF8 ? 1 : -1];

extern BattleSceneObject *gBattleMotionObjectList;

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
void BattleSceneObject_MoveByImmediate(BattleSceneObject *object,
                                       int delta_x, int delta_y, int delta_z);
void BattleSceneObject_AddPositionDelta(BattleSceneObject *object,
                                        int delta_x, int delta_y, int delta_z);
void BattleSceneObject_AdjustPosition(BattleSceneObject *object,
                                      int delta_x, int delta_y, int delta_z);
void BattleSceneObject_MoveTo(BattleSceneObject *object, int channel_index,
                              int target_x, int target_y, int target_z,
                              int duration);
void BattleSceneObject_UpdateMoveTo(BattleSceneObject *object,
                                    BattleMotionChannel *channel);
void BattleSceneObject_MoveBy(BattleSceneObject *object, int channel_index,
                              int delta_x, int delta_y, int delta_z,
                              int duration);
void BattleSceneObject_UpdateMoveBy(BattleSceneObject *object,
                                    BattleMotionChannel *channel);
void BattleSceneObject_UpdateTravelDistance(BattleSceneObject *object);
u32 BattleSceneObject_SetBattleAnimation(BattleSceneObject *object,
                                         int animation_id, int model_flag);
int BattleSceneObject_StartAcceleratedMotionForDuration(
    BattleSceneObject *object, int channel_index,
    int direction_x, int direction_y, int direction_z,
    int initial_speed, int duration, int target_speed,
    int positive_acceleration
);
s16 *BattleSceneObject_BeginMotionChannel(BattleSceneObject *object,
                                          int channel_index, int duration,
                                          BattleMotionCallback callback);
void BattleSceneObject_UnlinkMotion(BattleSceneObject *object);
void BattleSceneObject_StopMotionChannel(BattleSceneObject *object,
                                         int channel_index,
                                         int apply_deferred_delta);
int BattleSceneObject_IsAnimationChannelActive(BattleSceneObject *object,
                                               int channel_index);
int BattleSceneObject_IsAnimationActiveById(u32 object_id,
                                            int channel_index);
BattleSceneObject *BattleSceneObject_GetById(u32 object_id);

#endif
