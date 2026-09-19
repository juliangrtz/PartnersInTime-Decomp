#include "shop_scene_internal.h"
#include <game/shop_equipment_highlight.h>

struct ShopEquipmentHighlightTask {
    u8 unknown_00[32];
    s32 phase, unknown_24;
    s32 alpha, target, step;
    u8 unknown_34[20];
};
typedef char ShopEquipmentHighlightTaskSizeCheck[sizeof(ShopEquipmentHighlightTask) == 72 ? 1 : -1];

extern "C" {
/* Native alias of the 2 KiB tilemap at ShopSceneWork + 0xB6. */
extern u16 data_ov009_0207eaf2[1024];
void func_02036988(vu32 *, int, int, int, int);
void func_ov005_0206650c(void *);
}

#define WORK data_ov009_0207ea3c
#define HIGHLIGHT_ENABLED WORK.equipment_highlight_active
#define HIGHLIGHT_MAP data_ov009_0207eaf2
#define ITEM_PANEL ((ShopItemPanel *)data_ov009_0207ea20)

extern "C" void ShopEquipmentHighlight_Update(ShopEquipmentHighlightTask *task)
{
    if (task->phase < 100 && !HIGHLIGHT_ENABLED) task->phase = 100;
    switch (task->phase) {
    case 0:
        task->step = (task->target - task->alpha) / 6;
        ++task->phase;
        /* Fall through to the first fade-in update. */
    case 1:
        task->alpha += task->step;
        if (task->alpha >= 0x8000) {
            task->alpha = 0x8000;
            ++task->phase;
        }
        func_02036988((vu32 *)0x04001050, 1, 31,
            task->alpha / 4096, 16 - task->alpha / 4096);
        break;
    case 100:
        task->step = -task->alpha / 6;
        ++task->phase;
        /* Fall through to the first fade-out update. */
    case 101:
        task->alpha += task->step;
        if (task->alpha <= 0) {
            task->alpha = 0;
            Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG0);
            REG16(0x04001050) = 0;
            func_ov005_0206650c(task);
            return;
        }
        func_02036988((vu32 *)0x04001050, 1, 31,
            task->alpha / 4096, 16 - task->alpha / 4096);
        break;
    }
    if (ShopInventory_CanEquip(ShopItemPanel_GetRowItem(ITEM_PANEL,
            ShopItemPanel_GetSelection(ITEM_PANEL)), 0)) {
        if (WORK.available[2]) {
            Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_SUB, 0, 0);
            u16 *map = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_SUB, 0);
            u16 tile = 25;
            for (int row = 0; row < 3; ++row) {
                int column = 0;
                do {
                    map[32 * (row + 1) + column] = tile;
                    ++column;
                    ++tile;
                } while (column < 2);
            }
            tile = 19;
            for (int row = 0; row < 3; ++row) {
                int column = 0;
                do {
                    map[32 * (row + 1) + column + 15] = tile;
                    ++column;
                    ++tile;
                } while (column < 2);
            }
            Overlay5Display_EnablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG0);
        } else {
            Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG0);
        }
    } else if (WORK.available[0]) {
        Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_SUB, 0, -88);
        u16 *map = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_SUB, 0);
        u16 tile = 13;
        for (int row = 0; row < 3; ++row) {
            int column = 0;
            do {
                map[32 * (row + 1) + column] = tile;
                ++column;
                ++tile;
            } while (column < 2);
        }
        tile = 7;
        for (int row = 0; row < 3; ++row) {
            int column = 0;
            do {
                map[32 * (row + 1) + column + 15] = tile;
                ++column;
                ++tile;
            } while (column < 2);
        }
        Overlay5Display_EnablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG0);
    } else {
        Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG0);
    }
}

extern "C" void ShopEquipmentHighlight_Start(void)
{
    Overlay5DisplayBg_LoadScreen(DISPLAY_ENGINE_SUB, 0, HIGHLIGHT_MAP, 0, 2048);
    if (WORK.party_count == 1) {
        u16 *map = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_SUB, 0);
        for (int row = 0; row < 32; ++row)
            for (int column = 0; column < 16; ++column)
                map[32 * row + column] = 0;
    }
    HIGHLIGHT_ENABLED = 1;
    func_02036988((vu32 *)0x04001050, 1, 31, 0, 16);
    ShopEquipmentHighlightTask *task = (ShopEquipmentHighlightTask *)func_ov005_0206659c(
        (void (*)(ShopMenuElement *))ShopEquipmentHighlight_Update, 11, 1);
    task->alpha = 0;
    task->target = 0x8000;
}

extern "C" void ShopEquipmentHighlight_Stop(void)
{
    HIGHLIGHT_ENABLED = 0;
}
