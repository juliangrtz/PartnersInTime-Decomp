#include <game/field_party_manager.h>
#include <game/field_entity_lifecycle.h>
#include <game/field_entity_motion.h>
#include <game/field_presentation.h>
#include <game/field_linear.h>
extern "C" {
void func_ov000_0208addc(FieldPartyController *, fx32);
void func_ov000_0208bef4(FieldPartyController *, int);
void func_ov000_020a6d68(FieldEntity *, const void *, int, int, int, int, int);
void func_ov000_0209cb90(FieldPartyController *, int);
void func_ov000_0209ce18(FieldPartyController *);
void func_ov000_0209ce44(FieldPartyController *, int, int);
void func_020093b4(FieldRenderObject *, int);
void func_ov000_020a0c30(FieldPartyManager *, int, int, int, int);
}
#define ADULT manager->parties[0]
#define BABY manager->parties[1]
extern "C" void FieldPartyManager_BeginReunion(FieldPartyManager *manager, int side, int instant)
{
    FieldPartyController *party = &manager->parties[side];
    FieldPartyController *paired = &manager->parties[side ^ 1];
    if (!party->leader->bits.movement_mode && !paired->leader->bits.movement_mode) {
        if (instant) {
            FieldEntity3D_SetPosition(&party->leader->entity, paired->leader->entity.position_x,
                                      paired->leader->entity.position_y, paired->leader->entity.position_z);
            FieldEntity_SetFacingDirection(&party->leader->entity, 0,
                                           paired->leader->entity.base_state_flag_bits.facing_direction, 1);
            FieldEntity3D_SetPosition(&party->follower->entity, paired->follower->entity.position_x,
                                      paired->follower->entity.position_y,
                                      paired->follower->entity.position_z);
            FieldEntity_SetFacingDirection(&party->follower->entity, 0,
                                           paired->follower->entity.base_state_flag_bits.facing_direction, 1);
            party->flags.unknown_16 = 0;
            paired->flags.unknown_16 = 0;
            func_ov000_020a0c30(manager, 0, 0, 1, 0);
            FieldPartyManager_CompleteReunion(manager);
        } else {
            FieldLinear3D_Start(&party->leader->entity, 0,
                                (paired->leader->entity.position_x + paired->follower->entity.position_x) / 2,
                                (paired->leader->entity.position_y + paired->follower->entity.position_y) / 2,
                                paired->leader->entity.position_z, 0, 0, 0, 0, 0, 0, 1, 0);
            party->flags.unknown_16 = 1;
            paired->flags.unknown_16 = 1;
            manager->bits.unknown_08_11 = side;
        }
    }
}
extern "C" int FieldPartyManager_IsReuniting(FieldPartyManager *manager)
{
    return (ADULT.flags.unknown_16 | BABY.flags.unknown_16) != 0;
}
extern "C" void FieldPartyManager_CompleteReunion(FieldPartyManager *manager)
{
    int member = 0;
    ADULT.flags.unknown_04 = 0;
    ADULT.flags.unknown_14 = 0;
    ADULT.state_bits.unknown_01 = 0;
    ADULT.state_bits.unknown_02 = 1;
    ADULT.state_bits.unknown_03_04 = 0;
    ADULT.flags.unknown_06 = 1;
    ADULT.state_groups.members = 3;
    BABY.flags.unknown_04 = 0;
    BABY.flags.unknown_14 = 0;
    BABY.flags.unknown_06 = 0;
    BABY.flags.unknown_10_13 = 0;
    BABY.state_bits.unknown_01 = 0;
    BABY.state_bits.unknown_02 = 1;
    BABY.follower->follower_flags->enabled = 0;
    ADULT.flags.unknown_16 = 0;
    BABY.flags.unknown_16 = 0;
    do {
        ADULT.members[member]->entity.saved_presentation_flag_bits.has_saved_resource = 0;
        ADULT.members[member]->entity.resource_index = ADULT.members[member]->unknown_595;
        BABY.members[member]->entity.saved_presentation_flag_bits.has_saved_resource = 0;
        BABY.members[member]->entity.resource_index = BABY.members[member]->unknown_595;
        BABY.leader->entity.movement_speed = 0;
        BABY.follower->entity.movement_speed = 0;
        ADULT.members[member]->presentation.unknown_05 = 0;
        ADULT.members[member]->presentation.resource_index = (s16)(!member ? 39 : 40);
        func_ov000_020a6d68(&ADULT.members[member]->entity.base,
                            ADULT.resources + 24 * ADULT.members[member]->presentation.resource_index, 0, 0,
                            -1, 1, 256);
        ADULT.members[member]->partner = BABY.members[member];
        BABY.members[member]->partner = ADULT.members[member];
        BABY.members[member]->entity.base.visibility_bits.unknown_07 = 1;
        func_020093b4(BABY.members[member]->entity.render_object, 0);
        ADULT.members[member]->bits.movement_mode = 6;
        BABY.members[member]->bits.movement_mode = 6;
        ADULT.members[member]->entity.locomotion_state = 0;
        BABY.members[member]->entity.locomotion_state = 0;
        ADULT.members[member]->entity.base.map_locomotion_state();
        BABY.members[member]->entity.base.map_locomotion_state();
        FieldEntity_SetLocomotionParameters(&ADULT.members[member]->entity, 8192, 0, 8192, -8192, 0, 8192);
        ADULT.members[member]->entity.unknown_3c8 = 0;
        ADULT.members[member]->entity.unknown_3c8 |= 64;
        if (ADULT.members[member]->presentation.behavior_saved) {
            ADULT.members[member]->entity.saved_presentation_flag_bits.behavior_mode =
                ADULT.members[member]->presentation.saved_behavior;
            ADULT.members[member]->presentation.behavior_saved = 0;
        } else
            ADULT.members[member]->entity.saved_presentation_flag_bits.behavior_mode = 3;
        if (BABY.members[member]->presentation.behavior_saved) {
            BABY.members[member]->entity.saved_presentation_flag_bits.behavior_mode =
                BABY.members[member]->presentation.saved_behavior;
            BABY.members[member]->presentation.behavior_saved = 0;
        } else
            BABY.members[member]->entity.saved_presentation_flag_bits.behavior_mode = 3;
    } while (++member < 2);
    ADULT.state_bits.unknown_03_04 = 0;
    func_ov000_0209ce18(&ADULT);
    func_ov000_0209ce44(&BABY, 0, 8);
    func_ov000_0209ce44(&BABY, 1, 8);
    if (manager->areas[BABY.flags.field_screen]->unknown_23f0 == 71) {
        BABY.state_bits.movement_mode = 14;
        BABY.unknown_08c->unknown_060 = (u8)BABY.state_bits.movement_mode;
        BABY.unknown_090->unknown_060 = (u8)BABY.state_bits.movement_mode;
        BABY.unknown_088->unknown_060 = (u8)BABY.state_bits.movement_mode;
    }
    func_ov000_0209cb90(&ADULT, 0);
    func_ov000_0209cb90(&BABY, 0);
    FieldParty_InitializeFollowing(&ADULT, 81920);
}

extern "C" void FieldPartyManager_CompleteAirborneTransfers(FieldPartyManager *manager)
{
    if (BABY.leader->entity.locomotion_state >= 57U && BABY.leader->entity.locomotion_state <= 58U)
        FieldParty_FinishAirborneTransfer(&BABY, 0);
    if (BABY.follower->entity.locomotion_state >= 57U && BABY.follower->entity.locomotion_state <= 58U)
        FieldParty_FinishAirborneTransfer(&BABY, 1);
}
extern "C" void FieldPartyManager_BeginPiggybackMount(FieldPartyManager *manager)
{
    if (ADULT.flags.movement_active) {
        if (!ADULT.flags.unknown_19 && ADULT.leader->bits.movement_mode == 6 &&
            ADULT.leader->entity.locomotion_state <= 1U &&
            !BABY.leader->entity.field_state_flag_bits.vertical_motion_active &&
            ADULT.follower->bits.movement_mode == 6 && ADULT.follower->entity.locomotion_state <= 1U &&
            !BABY.follower->entity.field_state_flag_bits.vertical_motion_active) {
            FieldParty_BeginPiggybackMount(&ADULT, 0);
            FieldParty_BeginPiggybackMount(&ADULT, 1);
            func_ov000_020a0c30(manager, 1, 0, 1, 1);
        }
    } else {
        if (ADULT.leader->bits.movement_mode == 6 && ADULT.leader->entity.locomotion_state <= 1U &&
            !BABY.leader->entity.field_state_flag_bits.vertical_motion_active)
            FieldParty_BeginPiggybackMount(&ADULT, 0);
        if (ADULT.follower->bits.movement_mode == 6 && ADULT.follower->entity.locomotion_state <= 1U &&
            !BABY.follower->entity.field_state_flag_bits.vertical_motion_active)
            FieldParty_BeginPiggybackMount(&ADULT, 1);
        func_ov000_020a0c30(manager, 1, 0, 1, 1);
    }
}
extern "C" int FieldPartyManager_IsPiggybackMountActive(FieldPartyManager *manager)
{
    return (BABY.leader->entity.locomotion_state >= 51U && BABY.leader->entity.locomotion_state <= 54U) ||
           (BABY.follower->entity.locomotion_state >= 51U && BABY.follower->entity.locomotion_state <= 54U);
}
extern "C" void FieldPartyManager_CancelReunion(FieldPartyManager *manager)
{
    if (ADULT.leader->bits.movement_mode == 6 && ADULT.leader->entity.locomotion_state <= 3U) {
        FieldParty_RestorePairedMember(&ADULT, 0);
        FieldParty_RestorePairedMember(&ADULT, 1);
        ADULT.flags.unknown_04 = 1;
        ADULT.flags.unknown_06 = 1;
        ADULT.state.unknown_00 = 1;
        ADULT.state.unknown_01 = 1;
        ADULT.state_bits.unknown_01 = 1;
        ADULT.flags.unknown_14 = 1;
        BABY.flags.unknown_04 = 1;
        BABY.flags.unknown_06 = 1;
        BABY.state.unknown_00 = 1;
        BABY.state.unknown_01 = 1;
        BABY.state_bits.unknown_01 = 1;
        BABY.flags.unknown_14 = 1;
        FieldParty_RejoinFollower(&BABY, 1);
        manager->bits.unknown_12 = 1;
    }
}
extern "C" void FieldPartyManager_PrepareBabyDrillMarker(FieldPartyManager *manager, int animation)
{
    FieldParty_RestoreMode5(&BABY, 0);
    BABY.unknown_054 = animation;
}
extern "C" void FieldPartyManager_MoveToAnchor(FieldPartyManager *manager, int side, int index)
{
    if (!side)
        FieldParty_TryFollowEntityAtHeight(&ADULT, index);
    else
        func_ov000_0208bef4(&BABY, index);
}
extern "C" void FieldPartyManager_PrepareBrosBallLaunch(FieldPartyManager *manager)
{
    FieldParty_StopBrosBallRoll(&ADULT);
}
extern "C" void FieldPartyManager_LaunchBrosBall(FieldPartyManager *manager, int direction)
{
    FieldParty_BeginBrosBallState23(&ADULT, direction);
}
extern "C" void FieldPartyManager_LaunchBabiesToElevation(FieldPartyManager *manager, fx32 height)
{
    func_ov000_0208addc(&BABY, height);
}
extern "C" void FieldPartyManager_DropBabies(FieldPartyManager *manager)
{
    FieldParty_BeginBabyDrop(&BABY);
}
