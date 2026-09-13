#ifndef PIT_GAME_PAUSE_LIST_ROW_H
#define PIT_GAME_PAUSE_LIST_ROW_H

#include <nitro.h>

/* The parent task supplies a list row's text tiles, quantity and position. */
typedef struct PauseListRowTask {
    u8 unknown_00[40];
    u16 item, row, quantity, palette_offset, draw_list;
    u8 unknown_32[2];
    int tile;
    fx32 x, y, origin_x, origin_y;
} PauseListRowTask;

typedef struct PauseListSpriteTask {
    u8 unknown_00[16];
    PauseListRowTask *parent;
    u8 unknown_14[20];
    union { int segment; int divisor; } selection;
    u8 unknown_2c[28];
} PauseListSpriteTask;

typedef char PauseListRowTaskSizeCheck[sizeof(PauseListRowTask) == 72 ? 1 : -1];
typedef char PauseListSpriteTaskSizeCheck[sizeof(PauseListSpriteTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void PauseListRow_UpdateDigitSprite(PauseListSpriteTask *task);
void PauseListRow_UpdateMarkerSprite(PauseListSpriteTask *task);
void PauseListRow_UpdateTextSprite(PauseListSpriteTask *task);
#ifdef __cplusplus
}
#endif

#endif
