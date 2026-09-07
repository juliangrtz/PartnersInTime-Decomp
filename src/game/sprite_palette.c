#include <game/sprite_output.h>

extern u8 *data_02050960[2];
extern u32 func_02035588(void), func_02035578(void);
extern void GX_BeginLoadOBJExtPltt(void), GX_EndLoadOBJExtPltt(void);
extern void GXS_BeginLoadOBJExtPltt(void), GXS_EndLoadOBJExtPltt(void);

void GameSpritePalette_Unlink(GameSpritePalette *palette)
{
    int screen = palette->flags.bits.screen;
    if (data_0205a07c[screen] && data_0205a07c[screen] == palette)
        data_0205a07c[screen] = palette->next;
    if (data_0205a084[screen] && data_0205a084[screen] == palette)
        data_0205a084[screen] = palette->previous;
    if (palette->previous) palette->previous->next = palette->next;
    if (palette->next) palette->next->previous = palette->previous;
    palette->next = 0;
    palette->previous = palette->next;
    palette->flags.raw &= ~0x20;
}

void *GameSpritePalette_GetBuffer(const GameSpritePalette *palette)
{
    int bank = palette->bank;
    if (bank == 16) return data_0205a094[palette->flags.bits.screen] + 32 * palette->first;
    return data_0205a08c[palette->flags.bits.screen] + (bank << 9) + 32 * palette->first;
}

void GameSpritePalette_MarkDirty(GameSpritePalette *palette, const void *source)
{
    if (source) {
        u32 size = 32 * palette->count;
        void *destination = GameSpritePalette_GetBuffer(palette);
        GameSprite_CopyBytes(source, destination, size);
        palette->flags.raw |= 0x18;
    } else palette->flags.raw |= 8;
}

void GameSpritePalette_UploadScreen(int screen)
{
    GameSpritePalette_Upload(screen, data_0205a08c[screen], data_0205a094[screen]);
}

void GameSpritePalette_Upload(int screen, const u8 *extended, const u8 *standard)
{
    int changed_mapping;
    u8 *standard_vram;
    u8 *extended_vram;
    GameSpritePalette *palette;
    palette = data_0205a07c[screen];
    standard_vram = data_02050960[screen];
    changed_mapping = 0;
    if (!palette) return;
    if (palette->bank <= 15) {
        if (!screen) {
            u32 bank = func_02035588();
            if (bank == 32) extended_vram = (u8 *)0x06890000;
            else if (bank == 64) extended_vram = (u8 *)0x06894000;
            GX_BeginLoadOBJExtPltt();
        } else {
            if (func_02035578() == 256) extended_vram = (u8 *)0x068a0000;
            GXS_BeginLoadOBJExtPltt();
        }
        changed_mapping = 1;
    }
    while (palette) {
        if (palette->flags.raw & 8) {
            u32 bank = palette->bank;
            int first = palette->first;
            int count = palette->count;
            if (bank <= 15) {
                if (palette->flags.raw & 0x10)
                    GameSprite_CopyBytes(extended + (bank << 9), extended_vram + (bank << 9) + 32 * first, 32 * count);
                else
                    GameSprite_CopyBytes(palette->data, extended_vram + (bank << 9) + 32 * first, 32 * count);
            } else {
                if (palette->flags.raw & 0x10)
                    GameSprite_CopyBytes(standard + 32 * first, standard_vram + 32 * first, 32 * count);
                else
                    GameSprite_CopyBytes(palette->data, standard_vram + 32 * first, 32 * count);
            }
            palette->flags.raw &= ~0x18;
        }
        palette = palette->next;
    }
    if (changed_mapping == 1) {
        if (!screen) GX_EndLoadOBJExtPltt();
        else GXS_EndLoadOBJExtPltt();
    }
}
