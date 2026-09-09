extern "C" {
#include <game/menu_item_effect.h>
extern s16 FX_SinCosTable_[];
extern const MenuItemStarFrames data_ov007_0208db44;
MenuItemEffectSprite *Overlay5ResourceB_Get(MenuItemEffectTask *);
void func_ov005_0206650c(MenuItemEffectTask *);
void func_ov005_020692c0(MenuItemEffectSprite *, u16, u16, int);
void func_ov005_02069084(MenuItemEffectSprite *, int);
}

extern "C" void MenuItemStar_Update(MenuItemEffectTask *task)
{
    MenuItemEffectSprite *sprite = Overlay5ResourceB_Get(task);
    MenuItemStarFrames sequence = data_ov007_0208db44;
    if (task->star.delay) {
        --task->star.delay;
        return;
    }
    int speed = task->star.velocity;
    int direction = 2 * ((task->star.angle & 0xFFFF) >> 4);
    task->star.velocity = speed + task->star.acceleration;
    sprite->x += speed * FX_SinCosTable_[direction + 1] / 256;
    sprite->y += speed * FX_SinCosTable_[direction] / 256;
    ++task->counter;
    int scale;
    /* State changes fall through so the new phase applies in the same frame. */
    switch (task->state) {
    case MENU_STAR_GROW:
        scale = task->star.scale / 4 * task->counter;
        if (scale < task->star.scale)
            break;
        ++task->state;
    case MENU_STAR_HOLD:
        scale = task->star.scale;
        if (task->counter + 4 < task->star.duration)
            break;
        ++task->state;
    case MENU_STAR_SHRINK:
        scale = task->star.scale - task->star.scale / 4 * (task->counter - (task->star.duration - 4));
        break;
    }
    if (task->counter >= task->star.duration) {
        func_ov005_0206650c(task);
        return;
    }
    if (++task->star.tick >= sequence.steps[task->star.frame][1]) {
        task->star.tick = 0;
        if (!sequence.steps[++task->star.frame][1])
            task->star.frame = 0;
    }
    sprite->attributes_1 =
        (sprite->attributes_1 & ~0x3FF) | (((4 * sequence.steps[task->star.frame][0] + 182) / 2) & 0x3FF);
    func_ov005_020692c0(sprite, scale / 16, scale / 16, (u16)task->star.rotation);
    func_ov005_02069084(sprite, 40);
}
