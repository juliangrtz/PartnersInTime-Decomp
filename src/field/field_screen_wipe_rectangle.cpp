/* Clips the rectangle extents and writes the selected screen window registers.
 */
#include "screen_wipe_internal.h"

extern "C" {
void FieldScreenWipe_SetRectangle(FieldAreaContext *area, s16 left, s16 right,
                                  s16 top, s16 bottom) {
  FieldScreenWipe *effect = Effect(area);
  int x0 = effect->center_x - left;
  int x1 = effect->center_x + right;
  int y0 = effect->center_y - top;
  int y1 = effect->center_y + bottom;
  if (x0 < 0)
    x0 = 0;
  if (x1 > 255)
    x1 = 255;
  if (y0 < 0)
    y0 = 0;
  if (y1 > 192)
    y1 = 192;
  if (!area->flags.screen) {
    *(vu16 *)0x4000042 = ((x0 << 8) & 0xff00) | (x1 & 255);
    *(vu16 *)0x4000046 = ((y0 << 8) & 0xff00) | (y1 & 255);
  } else {
    *(vu16 *)0x4001042 = ((x0 << 8) & 0xff00) | (x1 & 255);
    *(vu16 *)0x4001046 = ((y0 << 8) & 0xff00) | (y1 & 255);
  }
}
}
