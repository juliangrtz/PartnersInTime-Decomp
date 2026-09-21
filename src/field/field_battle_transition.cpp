/* Field-to-battle preparation, wait gates and handoff (0x0206B1FC..0x0206BA2C). */

#include "field_scene_transition_internal.h"
#include <game/field_entity_motion.h>
#include <game/field_background.h>

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

struct FieldBattleScrollSaveView {
    u8 prefix[1380];
    s16 x[6], y[6];
};
typedef char FieldBattleScrollSaveView_SizeCheck[sizeof(FieldBattleScrollSaveView) == 1404 ? 1 : -1];

extern "C" void FieldSystem_PrepareBattleScene(FieldSystem *system)
{
    int index;
    FieldPartyController *party;
    FieldPartyEntity *member;
    /* Save each screen's three Q8 layer offsets before its background is released. */
    for (index = 0; index < 6; ++index) {
        ((FieldBattleScrollSaveView *)gSaveData)->x[index] =
            system->areas[index / 3]->background->scroll_x[index % 3] >> 8;
        ((FieldBattleScrollSaveView *)gSaveData)->y[index] =
            system->areas[index / 3]->background->scroll_y[index % 3] >> 8;
    }
    if (SAVE->request.formation != 1) party = &system->party_manager->parties[0];
    else party = &system->party_manager->parties[1];
    member = party->members[SAVE->request.flags.luigi];
    if (system->battle_transition.airborne_entry)
        FieldPartyTrail_StopCopies(party, member);
    switch (system->battle_transition.party_preparation) {
    case 1:
        FieldParty_EndHammerSwing(party, member->entity.base.index & 1);
        break;
    case 2:
    case 3:
    case 5:
    case 6:
        FieldParty_ResetActionState(party, 1, 0);
        break;
    }
    func_ov000_0206a130(system);
    /* Keep the two stores: the shared teardown first publishes its own flags. */
    data_0205a01c.first = 23;
    data_0205a01c.second = 46;
}
