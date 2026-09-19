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
