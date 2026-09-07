#ifndef PIT_GAME_FORMAT_H
#define PIT_GAME_FORMAT_H

#include <nitro.h>

int GameFormat_Length(const char *text);
u8 *GameFormat_Binary(u8 *destination, int width, int flags, u32 value);
u8 *GameFormat_Decimal(u8 *destination, int width, int flags, u32 value);
u8 *GameFormat_Hex(u8 *destination, int width, int flags, u32 value);
char *GameFormat_String(char *destination, int width, int flags, const char *text);

#endif
