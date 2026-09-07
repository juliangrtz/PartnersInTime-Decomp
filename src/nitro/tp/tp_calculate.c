#include <nitro/tp.h>

#define TP_DIVCNT (*(vu16 *)0x04000280)
#define TP_NUMERATOR (*(vu32 *)0x04000290)
#define TP_DENOMINATOR (*(volatile u64 *)0x04000298)
#define TP_QUOTIENT (*(volatile s32 *)0x040002A0)

int TP_CalcCalibrateParam(TpCalibration *output, u16 raw_x1, u16 raw_y1,
    u16 display_x1, u16 display_y1, u16 raw_x2, u16 raw_y2, u16 display_x2, u16 display_y2)
{
    u32 interrupts;
    int x_step;
    int y_step;
    int offset;
    s32 raw_difference_y;
    s32 display_difference_y;
    if (raw_x1 >= 0x1000 || raw_y1 >= 0x1000 || raw_x2 >= 0x1000 || raw_y2 >= 0x1000) return 1;
    if (display_x1 >= 0x100 || display_x2 >= 0x100 || display_y1 >= 0xc0 || display_y2 >= 0xc0) return 1;
    if (display_x1 == display_x2 || display_y1 == display_y2 || raw_x1 == raw_x2 || raw_y1 == raw_y2) return 1;
    interrupts = OS_DisableInterrupts();
    TP_DIVCNT = 0;
    TP_NUMERATOR = (raw_x1 - raw_x2) << 8;
    TP_DENOMINATOR = (u32)(display_x1 - display_x2);
    raw_difference_y = raw_y1 - raw_y2;
    display_difference_y = display_y1 - display_y2;
    while (TP_DIVCNT & 0x8000) {}
    x_step = TP_QUOTIENT;
    TP_DIVCNT = 0;
    TP_NUMERATOR = raw_difference_y << 8;
    TP_DENOMINATOR = (u32)display_difference_y;
    if (x_step >= 0x8000 || x_step < -0x8000) {
        OS_RestoreInterrupts(interrupts);
        return 1;
    }
    output->x_step = x_step;
    offset = (s16)((((raw_x1 + raw_x2) << 8) - output->x_step * (display_x1 + display_x2)) >> 7);
    if (offset >= 0x8000 || offset < -0x8000) {
        OS_RestoreInterrupts(interrupts);
        return 1;
    }
    output->x_offset = offset;
    while (TP_DIVCNT & 0x8000) {}
    y_step = TP_QUOTIENT;
    OS_RestoreInterrupts(interrupts);
    if (y_step >= 0x8000 || y_step < -0x8000) return 1;
    output->y_step = y_step;
    offset = (s16)((((raw_y1 + raw_y2) << 8) - output->y_step * (display_y1 + display_y2)) >> 7);
    if (offset >= 0x8000 || offset < -0x8000) return 1;
    output->y_offset = offset;
    return 0;
}
