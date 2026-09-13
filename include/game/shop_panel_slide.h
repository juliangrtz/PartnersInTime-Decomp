#ifndef PIT_GAME_SHOP_PANEL_SLIDE_H
#define PIT_GAME_SHOP_PANEL_SLIDE_H

typedef struct ShopPanelSlideTask ShopPanelSlideTask;
#ifdef __cplusplus
extern "C" {
#endif
void ShopPanelSlide_Begin(int entering);
void ShopPanelSlide_Update(ShopPanelSlideTask *task);
#ifdef __cplusplus
}
#endif
#endif
