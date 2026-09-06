#include <game/scene_script.h>

typedef struct SceneRenderablePosition {
    u8 unknown_00[0x5C];
    s16 x;
    s16 y;
} SceneRenderablePosition;

typedef struct SceneManagerWork {
    u8 unknown_000[0x25C];
    SceneObject *first_object;
    u8 unknown_260[0x0DC];
    SceneObject *render_rows[192];
} SceneManagerWork;

typedef struct SceneManager {
    u8 unknown_0000[0x3000];
    SceneManagerWork work;
} SceneManager;

extern void func_0203b76c(u16 value, void *destination, u32 size);
extern void func_ov005_02069084(void *renderable, int selector);
extern void func_ov007_02086d60(void);
extern void func_ov007_02089398(SceneObject *object);

void SceneObjects_UpdateAndDraw(u8 *manager) {
    SceneManager *scene = (SceneManager *)manager;
    SceneObject *object;
    int object_id;
    int render_x;
    int render_y;
    int row;
    int draw_row;
    volatile u16 clear_value;

    func_ov007_02086d60();
    clear_value = 0;
    func_0203b76c(
        clear_value, scene->work.render_rows,
        192 * sizeof(SceneObject *)
    );
    object = scene->work.first_object;
    for (object_id = 0; object_id < 56; object_id++) {
        if (object->secondary_renderable != 0 ||
            object->primary_renderable != 0) {
            int object_y = object->y;

            render_y = (s16)(object_y - object->base_y);
            row = (s16)((s16)(
                object->render_height + 16 * (192 - object_y)
            ) / 16);
            if (row < 0) {
                row = 0;
            }
            if (row >= 192) {
                row = 191;
            }
            render_x = object->x;
            if (object->primary_renderable != 0) {
                SceneRenderablePosition *renderable =
                    (SceneRenderablePosition *)object->primary_renderable;
                renderable->x = render_x + object->render_offset_x;
                renderable->y = render_y + object->render_offset_y;
            }
            if (object->secondary_renderable != 0) {
                SceneRenderablePosition *renderable =
                    (SceneRenderablePosition *)object->secondary_renderable;
                renderable->x = render_x + object->render_offset_x;
                renderable->y = render_y + object->render_offset_y;
            }
            object->render_next = scene->work.render_rows[row];
            scene->work.render_rows[row] = object;
            func_ov007_02089398(object);
        }
        object++;
    }

    for (draw_row = 0; draw_row < 192; draw_row++) {
        for (object = scene->work.render_rows[draw_row];
             object != 0;
             object = object->render_next) {
            if (object->primary_renderable != 0) {
                func_ov005_02069084(object->primary_renderable, 5);
            }
            if (object->secondary_renderable != 0) {
                func_ov005_02069084(object->secondary_renderable, 5);
            }
        }
    }
}
