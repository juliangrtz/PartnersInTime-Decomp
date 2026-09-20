/*
 * Spin motion and partner positioning (overlay 0, 0x020B97F8-0x020B9A68).
 * Angles wrap across eight Q12 facing directions. Rotation reads the stored
 * halfwords as signed values; the shared record also has unsigned users.
 * Apply this frame's angle before accelerating, then clamp the next speed.
 */
#include <game/field_party.h>
#include <game/field_presentation.h>
#include <game/field_entity_motion.h>
#include <game/field_entity_lifecycle.h>
#include <game/field_geometry.h>

extern "C" void FieldPartyEntity_SyncPartnerPosition(FieldPartyEntity *member)
{
    if (member->entity.locomotion_state <= 3 && member->partner) {
        if (member->partner->entity.locomotion_state <= 3) {
            FieldEntity3D_SetPosition(&member->partner->entity, member->entity.position_x,
                                      member->entity.position_y, member->entity.position_z);
        } else {
            fx32 x, y;
            FieldGeometry_GetDirectionVector(
                member->entity.base_state_flag_bits.facing_direction ^ 4, 0x4000, &x, &y);
            FieldEntity2D_SetPosition(&member->partner->entity, member->entity.position_x + x,
                                      member->entity.position_y + y);
        }
    }
}

extern "C" void FieldPartyEntity_AdvanceSlowSpin(FieldPartyEntity *member)
{
    member->state_record->angle =
        (s16)member->state_record->angle + (s16)member->state_record->angular_speed;
    member->state_record->angle = (s16)member->state_record->angle & 0x7fff;
    FieldEntity_SetFacingDirection(&member->entity, 0, (s16)member->state_record->angle / 4096, 1);
    member->state_record->angular_speed =
        (s16)member->state_record->angular_speed + member->state_record->angular_acceleration;
    if (member->state_record->angular_acceleration > 0) {
        if ((s16)member->state_record->angular_speed >= 2048) {
            member->state_record->angular_speed = 2048;
            member->state_record->angular_acceleration = 0;
        }
    } else if (member->state_record->angular_acceleration < 0) {
        if ((s16)member->state_record->angular_speed <= 0) {
            member->state_record->angular_speed = 0;
            member->state_record->angular_acceleration = 0;
        }
    }
}

extern "C" void FieldPartyEntity_AdvanceFastSpin(FieldPartyEntity *member)
{
    member->state_record->angle =
        (s16)member->state_record->angle + (s16)member->state_record->angular_speed;
    member->state_record->angle = (s16)member->state_record->angle & 0x7fff;
    FieldEntity_SetFacingDirection(&member->entity, 0, (s16)member->state_record->angle / 4096, 1);
    member->state_record->angular_speed =
        (s16)member->state_record->angular_speed + member->state_record->angular_acceleration;
    if (member->state_record->angular_acceleration > 0) {
        if ((s16)member->state_record->angular_speed >= 2867) {
            member->state_record->angular_speed = 2867;
            member->state_record->angular_acceleration = 0;
        }
    } else if (member->state_record->angular_acceleration < 0) {
        if ((s16)member->state_record->angular_speed <= 0) {
            member->state_record->angular_speed = 0;
            member->state_record->angular_acceleration = 0;
        }
    }
}
