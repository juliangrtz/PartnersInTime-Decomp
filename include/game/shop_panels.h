#ifndef PIT_GAME_SHOP_PANELS_H
#define PIT_GAME_SHOP_PANELS_H
#include <game/text.h>
#include <game/overlay005_display.h>
/* Common item-panel prefix. The buying and selling panels extend this view. */
typedef struct ShopItemPanel {
    void *vtable;
    u8 enabled, visible_rows, use_inventory, unknown_07, count, unknown_09;
    u16 inline_items[396];
    u16 *items;
    s8 first, ring_first;
    union {
        s8 signed_value;
        u8 value;
    } selected;
    u8 unknown_32b;
    s32 x, y, unknown_334, unknown_338;
} ShopItemPanel;
typedef char ShopItemPanelSizeCheck[sizeof(ShopItemPanel) == 828 ? 1 : -1];
/* Selling panel: category offsets refer to the packed inline inventory list. */
typedef struct ShopInventoryPanel {
    ShopItemPanel base;
    s32 unknown_33c;
    s32 category_first[4];
} ShopInventoryPanel;
typedef char ShopInventoryPanelSizeCheck[sizeof(ShopInventoryPanel) == 848 ? 1 : -1];

typedef struct ShopStockPanel {
    u8 unknown_00, category, unknown_02, count;
    u16 items[100];
    s8 *quantities;
    s8 first, ring_first;
    u8 unknown_0d2[6];
    s32 x, y;
} ShopStockPanel;
typedef char ShopStockPanelSizeCheck[sizeof(ShopStockPanel) == 224 ? 1 : -1];

typedef struct ShopText {
    GameText text;
    u8 pixels[2048];
} ShopText;
typedef char ShopTextSizeCheck[sizeof(ShopText) == 2096 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void ShopInventoryPanel_Hide(ShopInventoryPanel *panel, int adjust);
void ShopInventoryPanel_Show(ShopInventoryPanel *panel);
void ShopInventoryPanel_CycleCategory(ShopInventoryPanel *panel, s8 delta);
int ShopInventoryPanel_GetCategory(ShopInventoryPanel *panel, int item);
void ShopInventoryPanel_ShowEmptyMessage(ShopInventoryPanel *panel);
int ShopInventoryPanel_CanSellRow(ShopInventoryPanel *panel, u16 row);
s8 ShopInventoryPanel_GetRowQuantity(ShopInventoryPanel *panel, u16 row, u32 mode);
u32 ShopInventoryPanel_GetPriceBonus(ShopInventoryPanel *panel, u16 row);
void ShopInventoryPanel_RebuildList(ShopInventoryPanel *panel);
u32 ShopItemPanel_GetSelectedTile(ShopItemPanel *panel);
int ShopItemPanel_GetRingFirst(ShopItemPanel *panel);
int ShopItemPanel_CanScroll(ShopItemPanel *panel);
void ShopItemPanel_GetVector334(ShopItemPanel *panel, s32 *x, s32 *y);
void ShopItemPanel_SetVector334(ShopItemPanel *panel, s32 x, s32 y);
s32 ShopItemPanel_GetY(ShopItemPanel *panel);
s32 ShopItemPanel_GetX(ShopItemPanel *panel);
void ShopItemPanel_SetPosition(ShopItemPanel *panel, s32 x, s32 y);
u8 ShopItemPanel_GetCount(ShopItemPanel *panel);
u8 ShopItemPanel_GetVisibleRows(ShopItemPanel *panel);
u16 ShopItemPanel_GetRowItem(ShopItemPanel *panel, u16 row);
u8 ShopItemPanel_GetSelection(ShopItemPanel *panel);
int ShopItemPanel_IsSelectionAtEdge(ShopItemPanel *panel, int last);
int ShopItemPanel_IsWindowAtEdge(ShopItemPanel *panel, int last);
int ShopStockPanel_GetRingFirst(ShopStockPanel *panel);
s32 ShopStockPanel_GetY(ShopStockPanel *panel);
s32 ShopStockPanel_GetX(ShopStockPanel *panel);
void ShopStockPanel_SetPosition(ShopStockPanel *panel, s32 x, s32 y);
u16 ShopStockPanel_GetRowItem(ShopStockPanel *panel, int row);
u16 ShopStockPanel_GetRowQuantity(ShopStockPanel *panel, int row);
u8 ShopStockPanel_GetCount(ShopStockPanel *panel);
u8 ShopStockPanel_GetCategory(ShopStockPanel *panel);
void ShopStockPanel_Destroy(ShopStockPanel *panel);
ShopStockPanel *ShopStockPanel_Init(ShopStockPanel *panel);
void ShopText_CopyIcon(ShopText *renderer, DisplayEngine engine, int offset, int icon);
u16 ShopText_MeasureLine(ShopText *renderer, int kind, u16 item, u8 quantity);
const u8 *ShopText_GetString(ShopText *renderer, int kind, u16 item, u8 quantity);
#ifdef __cplusplus
}
#endif
#endif
