/*
 * Party action models (overlay 0, 0x0209CCA0-0x0209CD84).
 *
 * Updates the model shown for the action available in region 71.
 */

#include <game/field_party_manager.h>
typedef struct PartyActionModelView {
    u8 prefix[0x8c];
    FieldRenderObject *model;
} PartyActionModelView;
typedef char PartyActionModelView_SizeCheck[sizeof(PartyActionModelView) == 144 ? 1 : -1];
extern "C" void FieldParty_UpdateRegion71ActionModel(FieldPartyController *party, int member) {
    int mask = 1 << member;
    FieldResourceContext *area;
    FieldRenderObject *model;
    int animation;
    if (!(party->flags.unknown_00_01 & mask))
        return;
    area = party->areas[party->flags.field_screen];
    if (!area || *(u16 *)area->unknown_23f0 != 71)
        return;
    model = ((PartyActionModelView *)((u8 *)party + member * 4))->model;
    if (!model)
        return;
    animation = party->actions[member];
    if (member == party->flags.active_member) {
        int mode = party->state_bits.unknown_11_12;
        if (mode == 1)
            animation = 6;
        else if (mode == 2)
            animation = 7;
    }
    if (party->state_groups.unknown_02_03 & mask)
        animation += 15;
    if (animation != model->resource_animation)
        model->set_animation((u8)animation, 0, 1);
}
