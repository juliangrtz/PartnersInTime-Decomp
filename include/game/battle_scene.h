#ifndef PIT_GAME_BATTLE_SCENE_H
#define PIT_GAME_BATTLE_SCENE_H

#include <game/battle_object.h>
#include <nitro.h>

typedef struct BattleSceneObject BattleSceneObject;
typedef struct BattleMotionChannel BattleMotionChannel;
typedef struct BattleModel BattleModel;
typedef struct BattleModelVTable BattleModelVTable;
typedef void (*BattleMotionCallback)(BattleSceneObject *object,
                                     BattleMotionChannel *channel);

enum BattleModelFlag {
    BATTLE_MODEL_FLAG_10_SHIFT = 10,
    BATTLE_MODEL_FLAG_11_SHIFT = 11,
    BATTLE_MODEL_FLAG_10 = 1 << BATTLE_MODEL_FLAG_10_SHIFT,
    BATTLE_MODEL_FLAG_11 = 1 << BATTLE_MODEL_FLAG_11_SHIFT,
    BATTLE_MODEL_ANIMATION_MODE_MASK = 0xF000,
    BATTLE_MODEL_ANIMATION_MODE_BATTLE = 0x3000
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

struct BattleModelVTable {
    u8 unknown_000[0x88];
    int (*configure_animation_layer)(BattleModel *model, s8 layer,
                                     int animation_id, int enabled);
};

#ifdef __cplusplus
struct BattleModel {
    virtual void unknown_00();
    virtual void unknown_04();
    virtual void unknown_08();
    virtual void unknown_0c();
    virtual void unknown_10();
    virtual void unknown_14();
    virtual void unknown_18();
    virtual void unknown_1c();
    virtual void unknown_20();
    virtual void unknown_24();
    virtual void unknown_28();
    virtual void unknown_2c();
    virtual void unknown_30();
    virtual void unknown_34();
    virtual void unknown_38();
    virtual void unknown_3c();
    virtual void unknown_40();
    virtual void unknown_44();
    virtual void unknown_48();
    virtual void unknown_4c();
    virtual void unknown_50();
    virtual void unknown_54();
    virtual void unknown_58();
    virtual void unknown_5c();
    virtual void unknown_60();
    virtual void unknown_64();
    virtual void unknown_68();
    virtual void unknown_6c();
    virtual void unknown_70();
    virtual void unknown_74();
    virtual void unknown_78();
    virtual void unknown_7c();
    virtual void unknown_80();
    virtual void unknown_84();
    virtual int configure_animation_layer(s8 layer, int animation_id,
                                           int enabled);
    u8 unk_004[0x50];
    s16 animation_id;
    u8 unk_056[0x26];
    u32 flags;
};
#else
struct BattleModel {
    BattleModelVTable *vtable;
    u8 unk_004[0x50];
    s16 animation_id;
    u8 unk_056[0x26];
    u32 flags;
};
#endif

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
typedef char BattleModel_SizeCheck[sizeof(BattleModel) == 0x80 ? 1 : -1];

extern BattleSceneObject *gBattleMotionObjectList;

#ifdef __cplusplus
extern "C" {
#endif

u32 BattleMath_StartSqrt(u32 value);
u32 BattleMath_WaitForSqrtResult(void);

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
void BattleSceneObject_SetAnimation(BattleSceneObject *object,
                                    int animation_id, int argument_2);
u32 BattleSceneObject_SetBattleAnimation(BattleSceneObject *object,
                                         int animation_id, int model_flag);
u32 BattleSceneObject_SetBattleAnimationById(u32 object_id,
                                             int animation_id,
                                             int model_flag);
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
int BattleSceneObject_ConfigureAnimationLayer(int object_id, int layer);
BattleSceneObject *BattleSceneObject_GetById(u32 object_id);

#ifdef __cplusplus
}
#endif

#endif
