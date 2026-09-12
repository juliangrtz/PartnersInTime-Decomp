#include "nawatobi_render_internal.h"

void NawatobiRope_UpdateSubSprite(NawatobiRopeSpriteTask *task)
{
    NawatobiRopeSpriteTask *source = task->source;
    Overlay5ObjectSprite *sprite = Overlay5ResourceB_Get(task);
    Overlay5ObjectSprite *original = Overlay5ResourceB_Get(source);
    task->x = source->x;
    task->y = source->y;
    task->z = source->z;
    task->draw_list = func_ov007_0208c2f8(task->y, 1);
    sprite->x = original->x;
    sprite->y = original->y + 208 * 4096;
    func_ov005_020692c0(sprite, 256, (u16)source->scale, (u16)source->angle);
    func_ov005_02069084(sprite, (u8)task->draw_list);
}
