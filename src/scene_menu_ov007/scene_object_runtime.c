#include <game/scene_motion.h>

typedef struct SceneManagerWork {
    u8 unknown_000[0x25C];
    SceneObject *first_object;
    u8 unknown_260[0x0DC];
    SceneObject *render_rows[192];
    void *task_slots[72];
} SceneManagerWork;

typedef struct SceneManager {
    u8 unknown_0000[0x3000];
    SceneManagerWork work;
} SceneManager;

extern u8 *data_ov007_020a6b90;

extern void func_0202cbd4(void *destination, u32 value, u32 size);
extern void func_ov005_02068c54(void *renderable);
extern void func_ov005_020698f0(int slot);
extern void SceneModel_Stop(void *renderable);
extern SceneScriptState *SceneScript_GetObjectState(u32 object_id);

/* Metrowerks emits C functions in reverse source order. */
void SceneObjects_ClearPrimaryRange(u8 *manager) {
    SceneObject *object;
    int object_id;
    int slot;
    SceneManager *scene = (SceneManager *)manager;

    SceneScript_Stop(SCENE_SCRIPT_OWNER_PRIMARY);
    for (object_id = 28; object_id < 40; object_id++) {
        object = (SceneObject *)SceneObject_GetById(object_id);

        SceneObject_UnlinkMotion(object);
        if (object->primary_renderable != 0) {
            SceneModel_Stop(object->primary_renderable);
            if (object->primary_renderable != 0) {
                func_ov005_02068c54(object->primary_renderable);
                object->primary_renderable = 0;
            }
        }
        if (object->secondary_renderable != 0) {
            SceneModel_Stop(object->secondary_renderable);
            if (object->secondary_renderable != 0) {
                func_ov005_02068c54(object->secondary_renderable);
                object->secondary_renderable = 0;
            }
        }
        func_0202cbd4(object, 0, sizeof(SceneObject));
        object->object_id = object_id;
    }
    for (object_id = 28; object_id < 40; object_id++) {
        SceneScript_GetObjectState(object_id)->vm_state.script = 0;
    }
    for (slot = 36; slot < 52; slot++) {
        scene->work.task_slots[slot] = 0;
        func_ov005_020698f0(slot);
    }
}

void SceneScripts_UpdateGlobal(u8 *manager) {
    if (*(const u16 **)(manager + 0x386C) != 0) {
        VM_Run(
            (ScriptVm *)(manager + 0x375C),
            (ScriptVmState *)(manager + 0x386C)
        );
    }
    if (*(const u16 **)(manager + 0x3924) != 0) {
        VM_Run(
            (ScriptVm *)(manager + 0x375C),
            (ScriptVmState *)(manager + 0x3924)
        );
    }
}

typedef struct SceneRenderablePosition {
    u8 unknown_00[0x5C];
    s16 x;
    s16 y;
} SceneRenderablePosition;

extern void MIi_CpuClear16(u16 value, void *destination, u32 size);
extern void func_ov005_02069084(void *renderable, int selector);
extern void SceneObject_UpdateAnimationLimit(SceneObject *object);

void SceneObjects_UpdateAndDraw(u8 *manager) {
    SceneManager *scene = (SceneManager *)manager;
    SceneObject *object;
    int object_id;
    s16 render_x;
    int render_y;
    int row;
    int min_row;
    int draw_row;
    volatile u16 clear_value;

    SceneMotion_UpdateAll();
    clear_value = 0;
    MIi_CpuClear16(
        clear_value, scene->work.render_rows,
        192 * sizeof(SceneObject *)
    );
    object = scene->work.first_object;
    /* Keep the counter initialization after the initial object-pointer load. */
    asm {
        mov object_id, #0
    }
    min_row = object_id;
    do {
        if (object->secondary_renderable != 0 ||
            object->primary_renderable != 0) {
            /*
             * row = (s16)(height + 16 * (192 - y)) / 16;
             * render_y = (s16)(y - base_y).
             * Clamp negative rows to zero before the C upper-bound check.
             * Keep the original paired narrowing and signed-division schedule.
             * Only r0-r3 are scratch; object remains in a callee-saved register.
             */
            asm {
                ldrsh r2, [object, #6]
                ldrsh r1, [object, #0xd8]
                ldrsh r3, [object, #8]
                rsb r0, r2, #192
                add r0, r1, r0, lsl #4
                mov r0, r0, lsl #16
                mov r1, r0, asr #16
                mov r0, r1, asr #3
                add r0, r1, r0, lsr #28
                sub r2, r2, r3
                mov r1, r0, asr #4
                mov r0, r2, lsl #16
                mov r2, r1, lsl #16
                mov render_y, r0, asr #16
                movs row, r2, asr #16
                movmi row, min_row
            }
            if (row >= 192) {
                row = 191;
            }
            render_x = object->x;
            if (object->primary_renderable != 0) {
                SceneRenderablePosition *renderable =
                    (SceneRenderablePosition *)object->primary_renderable;
                int offset_x = object->render_offset_x;
                int offset_y = object->render_offset_y;
                renderable->x = render_x + offset_x;
                renderable->y = render_y + offset_y;
            }
            if (object->secondary_renderable != 0) {
                SceneRenderablePosition *renderable =
                    (SceneRenderablePosition *)object->secondary_renderable;
                int offset_y = object->render_offset_y;
                int offset_x = object->render_offset_x;
                renderable->x = render_x + offset_x;
                renderable->y = render_y + offset_y;
            }
            object->render_next = scene->work.render_rows[row];
            scene->work.render_rows[row] = object;
            SceneObject_UpdateAnimationLimit(object);
        }
        object_id++;
        object++;
    } while (object_id < 56);

    draw_row = 0;
    do {
        object = scene->work.render_rows[draw_row];
        if (object != 0) {
            do {
                if (object->primary_renderable != 0) {
                    func_ov005_02069084(object->primary_renderable, 5);
                }
                if (object->secondary_renderable != 0) {
                    func_ov005_02069084(object->secondary_renderable, 5);
                }
                object = object->render_next;
            } while (object != 0);
        }
        draw_row++;
    } while (draw_row < 192);
}
