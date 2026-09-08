#ifndef PIT_GAME_FIELD_PARTY_H
#define PIT_GAME_FIELD_PARTY_H

#include <game/field_entity.h>
#include <game/field_resources.h>

/* Checked allocation layouts with unknown interiors retained as opaque fields.
 * The party entity extends FieldRuntimeEntity. The low flag bytes save collision state, while the
 * same word holds the movement mode used by the state dispatcher. */
typedef struct FieldPartyFollowerFlags { u16 enabled : 1, unknown_01_15 : 15; } FieldPartyFollowerFlags;
typedef char FieldPartyFollowerFlags_SizeCheck[sizeof(FieldPartyFollowerFlags) == 2 ? 1 : -1];
typedef struct FieldPartyEntity {
    FieldRuntimeEntity entity;
    u8 unknown_514[12];
    union {
        u32 flags;
        struct {
            u32 unknown_00_15 : 16, saved_collision_faces : 1, collision_faces_saved : 1;
            u32 movement_mode : 4, unknown_22 : 1, unknown_23 : 1, unknown_24 : 1;
            u32 unknown_25_30 : 6, unknown_31 : 1;
        } bits;
        /* The auxiliary renderer uses the same word for its own state. */
        struct {
            u32 unknown_00 : 1, unknown_01 : 1, direction_mode : 2, unknown_04 : 1;
            u32 unknown_05_06 : 2, unknown_07_08 : 2, unknown_09 : 1;
            u32 unknown_10_15 : 6, unknown_16_18 : 3, unknown_19 : 1;
            s32 unknown_20_27 : 8, unknown_28_31 : 4;
        } auxiliary_bits;
        struct {
            u8 saved_collision, collision_saved, unknown_02[2];
        } bytes;
    };
    union {
        FieldLinearController movement;
        struct {
            u32 unknown_524[6];
            fx32 unknown_53c, unknown_540, unknown_544;
            u8 unknown_548[4];
            struct FieldPartyEntity *target;
            u8 unknown_550[24];
        } auxiliary_motion;
    };
    union { u16 *unknown_568; FieldPartyFollowerFlags *follower_flags; };
    u8 unknown_56c[8];
    struct FieldPartyEntity *partner;
    struct FieldPartyEntity *auxiliary;
    u8 unknown_57c[20];
    s16 saved_animation_speed;
    struct {
        u16 unknown_00 : 1, saved_behavior : 3, behavior_saved : 1, unknown_05 : 1;
        s16 resource_index : 8;
        u16 unknown_14 : 1, unknown_15 : 1;
    } presentation;
    struct { u8 unknown_00 : 1, unknown_01_07 : 7; } unknown_594;
    u8 unknown_595;
    s8 unknown_596, unknown_597, unknown_598, unknown_599;
    u8 unknown_59a[2];
    u32 unknown_59c;
} FieldPartyEntity;
typedef char FieldPartyEntity_SizeCheck[sizeof(FieldPartyEntity) == 0x5a0 ? 1 : -1];

typedef struct FieldPartySnapshot FieldPartySnapshot;
typedef struct FieldPartyStateRecord {
    u32 unknown_00[8];
} FieldPartyStateRecord;
typedef char FieldPartyStateRecord_SizeCheck[sizeof(FieldPartyStateRecord) == 32 ? 1 : -1];

typedef struct FieldPartyController {
    union {
        struct {
            u16 unknown_00 : 1, unknown_01 : 1, unknown_02_15 : 14;
        } state;
        struct {
            u16 members : 2, unknown_02_03 : 2, unknown_04_15 : 12;
        } state_groups;
    };
    u8 unknown_002, unknown_003;
    u16 input_masks[2];
    FieldPartyStateRecord records[2];
    union {
        struct {
            FieldPartyEntity *leader, *follower;
        };
        FieldPartyEntity *members[2];
    };
    struct {
        u32 unknown_00_01 : 2, unknown_02 : 1, unknown_03 : 1, unknown_04 : 1, unknown_05 : 1, unknown_06 : 1;
        u32 field_screen : 1, previous_field_screen : 1, special_contact_mode : 1;
        u32 unknown_10_13 : 4, unknown_14 : 1, follower_rejoin_active : 1;
        u32 unknown_16 : 1, active_member : 1, movement_active : 1;
        u32 unknown_19 : 1, movement_state : 10, unknown_30_31 : 2;
    } flags;
    u8 unknown_054;
    s8 unknown_055;
    u8 unknown_056[2];
    u32 unknown_058, unknown_05c;
    fx32 separation_x, separation_y, separation_z;
    u32 unknown_06c;
    u8 actions[4];
    union {
        u32 state_flags;
        struct {
            u32 visible : 1, unknown_01_04 : 4, slide_from_bottom : 1;
            u32 visibility_transition : 1, unknown_07 : 1, unknown_08 : 1;
            u32 sliding : 1, swap_active : 1, unknown_11_31 : 21;
        } indicator_bits;
        struct {
            u32 unknown_00 : 1, unknown_01 : 1, unknown_02 : 1;
            u32 unknown_03_04 : 2, unknown_05 : 1, unknown_06_10 : 5, unknown_11_12 : 2;
            u32 unknown_13 : 1, unknown_14_17 : 4, movement_mode : 4, unknown_22_31 : 10;
        } state_bits;
    };
    union {
        u8 unknown_078[16];
        struct {
            union { u16 unknown_078_halfword; u16 indicator_frame; };
            s16 indicator_velocity, indicator_x, indicator_y;
            s16 indicator_offset_x, indicator_offset_y, indicator_slide_y;
            u16 unknown_086;
        };
    };
    FieldRenderObject *unknown_088, *unknown_08c, *unknown_090;
    union {
        u8 unknown_094[2212];
        struct { FieldRenderObject *unknown_094_render; u8 follower_state[2208]; };
    };
    FieldResourceContext **areas;
    struct FieldPartyController *paired;
    u8 *resources;
    u8 unknown_944[3092];
    struct {
        u32 active : 1, movement_state : 10, unknown_11 : 1, snapshot_backed : 1, unknown_13_31 : 19;
    } backup;
    FieldPartyEntity backup_entities[2];
    const FieldPartySnapshot *backup_snapshots[2];
} FieldPartyController;
typedef char FieldPartyController_SizeCheck[sizeof(FieldPartyController) == 0x20a4 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void FieldParty_DisableFollowing(FieldPartyController *party);
void FieldParty_ResetFollowingContacts(FieldPartyController *party);
void FieldParty_InitializeFollowing(FieldPartyController *party, int argument);
void FieldParty_UpdateFollowing(FieldPartyController *party);
void FieldParty_EnableFollowing(FieldPartyController *party);
int FieldParty_CheckFollowerRejoin(FieldPartyController *party);
void FieldParty_RejoinFollower(FieldPartyController *party, int instant);
void FieldParty_UpdateIndicatorVisibility(FieldPartyController *party);
void FieldParty_SetIndicatorVisibility(FieldPartyController *party, int visible, int instant, int transfer);
void FieldParty_UpdateIndicatorSwap(FieldPartyController *party);
void FieldParty_StartIndicatorSwap(FieldPartyController *party);
void FieldParty_SetIndicatorLayer(FieldPartyController *party, int alternate);
void FieldParty_UpdateIndicatorPositions(FieldPartyController *party);
void FieldParty_UpdateIndicatorSlide(FieldPartyController *party);
void FieldParty_StartIndicatorSlide(FieldPartyController *party, int upward);
FieldPartyEntity *FieldPartyEntity_CopyState(FieldPartyEntity *entity, const FieldPartyEntity *source);
FieldPartyEntity *FieldPartyAuxiliary_CopyState(FieldPartyEntity *entity, const FieldPartyEntity *source);
void FieldParty_LinkPairedMember(FieldPartyController *party, int member);
void FieldParty_StartPairedFollowing(FieldPartyController *party);
void FieldParty_WaitForPairedFollowing(FieldPartyController *party);
void FieldParty_PrepareCollisionOverride(FieldPartyController *party, int preserve_state);
void FieldParty_RestoreCollisionState(FieldPartyController *party, int check_contacts);
void FieldParty_RestartCollisionOverride(FieldPartyController *party);
void FieldParty_InitializeAuxiliary(FieldPartyController *party, int preserve_state);
void FieldParty_UpdateAuxiliaryFacing(FieldPartyController *party);
void FieldParty_ReverseFollowerOffset(FieldPartyController *party);
void FieldParty_ReturnFollowerToLeader(FieldPartyController *party);
void FieldParty_RestartAuxiliary(FieldPartyController *party);
void FieldParty_RaiseFollowerToLeader(FieldPartyController *party);
/* State 34's gameplay identity has not yet been observed. */
void FieldParty_BeginState34(FieldPartyController *party);
#ifdef __cplusplus
}
#endif
#endif
