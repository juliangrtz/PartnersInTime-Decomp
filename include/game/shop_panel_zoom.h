#ifndef PIT_GAME_SHOP_PANEL_ZOOM_H
#define PIT_GAME_SHOP_PANEL_ZOOM_H
typedef struct ShopPanelZoomTask ShopPanelZoomTask;
#ifdef __cplusplus
extern "C" {
#endif
void ShopPanelZoom_BeginClose(void);
void ShopPanelZoom_UpdateClose(ShopPanelZoomTask *task);
void ShopPanelZoom_UpdateOpen(ShopPanelZoomTask *task);
#ifdef __cplusplus
}
#endif
#endif
