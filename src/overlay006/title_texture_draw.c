#include <game/title_effects.h>
extern u8 *data_ov006_0207b134;
extern int data_02059d78;
void func_0203b578(int, const void *, void *, u32);
void func_ov006_02071974(TitleTextureResource *, int, int, int, int, int, int);

void TitleTexture_DrawLayout(TitleTextureResource *texture, const TitleSpriteLayout *layout, int alpha,
                             int polygon_id)
{
    func_ov006_02071974(texture, layout->image_x, layout->image_y, layout->width, layout->height, alpha,
                        polygon_id);
}

void TitleTexture_Upload(TitleTextureResource *texture)
{
    u32 offset = TitleTexture_GetImageOffset(texture);
    func_0203b578(data_02059d78, texture->pixels, data_ov006_0207b134 + offset, texture->pixel_bytes);
}
