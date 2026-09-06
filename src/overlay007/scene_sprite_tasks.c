#include <game/scene_script.h>

extern void *func_ov005_0206659c(
    int (*callback)(void *), int group, int priority);
extern SceneSpriteTaskData *Overlay5ResourceB_Attach(
    void *task, int owner, int activate);
extern void func_ov005_020695ec(SceneSpriteTaskData *sprite, int enabled);
extern void func_ov005_02069330(SceneSpriteTaskData *sprite, int slot);

void SceneSpriteTasks_Create(int width, int layout) {
    int task_index;
    int half_width;
    int x;
    int base_y;
    int target_y;
    int y;
    int owner;

    task_index = 0;
    half_width = width / 2;
    x = 0;
    base_y = layout << 6;
    target_y = (12 * layout - 10) << 12;
    y = 20;
    owner = 0;
    do {
        void *task = func_ov005_0206659c(SceneSpriteTask_Update, 6, 1);
        SceneSpriteTaskData *sprite;

        *(int *)((u8 *)task + 0x28) = task_index;
        sprite = Overlay5ResourceB_Attach(task, owner, 1);
        func_ov005_020695ec(sprite, 1);
        sprite->attributes_1 =
            (sprite->attributes_1 & ~0x3FF) |
            (((y + base_y) / 2) & 0x3FF);
        sprite->attributes_0 &= ~0x2000;
        sprite->attributes_1 &= ~0xF000;
        sprite->attributes_0 &= ~0x0300;
        sprite->attributes_1 &= ~0x0C00;
        sprite->attributes_0 &= ~0x0C00;
        func_ov005_02069330(sprite, 6);
        sprite->target_x = (x - half_width) << 12;
        sprite->target_y = target_y;

        task_index++;
        y += 8;
        x += 32;
    } while (task_index < 8);
}
