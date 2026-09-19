/*
 * Field screen fades (overlay 0, 0x02075968-0x02075D28).
 *
 * The brightness fade and the alpha blend the field runs, each started, stepped
 * and reversible.
 */

#include <game/field_system.h>
extern void GXi_SetMasterBrightness(vu16 *, int);
void FieldArea_StartBrightness(FieldAreaContext *area, s16 initial, s16 target, u16 frames) {
    if (!frames) {
        if (!area->flags.screen)
            GXi_SetMasterBrightness((vu16 *)0x400006c, target);
        else
            GXi_SetMasterBrightness((vu16 *)0x400106c, target);
        area->brightness = target << 8;
        return;
    }
    area->unknown_2402.unknown_00 = 1;
    if (initial != 255)
        area->brightness = initial << 8;
    area->unknown_2408 = area->brightness;
    area->unknown_2402.unknown_02_07 = target;
    area->unknown_2404 = frames;
    area->unknown_2406 = frames;
    area->unknown_240c = ((area->unknown_2402.unknown_02_07 << 8) - area->brightness) / frames;
}

void FieldArea_ReverseBrightness(FieldAreaContext *area) {
    if (area->unknown_2402.unknown_00) {
        s8 target;
        area->unknown_2402.unknown_01 = 0;
        area->unknown_240c = -area->unknown_240c;
        area->unknown_2406 = area->unknown_2404 - area->unknown_2406;
        target = area->unknown_2402.unknown_02_07;
        area->unknown_2402.unknown_02_07 = area->unknown_2408;
        area->unknown_2408 = target;
    }
}

void FieldArea_UpdateBrightness(FieldAreaContext *area) {
    if (area->unknown_2402.unknown_00 && !area->unknown_2402.unknown_01) {
        if (--area->unknown_2406 == 0) {
            area->unknown_2402.unknown_00 = 0;
            area->brightness = area->unknown_2402.unknown_02_07 << 8;
            if (!area->brightness) {
                if (area->flags.screen == 1) {
                    area->party->bits.unknown_15 = 0;
                    area->party->bits.unknown_16 = 0;
                }
                area->state &= ~4;
            }
        } else
            area->brightness += area->unknown_240c;
        if (!area->flags.screen)
            GXi_SetMasterBrightness((vu16 *)0x400006c, area->brightness / 256);
        else
            GXi_SetMasterBrightness((vu16 *)0x400106c, area->brightness / 256);
    }
}

void FieldArea_UpdateAlphaBlend(FieldAreaContext *area) {
    if (area->unknown_2410.unknown_00 && !area->unknown_2410.unknown_01) {
        if (--area->unknown_2416 == 0) {
            area->unknown_2410.unknown_00 = 0;
            area->unknown_2418[2] = area->unknown_2410.unknown_14_18 << 8;
            area->unknown_2418[3] = area->unknown_2410.unknown_19_23 << 8;
        } else {
            area->unknown_2418[2] += area->unknown_2418[4];
            area->unknown_2418[3] += area->unknown_2418[5];
        }
        if (!area->flags.screen)
            *(vu16 *)0x4000052 =
                (area->unknown_2418[2] / 256) | ((area->unknown_2418[3] / 256) << 8);
        else
            *(vu16 *)0x4001052 =
                (area->unknown_2418[2] / 256) | ((area->unknown_2418[3] / 256) << 8);
    }
}
