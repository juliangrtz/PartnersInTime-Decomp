#include <game/menu_particles.h>

extern "C" {
MenuParticleSprite *Overlay5ResourceB_Get(MenuParticleTask *task);
void func_ov005_0206650c(MenuParticleTask *task);
void func_ov005_02069084(MenuParticleSprite *sprite, u8 draw_list);
int _s32_div_f(int numerator, int denominator);

/* Eight images, with a frame delay and 90% velocity retention each tick. */
void SceneMenuParticle_Update(MenuParticleTask *task)
{
    MenuParticleSprite *sprite = Overlay5ResourceB_Get(task);
    unsigned int image;
    if (task->delay) {
        --task->delay;
    } else {
        sprite->x += task->velocity_x;
        sprite->y += task->velocity_y;
        task->velocity_x = 90 * task->velocity_x / 100;
        task->velocity_y = 90 * task->velocity_y / 100;
        image = (u8)_s32_div_f(++task->frame, task->frames_per_image);
        if (image >= 8) {
            func_ov005_0206650c(task);
        } else {
            sprite->bits.tile = (int)(task->tile + 4 * image) / 2;
            func_ov005_02069084(sprite, (u8)task->draw_list);
        }
    }
}
}
