/* Party preparation for battle entry (overlay 0, 0x0206B85C..0x0206BA2C). */

#include "field_scene_transition_internal.h"
#include <game/field_entity_motion.h>
extern "C" void FieldSystem_PreparePartyForBattle(FieldSystem *system)
{
    /* Restore a battle-compatible party action before the scene handoff. */
    FieldPartyController *party;
    FieldPartyEntity *selected;
    if (SAVE->request.formation != 1) party = &system->party_manager->parties[0];
    else party = &system->party_manager->parties[1];
    selected = party->members[SAVE->request.flags.luigi];
    switch (system->battle_transition.party_preparation) {
    case 2:
        if (!selected->bits.movement_mode) {
            if (!party->flags.special_contact_mode) FieldParty_MoveFollowerToSeparationOffset(party);
            else FieldParty_ReverseFollowerOffset(party);
        } else {
            if (!party->flags.special_contact_mode) FieldParty_SeparateFollower(party);
            else FieldParty_ReturnFollowerToLeader(party);
        }
        FieldVertical_Stop(&party->leader->entity);
        party->leader->entity.field_state_flags &= ~0x1000;
        break;
    case 3:
        FieldParty_ExitBrosBall(party);
        break;
    case 4:
        party->leader->state_record->resources.flags.unknown_00 = 1;
        break;
    case 5:
        FieldParty_BeginAuxiliaryPlacement(party);
        FieldVertical_Stop(&party->leader->entity);
        party->leader->entity.field_state_flags &= ~0x1000;
        FieldVertical_Stop(&party->follower->entity);
        party->follower->entity.field_state_flags &= ~0x1000;
        break;
    case 6:
        FieldParty_BeginRecoveryBlink(party,0);
        FieldParty_BeginRecoveryBlink(party,1);
        FieldVertical_Stop(&party->leader->entity);
        party->leader->entity.field_state_flags &= ~0x1000;
        FieldVertical_Stop(&party->follower->entity);
        party->follower->entity.field_state_flags &= ~0x1000;
        break;
    }
    /* Both members must resume field updates while their entry motion settles. */
    party->leader->entity.base.property_00a &= ~0x100;
    party->follower->entity.base.property_00a &= ~0x100;
}
