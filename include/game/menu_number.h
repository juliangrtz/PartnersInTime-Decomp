#ifndef PIT_GAME_MENU_NUMBER_H
#define PIT_GAME_MENU_NUMBER_H
#include <nitro.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Build a 32-by-16-pixel OBJ strip from transparent 4bpp glyphs. */
void MenuNumber_DrawUnavailable(int tile, u32 x);
void MenuNumber_DrawValue(int tile, u32 x, u16 value, int wide);
void MenuNumber_PrepareGlyphs(void);
#ifdef __cplusplus
}
#endif
#endif
