#include <game/field_party_snapshot.h>
void FieldParty_RestoreSnapshot(FieldPartyController *party, int screen,
                                const FieldPartyControllerSnapshot *snapshot,
                                FieldResourceContext **areas, u8 *resources) {
    int i;
    party->state_groups.members = snapshot->flags.members;
    party->state_groups.unknown_02_03 = snapshot->flags.state_02_03;
    party->flags.unknown_00_01 = snapshot->flags.party_00_01;
    party->flags.unknown_02 = snapshot->flags.party_02;
    party->flags.unknown_03 = snapshot->flags.party_03;
    party->flags.unknown_04 = snapshot->flags.party_04;
    party->flags.unknown_05 = snapshot->flags.party_05;
    party->flags.unknown_06 = snapshot->flags.party_06;
    party->flags.field_screen = snapshot->flags.field_screen;
    party->flags.special_contact_mode = snapshot->flags.special_contact_mode;
    party->flags.unknown_10_13 = snapshot->flags.party_10_13;
    party->flags.unknown_14 = snapshot->flags.party_14;
    party->flags.follower_rejoin_active = snapshot->flags.follower_rejoin_active;
    party->flags.active_member = snapshot->flags.active_member;
    party->flags.movement_active = snapshot->flags.movement_active;
    party->flags.unknown_19 = snapshot->flags.party_19;
    party->flags.movement_state = snapshot->area.movement_state;
    party->unknown_002 = snapshot->unknown_006;
    party->unknown_003 = snapshot->unknown_007;
    party->input_masks[0] = snapshot->input_masks[0];
    party->input_masks[1] = snapshot->input_masks[1];
    party->unknown_054 = snapshot->unknown_00c;
    party->unknown_055 = snapshot->unknown_00d;
    party->unknown_058 = snapshot->unknown_010;
    party->unknown_05c = snapshot->unknown_014;
    party->separation_x = snapshot->separation_x;
    party->separation_y = snapshot->separation_y;
    party->separation_z = snapshot->separation_z;
    party->unknown_06c = snapshot->unknown_024;
    party->areas = areas;
    party->resources = resources;
    FieldParty_InitializeIndicator(party, snapshot->state.unknown_00 != 0, screen, 2,
                                   snapshot->state.unknown_09_12, snapshot->state.movement_mode);
    party->actions[0] = snapshot->actions[0];
    party->actions[1] = snapshot->actions[1];
    /* The native routine copies slot 2 twice and leaves slot 3 untouched. */
    party->actions[2] = snapshot->actions[2];
    party->actions[2] = snapshot->actions[2];
    party->state_bits.unknown_01 = snapshot->state.unknown_01;
    party->state_bits.unknown_02 = snapshot->state.unknown_02;
    party->state_bits.unknown_03_04 = snapshot->state.unknown_03_04;
    party->state_bits.unknown_05 = snapshot->state.unknown_05;
    party->state_bits.unknown_11_12 = snapshot->state.unknown_06_07;
    party->state_bits.unknown_13 = snapshot->state.unknown_08;
    party->backup.active = 1;
    party->backup.movement_state = party->flags.movement_state;
    party->backup.unknown_11 = snapshot->flags.backup_11;
    party->backup.snapshot_backed = 1;
    for (i = 0; i < 2; ++i)
        party->backup_snapshots[i] = &snapshot->members[i];
    party->records[0] = snapshot->records[0];
    party->records[1] = snapshot->records[1];
}
