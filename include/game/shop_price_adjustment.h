#ifndef PIT_GAME_SHOP_PRICE_ADJUSTMENT_H
#define PIT_GAME_SHOP_PRICE_ADJUSTMENT_H

/*
 * The display that shows how a price was adjusted.
 */
typedef struct ShopPriceAdjustmentTask ShopPriceAdjustmentTask;
#ifdef __cplusplus
extern "C" {
#endif
void ShopPriceAdjustment_SetMode(int mode);
void ShopPriceAdjustment_DrawValueMarker(ShopPriceAdjustmentTask *task);
void ShopPriceAdjustment_DrawValue(ShopPriceAdjustmentTask *task);
void ShopPriceAdjustment_DrawSymbol(ShopPriceAdjustmentTask *task);
#ifdef __cplusplus
}
#endif
#endif
