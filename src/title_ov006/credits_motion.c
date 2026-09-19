#include "frontend_scene_internal.h"

void CreditsMotion_SetEaseOut(CreditsMotion *motion, int frames)
{
    int dx = motion->target_x - motion->x;
    int dy = motion->target_y - motion->y;
    motion->vx = 2 * dx / frames;
    motion->vy = 2 * dy / frames;
    motion->ax = -motion->vx / frames;
    motion->ay = -motion->vy / frames;
    motion->frames = frames;
}

void CreditsMotion_SetAcceleration(CreditsMotion *motion, int frames)
{
    int dx = motion->target_x - motion->x;
    int dy = motion->target_y - motion->y;
    motion->ax = 2 * (dx - motion->vx * frames) / (frames * frames);
    motion->ay = 2 * (dy - motion->vy * frames) / (frames * frames);
    motion->frames = frames;
}

/* Only the first cell advances the shared illustration fade-in. */
void CreditsGridFade_Update(CreditsMotion *particle)
{
    switch (particle->phase) {
    case 0:
        particle->counter = 0;
        ++particle->phase;
        break;
    case 1:
        particle->counter += 2116;
        if (particle->counter >= 0x1f000)
            particle->phase = 100;
        data_ov006_0207c594.polygon_alpha = particle->counter >> 12;
        if (data_ov006_0207c594.polygon_alpha > 31)
            data_ov006_0207c594.polygon_alpha = 31;
        break;
    case 100:
        if (data_ov006_0207c594.active)
            --data_ov006_0207c594.active;
        ++particle->phase;
        break;
    }
}
