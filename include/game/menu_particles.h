#ifndef PIT_GAME_MENU_PARTICLES_H
#define PIT_GAME_MENU_PARTICLES_H
#include <nitro.h>
#include <nitro/fx.h>

typedef struct MenuParticleTask {
    u8 unknown_00[40];
    u16 tile, frame, frames_per_image, draw_list, delay;
    u8 unknown_32[10];
    fx32 velocity_x, velocity_y;
    u8 unknown_44[4];
} MenuParticleTask;

/* The common attached sprite prefix, including its affine matrix. */
typedef struct MenuParticleSprite {
    u8 unknown_00[24];
    union {
        u32 attributes_1;
        struct {
            u32 tile : 10, unknown_10_31 : 22;
        } bits;
    };
    fx32 x, y, offset_x, offset_y;
    s16 origin_x, origin_y;
    u8 unknown_30[8];
    s16 matrix[4];
} MenuParticleSprite;
typedef char MenuParticleTask_SizeCheck[sizeof(MenuParticleTask) == 72 ? 1 : -1];
typedef char MenuParticleSprite_SizeCheck[sizeof(MenuParticleSprite) == 64 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void SceneMenuParticle_Update(MenuParticleTask *task);
void ShopParticle_Update(MenuParticleTask *task);
#ifdef __cplusplus
}
#endif
#endif
