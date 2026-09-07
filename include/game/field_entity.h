#ifndef PARTNERS_IN_TIME_GAME_FIELD_ENTITY_H
#define PARTNERS_IN_TIME_GAME_FIELD_ENTITY_H

#include <nitro.h>
#include <nitro/fx.h>
#include <game/sprite_output.h>

enum FieldEntityStateFlag {
    FIELD_ENTITY_STATE_ACTIVE = 1 << 1,
    FIELD_ENTITY_STATE_FLAG_02 = 1 << 2
};

enum FieldRuntimeEntityFlag {
    FIELD_ENTITY_RETAIN_OFFSCREEN_CONTACT = 1 << 8,
    FIELD_ENTITY_RESERVED_STATE = 1 << 12,
    FIELD_ENTITY_CONTACT_MODE_MASK = 7,
    FIELD_ENTITY_TURN_TO_INTERACTOR = 1 << 11,
    FIELD_ENTITY_TRACK_GROUND = 1 << 12,
    FIELD_ENTITY_IGNORE_NAVIGATION_OBSTACLE = 1 << 13,
    FIELD_ENTITY_BODY_COLLISION_ENABLED = 1 << 14,
    FIELD_ENTITY_SHADOW_ENABLED = 1 << 15,
    FIELD_ENTITY_SHADOW_SUPPORT_ENABLED = 1 << 17,
    FIELD_ENTITY_SHADOW_STYLE_SHIFT = 18,
    FIELD_ENTITY_SHADOW_STYLE_MASK = 7 << FIELD_ENTITY_SHADOW_STYLE_SHIFT,
    FIELD_ENTITY_SAVED_RESOURCE = 1 << 16,
    FIELD_ENTITY_SAVED_PALETTE_PROFILE = 1 << 17,
    FIELD_ENTITY_SAVED_RESOURCE_ANIMATION = 1 << 18,
    FIELD_ENTITY_SAVED_ANIMATION = 1 << 19,
    FIELD_ENTITY_SAVED_BEHAVIOR = 1 << 13,
    FIELD_ENTITY_SYNC_HORIZONTAL = 1 << 0,
    FIELD_ENTITY_SYNC_VERTICAL = 1 << 1,
    FIELD_ENTITY_ALTERNATE_COLLISION_FACES = 1 << 3,
    FIELD_ENTITY_HORIZONTAL_SYNC_DIRTY = 1 << 4,
    FIELD_ENTITY_VERTICAL_SYNC_DIRTY = 1 << 5,
    FIELD_ENTITY_AUTO_AUXILIARY_PRIORITY = 1 << 25,
    FIELD_ENTITY_AUTO_PRIORITY_0 = 1 << 26,
    FIELD_ENTITY_AUTO_PRIORITY_1 = 1 << 27,
    FIELD_ENTITY_AUTO_PRIORITY_2 = 1 << 28,
    FIELD_ENTITY_AUTO_PRIORITY_3 = 1 << 29
};

enum FieldRenderObjectFlag {
    FIELD_RENDER_SEMITRANSPARENT = 1 << 3,
    FIELD_RENDER_ANIMATION_ACTIVE = 1 << 8,
    FIELD_RENDER_ANIMATION_SUPPRESSED = 1 << 9
};

typedef struct FieldEntity FieldEntity;
typedef struct FieldEntityVTable FieldEntityVTable;
typedef struct FieldRenderObject FieldRenderObject;
typedef struct FieldRenderObjectVTable FieldRenderObjectVTable;
typedef struct FieldRuntimeEntity FieldRuntimeEntity;
typedef void (*FieldEntityVisibilityCallback)(FieldEntity *entity);

typedef struct FieldContactDirectionFlags {
    u32 enabled_mask : 6;
    u32 reserved : 26;
} FieldContactDirectionFlags;

typedef struct FieldBaseStateFlags {
    /* Enables waiting for the entity renderer's current animation. */
    u32 animation_wait_enabled : 1;
    u32 unknown_01 : 1;
    u32 facing_direction : 3;
    u32 previous_facing_direction : 3;
    u32 retain_offscreen_contact : 1;
    u32 unknown_09_11 : 3;
    u32 reserved_state : 1;
    u32 blink_mode : 2;
    u32 blink_paused : 1;
    u32 blink_phase : 1;
    s32 blink_frames : 8;
    u32 blink_offset : 7;
} FieldBaseStateFlags;

typedef struct FieldPlanarMovementFlags {
    u32 active_state : 3;
    u32 unknown_03_31 : 29;
} FieldPlanarMovementFlags;

typedef struct FieldVerticalControllerFlags {
    u32 active : 1;
    u32 unknown_01_31 : 31;
} FieldVerticalControllerFlags;

typedef struct FieldTransformFlags {
    u16 scaling_active : 2;
    u16 rotation_active : 1;
    u16 unknown_03_15 : 13;
} FieldTransformFlags;

typedef struct FieldRenderSortKey {
    u32 order : 28;
    u32 layer : 4;
} FieldRenderSortKey;

typedef struct FieldEntityRuntimeFlags {
    u32 sync_horizontal : 1;
    u32 sync_vertical : 1;
    u32 unknown_02 : 1;
    u32 alternate_collision_faces : 1;
    u32 horizontal_sync_dirty : 1;
    u32 vertical_sync_dirty : 1;
    u32 unknown_06_24 : 19;
    u32 auto_auxiliary_priority : 1;
    u32 auto_priority_0 : 1;
    u32 auto_priority_1 : 1;
    u32 auto_priority_2 : 1;
    u32 auto_priority_3 : 1;
    u32 unknown_30_31 : 2;
} FieldEntityRuntimeFlags;

typedef struct FieldEntityFieldStateFlags {
    u32 contact_mode : 3;
    u32 unknown_03 : 1;
    u32 vertical_motion_active : 1;
    u32 unknown_05_10 : 6;
    u32 turn_to_interactor : 1;
    u32 track_ground : 1;
    u32 ignore_navigation_obstacle : 1;
    u32 body_collision_enabled : 1;
    u32 shadow_enabled : 1;
    u32 unknown_16 : 1;
    u32 shadow_support_enabled : 1;
    u32 shadow_style : 3;
    u32 unknown_21_31 : 11;
} FieldEntityFieldStateFlags;

typedef struct FieldCollisionStateFlags {
    u32 unknown_00_15 : 16;
    u32 reserved_0 : 1;
    u32 reserved_1 : 1;
    u32 unknown_18_31 : 14;
} FieldCollisionStateFlags;

typedef struct FieldRenderStateFlags {
    u32 unknown_00_02 : 3;
    u32 semitransparent : 1;
    u32 render_linked : 1;
    u32 unknown_05_07 : 3;
    u32 animation_active : 1;
    u32 animation_suppressed : 1;
    u32 unknown_10_11 : 2;
    u32 behavior_state : 4;
    u32 unknown_16_31 : 16;
} FieldRenderStateFlags;

typedef struct FieldSavedPresentationFlags {
    u32 unknown_00_06 : 7;
    u32 behavior_mode : 3;
    u32 saved_behavior_mode : 3;
    u32 has_saved_behavior : 1;
    u32 unknown_14_15 : 2;
    u32 has_saved_resource : 1;
    u32 has_saved_palette_profile : 1;
    u32 has_saved_resource_animation : 1;
    u32 has_saved_animation : 1;
    u32 unknown_20_29 : 10;
    u32 skip_auxiliary_renderer : 1;
    u32 unknown_31 : 1;
} FieldSavedPresentationFlags;

typedef struct FieldRoamingFlags {
    u32 clamp_to_boundary : 1;
    u32 unknown_01_31 : 31;
} FieldRoamingFlags;

typedef struct FieldInteractionFlags {
    u32 jump_first_strike_enabled : 1;
    u32 spiked_jump_response : 1;
    u32 remove_after_special_contact : 1;
    u32 remove_immediately_for_battle : 1;
    u32 unknown_04_13 : 10;
    s32 block_bounce_state : 8;
    u32 unknown_22_31 : 10;
} FieldInteractionFlags;

struct FieldRenderObjectVTable {
    u8 unknown_00[0x68];
    void (*set_animation)(FieldRenderObject *render_object,
                          u8 resource_animation, s16 animation_id,
                          int restart);
    u8 unknown_6c[0x10];
    void (*set_palette_animation_paused)(
        FieldRenderObject *render_object, s8 slot, int paused);
    void (*set_palette_animation_mode)(
        FieldRenderObject *render_object, s8 slot, u8 mode);
    u8 unknown_84[4];
    void (*bind_palette_animation)(FieldRenderObject *render_object,
                                   s8 slot, s16 animation_id,
                                   int restart);
    void (*request_base_palette_reload)(FieldRenderObject *render_object);
};

struct FieldEntityVTable {
    u8 unknown_00[0x0C];
    void (*stop_script)(FieldEntity *entity);
    void (*pause_script)(FieldEntity *entity);
    void (*resume_script)(FieldEntity *entity);
    int (*get_property)(FieldEntity *entity, int property_id);
    u8 unknown_1c[0x10];
    void (*release_renderers)(FieldEntity *entity);
    u8 unknown_30[0x20];
    void (*cancel_planar_movement)(FieldEntity *entity, void *controller,
                                   int snap_to_destination);
    u8 unknown_54[8];
    void (*cancel_vertical_movement)(FieldEntity *entity, void *controller,
                                     int snap_to_destination);
    void (*set_visible)(FieldEntity *entity, int visible);
    u8 unknown_64[0x10];
    void (*start_blink)(FieldEntity *entity, int mode, const s8 *durations, u8 length,
                        FieldEntityVisibilityCallback show, FieldEntityVisibilityCallback hide);
    u8 unknown_78[0x14];
    void (*set_collision_response_channels)(
        FieldEntity *entity, int channel_0, int channel_1, int channel_2,
        int channel_3, int channel_4);
    void (*set_collision_response_channels_masked)(
        FieldEntity *entity, u16 channel_mask, int enabled);
    void (*restore_collision_response_channels)(FieldEntity *entity);
};

typedef struct FieldEntity {
#ifdef __cplusplus
    virtual void unknown_00();
    virtual void unknown_04();
    virtual void unknown_08();
    virtual void stop_script();
    virtual void pause_script();
    virtual void resume_script();
    virtual int get_property(int property_id);
    virtual void unknown_1c();
    virtual void unknown_20();
    virtual void unknown_24();
    virtual void unknown_28();
    virtual void release_renderers();
    virtual void unknown_30();
    virtual void unknown_34();
    virtual void unknown_38();
    virtual void unknown_3c();
    virtual void unknown_40();
    virtual void unknown_44();
    virtual void unknown_48();
    virtual void unknown_4c();
    virtual void cancel_planar_movement(void *controller, int snap_to_destination);
    virtual void unknown_54();
    virtual void unknown_58();
    virtual void cancel_vertical_movement(void *controller, int snap_to_destination);
    virtual void set_visible(int visible);
    virtual void unknown_64();
    virtual void unknown_68();
    virtual void unknown_6c();
    virtual void unknown_70();
    virtual void start_blink(int mode, const s8 *durations, u8 length,
                             FieldEntityVisibilityCallback show, FieldEntityVisibilityCallback hide);
    virtual void unknown_78();
    virtual void unknown_7c();
    virtual void unknown_80();
    virtual void unknown_84();
    virtual void unknown_88();
    virtual void set_collision_response_channels(
        int channel_0, int channel_1, int channel_2, int channel_3, int channel_4);
    virtual void set_collision_response_channels_masked(u16 channel_mask, int enabled);
    virtual void restore_collision_response_channels();
#else
    FieldEntityVTable *vtable;
#endif
    u8 unknown_004[6];
    union {
        u16 property_00a;
        struct {
            u16 property_00a_flag_00 : 1;
            u16 property_00a_unknown_01 : 1;
            u16 subtype : 4;
            u16 resource_set : 1;
            u16 property_00a_unknown_07_08 : 2;
            u16 heap : 3;
            u16 property_00a_unknown_12_15 : 4;
        } property_00a_bits;
    };
    u8 unknown_00c[0xC4];
    union {
        u32 state_flags;
        struct {
            u32 unknown_flag_00 : 1;
            u32 active : 1;
            u32 flag_02 : 1;
            u32 unknown_flags_03_14 : 12;
            u32 interaction_state : 2;
            u32 unknown_flags_17_31 : 15;
        } state_flag_bits;
    };
    u8 unknown_0d4[0xC];
    u32 action_timer;
    u8 unknown_0e4[8];
} FieldEntity;

/*
 * Shared portion of the sprite/model object bound to a field entity. Keeping
 * the command-facing fields typed makes the VM reconstruction readable while
 * the renderer itself remains in assembly.
 */
typedef struct FieldRenderAnimationRange {
    u16 first, end;
    u8 unknown_04[4];
} FieldRenderAnimationRange;
typedef char FieldRenderAnimationRange_SizeCheck[sizeof(FieldRenderAnimationRange) == 8 ? 1 : -1];

typedef struct FieldInteractionBounds {
    s8 minimum_x, maximum_y, width, height, vertical_extent;
} FieldInteractionBounds;
typedef struct FieldAnimationBoundsIndex {
    u8 unknown_00[2], bounds_index, unknown_03[3];
} FieldAnimationBoundsIndex;
typedef char FieldInteractionBounds_SizeCheck[sizeof(FieldInteractionBounds) == 5 ? 1 : -1];
typedef char FieldAnimationBoundsIndex_SizeCheck[sizeof(FieldAnimationBoundsIndex) == 6 ? 1 : -1];

typedef struct FieldLocomotionParameters {
    fx32 starting_speed, acceleration, maximum_speed;
    fx32 deceleration, reverse_deceleration, turn_speed_limit;
} FieldLocomotionParameters;
typedef char FieldLocomotionParameters_SizeCheck[sizeof(FieldLocomotionParameters) == 24 ? 1 : -1];

struct FieldRenderObject {
#ifdef __cplusplus
    virtual void unknown_00();
    virtual void unknown_04();
    virtual void unknown_08();
    virtual void unknown_0c();
    virtual void unknown_10();
    virtual void delete_self();
    virtual void unknown_18();
    virtual void unknown_1c();
    virtual void unknown_20();
    virtual void unknown_24();
    virtual void stop();
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
    virtual void set_animation(u8 resource_animation, s16 animation_id, int restart);
    virtual void unknown_6c();
    virtual void unknown_70();
    virtual void unknown_74();
    virtual void unknown_78();
    virtual void set_palette_animation_paused(s8 slot, int paused);
    virtual void set_palette_animation_mode(s8 slot, u8 mode);
    virtual void unknown_84();
    virtual void bind_palette_animation(s8 slot, s16 animation_id, int restart);
    virtual void request_base_palette_reload();
#else
    FieldRenderObjectVTable *vtable;
#endif
    FieldRenderObject *render_previous, *render_next;
    void *owner;
    u8 screen, unknown_011[3];
    GameSpriteAllocation texture;
    u8 unknown_02c[0x1C];
    const FieldRenderAnimationRange *animation_ranges;
    const void *extra_resource_data;
    const u16 *texture_offsets;
    s16 resource_animation;
    s16 animation_id;
    u8 unknown_058[2];
    s16 animation_speed;
    s16 animation_offset_x, animation_offset_y;
    u8 unknown_060[8];
    s32 render_anchor_z;
    u8 unknown_06c[0x10];
    union {
        u32 state_flags;
        FieldRenderStateFlags state_flag_bits;
    };
    u8 unknown_080[0x38];
    u8 transition_active;
    u8 unknown_0b9[3];
    void *transition;
    u8 unknown_0c0[0x40];
    void *components[8];
    u8 unknown_120[0x10];
    union {
        u32 sort_key;
        FieldRenderSortKey sort_key_bits;
    };
    union {
        s8 overlap_priorities[4];
        u8 overlap_priority_bytes[4];
    };
};

/*
 * Runtime extension shared by the scriptable field-entity subclasses. The
 * small FieldEntity base above is still used by its byte-matching constructor.
 */
struct FieldRuntimeEntity {
    FieldEntity base;
    u8 unknown_0ec[8];
    fx32 position_x;
    fx32 position_y;
    u8 unknown_0fc[8];
    const void *bounds_resource;
    const FieldInteractionBounds *interaction_bounds;
    const FieldAnimationBoundsIndex *animation_bounds;
    u8 bounds_animation_count;
    s8 bounds_index;
    u8 unknown_112[2];
    fx32 interaction_min_x, interaction_max_x, interaction_min_y, interaction_max_y;
    fx32 interaction_vertical_extent;
    fx32 movement_speed, movement_velocity_x, movement_velocity_y, unknown_134;
    FieldLocomotionParameters locomotion, initial_locomotion;
    u8 unknown_168[0x10];
    u16 unknown_178;
    u8 unknown_17a[0xA];
    union {
        u32 base_state_flags;
        FieldBaseStateFlags base_state_flag_bits;
    };
    const s8 *blink_durations;
    FieldEntityVisibilityCallback blink_first_phase, blink_second_phase;
    union {
        u32 saved_presentation_flags;
        FieldSavedPresentationFlags saved_presentation_flag_bits;
    };
    s16 animation_speed;
    u8 resource_index;
    u8 saved_resource_index;
    u8 presentation_state;
    u8 saved_presentation_state;
    u16 animation_id;
    u16 saved_animation_id;
    u16 saved_model_animation;
    s16 saved_animation_frame;
    u8 unknown_1a6[0x3A];
    FieldRenderObject *render_object;
    u8 unknown_1e4[0x10];
    union {
        u32 planar_movement_flags;
        FieldPlanarMovementFlags planar_movement_flag_bits;
    };
    u8 unknown_1f8[0x40];
    union {
        u32 vertical_controller_flags;
        FieldVerticalControllerFlags vertical_controller_flag_bits;
    };
    u8 unknown_23c[0x44];
    union {
        u16 transform_flags;
        FieldTransformFlags transform_flag_bits;
    };
    u8 unknown_282[0x3A];
    fx32 position_z;
    s32 unknown_2c0;
    u8 unknown_2c4[0x24];
    s32 body_corner_angles[4];
    fx32 body_min_x;
    fx32 body_max_x;
    fx32 body_min_y;
    fx32 body_max_y;
    fx32 body_vertical_extent;
    u8 unknown_30c[0x10];
    fx32 body_center_y;
    fx32 navigation_min_x;
    fx32 navigation_max_x;
    fx32 navigation_min_y;
    fx32 navigation_max_y;
    fx32 navigation_vertical_extent;
    u8 unknown_334[0x20];
    fx32 unknown_354;
    u8 unknown_358[0xC];
    fx32 default_vertical_launch_velocity;
    fx32 default_gravity;
    fx32 terminal_fall_velocity;
    u32 unknown_370;
    fx32 initial_vertical_launch_velocity, initial_gravity, initial_terminal_fall_velocity;
    u8 unknown_380[0xC];
    union {
        u32 field_state_flags;
        FieldEntityFieldStateFlags field_state_flag_bits;
    };
    u32 collision_category_mask;
    s64 collision_policy;
    union {
        u32 collision_state_flags;
        FieldCollisionStateFlags collision_state_flag_bits;
    };
    union {
        u32 unknown_3a0;
        struct { u32 unknown_00_19 : 20, unknown_20_25 : 6, unknown_26_31 : 6; } unknown_3a0_bits;
    };
    FieldContactDirectionFlags contact_direction_flags;
    u8 unknown_3a8[0x24];
    union {
        u32 runtime_flags;
        FieldEntityRuntimeFlags runtime_flag_bits;
    };
    u8 unknown_3d0[4];
    s16 unknown_3d4;
    u8 unknown_3d6[0xA];
    union {
        u32 roaming_flags;
        FieldRoamingFlags roaming_flag_bits;
    };
    u8 unknown_3e4[0x12C];
    FieldRenderObject *auxiliary_render_object;
};

typedef char FieldEntity_SizeCheck[sizeof(FieldEntity) == 0xEC ? 1 : -1];
typedef char FieldRenderObject_SizeCheck[
    sizeof(FieldRenderObject) == 0x138 ? 1 : -1];
typedef char FieldRuntimeEntity_SizeCheck[
    sizeof(FieldRuntimeEntity) == 0x514 ? 1 : -1];

#endif
