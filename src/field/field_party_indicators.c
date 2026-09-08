#include <game/field_party.h>
#include <game/field_area.h>
extern const s16 data_ov000_020c0f5c[];
extern void func_ov000_02079d74(FieldAreaContext *, int);
extern void func_ov000_0209d024(FieldPartyController *, int, int, int, int, int);
extern void func_020093b4(FieldRenderObject *, int);
static inline FieldAreaContext *GetArea(FieldPartyController *party, int side)
{
    return (FieldAreaContext *)party->areas[side];
}
static inline void RefreshIndicators(FieldPartyController *party)
{
    FieldAreaContext *other;
    func_ov000_02079d74(GetArea(party, party->flags.field_screen), 1 << party->flags.special_contact_mode);
    other = GetArea(party, party->flags.field_screen ^ 1);
    if (other && other->unknown_23f0 >= 22)
        func_ov000_02079d74(other, 1 << party->flags.special_contact_mode);
}
void FieldParty_StartIndicatorSwap(FieldPartyController *party)
{
    party->indicator_bits.swap_active = 1;
    party->indicator_frame = 0;
}
void FieldParty_UpdateIndicatorSwap(FieldPartyController *party)
{
    if (party->indicator_bits.swap_active) {
        if (party->flags.unknown_03) {
            int direction = party->flags.special_contact_mode ? -1 : 1;
            party->indicator_offset_x = direction * data_ov000_020c0f5c[3 * party->indicator_frame];
            party->indicator_offset_y = direction * data_ov000_020c0f5c[3 * party->indicator_frame + 1];
            FieldParty_UpdateIndicatorPositions(party);
            if (data_ov000_020c0f5c[3 * party->indicator_frame + 2])
                FieldParty_SetIndicatorLayer(party, 0);
        } else if (data_ov000_020c0f5c[3 * party->indicator_frame + 2]) {
            FieldParty_SetIndicatorLayer(party, 1);
        }
        ++party->indicator_frame;
        if (party->indicator_frame == 11)
            party->indicator_bits.swap_active = 0;
    }
}
void FieldParty_SetIndicatorVisibility(FieldPartyController *party, int visible, int instant, int transfer)
{
    if (visible == party->indicator_bits.visible) {
        if (party->indicator_bits.visibility_transition) {
            party->indicator_bits.visible ^= 1;
            party->indicator_velocity = -party->indicator_velocity;
        }
    } else if (!party->indicator_bits.visibility_transition) {
        if (party->flags.movement_state == GetArea(party, party->flags.field_screen)->room_id || transfer) {
            if (instant) {
                if (!visible) {
                    if (!party->indicator_bits.slide_from_bottom)
                        party->indicator_slide_y = -52;
                    else
                        party->indicator_slide_y = 52;
                } else
                    party->indicator_slide_y = 0;
                party->indicator_bits.visible = visible;
                func_020093b4(party->unknown_08c, visible);
                func_020093b4(party->unknown_090, visible);
                func_020093b4(party->unknown_088, visible);
                FieldParty_UpdateIndicatorPositions(party);
                if (transfer) {
                    func_ov000_02079d74(GetArea(party, party->flags.field_screen), 3);
                    func_ov000_02079d74(GetArea(party, party->flags.field_screen ^ 1), 3);
                    FieldParty_SetIndicatorVisibility(party, 1, 1, 0);
                }
            } else {
                party->indicator_bits.visibility_transition = 1;
                party->indicator_bits.unknown_07 = transfer;
                party->indicator_bits.unknown_08 = 0;
                if (!visible) {
                    if (!party->indicator_bits.slide_from_bottom)
                        party->indicator_velocity = -4;
                    else
                        party->indicator_velocity = 4;
                } else {
                    if (!party->indicator_bits.slide_from_bottom)
                        party->indicator_velocity = 4;
                    else
                        party->indicator_velocity = -4;
                    func_020093b4(party->unknown_08c, 1);
                    func_020093b4(party->unknown_090, 1);
                    func_020093b4(party->unknown_088, 1);
                }
            }
        } else
            party->indicator_bits.visible = visible;
    } else {
        if (transfer && !party->indicator_bits.unknown_07)
            party->indicator_bits.unknown_08 = 1;
        else if (!transfer && party->indicator_bits.unknown_07)
            party->indicator_bits.unknown_07 = 0;
    }
}
void FieldParty_UpdateIndicatorVisibility(FieldPartyController *party)
{
    FieldAreaContext *area;
    if (!party->flags.unknown_00_01)
        return;
    area = GetArea(party, party->flags.field_screen);
    if (!area || area->unknown_23f0 != 71)
        return;
    if (!party->indicator_bits.visibility_transition)
        return;
    party->indicator_slide_y += party->indicator_velocity;
    if (party->indicator_bits.visible) {
        if (party->indicator_velocity > 0) {
            if (party->indicator_slide_y >= 52) {
                party->indicator_slide_y = 52;
                party->indicator_bits.visibility_transition = 0;
            }
        } else if (party->indicator_slide_y <= -52) {
            party->indicator_slide_y = -52;
            party->indicator_bits.visibility_transition = 0;
        }
        if (!party->indicator_bits.visibility_transition) {
            party->indicator_bits.visible = 0;
            RefreshIndicators(party);
            if (party->indicator_bits.unknown_07) {
                if (!party->flags.field_screen) {
                    FieldPartyController *paired;
                    FieldParty_SetIndicatorVisibility(party, 1, 0, 1);
                    paired = party->paired;
                    if (!paired->indicator_bits.visibility_transition) {
                        func_ov000_0209d024(paired, paired->indicator_bits.visible != 0, 1, 1,
                                            paired->state_bits.unknown_14_17,
                                            paired->state_bits.movement_mode);
                        FieldParty_UpdateIndicatorPositions(party->paired);
                    }
                } else
                    GetArea(party, 1)->flags.unknown_13 = 1;
            }
        }
    } else {
        if (party->indicator_velocity < 0) {
            if (party->indicator_slide_y <= 0)
                party->indicator_bits.visibility_transition = 0;
        } else if (party->indicator_slide_y >= 0)
            party->indicator_bits.visibility_transition = 0;
        if (!party->indicator_bits.visibility_transition) {
            party->indicator_slide_y = 0;
            party->indicator_bits.visible = 1;
            RefreshIndicators(party);
        }
    }
    FieldParty_UpdateIndicatorPositions(party);
}

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
