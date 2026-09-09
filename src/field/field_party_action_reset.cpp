#include <game/field_party.h>
#include <game/field_linear.h>
#include <game/field_entity_lifecycle.h>
#include <game/field_entity_motion.h>
#include <game/field_presentation.h>
extern "C" {
#include <game/audio.h>
}
#include <game/field_auxiliary.h>
#include <game/field_area.h>
#include <game/field_geometry.h>
extern "C" {
void func_ov000_02092e24(FieldPartyController *, FieldPartyEntity *);
void func_ov000_02093108(FieldPartyController *, FieldPartyEntity *, fx32, fx32 *, fx32 *);
void func_020093b4(FieldRenderObject *, int);
void func_ov000_0209ce18(FieldPartyController *);
extern const u8 data_ov000_020c0694[];
void func_ov000_020b90b4(FieldPartyEntity *, void *, FieldPartyEntity *, int, fx32);
}
#define MEMBER party->members[member]
#define ENTITY MEMBER->entity
#define FOLLOWER party->follower
#define LEADER party->leader
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))

extern "C" void FieldParty_AlignMovementToFacing(FieldPartyController *party)
{
    LEADER->entity.movement_direction = LEADER->entity.base_state_flag_bits.facing_direction << 13;
    party->flags.unknown_10_13 = data_ov000_020c0694[LEADER->entity.base_state_flag_bits.facing_direction];
}

extern "C" void FieldParty_BeginEntrySlide(FieldPartyController *party)
{
    fx32 x, y;
    FieldGeometry_GetDirectionVector(
        LEADER->entity.base_state_flag_bits.facing_direction,
        (fx32)(((s64)LEADER->entity.locomotion.maximum_speed * 65536 + 2048) >> 12), &x, &y);
    FieldEntity3D_SetPosition(&LEADER->entity, LEADER->entity.position_x - x, LEADER->entity.position_y - y,
                              LEADER->entity.position_z);
    if (party->flags.movement_active) {
        FieldEntity3D_SetPosition(&FOLLOWER->entity, FOLLOWER->entity.position_x - x,
                                  FOLLOWER->entity.position_y - y, FOLLOWER->entity.position_z);
        func_ov000_020b90b4(FOLLOWER, party->follower_state, LEADER, party->unknown_06c,
                            LEADER->entity.locomotion.starting_speed);
    }
    FieldLinear3D_Start(&LEADER->entity, 1, x, y, 0, 0, 0, 0, 0, 0, 0, 1, &LEADER->movement);
    LEADER->entity.base.visibility_bits.unknown_07 = 1;
    if (party->flags.movement_active)
        FOLLOWER->entity.base.visibility_bits.unknown_07 = 1;
    LEADER->bits.unknown_24 = 1;
}

extern "C" void FieldParty_ResetActionState(FieldPartyController *party, int immediate, int preserve_state)
{
    for (int member = 0; member < 2; ++member) {
        func_ov000_02092e24(party, MEMBER);
        if (MEMBER->presentation.behavior_saved) {
            ENTITY.saved_presentation_flag_bits.behavior_mode = MEMBER->presentation.saved_behavior;
            MEMBER->presentation.behavior_saved = 0;
            if (ENTITY.saved_presentation_flag_bits.has_saved_behavior)
                ENTITY.saved_presentation_flag_bits.saved_behavior_mode = 3;
        } else {
            ENTITY.saved_presentation_flag_bits.behavior_mode = 3;
        }
    }
    if (!preserve_state) {
        if (!party->flags.special_contact_mode) {
            GameAudio_StopEffect(312);
            if (party->paired->leader && (party->paired->leader->entity.locomotion_state < 73 ||
                                          party->paired->leader->entity.locomotion_state > 74))
                GameAudio_StopEffect(216);
        } else {
            GameAudio_StopEffect(216);
        }
        int member = 0;
        do {
            ENTITY.base.cancel_linear_movement(&MEMBER->movement, 0);
            ENTITY.render_object->state_flag_bits.behavior_state = 0;
            ENTITY.render_object->state_flag_bits.animation_suppressed = 0;
            ENTITY.locomotion_state = 0;
            MEMBER->bits.movement_mode = 0;
            ENTITY.base.map_locomotion_state();
            ENTITY.field_state_flag_bits.track_ground = 1;
            ENTITY.field_state_flag_bits.vertical_motion_paused = 0;
            ENTITY.base.reset_motion_parameters();
            ENTITY.runtime_flag_bits.unknown_07 = 0;
            ENTITY.runtime_flag_bits.unknown_21 = 1;
            ENTITY.unknown_3c8 = 0;
            ENTITY.unknown_3c8_bits.unknown_00 = 1;
            ENTITY.field_state_flag_bits.unknown_16 = 0;
            ENTITY.saved_presentation_flag_bits.unknown_15 = 0;
            ENTITY.field_state_flag_bits.body_collision_enabled = 1;
            if (MEMBER->bits.collision_faces_saved) {
                ENTITY.runtime_flag_bits.alternate_collision_faces = MEMBER->bits.saved_collision_faces;
                MEMBER->bits.collision_faces_saved = 0;
            } else {
                ENTITY.runtime_flag_bits.alternate_collision_faces = 0;
            }
            /* Each collision flag has an independent saved-value/valid pair. */
            if (MEMBER->collision_backup.has_00) {
                ENTITY.collision_state_flag_bits.unknown_00 = MEMBER->collision_backup.saved_00;
                MEMBER->collision_backup.has_00 = 0;
            } else {
                ENTITY.collision_state_flag_bits.unknown_00 = 0;
            }
            if (MEMBER->collision_backup.has_01) {
                ENTITY.collision_state_flag_bits.unknown_01 = MEMBER->collision_backup.saved_01;
                MEMBER->collision_backup.has_01 = 0;
            } else {
                ENTITY.collision_state_flag_bits.unknown_01 = 0;
            }
            if (MEMBER->collision_backup.has_02) {
                ENTITY.collision_state_flag_bits.unknown_02 = MEMBER->collision_backup.saved_02;
                MEMBER->collision_backup.has_02 = 0;
            } else {
                ENTITY.collision_state_flag_bits.unknown_02 = 0;
            }
            if (MEMBER->collision_backup.has_03) {
                ENTITY.collision_state_flag_bits.unknown_03 = MEMBER->collision_backup.saved_03;
                MEMBER->collision_backup.has_03 = 0;
            } else {
                ENTITY.collision_state_flag_bits.unknown_03 = 0;
            }
            if (MEMBER->collision_backup.has_04) {
                ENTITY.collision_state_flag_bits.unknown_04 = MEMBER->collision_backup.saved_04;
                MEMBER->collision_backup.has_04 = 0;
            } else {
                ENTITY.collision_state_flag_bits.unknown_04 = 1;
            }
            if (MEMBER->collision_backup.has_05) {
                ENTITY.collision_state_flag_bits.unknown_05 = MEMBER->collision_backup.saved_05;
                MEMBER->collision_backup.has_05 = 0;
            } else {
                ENTITY.collision_state_flag_bits.unknown_05 = 1;
            }
            if (MEMBER->collision_backup.has_06) {
                ENTITY.collision_state_flag_bits.unknown_06 = MEMBER->collision_backup.saved_06;
                MEMBER->collision_backup.has_06 = 0;
            } else {
                ENTITY.collision_state_flag_bits.unknown_06 = 1;
            }
            if (MEMBER->collision_backup.has_07) {
                ENTITY.collision_state_flag_bits.unknown_07 = MEMBER->collision_backup.saved_07;
                MEMBER->collision_backup.has_07 = 0;
            } else {
                ENTITY.collision_state_flag_bits.unknown_07 = 1;
            }
            int auxiliary = 0;
            do {
                if (MEMBER->auxiliaries[auxiliary]) {
                    MEMBER->auxiliaries[auxiliary]->entity.base.property_00a_bits.property_00a_flag_00 = 0;
                    MEMBER->auxiliaries[auxiliary]->entity.base.set_visible(0);
                    MEMBER->auxiliaries[auxiliary]->entity.base.unknown_30();
                }
                ++auxiliary;
            } while (auxiliary < 6);
            MEMBER->presentation.unknown_14 = 1;
            MEMBER->presentation.unknown_15 = 1;
            ENTITY.collision_extra_bits.unknown_23 = 0;
            MEMBER->unknown_594.unknown_01_07 = 100;
            ++member;
        } while (member < 2);
        FieldEntity_SetFacingDirection(&LEADER->entity, 0, (u32)LEADER->entity.movement_direction >> 13, 1);
        if (LEADER->entity.base.visibility_bits.unknown_07) {
            LEADER->entity.base.visibility_bits.unknown_07 = 0;
            LEADER->entity.saved_presentation_flag_bits.unknown_31 = 0;
            func_020093b4(LEADER->entity.render_object, 1);
        }
        if (FOLLOWER->entity.base.visibility_bits.unknown_07) {
            FOLLOWER->entity.base.visibility_bits.unknown_07 = 0;
            FOLLOWER->entity.saved_presentation_flag_bits.unknown_31 = 0;
            if (FOLLOWER->entity.base.property_00a_bits.property_00a_flag_00) {
                func_020093b4(FOLLOWER->entity.render_object, 1);
                if (party->flags.movement_active) {
                    fx32 offset_x, offset_y;
                    func_ov000_02093108(party, LEADER, party->unknown_06c, &offset_x, &offset_y);
                    if (((FieldAreaContext *)party->areas[party->flags.field_screen])->unknown_23f0 == 71) {
                        FieldEntity3D_SetPosition(&FOLLOWER->entity, LEADER->entity.position_x,
                                                  LEADER->entity.position_y, LEADER->entity.position_z);
                        FOLLOWER->entity.position_x += offset_x;
                        FOLLOWER->entity.position_y += offset_y;
                        FOLLOWER->entity.base.unknown_98(
                            party->areas[party->flags.field_screen]->entities[32]);
                        FieldPartyEntity *current = FOLLOWER;
                        FieldNavigationSurface *surface = current->entity.navigation_surfaces;
                        if (surface) {
                            fx32 limit =
                                current->entity.navigation_min_x +
                                MIN(current->entity.position_x, current->entity.previous_position_x) -
                                524288 - current->entity.locomotion.starting_speed;
                            while (!surface->bits.end && surface->sort_x < limit)
                                ++surface;
                        }
                        current->entity.navigation_cursor = surface;
                        current->entity.navigation_scan_limit =
                            current->entity.navigation_max_x +
                            MAX(current->entity.position_x, current->entity.previous_position_x) + 65536;
                        current->entity.swept_min_x =
                            current->entity.navigation_min_x +
                            MIN(current->entity.position_x, current->entity.previous_position_x) -
                            current->entity.locomotion.starting_speed;
                        current->entity.swept_min_y =
                            current->entity.navigation_min_y +
                            MIN(current->entity.position_y, current->entity.previous_position_y) -
                            current->entity.locomotion.starting_speed;
                        current->entity.swept_max_x =
                            current->entity.locomotion.starting_speed +
                            (current->entity.navigation_max_x +
                             MAX(current->entity.position_x, current->entity.previous_position_x));
                        current->entity.swept_max_y =
                            current->entity.locomotion.starting_speed +
                            (current->entity.navigation_max_y +
                             MAX(current->entity.position_y, current->entity.previous_position_y));
                        FOLLOWER->entity.base.unknown_9c();
                    } else {
                        FieldEntity3D_SetPosition(&FOLLOWER->entity, LEADER->entity.position_x + offset_x,
                                                  LEADER->entity.position_y + offset_y,
                                                  LEADER->entity.position_z);
                    }
                    FieldEntity_SetFacingDirection(&FOLLOWER->entity, 0,
                                                   LEADER->entity.base_state_flag_bits.facing_direction, 1);
                }
            }
        }
        party->flags.unknown_04 = 1;
        party->flags.unknown_05 = 0;
        party->flags.unknown_06 = 1;
        party->flags.unknown_14 = 1;
        party->state.unknown_00 = 1;
        party->state.unknown_01 = 1;
        party->state_bits.unknown_01 = 1;
        if (immediate)
            FieldParty_EnableFollowing(party);
        else
            FieldParty_UpdateFollowing(party);
    }
    func_ov000_0209ce18(party);
}
