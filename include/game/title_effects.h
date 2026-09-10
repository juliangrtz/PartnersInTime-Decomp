#ifndef PIT_GAME_TITLE_EFFECTS_H
#define PIT_GAME_TITLE_EFFECTS_H

#include <game/battle_scene.h>
#include <game/texture_allocation.h>

typedef struct TitleTextureHeader {
    u32 unknown_00, unknown_04;
    struct {
        u32 width_shift : 4, height_shift : 4, format : 4, unknown_12_31 : 20;
    } flags;
    u32 unknown_0c;
} TitleTextureHeader;
typedef struct TitleTextureResource {
    TitleTextureHeader *header;
    void *pixels;
    u32 pixel_bytes;
    void *palette_data;
    u16 palette_bytes, unknown_12;
    GameTextureAllocation texture;
    GameTexturePalette palette;
} TitleTextureResource;
/* Position view shared with the separate child renderer. */
typedef struct TitleOrbitChild {
    u8 unknown_00[12];
    s32 x, y;
} TitleOrbitChild;
typedef char TitleOrbitChild_SizeCheck[sizeof(TitleOrbitChild) == 20 ? 1 : -1];

typedef struct TitleSpriteLayout {
    s32 image_x, image_y, width, height;
} TitleSpriteLayout;
typedef char TitleSpriteLayout_SizeCheck[sizeof(TitleSpriteLayout) == 16 ? 1 : -1];

typedef struct TitleOrbitWork TitleOrbitWork;
struct TitleOrbitWork {
    void *unknown_00;
    s32 timer;
    u32 unknown_08;
    s32 x, y, depth;
    void (*update)(TitleOrbitWork *);
    void (*draw)(TitleOrbitWork *);
    u32 unknown_20;
    u8 phase, unknown_25, model_mask, unknown_27;
    BattleModel *sub_model, *main_model;
    TitleOrbitChild *child;
    s32 orbit_x, orbit_y, fall_velocity, start_angle, angle;
    u32 hidden_frames;
    u16 rotation;
    s8 countdown;
    u8 render_phase;
    struct {
        u8 side : 1, slot : 4, unknown_05_07 : 3;
    } flags;
    u8 unknown_51[3];
};
typedef char TitleTextureHeader_SizeCheck[sizeof(TitleTextureHeader) == 16 ? 1 : -1];
typedef char TitleTextureResource_SizeCheck[sizeof(TitleTextureResource) == 68 ? 1 : -1];
typedef char TitleOrbitWork_SizeCheck[sizeof(TitleOrbitWork) == 84 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

u32 TitleTexture_GetPaletteOffset(const TitleTextureResource *texture);
u32 TitleTexture_GetImageOffset(const TitleTextureResource *texture);
u32 TitleTexture_GetHeightCode(const TitleTextureResource *texture);
u32 TitleTexture_GetWidthCode(const TitleTextureResource *texture);
u32 TitleTexture_GetFormat(const TitleTextureResource *texture);
void TitleTexture_Release(TitleTextureResource *texture);
void TitleTexture_Upload(TitleTextureResource *texture);
void TitleTexture_DrawLayout(TitleTextureResource *texture, const TitleSpriteLayout *layout, int alpha,
                             int polygon_id);
void TitleTexture_Load(TitleTextureResource *texture, int image_id, u16 palette_id, int compressed);
void TitleOrbit_Destroy(TitleOrbitWork *work);
void TitleOrbit_Reset(TitleOrbitWork *work);
void TitleOrbit_StartPulse(TitleOrbitWork *work);
void TitleOrbit_StartExpansion(TitleOrbitWork *work);
void TitleOrbit_StartEntry(TitleOrbitWork *work);
void TitleOrbit_Init(TitleOrbitWork *work, TitleOrbitChild *child, int side, int slot);
void TitleOrbit_Draw(TitleOrbitWork *work);

#ifdef __cplusplus
}
#endif
#endif
