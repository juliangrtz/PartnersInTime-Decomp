/* Prepare up to four entities for time-hole arrival (0x02069B24..0x02069D38). */

extern "C" {
#include <nitro/fx.h>
#include <nitro/fx_atan.h>
}
#include <game/field_system.h>
#include <game/field_entity_motion.h>
#include <game/field_presentation.h>
extern "C" void func_ov000_02066e60(FieldSystem *, FieldRuntimeEntity *);

extern "C" void FieldSystem_PrepareTimeHoleArrival(
    FieldSystem *system, int *indices, int target, const int *directions)
{
    FieldRuntimeEntity *anchor = system->areas[0]->entities[target];
    int i;

    FieldSystem_PrepareTimeHoleEntity(system, anchor);
    system->time_hole.flags.mode = 1;
    system->time_hole.participants.mask = 0;
    for (i = 0; i < 4; ++i) {
        if (indices[i] != -1) {
            FieldRuntimeEntity *member = system->areas[0]->entities[indices[i]];
            /* Babies in movement mode 6 are removed from the caller's list. */
            if (member->base.property_00a_bits.subtype == 1 &&
                ((FieldPartyEntity *)member)->bits.movement_mode == 6) {
                indices[i] = -1;
            } else {
                fx32 x, y;
                system->time_hole.arrival_directions[i] = directions[i];
                member->saved_presentation_flag_bits.saved_behavior_mode =
                    member->saved_presentation_flag_bits.behavior_mode;
                member->saved_presentation_flag_bits.has_saved_behavior = 1;
                FieldEntity_SetBehaviorMode(&member->base, 1);
                if (member->base.property_00a_bits.subtype <= 1)
                    func_ov000_02066e60(system, member);
                member->base.set_visible(0);
                member->base.property_00a_bits.property_00a_flag_00 = 0;
                FieldEntity3D_SetPosition(member,
                    anchor->position_x, anchor->position_y, anchor->position_z);
                FieldEntity_SetFacingDirection(member, 0, 4, 1);
                x = member->position_x - system->time_hole.entity_x;
                y = member->position_y - system->time_hole.entity_y;
                system->time_hole.participant_angles[i] = FX_Atan2Idx(y, x) + 0x4000;
                /* Round each squared Q12 component before adding them. */
                system->time_hole.participant_radii[i] = FX_Sqrt(
                    (fx32)(((s64)x * x + 2048) >> 12) +
                    (fx32)(((s64)y * y + 2048) >> 12));
                system->time_hole.participants.mask |= 1 << i;
            }
        }
        system->time_hole.participant_indices[i] = indices[i];
    }
}
