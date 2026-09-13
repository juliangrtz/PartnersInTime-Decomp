#ifndef PIT_GAME_SHOP_STOCK_ROWS_H
#define PIT_GAME_SHOP_STOCK_ROWS_H
#include <nitro.h>

/* Shared 72-byte task slot: a stock row, or one of its child sprite segments. */
typedef struct ShopStockRowTask {
    u8 unknown_00[16];
    struct ShopStockRowTask *parent;
    u8 unknown_14[20];
    union {
        int part;
        struct {
            u16 row, quantity, palette, priority;
            s32 tile, x, y, offset_x, offset_y;
            u8 unknown_44[4];
        };
    };
} ShopStockRowTask;
typedef char StockRowTaskSizeCheck[sizeof(ShopStockRowTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void ShopStockRow_Update(ShopStockRowTask *task);
void ShopStockRow_DrawTextSegment(ShopStockRowTask *task);
void ShopStockRow_DrawIcon(ShopStockRowTask *task);
void ShopStockRow_DrawQuantityDigit(ShopStockRowTask *task);
void ShopStockCaption_DrawSegment(ShopStockRowTask *task);
#ifdef __cplusplus
}
#endif
#endif
