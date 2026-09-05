#include <game/battle_impact_effect.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);
extern void func_ov002_0206bd74(BattleImpactParticlePayload *particle);
extern void func_ov002_020aa8d4(BattleImpactParticlePayload *particle);

#define DEFINE_IMPACT_PARTICLE_UPDATE(name, render_particle)                 \
    void name(BattleAITask *base_task) {                                     \
        BattleImpactParticleTask *task =                                    \
            (BattleImpactParticleTask *)base_task;                          \
        BattleImpactParticlePayload *particle = &task->data;                \
        int target_scale_q4;                                                \
        u16 target_size;                                                    \
        int remaining_frames;                                               \
        BattleImpactTrailEmitterPayload *emitter;                           \
                                                                            \
        render_particle(particle);                                          \
        remaining_frames = particle->frame;                                 \
        if (particle->frame != 0) {                                         \
            emitter = (BattleImpactTrailEmitterPayload *)                   \
                particle->controller;                                       \
            target_scale_q4 = emitter->target_scale_q4;                     \
            particle->scale_x =                                            \
                (target_scale_q4 +                                          \
                 _s32_div_f((remaining_frames - 1) *                        \
                                (16 * particle->scale_x - target_scale_q4),  \
                            particle->frame)) /                             \
                16;                                                         \
            target_scale_q4 = emitter->target_scale_q4;                     \
            particle->scale_y =                                            \
                (target_scale_q4 +                                          \
                 _s32_div_f((remaining_frames - 1) *                        \
                                (16 * particle->scale_y - target_scale_q4),  \
                            remaining_frames)) /                            \
                16;                                                         \
            target_size = emitter->setting_bits.target_size;                \
            particle->render_bits.size =                                   \
                target_size +                                              \
                _s32_div_f((remaining_frames - 1) *                         \
                               (particle->render_bits.size - target_size),   \
                           remaining_frames);                               \
            particle->z +=                                                 \
                _s32_div_f(emitter->z_velocity,                             \
                           emitter->setting_bits.spawn_interval);           \
            particle->frame--;                                             \
        }                                                                   \
        if (particle->frame == 0) {                                         \
            task->callback = 0;                                             \
        }                                                                   \
    }

DEFINE_IMPACT_PARTICLE_UPDATE(BattleImpactParticle_UpdateResourceFrame,
                              func_ov002_020aa8d4)

DEFINE_IMPACT_PARTICLE_UPDATE(BattleImpactParticle_UpdateModelFrame,
                              func_ov002_0206bd74)

#undef DEFINE_IMPACT_PARTICLE_UPDATE
