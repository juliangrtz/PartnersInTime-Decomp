/* Uploads dirty tiles/maps and streams circular window edges through HBlank
 * DMA. */
#include "screen_wipe_internal.h"

extern "C" {
void FieldScreenWipe_Upload(FieldAreaContext *area) {
  if (!Effect(area))
    return;
  if (Effect(area)->flags.cleanup) {
    FieldScreenWipe_Clear(area);
    Effect(area)->flags.cleanup = 0;
  }
  if (!Effect(area)->flags.active && !Effect(area)->flags.retain)
    return;
  if (Effect(area)->flags.brightness_dirty) {
    FieldArea_StartBrightness(area, 255, 0, 0);
    Effect(area)->flags.brightness_dirty = 0;
  }
  if (Effect(area)->flags.tiles_dirty) {
    if (!area->flags.screen)
      func_02038170(Effect(area)->tiles, 0, 512);
    else
      func_02038108(Effect(area)->tiles, 0, 512);
    Effect(area)->flags.tiles_dirty = 0;
  }
  if (Effect(area)->flags.map_dirty) {
    if (!area->flags.screen)
      func_020384b0(Effect(area)->map, 0, 1536);
    else
      func_02038448(Effect(area)->map, 0, 1536);
    Effect(area)->flags.map_dirty = 0;
  }
  if (Effect(area)->flags.window_dirty) {
    void *tiles, *map;
    if (!area->flags.screen) {
      SetWindows((vu32 *)0x4000000, GetWindows((vu32 *)0x4000000) | 2);
      SetInside((vu16 *)0x4000048, 30, 1);
      SetOutside((vu16 *)0x400004a, 31, 1);
      tiles = func_02036670();
      map = func_02036930();
    } else {
      SetWindows((vu32 *)0x4001000, GetWindows((vu32 *)0x4001000) | 2);
      SetInside((vu16 *)0x4001048, 30, 1);
      SetOutside((vu16 *)0x400104a, 31, 1);
      tiles = func_02036650();
      map = func_02036910();
    }
    func_0202cbd4(tiles, 0x11111111, 32);
    func_0202cbd4(map, 0xe000, 1536);
    Effect(area)->flags.window_dirty = 0;
  }
  if (Effect(area)->flags.kind == 4) {
    if (Effect(area)->flags.active)
      Effect(area)->circle.buffer.selected =
          Effect(area)->circle.buffer.selected ^ 1;
    u16 *scanline =
        Effect(area)->circle.scanlines[Effect(area)->circle.buffer.selected];
    if (!area->flags.screen) {
      MI_StopDma(1);
      *(vu16 *)0x4000042 = *scanline;
      *(vu16 *)0x4000046 = 192;
      func_0203b684(1, scanline + 1, (vu16 *)0x4000042, 2);
    } else {
      MI_StopDma(2);
      *(vu16 *)0x4001042 = *scanline;
      *(vu16 *)0x4001046 = 192;
      func_0203b684(2, scanline + 1, (vu16 *)0x4001042, 2);
    }
  }
}
}
