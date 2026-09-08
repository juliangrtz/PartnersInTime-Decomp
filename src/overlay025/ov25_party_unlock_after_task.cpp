#include "effect_task_internal.h"

extern "C" {
void Overlay25Party_UnlockAfterTask(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    if (!work->tasks[5].update) {
        BattlePartyActor *actor = (BattlePartyActor *)BattleActor_GetPartySlot((u16)parameters->parameter);
        actor->state_flags.raw &= ~0x1000;
        task->update = 0;
    }
}
}
