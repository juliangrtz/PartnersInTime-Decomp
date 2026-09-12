#include <game/title_animation.h>
#include <game/title_effects.h>
#include "title_graphics_internal.h"
#include <game/heap.h>

extern void *data_ov006_0207c4e4;
void DC_FlushRange(const void *address, u32 size);

void TitleTexture_Load(TitleTextureResource *texture, int image_id, u16 palette_id, int compressed)
{
    u32 image_size, palette_size;
    TitleTextureHeader *image =
        TitleAnimation_ReadArchiveEntry(data_ov006_0207c4e4, 2, image_id, compressed != 0, 0, 1, &image_size);
    void *palette = TitleAnimation_ReadArchiveEntry(data_ov006_0207c4e4, 2, palette_id, 0, 0, 1, &palette_size);
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

void TitleTexture_DrawQuad(TitleTextureResource *texture, int x, int y, int width, int height, int alpha,
                           int polygon_id)
{
    u32 format, image, width_code, height_code;
    s16 left, top, right, bottom;
    int s0, t0, s1, t1;
    int polygon = polygon_id;
    if (!alpha)
        return;
    if (polygon == -1)
        polygon = alpha == 31 ? 0 : 8;
    PolygonAttributes(0, 0, 3, polygon, alpha, 0);
    format = TitleTexture_GetFormat(texture);
    REG32(0x040004ac) = TitleTexture_GetPaletteOffset(texture) >> (4 - (format == 2));
    image = TitleTexture_GetImageOffset(texture);
    height_code = TitleTexture_GetHeightCode(texture);
    width_code = TitleTexture_GetWidthCode(texture);
    format = TitleTexture_GetFormat(texture);
    REG32(0x040004a8) =
        (image >> 3) | (format << 26) | 0x40000000 | (width_code << 20) | (height_code << 23) | 0x20000000;
    left = (-width / 2) << 8;
    top = (-height / 2) << 8;
    right = (width / 2) << 8;
    bottom = (height / 2) << 8;
    s0 = x << 12;
    t0 = y << 12;
    s1 = (x + width) << 12;
    t1 = (y + height) << 12;
    REG32(0x04000500) = 1;
    TexCoord(s0, t0);
    Vertex(left, top, 0);
    TexCoord(s0, t1);
    VertexXY(left, bottom);
    TexCoord(s1, t1);
    VertexXY(right, bottom);
    TexCoord(s1, t0);
    VertexXY(right, top);
    REG32(0x04000504) = 0;
}

extern u8 *data_ov006_0207b134;
extern int data_02059d78;
void func_0203b578(int, const void *, void *, u32);

void TitleTexture_DrawLayout(TitleTextureResource *texture, const TitleSpriteLayout *layout, int alpha,
                             int polygon_id)
{
    TitleTexture_DrawQuad(texture, layout->image_x, layout->image_y, layout->width, layout->height, alpha,
                          polygon_id);
}

void TitleTexture_Upload(TitleTextureResource *texture)
{
    u32 offset = TitleTexture_GetImageOffset(texture);
    func_0203b578(data_02059d78, texture->pixels, data_ov006_0207b134 + offset, texture->pixel_bytes);
}
