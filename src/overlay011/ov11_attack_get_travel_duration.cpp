#include "shell_internal.h"

extern "C" {
void Overlay11Support_Reset(Overlay11Support *support)
{
    support->active = 0;
    support->state = 1;
    if (support->effect_handle) {
        func_ov002_02076178(support->effect_handle - 1);
        support->effect_handle = 0;
    }
}

int Overlay11Attack_GetTravelDuration(int start, int speed_q8, int target, int step)
{
    int frames = 0;
    if (speed_q8 > 0) {
        if (start < target) {
            int displacement = 0;
            do {
                displacement += speed_q8 * step;
                frames += step;
            } while (start + (displacement >> 8) < target);
        }
    } else if (start > target) {
        int displacement = 0;
        do {
            displacement += speed_q8 * step;
            frames += step;
        } while (start + (displacement >> 8) > target);
    }
    return frames;
}
}
