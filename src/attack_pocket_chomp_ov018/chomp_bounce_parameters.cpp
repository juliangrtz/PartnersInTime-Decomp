/* Pocket Chomp bounce solver, overlay 18, 0x020C40B8-0x020C4174. */
#include "pocket_chomp_internal.h"
extern "C" void PocketChomp_ConfigureBounce(PocketChomp *center, int duration, int height, int index) {
    int initial_height = center->object->z;
    /* The solver works with the requested rise above the current height. */
    height -= initial_height;
    int square = duration * duration;
    int root = FX_Sqrt((height *
        (duration * (duration * height) - (-initial_height) * square)) << 12);
    int velocity = _s32_div_f(((-height * duration) << 12) - root, -(square / 2));
    center->bounce_velocity_q12[index] = velocity;
    /* Widen before squaring the Q12 velocity; the denominator is still Q13. */
    center->bounce_acceleration_q12[index] = (-(s64)velocity * velocity) / (height << 13);
    center->height_q12 = initial_height << 12;
    center->velocity_q12 = 0;
}
