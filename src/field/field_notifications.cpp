/*
 * Item notification queue (overlay 0, 0x0206F768-0x0206FACC).
 * Each area has two text slots. A pending notice may replace the current one
 * during its final 30 updates; an accepted notice starts a 60-update timer.
 */
#include <game/field_area.h>

extern "C" {
int func_ov000_0206fd18(FieldAreaContext *, int, int, int, int, int,
                      int, int, int, int, int, int, int, const u8 *, int, int, int);

void FieldArea_QueueNotification(FieldAreaContext *area, const u8 *text, int quantity, int y)
{
    FieldAreaContext *state = area;
    FieldNotification *notice = state->notifications;
    for (int i = 0; i < 2; ++i, ++notice) {
        if (!notice->flags.active && !notice->flags.pending) {
            notice->flags.pending = 1;
            notice->y = y;
            u8 *output = notice->text;
            output[0] = 0xFF;
            output[1] = 3;
            output[2] = 0xFF;
            output[3] = 0x35;
            output += 4;
            while (text[0] != 0xFF || text[1] != 0xA || text[2] != 0)
                *output++ = *text++;
            if (quantity > 1) {
                *output++ = 0x20;
                *output++ = 0xC;
                *output++ = 0x20;
                if (quantity >= 10)
                    *output++ = quantity / 10 + '0';
                *output++ = quantity % 10 + '0';
            }
            output[0] = 0xFF;
            output[1] = 0xA;
            output[2] = 0;
            return;
        }
    }
}

void FieldArea_CloseNotification(FieldAreaContext *area, int index)
{
    FieldAreaContext *state = area;
    if (index == -1) {
        index = state->notification_index;
        if (index == -1)
            return;
    }
    FieldNotification *notice = &state->notifications[index];
    if (notice->flags.active) {
        FieldArea_CloseMessageWindows(area, area->flags.screen == 0 ? 6 : 7);
        notice->flags.active = 0;
        state->notification_index = -1;
        state->notification_timer = 0;
    }
}

void FieldArea_ClearNotifications(FieldAreaContext *area)
{
    FieldAreaContext *state = area;
    if (state->notification_index >= 0)
        FieldArea_CloseNotification(area, state->notification_index);
    FieldNotification *notice = state->notifications;
    for (int i = 0; i < 2; ++i, ++notice) {
        notice->flags.active = 0;
        notice->flags.pending = 0;
    }
}

void FieldArea_UpdateNotifications(FieldAreaContext *area)
{
    FieldAreaContext *state = area;
    if (state->notification_index >= 0 && --state->notification_timer == 0)
        FieldArea_CloseNotification(area, state->notification_index);
    FieldNotification *notice = state->notifications;
    for (int i = 0; i < 2; ++i, ++notice) {
        if (notice->flags.pending && state->notification_timer <= 30) {
            if (state->notification_index >= 0)
                FieldArea_CloseNotification(area, state->notification_index);
            if (func_ov000_0206fd18(area, 3, 0x8000, notice->y,
                    0, 0, 0, 0, 0, 0, 0, 1, 0, notice->text,
                    area->flags.screen == 0 ? 6 : 7, 0, 0) != -1) {
                notice->flags.active = 1;
                notice->flags.pending = 0;
                state->notification_index = i;
                state->notification_timer = 60;
            }
        }
    }
}

}
