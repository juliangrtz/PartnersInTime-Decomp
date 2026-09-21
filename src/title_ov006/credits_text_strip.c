/* Scroll a credits text strip across the two screens, growing it on entry
 * and shrinking it before it leaves the upper edge. */
#include "credits_text_internal.h"

Overlay5ObjectSprite *Overlay5ResourceB_Get(void *);
void func_ov005_0206650c(void *);
void func_ov005_020692c0(Overlay5ObjectSprite *, fx32, fx32, int);
void *func_ov005_02069084(Overlay5ObjectSprite *, int);

void CreditsTextStrip_Update(CreditsTextStripTask *task)
{
    Overlay5ObjectSprite *sprite = Overlay5ResourceB_Get(task);
    task->scroll_y -= data_ov006_0207c594.text_scroll_speed;
    if (task->scroll_y < -16 * 4096) {
        func_ov005_0206650c(task);
        return;
    }
    sprite->y = task->scroll_y;
    if (sprite->y > 192 * 4096) {
        sprite->y -= 224 * 4096;
        if (!data_ov006_0207c594.background_variant) sprite->screen = 0;
        else sprite->screen = 1;
    } else {
        if (!data_ov006_0207c594.background_variant) sprite->screen = 1;
        else sprite->screen = 0;
    }
    if (task->scroll_y > 192 * 4096) {
        if (sprite->attributes_0_bits.mode && task->scroll_y <= 408 * 4096) {
            if (task->first_delay) --task->first_delay;
            else {
                task->first_scale += 341;
                if (task->first_scale >= 4096) task->first_scale = 4096;
            }
            if (task->second_delay) --task->second_delay;
            else {
                task->second_scale += 341;
                if (task->second_scale >= 4096) task->second_scale = 4096;
            }
            if (task->first_scale == 4096 && task->second_scale == 4096)
                sprite->attributes_0_bits.mode = 0;
        }
    } else {
        if (!sprite->attributes_0_bits.mode && task->scroll_y < 12 * data_ov006_0207c594.text_scroll_speed - 32768) {
            sprite->attributes_0_bits.mode = 1;
            task->first_scale = 4096;
        }
        if (sprite->attributes_0_bits.mode == 1) {
            task->first_scale -= 341;
            if (task->first_scale < 0) task->first_scale = 0;
            task->second_scale = task->first_scale;
        }
    }
    if (sprite->attributes_0_bits.mode) {
        int scale = data_ov006_0207c594.background_variant ? task->second_scale : task->first_scale;
        if (!scale) return;
        sprite->target_y = 12288 - 3 * scale;
        func_ov005_020692c0(sprite, 256, (u16)(scale / 16), 0);
    } else sprite->target_y = 0;
    func_ov005_02069084(sprite, 10);
}
