#ifndef PIT_GAME_WINDOW_H
#define PIT_GAME_WINDOW_H
#include <game/text.h>
typedef struct GameWindowSkin {
    u16 border_x, border_y;
    u8 reserved04[20];
    const s16 *horizontal;
    const s16 *vertical;
} GameWindowSkin;
typedef struct GameWindowProperties {
    union { u32 raw; struct { u32 screen:1, skin:4, reserved5:1, width:4, height:5, position_mode:3, sound:14; } bits; } shape;
    union { u32 raw; struct { u32 mode:4, reserved4:6, width:8, reserved18:14; } bits; } layout;
    union { u32 raw; struct { u32 x:9, y:9, reserved18:1, tile_pitch:6, tile_height:6, reserved31:1; } bits; } position;
} GameWindowProperties;

typedef struct GameWindowLink {
    struct GameWindowLink *previous, *next;
    union { u32 raw; struct { u32 reserved0:26, linked:1, reserved27:5; } bits; } state;
    u16 id;
    u8 reserved0e[10];
} GameWindowLink;

typedef struct GameWindow {
    struct GameWindow *previous, *next;
    GameWindowProperties properties;
    u8 reserved14[16];
    GameText text;
    u8 reserved54[8];
    const u8 *cursor;
    u8 reserved60[5];
    u8 origin_y;
    u8 reserved66[2];
    union { u32 raw; struct { u32 dirty:1, id:5, reserved6:5, tile_offset:15, group:5, closing:1; } bits; } allocation;
    union { u32 raw; struct { u32 reserved0:1, size:16, animation:3, reserved20:2, layout_mode:5, reserved27:5; } bits; } state;
    u32 reserved70;
    u8 *front, *back;
    GameWindowLink links[2];
    u8 reservedac[22];
    u16 measured_height;
    u8 reservedc4[8];
} GameWindow;
typedef struct GameWindowManager {
    u8 reserved00[52];
    u16 *main_tilemap, *sub_tilemap;
    u8 reserved3c[16];
    u16 main_map_size, sub_map_size;
    union { u32 raw; struct { u32 reserved0:1, main_map_dirty:1, sub_map_dirty:1, dirty:1, main_count:5, sub_count:5, main_bg:2, sub_bg:2, main_priority:2, sub_priority:2, main_bg_palette:4, sub_bg_palette:4, buffer_mode:2; } bits; } state;
    union { u32 raw; struct { u32 main_palette:4, sub_palette:4, reserved8:17, fixed_main_scroll:1, fixed_sub_scroll:1, reserved27:5; } bits; } display;
    const u32 *tiles;
    u8 reserved5c[3748];
    GameWindow *windows;
    u8 reservedf04[12];
    GameWindow *main_windows;
    u8 reservedf14[12];
    GameWindow *sub_windows;
    u8 reservedf24[8];
    s16 main_scroll_x, main_scroll_y, sub_scroll_x, sub_scroll_y;
    s16 main_origin_x, main_origin_y, sub_origin_x, sub_origin_y;
} GameWindowManager;
extern const GameWindowSkin *data_0205671c[];
typedef char GameWindow_SizeCheck[sizeof(GameWindow) == 204 ? 1 : -1];
void GameWindow_ApplyScroll(GameWindowManager *manager);
void GameWindow_Upload(GameWindowManager *manager, GameWindow *window);
void GameWindow_UploadDirty(GameWindowManager *manager);
void GameWindow_UploadSubTilemap(GameWindowManager *manager);
void GameWindow_UploadMainTilemap(GameWindowManager *manager);
void GameWindow_ConfigureSubBackground(GameWindowManager *manager, int screen_base, int char_base);
void GameWindow_ConfigureMainBackground(GameWindowManager *manager, int screen_base, int char_base);
void GameWindow_RebuildTilemap(GameWindowManager *manager, int screen);
int GameWindow_Release(GameWindowManager *manager, s16 index);
void GameWindow_MeasureText(GameWindowManager *manager, const u8 *string, const u32 *const *fonts, GameTextBounds *bounds, u8 compact);
void GameWindow_AlignText(GameWindowManager *manager, GameWindow *window, GameText *text, int alignment);
void GameWindow_StartTextSound(GameWindowManager *manager, GameWindow *window);
#endif
