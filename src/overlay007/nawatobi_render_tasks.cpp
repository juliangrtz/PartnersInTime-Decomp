#include "nawatobi_render_internal.h"
#include <game/battle_scene.h>

extern "C" {
void *func_ov005_020698dc(int id);
NawatobiFollowerTask *func_ov005_0206659c(void (*)(NawatobiFollowerTask *), int, int);
BattleModel *Overlay5ResourceA_Attach(void *, BattleModel *, int);
BattleModel *Overlay5ResourceA_Get(void *);
void func_ov005_02068908(BattleModel *, u8, void *, int, int);

void NawatobiRope_UpdateAndDraw(void)
{
    NawatobiRope_Update(&data_ov007_020a6bd0, 6);
    func_ov007_0208c33c(1);
}

void NawatobiFollower_Update(NawatobiFollowerTask *task)
{
    NawatobiFollowerSource *source = task->source;
    BattleModel *model = Overlay5ResourceA_Get(task);
    int animation;
    if (source->z <= -20 * 4096) {
        animation = 0;
    } else if (source->z <= -10 * 4096) {
        animation = 1;
    } else {
        animation = 2;
    }
    int y = source->y - 2 * 4096;
    if (!task->screen)
        y -= 208 * 4096;
    model->animation_offset_x = source->x / 4096;
    model->animation_offset_y = y / 4096;
    model->set_primary_animation((u8)animation, 0, 1);
    func_ov005_02069084(model, 63);
}

void NawatobiFollower_Create(NawatobiFollowerSource *source, int screen)
{
    void *resource = func_ov005_020698dc(38);
    NawatobiFollowerTask *task = func_ov005_0206659c(NawatobiFollower_Update, 2, 1);
    BattleModel *model = Overlay5ResourceA_Attach(task, 0, 1);
    func_ov005_02068908(model, (u8)screen, resource, 0, -1);
    model->flags = (model->flags & ~3) | 1;
    task->screen = screen;
    task->source = source;
}
}
