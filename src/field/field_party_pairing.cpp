#include "field_party_internal.h"

extern "C" {

void FieldParty_PrepareCollisionOverride(FieldPartyController *party, int preserve_state)
{
    if (!preserve_state) {
        party->flags.unknown_14 = 0;
        party->state_bits.unknown_01 = 0;
        party->state_bits.unknown_03_04 = VM_ReadVariable(8194, 0, 0) | 2;
        func_ov000_0209ce18(party);
        *party->follower->unknown_568 &= ~1;
        party->leader->entity.locomotion_state = 0;
        party->follower->entity.locomotion_state = 0;
        party->leader->bits.movement_mode = 4;
        party->follower->bits.movement_mode = 4;
        party->leader->entity.base.map_locomotion_state();
        FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
        party->flags.unknown_04 = 1;
        party->flags.unknown_06 = 1;
        party->state.unknown_00 = 1;
        party->state.unknown_01 = 1;
        party->leader->entity.unknown_3c8 = 0;
        party->leader->entity.unknown_3c8 |= 2;
        party->follower->entity.unknown_3c8 = 0;
        party->follower->entity.unknown_3c8 |= 2;
        party->leader->bytes.saved_collision = party->leader->entity.collision_flag_bytes.current;
        party->leader->bytes.collision_saved = 255;
        party->leader->entity.collision_flag_bytes.current = 0;
        if (!party->leader->bits.collision_faces_saved) {
            party->leader->bits.saved_collision_faces =
                party->leader->entity.runtime_flag_bits.alternate_collision_faces;
            party->leader->bits.collision_faces_saved = 1;
        }
        party->leader->entity.runtime_flag_bits.alternate_collision_faces = 1;
    }
    party->leader->entity.field_state_flag_bits.body_collision_enabled = 0;
    party->leader->entity.render_object->sort_key =
        (party->leader->entity.render_object->sort_key & 0xf000003f) | 0x3ffffc0;
    FieldParty_BindActionResource(party, party->leader, 30, 128, 0);
    func_ov000_0209ce44(party, 0, 5);
    func_ov000_0209ce44(party, 1, 5);
    party->follower->entity.base.property_00a |= 0x80;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
    FieldPartyTrail_StartEmitter(party, party->leader, 62, 0, 0, 5, 10);
}

void FieldParty_WaitForPairedFollowing(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_14 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->state_bits.unknown_01 = 0;
    party->state_bits.unknown_02 = 1;
    party->flags.unknown_16 = 1;
    party->leader->entity.locomotion_state = 42;
    party->follower->entity.locomotion_state = 42;
    party->leader->bits.movement_mode = 6;
    party->follower->bits.movement_mode = 6;
}

void FieldParty_StartPairedFollowing(FieldPartyController *party)
{
    fx32 x, y;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_14 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->state_bits.unknown_01 = 0;
    party->state_bits.unknown_02 = 1;
    party->flags.unknown_16 = 1;
    party->leader->entity.locomotion_state = 41;
    party->follower->entity.locomotion_state = 41;
    party->leader->bits.movement_mode = 6;
    party->follower->bits.movement_mode = 6;
    *party->follower->unknown_568 &= ~1;
    func_ov000_02093108(party, party->paired->leader, 8 * 4096, &x, &y);
    party->leader->entity.field_state_flag_bits.track_ground = 0;
    party->follower->entity.field_state_flag_bits.track_ground = 0;
    FieldLinear3D_StartTimedFollowing(&party->leader->entity, &party->paired->leader->entity, x, y, 0, 8, 0,
                                      0, 1, &party->leader->movement);
    func_ov000_02093108(party, party->paired->follower, 8 * 4096, &x, &y);
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->paired->follower->entity, x, y, 0, 8,
                                      0, 0, 1, &party->follower->movement);
    party->state_bits.movement_mode = 14;
}

void FieldParty_LinkPairedMember(FieldPartyController *party, int member)
{
    FieldPartyEntity *entity = party->members[member];
    FieldPartyEntity *paired = party->paired->members[member];
    party->flags.unknown_06 = 1;
    entity->partner = paired;
    paired->partner = entity;
    entity->entity.unknown_3c8 = 0;
    entity->entity.unknown_3c8 |= 0x40;
    entity->entity.locomotion_state = 0;
    paired->entity.locomotion_state = 0;
    entity->bits.movement_mode = 6;
    paired->bits.movement_mode = 6;
    entity->entity.base.map_locomotion_state();
    paired->entity.base.map_locomotion_state();
    FieldParty_BindActionResource(party, entity, member + 39, 256, 0);
    FieldEntity_SetLocomotionParameters(&entity->entity, 8192, 0, 8192, -8192, 0, 8192);
    paired->entity.field_state_flag_bits.track_ground = 1;
    paired->entity.base.property_00a |= 0x80;
    paired->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(paired->entity.render_object, 0);
    party->state_bits.unknown_03_04 = 0;
    func_ov000_0209ce18(party);
    func_ov000_0209ce44(party->paired, member, 8);
    party->paired->state_bits.movement_mode = 14;
    party->paired->unknown_08c->unknown_060 = (u8)party->paired->state_bits.movement_mode;
    party->paired->unknown_090->unknown_060 = (u8)party->paired->state_bits.movement_mode;
    party->paired->unknown_088->unknown_060 = (u8)party->paired->state_bits.movement_mode;
}
}
