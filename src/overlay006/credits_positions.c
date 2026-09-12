#include "credits_transition_internal.h"

/* Credits use a 224-pixel origin offset for the second screen. */
void CreditsMotion_TargetLayoutOrigin(CreditsMotion *particle, int layout, int screen)
{
    particle->target_x = data_ov006_0207bff0[layout][0] << 12;
    particle->target_y = data_ov006_0207bff4[layout][0] << 12;
    if (!screen)
        particle->target_y += 224 << 12;
}

void CreditsMotion_TargetLayoutCell(CreditsMotion *particle, int layout, int screen)
{
    particle->target_x = data_ov006_0207bff0[layout][0] << 12;
    particle->target_y = data_ov006_0207bff4[layout][0] << 12;
    particle->target_x += (5 * particle->column + 2) << 12;
    particle->target_y += (4 * particle->row + 2) << 12;
    if (!screen)
        particle->target_y += 224 << 12;
}

void CreditsMotion_TargetLayoutCenter(CreditsMotion *particle, int layout, int screen)
{
    particle->target_x = (data_ov006_0207bff0[layout][0] + 80) << 12;
    particle->target_y = (data_ov006_0207bff4[layout][0] + 64) << 12;
    if (!screen)
        particle->target_y += 224 << 12;
}
