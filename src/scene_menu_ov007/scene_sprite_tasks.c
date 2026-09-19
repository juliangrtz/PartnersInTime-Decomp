/*
 * Scene sprite tasks (overlay 7, 0x02084FBC-0x02085168).
 *
 * Creates and steps the tasks that own the scene's sprites.
 */

#include <game/scene_script.h>

extern u8 *data_ov007_020a6b90;

extern int func_ov005_0206650c(void *task);
extern SceneSpriteTaskData *Overlay5ResourceB_Get(void *task);
extern int func_ov005_02069084(SceneSpriteTaskData *sprite, int selector);

int SceneSpriteTask_Update(void *task) {
    SceneSpriteTaskData *sprite = Overlay5ResourceB_Get(task);
    SceneObject *object = (SceneObject *)SceneObject_GetById(16);

    if (*(u16 *)(data_ov007_020a6b90 + 0xA7EA) == 0) {
        return func_ov005_0206650c(task);
    }
    if (object->secondary_renderable != 0) {
        sprite->x =
            *(s16 *)((u8 *)object->secondary_renderable + 0x5C) << 12;
        sprite->y =
            *(s16 *)((u8 *)object->secondary_renderable + 0x5E) << 12;
        return func_ov005_02069084(sprite, 4);
    }
    return (int)object;
}

#include <game/overlay005_resource.h>

extern void *func_ov005_0206659c(
    int (*callback)(void *), int group, int priority);
extern SceneSpriteTaskData *Overlay5ResourceB_Attach(
    void *task, int owner, int activate);
extern void func_ov005_02069330(SceneSpriteTaskData *sprite, int slot);

void SceneSpriteTasks_Create(int width, int layout) {
    int task_index;
    int y;
    int x;
    int base_y;
    int half_width;
    int target_y;
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
        Overlay5ObjectSprite_Init((Overlay5ObjectSprite *)sprite, 1);
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

        y += 8;
        x += 32;
        task_index++;
    } while (task_index < 8);
}
