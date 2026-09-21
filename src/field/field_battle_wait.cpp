/* Wait for field party actions to settle before battle (0x0206B3A0..0x0206B85C). */

#include "field_scene_transition_internal.h"
extern "C" int FieldSystem_IsBattleEntryPending(FieldSystem *system)
{
    FieldPartyController *party;
    FieldPartyEntity *selected;
    if (SAVE->request.formation != 1) party = &system->party_manager->parties[0];
    else party = &system->party_manager->parties[1];
    selected = party->members[SAVE->request.flags.luigi];
    if (system->battle_transition.airborne_entry) FieldPartyTrail_UpdateDelayedVisibility(party, selected);
    /* Freeze the initiating member once its upward motion ends. */
    if (system->battle_transition.wait_for_apex) {
        if (selected->entity.vertical_velocity <= 0) {
            system->battle_transition.wait_for_apex = 0;
            selected->entity.base.property_00a |= 0x100;
            if (system->battle_transition.party_preparation) FieldSystem_PreparePartyForBattle(system);
            else return 0;
        }
    } else {
        switch (system->battle_transition.party_preparation) {
        case 1:
            /* The native range check adds the wrapped 16-bit negative bound. */
            if ((u16)(selected->entity.locomotion_state + (u16)-25) > 1 ||
                selected->state_record->auxiliary_animation.flags.unknown_00_10 > 1) break;
            selected->entity.base.property_00a |= 0x100;
            return 0;
        case 2:
            if (party->follower->entity.support_clearance > party->follower->entity.default_gravity &&
                party->leader->entity.position_z + party->follower->entity.default_gravity < party->follower->entity.position_z) break;
            party->leader->entity.base.property_00a |= 0x100;
            party->follower->entity.base.property_00a |= 0x100;
            /* Approaches -2 and -1 restore the member's entry resource. */
            if ((u16)(s16)(SAVE->request.flags.approach + 2) <= 1)
                FieldEntity_RebindRendererResources(&selected->entity,
                    &system->shared_primary[selected->entity.base.index + 55], 0, 0,
                    selected->entity.base_state_flag_bits.facing_direction, 1, 256);
            return 0;
        case 3:
            if (party->leader->entity.support_clearance > party->leader->entity.default_gravity &&
                party->follower->entity.support_clearance > party->follower->entity.default_gravity) break;
            party->leader->entity.base.property_00a |= 0x100;
            party->follower->entity.base.property_00a |= 0x100;
            if ((u16)(s16)(SAVE->request.flags.approach + 2) <= 1)
                FieldEntity_RebindRendererResources(&selected->entity,
                    &system->shared_primary[selected->entity.base.index + 55], 0, 0,
                    selected->entity.base_state_flag_bits.facing_direction, 1, 256);
            return 0;
        case 4:
            if (party->leader->entity.locomotion_state == 22) system->battle_transition.party_preparation = 3;
            break;
        case 5:
            if (party->leader->state_record->auxiliary_launch.elapsed_frames != 3) break;
            party->leader->entity.base.property_00a |= 0x100;
            party->follower->entity.base.property_00a |= 0x100;
            if ((u16)(s16)(SAVE->request.flags.approach + 2) <= 1)
                FieldEntity_RebindRendererResources(&selected->entity,
                    &system->shared_primary[(selected->entity.base.index & 1) + 57], 0, 0,
                    selected->entity.base_state_flag_bits.facing_direction, 1, 256);
            return 0;
        case 6:
            if (party->leader->entity.locomotion_state > 3 || party->follower->entity.locomotion_state > 3) break;
            party->leader->entity.base.property_00a |= 0x100;
            party->follower->entity.base.property_00a |= 0x100;
            if ((u16)(s16)(SAVE->request.flags.approach + 2) <= 1)
                FieldEntity_RebindRendererResources(&selected->entity,
                    &system->shared_primary[(selected->entity.base.index & 1) + 57], 0, 0,
                    selected->entity.base_state_flag_bits.facing_direction, 1, 256);
            return 0;
        }
    }
    return 1;
}
