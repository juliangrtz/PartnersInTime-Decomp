/* Sub-screen model preparation and OAM assembly, 0x0206A02C-0x0206A0F0.
 * The caller forwards the battle context in r0; this stage reads global render
 * state instead. Its callbacks may change the next model in the list.
 */
#include <game/battle_scene.h>
#include <game/battle_common_resources.h>
extern "C" {
#include <game/matrix_animation.h>
extern u8 *data_ov002_020c0660;
extern GameOamEntry data_02060740[];
void func_02009254(int, u8, u8, void *);
void func_02009598(GameOamEntry *, int, u8);
void BattleScene_RenderSubscreen(void *unused_context)
{
    for (BattleModel *model = gModelRenderList[1]; model; model = model->render_next) {
        if (model->owner)
            BattleScene_UpdateAlternateModelAnchor(model);
        model->prepare_render();
    }
    BattleScene_HideQueuedObjects();
    BattleCommonResources_PrepareOam();
    GameMatrixAnimation_Update(*(GameMatrixAnimation **)(data_ov002_020c0660 + 53784));
    func_02009254(1, data_0205a05c[1], data_0205a060[1], 0);
    func_02009598(data_02060740, 0, data_0205a060[1]);
    GameOam_SortGroups(1);
    GameOam_BuildSorted(1, data_02060740, data_02060740);
}
}
