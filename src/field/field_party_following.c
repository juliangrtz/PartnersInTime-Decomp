#include <game/field_party.h>
#include <game/field_entity_lifecycle.h>
#include <game/field_entity_motion.h>
#include <game/field_presentation.h>
#include <game/field_linear.h>
static inline void SetCollisionPolicy(FieldRuntimeEntity *entity, int shift, int policy)
{
    entity->collision_policy &= ~(s32)(7u << shift);
    if (policy)
        entity->collision_policy |= (s32)((u32)policy << shift);
}
extern void func_ov000_020b941c(FieldPartyEntity *, void *, FieldPartyEntity *);
extern void func_ov000_02093108(FieldPartyController *, FieldPartyEntity *, fx32, fx32 *, fx32 *);
void FieldParty_RejoinFollower(FieldPartyController *party, int instant)
{
    fx32 x, y;
    if (party->flags.special_contact_mode == 1 && party->leader->bits.movement_mode == 6) {
        FieldParty_EnableFollowing(party);
        return;
    }
    func_ov000_02093108(party, party->leader, party->unknown_06c, &x, &y);
    if (instant) {
        FieldEntity3D_SetPosition(&party->follower->entity, party->leader->entity.position_x + x,
                                  party->leader->entity.position_y + y, party->leader->entity.position_z);
        FieldEntity_SetFacingDirection(&party->follower->entity, 0,
                                       party->leader->entity.base_state_flag_bits.facing_direction, 1);
        FieldParty_EnableFollowing(party);
        return;
    }
    if (!FieldLinear3D_StartFollowing(&party->follower->entity, &party->leader->entity, x, y, 0, 8192, 0, 0,
                                      1, &party->follower->movement)) {
        FieldEntity_SetFacingDirection(&party->follower->entity, 0,
                                       party->leader->entity.base_state_flag_bits.facing_direction, 1);
        FieldParty_EnableFollowing(party);
        return;
    }
    party->flags.follower_rejoin_active = 1;
    party->follower->bits.unknown_23 = 1;
}
int FieldParty_CheckFollowerRejoin(FieldPartyController *party)
{
    if (party->flags.follower_rejoin_active) {
        party->flags.follower_rejoin_active = party->follower->bits.unknown_23;
        if (!party->flags.follower_rejoin_active) {
            FieldEntity_SetFacingDirection(&party->follower->entity, 0,
                                           party->leader->entity.base_state_flag_bits.facing_direction, 1);
            FieldParty_EnableFollowing(party);
        }
    }
    return party->flags.follower_rejoin_active != 0;
}
void FieldParty_EnableFollowing(FieldPartyController *party)
{
    party->flags.movement_active = 1;
    party->leader->entity.collision_category_bits.categories &=
        ~(1 << party->leader->entity.base.property_00a_bits.subtype);
    party->follower->entity.collision_category_bits.categories &=
        ~(1 << party->follower->entity.base.property_00a_bits.subtype);
    FieldParty_InitializeFollowing(party, 0);
}
void FieldParty_UpdateFollowing(FieldPartyController *party)
{
    func_ov000_020b941c(party->follower, party->follower_state, party->leader);
    if (!party->flags.special_contact_mode || party->leader->bits.movement_mode != 6) {
        party->follower->follower_flags->enabled = (u16)party->flags.movement_active;
    }
}
extern void func_ov000_020b90b4(FieldPartyEntity *, void *, FieldPartyEntity *, int, fx32);
void FieldParty_InitializeFollowing(FieldPartyController *party, int argument)
{
    if (!argument)
        argument = party->unknown_06c;
    else
        party->unknown_06c = argument;
    if (party->flags.movement_active) {
        const FieldPartyEntity *leader;
        FieldParty_ResetFollowingContacts(party);
        leader = party->leader;
        FieldEntity_SetLocomotionParameters(
            &party->follower->entity, leader->entity.locomotion.starting_speed,
            leader->entity.locomotion.acceleration, leader->entity.locomotion.maximum_speed,
            leader->entity.locomotion.deceleration, leader->entity.locomotion.reverse_deceleration,
            leader->entity.locomotion.turn_speed_limit);
        party->follower->entity.animation_speed = party->leader->entity.animation_speed;
        func_ov000_020b90b4(party->follower, party->follower_state, party->leader, argument,
                            party->leader->entity.locomotion.starting_speed);
    }
}
void FieldParty_ResetFollowingContacts(FieldPartyController *party)
{
    party->leader->entity.collision_state_flags &= ~1;
    party->leader->entity.collision_state_flags &= ~2;
    party->leader->entity.collision_state_flags &= ~4;
    party->leader->entity.collision_state_flags &= ~8;
    party->follower->entity.collision_state_flags &= ~1;
    party->follower->entity.collision_state_flags &= ~2;
    party->follower->entity.collision_state_flags &= ~4;
    party->follower->entity.collision_state_flags &= ~8;
    SetCollisionPolicy(&party->follower->entity, 6, 0);
    SetCollisionPolicy(&party->follower->entity, 21, 5);
}
void FieldParty_DisableFollowing(FieldPartyController *party)
{
    SetCollisionPolicy(&party->follower->entity, 6, 7);
    SetCollisionPolicy(&party->follower->entity, 21, 7);
    if (party->follower->unknown_568) {
        *party->follower->unknown_568 &= ~1;
    }
    party->flags.movement_active = 0;
    party->follower->entity.movement_velocity_y = 0;
    party->follower->entity.movement_speed = party->follower->entity.movement_velocity_y;
    party->leader->entity.collision_category_bits.categories |=
        1 << party->leader->entity.base.property_00a_bits.subtype;
    party->follower->entity.collision_category_bits.categories |=
        1 << party->follower->entity.base.property_00a_bits.subtype;
}
