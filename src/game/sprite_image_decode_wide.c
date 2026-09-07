#include <game/sprite_image.h>

void GameSpriteImage_Decode16x16(const u32 *source, u8 *destination)
{
    GameSpriteImage_Decode8x16(source, destination);
    GameSpriteImage_Decode8x16(source + 8, destination + 8);
}

void GameSpriteImage_Decode16x20(const u32 *source, u8 *destination)
{
    GameSpriteImage_Decode8x20(source, destination);
    GameSpriteImage_Decode8x20(source + 10, destination + 8);
}

void GameSpriteImage_Decode16x24(const u32 *source, u8 *destination)
{
    GameSpriteImage_Decode8x24(source, destination);
    GameSpriteImage_Decode8x24(source + 12, destination + 8);
}
