/* Bind the placed leader/follower to their controller and state records.
 * The collision policy is a signed 64-bit field: retain both native writes.
 * Re-read the manager after stores, since the native interface permits aliasing.
 */
#include <game/field_party_manager.h>

extern "C" {

static inline void SetCollisionPolicy(FieldRuntimeEntity *entity, int enabled)
{
    int channels = enabled | (enabled << 1) | (enabled << 2);
    entity->collision_policy &= ~0x7000;
    entity->collision_policy |= channels << 12;
}
static inline void SetPartyCollisionPolicy(FieldPartyController *party)
{
    SetCollisionPolicy(&party->leader->entity, party->flags.unknown_03 != 0);
    SetCollisionPolicy(&party->follower->entity, party->flags.unknown_03 != 0);
}

void FieldArea_BindPartyMembers(FieldAreaContext *area, int side, int force)
{
    FieldPartyEntity *leader =
        (FieldPartyEntity *)area->entities[area->party->parties[side].unknown_002];
    FieldPartyEntity *follower =
        (FieldPartyEntity *)area->entities[area->party->parties[side].unknown_003];
    area->party->parties[side].leader = leader;
    area->party->parties[side].follower = follower;
    leader->linked_member = follower;
    follower->linked_member = leader;
    leader->state_record = &area->party->parties[side].records[0];
    follower->state_record = &area->party->parties[side].records[1];
    area->party->parties[side].flags.unknown_04 = 1;
    SetPartyCollisionPolicy(&area->party->parties[side]);
    if (!area->party->parties[side].flags.unknown_03 && leader->bits.movement_mode != 6)
        FieldParty_StartLeaderPaletteAnimation(&area->party->parties[side], force);
    else
        FieldParty_ClearLeaderPaletteAnimation(&area->party->parties[side], force);
}
}
