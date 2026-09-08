#include <game/title_effects.h>
#include <game/heap.h>

extern void *data_ov006_0207c4e4;
void *func_ov006_0206b77c(void *archive, int kind, int entry, int compressed, int argument_4, int heap,
                          u32 *size);
void DC_FlushRange(const void *address, u32 size);

void TitleTexture_Load(TitleTextureResource *texture, int image_id, u16 palette_id, int compressed)
{
    u32 image_size, palette_size;
    TitleTextureHeader *image =
        func_ov006_0206b77c(data_ov006_0207c4e4, 2, image_id, compressed != 0, 0, 1, &image_size);
    void *palette = func_ov006_0206b77c(data_ov006_0207c4e4, 2, palette_id, 0, 0, 1, &palette_size);
    DC_FlushRange(image, image_size);
    DC_FlushRange(palette, palette_size);
    texture->header = image;
    texture->pixels = image + 1;
    texture->pixel_bytes = image_size - 16;
    texture->palette_data = palette;
    texture->palette_bytes = palette_size;
    GameTextureAllocation_Allocate(&texture->texture, 1, texture->pixel_bytes, 1, image_id | 0xff000000, 0, 1,
                                   0);
    GameTexturePalette_Allocate(&texture->palette, 0, texture->header->flags.format,
                                texture->palette_bytes / 2, 1, 1, 0, texture->palette_data,
                                palette_id | 0xff000000, 0);
}
void TitleTexture_Release(TitleTextureResource *texture)
{
    if (texture->header) {
        GameHeap_Free(texture->header);
        texture->header = 0;
    }
    if (texture->palette_data) {
        GameHeap_Free(texture->palette_data);
        texture->palette_data = 0;
    }
    GameTextureAllocation_Unlink(&texture->texture);
    GameTexturePalette_Unlink(&texture->palette);
}
u32 TitleTexture_GetFormat(const TitleTextureResource *texture)
{
    return texture->header->flags.format;
}
u32 TitleTexture_GetWidthCode(const TitleTextureResource *texture)
{
    return texture->header->flags.width_shift;
}
u32 TitleTexture_GetHeightCode(const TitleTextureResource *texture)
{
    return texture->header->flags.height_shift;
}
u32 TitleTexture_GetImageOffset(const TitleTextureResource *texture)
{
    return texture->texture.offset;
}
u32 TitleTexture_GetPaletteOffset(const TitleTextureResource *texture)
{
    return texture->palette.offset;
}
