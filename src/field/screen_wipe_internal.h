#ifndef PIT_FIELD_SCREEN_WIPE_INTERNAL_H
#define PIT_FIELD_SCREEN_WIPE_INTERNAL_H

/*
 * The field screen wipe has a 24-byte record. Its tail is a tile buffer pair,
 * a double-buffered circle scanline table, or eight rectangle extents.
 * Graphics registers are read and written at their native access widths.
 */
#include <game/field_area.h>
extern "C" {
#include <game/heap.h>
void MI_StopDma(u32);
void func_0202cbd4(void *, int, unsigned);
void *func_02036650(void), *func_02036670(void);
void *func_02036910(void), *func_02036930(void);
void func_02038108(const void *, unsigned, unsigned);
void func_02038170(const void *, unsigned, unsigned);
void func_02038448(const void *, unsigned, unsigned);
void func_020384b0(const void *, unsigned, unsigned);
void func_0203b684(u32, const void *, volatile void *, u32);
}

typedef struct FieldScreenWipe FieldScreenWipe;
struct FieldScreenWipe {
  struct {
    u32 active : 1, paused : 1, kind : 5, tiles_dirty : 1, map_dirty : 1,
        window_dirty : 1;
    u32 brightness_dirty : 1, cleanup : 1, retain : 1, elapsed : 8,
        unknown : 11;
  } flags;
  union {
    const u8 *pattern;
    struct {
      u16 duration;
      u8 center_x, center_y;
    };
  };
  union {
    struct {
      u8 *tiles;
      u16 *map;
      u32 unknown_tiles[2];
    };
    struct {
      struct {
        u32 selected : 1, unknown : 31;
      } buffer;
      s16 start_radius, end_radius;
      u16 *scanlines[2];
    } circle;
    struct {
      s16 bounds[8];
    } rectangle;
  };
};
typedef char WindowEffectSize[sizeof(FieldScreenWipe) == 24 ? 1 : -1];
static inline FieldScreenWipe *Effect(FieldAreaContext *area) {
  return (FieldScreenWipe *)area->unknown_2bc8;
}
static inline u32 GetWindows(volatile u32 *reg) {
  return (*reg & 0xe000) >> 13;
}
static inline void SetWindows(volatile u32 *reg, u32 windows) {
  *reg = (*reg & ~0xe000) | (windows << 13);
}
static inline void SetInside(volatile u16 *reg, u32 planes, u32 effect) {
  u32 value = (*reg & ~0x3f00) | (planes << 8);
  if (effect)
    value |= 0x2000;
  *reg = value;
}
static inline void SetOutside(volatile u16 *reg, u32 planes, u32 effect) {
  u32 value = (*reg & ~0x3f) | planes;
  if (effect)
    value |= 0x20;
  *reg = value;
}

extern "C" {
void FieldScreenWipe_Upload(FieldAreaContext *area);
void FieldScreenWipe_SetRectangle(FieldAreaContext *area, s16 left, s16 right,
                                  s16 top, s16 bottom);
void FieldScreenWipe_Clear(FieldAreaContext *area);
void FieldScreenWipe_Finish(FieldAreaContext *area);
}
#endif
