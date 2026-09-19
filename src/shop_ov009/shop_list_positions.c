#include "shop_list_internal.h"

void ShopList_BeginExit(ShopList *list)
{
    list->visible = 0;
    list->moving = 0;
    list->entering = 0;
    list->phase = 1000;
}

s32 ShopList_GetPointX(ShopList *list, u8 index)
{
    return list->anchor_x + (list->points[index].x - list->anchor_x) / 5;
}

s32 ShopList_GetPointY(ShopList *list, u8 index)
{
    return *(s32 *)(data_ov009_0207ea3c + 0x8c8) + list->points[index].y;
}

int ShopList_GetPointPriority(ShopList *list, u8 index)
{
    return list->priority + 3 * (list->point_count - 1 - index);
}

s32 ShopList_GetRowX(ShopList *list, u8 row)
{
    return ShopList_GetPointX(list, (u8)(row + 4));
}

s32 ShopList_GetRowY(ShopList *list, u8 row)
{
    return ShopList_GetPointY(list, (u8)(row + 4));
}

int ShopList_GetRowPriority(ShopList *list, u8 row)
{
    return ShopList_GetPointPriority(list, (u8)(row + 4)) - 1;
}

void ShopList_SetScale(ShopList *list, s32 scale)
{
    list->scale = scale;
    if (list->scale > 4096)
        list->scale = 4096;
    if (list->scale < 81)
        list->scale = 81;
}

s32 ShopList_GetScale(ShopList *list)
{
    return list->scale;
}

void ShopList_AdvancePoints(ShopList *list, ShopListPoint *point, int count)
{
    int i;
    for (i = 0; i < count; ++i, ++point) {
        point->x += point->velocity_x;
        point->y += point->velocity_y;
        if (!point->pinned && !point->delay)
            point->y += (1638 * point->gravity_scale) >> 8;
    }
}
