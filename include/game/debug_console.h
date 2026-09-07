#ifndef PIT_GAME_DEBUG_CONSOLE_H
#define PIT_GAME_DEBUG_CONSOLE_H
#include <nitro.h>

typedef struct GameConsole {
    u16 tiles[24][32];
    char text[64];
    unsigned char sub_screen : 1;
    unsigned char background : 2;
    unsigned char reserved : 5;
    u8 x, y, palette;
} GameConsole;

void GameConsole_Write(GameConsole *console, const char *text);
void GameConsole_Printf(GameConsole *console, const char *format, ...);
void GameConsole_SetCursor(GameConsole *console, u8 x, u8 y);
void GameConsole_Upload(GameConsole *console);
void GameConsole_Clear(GameConsole *console);
GameConsole *GameConsole_Init(GameConsole *console, int sub_screen, int background,
                              int character_base, int screen_base, int palette, int color);
#endif
