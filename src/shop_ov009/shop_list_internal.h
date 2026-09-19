#ifndef PIT_SHOP_LIST_INTERNAL_H
#define PIT_SHOP_LIST_INTERNAL_H
#include <game/shop_list.h>
#include <game/save_data.h>
#include <game/item.h>
extern u8 data_ov009_0207ea3c[];
typedef struct ShopEquipmentRecord {
    u8 unknown[12];
    u32 flags;
    u8 unknown_10[12];
} ShopEquipmentRecord;
typedef char ShopEquipmentRecordSizeCheck[sizeof(ShopEquipmentRecord) == 28 ? 1 : -1];
extern u8 data_020505c4[];
extern void func_ov009_0207d660(ShopList *, ShopListLink *, int);
extern u8 func_ov009_0207dbc0(u16), func_ov009_0207dc58(u16);
extern int func_ov009_0207ddac(u16);
#endif
