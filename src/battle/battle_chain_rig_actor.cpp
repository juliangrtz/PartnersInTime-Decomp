/*
 * Chain rig actor and activity (overlay 2, 0x020B68E0-0x020B6A08).
 *
 * Binds a chain rig to its actor and checks completion of its tasks.
 */

#include <game/overlay025_effect_task.h>
#include <game/battle_object_link.h>
extern "C" {
#include <game/battle_damage.h>
}
#include <game/battle_context.h>
#include <game/battle_hit.h>
struct ChainRigOverride {
    BattleObjectLinkDraw draw;
    Overlay25WorkPrefix *state;
};
#include <game/battle_effect_controls.h>
extern "C" {
int BattleChainRig_IsActive(BattleSceneObject *object)
{
    Overlay25WorkPrefix *work = ((ChainRigOverride *)object->render_override)->state;
    if (work->unknown1a2e) {
        /* The hit queue ends at a record with kind zero. Reflection can change
         * its source, so test the second adult again after the first call. */
        for (int i = 0; ; ++i) {
            BattleHitRecord *hit = &((BattleHitRecord *)(gBattleContext + 51928))[i];
            if (!hit->kind) break;
            if (hit->source_id == 56 && hit->target_id) {
                BattleDamage_ReflectQueuedHits(hit->target_id);
            }
            if (hit->source_id == 57 && hit->target_id) {
                BattleDamage_ReflectQueuedHits(hit->target_id);
            }
        }
    }
    for (int i = 0; i < 8; ++i) {
        if (work->tasks[i].update) return 1;
    }
    work->unknown1a2e = 0;
    return 0;
}

void BattleChainRig_SetActor(BattleSceneObject *object, int index, u16 actor_id) {
    Overlay25ChainState *chain =
        &((ChainRigOverride *)object->render_override)->state->chains[index];
    chain->unknown_300 = actor_id;
    chain->countdown =
        BattleActor_CanReceiveStatus(BattleActor_GetById(actor_id)) ? chain->unknown_31a : 0;
}
}
