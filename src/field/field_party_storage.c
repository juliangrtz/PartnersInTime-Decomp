#include <game/field_party_snapshot.h>
extern FieldPartyEntity *func_ov000_020bc354(FieldPartyEntity *);
extern FieldPartyEntity *func_ov000_020bc3b0(FieldPartyEntity *);
extern void func_020488bc(void *, int, u32, FieldPartyEntity *(*)(FieldPartyEntity *));
extern void func_020489e0(void *, int, u32, FieldPartyEntity *(*)(FieldPartyEntity *),
                          FieldPartyEntity *(*)(FieldPartyEntity *));
extern void func_0202cbd4(void *, int, u32);
FieldPartyController *FieldParty_InitializeStorage(FieldPartyController *party)
{
    func_020489e0(party->backup_entities, 2, sizeof(FieldPartyEntity), func_ov000_020bc3b0,
                  func_ov000_020bc354);
    func_0202cbd4(party, 0, sizeof(*party));
    party->unknown_055 = -1;
    return party;
}
FieldPartyController *FieldParty_DestroyBackups(FieldPartyController *party)
{
    func_020488bc(party->backup_entities, 2, sizeof(FieldPartyEntity), func_ov000_020bc354);
    return party;
}

extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);

void FieldParty_SaveSnapshot(FieldPartyController *party, FieldPartyControllerSnapshot *snapshot)
{
    snapshot->flags.members = party->state_groups.members;
    snapshot->flags.state_02_03 = party->state_groups.unknown_02_03;
    snapshot->flags.party_00_01 = party->flags.unknown_00_01;
    snapshot->flags.party_02 = party->flags.unknown_02;
    snapshot->flags.party_03 = party->flags.unknown_03;
    snapshot->flags.party_04 = party->flags.unknown_04;
    snapshot->flags.party_05 = party->flags.unknown_05;
    snapshot->flags.party_06 = party->flags.unknown_06;
    snapshot->flags.field_screen = party->flags.field_screen;
    snapshot->flags.special_contact_mode = party->flags.special_contact_mode;
    snapshot->flags.party_10_13 = party->flags.unknown_10_13;
    snapshot->flags.party_14 = party->flags.unknown_14;
    snapshot->flags.follower_rejoin_active = party->flags.follower_rejoin_active;
    snapshot->flags.active_member = party->flags.active_member;
    snapshot->flags.movement_active = party->flags.movement_active;
    snapshot->flags.party_19 = party->flags.unknown_19;
    snapshot->area.movement_state = party->flags.movement_state;
    snapshot->unknown_006 = party->unknown_002;
    snapshot->unknown_007 = party->unknown_003;
    snapshot->input_masks[0] = party->input_masks[0];
    snapshot->input_masks[1] = party->input_masks[1];
    snapshot->unknown_00c = party->unknown_054;
    snapshot->unknown_00d = party->unknown_055;
    snapshot->unknown_010 = party->unknown_058;
    snapshot->unknown_014 = party->unknown_05c;
    snapshot->separation_x = party->separation_x;
    snapshot->separation_y = party->separation_y;
    snapshot->separation_z = party->separation_z;
    snapshot->unknown_024 = party->unknown_06c;
    snapshot->actions[0] = party->actions[0];
    snapshot->actions[1] = party->actions[1];
    snapshot->actions[2] = party->actions[2];
    snapshot->actions[3] = party->actions[3];
    snapshot->state.unknown_00 = party->state_bits.unknown_00;
    snapshot->state.unknown_01 = party->state_bits.unknown_01;
    snapshot->state.unknown_02 = party->state_bits.unknown_02;
    snapshot->state.unknown_03_04 = party->state_bits.unknown_03_04;
    snapshot->state.unknown_05 = party->state_bits.unknown_05;
    snapshot->state.unknown_06_07 = party->state_bits.unknown_11_12;
    snapshot->state.unknown_08 = party->state_bits.unknown_13;
    snapshot->state.unknown_09_12 = party->state_bits.unknown_14_17;
    snapshot->state.movement_mode = party->state_bits.movement_mode;
    if (!party->backup.active) {
        FieldPartyEntity_SaveSnapshot(party->leader, &snapshot->members[0]);
        FieldPartyEntity_SaveSnapshot(party->follower, &snapshot->members[1]);
        snapshot->flags.backup_11 = 0;
    } else {
        if (!party->backup.snapshot_backed) {
            FieldPartyEntity_SaveSnapshot(&party->backup_entities[0], &snapshot->members[0]);
            FieldPartyEntity_SaveSnapshot(&party->backup_entities[1], &snapshot->members[1]);
        } else {
            if (party->backup_snapshots[0] < &snapshot->members[0])
                func_0202cd2c(party->backup_snapshots[0], &snapshot->members[0],
                              sizeof(snapshot->members[0]));
            else
                func_0202cc58(party->backup_snapshots[0], &snapshot->members[0],
                              sizeof(snapshot->members[0]));
            if (party->backup_snapshots[1] < &snapshot->members[1])
                func_0202cd2c(party->backup_snapshots[1], &snapshot->members[1],
                              sizeof(snapshot->members[1]));
            else
                func_0202cc58(party->backup_snapshots[1], &snapshot->members[1],
                              sizeof(snapshot->members[1]));
        }
        snapshot->flags.backup_11 = party->backup.unknown_11;
    }
    snapshot->records[0] = party->records[0];
    snapshot->records[1] = party->records[1];
}
