#include "effect_task_internal.h"

extern "C" {
#include <game/battle_damage.h>
#include <game/battle_status.h>
void Overlay25Chain_ApplyLandingDamage(Overlay25Task *task, BattleSceneObject *enemy,
                                       Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *object = BattleSceneObject_GetById((u16)parameters->parameter);
    if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
        int damage = BattleDamage_CalculateByObject(enemy->actor_id, (u16)parameters->parameter);
        BattleDamage_ApplyToParty(object, 0, 0, damage, 0, 0);
        BattleStatus_TryApply(BattleActor_GetById((u16)parameters->parameter), 2,
                              *(s16 *)(gBattleContext + 300), 5, 5);
        BattlePosition position;
        Overlay25Object_GetViewPosition(&position, object);
        BattleSpriteEffect_Spawn(815, position.x, position.y, position.z, 256);
        BattleModelEffect_SpawnAttached(&work->model_effect, 816, object, 0, 0, 0, 256);
        task->update = Overlay25Party_UnlockAfterTask;
    }
}

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
