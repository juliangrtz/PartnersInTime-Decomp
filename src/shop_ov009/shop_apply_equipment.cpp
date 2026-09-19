/*
 * Equipment purchase (overlay 9, 0x0207662C-0x02076668).
 *
 * Applies the equipment the player selected to the chosen party member.
 */

#include "shop_rows_internal.h"
extern "C" {
#include <game/equipment.h>
extern ShopItemPanel *data_ov009_0207ea20;
void ShopEquipment_ApplySelection(u8 member) {
    Equipment_Apply(member,
                    ShopItemPanel_GetRowItem(data_ov009_0207ea20,
                                             ShopItemPanel_GetSelection(data_ov009_0207ea20)));
}
}
