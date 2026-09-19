#include "credits_transition_internal.h"

/* Fade the illustration out, replace its cell positions, then fade it in. */
void CreditsFade_Update(CreditsMotion *particle)
{
    switch (particle->phase) {
    case 0:
        particle->counter = 0;
        ++particle->phase;
        break;
    case 1:
        particle->counter += 2116;
        if (particle->counter >= 0x1f000)
            ++particle->phase;
        CREDITS_TRANSITION.polygon_alpha = 31 - (particle->counter >> 12);
        if (CREDITS_TRANSITION.polygon_alpha < 0)
            CREDITS_TRANSITION.polygon_alpha = 0;
        break;
    case 2: {
        CreditsMotion *tile = data_ov006_020847d0;
        int row, column;
        particle->counter = 120;
        for (row = 0; row < 32; ++row) {
            for (column = 0; column < 32; ++column) {
                tile->variant = CREDITS_TRANSITION.variant;
                CreditsMotion_TargetLayoutCell(tile, CREDITS_TRANSITION.layout,
                                               CREDITS_TRANSITION.screen);
                tile->x = tile->target_x;
                tile->y = tile->target_y;
                ++tile;
            }
        }
        ++particle->phase;
        break;
    }
    case 3:
        if (particle->counter)
            --particle->counter;
        else
            ++particle->phase;
        break;
    case 4:
        particle->counter += 1058;
        if (particle->counter >= 0x1f000)
            particle->phase = 100;
        CREDITS_TRANSITION.polygon_alpha = particle->counter >> 12;
        if (CREDITS_TRANSITION.polygon_alpha > 31)
            CREDITS_TRANSITION.polygon_alpha = 31;
        break;
    case 100:
        if (CREDITS_TRANSITION.active)
            --CREDITS_TRANSITION.active;
        ++particle->phase;
        break;
    }
}

void CreditsFade_InitAll(void)
{
    CreditsMotion *particle = data_ov006_020847d0;
    int i;
    for (i = 0; i < 1024; ++i) {
        particle->update = CreditsFade_Update;
        particle->phase = 100;
        ++CREDITS_TRANSITION.active;
        ++particle;
    }
    /* The first cell drives the fade; the others retire through phase 100. */
    data_ov006_020847d0[0].phase = 0;
}
