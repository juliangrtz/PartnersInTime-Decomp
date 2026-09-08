#ifndef PIT_GAME_FIELD_PARTY_H
#define PIT_GAME_FIELD_PARTY_H

#include <game/field_entity.h>
#include <game/field_resources.h>

/* Partial views used by the paired-member and collision-state transitions.
 * The party entity extends FieldRuntimeEntity; neither view describes the
 * complete allocation. The low flag bytes save collision state, while the
 * same word holds the movement mode used by the state dispatcher. */
typedef struct FieldPartyEntity {
    FieldRuntimeEntity entity;
    u8 unknown_514[12];
    union {
        u32 flags;
        struct {
            u32 unknown_00_15 : 16, saved_collision_faces : 1, collision_faces_saved : 1;
            u32 movement_mode : 4, unknown_22_31 : 10;
        } bits;
        /* The auxiliary renderer uses the same word for its own state. */
        struct {
            u32 unknown_00 : 1, unknown_01 : 1, direction_mode : 2, unknown_04 : 1;
            u32 unknown_05_06 : 2, unknown_07_08 : 2, unknown_09 : 1;
            u32 unknown_10_18 : 9, unknown_19 : 1, unknown_20_27 : 8, unknown_28_31 : 4;
        } auxiliary_bits;
        struct {
            u8 saved_collision, collision_saved, unknown_02[2];
        } bytes;
    };
    union {
        FieldLinearController movement;
        struct {
            u8 unknown_524[24];
            fx32 unknown_53c, unknown_540, unknown_544;
            u8 unknown_548[4];
            struct FieldPartyEntity *target;
            u8 unknown_550[24];
        } auxiliary_motion;
    };
    u16 *unknown_568;
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
    u8 unknown_594[2];
    s8 unknown_596, unknown_597, unknown_598;
    u8 unknown_599[3];
} FieldPartyEntity;
typedef char FieldPartyEntity_SizeCheck[sizeof(FieldPartyEntity) == 0x59c ? 1 : -1];

typedef struct FieldPartyController {
    struct {
        u16 unknown_00 : 1, unknown_01 : 1, unknown_02_15 : 14;
    } state;
    u8 unknown_002[70];
    union {
        struct {
            FieldPartyEntity *leader, *follower;
        };
        FieldPartyEntity *members[2];
    };
    struct {
        u32 unknown_00_03 : 4, unknown_04 : 1, unknown_05 : 1, unknown_06 : 1;
        u32 field_screen : 1, previous_field_screen : 1, special_contact_mode : 1;
        u32 unknown_10_13 : 4, unknown_14 : 1, follower_rejoin_active : 1;
        u32 unknown_16 : 1, active_member : 1, movement_active : 1;
        u32 unknown_19 : 1, movement_state : 10, unknown_30_31 : 2;
    } flags;
    u8 unknown_054[12];
    fx32 separation_x, separation_y, separation_z;
    u8 unknown_06c[8];
    union {
        u32 state_flags;
        struct {
            u32 unknown_00 : 1, unknown_01 : 1, unknown_02 : 1;
            u32 unknown_03_04 : 2, unknown_05_17 : 13, movement_mode : 4, unknown_22_31 : 10;
        } state_bits;
    };
    u8 unknown_078[16];
    FieldRenderObject *unknown_088, *unknown_08c, *unknown_090;
    u8 unknown_094[2212];
    FieldResourceContext **areas;
    struct FieldPartyController *paired;
    u8 *resources;
} FieldPartyController;
typedef char FieldPartyController_SizeCheck[sizeof(FieldPartyController) == 0x944 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
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
