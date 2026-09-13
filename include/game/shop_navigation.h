#ifndef PIT_GAME_SHOP_NAVIGATION_H
#define PIT_GAME_SHOP_NAVIGATION_H
#include <game/shop_rows.h>
#ifdef __cplusplus
extern "C" {
#endif
void ShopPanelNavigation_Clear(void);
void ShopPanelNavigation_Create(void);
void ShopPanelNavigation_DrawArrow(ShopRowTask *task);
void ShopPanelNavigation_DrawCursor(ShopRowTask *task);
void ShopListNavigation_Clear(void);
void ShopListNavigation_Create(void);
void ShopListNavigation_DrawArrow(ShopRowTask *task);
void ShopListNavigation_DrawCursor(ShopRowTask *task);
void ShopMenuNavigation_Clear(void);
void ShopMenuNavigation_Create(void);
void ShopMenuNavigation_DrawCursor(ShopRowTask *task);
void ShopMenuOptions_DrawModel(ShopRowTask *task);
void ShopMenuOptions_DrawText(ShopRowTask *task);
#ifdef __cplusplus
}
#endif
#endif
