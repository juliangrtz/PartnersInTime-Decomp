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
