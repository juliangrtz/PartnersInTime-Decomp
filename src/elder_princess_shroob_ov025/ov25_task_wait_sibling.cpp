/*
 * Elder Princess Shroob: chain retry and sibling wait (0x020C8AC8-0x020C8BF0).
 *
 * Retracts a countered chain before retrying, or waits for its sibling task.
 */

#include "effect_task_internal.h"

/* This phase reuses task parameters as a chain length, retry timer and
 * signed scale. Other chain phases interpret the same storage differently. */
struct ChainRetractParameters {
    s16 index, target, length, countdown, mode, scale;
    u8 unknown[20];
};
typedef char ChainRetractParameters_SizeCheck[sizeof(ChainRetractParameters) == 32 ? 1 : -1];
extern "C" void func_ov025_020c8bf0(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
extern "C" void Overlay25Chain_RetractForRetry(Overlay25Task *task, BattleSceneObject *enemy, Overlay25WorkPrefix *work)
{
    ChainRetractParameters *parameters = (ChainRetractParameters *)&task->parameters;
    Overlay25ChainState *chain = &work->chains[parameters->index];
    if (chain->unknown_31a < chain->countdown) {
        if (parameters->index == 2) parameters->length -= 152;
        else parameters->length -= 96;
        int count = ((parameters->length << 12) / parameters->scale) / chain->amplitude_q8;
        if (count < chain->countdown) --chain->countdown;
    }
    if (parameters->countdown > 0) --parameters->countdown;
    if (parameters->countdown <= 0) {
        BattleSound_Play(265, 0, 0, 0);
        BattleHitDescriptor_Configure(chain->object_id, 56, 0, enemy->actor_id, 63);
        parameters->countdown = 24;
        task->update = func_ov025_020c8bf0;
    }
}

extern "C" {
void Overlay25Task_WaitSibling(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *work)
{
    if (!work->tasks[(task->parameters.index & 1) + 5].update)
        task->update = 0;
}
}
