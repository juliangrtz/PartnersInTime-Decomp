#ifndef PIT_GAME_FIELD_PARTY_SNAPSHOT_H
#define PIT_GAME_FIELD_PARTY_SNAPSHOT_H

#include <game/field_party.h>

/* Compact field-transition state. Unassigned bits and byte +0x51 are preserved.
 * Entity pointers are stored as signed indices; -1 denotes no linked entity. */
typedef struct FieldPartySnapshot {
    struct {
        u32 active : 1, visibility_07 : 1, presentation_31 : 1, animation_wait : 1;
        u32 facing : 3, blink_mode : 2, blink_paused : 1, blink_phase : 1;
        s32 blink_frames : 9;
        u32 blink_offset : 7, movement_mode : 4, presentation_05 : 1;
    } flags;
    struct {
        s32 resource_index : 8;
        u32 contact_mask_a : 6, previous_contact_mask_a : 6;
        u32 contact_mask_b : 6, previous_contact_mask_b : 6;
    } contacts;
    struct {
        u32 contact_mask_b : 6, previous_contact_mask_b : 6;
        s32 saved_animation_speed : 16;
        u32 presentation_00 : 1, presentation_14 : 1, presentation_15 : 1, track_ground : 1;
    } runtime;
    struct {
        u32 runtime_21 : 1, unknown_01_31 : 31;
    } extra;
    u32 motion_flags;
    struct {
        u32 behavior_mode : 3, saved_behavior_mode : 3, has_saved_behavior : 1;
        u32 saved_behavior : 3, behavior_saved : 1;
        u32 saved_collision : 8, collision_saved : 8;
        u32 saved_collision_faces : 1, collision_faces_saved : 1, collision_23 : 1, unknown_30_31 : 2;
    } presentation;
    FieldLocomotionParameters locomotion, initial_locomotion;
    u16 direction;
    s16 animation_speed;
    u16 animation_id;
    s8 support_index, previous_support_index, linked_index, unknown_51, unknown_52, unknown_53;
    u16 locomotion_state;
    s16 unknown_56;
    fx32 default_vertical_launch_velocity, default_gravity, terminal_fall_velocity;
    fx32 initial_vertical_launch_velocity, initial_gravity, initial_terminal_fall_velocity;
    fx32 x, y, z, relative_height, support_clearance;
    struct {
        u32 contact_active : 1, previous_contact_active : 1, collision : 8;
        u32 alternate_collision_faces : 1, unknown_11_31 : 21;
    } current_contacts;
} FieldPartySnapshot;
typedef char FieldPartySnapshot_SizeCheck[sizeof(FieldPartySnapshot) == 136 ? 1 : -1];

typedef struct FieldPartyControllerSnapshot {
    struct {
        u32 members : 2, state_02_03 : 2, party_00_01 : 2, party_02 : 1, party_03 : 1;
        u32 party_04 : 1, party_05 : 1, party_06 : 1, field_screen : 1, special_contact_mode : 1;
        u32 party_10_13 : 4, party_14 : 1, follower_rejoin_active : 1, active_member : 1;
        u32 movement_active : 1, party_19 : 1, unknown_22_30 : 9, backup_11 : 1;
    } flags;
    struct {
        u16 movement_state : 14, unknown_14_15 : 2;
    } area;
    u8 unknown_006, unknown_007;
    u16 input_masks[2];
    u8 unknown_00c;
    s8 unknown_00d;
    u8 unknown_00e[2];
    u32 unknown_010, unknown_014;
    fx32 separation_x, separation_y, separation_z;
    u32 unknown_024;
    u8 actions[4];
    struct {
        u32 unknown_00 : 1, unknown_01 : 1, unknown_02 : 1, unknown_03_04 : 2, unknown_05 : 1;
        u32 unknown_06_07 : 2, unknown_08 : 1, unknown_09_12 : 4, movement_mode : 4, unknown_17_31 : 15;
    } state;
    FieldPartySnapshot members[2];
    FieldPartyStateRecord records[2];
} FieldPartyControllerSnapshot;
typedef char FieldPartyControllerSnapshot_SizeCheck[sizeof(FieldPartyControllerSnapshot) == 384 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void FieldParty_SaveSnapshot(FieldPartyController *party, FieldPartyControllerSnapshot *snapshot);
FieldPartyController *FieldParty_DestroyBackups(FieldPartyController *party);
FieldPartyController *FieldParty_InitializeStorage(FieldPartyController *party);
void FieldPartyEntity_SaveSnapshot(FieldPartyEntity *party, FieldPartySnapshot *snapshot);
FieldPartyEntity *FieldPartyEntity_RestoreSnapshot(FieldPartyEntity *party,
                                                   const FieldPartySnapshot *snapshot,
                                                   FieldRuntimeEntity **entities);
#ifdef __cplusplus
}
#endif
#endif
