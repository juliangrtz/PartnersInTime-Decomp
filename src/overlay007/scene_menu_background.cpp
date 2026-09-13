#include "scene_window_internal.h"
#include <game/scene_menu_motion.h>
extern "C" {
#include <game/overlay005_display.h>
}

extern "C" void SceneMenu_PrepareBlendBackground(int reset_offset, u16 character_base, u16 tile_bias)
{
    if (reset_offset) {
        Overlay5DisplayBg_SetOffset(DISPLAY_ENGINE_MAIN, 0, 0, 0);
        MANAGER->display.bits.fixed_main_scroll = 1;
    }
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_MAIN, 0);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_MAIN, 0, 0);
    Overlay5DisplayBg_SetColorMode(DISPLAY_ENGINE_MAIN, 0, 1);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 0, 7);
    Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 0, character_base);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_MAIN, 0, 0);
    Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_MAIN, 0, 1);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_MAIN, 0, 2);
    u16 *screen = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_MAIN, 0);
    for (int cell = 0; cell < 768; ++cell) {
        *screen = (*screen & 0x7ff) + tile_bias;
        ++screen;
    }
}
