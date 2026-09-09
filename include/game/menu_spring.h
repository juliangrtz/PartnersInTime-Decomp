#ifndef PIT_GAME_MENU_SPRING_H
#define PIT_GAME_MENU_SPRING_H
#include <nitro.h>

/* Position, previous position and velocity use 12 fractional bits. */
typedef struct MenuSpringPoint {
    s32 x, y, z;
    s32 previous_x, previous_y, previous_z;
    s32 velocity_x, velocity_y, velocity_z;
    s32 gravity_scale; /* 8 fractional bits */
    u16 pinned, unknown_2a;
} MenuSpringPoint;
typedef struct MenuSpringLink {
    MenuSpringPoint *first, *second;
    s32 length;
} MenuSpringLink;
typedef struct MenuSpringChain {
    int state;
    u8 unknown_04[16];
    s32 minimum_y;
    MenuSpringPoint points[4];
    MenuSpringLink links[3];
} MenuSpringChain;

typedef char MenuSpringPointSizeCheck[sizeof(MenuSpringPoint) == 44 ? 1 : -1];
typedef char MenuSpringLinkSizeCheck[sizeof(MenuSpringLink) == 12 ? 1 : -1];
typedef char MenuSpringChainSizeCheck[sizeof(MenuSpringChain) == 236 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void MenuSpring_AdvanceActiveChains(void);
void MenuSpring_Update(MenuSpringChain *chain, int iterations);
void MenuSpring_InitChain(int index, int x, int y);
void MenuSpring_UpdateVelocities(MenuSpringPoint *point, int count);
void MenuSpring_ConstrainLinks(MenuSpringLink *link, int count);
void MenuSpring_IntegratePoints(MenuSpringPoint *point, int count, int minimum_y);
#ifdef __cplusplus
}
#endif
#endif
