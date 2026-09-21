/* Snapshot background scrolling and hand the field to battle (0x0206B1FC..0x0206B3A0). */

#include "field_scene_transition_internal.h"
#include <game/field_background.h>
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
