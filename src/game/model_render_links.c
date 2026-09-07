#include <game/battle_scene.h>

void BattleModel_AppendRenderList(BattleModel *model)
{
    if (!model->flag_bits.render_linked) {
        if (gModelRenderList[model->screen] == 0) {
            gModelRenderListTail[model->screen] = model;
            gModelRenderList[model->screen] = gModelRenderListTail[model->screen];
            model->render_previous = 0;
            model->render_next = 0;
        } else {
            gModelRenderListTail[model->screen]->render_next = model;
            model->render_previous = gModelRenderListTail[model->screen];
            model->render_next = 0;
            gModelRenderListTail[model->screen] = model;
        }
        model->flag_bits.render_linked = 1;
    }
}

void BattleModel_UnlinkRenderList(BattleModel *model)
{
    if (model->flag_bits.render_linked) {
        if (model->render_previous != 0)
            model->render_previous->render_next = model->render_next;
        if (gModelRenderList[model->screen] == model)
            gModelRenderList[model->screen] = model->render_next;
        if (model->render_next != 0)
            model->render_next->render_previous = model->render_previous;
        if (gModelRenderListTail[model->screen] == model)
            gModelRenderListTail[model->screen] = model->render_previous;
        model->render_next = 0;
        model->render_previous = model->render_next;
        model->flag_bits.render_linked = 0;
    }
}

/* Detach without clearing the node's saved neighbours. */
void BattleModel_DetachRenderList(BattleModel *model)
{
    if (model->flag_bits.render_linked) {
        if (model->render_previous != 0)
            model->render_previous->render_next = model->render_next;
        if (gModelRenderList[model->screen] == model)
            gModelRenderList[model->screen] = model->render_next;
        if (model->render_next != 0)
            model->render_next->render_previous = model->render_previous;
        if (gModelRenderListTail[model->screen] == model)
            gModelRenderListTail[model->screen] = model->render_previous;
        model->flag_bits.render_linked = 0;
    }
}

void BattleModel_RestoreRenderList(BattleModel *model)
{
    if (!model->flag_bits.render_linked) {
        if (model->render_previous) model->render_previous->render_next = model;
        else gModelRenderList[model->screen] = model;
        if (model->render_next) model->render_next->render_previous = model;
        else gModelRenderListTail[model->screen] = model;
        model->flag_bits.render_linked = 1;
    }
}

int BattleModel_ReleaseTexture(BattleModel *model, int force)
{
    if (force) {
        GameSpriteAllocation_Unlink(&model->texture);
        return 1;
    } else {
        if (model->flag_bits.texture_allocation_mode != 3) return 0;
        GameSpriteAllocation_Unlink(&model->texture);
        return 1;
    }
}
