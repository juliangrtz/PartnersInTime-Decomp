#include <game/menu_spring.h>

extern MenuSpringChain data_ov007_020a67dc[4];
extern u8 data_ov007_020a67d8[];
extern u8 data_ov007_020905f0[];
extern s32 FX_Sqrt(s32);
extern s32 FX_Div(s32, s32);
static inline s32 FixedMultiply(s32 a, s32 b)
{
    return ((s64)a * b + 2048) >> 12;
}
void MenuSpring_IntegratePoints(MenuSpringPoint *point, int count, int minimum_y)
{
    int i;
    for (i = 0; i < count; i++, point++) {
        point->x += point->velocity_x;
        point->y += point->velocity_y;
        if (!point->pinned) {
            point->y += (1638 * point->gravity_scale) >> 8;
            if (point->y < minimum_y)
                point->y = minimum_y;
        }
    }
}

void MenuSpring_ConstrainLinks(MenuSpringLink *link, int count)
{
    MenuSpringPoint *first, *second;
    int i;
    for (i = 0; i < count; i++, link++) {
        first = link->first;
        second = link->second;
        {
            s32 dx = first->x - second->x, dy = first->y - second->y;
            s32 length = FX_Sqrt(FixedMultiply(dx, dx) + FixedMultiply(dy, dy));
            s32 force = (230 * (length - link->length)) >> 8;
            s32 correction_x, correction_y;
            if (length)
                force = FX_Div(force, length);
            correction_x = FixedMultiply(dx, force);
            correction_y = FixedMultiply(dy, force);
            first->x -= correction_x;
            first->y -= correction_y;
            second->x += correction_x;
            second->y += correction_y;
        }
    }
}

void MenuSpring_UpdateVelocities(MenuSpringPoint *point, int count)
{
    int i;
    for (i = 0; i < count; i++, point++) {
        if (!point->pinned) {
            point->velocity_x = (230 * (point->x - point->previous_x)) >> 8;
            point->velocity_y = (230 * (point->y - point->previous_y)) >> 8;
        } else {
            point->velocity_x = 0;
            point->velocity_y = 0;
            point->x = point->previous_x;
            point->y = point->previous_y;
            point->z = point->previous_z;
        }
        point->previous_x = point->x;
        point->previous_y = point->y;
        point->previous_z = point->z;
    }
}

void MenuSpring_InitChain(int index, int x, int y)
{
    MenuSpringChain *chain = &data_ov007_020a67dc[index];
    MenuSpringLink *link = chain->links;
    int i;
    MenuSpringPoint *point = chain->points;
    for (i = 0; i < 3; i++, point++, link++) {
        link->first = point;
        link->second = &chain->points[i + 1];
        link->length = 0x4000;
    }
    chain->links[0].length = 0x10000;
    point = chain->points;
    for (i = 0; i < 4; i++, point++) {
        point->x = x;
        point->y = y + ((4 - i) << 14);
        if (i == 0)
            point->y += chain->links[0].length - 0x4000;
        point->previous_x = point->x;
        point->previous_y = point->y;
        point->velocity_x = 0;
        point->velocity_y = 0;
        point->velocity_z = 0;
        if (i == 0)
            point->gravity_scale = 512;
        else
            point->gravity_scale = 256;
        if (i == 3)
            point->pinned = 1;
        else
            point->pinned = 0;
    }
}

void MenuSpring_Update(MenuSpringChain *chain, int iterations)
{
    int i;
    MenuSpring_IntegratePoints(chain->points, 4, chain->minimum_y);
    for (i = 0; i < iterations; i++)
        MenuSpring_ConstrainLinks(chain->links, 3);
    MenuSpring_UpdateVelocities(chain->points, 4);
}

void MenuSpring_AdvanceActiveChains(void)
{
    int i;
    u8 *available = data_ov007_020905f0;
    u8 *work = data_ov007_020a67d8;
    for (i = 0; i < 4; i++, available++, work += sizeof(MenuSpringChain)) {
        if (available[278])
            ((MenuSpringChain *)(work + 4))->state++;
    }
}
