#include <game/field_party.h>
static inline void SetAnchor(FieldRenderObject *render, s32 anchor)
{
    render->sort_key = anchor;
    render->render_anchor_z = anchor;
}
static inline void SetOffset(FieldRenderObject *render, s16 x, s16 y)
{
    render->animation_offset_x = x;
    render->animation_offset_y = y;
}
void FieldParty_StartIndicatorSlide(FieldPartyController *party, int upward)
{
    party->indicator_bits.sliding = 1;
    if (!upward)
        party->indicator_velocity = 4;
    else
        party->indicator_velocity = -4;
}
void FieldParty_UpdateIndicatorSlide(FieldPartyController *party)
{
    if (party->indicator_bits.sliding) {
        party->indicator_slide_y += party->indicator_velocity;
        if (party->indicator_velocity > 0) {
            if (party->indicator_slide_y >= 8) {
                party->indicator_slide_y = 0;
                party->indicator_y = 31;
                party->indicator_bits.sliding = 0;
            }
        } else {
            if (party->indicator_slide_y <= -8) {
                party->indicator_slide_y = 0;
                party->indicator_y = 23;
                party->indicator_bits.sliding = 0;
            }
        }
        FieldParty_UpdateIndicatorPositions(party);
    }
}
void FieldParty_UpdateIndicatorPositions(FieldPartyController *party)
{
    s16 x = party->indicator_x + party->indicator_offset_x;
    s16 y = party->indicator_y + party->indicator_slide_y + party->indicator_offset_y;
    if (party->flags.unknown_00_01 == 3) {
        SetOffset(party->unknown_08c, x + 8, y - 8);
        SetOffset(party->unknown_090, x - 8, y + 8);
    } else {
        SetOffset(party->unknown_08c, x, y);
        SetOffset(party->unknown_090, x, y);
    }
    SetOffset(party->unknown_088, x, y);
    if (party->state_bits.unknown_13 && party->unknown_094_render) {
        SetOffset(party->unknown_094_render, x - 10, y + 17);
    }
}
void FieldParty_SetIndicatorLayer(FieldPartyController *party, int alternate)
{
    if (!alternate) {
        SetAnchor(party->unknown_08c, 0x10000010);
        SetAnchor(party->unknown_090, 0x10000010);
        SetAnchor(party->unknown_088, 0x10000011);
        party->unknown_08c->unknown_060 = (u8)party->state_bits.unknown_14_17;
        party->unknown_090->unknown_060 = (u8)party->state_bits.unknown_14_17;
        party->unknown_088->unknown_060 = (u8)party->state_bits.unknown_14_17;
        if (party->state_bits.unknown_13 && party->unknown_094_render)
            party->unknown_094_render->unknown_060 = (u8)party->state_bits.unknown_14_17;
    } else {
        SetAnchor(party->unknown_08c, 0x10000020);
        SetAnchor(party->unknown_090, 0x10000020);
        SetAnchor(party->unknown_088, 0x10000021);
        party->unknown_08c->unknown_060 = (u8)party->state_bits.movement_mode;
        party->unknown_090->unknown_060 = (u8)party->state_bits.movement_mode;
        party->unknown_088->unknown_060 = (u8)party->state_bits.movement_mode;
        if (party->state_bits.unknown_13 && party->unknown_094_render)
            party->unknown_094_render->unknown_060 = (u8)party->state_bits.movement_mode;
    }
}
