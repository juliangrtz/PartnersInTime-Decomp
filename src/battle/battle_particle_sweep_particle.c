#include <game/battle_particle_sweep.h>
#include <game/battle_context.h>

void func_ov002_020af154(BattleAITask *);

BattleAITask *BattleParticleSweep_CreateParticle(BattleParticleSweepParameters *emitter, u16 angle, int size,
                                                 u8 red, u8 green, u8 blue)
{
    BattleSweepParticleTask *task =
        (BattleSweepParticleTask *)BattleTaskList_Insert((BattleTaskPool *)(gBattleContext + 37980), 0);
    BattleSweepParticleParameters *parameters = &task->parameters;
    parameters->emitter = emitter;
    parameters->age = 0;
    parameters->angle = angle;
    parameters->red = red;
    parameters->green = green;
    parameters->blue = blue;
    parameters->size = size;
    task->callback = func_ov002_020af154;
    return (BattleAITask *)task;
}
