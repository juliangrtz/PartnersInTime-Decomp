#include <game/format.h>

int GameFormat_Length(const char *text)
{
    int length = 0;
    if (*text) {
        do {
            ++text;
            ++length;
        } while (*text);
    }
    return length;
}

u8 *GameFormat_Binary(u8 *destination, int width, int flags, u32 value)
{
    u8 padding = (flags & 1) ? '0' : ' ';
    int count = 0;
    u8 digits[32];
    u8 *cursor;
    if (width > 32) width = 32;
    cursor = digits;
    do {
        *cursor++ = (value & 1) + '0';
        ++count;
        value >>= 1;
    } while ((!width || count < width) && value);
    if (width > 0 && count < width) {
        cursor = digits + count;
        do {
            ++count;
            *cursor++ = padding;
        } while (count < width);
    }
    cursor = digits + count;
    do {
        *destination++ = *--cursor;
        --count;
    } while (count > 0);
    *destination = 0;
    return destination;
}
