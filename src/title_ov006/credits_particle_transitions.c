#include "credits_transition_internal.h"
#include <game/random.h>
#include <nitro/fx_atan.h>

extern const s16 FX_SinCosTable_[];
extern const u8 data_ov006_0207be98[8][8];
/* X-coordinate view beginning one layout record before the current table. */
extern const s32 data_ov006_0207bfe0[][4];

/* Each effect owns the same cell array and retires its cells independently. */
static inline void ResetParticles(void (*update)(CreditsMotion *))
{
    CreditsMotion *particle = data_ov006_020847d0;
    int i;
    CREDITS_TRANSITION.active = 0;
    for (i = 0; i < 1024; ++i) {
        particle->update = update;
        particle->phase = 0;
        ++CREDITS_TRANSITION.active;
        ++particle;
    }
}

void CreditsCollapse_Update(CreditsMotion *particle)
{
    switch (particle->phase) {
    case 0:
        particle->counter = Random_NextModulo(16);
        particle->phase = 100;
        break;
    case 100:
        if (particle->counter) {
            --particle->counter;
        } else {
            particle->vx = ((Random_NextModulo(60) - 30) << 12) / 10;
            particle->vy = (Random_NextModulo(20) << 12) / 10;
            if (!CREDITS_TRANSITION.screen)
                particle->vy = -particle->vy;
            CreditsMotion_TargetLayoutCenter(particle, CREDITS_TRANSITION.layout,
                                            CREDITS_TRANSITION.screen);
            CreditsMotion_SetAcceleration(particle, Random_NextModulo(8) + 40);
            ++particle->phase;
        }
        break;
    case 101:
        particle->scale_x -= 512;
        if (particle->scale_x <= 2048) {
            particle->scale_x = 2048;
            ++particle->phase;
        }
        particle->scale_y = 8192 - particle->scale_x;
        break;
    case 102:
        if (func_ov006_02077ce8(particle))
            ++particle->phase;
        break;
    case 103:
        CreditsMotion_TargetLayoutCell(particle, CREDITS_TRANSITION.layout,
                                      CREDITS_TRANSITION.screen);
        CreditsMotion_SetEaseOut(particle, 24);
        particle->variant = CREDITS_TRANSITION.variant;
        ++particle->phase;
        break;
    case 104:
        if (particle->frames < 12) {
            particle->scale_x += 341;
            if (particle->scale_x >= 4096)
                particle->scale_x = 4096;
            particle->scale_y = 8192 - particle->scale_x;
        }
        if (func_ov006_02077ce8(particle)) {
            if (CREDITS_TRANSITION.active)
                --CREDITS_TRANSITION.active;
            ++particle->phase;
        }
        break;
    }
}

void CreditsCollapse_InitAll(void)
{
    ResetParticles(CreditsCollapse_Update);
}

void CreditsScatter_Update(CreditsMotion *particle)
{
    switch (particle->phase) {
    case 0:
        particle->counter = Random_NextModulo(24);
        particle->phase = 100;
        break;
    case 100:
        if (particle->counter) {
            --particle->counter;
        } else {
            int angle;
            int speed;
            CreditsMotion_TargetLayoutCenter(particle, CREDITS_TRANSITION.layout - 1,
                                            1 - CREDITS_TRANSITION.screen);
            angle = FX_Atan2Idx(particle->target_y - particle->y,
                               particle->target_x - particle->x);
            speed = Random_NextModulo(40) + 40;
            particle->vx = speed * FX_SinCosTable_[2 * ((angle & 0xffff) >> 4) + 1] / 10;
            particle->vy = speed * FX_SinCosTable_[2 * ((angle & 0xffff) >> 4)] / 10;
            ++particle->phase;
        }
        break;
    case 101:
        particle->x += particle->vx;
        particle->y += particle->vy;
        particle->vx = (250 * particle->vx) >> 8;
        particle->vy = (250 * particle->vy) >> 8;
        particle->scale_x -= 128;
        if (particle->scale_x < 819) {
            particle->scale_x = 819;
            ++particle->phase;
        }
        particle->scale_y = particle->scale_x;
        break;
    case 102:
        /* Launch toward the next illustration, then restore scale during flight. */
        if (!CREDITS_TRANSITION.screen)
            particle->vy = -20480;
        else
            particle->vy = 20480;
        if (data_ov006_0207bfe0[CREDITS_TRANSITION.layout][0] > 24)
            particle->vx = -32768;
        else
            particle->vx = 32768;
        CreditsMotion_TargetLayoutCell(particle, CREDITS_TRANSITION.layout,
                                      CREDITS_TRANSITION.screen);
        CreditsMotion_SetAcceleration(particle, Random_NextModulo(16) + 60);
        particle->counter = (u32)particle->frames >> 1;
        particle->unknown_40 = (4096 - particle->scale_x) / particle->counter;
        particle->counter = particle->frames - particle->counter;
        particle->variant = CREDITS_TRANSITION.variant;
        ++particle->phase;
        break;
    case 103:
        if (particle->counter) {
            --particle->counter;
        } else {
            particle->scale_x += particle->unknown_40;
            if (particle->scale_x > 4096)
                particle->scale_x = 4096;
            particle->scale_y = particle->scale_x;
        }
        if (func_ov006_02077ce8(particle)) {
            if (CREDITS_TRANSITION.active)
                --CREDITS_TRANSITION.active;
            particle->scale_x = 4096;
            particle->scale_y = particle->scale_x;
            ++particle->phase;
        }
        break;
    case 104:
        particle->scale_x += particle->unknown_40;
        if (particle->scale_x > 4096) {
            particle->scale_x = 4096;
            ++particle->phase;
        }
        particle->scale_y = particle->scale_x;
        break;
    }
}

void CreditsScatter_InitAll(void)
{
    ResetParticles(CreditsScatter_Update);
}

void CreditsPatternCollapse_Update(CreditsMotion *particle)
{
    switch (particle->phase) {
    case 0:
        /* Each delay-table entry covers a 4x4 group of cells. */
        particle->counter = data_ov006_0207be98[(u32)particle->row >> 2]
                                                [(u32)particle->column >> 2];
        particle->counter += Random_NextModulo(4);
        particle->phase = 100;
        break;
    case 100:
        if (particle->counter) {
            --particle->counter;
        } else {
            particle->vx = ((Random_NextModulo(60) - 30) << 12) / 10;
            particle->vy = (Random_NextModulo(20) << 12) / 10;
            if (!CREDITS_TRANSITION.screen)
                particle->vy = -particle->vy;
            CreditsMotion_TargetLayoutCenter(particle, CREDITS_TRANSITION.layout,
                                            CREDITS_TRANSITION.screen);
            CreditsMotion_SetAcceleration(particle, Random_NextModulo(8) + 40);
            ++particle->phase;
        }
        break;
    case 101:
        particle->scale_x -= 512;
        if (particle->scale_x <= 2048) {
            particle->scale_x = 2048;
            ++particle->phase;
        }
        particle->scale_y = 8192 - particle->scale_x;
        break;
    case 102:
        if (func_ov006_02077ce8(particle))
            ++particle->phase;
        break;
    case 103:
        CreditsMotion_TargetLayoutCell(particle, CREDITS_TRANSITION.layout,
                                      CREDITS_TRANSITION.screen);
        CreditsMotion_SetEaseOut(particle, 16);
        particle->variant = CREDITS_TRANSITION.variant;
        ++particle->phase;
        break;
    case 104:
        particle->scale_x += 256;
        if (particle->scale_x >= 4096)
            particle->scale_x = 4096;
        particle->scale_y = 8192 - particle->scale_x;
        if (func_ov006_02077ce8(particle)) {
            if (CREDITS_TRANSITION.active)
                --CREDITS_TRANSITION.active;
            ++particle->phase;
        }
        break;
    }
}

void CreditsPatternCollapse_InitAll(void)
{
    ResetParticles(CreditsPatternCollapse_Update);
}

void CreditsColumnCollapse_Update(CreditsMotion *particle)
{
    switch (particle->phase) {
    case 0:
        if (!CREDITS_TRANSITION.screen)
            particle->counter = 2 * particle->row;
        else
            particle->counter = 2 * (32 - particle->row);
        CreditsMotion_TargetLayoutCell(particle, CREDITS_TRANSITION.layout,
                                      CREDITS_TRANSITION.screen);
        if (particle->x > particle->target_x)
            particle->counter += (u32)particle->column >> 1;
        else
            particle->counter += (32 - particle->column) / 2;
        particle->phase = 100;
        break;
    case 100:
        if (particle->counter) {
            --particle->counter;
        } else {
            particle->vx = ((Random_NextModulo(60) - 30) << 12) / 10;
            particle->vy = (Random_NextModulo(20) << 12) / 10;
            if (!CREDITS_TRANSITION.screen)
                particle->vy = -particle->vy;
            CreditsMotion_TargetLayoutCell(particle, CREDITS_TRANSITION.layout,
                                          CREDITS_TRANSITION.screen);
            /* Collapse into the leftmost cell column before spreading out. */
            particle->target_x -= (5 * particle->column) << 12;
            CreditsMotion_SetAcceleration(particle, Random_NextModulo(8) + 40);
            ++particle->phase;
        }
        break;
    case 101:
        particle->scale_x -= 512;
        if (particle->scale_x <= 2048) {
            particle->scale_x = 2048;
            ++particle->phase;
        }
        particle->scale_y = 8192 - particle->scale_x;
        break;
    case 102:
        if (func_ov006_02077ce8(particle))
            ++particle->phase;
        break;
    case 103:
        CreditsMotion_TargetLayoutCell(particle, CREDITS_TRANSITION.layout,
                                      CREDITS_TRANSITION.screen);
        CreditsMotion_SetEaseOut(particle, 16);
        particle->variant = CREDITS_TRANSITION.variant;
        ++particle->phase;
        break;
    case 104:
        particle->scale_x += 256;
        if (particle->scale_x >= 4096)
            particle->scale_x = 4096;
        particle->scale_y = 8192 - particle->scale_x;
        if (func_ov006_02077ce8(particle)) {
            if (CREDITS_TRANSITION.active)
                --CREDITS_TRANSITION.active;
            ++particle->phase;
        }
        break;
    }
}

void CreditsColumnCollapse_InitAll(void)
{
    ResetParticles(CreditsColumnCollapse_Update);
}

void CreditsSpiralCollapse_Update(CreditsMotion *particle)
{
    switch (particle->phase) {
    /* This mode uses words 0x40/44/48 as angle, Q4 radius and angle step. */
    case 0:
        particle->unknown_4c = particle->unknown_44;
        particle->ax = particle->unknown_44 / 40;
        if (particle->ax < 2)
            particle->ax = 2;
        particle->phase = 100;
        break;
    case 100:
        if (particle->counter) {
            --particle->counter;
        } else {
            particle->unknown_40 += particle->unknown_48;
            particle->unknown_44 -= particle->ax;
            if (particle->scale_x > 819) {
                particle->scale_x -= 81;
                particle->scale_y -= 81;
            }
            if (particle->unknown_44 < 8) {
                particle->unknown_44 = 0;
                ++particle->phase;
            }
            particle->x = particle->vx + ((particle->unknown_44 *
                FX_SinCosTable_[2 * ((particle->unknown_40 & 0xffff) >> 4) + 1]) >> 4);
            particle->y = particle->vy + ((particle->unknown_44 *
                FX_SinCosTable_[2 * ((particle->unknown_40 & 0xffff) >> 4)]) >> 4);
        }
        break;
    case 101:
        CreditsMotion_TargetLayoutCenter(particle, CREDITS_TRANSITION.layout, CREDITS_TRANSITION.screen);
        particle->vx = ((Random_NextModulo(60) - 30) << 12) / 10;
        particle->vy = ((Random_NextModulo(20) + 10) << 12) / 10;
        if (!CREDITS_TRANSITION.screen)
            particle->vy = -particle->vy;
        CreditsMotion_SetAcceleration(particle, 24);
        ++particle->phase;
        break;
    case 102:
        if (func_ov006_02077ce8(particle))
            ++particle->phase;
        break;
    case 103:
        CreditsMotion_TargetLayoutCell(particle, CREDITS_TRANSITION.layout, CREDITS_TRANSITION.screen);
        CreditsMotion_SetEaseOut(particle, 24);
        particle->variant = CREDITS_TRANSITION.variant;
        ++particle->phase;
        break;
    case 104:
        particle->scale_x += 256;
        particle->scale_y += 256;
        if (particle->scale_x >= 4096)
            particle->scale_x = 4096;
        if (particle->scale_y >= 4096)
            particle->scale_y = 4096;
        if (func_ov006_02077ce8(particle)) {
            particle->scale_x = 4096;
            particle->scale_y = 4096;
            if (CREDITS_TRANSITION.active) --CREDITS_TRANSITION.active;
            ++particle->phase;
        }
        break;
    }
}
