/*
 * Pause transition projection (overlay 7, 0x0206E1C8-0x0206E27C).
 *
 * Projects a point through the transition's current camera, which the panel and
 * window tasks position themselves with.
 */

#include <game/pause_transition.h>

extern "C" {
extern s16 FX_SinCosTable_[];

void PauseTransition_Project(int x, int y, int angle, int *out_x, int *out_y,
                       int *out_width, int *out_height)
{
    int index = 2 * ((angle & 0xffff) >> 4);
    int cosine = FX_SinCosTable_[index + 1];
    *out_y = y * cosine;
    *out_height = 256 - (cosine >> 4);
    int scale = ((((y << 8) * (FX_SinCosTable_[index] >> 4)) >> 8) + 98304) / 384;
    int width = 65536 / scale;
    if (width > 384) width = 384;
    /* The native output order includes a width reload before calculating X. */
    *out_width = width;
    *out_x = (x * *out_width) << 4;
}

}
