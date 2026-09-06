#include <game/field_entity.h>
#include <game/field_script.h>
#include <game/save_data.h>

/*
 * Structured reconstruction of the field-specific VM command range. The
 * original is a single 23,492-byte switch; this complete semantic work unit is
 * intentionally unlinked until its Metrowerks instruction layout matches.
 */

extern FieldEntity *func_ov000_0208221c(
    FieldVmRuntime *runtime, FieldScriptState *state, int entity_selector);
extern int func_ov000_02082240(FieldScriptState *target,
                               FieldScriptState *parent, int owner_type,
                               const u16 *script);
extern void func_ov000_0208911c(void *paired_script_manager,
                                FieldScriptState *parent,
                                s16 script_slot);
extern void func_ov000_020a4f4c(FieldEntity *entity, s16 minimum_x,
                                s16 maximum_y, u16 width, u16 height,
                                u16 vertical_extent);
extern void func_ov000_020a4e84(FieldEntity *entity);
extern void func_ov000_020a4df8(FieldEntity *entity, int behavior_mode,
                                int transition_mode);
extern void func_ov000_020a4468(FieldEntity *entity, int scale_mode,
                                s16 target_x, s16 target_y, s16 step_x,
                                s16 step_y, int reserved);
extern void func_ov000_020a439c(FieldEntity *entity, int scale_mode,
                                s16 target_x, s16 target_y, int duration_x,
                                int duration_y, int reserved);
extern void func_ov000_020a4360(FieldEntity *entity, int snap_to_target,
                                int reserved);
extern void func_ov000_020a4214(FieldEntity *entity, int angle_mode,
                                int target_angle, int angular_step,
                                int signed_multiplier, int stop_at_target,
                                int reserved);
extern void func_ov000_020a412c(FieldEntity *entity, int angle_mode,
                                int target_angle, int duration,
                                int signed_multiplier, int stop_at_target,
                                int reserved);
extern void func_ov000_020a40f8(FieldEntity *entity, int snap_to_target,
                                int reserved);
extern void func_ov000_020a7410(FieldEntity *entity, int starting_speed,
                                int acceleration, int maximum_speed,
                                int idle_deceleration,
                                int reverse_deceleration,
                                int turn_speed_limit);
extern void func_ov000_020ae520(FieldEntity *entity);
extern void func_ov000_020b1394(FieldEntity *entity);
extern void func_ov000_020a6c7c(FieldEntity *entity, int animation_id,
                                int reserved);
extern void func_ov000_020a6d68(FieldEntity *entity,
                                const void *resource_record);
extern void func_ov000_020bd86c(FieldRenderObject *render_object);
extern void func_ov000_020a64a0(FieldEntity *entity, int coordinate_mode,
                                fx32 x, fx32 y, int motion_5, int motion_6,
                                int motion_7, int motion_8, int motion_11,
                                int reserved);
extern void func_ov000_020b3f00(FieldEntity *entity, int coordinate_mode,
                                fx32 x, fx32 y, fx32 z, int motion_5,
                                int motion_6, int motion_7, int motion_8,
                                int motion_9, int motion_10, int motion_11,
                                int reserved);
extern void func_ov000_020a6394(FieldEntity *entity, int coordinate_mode,
                                fx32 x, fx32 y, int duration,
                                int motion_flag, int reserved);
extern void func_ov000_020b3cb0(FieldEntity *entity, int coordinate_mode,
                                fx32 x, fx32 y, fx32 z, int duration,
                                int motion_6, int motion_7, int motion_flag,
                                int reserved);
extern void func_ov000_020a6260(FieldEntity *entity, FieldEntity *target,
                                fx32 x, fx32 y, int motion_5,
                                int motion_flag, int reserved);
extern void func_ov000_020b3b00(FieldEntity *entity, FieldEntity *target,
                                fx32 x, fx32 y, fx32 z, int motion_5,
                                int motion_6, int motion_7, int motion_flag,
                                int reserved);
extern void func_ov000_020a61b8(FieldEntity *entity, FieldEntity *target,
                                fx32 x, fx32 y, int duration,
                                int motion_flag, int reserved);
extern void func_ov000_020b39a8(FieldEntity *entity, FieldEntity *target,
                                fx32 x, fx32 y, fx32 z, int duration,
                                int motion_6, int motion_7, int motion_flag,
                                int reserved);
extern void func_ov000_020a59c4(
    FieldEntity *entity, int relative_center, fx32 center_x, fx32 center_y,
    int relative_angle, int angle, int initial_speed, int acceleration,
    int maximum_speed, int deceleration, int direction,
    int secondary_axis_scale, int snap_to_final_angle, int reserved);
extern void func_ov000_020b2e9c(
    FieldEntity *entity, int relative_center, fx32 center_x, fx32 center_y,
    fx32 center_z, int relative_angle, int angle, int initial_speed,
    int acceleration, int maximum_speed, int deceleration, int plane,
    int direction, int secondary_axis_scale, int stop_on_contact_mask,
    int stop_on_state_mask, int snap_to_final_angle, int reserved);
extern void func_ov000_020a57e4(
    FieldEntity *entity, int relative_center, fx32 center_x, fx32 center_y,
    int relative_angle, int angle, int duration, int direction,
    int secondary_axis_scale, int snap_to_final_angle, int reserved);
extern void func_ov000_020b2c08(
    FieldEntity *entity, int relative_center, fx32 center_x, fx32 center_y,
    fx32 center_z, int relative_angle, int angle, int duration, int plane,
    int direction, int secondary_axis_scale, int stop_on_contact_mask,
    int stop_on_state_mask, int snap_to_final_angle, int reserved);
extern void func_ov000_020a55f0(
    FieldEntity *entity, FieldEntity *center_entity, fx32 center_x,
    fx32 center_y, int relative_angle, int angle, int speed, int direction,
    int secondary_axis_scale, int snap_to_final_angle, int reserved);
extern void func_ov000_020b295c(
    FieldEntity *entity, FieldEntity *center_entity, fx32 center_x,
    fx32 center_y, fx32 center_z, int relative_angle, int angle, int speed,
    int plane, int direction, int secondary_axis_scale,
    int stop_on_contact_mask, int stop_on_state_mask,
    int snap_to_final_angle, int reserved);
extern void func_ov000_020a5408(
    FieldEntity *entity, FieldEntity *center_entity, fx32 center_x,
    fx32 center_y, int relative_angle, int angle, int duration,
    int direction, int secondary_axis_scale, int snap_to_final_angle,
    int reserved);
extern void func_ov000_020b26ac(
    FieldEntity *entity, FieldEntity *center_entity, fx32 center_x,
    fx32 center_y, fx32 center_z, int relative_angle, int angle,
    int duration, int plane, int direction, int secondary_axis_scale,
    int stop_on_contact_mask, int stop_on_state_mask,
    int snap_to_final_angle, int reserved);
extern void func_ov000_020b44ac(FieldEntity *entity, fx32 initial_velocity,
                                fx32 gravity, fx32 terminal_velocity);
extern void func_ov000_020b4414(FieldEntity *entity, fx32 height,
                                fx32 gravity, fx32 terminal_velocity);
extern void func_ov000_020b4300(FieldEntity *entity);
extern void func_ov000_020a6710(FieldEntity *entity, fx32 x, fx32 y);
extern void func_ov000_020b42c8(FieldEntity *entity, fx32 x, fx32 y,
                                fx32 z);
extern void func_ov000_020736a4(u8 *field_context, FieldEntity *entity,
                                int argument_2, int argument_3);
extern void func_ov000_020b2020(FieldEntity *entity, int minimum_x,
                                int minimum_y, int maximum_x,
                                int maximum_y);
extern void func_ov000_020b1efc(FieldEntity *entity, const void *profile,
                                int reserved);
extern void func_ov000_020b1b88(FieldEntity *entity);
extern void func_ov000_020b172c(FieldEntity *entity);
extern void func_ov000_020b16d4(FieldEntity *entity);
extern void func_ov000_020b167c(FieldEntity *entity);
extern void func_ov000_020b1e5c(FieldEntity *entity, int profile_slot);
extern void func_ov000_020b1a24(FieldEntity *entity, const void *path,
                                int path_size_halfwords);
extern void func_ov000_020b18e4(FieldEntity *entity);
extern void func_ov000_020b1a08(FieldEntity *entity);
extern void func_ov000_020a6690(FieldEntity *entity, int direction_mode,
                                int direction, int refresh);
extern void func_ov000_020b426c(FieldEntity *entity, FieldEntity *target);
extern void func_ov000_020713bc(u8 *field_context, FieldEntity *entity,
                                int effect_slot, int animation_id,
                                int position_mode, s16 x, s16 y,
                                int lifetime, int follow_entity);
extern void func_ov000_0207138c(u8 *field_context, FieldEntity *entity);
extern int func_ov000_0207133c(u8 *field_context, FieldEntity *entity);
extern void func_ov000_020bc8e4(FieldEntity *entity, int enabled);
extern void func_ov000_020bc7d0(FieldEntity *entity, int enabled);
extern void func_ov000_0209b53c(void *party_controller);
extern void func_ov000_0209b0b8(void *party_controller);
extern void func_ov000_0209e720(void *party_manager, int party_side,
                                int instant);
extern int func_ov000_0209e6f4(void *party_manager);
extern void func_ov000_0209dfd4(void *party_manager);
extern int func_ov000_0209df70(void *party_manager);
extern void func_ov000_0209ce44(void *party_controller, int member,
                                int character_id);
extern void func_ov000_0209ce00(void *party_controller);
extern void func_ov000_0209bc84(void *party_controller, int facing_direction,
                                int instant, int reserved);
extern void func_ov000_02071a38(u8 *field_context, int layout_mode,
                                int instant);
extern void func_ov000_02089da8(void *baby_party_controller, int visible);
extern void func_ov000_0209ec48(void *party_manager, int party_side,
                                u16 field_mode, int argument_3,
                                int argument_4);
extern void func_ov000_0209e8cc(void *party_manager, int party_side,
                                u16 action_type_mask);
extern int func_ov000_0209ebdc(void *party_manager, int party_side,
                               int present_party_mask);
extern void func_ov000_0208f1e0(void *baby_party_controller, int member);
extern int func_ov000_0208efc4(void *baby_party_controller, int member);
extern void func_ov000_0208efec(void *baby_party_controller, int member);
extern void func_ov000_0209de24(void *party_manager,
                                int marker_animation_id);
extern void func_ov000_0209dddc(void *party_manager, int party_side,
                                int anchor_entity_slot);
extern void func_ov000_0209ddcc(void *party_manager);
extern void func_ov000_0209ddbc(void *party_manager,
                                int legacy_parameter);
extern void func_ov000_0209dda4(void *party_manager,
                                fx32 target_elevation);
extern void func_ov000_0209dd8c(void *party_manager);
extern void func_ov000_0209e1a8(void *party_manager);
extern void func_ov000_020a0c30(void *party_manager, int party_side);
extern void func_ov000_0209cbfc(void *party_controller, int reserved);
extern void func_ov000_0209cb90(void *party_controller, int reserved);
extern void func_ov000_0207fd54(
    u8 *field_context, int party_side, int destination_room_id, s16 x, s16 y,
    s16 z, int facing_direction, int lock_control, s16 arrival_script_id,
    int bgm_policy, int reserved_0, int synchronize_paired_field,
    int reserved_1, int reserved_2);
extern void func_ov000_0207fac8(
    u8 *field_context, int destination_room_id, s16 party_0_x,
    s16 party_0_y, s16 party_0_z, int party_0_facing, s16 party_1_x,
    s16 party_1_y, s16 party_1_z, int party_1_facing,
    s16 arrival_script_id, int bgm_policy, int reserved_0,
    int synchronize_paired_field, int reserved_1, int reserved_2);
extern void func_ov000_0207fa18(u8 *field_context, int destination_room_id,
                                s16 arrival_script_id, int bgm_policy,
                                int reserved);
extern void func_ov000_0209f644(
    void *party_manager, int party_side, int destination_room_id, int x,
    int y, int z, int facing_direction, int animate_entry,
    s16 arrival_script_id, int bgm_policy, int preserve_field_0_anchor,
    int preserve_field_1_anchor);
extern void func_ov000_0207f288(u8 *field_context, int party_side, s16 x,
                                s16 y, s16 z, int facing_direction);
extern void func_ov000_0207af94(u8 *field_context, int map_event_index);
extern void func_ov000_02073498(
    u8 *field_context, int coordinate_mode, fx32 x, fx32 y, int motion_3,
    int motion_4, int motion_5, int motion_6, int x_motion_flag,
    int y_motion_flag, int reserved);
extern void func_ov000_020732f4(
    u8 *field_context, int coordinate_mode, fx32 x, fx32 y, int duration,
    int x_motion_flag, int y_motion_flag, int reserved);
extern void func_ov000_020731bc(
    u8 *field_context, FieldEntity *entity, fx32 x_offset, fx32 y_offset,
    int motion_3, int x_motion_flag, int y_motion_flag, int reserved);
extern void func_ov000_02073110(
    u8 *field_context, FieldEntity *entity, fx32 x_offset, fx32 y_offset,
    int duration, int x_motion_flag, int y_motion_flag, int reserved);
extern void func_ov000_02072a58(u8 *field_context, int argument_1,
                                int argument_2);
extern void func_ov000_020727fc(
    u8 *field_context, int bg_layer, fx32 x_delta, fx32 y_delta,
    int motion_3, int motion_4, int motion_5, int motion_6,
    int motion_flag);
extern void func_ov000_02072660(u8 *field_context, int bg_layer,
                                fx32 x_delta, fx32 y_delta, int duration,
                                int motion_flag);
extern void func_ov000_020721c0(u8 *field_context, int bg_layer);
extern void func_ov000_02072074(u8 *field_context, int axis,
                                fx32 amplitude, int step, int half_cycles,
                                int rumble_pattern);
extern void func_ov000_02071fec(u8 *field_context);
extern void func_ov000_02075c34(u8 *field_context, s16 start_brightness,
                                s16 target_brightness, u16 duration);
extern void func_ov000_0207c098(u8 *field_context);
extern void func_ov000_02093c98(void *party_controller);
extern void func_ov000_02075bc8(u8 *field_context);
extern void func_ov000_02075814(
    u8 *field_context, int animation_slot, int direction_profile,
    u16 object_mask_high, u16 object_mask_low, u16 standard_bg_mask,
    u16 extended_bg_mask_0, u16 extended_bg_mask_1,
    u16 extended_bg_mask_2, s16 duration, u16 color);
extern int func_ov000_02075790(u8 *field_context, int animation_slot);
extern void func_ov000_02075730(u8 *field_context, int animation_slot,
                                int paused);
extern void func_ov000_020756ac(u8 *field_context, int animation_slot);
extern void func_ov000_02075600(u8 *field_context, int palette_region,
                                int source_bank, int target_bank,
                                int duration);
extern void func_ov000_02074810(u8 *field_context, int wipe_type,
                                const void *parameters);
extern void func_02036988(volatile u32 *blend_control,
                          int first_target_plane_mask,
                          int second_target_plane_mask,
                          int first_coefficient, int second_coefficient);
extern void func_ov000_02069d38(
    void *field_system, const s32 *entity_selectors, int anchor_entity,
    int destination_room_id, int arrival_script_id,
    int paired_arrival_script_id, int legacy_parameter,
    int travel_direction, int transit_room_id);
extern void func_ov000_02069b24(void *field_system,
                                const s32 *entity_selectors,
                                int anchor_entity,
                                const s32 *arrival_directions);
extern void func_ov000_02069aac(void *field_system);
extern void func_ov000_02069284(
    void *field_system, const s32 *entity_selectors, int anchor_entity,
    int destination_room_id, int argument_4, int argument_5,
    int orbit_entities, s16 center_x_offset, s16 center_y_offset);
extern void func_ov000_0206900c(void *field_system, int reverse_direction,
                                fx32 initial_speed, fx32 acceleration,
                                fx32 maximum_speed);
extern void func_ov000_02068fc4(void *field_system, fx32 deceleration);
extern int func_ov000_02068fac(void *field_system);
extern int func_ov000_02066c50(void *field_system);
extern void func_ov000_02074e14(
    u8 *field_context, int screen, int resource_index, int anchor_entity_0,
    int anchor_entity_1, int anchor_entity_2, int anchor_entity_3,
    s16 x_offset, s16 y_offset, s16 z_offset, int attachment_mode,
    s16 render_priority);
extern int func_ov000_02074dd0(u8 *field_context);
extern void func_ov000_02074d8c(u8 *field_context, int effect_slot,
                                int screen, int resource_index, s16 x,
                                s16 y, s16 playback_speed);
extern int func_ov000_02074d50(u8 *field_context, int effect_slot);
extern void func_020052b0(int scene_id, int argument_1);
extern void func_ov000_020a23f8(void *party_manager, int reserved);
extern void func_ov000_02079d74(u8 *field_context, int party_mode);
extern void func_ov000_020714e4(u8 *field_context);
extern int func_ov000_020714a8(u8 *field_context);
extern void func_ov000_0206ba2c(
    void *field_system, int start_mode, s16 encounter_id,
    int reserved_transition_argument, u16 source_room_id, int contact_mode,
    int field_screen, FieldEntity *encounter_entity,
    FieldEntity *primary_entity, int party_context,
    int transition_variant);
extern void func_ov000_0206b0a0(void *field_system, int scene_mode,
                                int party_context, int initial_section,
                                int fade_to_black,
                                int return_screen_flag);
extern void func_ov000_0206aeb0(void *field_system, int party_context,
                                int fade_to_black,
                                int return_screen_flag);
extern void func_ov000_0206acf8(void *field_system, int shop_scene_id,
                                int fade_to_black,
                                int return_screen_flag);
extern void func_ov000_0206abd8(void *field_system, int fade_to_black);
extern void func_ov000_0206a9a4(void *field_system, int fade_to_black);
extern void func_02019330(int new_game_preset);
extern void func_ov000_0206d274(void *field_system, int character_index);
extern void func_ov000_0206d1dc(void *field_system, int character_index,
                                int hp_delta);
extern int func_02019174(u16 item_id, int count_delta);
extern const u16 data_02048f1a[];
extern void func_ov000_02081bd4(int enabled);
extern void func_ov000_02066b34(void *field_system);
extern int func_ov000_02066b0c(void *field_system);
extern void func_ov000_02066ae4(void *field_system);
extern int func_ov000_020660f8(void *field_system);
extern void func_02007408(int rumble_pattern, int repeat_count);
extern void *data_0205a00c;
extern void func_0202e04c(void);
extern void func_ov000_020beda0(void *timer, s8 minutes, s8 seconds,
                                s8 centiseconds);
extern void func_ov000_020beca4(void *timer, int screen, s16 x, s16 y);
extern void func_ov000_020becc8(void *timer, int visible);
extern void func_ov000_0206f8ac(u8 *field_context);
extern int func_ov000_02070930(
    u8 *field_context, int window_mode, int x, int y, int width,
    int height, int tail_style, int tail_size, int vertical_placement,
    int reserved, int tail_x, int text_control_enabled,
    int text_archive_id, int message_id, int message_slot,
    int message_speed, u8 *message_state);
extern int func_ov000_020700ec(
    u8 *field_context, int window_mode, FieldEntity *entity, int width,
    int height, int tail_style, int tail_size, int vertical_placement,
    int tail_x, int text_control_enabled, int text_archive_id,
    int message_id, int message_slot, int message_speed,
    u8 *message_state, FieldEntity *linked_owner);
extern int func_ov000_0206fb74(u8 *field_context, int message_slot);
extern int func_ov000_0206fc50(u8 *field_context, int message_slot);
extern void func_ov000_0206facc(u8 *field_context, int message_slot);
extern void func_ov000_0206f6c0(u8 *field_context, int window_slot,
                                int enabled);
extern void func_ov000_0206f448(u8 *field_context, int window_slot,
                                fx32 x, fx32 y, int speed);
extern void func_ov000_0206f378(u8 *field_context, int window_slot,
                                fx32 x, fx32 y, int duration);
extern void func_02027cb8(s16 sound_id, int playback_mode,
                          int reserved_channel);
extern void func_02027b7c(s16 sound_id);
extern void func_02027a2c(s16 sequence_id, int reserved_channel);
extern void func_02027818(s16 sequence_id);
extern u32 func_02027d6c(void);
extern void func_02027ea4(int sequence_id, u8 buffer_index,
                          int fade_duration);
extern int func_02027718(void);
extern void func_02027e3c(int buffer_index);
extern void func_02027e20(int fade_duration);
extern void func_02027db8(int muted, int reserved_channel);
extern void func_0202775c(int sound_group_id);
extern int func_02027728(void);
extern void func_02027744(void);

typedef struct FieldMapController FieldMapController;
typedef struct FieldMapControllerVTable FieldMapControllerVTable;

struct FieldMapControllerVTable {
    u8 unknown_00[0x44];
    void (*start_tile_animation)(FieldMapController *controller,
                                 u16 animation_index);
    u8 unknown_48[4];
    void (*pause_tile_animation)(FieldMapController *controller,
                                 u16 animation_index);
    void (*resume_tile_animation)(FieldMapController *controller,
                                  u16 animation_index);
    u8 unknown_54[8];
    int (*are_palette_animations_active)(FieldMapController *controller);
    u8 unknown_60[4];
    void (*start_palette_animation)(FieldMapController *controller,
                                    u8 animation_index, int loop_mode);
    u8 unknown_68[4];
    void (*reset_palette_animations)(FieldMapController *controller);
    void (*stop_palette_animation)(FieldMapController *controller,
                                   u8 animation_index);
    void (*resume_palette_animation)(FieldMapController *controller,
                                     u8 animation_index);
    void (*restore_base_palettes)(FieldMapController *controller);
};

struct FieldMapController {
    FieldMapControllerVTable *vtable;
};

typedef struct FieldCircularWipeParameters {
    u16 duration_and_retain_mask;
    u8 center_x;
    u8 center_y;
    s16 start_radius;
    s16 end_radius;
} FieldCircularWipeParameters;

typedef struct FieldRectangularWipeParameters {
    u16 duration_and_retain_mask;
    u8 center_x;
    u8 center_y;
    s16 start_left;
    s16 start_right;
    s16 start_top;
    s16 start_bottom;
    s16 end_left;
    s16 end_right;
    s16 end_top;
    s16 end_bottom;
} FieldRectangularWipeParameters;
extern void func_ov000_02076f40(u8 *field_context, FieldEntity *entity,
                                int palette_selector);
extern void func_0200940c(FieldRenderObject *render_object, int speed);
extern s32 FX_Atan2(fx32 y, fx32 x);

enum FieldVmOpcode {
    FIELD_VM_BRANCH_IF_OWNER_SUBTYPE = 0x033,
    FIELD_VM_BRANCH_RELATIVE = 0x034,
    FIELD_VM_SET_AUX_SCRIPT_ENABLED = 0x035,
    FIELD_VM_START_AUX_SCRIPT = 0x036,
    FIELD_VM_WAIT_AUX_SCRIPT = 0x037,
    FIELD_VM_STOP_AUX_SCRIPT = 0x038,
    FIELD_VM_PAUSE_AUX_SCRIPT = 0x039,
    FIELD_VM_RESUME_AUX_SCRIPT = 0x03A,
    FIELD_VM_GET_AUX_SCRIPT_STATE = 0x03B,
    FIELD_VM_START_INLINE_ENTITY_SCRIPT = 0x03C,
    FIELD_VM_START_INLINE_ENTITY_SCRIPT_AND_WAIT = 0x03D,
    FIELD_VM_START_RELATIVE_ENTITY_SCRIPT = 0x03E,
    FIELD_VM_WAIT_ENTITY_SCRIPT = 0x03F,
    FIELD_VM_WAIT_MATCHING_ENTITY_SCRIPTS = 0x040,
    FIELD_VM_STOP_ENTITY_SCRIPT = 0x041,
    FIELD_VM_STOP_MATCHING_ENTITY_SCRIPTS = 0x042,
    FIELD_VM_PAUSE_ENTITY_SCRIPT = 0x043,
    FIELD_VM_PAUSE_MATCHING_ENTITY_SCRIPTS = 0x044,
    FIELD_VM_RESUME_ENTITY_SCRIPT = 0x045,
    FIELD_VM_RESUME_MATCHING_ENTITY_SCRIPTS = 0x046,
    FIELD_VM_GET_ENTITY_SCRIPT_STATE = 0x047,
    FIELD_VM_START_PAIRED_FIELD_SCRIPT = 0x048,
    FIELD_VM_GET_ENTITY_PROPERTY = 0x049,
    FIELD_VM_SET_ENTITY_VISIBLE = 0x04A,
    FIELD_VM_SET_ENTITY_ENABLED = 0x04B,
    FIELD_VM_SET_ENTITY_TURN_TO_INTERACTOR_ENABLED = 0x04C,
    FIELD_VM_SET_ENTITY_GROUND_TRACKING = 0x04D,
    FIELD_VM_SET_ENTITY_ALTERNATE_COLLISION_FACES_ENABLED = 0x04E,
    FIELD_VM_SET_ENTITY_NAVIGATION_OBSTACLE_IGNORED = 0x04F,
    FIELD_VM_SET_ENTITY_OFFSCREEN_CONTACT_RETENTION_ENABLED = 0x050,
    FIELD_VM_SET_ENTITY_RESERVED_STATE_FLAG = 0x051,
    FIELD_VM_SET_ENTITY_LINKED_CONTACT_MODE = 0x052,
    FIELD_VM_SET_ENTITY_CONTACT_DIRECTION_FILTER = 0x053,
    FIELD_VM_SET_ENTITY_SEMITRANSPARENT = 0x054,
    FIELD_VM_CONFIGURE_ENTITY_SHADOW = 0x055,
    FIELD_VM_SET_ENTITY_MAP_SYNC_AXES = 0x056,
    FIELD_VM_SET_ENTITY_NAVIGATION_COLLISION_BOUNDS = 0x057,
    FIELD_VM_RESTORE_ENTITY_NAVIGATION_COLLISION_BOUNDS = 0x058,
    FIELD_VM_SET_ENTITY_COLLISION_RESPONSE_CHANNELS = 0x059,
    FIELD_VM_SET_ENTITY_COLLISION_RESPONSE_CHANNELS_MASKED = 0x05A,
    FIELD_VM_RESTORE_ENTITY_COLLISION_RESPONSE_CHANNELS = 0x05B,
    FIELD_VM_SET_ENTITY_BODY_COLLISION_BOUNDS = 0x05C,
    FIELD_VM_RESTORE_ENTITY_BODY_COLLISION_BOUNDS = 0x05D,
    FIELD_VM_SET_ENTITY_RESERVED_COLLISION_FLAG = 0x05E,
    FIELD_VM_SET_ENTITY_CATEGORY_COLLISION_POLICY = 0x05F,
    FIELD_VM_SET_ENTITY_RENDER_LAYER = 0x060,
    FIELD_VM_SET_ENTITY_BODY_COLLISION_ENABLED = 0x061,
    FIELD_VM_SET_ENTITY_RENDER_ORDER_PRIORITIES = 0x062,
    FIELD_VM_SET_ENTITY_INTERACTION_BOUNDS = 0x063,
    FIELD_VM_LEGACY_NOOP_064 = 0x064,
    FIELD_VM_RESTORE_ENTITY_INTERACTION_BOUNDS = 0x065,
    FIELD_VM_SET_ENTITY_ANIMATION_SPEED = 0x066,
    FIELD_VM_SET_ENTITY_LOCOMOTION_PARAMETERS = 0x067,
    FIELD_VM_SET_ENTITY_DEFAULT_VERTICAL_LAUNCH_VELOCITY = 0x068,
    FIELD_VM_SET_ENTITY_DEFAULT_GRAVITY = 0x069,
    FIELD_VM_SET_ENTITY_TERMINAL_FALL_VELOCITY = 0x06A,
    FIELD_VM_SET_ENTITY_SHADOW_SUPPORT_ENABLED = 0x06B,
    FIELD_VM_BIND_ENTITY_RESOURCE = 0x06C,
    FIELD_VM_RESTORE_ENTITY_RESOURCE_STATE = 0x06D,
    FIELD_VM_WAIT_ENTITY_ANIMATION = 0x06E,
    FIELD_VM_SET_ENTITY_ANIMATION = 0x06F,
    FIELD_VM_RESTORE_ENTITY_ANIMATION = 0x070,
    FIELD_VM_SET_ENTITY_BEHAVIOR_MODE = 0x071,
    FIELD_VM_RESTORE_ENTITY_BEHAVIOR_STATE = 0x072,
    FIELD_VM_SET_ENTITY_PALETTE_PROFILE = 0x073,
    FIELD_VM_RESTORE_ENTITY_PALETTE_PROFILE = 0x074,
    FIELD_VM_START_ENTITY_PALETTE_ANIMATION = 0x075,
    FIELD_VM_STOP_ENTITY_PALETTE_ANIMATION = 0x076,
    FIELD_VM_PAUSE_ENTITY_PALETTE_ANIMATION = 0x077,
    FIELD_VM_RESUME_ENTITY_PALETTE_ANIMATION = 0x078,
    FIELD_VM_REQUEST_ENTITY_BASE_PALETTE_RELOAD = 0x079,
    FIELD_VM_WAIT_ENTITY_BASE_PALETTE_RELOAD = 0x07A,
    FIELD_VM_START_ENTITY_SCALING = 0x07B,
    FIELD_VM_START_ENTITY_TIMED_SCALING = 0x07C,
    FIELD_VM_WAIT_ENTITY_SCALING = 0x07D,
    FIELD_VM_STOP_ENTITY_SCALING = 0x07E,
    FIELD_VM_START_ENTITY_ROTATION = 0x07F,
    FIELD_VM_START_ENTITY_TIMED_ROTATION = 0x080,
    FIELD_VM_WAIT_ENTITY_ROTATION = 0x081,
    FIELD_VM_STOP_ENTITY_ROTATION = 0x082,
    FIELD_VM_START_ENTITY_MOVEMENT = 0x083,
    FIELD_VM_START_ENTITY_TIMED_MOVEMENT = 0x084,
    FIELD_VM_START_ENTITY_MOVEMENT_RELATIVE_TO_ENTITY = 0x085,
    FIELD_VM_START_ENTITY_TIMED_MOVEMENT_RELATIVE_TO_ENTITY = 0x086,
    FIELD_VM_START_ENTITY_ORBIT_AROUND_POINT = 0x087,
    FIELD_VM_START_ENTITY_TIMED_ORBIT_AROUND_POINT = 0x088,
    FIELD_VM_START_ENTITY_ORBIT_AROUND_ENTITY = 0x089,
    FIELD_VM_START_ENTITY_TIMED_ORBIT_AROUND_ENTITY = 0x08A,
    FIELD_VM_WAIT_ENTITY_MOVEMENT = 0x08B,
    FIELD_VM_CANCEL_ENTITY_MOVEMENT = 0x08C,
    FIELD_VM_START_ENTITY_VERTICAL_MOTION = 0x08D,
    FIELD_VM_START_ENTITY_VERTICAL_MOTION_TO_HEIGHT = 0x08E,
    FIELD_VM_WAIT_ENTITY_VERTICAL_MOTION = 0x08F,
    FIELD_VM_STOP_ENTITY_VERTICAL_MOTION = 0x090,
    FIELD_VM_SET_ENTITY_POSITION = 0x091,
    FIELD_VM_SET_ENTITY_ROAMING_BOUNDS = 0x092,
    FIELD_VM_ADD_ENTITY_ROAMING_PROFILE = 0x093,
    FIELD_VM_SET_ENTITY_ROAMING_BOUNDARY_CLAMP_ENABLED = 0x094,
    FIELD_VM_START_ENTITY_RANDOM_ROAMING = 0x095,
    FIELD_VM_STOP_ENTITY_RANDOM_ROAMING = 0x096,
    FIELD_VM_PAUSE_ENTITY_RANDOM_ROAMING = 0x097,
    FIELD_VM_RESUME_ENTITY_RANDOM_ROAMING = 0x098,
    FIELD_VM_CLEAR_ENTITY_ROAMING_PROFILES = 0x099,
    FIELD_VM_LOAD_ENTITY_WAYPOINT_PATH = 0x09A,
    FIELD_VM_START_ENTITY_WAYPOINT_PATH = 0x09B,
    FIELD_VM_STOP_ENTITY_WAYPOINT_PATH = 0x09C,
    FIELD_VM_PAUSE_ENTITY_WAYPOINT_PATH = 0x09D,
    FIELD_VM_RESUME_ENTITY_WAYPOINT_PATH = 0x09E,
    FIELD_VM_CLEAR_ENTITY_WAYPOINT_PATH = 0x09F,
    FIELD_VM_SET_ENTITY_FACING_DIRECTION = 0x0A0,
    FIELD_VM_FACE_ENTITY_TOWARD_ENTITY = 0x0A1,
    FIELD_VM_SPAWN_ENTITY_EFFECT_SPRITE = 0x0A2,
    FIELD_VM_REMOVE_ENTITY_EFFECT_SPRITE = 0x0A3,
    FIELD_VM_WAIT_ENTITY_EFFECT_SPRITE = 0x0A4,
    FIELD_VM_SET_FIELD_BLOCK_IDLE_BOBBING_ENABLED = 0x0A5,
    FIELD_VM_SET_FIELD_BLOCK_BOUNCE_CONTROLLER_ENABLED = 0x0A6,
    FIELD_VM_WAIT_FIELD_BLOCK_BOUNCE = 0x0A7,
    FIELD_VM_SET_ENEMY_JUMP_FIRST_STRIKE_ENABLED = 0x0A8,
    FIELD_VM_SET_ENEMY_SPIKED_JUMP_RESPONSE = 0x0A9,
    FIELD_VM_SET_ENEMY_SPECIAL_CONTACT_REMOVAL_ENABLED = 0x0AA,
    FIELD_VM_SET_ENEMY_IMMEDIATE_BATTLE_REMOVAL_ENABLED = 0x0AB,
    FIELD_VM_SET_ENTITY_SCRIPT_VALUE = 0x0AC,
    FIELD_VM_GET_ENTITY_SCRIPT_VALUE = 0x0AD,
    FIELD_VM_REJOIN_PARTY_FOLLOWER = 0x0AE,
    FIELD_VM_WAIT_PARTY_FOLLOWER_REJOINED = 0x0AF,
    FIELD_VM_DETACH_PARTY_FOLLOWER = 0x0B0,
    FIELD_VM_REUNITE_SPLIT_PARTIES = 0x0B1,
    FIELD_VM_WAIT_SPLIT_PARTY_REUNION = 0x0B2,
    FIELD_VM_START_BABY_PIGGYBACK_MOUNT = 0x0B3,
    FIELD_VM_WAIT_BABY_PIGGYBACK_MOUNT = 0x0B4,
    FIELD_VM_SET_PARTY_MEMBER_CHARACTER_ID = 0x0B5,
    FIELD_VM_RESET_PARTY_MEMBER_CHARACTERS = 0x0B6,
    FIELD_VM_SET_PARTY_FACING_DIRECTION = 0x0B7,
    FIELD_VM_SET_PARTY_MEMBER_SWITCHING_ENABLED = 0x0B8,
    FIELD_VM_LEGACY_NOOP_0B9 = 0x0B9,
    FIELD_VM_SET_FIELD_PARTY_HUD_LAYOUT = 0x0BA,
    FIELD_VM_SET_PARTY_ATTACHED_SPRITE_VISIBLE = 0x0BB,
    FIELD_VM_SET_PARTY_FIELD_MODE = 0x0BC,
    FIELD_VM_CANCEL_PARTY_ACTIONS = 0x0BD,
    FIELD_VM_WAIT_PARTY_ACTIONS_IDLE = 0x0BE,
    FIELD_VM_START_BABY_HAMMER_SWING = 0x0BF,
    FIELD_VM_WAIT_BABY_HAMMER_SWING = 0x0C0,
    FIELD_VM_FINISH_BABY_HAMMER_SWING = 0x0C1,
    FIELD_VM_PREPARE_BABY_DRILL_MARKER = 0x0C2,
    FIELD_VM_MOVE_PARTY_TO_GIMMICK_ANCHOR = 0x0C3,
    FIELD_VM_PREPARE_BROS_BALL_CANNON_LAUNCH = 0x0C4,
    FIELD_VM_LAUNCH_BROS_BALL_FROM_CANNON = 0x0C5,
    FIELD_VM_LAUNCH_BABY_PARTY_TO_ELEVATION = 0x0C6,
    FIELD_VM_DROP_BABY_PARTY_FROM_ELEVATION = 0x0C7,
    FIELD_VM_COMPLETE_AIRBORNE_BABY_TRANSFERS = 0x0C8,
    FIELD_VM_GET_PARTY_CONTROLLER_PROPERTY = 0x0C9,
    FIELD_VM_SWITCH_ACTIVE_PARTY = 0x0CA,
    FIELD_VM_WAIT_ACTIVE_PARTY_SWITCH = 0x0CB,
    FIELD_VM_SET_PARTY_PIGGYBACK_STATE = 0x0CC,
    FIELD_VM_RELEASE_ROOM_TRANSITION_CONTROL_LOCK = 0x0CD,
    FIELD_VM_SET_PARTY_LEADER_ANIMATION_OVERRIDE = 0x0CE,
    FIELD_VM_CHANGE_FIELD_ROOM_FOR_PARTY = 0x0CF,
    FIELD_VM_CHANGE_FIELD_ROOM_FOR_BOTH_PARTIES = 0x0D0,
    FIELD_VM_CHANGE_FIELD_ROOM = 0x0D1,
    FIELD_VM_TRANSFER_PARTY_BETWEEN_FIELD_SCREENS = 0x0D2,
    FIELD_VM_ENSURE_PARTY_IN_CURRENT_FIELD = 0x0D3,
    FIELD_VM_ACTIVATE_FIELD_MAP_EVENT = 0x0D4,
    FIELD_VM_START_MAP_TILE_ANIMATION = 0x0D5,
    FIELD_VM_WAIT_MAP_TILE_ANIMATION = 0x0D6,
    FIELD_VM_PAUSE_MAP_TILE_ANIMATION = 0x0D7,
    FIELD_VM_RESUME_MAP_TILE_ANIMATION = 0x0D8,
    FIELD_VM_SET_FIELD_BG_LAYERS_ENABLED = 0x0D9,
    FIELD_VM_START_CAMERA_PROFILED_MOVEMENT = 0x0DA,
    FIELD_VM_START_CAMERA_TIMED_MOVEMENT = 0x0DB,
    FIELD_VM_START_CAMERA_PROFILED_ENTITY_TRACKING = 0x0DC,
    FIELD_VM_START_CAMERA_TIMED_ENTITY_TRACKING = 0x0DD,
    FIELD_VM_WAIT_CAMERA_MOVEMENT = 0x0DE,
    FIELD_VM_STOP_CAMERA_MOVEMENT = 0x0DF,
    FIELD_VM_START_BG_LAYER_PROFILED_SCROLL = 0x0E0,
    FIELD_VM_START_BG_LAYER_TIMED_SCROLL = 0x0E1,
    FIELD_VM_WAIT_BG_LAYER_SCROLL = 0x0E2,
    FIELD_VM_STOP_BG_LAYER_SCROLL = 0x0E3,
    FIELD_VM_START_CAMERA_SHAKE = 0x0E4,
    FIELD_VM_WAIT_CAMERA_SHAKE = 0x0E5,
    FIELD_VM_STOP_CAMERA_SHAKE = 0x0E6,
    FIELD_VM_START_MAP_PALETTE_ANIMATION = 0x0E7,
    FIELD_VM_RESET_ALL_MAP_PALETTE_ANIMATIONS = 0x0E8,
    FIELD_VM_STOP_MAP_PALETTE_ANIMATION = 0x0E9,
    FIELD_VM_RESUME_MAP_PALETTE_ANIMATION = 0x0EA,
    FIELD_VM_RESTORE_MAP_BASE_PALETTES = 0x0EB,
    FIELD_VM_WAIT_MAP_PALETTE_ANIMATIONS = 0x0EC,
    FIELD_VM_START_MASTER_BRIGHTNESS_TRANSITION = 0x0ED,
    FIELD_VM_WAIT_MASTER_BRIGHTNESS_TRANSITION = 0x0EE,
    FIELD_VM_PAUSE_MASTER_BRIGHTNESS_TRANSITION = 0x0EF,
    FIELD_VM_RESUME_MASTER_BRIGHTNESS_TRANSITION = 0x0F0,
    FIELD_VM_REVERSE_MASTER_BRIGHTNESS_TRANSITION = 0x0F1,
    FIELD_VM_START_FIELD_PALETTE_ANIMATION = 0x0F2,
    FIELD_VM_WAIT_FIELD_PALETTE_ANIMATION = 0x0F3,
    FIELD_VM_PAUSE_FIELD_PALETTE_ANIMATION = 0x0F4,
    FIELD_VM_RESUME_FIELD_PALETTE_ANIMATION = 0x0F5,
    FIELD_VM_REVERSE_FIELD_PALETTE_ANIMATION = 0x0F6,
    FIELD_VM_START_PALETTE_BANK_CROSSFADE = 0x0F7,
    FIELD_VM_WAIT_PALETTE_BANK_CROSSFADE = 0x0F8,
    FIELD_VM_START_PATTERN_SCREEN_WIPE = 0x0F9,
    FIELD_VM_START_CIRCULAR_SCREEN_WIPE = 0x0FA,
    FIELD_VM_START_RECTANGULAR_SCREEN_WIPE = 0x0FB,
    FIELD_VM_WAIT_SCREEN_WIPE = 0x0FC,
    FIELD_VM_SET_FIELD_SCREEN_ALPHA_BLEND = 0x0FD,
    FIELD_VM_START_TIME_HOLE_DEPARTURE = 0x0FE,
    FIELD_VM_PREPARE_TIME_HOLE_ARRIVAL = 0x0FF,
    FIELD_VM_START_PREPARED_TIME_HOLE_ARRIVAL = 0x100,
    FIELD_VM_START_TIME_HOLE_TUNNEL = 0x101,
    FIELD_VM_START_FIELD_VERTICAL_ACCELERATING_SCROLL = 0x102,
    FIELD_VM_DECELERATE_FIELD_VERTICAL_SCROLL = 0x103,
    FIELD_VM_WAIT_FIELD_VERTICAL_SCROLL = 0x104,
    FIELD_VM_BEGIN_TIME_HOLE_TUNNEL_WHITEOUT = 0x105,
    FIELD_VM_WAIT_TIME_HOLE_TRANSITION = 0x106,
    FIELD_VM_START_ANCHORED_FIELD_EFFECT_ANIMATION = 0x107,
    FIELD_VM_WAIT_ANCHORED_FIELD_EFFECT_ANIMATION = 0x108,
    FIELD_VM_START_FIELD_EFFECT_ANIMATION = 0x109,
    FIELD_VM_WAIT_FIELD_EFFECT_ANIMATION = 0x10A,
    FIELD_VM_RETURN_TO_TITLE_SCREEN = 0x10B,
    FIELD_VM_LEGACY_NOOP_10C = 0x10C,
    FIELD_VM_WAIT_PAIRED_FIELD_READY = 0x10D,
    FIELD_VM_ENABLE_FIELD_TRIGGER_AREA = 0x10E,
    FIELD_VM_DISABLE_FIELD_TRIGGER_AREA = 0x10F,
    FIELD_VM_APPLY_SAVED_PARTY_CONFIGURATION = 0x110,
    FIELD_VM_SET_FIELD_INPUT_DISABLE_MASK = 0x111,
    FIELD_VM_SET_FIELD_EVENT_INPUT_DISABLE_MASK = 0x112,
    FIELD_VM_SET_FIELD_INPUT_CHANNEL_OVERRIDE = 0x113,
    FIELD_VM_RESET_FIELD_INPUT_CHANNEL_OVERRIDES = 0x114,
    FIELD_VM_SET_FIELD_INPUT_DIRECTION_ROTATION = 0x115,
    FIELD_VM_SET_FIELD_PARTY_CONTROL_ENABLED = 0x116,
    FIELD_VM_SET_CAMERA_FOCUS_ENTITY = 0x117,
    FIELD_VM_REMOVE_ALL_ENTITY_EFFECT_SPRITES = 0x118,
    FIELD_VM_WAIT_ALL_ENTITY_EFFECT_SPRITES = 0x119,
    FIELD_VM_LEGACY_NOOP_11A = 0x11A,
    FIELD_VM_LEGACY_NOOP_11B = 0x11B,
    FIELD_VM_START_SCRIPTED_BATTLE = 0x11C,
    FIELD_VM_START_SCRIPTED_BATTLE_WITH_TRANSITION = 0x11D,
    FIELD_VM_OPEN_PAUSE_MENU = 0x11E,
    FIELD_VM_SET_SAVE_LOCATION_ID = 0x11F,
    FIELD_VM_OPEN_SAVE_MENU = 0x120,
    FIELD_VM_OPEN_SHOP = 0x121,
    FIELD_VM_START_STAFF_CREDITS = 0x122,
    FIELD_VM_OPEN_GAME_OVER_MENU = 0x123,
    FIELD_VM_INITIALIZE_NEW_GAME_PRESET = 0x124,
    FIELD_VM_RESTORE_PARTY_MEMBER_HP = 0x125,
    FIELD_VM_ADJUST_PARTY_MEMBER_HP = 0x126,
    FIELD_VM_SET_COINS = 0x127,
    FIELD_VM_ADJUST_COINS = 0x128,
    FIELD_VM_ADJUST_BEANS = 0x129,
    FIELD_VM_ADJUST_ITEM_COUNT = 0x12A,
    FIELD_VM_GET_ROOM_COMPANION_ID = 0x12B,
    FIELD_VM_ENABLE_TOUCHSCREEN_INPUT_TRACKING = 0x12C,
    FIELD_VM_DISABLE_TOUCHSCREEN_INPUT_TRACKING = 0x12D,
    FIELD_VM_SET_RETRY_CHECKPOINT = 0x12E,
    FIELD_VM_SET_RETRY_PROGRESS = 0x12F,
    FIELD_VM_CLEAR_EXTENDED_SAVE_FLAGS = 0x130,
    FIELD_VM_SET_EXTENDED_SAVE_FLAGS = 0x131,
    FIELD_VM_PREPARE_TOUCHSCREEN_MASK_ERASE = 0x132,
    FIELD_VM_WAIT_TOUCHSCREEN_MASK_ERASE_READY = 0x133,
    FIELD_VM_START_TOUCHSCREEN_MASK_ERASE = 0x134,
    FIELD_VM_WAIT_TOUCHSCREEN_MASK_ERASE_COMPLETE = 0x135,
    FIELD_VM_PLAY_RUMBLE_PATTERN = 0x136,
    FIELD_VM_STOP_RUMBLE = 0x137,
    FIELD_VM_SET_SINGLE_PASS_COLLISION = 0x138,
    FIELD_VM_SET_TIMER_VALUE = 0x139,
    FIELD_VM_SET_TIMER_DISPLAY_POSITION = 0x13A,
    FIELD_VM_SET_TIMER_VISIBLE = 0x13B,
    FIELD_VM_SET_TIMER_COUNT_RATE = 0x13C,
    FIELD_VM_PAUSE_TIMER = 0x13D,
    FIELD_VM_RESUME_TIMER = 0x13E,
    FIELD_VM_STOP_TIMER = 0x13F,
    FIELD_VM_OPEN_SCREEN_MESSAGE = 0x140,
    FIELD_VM_OPEN_ENTITY_MESSAGE = 0x141,
    FIELD_VM_WAIT_MESSAGE_FINISHED = 0x142,
    FIELD_VM_WAIT_MESSAGE_CLOSED = 0x143,
    FIELD_VM_CLOSE_MESSAGE = 0x144,
    FIELD_VM_SET_MESSAGE_WINDOW_SLIDE_MASK_ENABLED = 0x145,
    FIELD_VM_START_MESSAGE_WINDOW_SLIDE_BY_SPEED = 0x146,
    FIELD_VM_START_MESSAGE_WINDOW_SLIDE_OVER_FRAMES = 0x147,
    FIELD_VM_WAIT_MESSAGE_WINDOW_SLIDE = 0x148,
    FIELD_VM_PLAY_FIELD_SOUND = 0x149,
    FIELD_VM_STOP_FIELD_SOUND = 0x14A,
    FIELD_VM_PLAY_BACKGROUND_MUSIC = 0x14B,
    FIELD_VM_STOP_BACKGROUND_MUSIC = 0x14C,
    FIELD_VM_LOAD_BACKGROUND_MUSIC_RESOURCE = 0x14D,
    FIELD_VM_WAIT_BACKGROUND_MUSIC_RESOURCE = 0x14E,
    FIELD_VM_ACTIVATE_BACKGROUND_MUSIC = 0x14F,
    FIELD_VM_FADE_OUT_BACKGROUND_MUSIC = 0x150,
    FIELD_VM_SET_GLOBAL_SOUND_MUTED = 0x151,
    FIELD_VM_LOAD_SOUND_GROUP_ASYNC = 0x152,
    FIELD_VM_WAIT_SOUND_GROUP_LOAD = 0x153,
    FIELD_VM_RELEASE_SOUND_GROUP = 0x154
};

enum FieldVmContextOffset {
    FIELD_VM_RESOURCE_RECORDS_OFFSET = 0x00B4,
    FIELD_VM_RESOURCE_COUNTS_OFFSET = 0x2338,
    FIELD_VM_CURRENT_ROOM_ID_OFFSET = 0x23F4,
    FIELD_VM_SPECIAL_PARTY_STATE_OFFSET = 0x23F8,
    FIELD_VM_SPECIAL_PARTY_ENTITY_ID_OFFSET = 0x23FA,
    FIELD_VM_BRIGHTNESS_FLAGS_OFFSET = 0x2402,
    FIELD_VM_BRIGHTNESS_FRAMES_OFFSET = 0x2406,
    FIELD_VM_MESSAGE_WINDOW_SLIDE_FLAGS_OFFSET = 0x249C,
    FIELD_VM_CAMERA_SHAKE_FLAGS_OFFSET = 0x2488,
    FIELD_VM_CAMERA_FOCUS_X_OFFSET = 0x2468,
    FIELD_VM_CAMERA_FOCUS_Y_OFFSET = 0x246C,
    FIELD_VM_PARTY_OWNERSHIP_MASKS_OFFSET = 0x24C0,
    FIELD_VM_INPUT_DISABLE_MASK_OFFSET = 0x24C4,
    FIELD_VM_EVENT_INPUT_DISABLE_MASK_OFFSET = 0x24C6,
    FIELD_VM_INPUT_OVERRIDES_OFFSET = 0x24C8,
    FIELD_VM_INPUT_EXCLUSION_MASKS_OFFSET = 0x24D0,
    FIELD_VM_INPUT_DIRECTION_ROTATION_OFFSET = 0x24D8,
    FIELD_VM_TRIGGER_AREAS_OFFSET = 0x24F4,
    FIELD_VM_FIELD_SYSTEM_OFFSET = 0x24FC,
    FIELD_VM_AUX_SCRIPT_STATES_OFFSET = 0x26A4,
    FIELD_VM_PAIRED_FIELD_CONTEXT_OFFSET = 0x29D4,
    FIELD_VM_ENTITY_TABLE_OFFSET = 0x29D8,
    FIELD_VM_SPECIAL_RESOURCE_RECORDS_OFFSET = 0x2A68,
    FIELD_VM_PARTY_MANAGER_OFFSET = 0x256C,
    FIELD_VM_MAP_CONTROLLER_OFFSET = 0x2500,
    FIELD_VM_ENTITY_COUNT_OFFSET = 0x2B32,
    FIELD_VM_FIELD_CONTROL_FLAGS_OFFSET = 0x2B30,
    FIELD_VM_CAMERA_TARGET_FLAGS_OFFSET = 0x2B5C,
    FIELD_VM_CAMERA_MOVEMENT_FLAGS_OFFSET = 0x2B90,
    FIELD_VM_BG_SCROLL_CONTROLLERS_OFFSET = 0x2BC4,
    FIELD_VM_SCREEN_WIPE_STATE_OFFSET = 0x2BC8,
    FIELD_VM_PALETTE_CROSSFADE_STATE_OFFSET = 0x2BCC,
    FIELD_VM_RESOURCE_RECORD_SIZE = 24
};

enum FieldVmPartyLayout {
    FIELD_VM_PARTY_CONTROLLER_OFFSET = 0x20,
    FIELD_VM_PARTY_CONTROLLER_SIZE = 0x20A4,
    FIELD_VM_PARTY_CONTROLLER_FLAGS_OFFSET = 0x50,
    FIELD_VM_PARTY_CONTROLLER_CONTROL_FLAGS_OFFSET = 0x74
};

enum FieldVmFieldSystemOffset {
    FIELD_VM_FIELD_SYSTEM_SAVE_LOCATION_OFFSET = 0x25C,
    FIELD_VM_FIELD_SYSTEM_TOUCH_STATE_OFFSET = 0x268,
    FIELD_VM_FIELD_SYSTEM_TIMER_OFFSET = 0x3A0
};

enum FieldVmPairedContextOffset {
    FIELD_VM_PAIRED_ROOM_ID_OFFSET = 0x23F0,
    FIELD_VM_PAIRED_SCRIPT_TABLE_OFFSET = 0x24E0,
    FIELD_VM_PAIRED_SCRIPT_MANAGER_OFFSET = 0x2570,
    FIELD_VM_PAIRED_SCRIPT_FLAGS_OFFSET = 0x29B8,
    FIELD_VM_PAIRED_PARENT_ENTITY_ID_OFFSET = 0x29C1,
    FIELD_VM_PAIRED_QUEUED_SCRIPT_OFFSET = 0x29C8
};

static inline int FieldVm_GetOwnerType(const FieldScriptState *state) {
    return (state->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
        FIELD_SCRIPT_OWNER_TYPE_SHIFT;
}

static inline int FieldVm_GetParentType(const FieldScriptState *state) {
    return (state->flags & FIELD_SCRIPT_PARENT_TYPE_MASK) >>
        FIELD_SCRIPT_PARENT_TYPE_SHIFT;
}

static inline int FieldVm_GetEffectiveParentType(const FieldScriptState *state) {
    int owner_type = (state->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
        FIELD_SCRIPT_OWNER_TYPE_SHIFT;

    if (owner_type == FIELD_SCRIPT_OWNER_ENTITY) {
        return (state->flags & FIELD_SCRIPT_PARENT_TYPE_MASK) >>
            FIELD_SCRIPT_PARENT_TYPE_SHIFT;
    }
    return owner_type;
}

static inline int FieldVm_GetOwnerSubtype(const FieldScriptState *state) {
    return (s8)(state->flags >> FIELD_SCRIPT_OWNER_SUBTYPE_SHIFT);
}

static inline int FieldVm_GetParentEntityId(const FieldScriptState *state) {
    if (((state->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
         FIELD_SCRIPT_OWNER_TYPE_SHIFT) == FIELD_SCRIPT_OWNER_ENTITY) {
        return *(s8 *)((u8 *)state->owner_entity + 4);
    }
    return -1;
}

static inline int FieldVm_GetScriptState(const FieldScriptState *state) {
    if ((state->flags & FIELD_SCRIPT_ACTIVE) == 0) {
        return 0;
    }
    if ((state->flags & FIELD_SCRIPT_PAUSED) != 0) {
        return 3;
    }
    if (((state->flags & FIELD_SCRIPT_RESULT_MASK) >>
         FIELD_SCRIPT_RESULT_SHIFT) == 3) {
        return 2;
    }
    return 1;
}

static inline int FieldVm_RetryCurrentCommand(
    ScriptVm *vm, FieldScriptState *state, u16 opcode) {
    s32 descriptor = (u16)vm->command_descriptors[opcode];
    int halfword_count =
        (descriptor & SCRIPT_VM_ARGUMENT_COUNT_MASK) +
        ((descriptor & SCRIPT_VM_HAS_RESULT) >> 5) +
        ((descriptor & SCRIPT_VM_HAS_ARGUMENT_MODES) >> 6) + 1;

    state->vm_state.script -= halfword_count;
    return SCRIPT_VM_YIELDED;
}

static inline FieldScriptState *FieldVm_GetEntityScript(FieldEntity *entity) {
    return (FieldScriptState *)((u8 *)entity + 0x20);
}

static inline FieldScriptState *FieldVm_GetAuxScript(FieldVmRuntime *runtime,
                                                      int slot) {
    return (FieldScriptState *)(
        runtime->field_context + FIELD_VM_AUX_SCRIPT_STATES_OFFSET) + slot;
}

static inline int FieldVm_VisitMatchingEntityScripts(
    FieldVmRuntime *runtime, FieldScriptState *caller, int selector,
    int operation) {
    FieldEntity **entities = (FieldEntity **)(
        runtime->field_context + FIELD_VM_ENTITY_TABLE_OFFSET);
    int entity_count = *(u8 *)(
        runtime->field_context + FIELD_VM_ENTITY_COUNT_OFFSET);
    int parent_type = selector;
    int parent_entity_id = -1;
    int index;

    if (selector == -1) {
        parent_type = (caller->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
            FIELD_SCRIPT_OWNER_TYPE_SHIFT;
    }
    if (parent_type == FIELD_SCRIPT_OWNER_ENTITY) {
        parent_entity_id = *(s8 *)((u8 *)caller->owner_entity + 4);
    }

    for (index = 0; index < entity_count; index++) {
        FieldEntity *entity = entities[index];
        FieldScriptState *target =
            (FieldScriptState *)((u8 *)entity + 0x20);
        int target_parent_type =
            (target->flags & FIELD_SCRIPT_PARENT_TYPE_MASK) >>
            FIELD_SCRIPT_PARENT_TYPE_SHIFT;

        if ((target->flags & FIELD_SCRIPT_ACTIVE) == 0 ||
            (target_parent_type != parent_type &&
             (parent_type != FIELD_SCRIPT_OWNER_ENTITY ||
              target->parent_entity_id != parent_entity_id))) {
            continue;
        }
        switch (operation) {
        case FIELD_VM_WAIT_MATCHING_ENTITY_SCRIPTS:
            return 1;
        case FIELD_VM_STOP_MATCHING_ENTITY_SCRIPTS:
            entity->vtable->stop_script(entity);
            break;
        case FIELD_VM_PAUSE_MATCHING_ENTITY_SCRIPTS:
            entity->vtable->pause_script(entity);
            break;
        case FIELD_VM_RESUME_MATCHING_ENTITY_SCRIPTS:
            entity->vtable->resume_script(entity);
            break;
        }
    }
    return 0;
}

static inline void FieldVm_StartEntityScript(FieldScriptState *caller,
                                             FieldEntity *entity,
                                             const u16 *script,
                                             int chain_if_active) {
    FieldScriptState *target =
        (FieldScriptState *)((u8 *)entity + 0x20);

    if (chain_if_active && (target->flags & FIELD_SCRIPT_ACTIVE) != 0) {
        int parent_type =
            (caller->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
            FIELD_SCRIPT_OWNER_TYPE_SHIFT;

        if (parent_type == FIELD_SCRIPT_OWNER_ENTITY) {
            parent_type =
                (caller->flags & FIELD_SCRIPT_PARENT_TYPE_MASK) >>
                FIELD_SCRIPT_PARENT_TYPE_SHIFT;
        }
        target->queued_script = script;
        target->flags =
            (target->flags & ~FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK) |
            ((parent_type << FIELD_SCRIPT_QUEUED_PARENT_TYPE_SHIFT) &
             FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK);
        if (((caller->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
             FIELD_SCRIPT_OWNER_TYPE_SHIFT) == FIELD_SCRIPT_OWNER_ENTITY) {
            target->queued_parent_entity_id =
                *(s8 *)((u8 *)caller->owner_entity + 4);
        } else {
            target->queued_parent_entity_id = -1;
        }
        return;
    }
    entity->vtable->stop_script(entity);
    func_ov000_02082240(
        target, caller,
        ((caller->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
         FIELD_SCRIPT_OWNER_TYPE_SHIFT) & 3,
        script);
}

static inline void FieldVm_StartPairedScript(FieldVmRuntime *runtime,
                                             FieldScriptState *caller,
                                             ScriptVmCommand *command) {
    u8 *paired = *(u8 **)(runtime->field_context +
                         FIELD_VM_PAIRED_FIELD_CONTEXT_OFFSET);
    int slot = command->arguments[1];
    u32 *script_flags;

    if (paired == 0 ||
        *(u16 *)(paired + FIELD_VM_PAIRED_ROOM_ID_OFFSET) !=
            (u16)command->arguments[0]) {
        return;
    }

    script_flags = (u32 *)(paired + FIELD_VM_PAIRED_SCRIPT_FLAGS_OFFSET);
    if (command->arguments[2] != 0 &&
        (*script_flags & FIELD_SCRIPT_ACTIVE) != 0) {
        const u16 *script_table;
        const u16 *queued_script = 0;
        int parent_type =
            (caller->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
            FIELD_SCRIPT_OWNER_TYPE_SHIFT;

        if (slot != -1) {
            script_table = *(const u16 **)(
                paired + FIELD_VM_PAIRED_SCRIPT_TABLE_OFFSET);
            queued_script = (const u16 *)(
                (const u8 *)script_table + script_table[9 + (u16)slot]);
        }
        *(const u16 **)(paired + FIELD_VM_PAIRED_QUEUED_SCRIPT_OFFSET) =
            queued_script;
        if (parent_type == FIELD_SCRIPT_OWNER_ENTITY) {
            parent_type =
                (caller->flags & FIELD_SCRIPT_PARENT_TYPE_MASK) >>
                FIELD_SCRIPT_PARENT_TYPE_SHIFT;
        }
        *script_flags =
            (*script_flags & ~FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK) |
            ((parent_type <<
              FIELD_SCRIPT_QUEUED_PARENT_TYPE_SHIFT) &
             FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK);
        if (((caller->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
             FIELD_SCRIPT_OWNER_TYPE_SHIFT) == FIELD_SCRIPT_OWNER_ENTITY) {
            *(s8 *)(paired + FIELD_VM_PAIRED_PARENT_ENTITY_ID_OFFSET) =
                *(s8 *)((u8 *)caller->owner_entity + 4);
        } else {
            *(s8 *)(paired + FIELD_VM_PAIRED_PARENT_ENTITY_ID_OFFSET) = -1;
        }
        return;
    }

    func_ov000_0208911c(
        paired + FIELD_VM_PAIRED_SCRIPT_MANAGER_OFFSET, caller, (s16)slot);
}

static inline FieldRuntimeEntity *FieldVm_GetRuntimeEntity(
    FieldEntity *entity) {
    return (FieldRuntimeEntity *)entity;
}

static inline FieldEntity *FieldVm_GetEntityByIndex(FieldVmRuntime *runtime,
                                                    int entity_index) {
    FieldEntity **entities = (FieldEntity **)(
        runtime->field_context + FIELD_VM_ENTITY_TABLE_OFFSET);

    return entities[entity_index];
}

static inline u8 *FieldVm_GetPartyManager(FieldVmRuntime *runtime) {
    return *(u8 **)(runtime->field_context + FIELD_VM_PARTY_MANAGER_OFFSET);
}

static inline FieldMapController *FieldVm_GetMapController(
    FieldVmRuntime *runtime) {
    return *(FieldMapController **)(
        runtime->field_context + FIELD_VM_MAP_CONTROLLER_OFFSET);
}

static inline u8 *FieldVm_GetPartyController(u8 *party_manager,
                                             int party_side) {
    return party_manager + FIELD_VM_PARTY_CONTROLLER_OFFSET +
        FIELD_VM_PARTY_CONTROLLER_SIZE * party_side;
}

static inline int FieldVm_GetPresentPartyMask(FieldVmRuntime *runtime) {
    return (*(u16 *)(runtime->field_context +
                    FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) >> 2) & 3;
}

static inline int FieldVm_GetActivePartySide(const u8 *party_manager) {
    return *(const u32 *)(party_manager + 4) & 1;
}

static inline int FieldVm_ResolveMenuPartyContext(FieldVmRuntime *runtime,
                                                  int requested_context) {
    u8 *party_manager;
    u8 *party_controller;
    u8 *leader_entity;
    u32 interaction_flags;

    if (requested_context != -1) {
        return requested_context;
    }

    party_manager = *(u8 **)(runtime->field_context +
                             FIELD_VM_PARTY_MANAGER_OFFSET);
    party_controller = party_manager + FIELD_VM_PARTY_CONTROLLER_OFFSET +
        FIELD_VM_PARTY_CONTROLLER_SIZE *
            (*(const u32 *)(party_manager + 4) & 1);
    leader_entity = *(u8 **)(party_controller + 0x48);
    interaction_flags = *(u32 *)(leader_entity + 0x520);
    if (((interaction_flags >> 18) & 0xF) == 6) {
        return 2;
    }
    return (*(u32 *)(party_controller +
                     FIELD_VM_PARTY_CONTROLLER_FLAGS_OFFSET) & 0x200) != 0;
}

static inline int FieldVm_GetPartyControllerProperty(u8 *party_controller,
                                                     int property_id) {
    u32 flags = *(u32 *)(party_controller +
                        FIELD_VM_PARTY_CONTROLLER_FLAGS_OFFSET);

    switch (property_id) {
    case 0:
        return (flags >> 18) & 1;
    case 1: {
        int member = (flags >> 17) & 1;
        u8 *entity = *(u8 **)(party_controller + 0x48 +
                              member * sizeof(void *));

        return *(u8 *)(entity + 0x180);
    }
    case 2:
        return flags >> 20;
    case 3: {
        u8 *entity = *(u8 **)(party_controller + 0x48);

        switch (*(u16 *)(entity + 0x17C)) {
        case 19:
            return 0;
        case 20:
            return 1;
        case 21:
            return (**(u32 **)(entity + 0x59C) & 1) != 0 ? 3 : 2;
        default:
            return -1;
        }
    }
    case 4:
        return (*(u16 *)(party_controller + 0x98) >> 1) & 1;
    default:
        return 0;
    }
}

static inline int FieldVm_IsTwoDimensionalEntity(
    const FieldRuntimeEntity *entity) {
    return ((entity->base.property_00a >> 2) & 0xF) == 8;
}

static inline u32 *FieldVm_GetInteractionFlags(FieldRuntimeEntity *entity) {
    return (u32 *)((u8 *)entity + 0x520);
}

static inline void FieldVm_SetFlag(u32 *flags, u32 mask, int enabled) {
    if (enabled) {
        *flags |= mask;
    } else {
        *flags &= ~mask;
    }
}

static inline u32 FieldVm_MapContactDirectionMask(int script_mask) {
    u32 direction_mask = 0;

    if ((script_mask & 0x01) != 0) direction_mask |= 0x04;
    if ((script_mask & 0x04) != 0) direction_mask |= 0x01;
    if ((script_mask & 0x08) != 0) direction_mask |= 0x02;
    if ((script_mask & 0x02) != 0) direction_mask |= 0x08;
    if ((script_mask & 0x10) != 0) direction_mask |= 0x20;
    if ((script_mask & 0x20) != 0) direction_mask |= 0x10;
    return direction_mask;
}

static inline void FieldVm_SetNavigationBounds(FieldRuntimeEntity *entity,
                                               const s32 *arguments) {
    entity->navigation_min_x = (s16)arguments[1] << FX32B_INT;
    entity->navigation_max_x = entity->navigation_min_x +
        ((s16)arguments[3] << FX32B_INT);
    entity->navigation_max_y = (s16)arguments[2] << FX32B_INT;
    entity->navigation_min_y = entity->navigation_max_y -
        ((s16)arguments[4] << FX32B_INT);
    entity->navigation_vertical_extent =
        (s16)arguments[5] << FX32B_INT;
}

static inline void FieldVm_SetBodyBounds(FieldRuntimeEntity *entity,
                                         const s32 *arguments) {
    entity->body_min_x = (s16)arguments[1] << FX32B_INT;
    entity->body_max_x = entity->body_min_x +
        ((s16)arguments[3] << FX32B_INT);
    entity->body_max_y = (s16)arguments[2] << FX32B_INT;
    entity->body_min_y = entity->body_max_y -
        ((s16)arguments[4] << FX32B_INT);
    entity->body_vertical_extent = (s16)arguments[5] << FX32B_INT;
    entity->body_center_y =
        (entity->body_min_y + entity->body_max_y) / 2;
    entity->body_corner_angles[0] =
        FX_Atan2(entity->body_min_x, entity->body_center_y);
    entity->body_corner_angles[1] =
        FX_Atan2(entity->body_min_x, -entity->body_center_y);
    entity->body_corner_angles[2] =
        FX_Atan2(entity->body_max_x, -entity->body_center_y);
    entity->body_corner_angles[3] =
        FX_Atan2(entity->body_max_x, entity->body_center_y);
}

static inline void FieldVm_SetCollisionPolicy(FieldRuntimeEntity *entity,
                                              int category, int solid,
                                              int can_be_displaced,
                                              int can_displace_other) {
    u64 policy = entity->collision_policy_low |
        ((u64)entity->collision_policy_high << 32);
    u64 shift = 3 * category;
    u64 value = solid != 0;

    value |= (u64)(can_be_displaced != 0) << 1;
    value |= (u64)(can_displace_other != 0) << 2;
    policy = (policy & ~((u64)7 << shift)) | (value << shift);
    entity->collision_policy_low = (u32)policy;
    entity->collision_policy_high = (u32)(policy >> 32);
}

static inline void FieldVm_SetRenderPriorities(FieldRuntimeEntity *entity,
                                               const s32 *arguments) {
    static const u32 automatic_priority_flags[4] = {
        FIELD_ENTITY_AUTO_PRIORITY_0,
        FIELD_ENTITY_AUTO_PRIORITY_1,
        FIELD_ENTITY_AUTO_PRIORITY_2,
        FIELD_ENTITY_AUTO_PRIORITY_3
    };
    int subtype = (entity->base.property_00a >> 2) & 0xF;
    int index;

    if (subtype != 8 && entity->auxiliary_render_object != 0) {
        if ((s8)arguments[5] == -1) {
            entity->runtime_flags |= FIELD_ENTITY_AUTO_AUXILIARY_PRIORITY;
        } else {
            entity->runtime_flags &= ~FIELD_ENTITY_AUTO_AUXILIARY_PRIORITY;
            entity->auxiliary_render_object->overlap_priorities[0] =
                (s8)arguments[5];
            entity->field_state_flags =
                (entity->field_state_flags & ~(3 << 26)) |
                (((u8)arguments[5] & 3) << 26);
        }
    }

    if (entity->render_object == 0) {
        return;
    }
    for (index = 0; index < 4; index++) {
        if (subtype != 8 && (s8)arguments[index + 1] == -1) {
            entity->runtime_flags |= automatic_priority_flags[index];
        } else {
            entity->runtime_flags &= ~automatic_priority_flags[index];
            entity->render_object->overlap_priorities[index] =
                (s8)arguments[index + 1];
        }
    }
}

static inline s32 FieldVm_DecodeWideArgument(
    const ScriptVmCommand *command, int argument_index) {
    if ((command->argument_modes & (1 << argument_index)) != 0) {
        return command->arguments[argument_index];
    }
    return (u16)command->arguments[argument_index] |
        (command->arguments[argument_index + 1] << 16);
}

static inline int FieldVm_DegreesToTurn16(int degrees) {
    return (degrees << 16) / 360;
}

static inline const void *FieldVm_GetResourceRecord(
    FieldVmRuntime *runtime, FieldRuntimeEntity *entity, int resource_index) {
    int resource_set = (entity->base.property_00a >> 6) & 1;
    u8 *context = runtime->field_context;
    int count = *(int *)(context + FIELD_VM_RESOURCE_COUNTS_OFFSET +
                         resource_set * sizeof(int));
    const u8 *records = *(const u8 **)(
        context + FIELD_VM_RESOURCE_RECORDS_OFFSET +
        resource_set * sizeof(void *));

    if (resource_index >= count) {
        return 0;
    }
    return records + resource_index * FIELD_VM_RESOURCE_RECORD_SIZE;
}

int FieldVm_DispatchCommand(ScriptVm *vm, ScriptVmState *base_state,
                            ScriptVmCommand *command) {
    FieldVmRuntime *runtime = (FieldVmRuntime *)vm;
    FieldScriptState *state = (FieldScriptState *)base_state;
    FieldScriptState *target;
    FieldEntity *entity;
    FieldRuntimeEntity *runtime_entity;
    u8 *party_manager;
    u8 *party_controller;
    void *field_system;
    const u16 *script;
    int owner_type;

    switch (command->opcode) {
    case FIELD_VM_BRANCH_IF_OWNER_SUBTYPE:
        owner_type = FieldVm_GetOwnerType(state);
        if ((owner_type == FIELD_SCRIPT_OWNER_PRIMARY ||
             owner_type == FIELD_SCRIPT_OWNER_SECONDARY) &&
            FieldVm_GetOwnerSubtype(state) == command->arguments[0]) {
            state->vm_state.script += command->arguments[1];
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_BRANCH_RELATIVE:
        state->vm_state.script += command->arguments[0];
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_AUX_SCRIPT_ENABLED:
    case FIELD_VM_START_AUX_SCRIPT:
    case FIELD_VM_WAIT_AUX_SCRIPT:
    case FIELD_VM_STOP_AUX_SCRIPT:
    case FIELD_VM_PAUSE_AUX_SCRIPT:
    case FIELD_VM_RESUME_AUX_SCRIPT:
    case FIELD_VM_GET_AUX_SCRIPT_STATE:
        target = FieldVm_GetAuxScript(runtime, command->arguments[0]);
        switch (command->opcode) {
        case FIELD_VM_SET_AUX_SCRIPT_ENABLED:
            target->flags =
                (target->flags & ~FIELD_SCRIPT_ENABLED) |
                (command->arguments[1] & FIELD_SCRIPT_ENABLED);
            break;

        case FIELD_VM_START_AUX_SCRIPT:
            if ((target->flags & FIELD_SCRIPT_ENABLED) != 0) {
                script = state->vm_state.script + command->arguments[1];
                if (command->arguments[2] != 0 &&
                    (target->flags & FIELD_SCRIPT_ACTIVE) != 0) {
                    target->queued_script = script;
                    target->flags =
                        (target->flags &
                         ~FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK) |
                        ((FieldVm_GetEffectiveParentType(state) <<
                          FIELD_SCRIPT_QUEUED_PARENT_TYPE_SHIFT) &
                         FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK);
                    target->queued_parent_entity_id =
                        FieldVm_GetParentEntityId(state);
                } else {
                    func_ov000_02082240(target, state, 0, script);
                }
            }
            break;

        case FIELD_VM_WAIT_AUX_SCRIPT:
            if ((target->flags & FIELD_SCRIPT_ACTIVE) != 0) {
                return FieldVm_RetryCurrentCommand(
                    vm, state, FIELD_VM_WAIT_AUX_SCRIPT);
            }
            break;

        case FIELD_VM_STOP_AUX_SCRIPT:
            target->flags &= ~FIELD_SCRIPT_ACTIVE;
            target->queued_script = 0;
            break;

        case FIELD_VM_PAUSE_AUX_SCRIPT:
            target->flags |= FIELD_SCRIPT_PAUSED;
            break;

        case FIELD_VM_RESUME_AUX_SCRIPT:
            target->flags &= ~FIELD_SCRIPT_PAUSED;
            break;

        case FIELD_VM_GET_AUX_SCRIPT_STATE:
            VM_WriteVariable(
                command->result_variable, FieldVm_GetScriptState(target),
                vm, base_state);
            break;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_INLINE_ENTITY_SCRIPT:
    case FIELD_VM_START_INLINE_ENTITY_SCRIPT_AND_WAIT:
    case FIELD_VM_START_RELATIVE_ENTITY_SCRIPT:
    case FIELD_VM_WAIT_ENTITY_SCRIPT:
    case FIELD_VM_STOP_ENTITY_SCRIPT:
    case FIELD_VM_PAUSE_ENTITY_SCRIPT:
    case FIELD_VM_RESUME_ENTITY_SCRIPT:
    case FIELD_VM_GET_ENTITY_SCRIPT_STATE:
    case FIELD_VM_GET_ENTITY_PROPERTY:
    case FIELD_VM_SET_ENTITY_ENABLED:
    case FIELD_VM_SET_ENTITY_SCRIPT_VALUE:
    case FIELD_VM_GET_ENTITY_SCRIPT_VALUE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        target = FieldVm_GetEntityScript(entity);
        switch (command->opcode) {
        case FIELD_VM_START_INLINE_ENTITY_SCRIPT:
        case FIELD_VM_START_INLINE_ENTITY_SCRIPT_AND_WAIT:
            if ((state->flags & FIELD_SCRIPT_INLINE_WAIT) == 0) {
                FieldVm_StartEntityScript(
                    state, entity, state->vm_state.script,
                    command->arguments[1]);
                if (command->opcode ==
                    FIELD_VM_START_INLINE_ENTITY_SCRIPT_AND_WAIT) {
                    state->flags |= FIELD_SCRIPT_INLINE_WAIT;
                }
            }
            if ((state->flags & FIELD_SCRIPT_INLINE_WAIT) != 0 &&
                (target->flags & FIELD_SCRIPT_ACTIVE) != 0) {
                return FieldVm_RetryCurrentCommand(
                    vm, state,
                    FIELD_VM_START_INLINE_ENTITY_SCRIPT_AND_WAIT);
            }
            state->vm_state.script += command->arguments[2];
            state->flags &= ~FIELD_SCRIPT_INLINE_WAIT;
            break;

        case FIELD_VM_START_RELATIVE_ENTITY_SCRIPT:
            script = state->vm_state.script + command->arguments[1];
            FieldVm_StartEntityScript(
                state, entity, script, command->arguments[2]);
            break;

        case FIELD_VM_WAIT_ENTITY_SCRIPT:
            if ((target->flags & FIELD_SCRIPT_ACTIVE) != 0) {
                return FieldVm_RetryCurrentCommand(
                    vm, state, FIELD_VM_WAIT_ENTITY_SCRIPT);
            }
            break;

        case FIELD_VM_STOP_ENTITY_SCRIPT:
            entity->vtable->stop_script(entity);
            break;

        case FIELD_VM_PAUSE_ENTITY_SCRIPT:
            entity->vtable->pause_script(entity);
            break;

        case FIELD_VM_RESUME_ENTITY_SCRIPT:
            entity->vtable->resume_script(entity);
            break;

        case FIELD_VM_GET_ENTITY_SCRIPT_STATE:
            VM_WriteVariable(
                command->result_variable, FieldVm_GetScriptState(target),
                vm, base_state);
            break;

        case FIELD_VM_GET_ENTITY_PROPERTY:
            VM_WriteVariable(
                command->result_variable,
                entity->vtable->get_property(
                    entity, command->arguments[1]),
                vm, base_state);
            break;

        case FIELD_VM_SET_ENTITY_ENABLED:
            entity->property_00a = (entity->property_00a & ~1) |
                (command->arguments[1] != 0);
            break;

        case FIELD_VM_SET_ENTITY_SCRIPT_VALUE:
            entity->script_values[command->arguments[1]] =
                (s16)command->arguments[2];
            break;

        case FIELD_VM_GET_ENTITY_SCRIPT_VALUE:
            VM_WriteVariable(
                command->result_variable,
                entity->script_values[command->arguments[1]],
                vm, base_state);
            break;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_MATCHING_ENTITY_SCRIPTS:
    case FIELD_VM_STOP_MATCHING_ENTITY_SCRIPTS:
    case FIELD_VM_PAUSE_MATCHING_ENTITY_SCRIPTS:
    case FIELD_VM_RESUME_MATCHING_ENTITY_SCRIPTS:
        if (FieldVm_VisitMatchingEntityScripts(
                runtime, state, command->arguments[0], command->opcode)) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_MATCHING_ENTITY_SCRIPTS);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_PAIRED_FIELD_SCRIPT:
        FieldVm_StartPairedScript(runtime, state, command);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_VISIBLE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->vtable->set_visible(entity, command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_TURN_TO_INTERACTOR_ENABLED:
    case FIELD_VM_SET_ENTITY_GROUND_TRACKING:
    case FIELD_VM_SET_ENTITY_ALTERNATE_COLLISION_FACES_ENABLED:
    case FIELD_VM_SET_ENTITY_NAVIGATION_OBSTACLE_IGNORED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        switch (command->opcode) {
        case FIELD_VM_SET_ENTITY_TURN_TO_INTERACTOR_ENABLED:
            FieldVm_SetFlag(
                &runtime_entity->field_state_flags,
                FIELD_ENTITY_TURN_TO_INTERACTOR, command->arguments[1]);
            break;

        case FIELD_VM_SET_ENTITY_GROUND_TRACKING:
            FieldVm_SetFlag(
                &runtime_entity->field_state_flags,
                FIELD_ENTITY_TRACK_GROUND, command->arguments[1]);
            if (command->arguments[1] != 0) {
                runtime_entity->runtime_flags |=
                    FIELD_ENTITY_VERTICAL_SYNC_DIRTY;
            }
            break;

        case FIELD_VM_SET_ENTITY_ALTERNATE_COLLISION_FACES_ENABLED:
            FieldVm_SetFlag(
                &runtime_entity->runtime_flags,
                FIELD_ENTITY_ALTERNATE_COLLISION_FACES,
                command->arguments[1]);
            break;

        case FIELD_VM_SET_ENTITY_NAVIGATION_OBSTACLE_IGNORED:
            FieldVm_SetFlag(
                &runtime_entity->field_state_flags,
                FIELD_ENTITY_IGNORE_NAVIGATION_OBSTACLE,
                command->arguments[1]);
            break;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_OFFSCREEN_CONTACT_RETENTION_ENABLED:
    case FIELD_VM_SET_ENTITY_RESERVED_STATE_FLAG:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        switch (command->opcode) {
        case FIELD_VM_SET_ENTITY_OFFSCREEN_CONTACT_RETENTION_ENABLED:
            FieldVm_SetFlag(
                &runtime_entity->base_state_flags,
                FIELD_ENTITY_RETAIN_OFFSCREEN_CONTACT,
                command->arguments[1]);
            break;

        case FIELD_VM_SET_ENTITY_RESERVED_STATE_FLAG:
            FieldVm_SetFlag(
                &runtime_entity->base_state_flags,
                FIELD_ENTITY_RESERVED_STATE, command->arguments[1]);
            break;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_LINKED_CONTACT_MODE:
    case FIELD_VM_SET_ENTITY_CONTACT_DIRECTION_FILTER:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        switch (command->opcode) {
        case FIELD_VM_SET_ENTITY_LINKED_CONTACT_MODE:
            runtime_entity->field_state_flags =
                (runtime_entity->field_state_flags &
                 ~FIELD_ENTITY_CONTACT_MODE_MASK) |
                (command->arguments[1] & FIELD_ENTITY_CONTACT_MODE_MASK);
            break;

        case FIELD_VM_SET_ENTITY_CONTACT_DIRECTION_FILTER:
            runtime_entity->contact_direction_flags =
                (runtime_entity->contact_direction_flags & ~0x3F) |
                FieldVm_MapContactDirectionMask(command->arguments[1]);
            break;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_SEMITRANSPARENT:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->render_object->state_flags,
            FIELD_RENDER_SEMITRANSPARENT, command->arguments[1] & 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_CONFIGURE_ENTITY_SHADOW:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->field_state_flags,
            FIELD_ENTITY_SHADOW_ENABLED, command->arguments[1] & 1);
        if (command->arguments[2] == -1) {
            if ((runtime_entity->field_state_flags &
                 FIELD_ENTITY_SHADOW_ENABLED) != 0 &&
                (runtime_entity->field_state_flags &
                 FIELD_ENTITY_SHADOW_STYLE_MASK) == 0) {
                runtime_entity->field_state_flags |=
                    1 << FIELD_ENTITY_SHADOW_STYLE_SHIFT;
            }
        } else {
            runtime_entity->field_state_flags =
                (runtime_entity->field_state_flags &
                 ~FIELD_ENTITY_SHADOW_STYLE_MASK) |
                ((command->arguments[2] & 7) <<
                 FIELD_ENTITY_SHADOW_STYLE_SHIFT);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_MAP_SYNC_AXES:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (command->arguments[1] != -1) {
            FieldVm_SetFlag(
                &runtime_entity->runtime_flags,
                FIELD_ENTITY_SYNC_HORIZONTAL,
                command->arguments[1] & 1);
        }
        if (command->arguments[2] != -1) {
            FieldVm_SetFlag(
                &runtime_entity->runtime_flags,
                FIELD_ENTITY_SYNC_VERTICAL,
                command->arguments[2] & 1);
        }
        if (command->arguments[1] == 1) {
            runtime_entity->runtime_flags |=
                FIELD_ENTITY_HORIZONTAL_SYNC_DIRTY;
        }
        if (command->arguments[2] == 1) {
            runtime_entity->runtime_flags |=
                FIELD_ENTITY_VERTICAL_SYNC_DIRTY;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_NAVIGATION_COLLISION_BOUNDS:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetNavigationBounds(runtime_entity, command->arguments);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_ENTITY_NAVIGATION_COLLISION_BOUNDS:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020ae520(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_COLLISION_RESPONSE_CHANNELS:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->vtable->set_collision_response_channels(
            entity, command->arguments[1], command->arguments[2],
            command->arguments[3], command->arguments[4],
            command->arguments[5]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_COLLISION_RESPONSE_CHANNELS_MASKED:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->vtable->set_collision_response_channels_masked(
            entity, (u16)command->arguments[1], command->arguments[2]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_ENTITY_COLLISION_RESPONSE_CHANNELS:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->vtable->restore_collision_response_channels(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_BODY_COLLISION_BOUNDS:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetBodyBounds(runtime_entity, command->arguments);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_ENTITY_BODY_COLLISION_BOUNDS:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b1394(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_RESERVED_COLLISION_FLAG:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (command->arguments[1] == 0) {
            FieldVm_SetFlag(
                &runtime_entity->collision_state_flags, 1 << 16,
                command->arguments[2]);
        } else if (command->arguments[1] == 1) {
            FieldVm_SetFlag(
                &runtime_entity->collision_state_flags, 1 << 17,
                command->arguments[2]);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_CATEGORY_COLLISION_POLICY:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetCollisionPolicy(
            runtime_entity, command->arguments[1], command->arguments[2],
            command->arguments[3], command->arguments[4]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_RENDER_LAYER:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (runtime_entity->render_object != 0) {
            runtime_entity->render_object->sort_key =
                (runtime_entity->render_object->sort_key & 0x0FFFFFFF) |
                ((u8)command->arguments[1] << 28);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_BODY_COLLISION_ENABLED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->field_state_flags,
            FIELD_ENTITY_BODY_COLLISION_ENABLED,
            command->arguments[1] & 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_RENDER_ORDER_PRIORITIES:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetRenderPriorities(runtime_entity, command->arguments);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_INTERACTION_BOUNDS:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a4f4c(
            entity, (s16)command->arguments[1],
            (s16)command->arguments[2], (u16)command->arguments[3],
            (u16)command->arguments[4], (u16)command->arguments[5]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_LEGACY_NOOP_064:
        /* The outer jump table never enters the dead inner 0x064 handler. */
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_ENTITY_INTERACTION_BOUNDS:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a4e84(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_ANIMATION_SPEED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->animation_speed = (s16)command->arguments[1];
        func_0200940c(
            runtime_entity->render_object, runtime_entity->animation_speed);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_LOCOMOTION_PARAMETERS:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a7410(
            entity, command->arguments[1], command->arguments[2],
            command->arguments[3], -command->arguments[4],
            -command->arguments[5], command->arguments[6]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_DEFAULT_VERTICAL_LAUNCH_VELOCITY:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->default_vertical_launch_velocity =
            FieldVm_DecodeWideArgument(command, 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_DEFAULT_GRAVITY:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->default_gravity =
            FieldVm_DecodeWideArgument(command, 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_TERMINAL_FALL_VELOCITY:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->terminal_fall_velocity =
            FieldVm_DecodeWideArgument(command, 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_SHADOW_SUPPORT_ENABLED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->field_state_flags,
            FIELD_ENTITY_SHADOW_SUPPORT_ENABLED,
            command->arguments[1] & 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_BIND_ENTITY_RESOURCE:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (command->arguments[2] != -1 && command->arguments[4] != 0) {
            runtime_entity->saved_animation_id =
                runtime_entity->animation_id;
            runtime_entity->saved_presentation_flags |=
                FIELD_ENTITY_SAVED_RESOURCE_ANIMATION;
        }
        if (command->arguments[1] == -1) {
            if (command->arguments[2] != -1) {
                func_ov000_020a6c7c(
                    &runtime_entity->base, command->arguments[2], -1);
            }
        } else {
            const void *resource_record = FieldVm_GetResourceRecord(
                runtime, runtime_entity, command->arguments[1]);

            if (command->arguments[4] != 0) {
                runtime_entity->saved_resource_index =
                    runtime_entity->resource_index;
                runtime_entity->saved_presentation_flags |=
                    FIELD_ENTITY_SAVED_RESOURCE;
            }
            runtime_entity->resource_index = (u8)command->arguments[1];
            func_ov000_020a6d68(
                &runtime_entity->base, resource_record);
        }
        if (command->arguments[3] != -1) {
            func_ov000_020bd86c(runtime_entity->render_object);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_ENTITY_RESOURCE_STATE:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((runtime_entity->saved_presentation_flags &
             FIELD_ENTITY_SAVED_RESOURCE) != 0) {
            const void *resource_record;
            u16 special_resource_state =
                *(u16 *)((u8 *)runtime_entity + 0x592);
            int special_resource_index =
                (s8)(special_resource_state >> 6);
            int subtype = (runtime_entity->base.property_00a >> 2) & 0xF;

            runtime_entity->resource_index =
                runtime_entity->saved_resource_index;
            runtime_entity->saved_presentation_flags &=
                ~FIELD_ENTITY_SAVED_RESOURCE;
            if (subtype > 1 || special_resource_index == -1) {
                resource_record = FieldVm_GetResourceRecord(
                    runtime, runtime_entity,
                    runtime_entity->resource_index);
            } else if ((special_resource_state & 0x20) != 0) {
                u8 *context = runtime->field_context;
                int count = *(int *)(
                    context + FIELD_VM_RESOURCE_COUNTS_OFFSET);
                const u8 *records = *(const u8 **)(
                    context + FIELD_VM_RESOURCE_RECORDS_OFFSET);

                if (special_resource_index >= count) {
                    resource_record = 0;
                } else {
                    resource_record = records +
                        special_resource_index *
                            FIELD_VM_RESOURCE_RECORD_SIZE;
                }
            } else {
                const u8 *records = *(const u8 **)(
                    runtime->field_context +
                    FIELD_VM_SPECIAL_RESOURCE_RECORDS_OFFSET);

                resource_record = records + special_resource_index *
                    FIELD_VM_RESOURCE_RECORD_SIZE;
            }
            if ((runtime_entity->saved_presentation_flags &
                 FIELD_ENTITY_SAVED_RESOURCE_ANIMATION) != 0) {
                runtime_entity->saved_presentation_flags &=
                    ~FIELD_ENTITY_SAVED_RESOURCE_ANIMATION;
            }
            func_ov000_020a6d68(
                &runtime_entity->base, resource_record);
        } else if ((runtime_entity->saved_presentation_flags &
                    FIELD_ENTITY_SAVED_RESOURCE_ANIMATION) != 0) {
            runtime_entity->animation_id =
                runtime_entity->saved_animation_id;
            runtime_entity->saved_presentation_flags &=
                ~FIELD_ENTITY_SAVED_RESOURCE_ANIMATION;
            func_ov000_020a6c7c(
                &runtime_entity->base, runtime_entity->animation_id, -1);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ENTITY_ANIMATION:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((runtime_entity->base_state_flags & 1) != 0 &&
            (runtime_entity->render_object->state_flags &
             FIELD_RENDER_ANIMATION_ACTIVE) != 0 &&
            (runtime_entity->render_object->state_flags &
             FIELD_RENDER_ANIMATION_SUPPRESSED) == 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_ENTITY_ANIMATION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_ANIMATION:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (command->arguments[2] != 0) {
            runtime_entity->saved_model_animation =
                runtime_entity->render_object->animation_id;
            runtime_entity->saved_presentation_flags |=
                FIELD_ENTITY_SAVED_ANIMATION;
        }
        runtime_entity->render_object->vtable->set_animation(
            runtime_entity->render_object,
            (u8)runtime_entity->animation_id,
            (s16)command->arguments[1], 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_ENTITY_ANIMATION:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((runtime_entity->saved_presentation_flags &
             FIELD_ENTITY_SAVED_ANIMATION) != 0) {
            runtime_entity->saved_presentation_flags &=
                ~FIELD_ENTITY_SAVED_ANIMATION;
            runtime_entity->render_object->vtable->set_animation(
                runtime_entity->render_object,
                (u8)runtime_entity->animation_id,
                (s16)runtime_entity->saved_model_animation, 1);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_BEHAVIOR_MODE:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (command->arguments[2] != 0) {
            runtime_entity->saved_presentation_flags =
                (runtime_entity->saved_presentation_flags & ~0x1C00) |
                (((runtime_entity->saved_presentation_flags >> 7) & 7)
                 << 10);
            runtime_entity->saved_presentation_flags |=
                FIELD_ENTITY_SAVED_BEHAVIOR;
        }
        func_ov000_020a4df8(
            &runtime_entity->base, command->arguments[1],
            command->opcode - 74);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_ENTITY_BEHAVIOR_STATE:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((runtime_entity->saved_presentation_flags &
             FIELD_ENTITY_SAVED_BEHAVIOR) != 0) {
            func_ov000_020a4df8(
                &runtime_entity->base,
                (runtime_entity->saved_presentation_flags >> 10) & 7,
                (runtime_entity->saved_presentation_flags >> 13) & 1);
            runtime_entity->saved_presentation_flags &=
                ~FIELD_ENTITY_SAVED_BEHAVIOR;
        }
        if ((runtime_entity->render_object->state_flags >> 12) != 0) {
            runtime_entity->render_object->state_flags &= 0xFFFF0FFF;
            runtime_entity->render_object->state_flags &=
                ~FIELD_RENDER_ANIMATION_SUPPRESSED;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_PALETTE_PROFILE:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (command->arguments[2] != 0) {
            runtime_entity->saved_presentation_state =
                runtime_entity->presentation_state;
            runtime_entity->saved_presentation_flags |=
                FIELD_ENTITY_SAVED_PALETTE_PROFILE;
        }
        func_ov000_02076f40(
            runtime->field_context, &runtime_entity->base,
            command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_ENTITY_PALETTE_PROFILE:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((runtime_entity->saved_presentation_flags &
             FIELD_ENTITY_SAVED_PALETTE_PROFILE) != 0) {
            int palette_selector =
                runtime_entity->saved_presentation_state & 0x0F;

            runtime_entity->presentation_state =
                runtime_entity->saved_presentation_state;
            runtime_entity->saved_presentation_flags &=
                ~FIELD_ENTITY_SAVED_PALETTE_PROFILE;
            if ((runtime_entity->saved_presentation_state & 0x80) != 0) {
                palette_selector += 16;
            }
            func_ov000_02076f40(
                runtime->field_context, &runtime_entity->base,
                palette_selector);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_PALETTE_ANIMATION:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (command->arguments[1] == -1) {
            int palette_slot;

            for (palette_slot = 0; palette_slot < 8; palette_slot++) {
                if (runtime_entity->render_object->components[palette_slot]
                    == 0) {
                    command->arguments[1] = palette_slot;
                }
            }
        }
        if (command->arguments[1] != -1) {
            int palette_mode = command->arguments[3] != 0 ? 2 : 0;

            runtime_entity->render_object->vtable->bind_palette_animation(
                runtime_entity->render_object,
                (s8)command->arguments[1],
                (s16)command->arguments[2], 1);
            runtime_entity->render_object->vtable->
                set_palette_animation_mode(
                    runtime_entity->render_object,
                    (s8)command->arguments[1], palette_mode);
            VM_WriteVariable(
                command->result_variable, command->arguments[1], vm,
                base_state);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_ENTITY_PALETTE_ANIMATION:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->render_object->vtable->bind_palette_animation(
            runtime_entity->render_object, (s8)command->arguments[1], -1,
            1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PAUSE_ENTITY_PALETTE_ANIMATION:
    case FIELD_VM_RESUME_ENTITY_PALETTE_ANIMATION:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->render_object->vtable->set_palette_animation_paused(
            runtime_entity->render_object, (s8)command->arguments[1],
            command->opcode == FIELD_VM_PAUSE_ENTITY_PALETTE_ANIMATION);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_REQUEST_ENTITY_BASE_PALETTE_RELOAD:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->render_object->vtable->request_base_palette_reload(
            runtime_entity->render_object);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ENTITY_BASE_PALETTE_RELOAD:
        runtime_entity = FieldVm_GetRuntimeEntity(state->owner_entity);
        if (runtime_entity->render_object->transition != 0 &&
            runtime_entity->render_object->transition_active == 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_ENTITY_BASE_PALETTE_RELOAD);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_SCALING:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a4468(
            entity, command->arguments[1],
            (s16)(command->arguments[2] / 16),
            (s16)(command->arguments[3] / 16),
            (s16)(command->arguments[4] / 16),
            (s16)(command->arguments[5] / 16), 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_TIMED_SCALING:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a439c(
            entity, command->arguments[1],
            (s16)(command->arguments[2] / 16),
            (s16)(command->arguments[3] / 16), command->arguments[4],
            command->arguments[5], 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ENTITY_SCALING:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((runtime_entity->transform_flags & 3) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_ENTITY_SCALING);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_ENTITY_SCALING:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a4360(entity, 0, 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_ROTATION:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a4214(
            entity, command->arguments[1],
            FieldVm_DegreesToTurn16(command->arguments[2]),
            FieldVm_DegreesToTurn16(command->arguments[3]),
            command->arguments[4],
            command->arguments[5] != 0, 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_TIMED_ROTATION:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a412c(
            entity, command->arguments[1],
            FieldVm_DegreesToTurn16(command->arguments[2]),
            command->arguments[3], command->arguments[4],
            command->arguments[5] != 0, 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ENTITY_ROTATION:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((runtime_entity->transform_flags & 4) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_ENTITY_ROTATION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_ENTITY_ROTATION:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a40f8(entity, 0, 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_MOVEMENT:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (FieldVm_IsTwoDimensionalEntity(runtime_entity)) {
            func_ov000_020a64a0(
                &runtime_entity->base, command->arguments[1],
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[5], command->arguments[6],
                command->arguments[7], -command->arguments[8],
                command->arguments[11] != 0, 0);
        } else {
            func_ov000_020b3f00(
                &runtime_entity->base, command->arguments[1],
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[4] << FX32B_INT,
                command->arguments[5], command->arguments[6],
                command->arguments[7], -command->arguments[8],
                command->arguments[9], command->arguments[10],
                command->arguments[11] != 0, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_TIMED_MOVEMENT:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (FieldVm_IsTwoDimensionalEntity(runtime_entity)) {
            func_ov000_020a6394(
                &runtime_entity->base, command->arguments[1],
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[5], command->arguments[8] != 0, 0);
        } else {
            func_ov000_020b3cb0(
                &runtime_entity->base, command->arguments[1],
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[4] << FX32B_INT,
                command->arguments[5], command->arguments[6],
                command->arguments[7], command->arguments[8] != 0, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_MOVEMENT_RELATIVE_TO_ENTITY:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        entity = FieldVm_GetEntityByIndex(runtime, command->arguments[1]);
        if (FieldVm_IsTwoDimensionalEntity(runtime_entity)) {
            func_ov000_020a6260(
                &runtime_entity->base, entity,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[5], command->arguments[8] != 0, 0);
        } else {
            func_ov000_020b3b00(
                &runtime_entity->base, entity,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[4] << FX32B_INT,
                command->arguments[5], command->arguments[6],
                command->arguments[7], command->arguments[8] != 0, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_TIMED_MOVEMENT_RELATIVE_TO_ENTITY:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        entity = FieldVm_GetEntityByIndex(runtime, command->arguments[1]);
        if (FieldVm_IsTwoDimensionalEntity(runtime_entity)) {
            func_ov000_020a61b8(
                &runtime_entity->base, entity,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[5], command->arguments[8] != 0, 0);
        } else {
            func_ov000_020b39a8(
                &runtime_entity->base, entity,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[4] << FX32B_INT,
                command->arguments[5], command->arguments[6],
                command->arguments[7], command->arguments[8] != 0, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_ORBIT_AROUND_POINT:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (FieldVm_IsTwoDimensionalEntity(runtime_entity)) {
            func_ov000_020a59c4(
                &runtime_entity->base,
                (command->arguments[1] >> 3) & 1,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                (command->arguments[1] >> 2) & 1,
                FieldVm_DegreesToTurn16(command->arguments[5]),
                command->arguments[6], command->arguments[7],
                command->arguments[8], -command->arguments[9],
                command->arguments[10], command->arguments[11],
                command->arguments[14] != 0, 0);
        } else {
            func_ov000_020b2e9c(
                &runtime_entity->base,
                (command->arguments[1] >> 3) & 1,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[4] << FX32B_INT,
                (command->arguments[1] >> 2) & 1,
                FieldVm_DegreesToTurn16(command->arguments[5]),
                command->arguments[6], command->arguments[7],
                command->arguments[8], -command->arguments[9],
                command->arguments[1] & 3, command->arguments[10],
                command->arguments[11], command->arguments[12],
                command->arguments[13], command->arguments[14] != 0, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_TIMED_ORBIT_AROUND_POINT:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if (FieldVm_IsTwoDimensionalEntity(runtime_entity)) {
            func_ov000_020a57e4(
                &runtime_entity->base,
                (command->arguments[1] >> 3) & 1,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                (command->arguments[1] >> 2) & 1,
                FieldVm_DegreesToTurn16(command->arguments[5]),
                command->arguments[6], command->arguments[7],
                command->arguments[8], command->arguments[11] != 0, 0);
        } else {
            func_ov000_020b2c08(
                &runtime_entity->base,
                (command->arguments[1] >> 3) & 1,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[4] << FX32B_INT,
                (command->arguments[1] >> 2) & 1,
                FieldVm_DegreesToTurn16(command->arguments[5]),
                command->arguments[6], command->arguments[1] & 3,
                command->arguments[7], command->arguments[8],
                command->arguments[9], command->arguments[10],
                command->arguments[11] != 0, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_ORBIT_AROUND_ENTITY:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        entity = FieldVm_GetEntityByIndex(runtime, command->arguments[1]);
        if (FieldVm_IsTwoDimensionalEntity(runtime_entity)) {
            func_ov000_020a55f0(
                &runtime_entity->base, entity,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                (command->arguments[5] >> 2) & 1,
                FieldVm_DegreesToTurn16(command->arguments[6]),
                command->arguments[7], command->arguments[8],
                command->arguments[9], command->arguments[12] != 0, 0);
        } else {
            func_ov000_020b295c(
                &runtime_entity->base, entity,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[4] << FX32B_INT,
                (command->arguments[5] >> 2) & 1,
                FieldVm_DegreesToTurn16(command->arguments[6]),
                command->arguments[7], command->arguments[5] & 3,
                command->arguments[8], command->arguments[9],
                command->arguments[10], command->arguments[11],
                command->arguments[12] != 0, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_TIMED_ORBIT_AROUND_ENTITY:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        entity = FieldVm_GetEntityByIndex(runtime, command->arguments[1]);
        if (FieldVm_IsTwoDimensionalEntity(runtime_entity)) {
            func_ov000_020a5408(
                &runtime_entity->base, entity,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                (command->arguments[5] >> 2) & 1,
                FieldVm_DegreesToTurn16(command->arguments[6]),
                command->arguments[7], command->arguments[8],
                command->arguments[9], command->arguments[12] != 0, 0);
        } else {
            func_ov000_020b26ac(
                &runtime_entity->base, entity,
                command->arguments[2] << FX32B_INT,
                command->arguments[3] << FX32B_INT,
                command->arguments[4] << FX32B_INT,
                (command->arguments[5] >> 2) & 1,
                FieldVm_DegreesToTurn16(command->arguments[6]),
                command->arguments[7], command->arguments[5] & 3,
                command->arguments[8], command->arguments[9],
                command->arguments[10], command->arguments[11],
                command->arguments[12] != 0, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ENTITY_MOVEMENT:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((runtime_entity->planar_movement_flags & 7) != 0 ||
            (runtime_entity->vertical_controller_flags & 1) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_ENTITY_MOVEMENT);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_CANCEL_ENTITY_MOVEMENT:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->vtable->cancel_planar_movement(entity, 0, 0);
        entity->vtable->cancel_vertical_movement(entity, 0, 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_VERTICAL_MOTION:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b44ac(
            entity, FieldVm_DecodeWideArgument(command, 1),
            FieldVm_DecodeWideArgument(command, 3), -1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_VERTICAL_MOTION_TO_HEIGHT:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b4414(
            entity, command->arguments[1] << FX32B_INT,
            FieldVm_DecodeWideArgument(command, 2), -1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ENTITY_VERTICAL_MOTION:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((runtime_entity->field_state_flags & 0x10) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_ENTITY_VERTICAL_MOTION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_ENTITY_VERTICAL_MOTION:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b4300(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_POSITION:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        command->arguments[2] <<= FX32B_INT;
        command->arguments[3] <<= FX32B_INT;
        if (FieldVm_IsTwoDimensionalEntity(runtime_entity)) {
            if (command->arguments[1] != 0) {
                command->arguments[2] += runtime_entity->position_x;
                command->arguments[3] += runtime_entity->position_y;
            }
            func_ov000_020a6710(
                &runtime_entity->base, command->arguments[2],
                command->arguments[3]);
        } else {
            command->arguments[4] <<= FX32B_INT;
            if (command->arguments[1] != 0) {
                command->arguments[2] += runtime_entity->position_x;
                command->arguments[3] += runtime_entity->position_y;
                command->arguments[4] += runtime_entity->position_z;
            }
            func_ov000_020b42c8(
                &runtime_entity->base, command->arguments[2],
                command->arguments[3], command->arguments[4]);
            if ((*(u16 *)(runtime->field_context +
                         FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 0x8000) != 0 &&
                *(s8 *)(runtime->field_context +
                        FIELD_VM_SPECIAL_PARTY_ENTITY_ID_OFFSET) != -1 &&
                *(s8 *)(runtime->field_context +
                        FIELD_VM_SPECIAL_PARTY_ENTITY_ID_OFFSET) ==
                    *(u8 *)((u8 *)runtime_entity + 4)) {
                func_ov000_020736a4(
                    runtime->field_context, &runtime_entity->base, 0, 0);
            }
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_ROAMING_BOUNDS:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b2020(
            entity, command->arguments[1], command->arguments[2],
            command->arguments[3], command->arguments[4]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_ADD_ENTITY_ROAMING_PROFILE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b1efc(
            entity, state->vm_state.script + command->arguments[1] + 2,
            -1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_ROAMING_BOUNDARY_CLAMP_ENABLED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->roaming_flags, 1, command->arguments[1] & 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_RANDOM_ROAMING:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b1b88(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_ENTITY_RANDOM_ROAMING:
    case FIELD_VM_STOP_ENTITY_WAYPOINT_PATH:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b172c(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PAUSE_ENTITY_RANDOM_ROAMING:
    case FIELD_VM_PAUSE_ENTITY_WAYPOINT_PATH:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b16d4(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESUME_ENTITY_RANDOM_ROAMING:
    case FIELD_VM_RESUME_ENTITY_WAYPOINT_PATH:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b167c(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_CLEAR_ENTITY_ROAMING_PROFILES:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b1e5c(entity, -1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_LOAD_ENTITY_WAYPOINT_PATH:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        {
            const u8 *path_record = (const u8 *)state->vm_state.script +
                2 * command->arguments[1];

            func_ov000_020b1a24(
                entity, path_record + 4,
                (*(const u32 *)path_record - 4) / 2);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ENTITY_WAYPOINT_PATH:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b18e4(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_CLEAR_ENTITY_WAYPOINT_PATH:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b1a08(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_FACING_DIRECTION:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020a6690(
            entity, command->arguments[1], command->arguments[2], 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_FACE_ENTITY_TOWARD_ENTITY:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020b426c(
            entity, FieldVm_GetEntityByIndex(runtime, command->arguments[1]));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SPAWN_ENTITY_EFFECT_SPRITE:
    case FIELD_VM_REMOVE_ENTITY_EFFECT_SPRITE:
    case FIELD_VM_WAIT_ENTITY_EFFECT_SPRITE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        switch (command->opcode) {
        case FIELD_VM_SPAWN_ENTITY_EFFECT_SPRITE:
            func_ov000_020713bc(
                runtime->field_context, entity, -1,
                command->arguments[1], command->arguments[2],
                (s16)command->arguments[3],
                (s16)command->arguments[4], command->arguments[5],
                command->arguments[6] != 0);
            break;

        case FIELD_VM_REMOVE_ENTITY_EFFECT_SPRITE:
            func_ov000_0207138c(runtime->field_context, entity);
            break;

        case FIELD_VM_WAIT_ENTITY_EFFECT_SPRITE:
            if (func_ov000_0207133c(runtime->field_context, entity)) {
                return FieldVm_RetryCurrentCommand(
                    vm, state, FIELD_VM_WAIT_ENTITY_EFFECT_SPRITE);
            }
            break;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_FIELD_BLOCK_IDLE_BOBBING_ENABLED:
    case FIELD_VM_SET_FIELD_BLOCK_BOUNCE_CONTROLLER_ENABLED:
    case FIELD_VM_WAIT_FIELD_BLOCK_BOUNCE:
    case FIELD_VM_SET_ENEMY_JUMP_FIRST_STRIKE_ENABLED:
    case FIELD_VM_SET_ENEMY_SPIKED_JUMP_RESPONSE:
    case FIELD_VM_SET_ENEMY_SPECIAL_CONTACT_REMOVAL_ENABLED:
    case FIELD_VM_SET_ENEMY_IMMEDIATE_BATTLE_REMOVAL_ENABLED:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        runtime_entity = FieldVm_GetRuntimeEntity(entity);
        switch (command->opcode) {
        case FIELD_VM_SET_FIELD_BLOCK_IDLE_BOBBING_ENABLED:
            func_ov000_020bc8e4(entity, command->arguments[1] != 0);
            break;

        case FIELD_VM_SET_FIELD_BLOCK_BOUNCE_CONTROLLER_ENABLED:
            func_ov000_020bc7d0(entity, command->arguments[1] != 0);
            break;

        case FIELD_VM_WAIT_FIELD_BLOCK_BOUNCE:
            if ((s8)(*FieldVm_GetInteractionFlags(runtime_entity) >> 14) !=
                -1) {
                return FieldVm_RetryCurrentCommand(
                    vm, state, FIELD_VM_WAIT_FIELD_BLOCK_BOUNCE);
            }
            break;

        case FIELD_VM_SET_ENEMY_JUMP_FIRST_STRIKE_ENABLED:
            FieldVm_SetFlag(
                FieldVm_GetInteractionFlags(runtime_entity), 1 << 0,
                command->arguments[1] != 0);
            break;

        case FIELD_VM_SET_ENEMY_SPIKED_JUMP_RESPONSE:
            FieldVm_SetFlag(
                FieldVm_GetInteractionFlags(runtime_entity), 1 << 1,
                command->arguments[1] != 0);
            break;

        case FIELD_VM_SET_ENEMY_SPECIAL_CONTACT_REMOVAL_ENABLED:
            FieldVm_SetFlag(
                FieldVm_GetInteractionFlags(runtime_entity), 1 << 2,
                command->arguments[1] != 0);
            break;

        case FIELD_VM_SET_ENEMY_IMMEDIATE_BATTLE_REMOVAL_ENABLED:
            FieldVm_SetFlag(
                FieldVm_GetInteractionFlags(runtime_entity), 1 << 3,
                command->arguments[1] != 0);
            break;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_REJOIN_PARTY_FOLLOWER:
        party_manager = FieldVm_GetPartyManager(runtime);
        if ((FieldVm_GetPresentPartyMask(runtime) &
             (1 << command->arguments[0])) != 0) {
            func_ov000_0209b53c(FieldVm_GetPartyController(
                party_manager, command->arguments[0]));
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_PARTY_FOLLOWER_REJOINED:
        party_manager = FieldVm_GetPartyManager(runtime);
        if ((FieldVm_GetPresentPartyMask(runtime) &
             (1 << command->arguments[0])) != 0) {
            party_controller = FieldVm_GetPartyController(
                party_manager, command->arguments[0]);
            if ((*(u32 *)(party_controller +
                         FIELD_VM_PARTY_CONTROLLER_FLAGS_OFFSET) &
                 0x8000) != 0) {
                return FieldVm_RetryCurrentCommand(
                    vm, state, FIELD_VM_WAIT_PARTY_FOLLOWER_REJOINED);
            }
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_DETACH_PARTY_FOLLOWER:
        party_manager = FieldVm_GetPartyManager(runtime);
        if ((FieldVm_GetPresentPartyMask(runtime) &
             (1 << command->arguments[0])) != 0) {
            func_ov000_0209b0b8(FieldVm_GetPartyController(
                party_manager, command->arguments[0]));
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_REUNITE_SPLIT_PARTIES:
        party_manager = FieldVm_GetPartyManager(runtime);
        if (command->arguments[0] == -1) {
            command->arguments[0] =
                FieldVm_GetActivePartySide(party_manager);
        }
        func_ov000_0209e720(
            party_manager, command->arguments[0],
            command->arguments[1] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_SPLIT_PARTY_REUNION:
        if (func_ov000_0209e6f4(FieldVm_GetPartyManager(runtime))) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_SPLIT_PARTY_REUNION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_BABY_PIGGYBACK_MOUNT:
        func_ov000_0209dfd4(FieldVm_GetPartyManager(runtime));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_BABY_PIGGYBACK_MOUNT:
        if (func_ov000_0209df70(FieldVm_GetPartyManager(runtime))) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_BABY_PIGGYBACK_MOUNT);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_PARTY_MEMBER_CHARACTER_ID:
        party_manager = FieldVm_GetPartyManager(runtime);
        func_ov000_0209ce44(
            FieldVm_GetPartyController(
                party_manager, command->arguments[0] / 2),
            command->arguments[0] & 1, command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESET_PARTY_MEMBER_CHARACTERS:
        party_manager = FieldVm_GetPartyManager(runtime);
        func_ov000_0209ce00(FieldVm_GetPartyController(
            party_manager, command->arguments[0]));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_PARTY_FACING_DIRECTION:
        party_manager = FieldVm_GetPartyManager(runtime);
        func_ov000_0209bc84(
            FieldVm_GetPartyController(
                party_manager, command->arguments[0]),
            command->arguments[1] != 0, command->arguments[2] != 0, 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_PARTY_MEMBER_SWITCHING_ENABLED:
        party_manager = FieldVm_GetPartyManager(runtime);
        party_controller = FieldVm_GetPartyController(
            party_manager, command->arguments[0]);
        FieldVm_SetFlag(
            (u32 *)(party_controller +
                    FIELD_VM_PARTY_CONTROLLER_CONTROL_FLAGS_OFFSET),
            1 << 2, command->arguments[1] & 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_LEGACY_NOOP_0B9:
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_FIELD_PARTY_HUD_LAYOUT:
        if ((*(u16 *)(runtime->field_context +
                     FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1) != 0) {
            func_ov000_02071a38(
                runtime->field_context, command->arguments[0] != 0,
                command->arguments[1] != 0);
        } else {
            func_ov000_02071a38(
                *(u8 **)(runtime->field_context +
                         FIELD_VM_PAIRED_FIELD_CONTEXT_OFFSET),
                command->arguments[0] != 0,
                command->arguments[1] != 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_PARTY_ATTACHED_SPRITE_VISIBLE:
        party_manager = FieldVm_GetPartyManager(runtime);
        func_ov000_02089da8(
            FieldVm_GetPartyController(party_manager, 1),
            command->arguments[0] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_PARTY_FIELD_MODE:
        party_manager = FieldVm_GetPartyManager(runtime);
        if ((FieldVm_GetPresentPartyMask(runtime) &
             (1 << command->arguments[0])) != 0) {
            func_ov000_0209ec48(
                party_manager, command->arguments[0],
                (u16)command->arguments[1], 0, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_CANCEL_PARTY_ACTIONS:
        party_manager = FieldVm_GetPartyManager(runtime);
        if (command->arguments[0] == -1) {
            if ((FieldVm_GetPresentPartyMask(runtime) & 1) != 0) {
                func_ov000_0209e8cc(
                    party_manager, 0, (u16)command->arguments[1]);
            }
            if ((FieldVm_GetPresentPartyMask(runtime) & 2) != 0) {
                func_ov000_0209e8cc(
                    party_manager, 1, (u16)command->arguments[1]);
            }
        } else if ((FieldVm_GetPresentPartyMask(runtime) &
                    (1 << command->arguments[0])) != 0) {
            func_ov000_0209e8cc(
                party_manager, command->arguments[0],
                (u16)command->arguments[1]);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_PARTY_ACTIONS_IDLE:
        party_manager = FieldVm_GetPartyManager(runtime);
        if ((FieldVm_GetPresentPartyMask(runtime) &
             (1 << command->arguments[0])) != 0 &&
            func_ov000_0209ebdc(
                party_manager, command->arguments[0],
                FieldVm_GetPresentPartyMask(runtime))) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_PARTY_ACTIONS_IDLE);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_BABY_HAMMER_SWING:
        party_manager = FieldVm_GetPartyManager(runtime);
        func_ov000_0208f1e0(
            FieldVm_GetPartyController(party_manager, 1),
            command->arguments[0] & 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_BABY_HAMMER_SWING:
        party_manager = FieldVm_GetPartyManager(runtime);
        if (func_ov000_0208efc4(
                FieldVm_GetPartyController(party_manager, 1),
                command->arguments[0] & 1)) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_BABY_HAMMER_SWING);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_FINISH_BABY_HAMMER_SWING:
        party_manager = FieldVm_GetPartyManager(runtime);
        func_ov000_0208efec(
            FieldVm_GetPartyController(party_manager, 1),
            command->arguments[0] & 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PREPARE_BABY_DRILL_MARKER:
        func_ov000_0209de24(
            FieldVm_GetPartyManager(runtime), command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_MOVE_PARTY_TO_GIMMICK_ANCHOR:
        func_ov000_0209dddc(
            FieldVm_GetPartyManager(runtime), command->arguments[0],
            command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PREPARE_BROS_BALL_CANNON_LAUNCH:
        func_ov000_0209ddcc(FieldVm_GetPartyManager(runtime));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_LAUNCH_BROS_BALL_FROM_CANNON:
        func_ov000_0209ddbc(
            FieldVm_GetPartyManager(runtime), command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_LAUNCH_BABY_PARTY_TO_ELEVATION:
        func_ov000_0209dda4(
            FieldVm_GetPartyManager(runtime),
            command->arguments[0] << FX32B_INT);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_DROP_BABY_PARTY_FROM_ELEVATION:
        func_ov000_0209dd8c(FieldVm_GetPartyManager(runtime));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_COMPLETE_AIRBORNE_BABY_TRANSFERS:
        func_ov000_0209e1a8(FieldVm_GetPartyManager(runtime));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_GET_PARTY_CONTROLLER_PROPERTY:
        party_manager = FieldVm_GetPartyManager(runtime);
        VM_WriteVariable(
            command->result_variable,
            FieldVm_GetPartyControllerProperty(
                FieldVm_GetPartyController(
                    party_manager, command->arguments[0]),
                command->arguments[1]),
            vm, base_state);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SWITCH_ACTIVE_PARTY:
        party_manager = FieldVm_GetPartyManager(runtime);
        if (command->arguments[0] == -1) {
            command->arguments[0] =
                !FieldVm_GetActivePartySide(party_manager);
        }
        func_ov000_020a0c30(party_manager, command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ACTIVE_PARTY_SWITCH:
        party_manager = FieldVm_GetPartyManager(runtime);
        if ((*(u32 *)(party_manager + 4) & 2) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_ACTIVE_PARTY_SWITCH);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_PARTY_PIGGYBACK_STATE:
        party_manager = FieldVm_GetPartyManager(runtime);
        FieldVm_SetFlag(
            (u32 *)(party_manager + 4), 1 << 7,
            command->arguments[0] & 1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RELEASE_ROOM_TRANSITION_CONTROL_LOCK:
        *(u16 *)(runtime->field_context +
                 FIELD_VM_FIELD_CONTROL_FLAGS_OFFSET) &= ~8;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_PARTY_LEADER_ANIMATION_OVERRIDE:
        party_manager = FieldVm_GetPartyManager(runtime);
        party_controller = FieldVm_GetPartyController(
            party_manager, command->arguments[0]);
        if (command->arguments[1] != 0) {
            func_ov000_0209cbfc(party_controller, 0);
        } else {
            func_ov000_0209cb90(party_controller, 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_CHANGE_FIELD_ROOM_FOR_PARTY:
        party_manager = FieldVm_GetPartyManager(runtime);
        if (command->arguments[0] == -1) {
            int active_side = FieldVm_GetActivePartySide(party_manager);
            u8 *active_controller = FieldVm_GetPartyController(
                party_manager, active_side);
            int controller_screen =
                (*(u32 *)(active_controller +
                          FIELD_VM_PARTY_CONTROLLER_FLAGS_OFFSET) >> 7) & 1;
            int field_screen =
                *(u16 *)(runtime->field_context +
                         FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1;

            if (field_screen == controller_screen) {
                command->arguments[0] = active_side;
            }
        }
        func_ov000_0207fd54(
            runtime->field_context, command->arguments[0],
            command->arguments[1], (s16)command->arguments[2],
            (s16)command->arguments[3], (s16)command->arguments[4],
            command->arguments[5], command->arguments[6] != 0,
            (s16)command->arguments[7], command->arguments[8], 0,
            command->arguments[9] != 0, -1, 1);
        return SCRIPT_VM_RETURNED;

    case FIELD_VM_CHANGE_FIELD_ROOM_FOR_BOTH_PARTIES:
        func_ov000_0207fac8(
            runtime->field_context, command->arguments[0],
            (s16)command->arguments[1], (s16)command->arguments[2],
            (s16)command->arguments[3], command->arguments[4],
            (s16)command->arguments[5], (s16)command->arguments[6],
            (s16)command->arguments[7], command->arguments[8],
            (s16)command->arguments[9], command->arguments[10], 0,
            command->arguments[11] != 0, -1, 1);
        return SCRIPT_VM_RETURNED;

    case FIELD_VM_CHANGE_FIELD_ROOM:
        func_ov000_0207fa18(
            runtime->field_context, command->arguments[0],
            (s16)command->arguments[1], command->arguments[2], 0);
        return SCRIPT_VM_RETURNED;

    case FIELD_VM_TRANSFER_PARTY_BETWEEN_FIELD_SCREENS:
        party_manager = FieldVm_GetPartyManager(runtime);
        if (command->arguments[0] == -1) {
            command->arguments[0] =
                FieldVm_GetActivePartySide(party_manager);
        }
        party_controller = FieldVm_GetPartyController(
            party_manager, command->arguments[0]);
        if ((*(u16 *)(runtime->field_context +
                     FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1) ==
            ((*(u32 *)(party_controller +
                       FIELD_VM_PARTY_CONTROLLER_FLAGS_OFFSET) >> 7) & 1)) {
            func_ov000_0209f644(
                party_manager, command->arguments[0],
                command->arguments[1], command->arguments[2],
                command->arguments[3], command->arguments[4],
                command->arguments[5], command->arguments[6] != 0,
                (s16)command->arguments[7], command->arguments[8],
                command->arguments[9] != 0,
                command->arguments[10] != 0);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_ENSURE_PARTY_IN_CURRENT_FIELD:
        party_manager = FieldVm_GetPartyManager(runtime);
        if (command->arguments[0] == -1) {
            command->arguments[0] =
                !FieldVm_GetActivePartySide(party_manager);
        }
        func_ov000_0207f288(
            runtime->field_context, command->arguments[0],
            (s16)command->arguments[1], (s16)command->arguments[2],
            (s16)command->arguments[3], command->arguments[4]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_ACTIVATE_FIELD_MAP_EVENT:
        func_ov000_0207af94(
            runtime->field_context, command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_MAP_TILE_ANIMATION:
        FieldVm_GetMapController(runtime)->vtable->start_tile_animation(
            FieldVm_GetMapController(runtime),
            (u16)command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_MAP_TILE_ANIMATION:
        if (*(u8 *)((u8 *)FieldVm_GetMapController(runtime) + 0x670 +
                    12 * command->arguments[0]) == 1) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_MAP_TILE_ANIMATION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PAUSE_MAP_TILE_ANIMATION:
        FieldVm_GetMapController(runtime)->vtable->pause_tile_animation(
            FieldVm_GetMapController(runtime),
            (u16)command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESUME_MAP_TILE_ANIMATION:
        FieldVm_GetMapController(runtime)->vtable->resume_tile_animation(
            FieldVm_GetMapController(runtime),
            (u16)command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_FIELD_BG_LAYERS_ENABLED: {
        int screen = *(u16 *)(runtime->field_context +
                             FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1;
        volatile u32 *display_control =
            (volatile u32 *)(screen != 0 ? 0x04001000 : 0x04000000);
        u32 layer_mask = (*display_control >> 8) & 0xF;
        int layer;

        for (layer = 0; layer < 4; layer++) {
            if (command->arguments[layer] == 1) {
                layer_mask |= 1 << layer;
            } else if (command->arguments[layer] == 0) {
                layer_mask &= ~(1 << layer);
            }
        }
        *display_control = (*display_control & 0xFFFFE0FF) |
            ((layer_mask | 0x10) << 8);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_START_CAMERA_PROFILED_MOVEMENT:
        func_ov000_02073498(
            runtime->field_context, command->arguments[0],
            command->arguments[1] << FX32B_INT,
            command->arguments[2] << FX32B_INT,
            command->arguments[3], command->arguments[4],
            command->arguments[5], -command->arguments[6],
            command->arguments[7] != 0,
            command->arguments[8] != 0, 0);
        *(u16 *)(runtime->field_context +
                 FIELD_VM_CAMERA_TARGET_FLAGS_OFFSET) &= ~3;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_CAMERA_TIMED_MOVEMENT:
        func_ov000_020732f4(
            runtime->field_context, command->arguments[0],
            command->arguments[1] << FX32B_INT,
            command->arguments[2] << FX32B_INT,
            command->arguments[3], command->arguments[4] != 0,
            command->arguments[5] != 0, 0);
        *(u16 *)(runtime->field_context +
                 FIELD_VM_CAMERA_TARGET_FLAGS_OFFSET) &= ~3;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_CAMERA_PROFILED_ENTITY_TRACKING:
        func_ov000_020731bc(
            runtime->field_context,
            FieldVm_GetEntityByIndex(runtime, command->arguments[0]),
            command->arguments[1] << FX32B_INT,
            command->arguments[2] << FX32B_INT,
            command->arguments[3], command->arguments[4] != 0,
            command->arguments[5] != 0, 0);
        *(u16 *)(runtime->field_context +
                 FIELD_VM_CAMERA_TARGET_FLAGS_OFFSET) &= ~3;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_CAMERA_TIMED_ENTITY_TRACKING:
        func_ov000_02073110(
            runtime->field_context,
            FieldVm_GetEntityByIndex(runtime, command->arguments[0]),
            command->arguments[1] << FX32B_INT,
            command->arguments[2] << FX32B_INT,
            command->arguments[3], command->arguments[4] != 0,
            command->arguments[5] != 0, 0);
        *(u16 *)(runtime->field_context +
                 FIELD_VM_CAMERA_TARGET_FLAGS_OFFSET) &= ~3;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_CAMERA_MOVEMENT:
        if ((*(u16 *)(runtime->field_context +
                     FIELD_VM_CAMERA_MOVEMENT_FLAGS_OFFSET) & 3) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_CAMERA_MOVEMENT);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_CAMERA_MOVEMENT:
        func_ov000_02072a58(runtime->field_context, 0, 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_BG_LAYER_PROFILED_SCROLL:
        func_ov000_020727fc(
            runtime->field_context, command->arguments[0],
            command->arguments[1] << FX32B_INT,
            command->arguments[2] << FX32B_INT,
            command->arguments[3], command->arguments[4],
            command->arguments[5], -command->arguments[6],
            command->arguments[7] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_BG_LAYER_TIMED_SCROLL:
        func_ov000_02072660(
            runtime->field_context, command->arguments[0],
            command->arguments[1] << FX32B_INT,
            command->arguments[2] << FX32B_INT,
            command->arguments[3], command->arguments[4] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_BG_LAYER_SCROLL: {
        u8 *controllers = *(u8 **)(
            runtime->field_context + FIELD_VM_BG_SCROLL_CONTROLLERS_OFFSET);

        if ((*(u16 *)(controllers + 56 * command->arguments[0]) & 3) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_BG_LAYER_SCROLL);
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_STOP_BG_LAYER_SCROLL:
        func_ov000_020721c0(
            runtime->field_context, command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_CAMERA_SHAKE:
        func_ov000_02072074(
            runtime->field_context, command->arguments[0],
            command->arguments[1] << FX32B_INT, command->arguments[2],
            command->arguments[3], command->arguments[4]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_CAMERA_SHAKE:
        if ((*(u16 *)(runtime->field_context +
                     FIELD_VM_CAMERA_SHAKE_FLAGS_OFFSET) & 1) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_CAMERA_SHAKE);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_CAMERA_SHAKE:
        func_ov000_02071fec(runtime->field_context);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_MAP_PALETTE_ANIMATION:
        FieldVm_GetMapController(runtime)->vtable->start_palette_animation(
            FieldVm_GetMapController(runtime),
            (u8)command->arguments[0],
            command->arguments[1] != 0 ? 0x10 : 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESET_ALL_MAP_PALETTE_ANIMATIONS:
        FieldVm_GetMapController(runtime)->vtable->reset_palette_animations(
            FieldVm_GetMapController(runtime));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_MAP_PALETTE_ANIMATION:
        FieldVm_GetMapController(runtime)->vtable->stop_palette_animation(
            FieldVm_GetMapController(runtime),
            (u8)command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESUME_MAP_PALETTE_ANIMATION:
        FieldVm_GetMapController(runtime)->vtable->resume_palette_animation(
            FieldVm_GetMapController(runtime),
            (u8)command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_MAP_BASE_PALETTES:
        FieldVm_GetMapController(runtime)->vtable->restore_base_palettes(
            FieldVm_GetMapController(runtime));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_MAP_PALETTE_ANIMATIONS:
        if (FieldVm_GetMapController(runtime)->vtable->
            are_palette_animations_active(FieldVm_GetMapController(runtime))) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_MAP_PALETTE_ANIMATIONS);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_MASTER_BRIGHTNESS_TRANSITION: {
        u16 field_flags;

        func_ov000_02075c34(
            runtime->field_context, (s16)command->arguments[0],
            (s16)command->arguments[1], (u16)command->arguments[2]);
        if ((*(u16 *)(runtime->field_context +
                     FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1) == 0) {
            func_ov000_0207c098(runtime->field_context);
        }
        field_flags = *(u16 *)(runtime->field_context +
                              FIELD_VM_FIELD_CONTROL_FLAGS_OFFSET);
        if ((field_flags & 8) != 0) {
            party_manager = FieldVm_GetPartyManager(runtime);
            func_ov000_02093c98(FieldVm_GetPartyController(
                party_manager, (field_flags >> 4) & 1));
            *(u16 *)(runtime->field_context +
                     FIELD_VM_FIELD_CONTROL_FLAGS_OFFSET) &= ~8;
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_WAIT_MASTER_BRIGHTNESS_TRANSITION:
        if (*(u16 *)(runtime->field_context +
                    FIELD_VM_BRIGHTNESS_FRAMES_OFFSET) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_MASTER_BRIGHTNESS_TRANSITION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PAUSE_MASTER_BRIGHTNESS_TRANSITION:
        if ((*(u8 *)(runtime->field_context +
                    FIELD_VM_BRIGHTNESS_FLAGS_OFFSET) & 1) != 0) {
            *(u8 *)(runtime->field_context +
                    FIELD_VM_BRIGHTNESS_FLAGS_OFFSET) |= 2;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESUME_MASTER_BRIGHTNESS_TRANSITION:
        if ((*(u8 *)(runtime->field_context +
                    FIELD_VM_BRIGHTNESS_FLAGS_OFFSET) & 1) != 0) {
            *(u8 *)(runtime->field_context +
                    FIELD_VM_BRIGHTNESS_FLAGS_OFFSET) &= ~2;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_REVERSE_MASTER_BRIGHTNESS_TRANSITION:
        if ((*(u8 *)(runtime->field_context +
                    FIELD_VM_BRIGHTNESS_FLAGS_OFFSET) & 1) != 0) {
            func_ov000_02075bc8(runtime->field_context);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_FIELD_PALETTE_ANIMATION:
        func_ov000_02075814(
            runtime->field_context, command->arguments[0],
            command->arguments[1], (u16)command->arguments[2],
            (u16)command->arguments[3], (u16)command->arguments[4],
            (u16)command->arguments[5], (u16)command->arguments[6],
            (u16)command->arguments[7], (s16)command->arguments[8],
            (u16)(command->arguments[9] |
                  (command->arguments[10] << 5) |
                  (command->arguments[11] << 10)));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_FIELD_PALETTE_ANIMATION:
        if (command->arguments[0] == -1) {
            int animation_slot;

            for (animation_slot = 0; animation_slot < 2;
                 animation_slot++) {
                if (func_ov000_02075790(
                        runtime->field_context, animation_slot)) {
                    return FieldVm_RetryCurrentCommand(
                        vm, state, FIELD_VM_WAIT_FIELD_PALETTE_ANIMATION);
                }
            }
        } else if (func_ov000_02075790(
                       runtime->field_context, command->arguments[0])) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_FIELD_PALETTE_ANIMATION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PAUSE_FIELD_PALETTE_ANIMATION:
    case FIELD_VM_RESUME_FIELD_PALETTE_ANIMATION: {
        int paused =
            command->opcode == FIELD_VM_PAUSE_FIELD_PALETTE_ANIMATION;

        if (command->arguments[0] == -1) {
            int animation_slot;

            for (animation_slot = 0; animation_slot < 2;
                 animation_slot++) {
                func_ov000_02075730(
                    runtime->field_context, animation_slot, paused);
            }
        } else {
            func_ov000_02075730(
                runtime->field_context, command->arguments[0], paused);
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_REVERSE_FIELD_PALETTE_ANIMATION:
        if (command->arguments[0] == -1) {
            int animation_slot;

            for (animation_slot = 0; animation_slot < 2;
                 animation_slot++) {
                func_ov000_020756ac(
                    runtime->field_context, animation_slot);
            }
        } else {
            func_ov000_020756ac(
                runtime->field_context, command->arguments[0]);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_PALETTE_BANK_CROSSFADE:
        func_ov000_02075600(
            runtime->field_context, command->arguments[0],
            command->arguments[1], command->arguments[2],
            command->arguments[3]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_PALETTE_BANK_CROSSFADE:
        if ((**(u32 **)(runtime->field_context +
                       FIELD_VM_PALETTE_CROSSFADE_STATE_OFFSET) & 1) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_PALETTE_BANK_CROSSFADE);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_PATTERN_SCREEN_WIPE:
        func_ov000_02074810(
            runtime->field_context, command->arguments[0], 0);
        if ((*(u16 *)(runtime->field_context +
                     FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1) == 0 &&
            (command->arguments[0] == 0 ||
             command->arguments[0] == 2)) {
            func_ov000_0207c098(runtime->field_context);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_CIRCULAR_SCREEN_WIPE: {
        FieldCircularWipeParameters parameters;

        parameters.duration_and_retain_mask =
            (command->arguments[0] & 0x7FFF) |
            ((command->arguments[5] & 1) << 15);
        parameters.center_x = (u8)command->arguments[1];
        parameters.center_y = (u8)command->arguments[2];
        parameters.start_radius = (s16)command->arguments[3];
        parameters.end_radius = (s16)command->arguments[4];
        func_ov000_02074810(runtime->field_context, 4, &parameters);
        if ((*(u16 *)(runtime->field_context +
                     FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1) == 0 &&
            command->arguments[3] == 0) {
            func_ov000_0207c098(runtime->field_context);
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_START_RECTANGULAR_SCREEN_WIPE: {
        FieldRectangularWipeParameters parameters;

        parameters.duration_and_retain_mask =
            (command->arguments[0] & 0x7FFF) |
            ((command->arguments[11] & 1) << 15);
        parameters.center_x = (u8)command->arguments[1];
        parameters.center_y = (u8)command->arguments[2];
        parameters.start_left = (s16)command->arguments[3];
        parameters.start_right = (s16)command->arguments[4];
        parameters.start_top = (s16)command->arguments[5];
        parameters.start_bottom = (s16)command->arguments[6];
        parameters.end_left = (s16)command->arguments[7];
        parameters.end_right = (s16)command->arguments[8];
        parameters.end_top = (s16)command->arguments[9];
        parameters.end_bottom = (s16)command->arguments[10];
        func_ov000_02074810(runtime->field_context, 5, &parameters);
        if ((*(u16 *)(runtime->field_context +
                     FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1) == 0 &&
            command->arguments[3] == 0 && command->arguments[4] == 0 &&
            command->arguments[5] == 0 && command->arguments[6] == 0) {
            func_ov000_0207c098(runtime->field_context);
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_WAIT_SCREEN_WIPE:
        if ((**(u32 **)(runtime->field_context +
                       FIELD_VM_SCREEN_WIPE_STATE_OFFSET) & 1) != 0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_SCREEN_WIPE);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_FIELD_SCREEN_ALPHA_BLEND: {
        int screen = *(u16 *)(runtime->field_context +
                             FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1;
        volatile u32 *blend_control =
            (volatile u32 *)(screen != 0 ? 0x04001050 : 0x04000050);

        func_02036988(
            blend_control, command->arguments[0], command->arguments[1],
            command->arguments[2], command->arguments[3]);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_START_TIME_HOLE_DEPARTURE:
        func_ov000_02069d38(
            *(void **)(runtime->field_context + FIELD_VM_FIELD_SYSTEM_OFFSET),
            command->arguments, command->arguments[4],
            command->arguments[5], command->arguments[6],
            command->arguments[7], command->arguments[8],
            command->arguments[9], command->arguments[10]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PREPARE_TIME_HOLE_ARRIVAL:
        func_ov000_02069b24(
            *(void **)(runtime->field_context + FIELD_VM_FIELD_SYSTEM_OFFSET),
            command->arguments, command->arguments[4],
            &command->arguments[5]);
        func_ov000_0207c098(runtime->field_context);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_PREPARED_TIME_HOLE_ARRIVAL:
        func_ov000_02069aac(
            *(void **)(runtime->field_context + FIELD_VM_FIELD_SYSTEM_OFFSET));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_TIME_HOLE_TUNNEL:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_02069284(
            field_system, command->arguments, command->arguments[4],
            command->arguments[5], 1, 0, command->arguments[6] != 0,
            (s16)command->arguments[7], (s16)command->arguments[8]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_FIELD_VERTICAL_ACCELERATING_SCROLL:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_0206900c(
            field_system, command->arguments[6],
            FieldVm_DecodeWideArgument(command, 0),
            FieldVm_DecodeWideArgument(command, 2),
            FieldVm_DecodeWideArgument(command, 4));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_DECELERATE_FIELD_VERTICAL_SCROLL:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_02068fc4(
            field_system, -FieldVm_DecodeWideArgument(command, 0));
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_FIELD_VERTICAL_SCROLL:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        if (func_ov000_02068fac(field_system)) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_FIELD_VERTICAL_SCROLL);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_BEGIN_TIME_HOLE_TUNNEL_WHITEOUT:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        *(u32 *)((u8 *)field_system + 0x274) |= 0x40000;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_TIME_HOLE_TRANSITION:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        if (func_ov000_02066c50(field_system)) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_TIME_HOLE_TRANSITION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_ANCHORED_FIELD_EFFECT_ANIMATION:
        func_ov000_02074e14(
            runtime->field_context, runtime->screen_selector & 1,
            command->arguments[0], command->arguments[1],
            command->arguments[2], command->arguments[3],
            command->arguments[4], (s16)command->arguments[5],
            (s16)command->arguments[6], (s16)command->arguments[7],
            command->arguments[8], (s16)command->arguments[9]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ANCHORED_FIELD_EFFECT_ANIMATION:
        if (func_ov000_02074dd0(runtime->field_context)) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_ANCHORED_FIELD_EFFECT_ANIMATION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_FIELD_EFFECT_ANIMATION:
        func_ov000_02074d8c(
            runtime->field_context, command->arguments[0],
            runtime->screen_selector & 1, command->arguments[1],
            (s16)command->arguments[2], (s16)command->arguments[3],
            (s16)command->arguments[4]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_FIELD_EFFECT_ANIMATION:
        if (func_ov000_02074d50(
                runtime->field_context, command->arguments[0])) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_FIELD_EFFECT_ANIMATION);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RETURN_TO_TITLE_SCREEN:
        func_020052b0(16, 1);
        return SCRIPT_VM_FINISHED;

    case FIELD_VM_LEGACY_NOOP_10C:
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_PAIRED_FIELD_READY: {
        u8 *paired_context = *(u8 **)(
            runtime->field_context + FIELD_VM_PAIRED_FIELD_CONTEXT_OFFSET);

        if (paired_context == 0 ||
            *(u16 *)(paired_context + FIELD_VM_PAIRED_ROOM_ID_OFFSET) != 71) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_PAIRED_FIELD_READY);
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_ENABLE_FIELD_TRIGGER_AREA:
    case FIELD_VM_DISABLE_FIELD_TRIGGER_AREA: {
        u8 *trigger_areas = *(u8 **)(
            runtime->field_context + FIELD_VM_TRIGGER_AREAS_OFFSET);
        u32 *trigger_flags = (u32 *)(
            trigger_areas + 44 * command->arguments[0] + 40);

        FieldVm_SetFlag(
            trigger_flags, 1,
            command->opcode == FIELD_VM_DISABLE_FIELD_TRIGGER_AREA);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_APPLY_SAVED_PARTY_CONFIGURATION: {
        u32 *ownership_masks = (u32 *)(
            runtime->field_context + FIELD_VM_PARTY_OWNERSHIP_MASKS_OFFSET);
        u16 *field_flags = (u16 *)(
            runtime->field_context + FIELD_VM_SPECIAL_PARTY_STATE_OFFSET);

        party_manager = FieldVm_GetPartyManager(runtime);
        func_ov000_020a23f8(party_manager, 0);
        if (VM_ReadVariable(0x2005, 0, 0) ||
            VM_ReadVariable(0x2006, 0, 0)) {
            *ownership_masks = (*ownership_masks | 3) & ~0xC;
            *field_flags = (*field_flags & 0xFFF3) | 4;
        } else if (VM_ReadVariable(0x2007, 0, 0)) {
            *ownership_masks = (*ownership_masks | 0xC) & ~3;
            *field_flags = (*field_flags & 0xFFF3) | 8;
        } else {
            *ownership_masks |= 0xF;
            *field_flags = (*field_flags & 0xFFF3) | 0xC;
        }
        func_ov000_02079d74(runtime->field_context, 3);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_SET_FIELD_INPUT_DISABLE_MASK:
    case FIELD_VM_SET_FIELD_EVENT_INPUT_DISABLE_MASK: {
        u8 *target_context = runtime->field_context;
        int screen = *(u16 *)(target_context +
                             FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1;
        u32 mask_offset = command->opcode ==
            FIELD_VM_SET_FIELD_INPUT_DISABLE_MASK
                ? FIELD_VM_INPUT_DISABLE_MASK_OFFSET
                : FIELD_VM_EVENT_INPUT_DISABLE_MASK_OFFSET;
        u16 previous_mask;

        if (command->arguments[0] == -1) {
            command->arguments[0] = screen;
        }
        if (command->arguments[0] != screen) {
            target_context = *(u8 **)(
                target_context + FIELD_VM_PAIRED_FIELD_CONTEXT_OFFSET);
        }
        previous_mask = *(u16 *)(target_context + mask_offset);
        *(u16 *)(target_context + mask_offset) =
            ~(u16)command->arguments[1];
        VM_WriteVariable(
            command->result_variable, previous_mask ^ 0xFFFF, vm,
            base_state);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_SET_FIELD_INPUT_CHANNEL_OVERRIDE:
        *(u16 *)(runtime->field_context + FIELD_VM_INPUT_OVERRIDES_OFFSET +
                 2 * command->arguments[0]) = (u16)command->arguments[1];
        *(u16 *)(runtime->field_context +
                 FIELD_VM_INPUT_EXCLUSION_MASKS_OFFSET +
                 2 * command->arguments[0]) = ~(u16)command->arguments[2];
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESET_FIELD_INPUT_CHANNEL_OVERRIDES: {
        int channel;

        for (channel = 0; channel < 4; channel++) {
            *(u16 *)(runtime->field_context +
                     FIELD_VM_INPUT_OVERRIDES_OFFSET + 2 * channel) = 0;
            *(u16 *)(runtime->field_context +
                     FIELD_VM_INPUT_EXCLUSION_MASKS_OFFSET + 2 * channel) =
                0xFFFF;
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_SET_FIELD_INPUT_DIRECTION_ROTATION:
        *(u16 *)(runtime->field_context +
                 FIELD_VM_INPUT_DIRECTION_ROTATION_OFFSET) =
            (*(u16 *)(runtime->field_context +
                      FIELD_VM_INPUT_DIRECTION_ROTATION_OFFSET) & ~3) |
            (command->arguments[0] & 3);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_FIELD_PARTY_CONTROL_ENABLED:
        *(u16 *)(runtime->field_context +
                 FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) =
            (*(u16 *)(runtime->field_context +
                      FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & ~2) |
            ((command->arguments[0] & 1) << 1);
        if (command->arguments[1] != -1) {
            int selected_party_side = (command->arguments[1] >> 1) & 1;
            int current_screen = *(u16 *)(
                runtime->field_context +
                FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1;

            party_manager = FieldVm_GetPartyManager(runtime);
            party_controller = FieldVm_GetPartyController(
                party_manager, selected_party_side);
            if (current_screen ==
                ((*(u32 *)(party_controller +
                           FIELD_VM_PARTY_CONTROLLER_FLAGS_OFFSET) >> 7) & 1)) {
                u32 *party_0_flags = (u32 *)(
                    FieldVm_GetPartyController(party_manager, 0) +
                    FIELD_VM_PARTY_CONTROLLER_FLAGS_OFFSET);

                *party_0_flags = (*party_0_flags & ~(1 << 17)) |
                    ((command->arguments[1] & 1) << 17);
            }
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_CAMERA_FOCUS_ENTITY:
        *(u16 *)(runtime->field_context +
                 FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) =
            (*(u16 *)(runtime->field_context +
                      FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 0x7FFF) |
            ((command->arguments[0] & 1) << 15);
        if (command->arguments[1] != -1) {
            runtime_entity = FieldVm_GetRuntimeEntity(
                FieldVm_GetEntityByIndex(runtime, command->arguments[1]));
            *(s8 *)(runtime->field_context +
                    FIELD_VM_SPECIAL_PARTY_ENTITY_ID_OFFSET) =
                (s8)command->arguments[1];
            *(fx32 *)(runtime->field_context +
                      FIELD_VM_CAMERA_FOCUS_X_OFFSET) =
                runtime_entity->position_x;
            *(fx32 *)(runtime->field_context +
                      FIELD_VM_CAMERA_FOCUS_Y_OFFSET) =
                runtime_entity->position_y - runtime_entity->position_z;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_REMOVE_ALL_ENTITY_EFFECT_SPRITES:
        func_ov000_020714e4(runtime->field_context);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ALL_ENTITY_EFFECT_SPRITES:
        if (func_ov000_020714a8(runtime->field_context)) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_ALL_ENTITY_EFFECT_SPRITES);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_LEGACY_NOOP_11A:
    case FIELD_VM_LEGACY_NOOP_11B:
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_SCRIPTED_BATTLE:
    case FIELD_VM_START_SCRIPTED_BATTLE_WITH_TRANSITION: {
        FieldEntity **entities = (FieldEntity **)(
            runtime->field_context + FIELD_VM_ENTITY_TABLE_OFFSET);
        int transition_variant =
            command->opcode == FIELD_VM_START_SCRIPTED_BATTLE
                ? 3
                : command->arguments[6];

        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_0206ba2c(
            field_system, 1, (s16)command->arguments[0],
            command->arguments[1],
            *(u16 *)(runtime->field_context +
                     FIELD_VM_CURRENT_ROOM_ID_OFFSET),
            command->arguments[3] & 0xFF,
            *(u16 *)(runtime->field_context +
                     FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1,
            entities[command->arguments[4]], entities[0],
            command->arguments[5], transition_variant);
        return SCRIPT_VM_YIELDED;
    }

    case FIELD_VM_OPEN_PAUSE_MENU:
        command->arguments[0] = FieldVm_ResolveMenuPartyContext(
            runtime, command->arguments[0]);
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_0206b0a0(
            field_system, 1, command->arguments[0], command->arguments[1],
            command->arguments[2] != 0, command->arguments[3] != 0);
        return SCRIPT_VM_YIELDED;

    case FIELD_VM_SET_SAVE_LOCATION_ID:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        *(u16 *)((u8 *)field_system +
                 FIELD_VM_FIELD_SYSTEM_SAVE_LOCATION_OFFSET) =
            (u16)command->arguments[0];
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_OPEN_SAVE_MENU:
        command->arguments[0] = FieldVm_ResolveMenuPartyContext(
            runtime, command->arguments[0]);
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_0206aeb0(
            field_system, command->arguments[0],
            command->arguments[1] != 0, command->arguments[2] != 0);
        return SCRIPT_VM_YIELDED;

    case FIELD_VM_OPEN_SHOP:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_0206acf8(
            field_system, command->arguments[0],
            command->arguments[1] != 0, command->arguments[2] != 0);
        return SCRIPT_VM_YIELDED;

    case FIELD_VM_START_STAFF_CREDITS:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_0206abd8(field_system, command->arguments[0] != 0);
        return SCRIPT_VM_YIELDED;

    case FIELD_VM_OPEN_GAME_OVER_MENU:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_0206a9a4(field_system, command->arguments[0] != 0);
        return SCRIPT_VM_YIELDED;

    case FIELD_VM_INITIALIZE_NEW_GAME_PRESET:
        func_02019330(command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESTORE_PARTY_MEMBER_HP:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_0206d274(field_system, command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_ADJUST_PARTY_MEMBER_HP:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_0206d1dc(
            field_system, command->arguments[0], command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_COINS:
        *(u32 *)(gSaveData + 1160) = command->arguments[0];
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_ADJUST_COINS: {
        int updated_count = func_02019174(
            0xFFFF, command->arguments[0]);

        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] == updated_count, vm, base_state);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_ADJUST_BEANS: {
        int updated_count = func_02019174(
            0xFFFE, command->arguments[0]);

        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] == updated_count, vm, base_state);
        if (updated_count > 0) {
            VM_WriteVariable(0x201F, 1, 0, 0);
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_ADJUST_ITEM_COUNT: {
        int updated_count = func_02019174(
            (u16)command->arguments[0], command->arguments[1]);

        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] == updated_count, vm, base_state);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_GET_ROOM_COMPANION_ID:
        VM_WriteVariable(
            command->result_variable,
            data_02048f1a[2 * command->arguments[0]], vm, base_state);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_ENABLE_TOUCHSCREEN_INPUT_TRACKING:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        *(u32 *)((u8 *)field_system +
                 FIELD_VM_FIELD_SYSTEM_TOUCH_STATE_OFFSET) |= 1;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_DISABLE_TOUCHSCREEN_INPUT_TRACKING: {
        u32 *touch_state;

        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        touch_state = (u32 *)((u8 *)field_system +
                             FIELD_VM_FIELD_SYSTEM_TOUCH_STATE_OFFSET);
        *touch_state &= ~1;
        *touch_state &= 0xFFFFFFF1;
        *touch_state = (*touch_state & 0xFFFFE00F) | 0x1FF0;
        *touch_state = (*touch_state & 0xFFC01FFF) | 0x3FE000;
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_SET_RETRY_CHECKPOINT:
        gSaveData[1302] = command->arguments[0];
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_RETRY_PROGRESS:
        gSaveData[1303] = command->arguments[0];
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_CLEAR_EXTENDED_SAVE_FLAGS:
        func_ov000_02081bd4(0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_EXTENDED_SAVE_FLAGS:
        func_ov000_02081bd4(1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PREPARE_TOUCHSCREEN_MASK_ERASE:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_02066b34(field_system);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_TOUCHSCREEN_MASK_ERASE_READY:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        if (func_ov000_02066b0c(field_system)) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_TOUCHSCREEN_MASK_ERASE_READY);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_TOUCHSCREEN_MASK_ERASE:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        func_ov000_02066ae4(field_system);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_TOUCHSCREEN_MASK_ERASE_COMPLETE:
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        if (func_ov000_020660f8(field_system)) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_TOUCHSCREEN_MASK_ERASE_COMPLETE);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PLAY_RUMBLE_PATTERN:
        func_02007408(
            command->arguments[0] - 1, command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_RUMBLE:
        if (data_0205a00c != 0) {
            func_0202e04c();
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_SINGLE_PASS_COLLISION:
        *(u16 *)(runtime->field_context +
                 FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) =
            (*(u16 *)(runtime->field_context +
                      FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 0xBFFF) |
            ((command->arguments[0] & 1) << 14);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_TIMER_VALUE: {
        void *timer;

        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        timer = *(void **)((u8 *)field_system +
                           FIELD_VM_FIELD_SYSTEM_TIMER_OFFSET);
        func_ov000_020beda0(
            timer, (s8)command->arguments[0],
            (s8)command->arguments[1], (s8)command->arguments[2]);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_SET_TIMER_DISPLAY_POSITION: {
        void *timer;

        if (command->arguments[0] == -1) {
            command->arguments[0] = *(u16 *)(
                runtime->field_context +
                FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1;
        }
        if ((u16)command->arguments[1] == 0x8000) {
            command->arguments[1] = 87;
        }
        if ((u16)command->arguments[2] == 0x8000) {
            command->arguments[2] = 88;
        }
        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        timer = *(void **)((u8 *)field_system +
                           FIELD_VM_FIELD_SYSTEM_TIMER_OFFSET);
        func_ov000_020beca4(
            timer, command->arguments[0], (s16)command->arguments[1],
            (s16)command->arguments[2]);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_SET_TIMER_VISIBLE: {
        void *timer;

        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        timer = *(void **)((u8 *)field_system +
                           FIELD_VM_FIELD_SYSTEM_TIMER_OFFSET);
        func_ov000_020becc8(timer, command->arguments[0] != 0);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_SET_TIMER_COUNT_RATE: {
        u8 *timer;

        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        timer = *(u8 **)((u8 *)field_system +
                         FIELD_VM_FIELD_SYSTEM_TIMER_OFFSET);
        timer[1] = command->arguments[0];
        timer[0] = (timer[0] & 0xF7) |
            (8 * (command->arguments[1] != 0));
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_PAUSE_TIMER:
    case FIELD_VM_RESUME_TIMER: {
        u8 *timer;

        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        timer = *(u8 **)((u8 *)field_system +
                         FIELD_VM_FIELD_SYSTEM_TIMER_OFFSET);
        if (command->opcode == FIELD_VM_PAUSE_TIMER) {
            timer[0] |= 4;
        } else {
            timer[0] &= ~4;
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_STOP_TIMER: {
        u8 *timer;

        field_system = *(void **)(runtime->field_context +
                                  FIELD_VM_FIELD_SYSTEM_OFFSET);
        timer = *(u8 **)((u8 *)field_system +
                         FIELD_VM_FIELD_SYSTEM_TIMER_OFFSET);
        func_ov000_020becc8(timer, 0);
        timer[1] = 0;
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_OPEN_SCREEN_MESSAGE: {
        int message_speed;
        int message_slot;

        func_ov000_0206f8ac(runtime->field_context);
        message_speed = VM_ReadVariable(0xC000, vm, base_state);
        message_slot = func_ov000_02070930(
            runtime->field_context, command->arguments[4],
            command->arguments[0], command->arguments[1],
            command->arguments[2], command->arguments[3],
            command->arguments[5], command->arguments[6],
            command->arguments[7], 0, command->arguments[8],
            command->arguments[9] != 1, command->arguments[11],
            command->arguments[12], command->arguments[10], message_speed,
            gSaveData + 68);
        VM_WriteVariable(
            command->result_variable, message_slot, vm, base_state);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_OPEN_ENTITY_MESSAGE: {
        FieldEntity *linked_owner = 0;
        int message_speed;
        int message_slot;

        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_0206f8ac(runtime->field_context);
        if (FieldVm_GetOwnerType(state) == FIELD_SCRIPT_OWNER_PRIMARY) {
            linked_owner = FieldVm_GetEntityByIndex(
                runtime, (s8)state->owner_data[0]);
        }
        message_speed = VM_ReadVariable(0xC000, vm, base_state);
        message_slot = func_ov000_020700ec(
            runtime->field_context, command->arguments[3], entity,
            command->arguments[1], command->arguments[2],
            command->arguments[4], command->arguments[5],
            command->arguments[6], command->arguments[7],
            command->arguments[8] != 1, command->arguments[10],
            command->arguments[11], command->arguments[9], message_speed,
            gSaveData + 68, linked_owner);
        VM_WriteVariable(
            command->result_variable, message_slot, vm, base_state);
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_WAIT_MESSAGE_FINISHED:
        if (func_ov000_0206fb74(
                runtime->field_context, command->arguments[0]) &&
            func_ov000_0206fc50(
                runtime->field_context, command->arguments[0])) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_MESSAGE_FINISHED);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_MESSAGE_CLOSED:
        if (func_ov000_0206fb74(
                runtime->field_context, command->arguments[0])) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_MESSAGE_CLOSED);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_CLOSE_MESSAGE:
        func_ov000_0206facc(
            runtime->field_context, command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_MESSAGE_WINDOW_SLIDE_MASK_ENABLED:
        func_ov000_0206f6c0(
            runtime->field_context, command->arguments[0],
            command->arguments[1] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_MESSAGE_WINDOW_SLIDE_BY_SPEED:
        func_ov000_0206f448(
            runtime->field_context, command->arguments[0],
            command->arguments[1] << FX32B_INT,
            command->arguments[2] << FX32B_INT, command->arguments[3]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_MESSAGE_WINDOW_SLIDE_OVER_FRAMES:
        func_ov000_0206f378(
            runtime->field_context, command->arguments[0],
            command->arguments[1] << FX32B_INT,
            command->arguments[2] << FX32B_INT, command->arguments[3]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_MESSAGE_WINDOW_SLIDE:
        if ((*(u32 *)(runtime->field_context +
                      FIELD_VM_MESSAGE_WINDOW_SLIDE_FLAGS_OFFSET) & 1) !=
            0) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_MESSAGE_WINDOW_SLIDE);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PLAY_FIELD_SOUND: {
        u16 *tracked_sounds;
        u16 tracked_sound;
        int index;

        func_02027cb8(
            (s16)command->arguments[0], command->arguments[1], -1);
        if (command->arguments[2] == 0) {
            return SCRIPT_VM_CONTINUE;
        }

        party_manager = FieldVm_GetPartyManager(runtime);
        tracked_sounds = (u16 *)(party_manager + 12);
        tracked_sound = (u16)command->arguments[0] |
            ((*(u16 *)(runtime->field_context +
                       FIELD_VM_SPECIAL_PARTY_STATE_OFFSET) & 1) << 15);
        for (index = 0; index < 4; index++) {
            if (command->arguments[0] ==
                (tracked_sounds[index] & 0x7FFF)) {
                tracked_sounds[index] = tracked_sound;
                break;
            }
        }
        if (index == 4) {
            for (index = 0; index < 4; index++) {
                if (tracked_sounds[index] == 0) {
                    tracked_sounds[index] = tracked_sound;
                    break;
                }
            }
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_STOP_FIELD_SOUND: {
        u16 *tracked_sounds;
        int index;

        func_02027b7c((s16)command->arguments[0]);
        party_manager = FieldVm_GetPartyManager(runtime);
        tracked_sounds = (u16 *)(party_manager + 12);
        for (index = 0; index < 4; index++) {
            if (command->arguments[0] ==
                (tracked_sounds[index] & 0x7FFF)) {
                tracked_sounds[index] = 0;
                break;
            }
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_PLAY_BACKGROUND_MUSIC:
        func_02027a2c((s16)command->arguments[0], -1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_BACKGROUND_MUSIC:
        func_02027818((s16)command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_LOAD_BACKGROUND_MUSIC_RESOURCE: {
        u32 loaded_resources = func_02027d6c();
        int buffer_index = command->arguments[1];
        int loaded_resource = buffer_index == 0
            ? loaded_resources & 0xFFFF
            : loaded_resources >> 16;

        if ((buffer_index == 0 || buffer_index == 1) &&
            command->arguments[0] != loaded_resource) {
            func_02027ea4(
                command->arguments[0], buffer_index & 0xFF, 0xC00);
        }
        if (buffer_index == 1) {
            party_manager = FieldVm_GetPartyManager(runtime);
            *(u32 *)(party_manager + 4) &= ~0x80000000;
        }
        return SCRIPT_VM_CONTINUE;
    }

    case FIELD_VM_WAIT_BACKGROUND_MUSIC_RESOURCE:
        if (func_02027718()) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_BACKGROUND_MUSIC_RESOURCE);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_ACTIVATE_BACKGROUND_MUSIC:
        if ((gSaveData[1300] & 1) == 0) {
            u32 *party_flags;

            func_02027d6c();
            func_02027e3c(command->arguments[0]);
            party_manager = FieldVm_GetPartyManager(runtime);
            party_flags = (u32 *)(party_manager + 4);
            *party_flags = (*party_flags & 0xFF01FFFF) |
                ((command->arguments[0] & 0x7F) << 17);
            *party_flags = (*party_flags & 0x80FFFFFF) | 0x7F000000;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_FADE_OUT_BACKGROUND_MUSIC:
        func_02027e20(command->arguments[0]);
        party_manager = FieldVm_GetPartyManager(runtime);
        *(u32 *)(party_manager + 4) &= 0xFF01FFFF;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_GLOBAL_SOUND_MUTED:
        func_02027db8(command->arguments[0], -1);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_LOAD_SOUND_GROUP_ASYNC:
        func_0202775c(command->arguments[0]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_SOUND_GROUP_LOAD:
        if (func_02027728() == 1) {
            return FieldVm_RetryCurrentCommand(
                vm, state, FIELD_VM_WAIT_SOUND_GROUP_LOAD);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RELEASE_SOUND_GROUP:
        func_02027744();
        return SCRIPT_VM_CONTINUE;
    }

    return SCRIPT_VM_CONTINUE;
}
