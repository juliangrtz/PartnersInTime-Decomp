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
void func_0202cbd4(void *, int, u32);
extern Overlay25Callback data_ov002_020c0620[];
void BattleChainRig_StartSequence(BattleSceneObject *object, int index) {
    Overlay25WorkPrefix *work = ((ChainRigOverride *)object->render_override)->state;
    work->unknown1a2e = 0;
    func_0202cbd4(work->tasks, 0, sizeof(work->tasks));
    work->tasks[0].update = data_ov002_020c0620[index];
}
}
