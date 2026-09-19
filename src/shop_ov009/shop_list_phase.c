/*
 * Shop list phase (overlay 9, 0x0207D218-0x0207D220).
 *
 * Reports which phase the list animation is in.
 */

#include "shop_list_internal.h"

u16 ShopList_GetPhase(ShopList *list)
{
    return list->phase;
}
