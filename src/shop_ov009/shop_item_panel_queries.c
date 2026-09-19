#include "shop_panels_internal.h"

int ShopItemPanel_IsWindowAtEdge(ShopItemPanel *panel, int last)
{
    u32 visible = panel->visible_rows, count = panel->count;
    if (count <= visible)
        return 1;
    if (!last) {
        if (panel->first == 0)
            return 1;
    } else if (count - 1 == (int)(panel->first + visible - 1) % panel->count)
        return 1;
    return 0;
}

int ShopItemPanel_IsSelectionAtEdge(ShopItemPanel *panel, int last)
{
    if (!last) {
        if (panel->first + panel->selected.signed_value == 0)
            return 1;
    } else if (panel->count - 1 == panel->first + panel->selected.signed_value)
        return 1;
    return 0;
}

u8 ShopItemPanel_GetSelection(ShopItemPanel *panel)
{
    return panel->selected.value;
}

u16 ShopItemPanel_GetRowItem(ShopItemPanel *panel, u16 row)
{
    return panel->items[(panel->first + row) % panel->count];
}

u8 ShopItemPanel_GetVisibleRows(ShopItemPanel *panel)
{
    return panel->visible_rows;
}

u8 ShopItemPanel_GetCount(ShopItemPanel *panel)
{
    return panel->count;
}

void ShopItemPanel_SetPosition(ShopItemPanel *panel, s32 x, s32 y)
{
    panel->x = x;
    panel->y = y;
}

s32 ShopItemPanel_GetX(ShopItemPanel *panel)
{
    return panel->x;
}

s32 ShopItemPanel_GetY(ShopItemPanel *panel)
{
    return panel->y + *(s32 *)(data_ov009_0207ea3c + 0x8c8);
}

void ShopItemPanel_SetVector334(ShopItemPanel *panel, s32 x, s32 y)
{
    panel->unknown_334 = x;
    panel->unknown_338 = y;
}

void ShopItemPanel_GetVector334(ShopItemPanel *panel, s32 *x, s32 *y)
{
    *x = panel->unknown_334;
    *y = panel->unknown_338;
}

int ShopItemPanel_CanScroll(ShopItemPanel *panel)
{
    return panel->count > panel->visible_rows;
}

int ShopItemPanel_GetRingFirst(ShopItemPanel *panel)
{
    return panel->ring_first;
}

u32 ShopItemPanel_GetSelectedTile(ShopItemPanel *panel)
{
    return 36 * ((panel->ring_first + panel->selected.signed_value) % panel->visible_rows) + 76;
}
