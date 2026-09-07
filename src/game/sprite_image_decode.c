#include <game/sprite_image.h>

/* The decoded image uses a fixed 20-pixel row pitch. Each pair of input
 * words supplies the low and high planes for four rows of eight pixels. */

void GameSpriteImage_Decode8x8(const u32 *source, u8 *destination)
{
    {
        u32 plane0 = source[0];
        u32 plane1 = source[1];
        u32 plane2 = source[2];
        u32 plane3 = source[3];
        s16 remaining = 8;
        do {
            --remaining;
            destination[0] = (plane0 & 1) + 2 * (plane1 & 1);
            destination[20] = ((plane0 & 2) + 2 * (plane1 & 2)) >> 1;
            destination[40] = ((plane0 & 4) + 2 * (plane1 & 4)) >> 2;
            destination[60] = ((plane0 & 8) + 2 * (plane1 & 8)) >> 3;
            destination[80] = (plane2 & 1) + 2 * (plane3 & 1);
            destination[100] = ((plane2 & 2) + 2 * (plane3 & 2)) >> 1;
            destination[120] = ((plane2 & 4) + 2 * (plane3 & 4)) >> 2;
            destination[140] = ((plane2 & 8) + 2 * (plane3 & 8)) >> 3;
            plane0 >>= 4;
            plane1 >>= 4;
            plane2 >>= 4;
            plane3 >>= 4;
            ++destination;
        } while (remaining > 0);
    }
}

void GameSpriteImage_Decode8x12(const u32 *source, u8 *destination)
{
    {
        u32 plane0 = source[0];
        u32 plane1 = source[1];
        u32 plane2 = source[2];
        u32 plane3 = source[3];
        u32 plane4 = source[4];
        u32 plane5 = source[5];
        s16 remaining = 8;
        do {
            --remaining;
            destination[0] = (plane0 & 1) + 2 * (plane1 & 1);
            destination[20] = ((plane0 & 2) + 2 * (plane1 & 2)) >> 1;
            destination[40] = ((plane0 & 4) + 2 * (plane1 & 4)) >> 2;
            destination[60] = ((plane0 & 8) + 2 * (plane1 & 8)) >> 3;
            destination[80] = (plane2 & 1) + 2 * (plane3 & 1);
            destination[100] = ((plane2 & 2) + 2 * (plane3 & 2)) >> 1;
            destination[120] = ((plane2 & 4) + 2 * (plane3 & 4)) >> 2;
            destination[140] = ((plane2 & 8) + 2 * (plane3 & 8)) >> 3;
            destination[160] = (plane4 & 1) + 2 * (plane5 & 1);
            destination[180] = ((plane4 & 2) + 2 * (plane5 & 2)) >> 1;
            destination[200] = ((plane4 & 4) + 2 * (plane5 & 4)) >> 2;
            destination[220] = ((plane4 & 8) + 2 * (plane5 & 8)) >> 3;
            plane0 >>= 4;
            plane1 >>= 4;
            plane2 >>= 4;
            plane3 >>= 4;
            plane4 >>= 4;
            plane5 >>= 4;
            ++destination;
        } while (remaining > 0);
    }
}

void GameSpriteImage_Decode8x16(const u32 *source, u8 *destination)
{
    u32 plane0 = source[0];
    u32 plane1 = source[1];
    u32 plane2 = source[2];
    u32 plane3 = source[3];
    u8 *start = destination;
    s16 remaining = 8;
    do {
        --remaining;
        destination[0] = (plane0 & 1) + 2 * (plane1 & 1);
        destination[20] = ((plane0 & 2) + 2 * (plane1 & 2)) >> 1;
        destination[40] = ((plane0 & 4) + 2 * (plane1 & 4)) >> 2;
        destination[60] = ((plane0 & 8) + 2 * (plane1 & 8)) >> 3;
        destination[80] = (plane2 & 1) + 2 * (plane3 & 1);
        destination[100] = ((plane2 & 2) + 2 * (plane3 & 2)) >> 1;
        destination[120] = ((plane2 & 4) + 2 * (plane3 & 4)) >> 2;
        destination[140] = ((plane2 & 8) + 2 * (plane3 & 8)) >> 3;
        plane0 >>= 4;
        plane1 >>= 4;
        plane2 >>= 4;
        plane3 >>= 4;
        ++destination;
    } while (remaining > 0);
    {
        s16 remaining;
        u32 plane0 = source[4];
        u32 plane1 = source[5];
        u32 plane2 = source[6];
        u32 plane3 = source[7];
        remaining = 8;
        destination = start;
        do {
            --remaining;
            destination[160] = (plane0 & 1) + 2 * (plane1 & 1);
            destination[180] = ((plane0 & 2) + 2 * (plane1 & 2)) >> 1;
            destination[200] = ((plane0 & 4) + 2 * (plane1 & 4)) >> 2;
            destination[220] = ((plane0 & 8) + 2 * (plane1 & 8)) >> 3;
            destination[240] = (plane2 & 1) + 2 * (plane3 & 1);
            destination[260] = ((plane2 & 2) + 2 * (plane3 & 2)) >> 1;
            destination[280] = ((plane2 & 4) + 2 * (plane3 & 4)) >> 2;
            destination[300] = ((plane2 & 8) + 2 * (plane3 & 8)) >> 3;
            plane0 >>= 4;
            plane1 >>= 4;
            plane2 >>= 4;
            plane3 >>= 4;
            ++destination;
        } while (remaining > 0);
    }
}
