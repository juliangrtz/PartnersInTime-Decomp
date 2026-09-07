#include <game/field_timer.h>

extern void func_0202cbd4(void *destination, int value, u32 size);
extern void func_02038578(const void *source, u32 offset, u32 size);
extern void func_02038518(const void *source, u32 offset, u32 size);

FieldTimer *FieldTimer_Init(FieldTimer *timer)
{
    func_0202cbd4(timer, 0, sizeof(*timer));
    return timer;
}

void FieldTimer_Destroy(FieldTimer *timer) {}

void FieldTimer_SetImage(FieldTimer *timer, const FieldTimerImage *image)
{
    timer->image = image;
}

void FieldTimer_SetValue(FieldTimer *timer, s8 minutes, s8 seconds, s8 frames)
{
    timer->minutes = minutes;
    timer->seconds = seconds;
    timer->frames = frames;
    timer->hundredths = frames * 100 / 60;
    timer->step = 0;
    timer->flags.paused = 0;
}

void FieldTimer_SetVisible(FieldTimer *timer, u8 enabled)
{
    if (enabled) {
        if (!timer->flags.allocated) {
            GameSpriteAllocation_Allocate(&timer->allocation, timer->flags.screen,
                1, ((timer->image->packed_tile_size >> 2) + 127) >> 7,
                1, timer->image->resource, 1, 0);
        }
        if (!timer->flags.screen) {
            func_02038578(timer->image->tiles, timer->allocation.offset,
                timer->image->packed_tile_size >> 2);
        } else {
            func_02038518(timer->image->tiles, timer->allocation.offset,
                timer->image->packed_tile_size >> 2);
        }
    } else {
        GameSpriteAllocation_Unlink(&timer->allocation);
    }
    timer->flags.allocated = enabled;
}

void FieldTimer_SetPosition(FieldTimer *timer, int screen, s16 x, s16 y)
{
    timer->flags.screen = (u8)screen;
    timer->x = x;
    timer->y = y;
}

void FieldTimer_Update(FieldTimer *timer)
{
    if (timer->flags.paused) return;
    if (!timer->step) return;
    timer->frames += timer->step;
    if (timer->step > 0) {
        if (timer->frames >= 60) {
            timer->frames -= 60;
            ++timer->seconds;
            if (timer->seconds == 60) {
                timer->seconds = 0;
                ++timer->minutes;
                if (timer->minutes == 100) {
                    /* Preserve the original rollover store before saturation. */
                    timer->minutes = 0;
                    timer->minutes = 99;
                    timer->seconds = 59;
                    timer->frames = 59;
                    timer->hundredths = 99;
                    timer->step = 0;
                    return;
                }
            }
        }
    } else {
        if (timer->frames < 0) {
            timer->frames += 60;
            --timer->seconds;
            if (timer->seconds < 0) {
                timer->seconds = 59;
                --timer->minutes;
                if (timer->minutes < 0) {
                    timer->frames = timer->seconds = timer->minutes =
                        timer->hundredths = timer->step = 0;
                }
            }
        }
        if (!timer->frames && !timer->seconds && !timer->minutes) {
            timer->hundredths = 0;
            timer->step = 0;
        }
    }
    timer->hundredths = timer->frames * 100 / 60;
}
