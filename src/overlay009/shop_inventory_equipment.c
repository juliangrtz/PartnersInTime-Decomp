#include "shop_list_internal.h"

void ShopInventory_Add(u16 item, s8 delta)
{
    s8 *inventory = ShopInventory_GetArray(item);
    if (inventory) {
        u16 limit = func_ov009_0207ddac(item), index = item & ITEM_ID_INDEX_MASK;
        inventory[index] += delta;
        if (inventory[index] > limit)
            inventory[index] = limit;
        if (inventory[index] < 0)
            inventory[index] = 0;
    }
}

int ShopInventory_CanEquip(u16 item, u32 member)
{
    if ((item & ITEM_ID_TAG_MASK) == ITEM_ID_BADGE_TAG)
        return 1;
    return member >> 1 == ((ShopEquipmentRecord *)(data_020505c4 + 28 * (item & ITEM_ID_INDEX_MASK)))->flags << 7 >> 31;
}
