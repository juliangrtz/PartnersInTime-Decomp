#include <game/pause_clock.h>

extern "C" {
void func_ov007_02081334(int, int, int, int, int, int, int);
void func_ov007_020814e4(int, int, int, int);
}

extern "C" void PauseClock_UpdateSeparator(PauseClockSeparatorTask *task)
{
    if (++task->timer >= 30) {
        task->timer -= 30;
        task->visible ^= 1;
        if (task->visible)
            func_ov007_02081334(72, 161, 268, 1, 2, 48, 1);
        else
            func_ov007_020814e4(74, 165, 4, 8);
    }
}
