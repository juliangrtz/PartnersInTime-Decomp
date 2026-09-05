#include <game/battle_scene.h>
#include <game/overlay018_attack.h>

#include "attack_state.h"

enum Overlay18AttackObjectConstant {
    OVERLAY18_FIRST_VARIANT_OBJECT_ID = 46,
    OVERLAY18_OBJECT_FLAG_MODE_MASK = 0xF,
    OVERLAY18_OBJECT_FLAG_VARIANT_MASK = 0x30,
    OVERLAY18_OBJECT_RUNTIME_FLAGS_MASK = 0x7C0,
};

extern void func_ov010_020c1e1c(BattleSceneObject *object);
extern void func_ov018_020c2e50(
    Overlay18AttackObjectController *controller, int state);

/* Metrowerks emits C functions in reverse source order. */
void Overlay18Attack_InitializeObjectController(
    Overlay18AttackObjectController *controller, int variant,
    Overlay18AttackObjectSource *source) {
    u16 mode;

    controller->object = source->object;
    controller->variant_object = BattleSceneObject_GetById(
        (u16)(variant + OVERLAY18_FIRST_VARIANT_OBJECT_ID));
    controller->resource_index =
        source->object->resource->flags.bits.resource_index - 1;
    controller->counter = 0;

    mode = source->mode;
    if (mode >= 4) {
        controller->flag_bits.mode = mode & 1;
    } else {
        controller->flags =
            (controller->flags & ~OVERLAY18_OBJECT_FLAG_MODE_MASK) |
            (mode & OVERLAY18_OBJECT_FLAG_MODE_MASK);
    }
    controller->flags =
        (controller->flags & ~OVERLAY18_OBJECT_FLAG_VARIANT_MASK) |
        ((variant & 3) << 4);
    controller->flags &= ~OVERLAY18_OBJECT_RUNTIME_FLAGS_MASK;
    controller->source = source;
    controller->object->effect_anchor_z = -8 * (variant + 1);
}

void Overlay18Attack_ClearObjectAnchor(
    Overlay18AttackObjectController *controller) {
    controller->object->effect_anchor_z = 0;
}

void Overlay18Attack_ResetObjectController(
    Overlay18AttackObjectController *controller) {
    func_ov010_020c1e1c(controller->object);
    func_ov018_020c2e50(controller, -1);
    controller->flags &= ~OVERLAY18_OBJECT_RUNTIME_FLAGS_MASK;
}
