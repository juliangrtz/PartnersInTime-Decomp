/* Maintain party locomotion and synchronize the partner in movement mode six. */
#include <game/field_party.h>
#include <game/field_entity_motion.h>
extern "C" void FieldPartyEntity_UpdateLocomotionState(FieldPartyEntity *member)
{
    if (member->entity.base.property_00a_bits.property_00a_flag_00) {
        if (member->entity.locomotion_state <= 3) {
            if (!member->entity.base.property_00a_bits.subtype || member->bits.movement_mode != 6) {
                FieldEntity3D_UpdateLocomotionState(&member->entity);
                if (!member->entity.base.property_00a_bits.subtype &&
                    member->bits.movement_mode == 6 &&
                    member->partner->entity.locomotion_state <= 3)
                    member->partner->entity.locomotion_state = member->entity.locomotion_state;
            }
        } else if (member->entity.locomotion_state == 57 || member->entity.locomotion_state == 70) {
            if (member->entity.position_z < member->entity.previous_position_z)
                ++member->entity.locomotion_state;
        }
        member->entity.base.map_locomotion_state();
    }
}

extern "C" {
extern const u8 data_ov000_020c0c00[];
void FieldPartyEntity_MapLocomotionState(FieldPartyEntity *member)
{
    member->entity.locomotion_category = data_ov000_020c0c00[member->entity.locomotion_state];
    if (member->entity.locomotion_category == 1 && member->entity.field_state_flag_bits.unknown_07)
        member->entity.locomotion_category = 0;
    switch (member->entity.locomotion_category) {
    case 0:
        if (!member->entity.base.property_00a_bits.subtype) {
            switch (member->bits.movement_mode) {
            case 1:
                member->entity.locomotion_category = 2;
                break;
            case 3:
                member->entity.locomotion_category = 6;
                break;
            case 6:
                member->entity.locomotion_category = 27;
                break;
            }
        } else {
            switch (member->bits.movement_mode) {
            case 1:
                member->entity.locomotion_category = 10;
                break;
            case 4:
                member->entity.locomotion_category = 14;
                break;
            case 5:
                member->entity.locomotion_category = 11;
                break;
            case 8:
                if (member->state_record->resources.flags.unknown_20_21 != 2 &&
                    member->linked_member->state_record->resources.flags.unknown_20_21 != 2)
                    member->entity.locomotion_category = 21;
                else
                    member->entity.locomotion_category = 8;
                break;
            default:
                member->entity.locomotion_category = 8;
                break;
            }
        }
        break;
    case 1:
        if (!member->entity.base.property_00a_bits.subtype) {
            switch (member->bits.movement_mode) {
            case 1:
                member->entity.locomotion_category = 2;
                break;
            case 3:
                member->entity.locomotion_category = 6;
                break;
            case 6:
                member->entity.locomotion_category = 28;
                break;
            }
        } else {
            switch (member->bits.movement_mode) {
            case 1:
                member->entity.locomotion_category = 10;
                break;
            case 4:
                member->entity.locomotion_category = 14;
                break;
            case 5:
                member->entity.locomotion_category = 11;
                break;
            case 8:
                if (member->state_record->resources.flags.unknown_20_21 != 2 &&
                    member->linked_member->state_record->resources.flags.unknown_20_21 != 2)
                    member->entity.locomotion_category = 21;
                else
                    member->entity.locomotion_category = 9;
                break;
            case 6:
                member->entity.locomotion_category = 29;
                break;
            default:
                member->entity.locomotion_category = 9;
                break;
            }
        }
        break;
    }
}
}
