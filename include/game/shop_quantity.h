#ifndef PIT_GAME_SHOP_QUANTITY_H
#define PIT_GAME_SHOP_QUANTITY_H
#include <nitro.h>
typedef struct ShopQuantityTask ShopQuantityTask;
#ifdef __cplusplus
extern "C" {
#endif
void ShopQuantity_DrawTotal(ShopQuantityTask *task);
void ShopQuantity_DrawDigit(ShopQuantityTask *task);
void ShopQuantity_CopySelectedIcon(void);
void ShopQuantity_DrawItemName(u16 item);
void ShopQuantity_DrawPositionedSprite(ShopQuantityTask *task);
void ShopQuantity_DrawSprite(ShopQuantityTask *task);
void ShopQuantity_DrawSlidingSprite(ShopQuantityTask *task);
#ifdef __cplusplus
}
#endif
#endif
