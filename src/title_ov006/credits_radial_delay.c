#include "credits_transition_internal.h"
extern const s32 data_ov006_0207bfe0[][4];
void CreditsRadialDelay_Update(CreditsMotion *particle)
{
    switch (particle->phase) {
    case 0:
        if (particle->counter) --particle->counter;
        else particle->phase = 100;
        break;
    case 100:
        CreditsMotion_TargetLayoutCenter(particle, CREDITS_TRANSITION.layout, CREDITS_TRANSITION.screen);
        particle->vx = 0;
        if (data_ov006_0207bfe0[CREDITS_TRANSITION.layout][0] > 24) particle->vx = -16384;
        else particle->vx = 16384;
        particle->vy = 16384;
        if (!CREDITS_TRANSITION.screen) particle->vy = -particle->vy;
        CreditsMotion_SetAcceleration(particle, 60);
        ++particle->phase;
        break;
    case 101:
        if (particle->frames < 16 && particle->scale_x > 819) {
            particle->scale_x -= 256;
            particle->scale_y -= 256;
        }
        if (func_ov006_02077ce8(particle)) {
            particle->counter = 2 * ((particle->unknown_44 >> 4) / 4);
            ++particle->phase;
        }
        break;
    case 102:
        if (particle->counter) {
            --particle->counter;
        } else {
            CreditsMotion_TargetLayoutCell(particle, CREDITS_TRANSITION.layout, CREDITS_TRANSITION.screen);
            CreditsMotion_SetEaseOut(particle, 30);
            particle->variant = CREDITS_TRANSITION.variant;
            ++particle->phase;
        }
        break;
    case 103:
        particle->scale_x += 256;
        particle->scale_y += 256;
        if (particle->scale_x >= 4096) particle->scale_x = 4096;
        if (particle->scale_y >= 4096) particle->scale_y = 4096;
        if (func_ov006_02077ce8(particle)) {
            particle->scale_x = 4096;
            particle->scale_y = 4096;
            if (CREDITS_TRANSITION.active) --CREDITS_TRANSITION.active;
            ++particle->phase;
        }
        break;
    }
}
