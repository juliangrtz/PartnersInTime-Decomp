#ifndef PIT_GAME_TEXT_H
#define PIT_GAME_TEXT_H
#include <nitro.h>

typedef union GameTextCursor {
    u32 raw;
    struct { u32 x:10, y:10, spacing:4, leading:4, base_color:4; } bits;
} GameTextCursor;
typedef union GameTextStyle {
    u32 raw;
    struct { u32 color:4, space_width:5, tile_pitch:7, alignment_width:7, language:8, reserved:1; } bits;
} GameTextStyle;
typedef union GameTextFontState {
    u32 raw;
    u16 font;
    struct { u32 font:16, reserved:1, alignment:2, reserved2:3, origin_x:10; } bits;
} GameTextFontState;
typedef union GameTextLineState {
    u32 raw;
    u8 origin_y;
    struct { u32 origin_y:8, alignment:2, whole_image:1, height:6, spacing:4, measuring:1, start_x:10; } bits;
} GameTextLineState;
typedef union GameTextGlyphRect {
    u32 raw;
    struct { u32 x:10, y:10, width:6, height:6; } bits;
} GameTextGlyphRect;
typedef struct GameTextBounds {
    union {
        u32 raw;
        u8 lines;
        struct { u32 lines:8, x:10, y:10, reserved:4; } bits;
    } origin;
    u16 width, height;
} GameTextBounds;
typedef struct GameText {
    void *destination;
    const u8 *text;
    GameTextCursor cursor;
    GameTextStyle style;
    s16 scale_x, scale_y;
    const u32 *const *fonts;
    u8 margin;
    u8 reserved[3];
    GameTextFontState font;
    GameTextLineState line;
    GameTextGlyphRect glyph;
    GameTextBounds bounds;
} GameText;
typedef union GameTextCharacter {
    u16 raw;
    struct { u8 image, index; } bytes;
} GameTextCharacter;
typedef struct GameTextToken {
    union {
        u32 raw;
        struct { GameTextCharacter character; u16 code; } fields;
    } value;
    const u8 *text;
} GameTextToken;
typedef int (*GameTextCallback)(void *argument, GameText *text, int skip_controls);
typedef struct GameTextControl {
    s8 length;
    u8 reserved[3];
    GameTextCallback callback;
    void *argument;
} GameTextControl;

typedef char GameTextSizeCheck[sizeof(GameText) == 48 ? 1 : -1];
typedef char GameTextTokenSizeCheck[sizeof(GameTextToken) == 8 ? 1 : -1];
typedef char GameTextBoundsSizeCheck[sizeof(GameTextBounds) == 8 ? 1 : -1];
extern GameTextControl data_0205a934[];
void GameText_Reset(GameText *text);
void GameText_Init(GameText *text, const u32 *const *fonts, void *destination,
    const u8 *string, u8 x, u8 y, u8 spacing, u8 leading, u8 color,
    u8 space_width, u8 tile_pitch, int unused, u8 alignment_width, u8 margin);
GameTextToken GameText_Next(GameText *text, int measure, int skip_controls);
int GameText_ProcessControl(GameText *text, GameTextToken *token, int skip_controls);
u16 GameText_MeasureLine(const GameText *text);
/* Packs the line count above the measured height, as height + (lines << 8). */
u16 GameText_MeasureHeight(const GameText *text);
void GameText_MeasureBounds(const GameText *text, GameTextBounds *bounds);
void GameText_SetControl(int index, s8 length, GameTextCallback callback, void *argument);
#endif
