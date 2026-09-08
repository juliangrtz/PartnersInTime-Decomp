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
        struct {
            u8 saved_collision, collision_saved, unknown_02[2];
        } bytes;
    };
    FieldLinearController movement;
    u16 *unknown_568;
    u8 unknown_56c[8];
    struct FieldPartyEntity *partner;
    struct FieldPartyEntity *auxiliary;
    u8 unknown_57c[22];
    struct {
        u16 unknown_00 : 1, saved_behavior : 3, behavior_saved : 1, unknown_05_15 : 11;
    } presentation;
} FieldPartyEntity;
typedef char FieldPartyEntity_SizeCheck[sizeof(FieldPartyEntity) == 0x594 ? 1 : -1];

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
    u8 unknown_054[32];
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
} FieldPartyController;
typedef char FieldPartyController_SizeCheck[sizeof(FieldPartyController) == 0x940 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void FieldParty_LinkPairedMember(FieldPartyController *party, int member);
void FieldParty_StartPairedFollowing(FieldPartyController *party);
void FieldParty_WaitForPairedFollowing(FieldPartyController *party);
void FieldParty_PrepareCollisionOverride(FieldPartyController *party, int preserve_state);
void FieldParty_RestoreCollisionState(FieldPartyController *party, int check_contacts);
void FieldParty_RestartCollisionOverride(FieldPartyController *party);
/* State 34's gameplay identity has not yet been observed. */
void FieldParty_BeginState34(FieldPartyController *party);
#ifdef __cplusplus
}
#endif
#endif
