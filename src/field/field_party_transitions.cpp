#include "field_party_internal.h"
#include <game/field_timed_renderer.h>
/* Spin jump, Bros. Ball, hammer swings and partner separation were observed in game.
 * The remaining state numbers retain neutral names. */
extern "C" {
#include <game/rumble.h>
extern u8 data_0205a00c;
void func_ov000_020931b0(FieldPartyController *, FieldPartyEntity *, int);
s16 func_02009224(FieldRenderObject *, int);
void func_ov000_020a8300(FieldRuntimeEntity *, fx32, int, u16, int, fx32 *, fx32 *, int);
extern const fx32 data_ov000_020c0978[][2], data_ov000_020c097c[][2];

void func_ov000_02093004(FieldPartyController *);
void func_ov000_02093dec(FieldPartyController *);
/* Interior labels for the X and Y lanes of the same interleaved table. */
extern const fx32 data_ov000_020c0878[][2], data_ov000_020c087c[][2];
void func_ov000_02092770(FieldPartyController *, FieldPartyEntity *, int, int, int, int);
void FieldParty_StackFollower(FieldPartyController *party)
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
    party->leader->entity.locomotion_state = 7;
    party->follower->entity.locomotion_state = 4;
    party->follower->follower_flags->enabled = 0;
    func_ov000_02094140(party);
    FieldVertical_StartToHeight(
        &party->follower->entity,
        party->leader->entity.position_z - party->follower->entity.position_z + 147456, -1, -1);
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, 0, 0, 0, 10, 0, 0, 1,
                                      &party->follower->movement);
    GameAudio_PlayEffectDelayed(214, 0, -1);
}

void FieldParty_RestoreGroundMovementAndHideFollower(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->leader->bits.movement_mode = 1;
    party->follower->bits.movement_mode = 1;
    party->leader->entity.locomotion_state = 0;
    party->follower->entity.locomotion_state = 0;
    func_ov000_02092f30(party, party->leader, 18, 128, 0);
    FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
    party->flags.unknown_04 = 1;
    party->flags.unknown_06 = 1;
    party->state.unknown_00 = 1;
    party->state.unknown_01 = 1;
    party->leader->entity.unknown_3c8 = 0;
    party->leader->entity.unknown_3c8 |= 0x10;
    party->follower->entity.unknown_3c8 = 0;
    party->follower->entity.unknown_3c8 |= 0x10;
    func_ov000_0209ce44(party, 0, 9);
    party->follower->entity.base.visibility_bits.unknown_07 = 1;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
}

void FieldParty_SeparateFollower(FieldPartyController *party)
{
    FieldPartyEntity *follower;
    fx32 clearance, x, y;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->leader->bits.movement_mode = 0;
    party->follower->bits.movement_mode = 0;
    party->leader->entity.locomotion_state = 9;
    party->follower->entity.locomotion_state = 8;
    clearance = party->leader->entity.navigation_vertical_extent;
    func_ov000_02092e24(party, party->leader);
    party->follower->entity.base.visibility_bits.unknown_07 = 0;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 0;
    func_020093b4(party->follower->entity.render_object, 1);
    FieldEntity3D_SetPosition(
        &party->follower->entity, party->leader->entity.position_x, party->leader->entity.position_y,
        party->leader->entity.position_z + (clearance - party->follower->entity.navigation_vertical_extent));
    follower = party->follower;
    if (!follower->presentation.behavior_saved) {
        follower->presentation.saved_behavior =
            (u16)follower->entity.saved_presentation_flag_bits.behavior_mode;
        party->follower->presentation.behavior_saved = 1;
    }
    party->follower->entity.saved_presentation_flag_bits.behavior_mode = 2;
    FieldEntity_SetFacingDirection(&party->follower->entity, 0,
                                   party->leader->entity.base_state_flag_bits.facing_direction, 1);
    func_ov000_02093108(party, party->leader, 20 * 4096, &x, &y);
    FieldVertical_StartToHeight(&party->follower->entity, 0, -1, -1);
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, x, y, 0, 10, 0, 0, 1,
                                      &party->follower->movement);
    GameAudio_PlayEffectDelayed(214, 0, -1);
}

void FieldParty_MoveFollowerToSeparationOffset(FieldPartyController *party)
{
    FieldPartyEntity *follower;
    party->flags.previous_field_screen = party->flags.field_screen;
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, -party->separation_x,
                                      -party->separation_y, 0, party->follower->movement.elapsed_frames, 0, 0,
                                      1, &party->follower->movement);
    follower = party->follower;
    if (!follower->presentation.behavior_saved) {
        follower->presentation.saved_behavior =
            (u16)follower->entity.saved_presentation_flag_bits.behavior_mode;
        party->follower->presentation.behavior_saved = 1;
    }
    party->follower->entity.saved_presentation_flag_bits.behavior_mode = 2;
    party->follower->entity.locomotion_state = 6;
}

void FieldParty_ResumeGroundMovement(FieldPartyController *party, int preserve_state)
{
    if (!preserve_state) {
        party->flags.unknown_14 = 0;
        party->state_bits.unknown_01 = 0;
        party->state_bits.unknown_03_04 = VM_ReadVariable(8193, 0, 0) | 2;
        func_ov000_0209ce18(party);
        party->follower->follower_flags->enabled = 0;
        party->leader->entity.locomotion_state = 0;
        party->follower->entity.locomotion_state = 0;
        party->leader->bits.movement_mode = 1;
        party->follower->bits.movement_mode = 1;
        party->leader->entity.base.map_locomotion_state();
        FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
        party->flags.unknown_04 = 1;
        party->flags.unknown_06 = 1;
        party->state.unknown_00 = 1;
        party->state.unknown_01 = 1;
        party->leader->entity.unknown_3c8 = 0;
        party->leader->entity.unknown_3c8 |= 0x10;
        party->follower->entity.unknown_3c8 = 0;
        party->follower->entity.unknown_3c8 |= 0x10;
    }
    func_ov000_02092f30(party, party->leader, 18, 128, 0);
    func_ov000_0209ce44(party, 0, 9);
    func_ov000_0209ce44(party, 1, 1);
    party->follower->entity.base.visibility_bits.unknown_07 = 1;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
}

void FieldParty_BeginSpinJump(FieldPartyController *party)
{
    FieldPartyEntity *leader;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->unknown_055 = -1;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->leader->entity.unknown_3c8 = 0;
    party->leader->entity.unknown_3c8 |= 0x20;
    party->follower->entity.unknown_3c8 = 0;
    party->follower->entity.unknown_3c8 |= 0x20;
    party->leader->entity.field_state_flag_bits.track_ground = 0;
    party->leader->state_record->velocity = 0x2000;
    party->leader->state_record->acceleration = 0;
    party->leader->state_record->target_z = party->leader->entity.position_z + 0xC000;
    party->leader->state_record->angle = party->leader->entity.base_state_flag_bits.facing_direction << 12;
    party->leader->state_record->angular_speed = 0x800;
    party->leader->state_record->unknown_10 = 0;
    party->leader->state_record->flags.facing_direction =
        (u16)party->leader->entity.base_state_flag_bits.facing_direction;
    leader = party->leader;
    if (!leader->presentation.behavior_saved) {
        leader->presentation.saved_behavior = (u16)leader->entity.saved_presentation_flag_bits.behavior_mode;
        party->leader->presentation.behavior_saved = 1;
    }
    party->leader->entity.saved_presentation_flag_bits.behavior_mode = 1;
    party->leader->entity.locomotion_state = 10;
    party->leader->bits.movement_mode = 2;
    party->leader->presentation.unknown_15 = 0;
    func_ov000_0209ce44(party, 1, 9);
    func_ov000_02092770(party, party->leader, 2, 4, 1, 1);
    GameAudio_PlayEffectDelayed(215, 0, -1);
}

void FieldParty_BeginState11(FieldPartyController *party)
{
    party->leader->state_record->timer = 60;
    party->leader->entity.locomotion_state = 11;
    party->flags.unknown_04 = 1;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
}

void FieldParty_BeginSpinJumpFall(FieldPartyController *party, u8 mode)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->flags.unknown_14 = 0;
    party->leader->entity.field_state_flag_bits.track_ground = 1;
    FieldVertical_Start(&party->leader->entity, 0, 1296, -1);
    party->leader->state_record->unknown_10 = 0;
    party->leader->entity.locomotion_state = 13;
    party->leader->state_record->flags.unknown_03 = mode;
}

void FieldParty_RestoreGroundMovement(FieldPartyController *party)
{
    FieldPartyEntity *leader;
    party->flags.previous_field_screen = party->flags.field_screen;
    FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
    party->flags.unknown_04 = 1;
    party->flags.unknown_06 = 1;
    party->state.unknown_00 = 1;
    party->state.unknown_01 = 1;
    party->leader->entity.unknown_3c8 = 0;
    party->leader->entity.unknown_3c8 |= 0x10;
    party->follower->entity.unknown_3c8 = 0;
    party->follower->entity.unknown_3c8 |= 0x10;
    leader = party->leader;
    if (!leader->presentation.behavior_saved) {
        leader->entity.saved_presentation_flag_bits.behavior_mode = 3;
    } else {
        leader->entity.saved_presentation_flag_bits.behavior_mode = leader->presentation.saved_behavior;
        party->leader->presentation.behavior_saved = 0;
    }
    FieldEntity_SetFacingDirection(&party->leader->entity, 0,
                                   party->leader->state_record->flags.facing_direction, 1);
    party->leader->entity.locomotion_state = 0;
    party->follower->entity.locomotion_state = 0;
    party->leader->bits.movement_mode = 1;
    party->follower->bits.movement_mode = 1;
    party->leader->presentation.unknown_15 = 1;
}

void FieldParty_ResumeState11Or17(FieldPartyController *party, int preserve_state)
{
    FieldPartyEntity *leader;
    if (!preserve_state) {
        party->flags.unknown_14 = 1;
        party->state_bits.unknown_01 = 0;
        party->state_bits.unknown_03_04 = VM_ReadVariable(8193, 0, 0) | 2;
        func_ov000_0209ce18(party);
        party->follower->follower_flags->enabled = 0;
        if (party->unknown_055 == -1) {
            party->leader->state_record->timer = 60;
            party->leader->entity.locomotion_state = 11;
        } else
            party->leader->entity.locomotion_state = 17;
        party->follower->entity.locomotion_state = 0;
        party->leader->bits.movement_mode = 1;
        party->follower->bits.movement_mode = 1;
        party->leader->entity.base.map_locomotion_state();
        if (party->unknown_055 == -1)
            FieldEntity_SetLocomotionParameters(&party->leader->entity, 0x2000, 0, 0x2000, 0, 0, 0x2000);
        else
            FieldEntity_SetLocomotionParameters(&party->leader->entity, 0x3000, 0, 0x3000, 0, 0, 0x3000);
        party->flags.unknown_04 = 1;
        party->flags.unknown_06 = 1;
        party->state.unknown_00 = 1;
        party->state.unknown_01 = 1;
        party->leader->entity.unknown_3c8 = 0;
        party->leader->entity.unknown_3c8 |= 0x20;
        party->follower->entity.unknown_3c8 = 0;
        party->follower->entity.unknown_3c8 |= 0x20;
        party->leader->entity.field_state_flag_bits.track_ground = 0;
        party->leader->state_record->angle = party->leader->entity.base_state_flag_bits.facing_direction
                                             << 12;
        if (party->unknown_055 == -1) {
            party->leader->state_record->angular_speed = 0x800;
            party->leader->state_record->unknown_10 = 0;
        } else {
            party->leader->state_record->angular_speed = 0x1000;
            party->leader->state_record->unknown_10 = 0;
        }
        party->leader->state_record->flags.facing_direction =
            (u16)party->leader->entity.base_state_flag_bits.facing_direction;
        party->leader->presentation.unknown_15 = 0;
        func_ov000_02093004(party);
    }
    func_ov000_02092f30(party, party->leader, 18, 128, 0);
    leader = party->leader;
    if (!leader->presentation.behavior_saved) {
        leader->presentation.saved_behavior = (u16)leader->entity.saved_presentation_flag_bits.behavior_mode;
        party->leader->presentation.behavior_saved = 1;
    }
    party->leader->entity.saved_presentation_flag_bits.behavior_mode = 1;
    func_ov000_0209ce44(party, 0, party->leader->entity.locomotion_state == 17 ? 30 : 9);
    func_ov000_0209ce44(party, 1, party->leader->entity.locomotion_state == 17 ? 30 : 9);
    party->follower->entity.base.visibility_bits.unknown_07 = 1;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
    func_ov000_02092770(party, party->leader, 2, 4, 1, 1);
    if (party->leader->entity.locomotion_state == 17)
        GameAudio_PlayEffectDelayed(216, 0, -1);
}
void FieldParty_TryFollowEntityAtHeight(FieldPartyController *party, int index)
{
    FieldPartyEntity *leader = party->leader;
    if (leader->entity.locomotion_state == 11 || leader->entity.locomotion_state == 12 ||
        leader->entity.locomotion_state == 18 || leader->entity.locomotion_state == 15) {
        FieldRuntimeEntity *target = party->areas[party->flags.field_screen]->entities[index];
        if (leader->entity.position_z <= target->position_z + 0x10000 &&
            leader->entity.position_z >= target->position_z - 0x10000) {
            party->flags.previous_field_screen = party->flags.field_screen;
            party->unknown_055 = index;
            party->flags.unknown_04 = 0;
            party->flags.unknown_06 = 0;
            party->flags.unknown_10_13 = 0;
            party->leader->entity.movement_speed = 0;
            party->follower->entity.movement_speed = 0;
            party->state.unknown_00 = 0;
            party->state.unknown_01 = 0;
            party->flags.unknown_14 = 0;
            FieldLinear3D_StartFollowing(&party->leader->entity, target, 0, 0, 0, 0, 0, 0, 1,
                                         &party->leader->movement);
            party->leader->entity.locomotion_state = 16;
            func_ov000_02093004(party);
            func_ov000_0209ce44(party, 0, 30);
            func_ov000_0209ce44(party, 1, 30);
            GameAudio_PlayEffectDelayed(216, 0, -1);
        }
    }
}

void FieldParty_BeginBrosBall(FieldPartyController *party)
{
    FieldPartyEntity *leader;
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
    party->leader->state_record->unknown_00[0] &= ~1u;
    party->leader->state_record->unknown_00[0] &= ~2u;
    party->leader->entity.locomotion_state = 19;
    party->leader->presentation.unknown_14 = 0;
    party->leader->presentation.unknown_15 = 0;
    party->follower->follower_flags->enabled = 0;
    party->leader->entity.saved_presentation_flag_bits.unknown_15 = 0;
    party->follower->entity.saved_presentation_flag_bits.unknown_15 = 0;
    func_ov000_02092f30(party, party->leader, 19, 256, 0);
    party->leader->entity.render_object->state_flag_bits.behavior_state = 1;
    leader = party->leader;
    if (!leader->presentation.behavior_saved) {
        leader->presentation.saved_behavior = (u16)leader->entity.saved_presentation_flag_bits.behavior_mode;
        party->leader->presentation.behavior_saved = 1;
    }
    party->leader->entity.saved_presentation_flag_bits.behavior_mode = 2;
    leader = party->leader;
    FieldEntity2D_SetPosition(
        &leader->entity,
        leader->entity.position_x +
            data_ov000_020c0878[leader->entity.base_state_flag_bits.facing_direction][0],
        leader->entity.position_y +
            data_ov000_020c087c[leader->entity.base_state_flag_bits.facing_direction][0]);
    func_ov000_02094140(party);
    party->follower->entity.base.visibility_bits.unknown_07 = 1;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
    GameAudio_PlayEffectDelayed(311, 0, -1);
}

void FieldParty_StartBrosBallRoll(FieldPartyController *party, int fast)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    FieldEntity_SetLocomotionParameters(&party->leader->entity, !fast ? 1024 : 14336, 696, 14336, 0, -1024,
                                        4096);
    party->leader->entity.saved_presentation_flag_bits.unknown_15 = 1;
    party->follower->entity.saved_presentation_flag_bits.unknown_15 = 1;
    party->leader->entity.runtime_flag_bits.unknown_07 = 1;
    party->leader->entity.runtime_flag_bits.unknown_21 = 0;
    party->flags.unknown_06 = 1;
    party->flags.unknown_04 = 0;
    party->flags.unknown_05 = 1;
    party->flags.unknown_14 = 1;
    party->leader->unknown_594.unknown_01_07 = 50;
    party->leader->state_record->unknown_00[0] &= ~1u;
    party->leader->entity.unknown_3c8 = 0;
    party->leader->entity.unknown_3c8 |= 4;
    party->leader->entity.unknown_3c8 |= 0x40000;
    party->follower->entity.unknown_3c8 = 0;
    party->follower->entity.unknown_3c8 |= 4;
    party->follower->entity.unknown_3c8 |= 0x40000;
    party->leader->bits.movement_mode = 3;
    party->follower->bits.movement_mode = 3;
    party->leader->entity.locomotion_state = 21;
    party->leader->presentation.unknown_15 = 1;
    func_ov000_02092f30(party, party->leader, 20, 256, 0);
    if (fast)
        func_ov000_02093dec(party);
    func_ov000_020922a4(party, party->leader, 61, 0, 0, 5, 9);
    GameAudio_PlayEffectDelayed(312, 0, -1);
}
void FieldParty_StopBrosBallRoll(FieldPartyController *party)
{
    FieldPartyEntity *leader;
    s16 last_frame;
    func_ov000_02091d34(party, party->leader);
    GameAudio_StopEffect(312);
    party->flags.unknown_04 = 1;
    party->flags.unknown_05 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 1;
    party->state.unknown_01 = 1;
    party->leader->state_record->unknown_00[0] &= ~1u;
    party->leader->bits.movement_mode = 3;
    party->follower->bits.movement_mode = 3;
    party->leader->entity.locomotion_state = 20;
    leader = party->leader;
    if (!leader->presentation.behavior_saved) {
        leader->presentation.saved_behavior = (u16)leader->entity.saved_presentation_flag_bits.behavior_mode;
        party->leader->presentation.behavior_saved = 1;
    }
    party->leader->entity.saved_presentation_flag_bits.behavior_mode = 5;
    func_ov000_02092f30(party, party->leader, 19, 256, 0);
    leader = party->leader;
    last_frame = func_02009224(leader->entity.render_object, -1);
    leader->entity.render_object->set_animation((u8)leader->entity.animation_id, (s16)(last_frame - 1), 1);
    party->leader->entity.render_object->state_flag_bits.behavior_state = 1;
    party->leader->entity.saved_presentation_flag_bits.unknown_15 = 0;
    party->follower->entity.saved_presentation_flag_bits.unknown_15 = 0;
}

void FieldParty_ExitBrosBall(FieldPartyController *party)
{
    FieldPartyEntity *leader, *follower;
    fx32 x, y;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->leader->unknown_594.unknown_01_07 = 100;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->leader->entity.saved_presentation_flag_bits.unknown_15 = 0;
    party->follower->entity.saved_presentation_flag_bits.unknown_15 = 0;
    party->leader->entity.render_object->state_flag_bits.behavior_state = 0;
    party->leader->entity.render_object->state_flag_bits.animation_suppressed = 0;
    party->leader->bits.movement_mode = 0;
    party->follower->bits.movement_mode = 0;
    party->leader->entity.locomotion_state = 22;
    party->follower->entity.locomotion_state = 22;
    func_ov000_02092f30(party, party->leader, 21, 256, 0);
    func_ov000_02092f30(party, party->follower, 22, 256, 0);
    party->follower->entity.base.visibility_bits.unknown_07 = 0;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 0;
    func_020093b4(party->follower->entity.render_object, 1);
    FieldEntity3D_SetPosition(&party->follower->entity, party->leader->entity.position_x,
                              party->leader->entity.position_y, party->leader->entity.position_z);
    leader = party->leader;
    if (!leader->presentation.behavior_saved) {
        leader->presentation.saved_behavior = (u16)leader->entity.saved_presentation_flag_bits.behavior_mode;
        party->leader->presentation.behavior_saved = 1;
    }
    party->leader->entity.saved_presentation_flag_bits.behavior_mode = 1;
    follower = party->follower;
    if (!follower->presentation.behavior_saved) {
        follower->presentation.saved_behavior =
            (u16)follower->entity.saved_presentation_flag_bits.behavior_mode;
        party->follower->presentation.behavior_saved = 1;
    }
    party->follower->entity.saved_presentation_flag_bits.behavior_mode = 1;
    FieldEntity_SetFacingDirection(&party->follower->entity, 0,
                                   party->leader->entity.base_state_flag_bits.facing_direction, 1);
    FieldVertical_StartToHeight(&party->leader->entity, 0x10000, -1, -1);
    FieldLinear3D_StartTimed(
        &party->leader->entity, 1,
        data_ov000_020c0978[party->leader->entity.base_state_flag_bits.facing_direction][0],
        data_ov000_020c097c[party->leader->entity.base_state_flag_bits.facing_direction][0], 0, 16, 0, 0, 1,
        &party->leader->movement);
    func_ov000_02093108(party, party->leader, 20 * 4096, &x, &y);
    FieldVertical_StartToHeight(&party->follower->entity, 0x10000, -1, -1);
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, x, y, 0, 16, 0, 0, 1,
                                      &party->follower->movement);
    GameAudio_StopEffect(312);
    GameAudio_PlayEffectDelayed(214, 0, -1);
}

void FieldParty_ResumeBrosBall(FieldPartyController *party, int preserve_state)
{
    FieldPartyEntity *leader;
    s16 last_frame;
    if (!preserve_state) {
        party->flags.unknown_04 = 1;
        party->state.unknown_00 = 1;
        party->state.unknown_01 = 1;
        party->state_bits.unknown_01 = 0;
        party->state_bits.unknown_03_04 = (VM_ReadVariable(8192, 0, 0) << 1) | 1;
        func_ov000_0209ce18(party);
        party->leader->unknown_594.unknown_01_07 = 50;
        party->leader->presentation.unknown_14 = 1;
        party->leader->entity.locomotion_state = 20;
        party->leader->bits.movement_mode = 3;
        party->follower->bits.movement_mode = 3;
        party->leader->entity.base.map_locomotion_state();
        party->follower->follower_flags->enabled = 0;
    }
    if (party->leader->entity.locomotion_state == 20) {
        func_ov000_02092f30(party, party->leader, 19, 256, 0);
        leader = party->leader;
        last_frame = func_02009224(leader->entity.render_object, -1);
        leader->entity.render_object->set_animation((u8)leader->entity.animation_id, (s16)(last_frame - 1),
                                                    1);
        party->leader->entity.render_object->state_flag_bits.behavior_state = 1;
        leader = party->leader;
        if (!leader->presentation.behavior_saved) {
            leader->presentation.saved_behavior =
                (u16)leader->entity.saved_presentation_flag_bits.behavior_mode;
            party->leader->presentation.behavior_saved = 1;
        }
        party->leader->entity.saved_presentation_flag_bits.behavior_mode = 5;
        party->leader->entity.saved_presentation_flag_bits.unknown_15 = 0;
        party->follower->entity.saved_presentation_flag_bits.unknown_15 = 0;
        if (!preserve_state)
            party->leader->presentation.unknown_15 = 0;
    } else {
        func_ov000_02092f30(party, party->leader, 20, 256, 0);
        leader = party->leader;
        if (!leader->presentation.behavior_saved) {
            leader->entity.saved_presentation_flag_bits.behavior_mode = 3;
        } else {
            leader->entity.saved_presentation_flag_bits.behavior_mode = leader->presentation.saved_behavior;
            party->leader->presentation.behavior_saved = 0;
        }
        party->leader->entity.runtime_flag_bits.unknown_07 = 1;
        party->leader->entity.runtime_flag_bits.unknown_21 = 0;
        party->leader->entity.saved_presentation_flag_bits.unknown_15 = 1;
        party->follower->entity.saved_presentation_flag_bits.unknown_15 = 1;
        if (!preserve_state)
            party->leader->presentation.unknown_15 = 1;
    }
    party->follower->entity.base.visibility_bits.unknown_07 = 1;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
    func_ov000_0209ce44(party, 0, 9);
    func_ov000_0209ce44(party, 1, 9);
    if (party->leader->entity.locomotion_state == 21) {
        func_ov000_020922a4(party, party->leader, 61, 0, 0, 5, 9);
        GameAudio_PlayEffectDelayed(312, 0, -1);
    }
}

void FieldParty_RestoreBrosBallRoll(FieldPartyController *party)
{
    FieldPartyEntity *leader;
    party->flags.unknown_04 = 1;
    party->flags.unknown_14 = 1;
    party->state.unknown_00 = 1;
    party->state.unknown_01 = 1;
    party->flags.unknown_06 = 1;
    party->leader->entity.movement_speed = party->leader->state_record->unknown_00[0];
    /* Restore the four packed bits at positions 15 through 18. */
    party->flags.unknown_10_13 = (party->leader->state_record->unknown_00[1] << 13) >> 28;
    leader = party->leader;
    if (!leader->presentation.behavior_saved) {
        leader->entity.saved_presentation_flag_bits.behavior_mode = 3;
    } else {
        leader->entity.saved_presentation_flag_bits.behavior_mode = leader->presentation.saved_behavior;
        party->leader->presentation.behavior_saved = 0;
    }
    party->leader->entity.locomotion_state = 21;
    func_ov000_02091d34(party, party->leader);
    func_ov000_020922a4(party, party->leader, 61, 0, 0, 5, 9);
    GameAudio_PlayEffectDelayed(312, 0, -1);
}

void FieldParty_BeginBrosBallState23(FieldPartyController *party, int direction)
{
    fx32 x, y;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->leader->entity.saved_presentation_flag_bits.unknown_15 = 1;
    party->follower->entity.saved_presentation_flag_bits.unknown_15 = 1;
    party->leader->entity.runtime_flag_bits.unknown_21 = 0;
    party->flags.unknown_04 = 0;
    party->flags.unknown_14 = 1;
    party->leader->state_record->unknown_00[0] &= ~1u;
    party->leader->entity.unknown_3c8 = 0;
    party->leader->entity.unknown_3c8 |= 4;
    party->leader->entity.unknown_3c8 |= 0x40000;
    party->follower->entity.unknown_3c8 = 0;
    party->follower->entity.unknown_3c8 |= 4;
    party->follower->entity.unknown_3c8 |= 0x40000;
    party->leader->entity.field_state_flag_bits.track_ground = 0;
    func_ov000_020a8300(&party->leader->entity, 24576, 0, (u16)(direction << 13), 0, &x, &y, 0);
    FieldLinear3D_Start(&party->leader->entity, 1, x, y, 0, 24576, 0, 24576, 0, 15, 15, 0,
                        &party->leader->movement);
    party->leader->entity.locomotion_state = 23;
    func_ov000_02092f30(party, party->leader, 20, 256, 0);
}
void FieldParty_BeginHammerSwing(FieldPartyController *party, int member)
{
    FieldPartyEntity *auxiliary;
    u32 priority_a, priority_b;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_14 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->members[member]->entity.movement_speed = 0;
    if (party->flags.movement_active)
        party->members[member ^ 1]->entity.movement_speed = 0;
    party->state_groups.members = (u16)(party->state_groups.members & ~(1 << member));
    party->state_bits.unknown_01 = 0;
    party->members[member]->state_record->member_action.flags.area_value_saved = 0;
    party->members[member]->entity.locomotion_state = 24;
    func_ov000_02092f30(party, party->members[member], member + 23, 256, 1);
    party->members[member]->entity.render_object->state_flag_bits.behavior_state = 1;
    if (!party->members[member]->presentation.behavior_saved) {
        party->members[member]->presentation.saved_behavior =
            (u16)party->members[member]->entity.saved_presentation_flag_bits.behavior_mode;
        party->members[member]->presentation.behavior_saved = 1;
    }
    party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 1;
    party->members[member]->entity.field_state_flag_bits.unknown_16 = 1;
    auxiliary = party->members[member]->auxiliary;
    auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 1;
    auxiliary->entity.base_state_flag_bits.animation_wait_enabled = 1;
    auxiliary->entity.runtime_flag_bits.sync_horizontal = 1;
    auxiliary->entity.runtime_flag_bits.sync_vertical = 0;
    auxiliary->entity.runtime_flag_bits.unknown_02 = 0;
    auxiliary->entity.unknown_3c8 = 0;
    auxiliary->entity.unknown_3c8 |= 0x800;
    auxiliary->entity.runtime_flag_bits.auto_priority_0 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_04 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_06 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_07 = 1;
    auxiliary->entity.field_state_flag_bits.body_collision_enabled = 1;
    auxiliary->auxiliary_bits.unknown_00 = 1;
    auxiliary->auxiliary_bits.unknown_01 = 1;
    auxiliary->auxiliary_motion.target = 0;
    auxiliary->auxiliary_bits.direction_mode = 0;
    auxiliary->auxiliary_bits.unknown_04 = 0;
    auxiliary->auxiliary_bits.unknown_05_06 = 0;
    auxiliary->auxiliary_bits.unknown_07_08 = 0;
    auxiliary->auxiliary_bits.unknown_09 = 1;
    auxiliary->entity.collision_state_flags |= 0x100000;
    auxiliary->entity.unknown_3c8 |= 0x800;
    auxiliary->entity.locomotion_state = party->members[member]->entity.locomotion_state;
    auxiliary->auxiliary_bits.unknown_19 = 0;
    auxiliary->auxiliary_bits.unknown_20_27 = member + 25;
    auxiliary->auxiliary_bits.unknown_28_31 = -1;
    auxiliary->auxiliary_motion.unknown_53c = 0;
    auxiliary->auxiliary_motion.unknown_540 = 0;
    auxiliary->auxiliary_motion.unknown_544 = 0;
    auxiliary->entity.animation_id = party->members[member]->entity.base_state_flag_bits.facing_direction;
    auxiliary->entity.saved_animation_frame = 0;
    func_ov000_020a7010(auxiliary, party->resources + 24 * (member + 25),
                        party->members[member]->entity.unknown_1e8,
                        party->members[member]->entity.unknown_1ec, 256);
    FieldTimedRenderer_SetLoopCount((FieldAnimationRenderer *)auxiliary->entity.render_object, 1);
    func_020093b4(auxiliary->entity.render_object, 1);
    auxiliary->entity.render_object->sort_key = party->members[member]->entity.render_object->sort_key;
    priority_b = party->members[member]->entity.render_object->overlap_priority_bytes[1];
    priority_a = party->members[member]->entity.render_object->overlap_priority_bytes[0];
    if (priority_a <= priority_b)
        priority_a = priority_b;
    auxiliary->entity.render_object->overlap_priority_bytes[1] = priority_a;
    auxiliary->entity.render_object->overlap_priority_bytes[0] =
        auxiliary->entity.render_object->overlap_priority_bytes[1];
    party->members[member]->state_record->unknown_00[0] |= 0x800;
    party->members[member]->state_record->unknown_00[0] &= ~0x3000u;
    party->members[member]->state_record->unknown_00[0] &= ~0x4000u;
    func_ov000_020931b0(party, party->members[member], 1);
    GameAudio_PlayEffectDelayed(54, 0, -1);
}
void FieldParty_EndHammerSwing(FieldPartyController *party, int member)
{
    FieldPartyEntity *entity;
    if (data_0205a00c)
        GameRumble_Stop();
    func_ov000_02092e24(party, party->members[member]);
    func_ov000_02091d34(party, party->members[member]);
    party->members[member]->entity.locomotion_state = 0;
    party->flags.unknown_04 = 1;
    party->flags.unknown_14 = 1;
    party->flags.unknown_06 = 1;
    party->state_groups.members = (u16)(party->state_groups.members | (1 << member));
    party->state_bits.unknown_01 = 1;
    entity = party->members[member];
    if (!entity->presentation.behavior_saved) {
        entity->entity.saved_presentation_flag_bits.behavior_mode = 3;
    } else {
        entity->entity.saved_presentation_flag_bits.behavior_mode = entity->presentation.saved_behavior;
        party->members[member]->presentation.behavior_saved = 0;
    }
    party->members[member]->entity.field_state_flag_bits.unknown_16 = 0;
    party->members[member]->entity.render_object->state_flag_bits.behavior_state = 0;
    party->members[member]->entity.render_object->state_flag_bits.animation_suppressed = 0;
    party->members[member]->auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 0;
    party->members[member]->auxiliary->entity.base.set_visible(0);
    party->members[member]->auxiliary->entity.base.unknown_30();
    if (party->members[member]->state_record->member_action.flags.area_value_saved) {
        party->areas[party->flags.previous_field_screen]->unknown_2458 =
            party->members[member]->state_record->member_action.saved_area_value;
        party->members[member]->state_record->member_action.flags.area_value_saved = 0;
    }
}
int FieldParty_IsHammerActive(FieldPartyController *party, int member)
{
    int result = 0;
    u32 state = party->members[member]->entity.locomotion_state;
    if (state < 24)
        return result;
    if (state <= 27)
        result = 1;
    return result;
}
void FieldParty_RaiseFollowerToLeader(FieldPartyController *party)
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
    party->leader->entity.locomotion_state = 31;
    party->follower->entity.locomotion_state = 28;
    *party->follower->unknown_568 &= ~1;
    func_ov000_02094140(party);
    FieldVertical_StartToHeight(
        &party->follower->entity,
        party->leader->entity.position_z - party->follower->entity.position_z + 24 * 4096, -1, -1);
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, 0, 0, 0, 10, 0, 0, 1,
                                      &party->follower->movement);
    GameAudio_PlayEffectDelayed(218, 0, -1);
}

void FieldParty_RestartAuxiliary(FieldPartyController *party)
{
    FieldPartyEntity *auxiliary;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 1;
    party->flags.unknown_06 = 1;
    party->state.unknown_00 = 1;
    party->state.unknown_01 = 1;
    party->leader->entity.unknown_3c8 = 0;
    party->leader->entity.unknown_3c8 |= 16;
    party->follower->entity.unknown_3c8 = 0;
    party->follower->entity.unknown_3c8 |= 16;
    party->leader->bits.movement_mode = 1;
    party->follower->bits.movement_mode = 1;
    party->leader->entity.locomotion_state = 0;
    party->follower->entity.locomotion_state = 0;
    func_ov000_02092f30(party, party->leader, 27, 128, 0);
    FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
    func_ov000_0209ce44(party, 0, 9);
    party->follower->entity.base.property_00a |= 0x80;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
    auxiliary = party->leader->auxiliary;
    auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 1;
    auxiliary->entity.base_state_flag_bits.animation_wait_enabled = 1;
    auxiliary->entity.runtime_flag_bits.sync_horizontal = 0;
    auxiliary->entity.runtime_flag_bits.sync_vertical = 0;
    auxiliary->entity.runtime_flag_bits.unknown_02 = 0;
    auxiliary->entity.runtime_flag_bits.auto_priority_0 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_04 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_06 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_07 = 0;
    auxiliary->entity.field_state_flag_bits.body_collision_enabled = 1;
    auxiliary->auxiliary_bits.unknown_00 = 1;
    auxiliary->auxiliary_bits.unknown_01 = 1;
    auxiliary->auxiliary_motion.target = 0;
    auxiliary->auxiliary_bits.direction_mode = 0;
    auxiliary->auxiliary_bits.unknown_04 = 1;
    auxiliary->auxiliary_bits.unknown_05_06 = 1;
    auxiliary->auxiliary_bits.unknown_07_08 = 2;
    auxiliary->auxiliary_bits.unknown_09 = 1;
    auxiliary->entity.collision_state_flags &= ~0x100000;
    auxiliary->entity.locomotion_state = party->leader->entity.locomotion_state;
    auxiliary->auxiliary_bits.unknown_19 = 0;
    auxiliary->auxiliary_bits.unknown_20_27 = 28;
    auxiliary->auxiliary_bits.unknown_28_31 = -1;
    auxiliary->auxiliary_motion.unknown_53c = 0;
    auxiliary->auxiliary_motion.unknown_540 = 0;
    auxiliary->auxiliary_motion.unknown_544 = 0;
    auxiliary->entity.animation_id = party->leader->entity.base_state_flag_bits.facing_direction;
    func_ov000_020a7010(auxiliary, party->resources + 672, party->leader->entity.unknown_1e8,
                        party->leader->entity.unknown_1ec, 128);
    auxiliary->entity.render_object->state_flag_bits.behavior_state = 0;
    auxiliary->entity.render_object->state_flag_bits.animation_suppressed = 0;
    func_020093b4(auxiliary->entity.render_object, 1);
    FieldParty_UpdateAuxiliaryFacing(party);
}

void FieldParty_ReturnFollowerToLeader(FieldPartyController *party)
{
    fx32 height, x, y;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->leader->bits.movement_mode = 0;
    party->follower->bits.movement_mode = 0;
    party->leader->entity.locomotion_state = 33;
    party->follower->entity.locomotion_state = 32;
    height = party->leader->entity.navigation_vertical_extent;
    func_ov000_02092e24(party, party->leader);
    party->follower->entity.base.property_00a &= ~0x80;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 0;
    func_020093b4(party->follower->entity.render_object, 1);
    if (!party->follower->presentation.behavior_saved) {
        party->follower->presentation.saved_behavior =
            (u16)party->follower->entity.saved_presentation_flag_bits.behavior_mode;
        party->follower->presentation.behavior_saved = 1;
    }
    party->follower->entity.saved_presentation_flag_bits.behavior_mode = 2;
    FieldEntity_SetFacingDirection(&party->follower->entity, 0,
                                   party->leader->entity.base_state_flag_bits.facing_direction, 1);
    FieldEntity3D_SetPosition(
        &party->follower->entity, party->leader->entity.position_x, party->leader->entity.position_y,
        party->leader->entity.position_z + (height - party->follower->entity.navigation_vertical_extent));
    func_ov000_02093108(party, party->leader, 16 * 4096, &x, &y);
    FieldVertical_StartToHeight(&party->follower->entity, 0, -1, -1);
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, x, y, 0, 10, 0, 0, 1,
                                      &party->follower->movement);
    GameAudio_PlayEffectDelayed(218, 0, -1);
    party->leader->auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 0;
    party->leader->auxiliary->entity.base.set_visible(0);
    party->leader->auxiliary->entity.base.unknown_30();
}

void FieldParty_ReverseFollowerOffset(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, -party->separation_x,
                                      -party->separation_y, 0, party->follower->movement.elapsed_frames, 0, 0,
                                      1, &party->follower->movement);
    if (!party->follower->presentation.behavior_saved) {
        party->follower->presentation.saved_behavior =
            (u16)party->follower->entity.saved_presentation_flag_bits.behavior_mode;
        party->follower->presentation.behavior_saved = 1;
    }
    party->follower->entity.saved_presentation_flag_bits.behavior_mode = 2;
    party->follower->entity.locomotion_state = 30;
}

void FieldParty_UpdateAuxiliaryFacing(FieldPartyController *party)
{
    party->leader->auxiliary->auxiliary_motion.target = party->leader;
    party->leader->auxiliary->auxiliary_bits.direction_mode =
        data_ov000_020c06a4[party->leader->auxiliary->entity.base_state_flag_bits.facing_direction];
    party->leader->auxiliary->auxiliary_bits.unknown_04 = 1;
    party->leader->auxiliary->auxiliary_bits.unknown_05_06 = 1;
    party->leader->auxiliary->auxiliary_bits.unknown_07_08 = 2;
}

void FieldParty_InitializeAuxiliary(FieldPartyController *party, int preserve_state)
{
    FieldPartyEntity *auxiliary;
    if (!preserve_state) {
        party->flags.unknown_14 = 0;
        party->state_bits.unknown_01 = 0;
        party->state_bits.unknown_03_04 = VM_ReadVariable(8194, 0, 0) | 2;
        func_ov000_0209ce18(party);
        *party->follower->unknown_568 &= ~1;
        party->leader->entity.locomotion_state = 0;
        party->follower->entity.locomotion_state = 0;
        party->leader->bits.movement_mode = 1;
        party->follower->bits.movement_mode = 1;
        party->leader->entity.base.map_locomotion_state();
        FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
        party->flags.unknown_04 = 1;
        party->flags.unknown_06 = 1;
        party->state.unknown_00 = 1;
        party->state.unknown_01 = 1;
        party->leader->entity.unknown_3c8 = 0;
        party->leader->entity.unknown_3c8 |= 16;
        party->follower->entity.unknown_3c8 = 0;
        party->follower->entity.unknown_3c8 |= 16;
    }
    func_ov000_02092f30(party, party->leader, 27, 128, 0);
    func_ov000_0209ce44(party, 0, 9);
    party->follower->entity.base.property_00a |= 0x80;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
    auxiliary = party->leader->auxiliary;
    auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 1;
    auxiliary->entity.base_state_flag_bits.animation_wait_enabled = 1;
    auxiliary->entity.runtime_flag_bits.sync_horizontal = 0;
    auxiliary->entity.runtime_flag_bits.sync_vertical = 0;
    auxiliary->entity.runtime_flag_bits.unknown_02 = 0;
    auxiliary->entity.runtime_flag_bits.auto_priority_0 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_04 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_06 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_07 = 0;
    auxiliary->entity.field_state_flag_bits.body_collision_enabled = 1;
    auxiliary->auxiliary_bits.unknown_00 = 1;
    auxiliary->auxiliary_bits.unknown_01 = 1;
    auxiliary->auxiliary_motion.target = 0;
    auxiliary->auxiliary_bits.direction_mode = 0;
    auxiliary->auxiliary_bits.unknown_04 = 1;
    auxiliary->auxiliary_bits.unknown_05_06 = 1;
    auxiliary->auxiliary_bits.unknown_07_08 = 2;
    auxiliary->auxiliary_bits.unknown_09 = 1;
    auxiliary->entity.collision_state_flags &= ~0x100000;
    auxiliary->entity.locomotion_state = party->leader->entity.locomotion_state;
    auxiliary->auxiliary_bits.unknown_19 = 0;
    auxiliary->auxiliary_bits.unknown_20_27 = 28;
    auxiliary->auxiliary_bits.unknown_28_31 = -1;
    auxiliary->auxiliary_motion.unknown_53c = 0;
    auxiliary->auxiliary_motion.unknown_540 = 0;
    auxiliary->auxiliary_motion.unknown_544 = 0;
    auxiliary->entity.animation_id = party->leader->entity.base_state_flag_bits.facing_direction;
    func_ov000_020a7010(auxiliary, party->resources + 672, party->leader->entity.unknown_1e8,
                        party->leader->entity.unknown_1ec, 128);
    auxiliary->entity.render_object->state_flag_bits.behavior_state = 0;
    auxiliary->entity.render_object->state_flag_bits.animation_suppressed = 0;
    func_020093b4(auxiliary->entity.render_object, 1);
    FieldParty_UpdateAuxiliaryFacing(party);
}
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
