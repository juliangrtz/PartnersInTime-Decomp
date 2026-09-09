#include <game/field_area.h>
#include <game/window.h>

extern void func_ov000_020be654(fx32, fx32, fx32, fx32 *, fx32 *);

static inline void SetWindowPlanes(vu16 *reg, int planes, int effects)
{
    /* WININ/WINOUT: five layer bits followed by the color-effect enable bit. */
    u32 value = (*reg & ~0x3f) | planes;
    if (effects)
        value |= 0x20;
    *reg = value;
}

void FieldArea_SetMessageWindowClipEnabled(FieldAreaContext *area, int window, int enabled)
{
    /* DISPCNT bits 13..15 enable window 0, window 1 and the OBJ window. */
    u32 mask;
    if (!area->flags.screen)
        mask = (*(vu32 *)0x4000000 & 0xe000) >> 13;
    else
        mask = (*(vu32 *)0x4001000 & 0xe000) >> 13;
    if (enabled) {
        mask |= 1;
        FieldArea_UpdateMessageWindowClip(area, window);
    } else
        mask &= ~1;
    if (!area->flags.screen)
        *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0xe000) | (mask << 13);
    else
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0xe000) | (mask << 13);
}
void FieldArea_UpdateMessageWindowClip(FieldAreaContext *area, int window)
{
    /* Window indices are signed halfwords in the original array access. */
    GameWindowProperties *properties = &((GameWindowManager *)area->owner)->windows[(s16)window].properties;
    int left = (s16)(properties->position.bits.x + area->window_slide.x / 4096);
    int right = (s16)(left + 8 * (properties->position.bits.tile_pitch + 1));
    int top = (s16)(properties->position.bits.y + area->window_slide.y / 4096);
    int bottom = (s16)(top + 8 * (properties->position.bits.tile_height + 1));
    if (left < 0)
        left = 0;
    if (left > 255)
        left = 255;
    if (right < 0)
        right = 0;
    if (right > 255)
        right = 255;
    if (top < 0)
        top = 0;
    if (top > 192)
        top = 192;
    if (bottom < 0)
        bottom = 0;
    if (bottom > 192)
        bottom = 192;
    if (!area->flags.screen) {
        /* Main WININ/WINOUT and WIN0H/WIN0V; BG0 is hidden outside the window. */
        SetWindowPlanes((vu16 *)0x4000048, 0x1f, 1);
        SetWindowPlanes((vu16 *)0x400004a, 0x1e, 1);
        *(vu16 *)0x4000040 = ((left << 8) & 0xff00) | (right & 0xff);
        *(vu16 *)0x4000044 = ((top << 8) & 0xff00) | (bottom & 0xff);
    } else {
        /* The sub engine exposes the same window registers at +0x1000. */
        SetWindowPlanes((vu16 *)0x4001048, 0x1f, 1);
        SetWindowPlanes((vu16 *)0x400104a, 0x1e, 1);
        *(vu16 *)0x4001040 = ((left << 8) & 0xff00) | (right & 0xff);
        *(vu16 *)0x4001044 = ((top << 8) & 0xff00) | (bottom & 0xff);
    }
}
void FieldArea_StartMessageWindowSlideBySpeed(FieldAreaContext *area, int window, fx32 x, fx32 y, fx32 speed)
{
    area->window_slide.flags.active = 1;
    area->window_slide.flags.fixed_duration = 0;
    area->window_slide.flags.window = window;
    area->window_slide.flags.elapsed = 0;
    area->window_slide.target_x = x;
    area->window_slide.target_y = y;
    func_ov000_020be654(x, y, speed, &area->window_slide.velocity_x, &area->window_slide.velocity_y);
    FieldArea_SetMessageWindowClipEnabled(area, window, 1);
}
