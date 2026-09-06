#include <game/scene_script.h>

typedef struct SceneTransitionTask {
    u8 unknown_00[0x20];
    u32 state;
    u32 unknown_24;
    u32 scene_id;
} SceneTransitionTask;

typedef struct SceneRenderable {
    u8 unknown_00[0x7C];
    u32 flags;
} SceneRenderable;

extern u8 data_ov005_0206a180[];
extern u8 *data_ov007_020a6b90;

extern int func_ov005_02067288(void *archive_manager);
extern int func_ov005_020698dc(int resource_id);
extern void *func_ov005_02068c80(void);
extern void func_ov005_02068c54(void *renderable);
extern void func_ov005_02068908(
    void *renderable, int animation_id, int resource, int enabled,
    int frame);
extern void SceneScript_LoadSecondaryArchive(u8 *manager, u8 scene_id);
extern void func_ov007_02088c50(
    SceneObject *object, int variant, int enabled);
extern void func_ov007_02086d08(
    SceneObject *object, int x, int y, int z);
extern void func_ov007_0208701c(SceneObject *object);
extern void func_ov007_020894c4(void *renderable);

static inline void SceneTransition_InitializeObject(
    SceneObject *object, int variant, int resource) {
    object->resource_id = 20;
    object->animation_id = 0;
    if (object->secondary_renderable != 0) {
        return;
    }

    object->flags.raw = (object->flags.raw & ~1) | 1;
    object->resource = *(void **)(
        data_ov007_020a6b90 + 0x363C + object->resource_id * 4);
    object->secondary_renderable = func_ov005_02068c80();
    ((SceneRenderable *)object->secondary_renderable)->flags &= ~0x100;
    ((SceneRenderable *)object->secondary_renderable)->flags &= ~0x200;
    ((SceneRenderable *)object->secondary_renderable)->flags &= ~4;
    func_ov005_02068908(
        object->secondary_renderable, 1, resource, 1, -1);
    ((SceneRenderable *)object->secondary_renderable)->flags =
        (((SceneRenderable *)object->secondary_renderable)->flags & ~3) | 1;
    func_ov007_02088c50(object, variant, 1);
}

static inline void SceneTransition_DestroySecondaryObject(
    SceneObject *object) {
    if (object->secondary_renderable != 0) {
        func_ov007_020894c4(object->secondary_renderable);
        if (object->secondary_renderable != 0) {
            func_ov005_02068c54(object->secondary_renderable);
            object->secondary_renderable = 0;
        }
    }
    func_ov007_0208701c(object);
}

int SceneTransitionController_Update(SceneTransitionTask *task) {
    SceneObject *object;
    int resource;
    int result;

    result = task->state;
    switch (result) {
    case 0:
    case 3:
        break;

    case 1:
        result = func_ov005_02067288(
            *(void **)(data_ov005_0206a180 + 0x2C));
        if (result) {
            SceneObjects_ClearSecondaryRange(data_ov007_020a6b90);
            SceneScript_LoadSecondaryArchive(
                data_ov007_020a6b90, (u8)task->scene_id);
            result = 2;
            task->state = result;
        }
        break;

    case 2:
        result = func_ov005_02067288(
            *(void **)(data_ov005_0206a180 + 0x2C));
        if (result) {
            resource = func_ov005_020698dc(20);

            object = (SceneObject *)SceneObject_GetById(16);
            SceneTransition_InitializeObject(object, 0, resource);
            {
                int x = -object->x;
                int y = -24 - object->y;
                int z = -object->base_y;

                func_ov007_02086d08(object, x, y, z);
            }

            object = (SceneObject *)SceneObject_GetById(17);
            SceneTransition_InitializeObject(object, 1, resource);
            {
                int x = 128 - object->x;
                int y = -16 - object->y;
                int z = -136 - object->base_y;

                func_ov007_02086d08(object, x, y, z);
            }

            SceneScript_StartSecondary(data_ov007_020a6b90);
            result = 3;
            task->state = result;
        }
        break;

    case 4:
        result = func_ov005_02067288(
            *(void **)(data_ov005_0206a180 + 0x2C));
        if (result) {
            SceneObjects_ClearSecondaryRange(data_ov007_020a6b90);
            *(u16 *)(data_ov007_020a6b90 + 0xA7EA) = 0;

            object = (SceneObject *)SceneObject_GetById(16);
            SceneTransition_DestroySecondaryObject(object);
            func_ov007_02086d08(
                object, -128 - object->x, -object->y, -object->base_y);

            object = (SceneObject *)SceneObject_GetById(17);
            SceneTransition_DestroySecondaryObject(object);
            func_ov007_02086d08(
                object, -128 - object->x, -object->y, -object->base_y);

            result = 0;
            task->state = result;
        }
        break;

    default:
        break;
    }
    return result;
}
