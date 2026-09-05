#ifndef PIT_GAME_BATTLE_SCENE_H
#define PIT_GAME_BATTLE_SCENE_H

#include <game/battle_object.h>
#include <nitro.h>

typedef struct BattleSceneObject BattleSceneObject;
typedef struct BattleMotionChannel BattleMotionChannel;
typedef struct BattleModel BattleModel;
typedef struct BattleModelVTable BattleModelVTable;
typedef struct BattleModelAnimationData BattleModelAnimationData;
typedef struct BattleSceneRenderOverride BattleSceneRenderOverride;
struct BattleSpriteTransform;
typedef void (*BattleMotionCallback)(BattleSceneObject *object,
                                     BattleMotionChannel *channel);

enum BattleModelFlag {
    BATTLE_MODEL_FLAG_02 = 1 << 2,
    BATTLE_MODEL_FLAG_ANIMATION_ACTIVE = 1 << 8,
    BATTLE_MODEL_FLAG_09 = 1 << 9,
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

struct BattleModelAnimationData {
    u8 unknown_000[0xC8];
    u16 start_frame;
    u16 end_frame;
};

struct BattleSceneRenderOverride {
    int (*render)(BattleSceneObject *object, int pass);
};

typedef union BattleSceneFlags {
    u32 raw;
    s8 shadow_delay;
    struct {
        u32 state : 8;
        u32 shadow_alpha : 6;
        u32 use_alternate_model : 1;
        u32 unk_15 : 1;
        u32 suppress_shadow : 1;
        u32 use_raw_position : 1;
        u32 independent_flag : 1;
        u32 unk_19_23 : 5;
        u32 render_mode : 3;
        u32 unk_27_31 : 5;
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
    void (*prepare_render)(BattleModel *model);
    void (*unknown_004)(BattleModel *model);
    void (*draw)(BattleModel *model, int argument_1,
                 int argument_2, int argument_3);
    u8 unknown_00c[0x18];
    void (*start)(BattleModel *model);
    void (*stop)(BattleModel *model);
    u8 unknown_02c[8];
    int (*set_animation)(BattleModel *model, u8 animation_id, int argument_2);
    u8 unknown_038[0x30];
    int (*set_primary_animation)(BattleModel *model, u8 animation_id,
                                 int argument_2, int enabled);
    u8 unknown_06c[0x1C];
    int (*configure_animation_layer)(BattleModel *model, s8 layer,
                                     int animation_id, int enabled);
    u8 unknown_08c[4];
    int (*test_relation)(BattleModel *model, BattleModel *other);
    u8 unknown_094[0x34];
    struct BattleSpriteTransform *(*get_sprite_transform)(BattleModel *model);
};

#ifdef __cplusplus
struct BattleModel {
    virtual void prepare_render();
    virtual void unknown_04();
    virtual void draw(int argument_1, int argument_2, int argument_3);
    virtual void unknown_0c();
    virtual void unknown_10();
    virtual void unknown_14();
    virtual void unknown_18();
    virtual void unknown_1c();
    virtual void unknown_20();
    virtual void start();
    virtual void stop();
    virtual void unknown_2c();
    virtual void unknown_30();
    virtual int set_animation(u8 animation_id, int argument_2);
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
    virtual int set_primary_animation(u8 animation_id, int argument_2,
                                      int enabled);
    virtual void unknown_6c();
    virtual void unknown_70();
    virtual void unknown_74();
    virtual void unknown_78();
    virtual void unknown_7c();
    virtual void unknown_80();
    virtual void unknown_84();
    virtual int configure_animation_layer(s8 layer, int animation_id,
                                           int enabled);
    virtual void unknown_8c();
    virtual int test_relation(BattleModel *other);
    virtual void unknown_94();
    virtual void unknown_98();
    virtual void unknown_9c();
    virtual void unknown_a0();
    virtual void unknown_a4();
    virtual void unknown_a8();
    virtual void unknown_ac();
    virtual void unknown_b0();
    virtual void unknown_b4();
    virtual void unknown_b8();
    virtual void unknown_bc();
    virtual void unknown_c0();
    virtual void unknown_c4();
    virtual struct BattleSpriteTransform *get_sprite_transform();
    u8 unk_004[4];
    BattleModel *render_next;
    BattleSceneObject *owner;
    u8 unk_010[0x38];
    BattleModelAnimationData *animation_data;
    u8 unk_04c[8];
    s16 animation_id;
    u8 unk_056[4];
    u16 enemy_idle_frame;
    s16 animation_offset_x;
    s16 animation_offset_y;
    u8 unk_060[8];
    s32 render_anchor_z;
    u8 unk_06c[8];
    s16 scale_x;
    s16 scale_y;
    u16 rotation_z;
    u16 unk_07a;
    union {
        u32 flags;
        struct {
            u32 unknown_00_01 : 2;
            u32 panel_animation_trigger : 1;
            u32 unknown_03_07 : 5;
            u32 animation_active : 1;
            u32 unknown_09 : 1;
            u32 facing_left : 1;
            u32 flip_y : 1;
            u32 unknown_12_31 : 20;
        } flag_bits;
    };
    u8 unk_080[0xC4];
    u8 render_flags;
    u8 unk_145[0x19];
    u8 transform_flags;
    u8 unk_15f[3];
    union {
        u16 animation_state;
        struct {
            u16 state : 5;
            u16 unknown_05_15 : 11;
        } animation_state_bits;
    };
    u16 owner_render_state;
    u8 unk_166[0x52];
};
#else
struct BattleModel {
    BattleModelVTable *vtable;
    u8 unk_004[4];
    BattleModel *render_next;
    BattleSceneObject *owner;
    u8 unk_010[0x38];
    BattleModelAnimationData *animation_data;
    u8 unk_04c[8];
    s16 animation_id;
    u8 unk_056[4];
    u16 enemy_idle_frame;
    s16 animation_offset_x;
    s16 animation_offset_y;
    u8 unk_060[8];
    s32 render_anchor_z;
    u8 unk_06c[8];
    s16 scale_x;
    s16 scale_y;
    u16 rotation_z;
    u16 unk_07a;
    union {
        u32 flags;
        struct {
            u32 unknown_00_01 : 2;
            u32 panel_animation_trigger : 1;
            u32 unknown_03_07 : 5;
            u32 animation_active : 1;
            u32 unknown_09 : 1;
            u32 facing_left : 1;
            u32 flip_y : 1;
            u32 unknown_12_31 : 20;
        } flag_bits;
    };
    u8 unk_080[0xC4];
    u8 render_flags;
    u8 unk_145[0x19];
    u8 transform_flags;
    u8 unk_15f[3];
    union {
        u16 animation_state;
        struct {
            u16 state : 5;
            u16 unknown_05_15 : 11;
        } animation_state_bits;
    };
    u16 owner_render_state;
    u8 unk_166[0x52];
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
    BattleSceneRenderOverride *render_override;
    u8 unk_0cc[0x14];
    s32 loaded_resource_id;
    s16 animation_id;
    s16 previous_base_animation_id;
    s16 animation_variant_offset;
    s16 effect_anchor_z;
    u16 actor_id;
    u16 linked_actor_id;
    u16 render_state;
    u8 unk_0f2[2];
    BattleSceneFlags flags;
    s16 property_0f8;
    s16 property_0fa;
    s8 property_0fc;
    s8 property_0fd;
    s8 property_0fe;
    s8 property_0ff;
    s8 property_100;
    s8 property_101;
    union {
        s8 property_102;
        u8 removal_state_102;
    };
    union {
        s8 property_103;
        u8 removal_state_103;
    };
};

typedef char BattleMotionChannel_SizeCheck[
    sizeof(BattleMotionChannel) == 0x28 ? 1 : -1];
typedef char BattleSceneObject_SizeCheck[
    sizeof(BattleSceneObject) == 0x104 ? 1 : -1];
typedef char BattleModel_SizeCheck[sizeof(BattleModel) == 0x1B8 ? 1 : -1];

extern BattleSceneObject *gBattleMotionObjectList;
extern BattleModel *gModelRenderList;

#ifdef __cplusplus
extern "C" {
#endif

u32 BattleMath_StartSqrt(u32 value);
u32 BattleMath_WaitForSqrtResult(void);

void BattleSceneObject_SetStateFlags(BattleSceneObject *object, int state,
                                     int independent_flag);
int BattleScene_RenderModels(void);
void BattleScene_RenderShadows(void);
void BattleScene_UpdateAlternateModelAnchor(BattleModel *model);
int BattleScene_TestModelRelation(BattleSceneObject *first,
                                  BattleSceneObject *second,
                                  int first_model, int second_model);
void BattleScene_DrawShadow(int variant, int animation_state,
                            int x, int y, int height, int alpha);
void BattleScene_HideQueuedObjects(void);
int BattleScene_UpdateShadowVisibility(BattleSceneObject *object);
void BattleSceneObject_SwapSlots(u32 first_id, u32 second_id);
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
void BattleSceneObject_ApplyMovement(u32 object_id, int channel_index,
                                     int movement_kind, int x, int y, int z,
                                     int duration, int target_object_id,
                                     int argument_8, int argument_9);
void BattleSceneObject_SetAnimation(BattleSceneObject *object,
                                    int animation_id, int argument_2);
u32 BattleSceneObject_SetAnimationFromComponent(BattleSceneObject *object,
                                                int animation_id,
                                                int argument_2,
                                                int component_index);
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
int BattleModel_SetAlpha(BattleModel *model, u8 alpha, u8 mode);

#ifdef __cplusplus
}
#endif

#endif
