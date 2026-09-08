#include "shop_list_internal.h"

void ShopList_UpdateVelocities(ShopList *list, ShopListPoint *point, int count)
{
    int i;
    for (i = 0; i < count; ++i, ++point) {
        if (point->delay)
            --point->delay;
        if (!point->pinned && !point->delay) {
            point->velocity_x = (230 * (point->x - point->previous_x)) >> 8;
            point->velocity_y = (230 * (point->y - point->previous_y)) >> 8;
            point->previous_x = point->x;
            point->previous_y = point->y;
        }
    }
}

void ShopList_RestorePinnedPoints(ShopList *list, ShopListPoint *point, int count)
{
    int i;
    for (i = 0; i < count; ++i, ++point) {
        if (point->pinned || point->delay) {
            point->velocity_x = 0;
            point->velocity_y = 0;
            point->x = point->previous_x;
            point->y = point->previous_y;
        }
    }
}

void ShopList_Simulate(ShopList *list, int iterations)
{
    int i;
    ShopList_AdvancePoints(list, list->points, list->point_count);
    for (i = 0; i < iterations; ++i) {
        func_ov009_0207d660(list, list->links, list->point_count - 1);
        ShopList_RestorePinnedPoints(list, list->points, list->point_count);
    }
    ShopList_UpdateVelocities(list, list->points, list->point_count);
}
