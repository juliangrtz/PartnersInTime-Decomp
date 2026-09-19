/* Party placement initializes adult/baby motion and owned auxiliary entities.
 * The room placement record is required by the collision setup. */
extern "C" {
#include <game/heap.h>
}
#include <game/field_auxiliary.h>
#include <game/field_party.h>
extern "C" {
extern FieldEntityVTable data_ov000_020c1244;
extern const u8 data_ov000_020c0b74[];
void func_0202cbd4(void *, int, u32);

FieldPartyEntity *FieldPartyEntity_InitPlacement(FieldPartyEntity *party, int index,
                                                 const FieldSpawnRecord *spawn, int screen,
                                                 int resource_set, u8 enabled, int baby)
{
    FieldEntity3D_InitPlacement(&party->entity, index, spawn, screen, resource_set, enabled);
    *(FieldEntityVTable **)party = &data_ov000_020c1244;
    func_0202cbd4((u8 *)party + sizeof(FieldRuntimeEntity), 0,
                  sizeof(*party) - sizeof(FieldRuntimeEntity));
    party->entity.collision_state_flag_bits.unknown_05 =
        party->entity.collision_state_flag_bits.unknown_04;
    party->entity.collision_state_flag_bits.unknown_07 = spawn->flags.bits.collision_flag_06;
    party->entity.collision_flag_bytes.saved = party->entity.collision_flag_bytes.current;
    party->entity.base_state_flags |= 0x200;
    party->entity.collision_state_flag_bits.reserved_0 = 0;
    party->entity.collision_state_flag_bits.reserved_1 = 0;
    party->bits.movement_mode = 0;
    party->bytes.saved_collision = 0;
    party->bits.collision_faces_saved = 0;
    party->entity.initial_locomotion.starting_speed = 8192;
    party->entity.initial_locomotion.acceleration = 0;
    party->entity.initial_locomotion.maximum_speed = 8192;
    party->entity.initial_locomotion.deceleration = -8192;
    party->entity.initial_locomotion.reverse_deceleration = 0;
    party->bits.unknown_22 = baby;
    if (!baby) {
        party->entity.initial_vertical_launch_velocity = 21496;
        party->entity.initial_gravity = 1296;
        party->entity.initial_terminal_fall_velocity = 0;
    } else {
        party->entity.initial_vertical_launch_velocity = 15772;
        party->entity.initial_gravity = 1076;
        party->entity.initial_terminal_fall_velocity = 0;
    }
    party->entity.base.reset_motion_parameters();
    if (!baby)
        party->entity.unknown_370 = 10;
    else
        party->entity.unknown_370 = 7;
    party->entity.animation_speed = 256;
    party->presentation.unknown_05 = 0;
    party->presentation.resource_index = -1;
    party->presentation.unknown_14 = 1;
    party->presentation.unknown_15 = 1;
    party->unknown_594.unknown_00 = 1;
    party->unknown_594.unknown_01_07 = 100;
    party->unknown_595 = party->entity.resource_index;
    party->unknown_596 = -1;
    party->unknown_597 = -1;
    party->paired_bounds_index = -1;
    party->previous_paired_bounds_index = -1;
    party->movement.bits.active_axes = 0;
    party->linked_member = 0;
    party->follower_flags = 0;
    party->unknown_56c_word = 0;
    if (spawn) {
        for (int i = 0; i < data_ov000_020c0b74[index]; ++i) {
            FieldAuxiliaryEntity *aux = (FieldAuxiliaryEntity *)GameHeap_New(
                sizeof(FieldAuxiliaryEntity), party->entity.base.property_00a_bits.heap, 0, 1);
            if (aux)
                aux = FieldAuxiliary_InitPlacement(aux, index, screen, resource_set, i,
                                                   &party->entity);
            party->auxiliaries[i] = aux;
        }
    }
    return party;
}
}
