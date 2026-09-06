#ifndef PARTNERS_IN_TIME_GAME_FIELD_ENTITY_H
#define PARTNERS_IN_TIME_GAME_FIELD_ENTITY_H

#include <nitro.h>
#include <nitro/fx.h>

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

typedef struct FieldContactDirectionFlags {
    u32 enabled_mask : 6;
    u32 reserved : 26;
} FieldContactDirectionFlags;

typedef struct FieldBaseStateFlags {
    u32 unknown_00_07 : 8;
    u32 retain_offscreen_contact : 1;
    u32 unknown_09_11 : 3;
    u32 reserved_state : 1;
    u32 unknown_13_31 : 19;
} FieldBaseStateFlags;

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
    u32 unknown_03_10 : 8;
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
    u32 unknown_04_07 : 4;
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
    u32 unknown_20_31 : 12;
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
    u32 unknown_04_31 : 28;
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
        FieldRenderObject *render_object, s8 slot, int mode);
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
    u8 unknown_1c[0x34];
    void (*cancel_planar_movement)(FieldEntity *entity, void *controller,
                                   int snap_to_destination);
    u8 unknown_54[8];
    void (*cancel_vertical_movement)(FieldEntity *entity, void *controller,
                                     int snap_to_destination);
    void (*set_visible)(FieldEntity *entity, int visible);
    u8 unknown_64[0x28];
    void (*set_collision_response_channels)(
        FieldEntity *entity, int channel_0, int channel_1, int channel_2,
        int channel_3, int channel_4);
    void (*set_collision_response_channels_masked)(
        FieldEntity *entity, u16 channel_mask, int enabled);
    void (*restore_collision_response_channels)(FieldEntity *entity);
};

typedef struct FieldEntity {
    FieldEntityVTable *vtable;
    u8 unknown_004[6];
    union {
        u16 property_00a;
        struct {
            u16 property_00a_flag_00 : 1;
            u16 property_00a_unknown_01 : 1;
            u16 subtype : 4;
            u16 resource_set : 1;
            u16 property_00a_unknown_07_15 : 9;
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
    u8 unknown_0d4[6];
    s16 script_values[2];
    u8 unknown_0de[2];
    u32 action_timer;
    u8 unknown_0e4[8];
} FieldEntity;

/*
 * Shared portion of the sprite/model object bound to a field entity. Keeping
 * the command-facing fields typed makes the VM reconstruction readable while
 * the renderer itself remains in assembly.
 */
struct FieldRenderObject {
    FieldRenderObjectVTable *vtable;
    u8 unknown_004[0x52];
    u16 animation_id;
    u8 unknown_058[0x24];
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
    u32 sort_key;
    s8 overlap_priorities[4];
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
    u8 unknown_0fc[0x28];
    fx32 interaction_vertical_extent;
    u8 unknown_128[0x5C];
    union {
        u32 base_state_flags;
        FieldBaseStateFlags base_state_flag_bits;
    };
    u8 unknown_188[0x0C];
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
    u8 unknown_1a4[0x3C];
    FieldRenderObject *render_object;
    u8 unknown_1e4[0x10];
    u32 planar_movement_flags;
    u8 unknown_1f8[0x40];
    u32 vertical_controller_flags;
    u8 unknown_23c[0x44];
    u16 transform_flags;
    u8 unknown_282[0x3A];
    fx32 position_z;
    u8 unknown_2c0[0x28];
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
    u8 unknown_334[0x30];
    fx32 default_vertical_launch_velocity;
    fx32 default_gravity;
    fx32 terminal_fall_velocity;
    u8 unknown_370[0x1C];
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
    u32 unknown_3a0;
    FieldContactDirectionFlags contact_direction_flags;
    u8 unknown_3a8[0x24];
    union {
        u32 runtime_flags;
        FieldEntityRuntimeFlags runtime_flag_bits;
    };
    u8 unknown_3d0[0x10];
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
