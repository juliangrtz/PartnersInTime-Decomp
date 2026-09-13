#ifndef PIT_GAME_SHOP_EQUIPMENT_HIGHLIGHT_H
#define PIT_GAME_SHOP_EQUIPMENT_HIGHLIGHT_H

typedef struct ShopEquipmentHighlightTask ShopEquipmentHighlightTask;
#ifdef __cplusplus
extern "C" {
#endif
void ShopEquipmentHighlight_Stop(void);
void ShopEquipmentHighlight_Start(void);
void ShopEquipmentHighlight_Update(ShopEquipmentHighlightTask *task);
#ifdef __cplusplus
}
#endif
#endif
