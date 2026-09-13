#ifndef PIT_GAME_SHOP_TEXT_TASKS_H
#define PIT_GAME_SHOP_TEXT_TASKS_H
#include <nitro.h>

/* Views of the shared 72-byte overlay-5 task pool. Payload arguments stay
 * full width until the rendering callback narrows them. */
typedef struct ShopItemTextTask {
    u8 unknown_00[40];
    int row, item, color, quantity;
    u8 unknown_38[16];
} ShopItemTextTask;
typedef struct ShopStockTextTask {
    u8 unknown_00[40];
    int row, item, quantity;
    u8 unknown_34[20];
} ShopStockTextTask;
typedef char ShopItemTextTaskSizeCheck[sizeof(ShopItemTextTask) == 72 ? 1 : -1];
typedef char ShopStockTextTaskSizeCheck[sizeof(ShopStockTextTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
ShopItemTextTask *ShopItemText_Queue(int row, int item, int color, int quantity);
void ShopItemText_Update(ShopItemTextTask *task);
ShopStockTextTask *ShopStockText_Queue(int row, int item, int quantity);
void ShopStockText_Update(ShopStockTextTask *task);
#ifdef __cplusplus
}
#endif
#endif
