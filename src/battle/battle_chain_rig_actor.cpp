#include <game/overlay025_effect_task.h>
#include <game/battle_object_link.h>
#include <game/battle_damage.h>
#include <game/battle_context.h>
struct ChainRigOverride {
    BattleObjectLinkDraw draw;
    Overlay25WorkPrefix *state;
};
#include <game/battle_effect_controls.h>
extern "C" {
void BattleChainRig_SetActor(BattleSceneObject *object, int index, u16 actor_id) {
    Overlay25ChainState *chain =
        &((ChainRigOverride *)object->render_override)->state->chains[index];
    chain->unknown_300 = actor_id;
    chain->countdown =
        BattleActor_CanReceiveStatus(BattleActor_GetById(actor_id)) ? chain->unknown_31a : 0;
}
}
