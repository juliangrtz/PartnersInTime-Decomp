#include <game/save_menu.h>
extern u8 data_ov008_02078290[];

int LoadMenuMotion_Update(SaveMenuMotion *motion)
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
