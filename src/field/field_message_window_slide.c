#include <game/field_area.h>
#include <game/window.h>

void FieldArea_UpdateMessageWindowSlide(FieldAreaContext *area)
{
    if (area->window_slide.flags.active) {
        area->window_slide.previous_x = area->window_slide.x;
        area->window_slide.previous_y = area->window_slide.y;
        area->window_slide.x += area->window_slide.velocity_x;
        area->window_slide.y += area->window_slide.velocity_y;
        GameWindow_SetOrigin((GameWindowManager *)area->owner, (u8)area->flags.screen,
                             -area->window_slide.x / 4096, -area->window_slide.y / 4096);
        FieldArea_UpdateMessageWindowClip(area, area->window_slide.flags.window);
        ++area->window_slide.flags.elapsed;
    }
}
