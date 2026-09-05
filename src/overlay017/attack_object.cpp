#include <game/battle_object.h>
#include <game/battle_scene.h>
#include <game/overlay017_attack.h>

#include "attack_state.h"

enum Overlay17AttackResourceConstant {
    OVERLAY17_RESOURCE_BASE = 53,
    OVERLAY17_PARTY_RESOURCE_BASE = 57,
    OVERLAY17_BABY_LUIGI_RESOURCE = 59,
};

extern "C" {
void func_ov002_02071750(BattleSceneObject *object, int value);
void func_0200940c(BattleModel *model, int scale);
int func_02009224(BattleModel *model, int animation_id);
}

/* Metrowerks emits C++ functions in reverse source order. */
void Overlay17Attack_ConfigureObjectModel(
    Overlay17AttackObject *object, int animation_command,
    int model_value, int render_state,
    int synchronize_animation_layer, int flag_10) {
    BattleSceneObject *scene_object = &object->object;
    BattleModel *model;
    u16 previous_render_state;
    u16 resource_id;
    int resource_mode;

    if (object->flag_bits.enabled == 0) {
        return;
    }

    BattleSceneObject_GetActiveModel(scene_object);
    resource_mode = animation_command & 0xF00;
    switch (resource_mode) {
        case 0:
            resource_id = (u16)(((object->flags << 25) >> 29) +
                                OVERLAY17_RESOURCE_BASE);
            break;
        case 0x100:
            resource_id = (u16)((((object->flags << 25) >> 29) & 1) +
                                OVERLAY17_PARTY_RESOURCE_BASE);
            break;
        case 0x200:
            resource_id = OVERLAY17_BABY_LUIGI_RESOURCE;
            break;
    }

    if (scene_object->resource == 0 ||
        scene_object->resource->object_data_id != resource_id) {
        scene_object->resource = BattleObjectData_GetLoadState(resource_id);
        scene_object->animation_variant_offset = 0;
    }

    previous_render_state = scene_object->render_state;
    BattleSceneObject_SetAnimationFromComponent(
        scene_object, animation_command & 0xFF, 0,
        object->animation_component);
    func_ov002_02071750(scene_object, model_value);
    model = BattleSceneObject_GetActiveModel(scene_object);
    func_0200940c(model, 0x100);
    model->flags = (model->flags & ~BATTLE_MODEL_FLAG_10) |
                   ((((u8)flag_10) & 1) << BATTLE_MODEL_FLAG_10_SHIFT);
    model->flags &= ~BATTLE_MODEL_FLAG_11;

    if (render_state != -1) {
        scene_object->render_state = render_state;
    } else {
        scene_object->render_state = previous_render_state;
    }

    if (synchronize_animation_layer) {
        int phase = (object->flags << 22) >> 29;
        int divisor = func_02009224(model, -1);
        int animation_offset = phase % divisor;
        BattleModel *getter_model;
        BattleModelVTable *getter_vtable;
        int saved_animation_offset;
        int animation_id;

        /* Preserve the original virtual-call setup and live remainder. */
        asm {
            mov getter_model, model
            ldr getter_vtable, [getter_model]
            mov saved_animation_offset, animation_offset
            ldr getter_vtable, [getter_vtable, #0x3c]
        }
        animation_id =
            ((int (*)(BattleModel *))getter_vtable)(getter_model);
        model->set_primary_animation(animation_id & 0xFF,
                                     (s16)saved_animation_offset, 1);
    }
    model->flags |= BATTLE_MODEL_FLAG_09;
}

int Overlay17Attack_AreObjectStatesIdle(Overlay17AttackObject *objects,
                                        int object_count) {
    int object_index = 0;

    if (object_count > 0) {
        do {
            if (objects->flag_bits.state != 0) {
                return 0;
            }
            ++object_index;
            ++objects;
        } while (object_index < object_count);
    }
    return 1;
}
