#include <game/field_entity.h>
#include <game/field_script.h>

/*
 * Structured reconstruction of the field-specific VM command range. The
 * original is a single 23,492-byte switch; this work unit is intentionally
 * unlinked until its semantics and Metrowerks instruction layout are complete.
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
    FIELD_VM_SET_ENTITY_INTERACTION_HEIGHT = 0x064,
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
    FIELD_VM_GET_ENTITY_SCRIPT_VALUE = 0x0AD
};

enum FieldVmContextOffset {
    FIELD_VM_RESOURCE_RECORDS_OFFSET = 0x00B4,
    FIELD_VM_RESOURCE_COUNTS_OFFSET = 0x2338,
    FIELD_VM_SPECIAL_PARTY_STATE_OFFSET = 0x23F8,
    FIELD_VM_SPECIAL_PARTY_ENTITY_ID_OFFSET = 0x23FA,
    FIELD_VM_AUX_SCRIPT_STATES_OFFSET = 0x26A4,
    FIELD_VM_PAIRED_FIELD_CONTEXT_OFFSET = 0x29D4,
    FIELD_VM_ENTITY_TABLE_OFFSET = 0x29D8,
    FIELD_VM_SPECIAL_RESOURCE_RECORDS_OFFSET = 0x2A68,
    FIELD_VM_ENTITY_COUNT_OFFSET = 0x2B32,
    FIELD_VM_RESOURCE_RECORD_SIZE = 24
};

enum FieldVmPairedContextOffset {
    FIELD_VM_PAIRED_ROOM_ID_OFFSET = 0x23F0,
    FIELD_VM_PAIRED_SCRIPT_TABLE_OFFSET = 0x24E0,
    FIELD_VM_PAIRED_SCRIPT_MANAGER_OFFSET = 0x2570,
    FIELD_VM_PAIRED_SCRIPT_FLAGS_OFFSET = 0x29B8,
    FIELD_VM_PAIRED_PARENT_ENTITY_ID_OFFSET = 0x29C1,
    FIELD_VM_PAIRED_QUEUED_SCRIPT_OFFSET = 0x29C8
};

static int FieldVm_GetOwnerType(const FieldScriptState *state) {
    return (state->flags & FIELD_SCRIPT_OWNER_TYPE_MASK) >>
        FIELD_SCRIPT_OWNER_TYPE_SHIFT;
}

static int FieldVm_GetParentType(const FieldScriptState *state) {
    return (state->flags & FIELD_SCRIPT_PARENT_TYPE_MASK) >>
        FIELD_SCRIPT_PARENT_TYPE_SHIFT;
}

static int FieldVm_GetEffectiveParentType(const FieldScriptState *state) {
    int owner_type = FieldVm_GetOwnerType(state);

    if (owner_type == FIELD_SCRIPT_OWNER_ENTITY) {
        return FieldVm_GetParentType(state);
    }
    return owner_type;
}

static int FieldVm_GetOwnerSubtype(const FieldScriptState *state) {
    return (s8)(state->flags >> FIELD_SCRIPT_OWNER_SUBTYPE_SHIFT);
}

static int FieldVm_GetParentEntityId(const FieldScriptState *state) {
    if (FieldVm_GetOwnerType(state) == FIELD_SCRIPT_OWNER_ENTITY) {
        return *(s8 *)((u8 *)state->owner_entity + 4);
    }
    return -1;
}

static void FieldVm_SetQueuedOwner(FieldScriptState *target,
                                   const FieldScriptState *parent) {
    int parent_type = FieldVm_GetEffectiveParentType(parent);

    target->flags =
        (target->flags & ~FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK) |
        ((parent_type << FIELD_SCRIPT_QUEUED_PARENT_TYPE_SHIFT) &
         FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK);
    target->queued_parent_entity_id = FieldVm_GetParentEntityId(parent);
}

static int FieldVm_GetScriptState(const FieldScriptState *state) {
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

static int FieldVm_RetryCurrentCommand(ScriptVm *vm,
                                       FieldScriptState *state,
                                       ScriptVmCommand *command) {
    u32 descriptor = vm->command_descriptors[command->opcode];
    int halfword_count =
        (descriptor & SCRIPT_VM_ARGUMENT_COUNT_MASK) +
        ((descriptor & SCRIPT_VM_HAS_RESULT) != 0) +
        ((descriptor & SCRIPT_VM_HAS_ARGUMENT_MODES) != 0) + 1;

    state->vm_state.script -= halfword_count;
    return SCRIPT_VM_YIELDED;
}

static FieldScriptState *FieldVm_GetEntityScript(FieldEntity *entity) {
    return (FieldScriptState *)((u8 *)entity + 0x20);
}

static FieldScriptState *FieldVm_GetAuxScript(FieldVmRuntime *runtime,
                                               int slot) {
    return (FieldScriptState *)(
        runtime->field_context + FIELD_VM_AUX_SCRIPT_STATES_OFFSET) + slot;
}

static int FieldVm_EntityScriptMatches(const FieldScriptState *target,
                                       int parent_type,
                                       int parent_entity_id) {
    int target_parent_type =
        (target->flags & FIELD_SCRIPT_PARENT_TYPE_MASK) >>
        FIELD_SCRIPT_PARENT_TYPE_SHIFT;

    return (target->flags & FIELD_SCRIPT_ACTIVE) != 0 &&
        (target_parent_type == parent_type ||
         (parent_type == FIELD_SCRIPT_OWNER_ENTITY &&
          target->parent_entity_id == parent_entity_id));
}

static int FieldVm_VisitMatchingEntityScripts(
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
        parent_type = FieldVm_GetOwnerType(caller);
    }
    if (parent_type == FIELD_SCRIPT_OWNER_ENTITY) {
        parent_entity_id = FieldVm_GetParentEntityId(caller);
    }

    for (index = 0; index < entity_count; index++) {
        FieldEntity *entity = entities[index];
        FieldScriptState *target = FieldVm_GetEntityScript(entity);

        if (!FieldVm_EntityScriptMatches(
                target, parent_type, parent_entity_id)) {
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

static void FieldVm_StartEntityScript(FieldScriptState *caller,
                                      FieldEntity *entity,
                                      const u16 *script,
                                      int chain_if_active) {
    FieldScriptState *target = FieldVm_GetEntityScript(entity);

    if (chain_if_active && (target->flags & FIELD_SCRIPT_ACTIVE) != 0) {
        target->queued_script = script;
        FieldVm_SetQueuedOwner(target, caller);
        return;
    }
    entity->vtable->stop_script(entity);
    func_ov000_02082240(
        target, caller, FieldVm_GetOwnerType(caller) & 3, script);
}

static void FieldVm_StartPairedScript(FieldVmRuntime *runtime,
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

        if (slot != -1) {
            script_table = *(const u16 **)(
                paired + FIELD_VM_PAIRED_SCRIPT_TABLE_OFFSET);
            queued_script = (const u16 *)(
                (const u8 *)script_table + script_table[9 + (u16)slot]);
        }
        *(const u16 **)(paired + FIELD_VM_PAIRED_QUEUED_SCRIPT_OFFSET) =
            queued_script;
        *script_flags =
            (*script_flags & ~FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK) |
            ((FieldVm_GetEffectiveParentType(caller) <<
              FIELD_SCRIPT_QUEUED_PARENT_TYPE_SHIFT) &
             FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK);
        *(s8 *)(paired + FIELD_VM_PAIRED_PARENT_ENTITY_ID_OFFSET) =
            FieldVm_GetParentEntityId(caller);
        return;
    }

    func_ov000_0208911c(
        paired + FIELD_VM_PAIRED_SCRIPT_MANAGER_OFFSET, caller, (s16)slot);
}

static FieldRuntimeEntity *FieldVm_GetRuntimeEntity(FieldEntity *entity) {
    return (FieldRuntimeEntity *)entity;
}

static FieldEntity *FieldVm_GetEntityByIndex(FieldVmRuntime *runtime,
                                             int entity_index) {
    FieldEntity **entities = (FieldEntity **)(
        runtime->field_context + FIELD_VM_ENTITY_TABLE_OFFSET);

    return entities[entity_index];
}

static int FieldVm_IsTwoDimensionalEntity(
    const FieldRuntimeEntity *entity) {
    return ((entity->base.property_00a >> 2) & 0xF) == 8;
}

static u32 *FieldVm_GetInteractionFlags(FieldRuntimeEntity *entity) {
    return (u32 *)((u8 *)entity + 0x520);
}

static void FieldVm_SetFlag(u32 *flags, u32 mask, int enabled) {
    if (enabled) {
        *flags |= mask;
    } else {
        *flags &= ~mask;
    }
}

static u32 FieldVm_MapContactDirectionMask(int script_mask) {
    u32 direction_mask = 0;

    if ((script_mask & 0x01) != 0) direction_mask |= 0x04;
    if ((script_mask & 0x04) != 0) direction_mask |= 0x01;
    if ((script_mask & 0x08) != 0) direction_mask |= 0x02;
    if ((script_mask & 0x02) != 0) direction_mask |= 0x08;
    if ((script_mask & 0x10) != 0) direction_mask |= 0x20;
    if ((script_mask & 0x20) != 0) direction_mask |= 0x10;
    return direction_mask;
}

static void FieldVm_SetNavigationBounds(FieldRuntimeEntity *entity,
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

static void FieldVm_SetBodyBounds(FieldRuntimeEntity *entity,
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

static void FieldVm_SetCollisionPolicy(FieldRuntimeEntity *entity,
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

static void FieldVm_SetRenderPriorities(FieldRuntimeEntity *entity,
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

static s32 FieldVm_DecodeWideArgument(const ScriptVmCommand *command,
                                      int argument_index) {
    if ((command->argument_modes & (1 << argument_index)) != 0) {
        return command->arguments[argument_index];
    }
    return (u16)command->arguments[argument_index] |
        (command->arguments[argument_index + 1] << 16);
}

static int FieldVm_DegreesToTurn16(int degrees) {
    return (degrees << 16) / 360;
}

static const void *FieldVm_GetResourceRecord(FieldVmRuntime *runtime,
                                             FieldRuntimeEntity *entity,
                                             int resource_index) {
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
    const u16 *script;
    int selector;
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
        target = FieldVm_GetAuxScript(runtime, command->arguments[0]);
        target->flags =
            (target->flags & ~FIELD_SCRIPT_ENABLED) |
            (command->arguments[1] & FIELD_SCRIPT_ENABLED);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_AUX_SCRIPT:
        target = FieldVm_GetAuxScript(runtime, command->arguments[0]);
        if ((target->flags & FIELD_SCRIPT_ENABLED) != 0) {
            script = state->vm_state.script + command->arguments[1];
            if (command->arguments[2] != 0 &&
                (target->flags & FIELD_SCRIPT_ACTIVE) != 0) {
                target->queued_script = script;
                FieldVm_SetQueuedOwner(target, state);
            } else {
                func_ov000_02082240(target, state, 0, script);
            }
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_AUX_SCRIPT:
        target = FieldVm_GetAuxScript(runtime, command->arguments[0]);
        if ((target->flags & FIELD_SCRIPT_ACTIVE) != 0) {
            return FieldVm_RetryCurrentCommand(vm, state, command);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_AUX_SCRIPT:
        target = FieldVm_GetAuxScript(runtime, command->arguments[0]);
        target->flags &= ~FIELD_SCRIPT_ACTIVE;
        target->queued_script = 0;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PAUSE_AUX_SCRIPT:
        FieldVm_GetAuxScript(runtime, command->arguments[0])->flags |=
            FIELD_SCRIPT_PAUSED;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESUME_AUX_SCRIPT:
        FieldVm_GetAuxScript(runtime, command->arguments[0])->flags &=
            ~FIELD_SCRIPT_PAUSED;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_GET_AUX_SCRIPT_STATE:
        target = FieldVm_GetAuxScript(runtime, command->arguments[0]);
        VM_WriteVariable(
            command->result_variable, FieldVm_GetScriptState(target),
            vm, base_state);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_INLINE_ENTITY_SCRIPT:
    case FIELD_VM_START_INLINE_ENTITY_SCRIPT_AND_WAIT:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        target = FieldVm_GetEntityScript(entity);
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
            return FieldVm_RetryCurrentCommand(vm, state, command);
        }
        state->vm_state.script += command->arguments[2];
        state->flags &= ~FIELD_SCRIPT_INLINE_WAIT;
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_RELATIVE_ENTITY_SCRIPT:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        script = state->vm_state.script + command->arguments[1];
        FieldVm_StartEntityScript(
            state, entity, script, command->arguments[2]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ENTITY_SCRIPT:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        target = FieldVm_GetEntityScript(entity);
        if ((target->flags & FIELD_SCRIPT_ACTIVE) != 0) {
            return FieldVm_RetryCurrentCommand(vm, state, command);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_MATCHING_ENTITY_SCRIPTS:
    case FIELD_VM_STOP_MATCHING_ENTITY_SCRIPTS:
    case FIELD_VM_PAUSE_MATCHING_ENTITY_SCRIPTS:
    case FIELD_VM_RESUME_MATCHING_ENTITY_SCRIPTS:
        if (FieldVm_VisitMatchingEntityScripts(
                runtime, state, command->arguments[0], command->opcode)) {
            return FieldVm_RetryCurrentCommand(vm, state, command);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_STOP_ENTITY_SCRIPT:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->vtable->stop_script(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_PAUSE_ENTITY_SCRIPT:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->vtable->pause_script(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_RESUME_ENTITY_SCRIPT:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->vtable->resume_script(entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_GET_ENTITY_SCRIPT_STATE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        VM_WriteVariable(
            command->result_variable,
            FieldVm_GetScriptState(FieldVm_GetEntityScript(entity)),
            vm, base_state);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_START_PAIRED_FIELD_SCRIPT:
        FieldVm_StartPairedScript(runtime, state, command);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_GET_ENTITY_PROPERTY:
        selector = command->arguments[0];
        entity = func_ov000_0208221c(runtime, state, selector);
        VM_WriteVariable(
            command->result_variable,
            entity->vtable->get_property(entity, command->arguments[1]),
            vm, base_state);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_VISIBLE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->vtable->set_visible(entity, command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_ENABLED:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->property_00a = (entity->property_00a & ~1) |
            (command->arguments[1] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_TURN_TO_INTERACTOR_ENABLED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->field_state_flags,
            FIELD_ENTITY_TURN_TO_INTERACTOR, command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_GROUND_TRACKING:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->field_state_flags, FIELD_ENTITY_TRACK_GROUND,
            command->arguments[1]);
        if (command->arguments[1] != 0) {
            runtime_entity->runtime_flags |=
                FIELD_ENTITY_VERTICAL_SYNC_DIRTY;
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_ALTERNATE_COLLISION_FACES_ENABLED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->runtime_flags,
            FIELD_ENTITY_ALTERNATE_COLLISION_FACES,
            command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_NAVIGATION_OBSTACLE_IGNORED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->field_state_flags,
            FIELD_ENTITY_IGNORE_NAVIGATION_OBSTACLE,
            command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_OFFSCREEN_CONTACT_RETENTION_ENABLED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->base_state_flags,
            FIELD_ENTITY_RETAIN_OFFSCREEN_CONTACT,
            command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_RESERVED_STATE_FLAG:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            &runtime_entity->base_state_flags,
            FIELD_ENTITY_RESERVED_STATE, command->arguments[1]);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_LINKED_CONTACT_MODE:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->field_state_flags =
            (runtime_entity->field_state_flags &
             ~FIELD_ENTITY_CONTACT_MODE_MASK) |
            (command->arguments[1] & FIELD_ENTITY_CONTACT_MODE_MASK);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_CONTACT_DIRECTION_FILTER:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->contact_direction_flags =
            (runtime_entity->contact_direction_flags & ~0x3F) |
            FieldVm_MapContactDirectionMask(command->arguments[1]);
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

    case FIELD_VM_SET_ENTITY_INTERACTION_HEIGHT:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        runtime_entity->interaction_vertical_extent =
            command->arguments[1] << FX32B_INT;
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
            return FieldVm_RetryCurrentCommand(vm, state, command);
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
            return FieldVm_RetryCurrentCommand(vm, state, command);
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
            return FieldVm_RetryCurrentCommand(vm, state, command);
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
            return FieldVm_RetryCurrentCommand(vm, state, command);
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
            return FieldVm_RetryCurrentCommand(vm, state, command);
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
            return FieldVm_RetryCurrentCommand(vm, state, command);
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
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020713bc(
            runtime->field_context, entity, -1, command->arguments[1],
            command->arguments[2], (s16)command->arguments[3],
            (s16)command->arguments[4], command->arguments[5],
            command->arguments[6] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_REMOVE_ENTITY_EFFECT_SPRITE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_0207138c(runtime->field_context, entity);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_ENTITY_EFFECT_SPRITE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        if (func_ov000_0207133c(runtime->field_context, entity)) {
            return FieldVm_RetryCurrentCommand(vm, state, command);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_FIELD_BLOCK_IDLE_BOBBING_ENABLED:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020bc8e4(entity, command->arguments[1] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_FIELD_BLOCK_BOUNCE_CONTROLLER_ENABLED:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        func_ov000_020bc7d0(entity, command->arguments[1] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_WAIT_FIELD_BLOCK_BOUNCE:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        if ((s8)(*FieldVm_GetInteractionFlags(runtime_entity) >> 14) !=
            -1) {
            return FieldVm_RetryCurrentCommand(vm, state, command);
        }
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENEMY_JUMP_FIRST_STRIKE_ENABLED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            FieldVm_GetInteractionFlags(runtime_entity), 1 << 0,
            command->arguments[1] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENEMY_SPIKED_JUMP_RESPONSE:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            FieldVm_GetInteractionFlags(runtime_entity), 1 << 1,
            command->arguments[1] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENEMY_SPECIAL_CONTACT_REMOVAL_ENABLED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            FieldVm_GetInteractionFlags(runtime_entity), 1 << 2,
            command->arguments[1] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENEMY_IMMEDIATE_BATTLE_REMOVAL_ENABLED:
        runtime_entity = FieldVm_GetRuntimeEntity(func_ov000_0208221c(
            runtime, state, command->arguments[0]));
        FieldVm_SetFlag(
            FieldVm_GetInteractionFlags(runtime_entity), 1 << 3,
            command->arguments[1] != 0);
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_SET_ENTITY_SCRIPT_VALUE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        entity->script_values[command->arguments[1]] =
            (s16)command->arguments[2];
        return SCRIPT_VM_CONTINUE;

    case FIELD_VM_GET_ENTITY_SCRIPT_VALUE:
        entity = func_ov000_0208221c(
            runtime, state, command->arguments[0]);
        VM_WriteVariable(
            command->result_variable,
            entity->script_values[command->arguments[1]], vm, base_state);
        return SCRIPT_VM_CONTINUE;
    }

    return SCRIPT_VM_CONTINUE;
}
