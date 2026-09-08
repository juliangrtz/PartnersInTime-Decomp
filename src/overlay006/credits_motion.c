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
