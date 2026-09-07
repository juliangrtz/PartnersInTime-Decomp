#include <nitro.h>

/* The affine coefficients are reduced from Q12 to the hardware's Q8 format. */
void G2x_SetBGyAffine_(vu32 *registers, const s32 *matrix,
                     int center_x, int center_y, int x, int y) {
    s16 pa = matrix[0] >> 4;
    s16 pb = matrix[1] >> 4;
    s16 pc, pd;
    int dx, dy, origin_x, origin_y;
    registers[0] = (u16)pa | ((u16)pb << 16);
    pc = matrix[2] >> 4;
    pd = matrix[3] >> 4;
    registers[1] = (u16)pc | ((u16)pd << 16);
    dx = x - center_x;
    dy = y - center_y;
    origin_x = matrix[0] * dx + matrix[1] * dy + (center_x << 12);
    origin_y = matrix[2] * dx + matrix[3] * dy + (center_y << 12);
    registers[2] = origin_x >> 4;
    registers[3] = origin_y >> 4;
}

void func_02036988(vu32 *registers, int planes_a, int planes_b,
                   int coefficient_a, int coefficient_b) {
    *registers = planes_a | 0x40 | (planes_b << 8) |
                 ((coefficient_a | (coefficient_b << 8)) << 16);
}

void func_02036964(vu16 *registers, int planes, int brightness) {
    if (brightness < 0) {
        registers[0] = planes | 0xC0;
        registers[2] = -brightness;
    } else {
        registers[0] = planes | 0x80;
        registers[2] = brightness;
    }
}
