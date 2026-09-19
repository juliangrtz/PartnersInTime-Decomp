/*
 * Shop list setup (overlay 9, 0x0207DBA4-0x0207DBC0).
 *
 * Initializes the shop's point list.
 */

#include "shop_list_internal.h"

ShopList *ShopList_Init(ShopList *list)
{
    list->visible = 0;
    list->moving = 0;
    list->entering = 0;
    list->phase = 0;
    return list;
}

void ShopList_Destroy(ShopList *list) {}
