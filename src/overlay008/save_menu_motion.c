#include "save_menu_internal.h"

void SaveMenuMotion_Initialize(SaveMenuMotion *motion, int x, int y)
{
    int acceleration_y;
    u8 *menu;
    int dy = y - motion->y;

    motion->acceleration_x = 2 * (x - motion->x - motion->velocity_x * motion->remaining)
        / (motion->remaining * motion->remaining);
    acceleration_y = 2 * (dy - motion->velocity_y * motion->remaining)
        / (motion->remaining * motion->remaining);
    menu = data_ov008_02078290;
    motion->acceleration_y = acceleration_y;
    menu[412] = 1;
}

int SaveMenuMotion_Update(SaveMenuMotion *motion)
{
    motion->velocity_x += motion->acceleration_x;
    motion->velocity_y += motion->acceleration_y;
    motion->x += motion->velocity_x;
    motion->y += motion->velocity_y;
    if (--motion->remaining == 1) {
        data_ov008_02078290[412] = 0;
        return 1;
    }
    return 0;
}
