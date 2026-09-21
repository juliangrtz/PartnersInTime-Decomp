/*
 * Message window slide completion (overlay 0, 0x0206F110-0x0206F288).
 * Clamp finished axes, or commit the target when the elapsed count equals the
 * fixed duration. The return value reports whether the slide remains active.
 */
#include <game/field_area.h>
#include <game/window.h>

extern "C" {
int FieldArea_UpdateMessageWindowSlideCompletion(FieldAreaContext *area)
{
    if (area->window_slide.flags.active) {
        if (!area->window_slide.flags.fixed_duration) {
            if ((area->window_slide.previous_x < area->window_slide.target_x &&
                 area->window_slide.x < area->window_slide.target_x) ||
                (area->window_slide.previous_x > area->window_slide.target_x &&
                 area->window_slide.x > area->window_slide.target_x))
                return 1;
            area->window_slide.x = area->window_slide.target_x;
            area->window_slide.velocity_x = 0;
            if ((area->window_slide.previous_y < area->window_slide.target_y &&
                 area->window_slide.y < area->window_slide.target_y) ||
                (area->window_slide.previous_y > area->window_slide.target_y &&
                 area->window_slide.y > area->window_slide.target_y))
                return 1;
            area->window_slide.y = area->window_slide.target_y;
            area->window_slide.velocity_y = 0;
            area->window_slide.flags.active = 0;
        } else if (area->window_slide.flags.duration == area->window_slide.flags.elapsed) {
            area->window_slide.x = area->window_slide.target_x;
            area->window_slide.y = area->window_slide.target_y;
            GameWindow_SetOrigin((GameWindowManager *)area->owner, (u8)area->flags.screen,
                                 -area->window_slide.x / 4096, -area->window_slide.y / 4096);
            area->window_slide.flags.active = 0;
        }
    }
    return area->window_slide.flags.active != 0;
}

}
