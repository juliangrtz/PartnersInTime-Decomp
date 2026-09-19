#ifndef PIT_TITLE_MENU_INTERNAL_H
#define PIT_TITLE_MENU_INTERNAL_H
#include <game/title_sprite_sequence.h>
#include <game/title_model.h>

typedef struct TitleMenuCursor {
    TitleSpriteHeader header;
    BattleModel *sub_model, *main_model;
    s32 start_x, start_y, target_x, target_y;
} TitleMenuCursor;
typedef char CursorSize[sizeof(TitleMenuCursor) == 64 ? 1 : -1];

enum TitleMenuCursorState {
    TITLE_CURSOR_IDLE, TITLE_CURSOR_MOVING, TITLE_CURSOR_APPEARING
};

typedef struct TitleMenuItem {
    s16 x, y, unknown_04[2];
} TitleMenuItem;

typedef struct TitleMenuBase {
    TitleSpriteHeader header;
    BattleModel *sub_model, *main_model;
    TitleMenuCursor *cursor;
    const TitleMenuItem *items;
    s8 selection, minimum, maximum;
    u8 input_disabled;
} TitleMenuBase;

typedef char TitleMenuItemSize[sizeof(TitleMenuItem) == 8 ? 1 : -1];
typedef char TitleMenuBaseSize[sizeof(TitleMenuBase) == 60 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void TitleMenuCursor_Release(TitleMenuCursor *work);
void TitleMenuCursor_Init(TitleMenuCursor *work);
void TitleMenuCursor_Show(TitleMenuCursor *work, int x, int y, int frames);
void TitleMenuCursor_Move(TitleMenuCursor *work, int x, int y, int frames);
void TitleMenuCursor_Draw(void *element);
void TitleMenuCursor_Update(void *element);
void TitleMenuBase_Init(TitleMenuBase *work, u32 resource, void *cursor);
void TitleMenuBase_MoveSelection(TitleMenuBase *work, int direction, int frames, int next_state);
#ifdef __cplusplus
}
#endif
#endif
