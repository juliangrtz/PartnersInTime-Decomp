/* Field sprite animation start and graphics setup (0x020BF2E0..0x020BF510). */

#include <game/field_animation.h>
#include <game/graphics_resource.h>
#include <nitro/gx_init.h>
extern void func_02037190(void), func_02036d70(u32), func_02036e28(void), func_02037024(void);
extern void func_02036ee0(int, int, int, int, int), func_02036f24(int, int, int, int);
#define REG16(address) (*(vu16 *)(address))
#define REG32(address) (*(vu32 *)(address))
void FieldSpriteAnimation_InitializeGraphics(FieldSpriteAnimation *state)
{
    if (state->flags.active)
        return;
    /* Keep the main window layer at a fixed scroll while the effect runs. */
    state->field_system->windows->display_flags |= 0x02000000;
    GX_SetGraphicsMode(1, 0, 1);
    func_02037190();
    func_02036d70(0);
    func_02036e28();
    func_02037024();
    func_02036ee0(0, 0, 0x7fff, 63, 0);
    /* Preserve all five DISP3DCNT updates and their hardware write order. */
    REG16(0x04000060) &= ~0x3002;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 0x10;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 8;
    REG16(0x04000060) &= 0xcffb;
    REG16(0x04000060) &= 0xcfdf;
    func_02036f24(0, 0, 0, 0);
    REG32(0x04000580) = 0xbfff0000;
    REG32(0x04000540) = 1;
    GameGraphics_SetOrthographicProjection(0, -4096);
    /* Reuse existing alpha blending; Stop releases only blending we enabled. */
    if ((REG16(0x04000050) & 0xc0) == 0x40) {
        REG16(0x04000050) |= 1;
        state->flags.owns_blend = 0;
    } else {
        REG16(0x04000050) = 0x1e41;
        state->flags.owns_blend = 1;
    }
    state->projection._00 = 256;
    state->projection._11 = 256;
    state->projection._22 = 1;
    state->projection._30 = 0;
    state->projection._31 = 0;
    state->projection._32 = 512;
    state->projection._33 = 4096;
    state->flags.active = 1;
}

GameSpriteAnimationTrack *FieldSpriteAnimation_Start(FieldSpriteAnimation *state, int index,
    const s16 *commands, int x, s16 y, s16 speed)
{
    GameSpriteAnimationTrack *track;
    FieldSpriteAnimation_InitializeGraphics(state);
    track = GameSpriteAnimation_Start(state->animation, commands, x, y, 0, speed);
    state->tracks[index] = track;
    return track;
}
