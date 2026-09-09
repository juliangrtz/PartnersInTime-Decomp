#include <game/battle_particle_sweep.h>
#include <game/battle_context.h>

void func_ov002_020af5f8(BattleAITask *);

BattleAITask *BattleParticleSweep_CreateEmitter(int x, int y, int velocity, int acceleration, u16 color,
                                                u8 alpha, u16 anchor_object_id)
{
    BattleParticleSweepTask *task =
        (BattleParticleSweepTask *)BattleTaskList_Insert((BattleTaskPool *)(gBattleContext + 35652), 0);
    BattleParticleSweepParameters *parameters = &task->parameters;
    parameters->x = x;
    parameters->y = y;
    parameters->anchor_object_id = anchor_object_id;
    parameters->emission_remainder = 0;
    parameters->velocity_q8 = velocity;
    parameters->acceleration_q8 = acceleration;
    parameters->color = color;
    parameters->position_q8 = 0x3000;
    parameters->alpha = alpha;
    parameters->has_active_particles = 1;
    task->callback = func_ov002_020af5f8;
    return (BattleAITask *)task;
}
