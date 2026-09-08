#include <game/field_area.h>
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);
FieldAreaContext *FieldArea_CopyState(FieldAreaContext *field, const FieldAreaContext *source)
{
    int i;
    field->unknown_23f0 = source->unknown_23f0;
    field->room_id = source->room_id;
    field->flags.screen = source->flags.screen;
    field->flags.party_active = source->flags.party_active;
    field->flags.mode = source->flags.mode;
    field->flags.unknown_04_05 = source->flags.unknown_04_05;
    field->flags.unknown_06 = source->flags.unknown_06;
    field->flags.unknown_07_12 = source->flags.unknown_07_12;
    field->flags.unknown_13 = source->flags.unknown_13;
    field->flags.unknown_14 = source->flags.unknown_14;
    field->flags.unknown_15 = source->flags.unknown_15;
    field->area_value = source->area_value;
    field->unknown_23fb = source->unknown_23fb;
    field->unknown_23fc = source->unknown_23fc;
    field->unknown_23fe = source->unknown_23fe;
    field->unknown_2400 = source->unknown_2400;
    field->unknown_2402.unknown_00 = source->unknown_2402.unknown_00;
    field->unknown_2402.unknown_01 = source->unknown_2402.unknown_01;
    field->unknown_2402.unknown_02_07 = source->unknown_2402.unknown_02_07;
    field->unknown_2404 = source->unknown_2404;
    field->unknown_2406 = source->unknown_2406;
    field->unknown_2408 = source->unknown_2408;
    field->brightness = source->brightness;
    field->unknown_240c = source->unknown_240c;
    field->unknown_2410.unknown_00 = source->unknown_2410.unknown_00;
    field->unknown_2410.unknown_01 = source->unknown_2410.unknown_01;
    field->unknown_2410.unknown_02_07 = source->unknown_2410.unknown_02_07;
    field->unknown_2410.unknown_08_13 = source->unknown_2410.unknown_08_13;
    field->unknown_2410.unknown_14_18 = source->unknown_2410.unknown_14_18;
    field->unknown_2410.unknown_19_23 = source->unknown_2410.unknown_19_23;
    field->unknown_2414 = source->unknown_2414;
    field->unknown_2416 = source->unknown_2416;
    field->unknown_2418[0] = source->unknown_2418[0];
    field->unknown_2418[1] = source->unknown_2418[1];
    field->unknown_2418[2] = source->unknown_2418[2];
    field->unknown_2418[3] = source->unknown_2418[3];
    field->unknown_2418[4] = source->unknown_2418[4];
    field->unknown_2418[5] = source->unknown_2418[5];
    field->unknown_2424[0] = source->unknown_2424[0];
    field->unknown_2424[1] = source->unknown_2424[1];
    field->unknown_2424[2] = source->unknown_2424[2];
    field->unknown_2424[3] = source->unknown_2424[3];
    field->camera_x = source->camera_x;
    field->camera_y = source->camera_y;
    field->unknown_243c[0] = source->unknown_243c[0];
    field->unknown_243c[1] = source->unknown_243c[1];
    field->unknown_243c[2] = source->unknown_243c[2];
    field->unknown_243c[3] = source->unknown_243c[3];
    field->unknown_244c[0] = source->unknown_244c[0];
    field->unknown_244c[1] = source->unknown_244c[1];
    field->unknown_244c[2] = source->unknown_244c[2];
    field->unknown_244c[3] = source->unknown_244c[3];
    field->unknown_2454[0] = source->unknown_2454[0];
    field->unknown_2454[1] = source->unknown_2454[1];
    field->unknown_2454[2] = source->unknown_2454[2];
    field->unknown_2454[3] = source->unknown_2454[3];
    field->unknown_2464[0] = source->unknown_2464[0];
    field->unknown_2464[1] = source->unknown_2464[1];
    field->unknown_2468[0] = source->unknown_2468[0];
    field->unknown_2468[1] = source->unknown_2468[1];
    field->unknown_2470[0] = source->unknown_2470[0];
    field->unknown_2470[1] = source->unknown_2470[1];
    field->unknown_2470[2] = source->unknown_2470[2];
    field->unknown_2470[3] = source->unknown_2470[3];
    field->unknown_2470[4] = source->unknown_2470[4];
    field->unknown_2470[5] = source->unknown_2470[5];
    field->unknown_2488.unknown_00 = source->unknown_2488.unknown_00;
    field->unknown_2488.unknown_01 = source->unknown_2488.unknown_01;
    field->unknown_248a = source->unknown_248a;
    field->unknown_248c[0] = source->unknown_248c[0];
    field->unknown_248c[1] = source->unknown_248c[1];
    field->unknown_248c[2] = source->unknown_248c[2];
    field->unknown_248c[3] = source->unknown_248c[3];
    field->unknown_249c.unknown_00 = source->unknown_249c.unknown_00;
    field->unknown_249c.unknown_01 = source->unknown_249c.unknown_01;
    field->unknown_249c.unknown_02_09 = source->unknown_249c.unknown_02_09;
    field->unknown_249c.unknown_10_19 = source->unknown_249c.unknown_10_19;
    field->unknown_249c.unknown_20_29 = source->unknown_249c.unknown_20_29;
    field->unknown_24a0[0] = source->unknown_24a0[0];
    field->unknown_24a0[1] = source->unknown_24a0[1];
    field->unknown_24a0[2] = source->unknown_24a0[2];
    field->unknown_24a0[3] = source->unknown_24a0[3];
    field->unknown_24a0[4] = source->unknown_24a0[4];
    field->unknown_24a0[5] = source->unknown_24a0[5];
    field->unknown_24a0[6] = source->unknown_24a0[6];
    field->unknown_24a0[7] = source->unknown_24a0[7];
    field->unknown_24c0 = source->unknown_24c0;
    for (i = 0; i < 2; ++i)
        field->input_masks[i] = source->input_masks[i];
    field->hud.visible = source->hud.visible;
    field->hud.moving = source->hud.moving;
    field->unknown_24da = source->unknown_24da;
    field->unknown_24dc = source->unknown_24dc;
    field->unknown_24f0 = source->unknown_24f0;
    FieldScriptManager_Copy(&field->scripts, &source->scripts);
    field->state_bits.unknown_00_01 = source->state_bits.unknown_00_01;
    for (i = 0; i < 8; ++i)
        field->effect_owners[i] = source->effect_owners[i];
    if (source->unknown_2504 < field->unknown_2504)
        func_0202cd2c(source->unknown_2504, field->unknown_2504, sizeof(field->unknown_2504));
    else
        func_0202cc58(source->unknown_2504, field->unknown_2504, sizeof(field->unknown_2504));
    if (source->unknown_2538 < field->unknown_2538)
        func_0202cd2c(source->unknown_2538, field->unknown_2538, sizeof(field->unknown_2538));
    else
        func_0202cc58(source->unknown_2538, field->unknown_2538, sizeof(field->unknown_2538));
    if (source->unknown_2b5c < field->unknown_2b5c)
        func_0202cd2c(source->unknown_2b5c, field->unknown_2b5c, sizeof(field->unknown_2b5c));
    else
        func_0202cc58(source->unknown_2b5c, field->unknown_2b5c, sizeof(field->unknown_2b5c));
    if (source->unknown_2b90 < field->unknown_2b90)
        func_0202cd2c(source->unknown_2b90, field->unknown_2b90, sizeof(field->unknown_2b90));
    else
        func_0202cc58(source->unknown_2b90, field->unknown_2b90, sizeof(field->unknown_2b90));
    return field;
}

extern void func_020090b8(int);
extern void func_ov000_020751d8(FieldAreaContext *);
extern void func_ov000_02073c54(FieldAreaContext *);
void FieldArea_UpdateGraphics(FieldAreaContext *field)
{
    if (field->unknown_23f0 == 71 || field->flags.unknown_06) {
        func_020090b8((u8)field->flags.screen);
        GameSpritePalette_UploadScreen((u8)field->flags.screen);
        field->flags.unknown_06 = 0;
    }
    if (field->unknown_23f0 == 71) {
        func_ov000_020751d8(field);
        func_ov000_02073c54(field);
    }
}
