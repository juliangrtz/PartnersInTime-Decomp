#include "field_party_internal.h"

extern "C" {

void FieldParty_BeginState34(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->leader->entity.locomotion_state = 34;
    party->leader->bits.movement_mode = 4;
    party->follower->bits.movement_mode = 4;
    party->leader->auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 0;
    party->leader->auxiliary->entity.base_state_flag_bits.animation_wait_enabled = 0;
    func_020093b4(party->leader->auxiliary->entity.render_object, 0);
    func_ov000_02092f30(party, party->leader, 29, 256, 0);
    FieldEntity_SetResourceAnimation(&party->leader->entity, 0,
                                     party->leader->entity.base_state_flag_bits.facing_direction, 1, 1);
    party->leader->entity.render_object->state_flag_bits.behavior_state = 1;
    if (!party->leader->presentation.behavior_saved) {
        party->leader->presentation.saved_behavior =
            (u16)party->leader->entity.saved_presentation_flag_bits.behavior_mode;
        party->leader->presentation.behavior_saved = 1;
    }
    party->leader->entity.saved_presentation_flag_bits.behavior_mode = 1;
    party->leader->auxiliary->entity.base.unknown_30();
    func_ov000_02091e58(party, party->leader, party->leader->entity.position_x,
                        party->leader->entity.position_y, party->leader->entity.position_z, 62, 0, 8, 0,
                        data_ov000_020c0684[party->leader->entity.base_state_flag_bits.facing_direction] + 16,
                        -1, data_ov000_020c06ac);
    GameAudio_PlayEffectDelayed(221, 0, -1);
}

void FieldParty_RestartCollisionOverride(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
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
    party->leader->bits.saved_collision_faces =
        party->leader->entity.runtime_flag_bits.alternate_collision_faces;
    party->leader->bits.collision_faces_saved = 1;
    party->leader->entity.runtime_flag_bits.alternate_collision_faces = 1;
    party->leader->entity.field_state_flag_bits.body_collision_enabled = 0;
    party->leader->entity.render_object->sort_key =
        (party->leader->entity.render_object->sort_key & 0xf000003f) | 0x3ffffc0;
    party->leader->entity.locomotion_state = 0;
    party->follower->entity.locomotion_state = 0;
    func_ov000_02092f30(party, party->leader, 30, 128, 0);
    party->leader->entity.render_object->state_flag_bits.animation_suppressed = 1;
    FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
    func_ov000_0209ce44(party, 0, 5);
    func_ov000_0209ce44(party, 1, 5);
    func_ov000_020922a4(party, party->leader, 62, 0, 0, 5, 10);
}

void FieldParty_RestoreCollisionState(FieldPartyController *party, int check_contacts)
{
    func_ov000_02091d34(party, party->leader);
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->leader->entity.field_state_flag_bits.body_collision_enabled = 1;
    party->leader->entity.locomotion_state = 36;
    party->follower->entity.locomotion_state = 37;
    if (party->leader->bytes.collision_saved) {
        party->leader->entity.collision_flag_bytes.current = party->leader->bytes.saved_collision;
        party->leader->bytes.collision_saved = 0;
    }
    if (party->leader->bits.collision_faces_saved) {
        party->leader->entity.runtime_flag_bits.alternate_collision_faces =
            party->leader->bits.saved_collision_faces;
        party->leader->bits.collision_faces_saved = 0;
    }
    party->leader->entity.collision_state_flags |= 0x800000;
    party->follower->entity.collision_state_flags |= 0x800000;
    party->leader->entity.unknown_3c8 = 0;
    party->leader->entity.unknown_3c8_bits.unknown_00 = 1;
    party->follower->entity.unknown_3c8 = 0;
    party->follower->entity.unknown_3c8_bits.unknown_00 = 1;
    party->leader->entity.base.property_00a |= 0x80;
    party->leader->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->leader->entity.render_object, 0);
    party->follower->entity.base.property_00a &= ~0x80;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 0;
    func_020093b4(party->follower->entity.render_object, 1);
    FieldEntity3D_SetPosition(&party->follower->entity, party->leader->entity.position_x,
                              party->leader->entity.position_y, party->leader->entity.position_z);
    if (!party->follower->presentation.behavior_saved) {
        party->follower->presentation.saved_behavior =
            (u16)party->follower->entity.saved_presentation_flag_bits.behavior_mode;
        party->follower->presentation.behavior_saved = 1;
    }
    party->follower->entity.saved_presentation_flag_bits.behavior_mode = 1;
    FieldEntity_SetFacingDirection(&party->follower->entity, 0,
                                   party->leader->entity.base_state_flag_bits.facing_direction, 1);
    func_ov000_02092f30(party, party->follower, 32, 256, 0);
    FieldEntity_SetResourceAnimation(&party->follower->entity,
                                     party->follower->entity.base_state_flag_bits.facing_direction, 0, 1, 1);
    party->follower->entity.render_object->state_flag_bits.behavior_state = 1;
    if (check_contacts) {
        void *contact = func_ov000_0207f080(party->areas[party->flags.field_screen], party->leader);
        if (contact) {
            func_ov000_0207ef18(party->areas[party->flags.field_screen], contact);
        }
        func_ov000_0208dbd4(party, party->areas[party->flags.field_screen]->entities,
                            party->areas[party->flags.field_screen]->entity_count);
    }
    func_ov000_02091e58(party, party->leader, party->leader->entity.position_x,
                        party->leader->entity.position_y, party->leader->entity.position_z, 62, 0, 9, 0, 2,
                        -1, data_ov000_020c068c);
    GameAudio_PlayEffectDelayed(228, 0, -1);
}
}
