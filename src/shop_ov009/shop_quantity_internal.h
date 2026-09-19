#ifndef PIT_SHOP_QUANTITY_INTERNAL_H
#define PIT_SHOP_QUANTITY_INTERNAL_H
#include <game/shop_quantity.h>
extern "C" {
#include <game/overlay005_display.h>
}
#include "shop_rows_internal.h"
extern "C" {
#include "shop_text_tasks_internal.h"
#include <game/save_data.h>
int func_ov009_0207ddac(u16 item);
extern u8 data_ov009_0207ea3c[];
extern ShopItemPanel *data_ov009_0207ea20;
void MIi_CpuCopy16(const void *, void *, u32);

}

struct ShopQuantityTask {
    u8 unknown_00[40];
    union { int part; int y; };
    union { int previous_total; int first_tile; int selling; };
    int previous_quantity;
    u32 item;
    u8 unknown_38[16];
};
typedef char ShopQuantityTaskSize[sizeof(ShopQuantityTask) == 72 ? 1 : -1];
#define VISIBLE (*(s8 *)(data_ov009_0207ea3c + 0xa2))
#define SCALE (*(s32 *)(data_ov009_0207ea3c + 0xa4))
#define PRICE (*(s32 *)(data_ov009_0207ea3c + 0xa8))
#define QUANTITY (*(s16 *)(data_ov009_0207ea3c + 0xac))
#define SPECIAL data_ov009_0207ea3c[0x99]


#endif
