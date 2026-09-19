/*
 * Shell travel timing (overlay 11, 0x020C2E88-0x020C2F38).
 *
 * How long the shell takes to reach its target, and the reset that puts the
 * support character back to its starting state.
 */

#include "shell_internal.h"

extern "C" {
void Overlay11Support_Reset(Overlay11Support *support)
{
    support->active = 0;
    support->state = 1;
    if (support->effect_handle) {
        BattleSound_Stop(support->effect_handle - 1);
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
