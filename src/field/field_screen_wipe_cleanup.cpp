/* Finishes or clears a wipe, releasing its buffers and restoring screen
 * brightness. */
#include "screen_wipe_internal.h"

extern "C" {
void FieldScreenWipe_Finish(FieldAreaContext *area) {
  if (Effect(area)->flags.active || Effect(area)->flags.retain) {
    if ((Effect(area)->flags.active && !Effect(area)->flags.retain) ||
        (!Effect(area)->flags.active && Effect(area)->flags.retain))
      Effect(area)->flags.cleanup = 1;
    Effect(area)->flags.active = 0;
  }
}

void FieldScreenWipe_Clear(FieldAreaContext *area) {
  int darken = 0;
  switch (Effect(area)->flags.kind) {
  case 0:
  case 1:
  case 2:
  case 3: {
    GameHeap_DeleteArray(Effect(area)->tiles);
    Effect(area)->tiles = 0;
    GameHeap_DeleteArray(Effect(area)->map);
    Effect(area)->map = 0;
    void *tiles, *map;
    if (!area->flags.screen) {
      tiles = func_02036670();
      map = func_02036930();
    } else {
      tiles = func_02036650();
      map = func_02036910();
    }
    func_0202cbd4(tiles, 0, 512);
    func_0202cbd4(map, 0, 1536);
    int kind = Effect(area)->flags.kind;
    if (kind == 1 || kind == 3)
      darken = 1;
    break;
  }
  case 4:
    if (!area->flags.screen)
      MI_StopDma(1);
    else
      MI_StopDma(2);
    GameHeap_DeleteArray(Effect(area)->circle.scanlines[0]);
    Effect(area)->circle.scanlines[0] = 0;
    Effect(area)->circle.scanlines[1] = 0;
    /* Fall through to disable the hardware window. */
  case 5: {
    void *tiles, *map;
    if (!area->flags.screen) {
      SetWindows((vu32 *)0x4000000, GetWindows((vu32 *)0x4000000) & ~2);
      tiles = func_02036670();
      map = func_02036930();
    } else {
      SetWindows((vu32 *)0x4001000, GetWindows((vu32 *)0x4001000) & ~2);
      tiles = func_02036650();
      map = func_02036910();
    }
    func_0202cbd4(tiles, 0, 32);
    func_0202cbd4(map, 0, 1536);
    if (Effect(area)->flags.kind == 4) {
      if (!Effect(area)->circle.end_radius)
        darken = 1;
    } else if (!Effect(area)->rectangle.bounds[4] &&
               !Effect(area)->rectangle.bounds[5] &&
               !Effect(area)->rectangle.bounds[6] &&
               !Effect(area)->rectangle.bounds[7])
      darken = 1;
    break;
  }
  }
  func_0202cbd4(Effect(area), 0, 24);
  if (!area->flags.screen)
    *(u32 *)((u8 *)area->owner + 84) &= ~0x2000000;
  else
    *(u32 *)((u8 *)area->owner + 84) &= ~0x4000000;
  if (darken)
    FieldArea_StartBrightness(area, 255, -16, 0);
}
}
