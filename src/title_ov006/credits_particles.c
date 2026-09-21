/* The final credits reveal emits a trail of six-frame, fading particles. */
#include "frontend_scene_internal.h"
#include "title_graphics_internal.h"
#include <game/random.h>

typedef struct CreditsParticleEmitter {
    u8 unknown_00[16];
    MenuElement *parent;
    u8 unknown_14[12];
    int phase, delay;
    s32 x, y;
    u32 unknown_30;
    int emission_count;
    u8 unknown_38[16];
} CreditsParticleEmitter;

typedef struct CreditsSmallParticle {
    u8 unknown_00[40];
    u8 frame, frame_duration, timer, alpha;
    s32 x, y, vx, vy;
    u8 unknown_3c[12];
} CreditsSmallParticle;

typedef char CreditsParticleEmitter_SizeCheck[sizeof(CreditsParticleEmitter) == 72 ? 1 : -1];
typedef char CreditsSmallParticle_SizeCheck[sizeof(CreditsSmallParticle) == 72 ? 1 : -1];

extern const u8 data_ov006_0207b528[];
extern s16 data_ov006_0207c554[32];
void func_ov005_0206650c(void *);
int _s32_div_f(int, int);

void CreditsSmallParticle_Update(CreditsSmallParticle *particle)
{
    int x, y, left, right;
    ++particle->timer;
    particle->frame = _s32_div_f(particle->timer, particle->frame_duration);
    if (particle->frame >= 6 || !particle->alpha) {
        func_ov005_0206650c(particle);
        return;
    }
    particle->x += particle->vx;
    particle->y += particle->vy;
    particle->vx = 94 * particle->vx / 100;
    particle->vy = 94 * particle->vy / 100;
    x = particle->x;
    y = particle->y;

    /* Motion advances on either pass; only the owning screen draws and fades.
     * The credits coordinate space includes a 32-pixel gap between screens. */
    if (!data_ov006_0207c594.background_variant) {
        if (y <= 192 * 4096) return;
        y -= 224 * 4096;
    } else if (y > 192 * 4096) return;

    PolygonAttributes(0, 0, 3, data_ov006_0207c594.polygon_id, particle->alpha, 0);
    REG32(0x04000480) = 32767;
    REG32(0x04000444) = 0;
    --particle->alpha;
    left = (5 - particle->frame) << 16;
    right = left + 65536;
    /* The shared depth counts draw slots; the matrix takes Q12 coordinates. */
    Translate(x, y, data_ov006_0207c594.draw_depth << 12);
    Scale(65536, 65536, 4096);
    REG32(0x04000480) = 32767;
    REG32(0x04000500) = 3;
    TexCoord(left, 0);
    Vertex(0, 0, 0);
    TexCoord(left, 65536);
    Vertex(0, 4096, 0);
    TexCoord(right, 0);
    Vertex(4096, 0, 0);
    TexCoord(right, 65536);
    Vertex(4096, 4096, 0);
    REG32(0x04000504) = 0;
    REG32(0x04000448) = 1;
    ++data_ov006_0207c594.draw_depth;
    ++data_ov006_0207c594.polygon_id;
}

void CreditsParticleEmitter_Update(CreditsParticleEmitter *task)
{
    int count, i;
    CreditsSmallParticle *particle;
    REG32(0x040004a8) = 1153442368;
    REG32(0x040004ac) = 74;
    if (task->delay) {
        --task->delay;
        return;
    }
    switch (task->phase) {
    case 0:
        task->x = (176 - (int)((u32)data_ov006_0207b528[gSaveData[0x515]] >> 1)) << 12;
        task->y = 172 * 4096;
        ++task->phase;
        break;
    case 1:
        task->x += 4096;
        if (task->x >= 352 * 4096) {
            task->parent->arguments[5] = 0;
            func_ov005_0206650c(task);
            return;
        }
        break;
    }
    /* Reveal successive strips behind the emitter, saturating their alpha.
     * The count can be negative before reaching the localized title's edge. */
    count = (task->x - ((208 - (int)((u32)data_ov006_0207b528[gSaveData[0x515]] >> 1)) << 12)) / 16384;
    if (count > 32) count = 32;
    for (i = 0; i < count; ++i) {
        ++data_ov006_0207c554[i];
        if (data_ov006_0207c554[i] > 31) data_ov006_0207c554[i] = 31;
    }
    particle = (CreditsSmallParticle *)func_ov005_0206659c(
        (void (*)(MenuElement *))CreditsSmallParticle_Update, 3, 1);
    particle->x = task->x - 32768;
    particle->y = task->y + 216 * 4096;
    particle->x -= Random_NextModulo(8) << 12;
    /* Shift the unsigned representation of the negated random value, then
     * restore signed Q12 division; this preserves truncation toward zero. */
    particle->vx = (s32)((u32)-Random_NextModulo(20) << 12) / 10;
    if (task->emission_count & 1) {
        particle->y -= Random_NextModulo(4) << 12;
        particle->vy = (s32)((u32)-Random_NextModulo(16) << 12) / 10;
    } else {
        particle->y += Random_NextModulo(4) << 12;
        particle->vy = (Random_NextModulo(16) << 12) / 10;
    }
    particle->frame = 0;
    particle->timer = 0;
    particle->frame_duration = 5;
    particle->alpha = 31;
    ++task->emission_count;
}
