#ifndef PIT_SHOP_EQUIPMENT_DISPLAY_H
#define PIT_SHOP_EQUIPMENT_DISPLAY_H

/* Equipped-item descriptions and sprites attached to a moving shop panel. */
typedef struct ShopEquipmentDisplayTask ShopEquipmentDisplayTask;
#ifdef __cplusplus
extern "C" {
#endif
void ShopEquipment_UpdateDescription(ShopEquipmentDisplayTask *task);
void ShopPanel_DrawAttachedSprite(ShopEquipmentDisplayTask *task);
#ifdef __cplusplus
}
#endif
#endif
