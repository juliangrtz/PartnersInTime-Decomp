#include "pair_effects_internal.h"

void Overlay16PairEffect_ChangeKind(Overlay16PairEffect *effect, int kind)
{
    int previous = effect->control.bits.kind;
    if ((previous == 2 && kind == 2) || (previous == 3 && kind == 3) || (previous == 1 && kind == 1)) {
        Overlay16PairEffect_ApplyKindAnimation(effect, 0);
        effect->control.bits.phase = 2;
    } else {
        if (previous != 2 && kind == 2) {
            Overlay16PairEffect_SetAnimation(effect, 3, 0);
            effect->primary.flags.raw = (effect->primary.flags.raw & 0xff0fffff) | 0x300000;
            effect->control.bits.phase = 1;
        } else if (previous != 3 && kind == 3) {
            Overlay16PairEffect_SetAnimation(effect, 4, 0);
            effect->primary.flags.raw = (effect->primary.flags.raw & 0xff0fffff) | 0x400000;
            effect->control.bits.phase = 1;
        } else if (kind == 1) {
            Overlay16PairEffect_SetAnimation(effect, 11, 0);
            effect->primary.flags.raw = (effect->primary.flags.raw & 0xff0fffff) | 0x200000;
            effect->control.bits.phase = 1;
        }
        BattleSceneObject_SetAnimationMode01(&effect->primary, 0);
        effect->control.bits.kind = kind;
    }
}

void Overlay16PairEffect_ApplyKindAnimation(Overlay16PairEffect *effect, int active)
{
    if (active) {
        switch (effect->control.bits.kind) {
        case 1:
            Overlay16PairEffect_SetAnimation(effect, 16, 1);
            effect->primary.flags.raw = (effect->primary.flags.raw & 0xff0fffff) | 0x300000;
            break;
        case 2:
            Overlay16PairEffect_SetAnimation(effect, 5, 1);
            effect->primary.flags.raw = (effect->primary.flags.raw & 0xff0fffff) | 0x300000;
            break;
        case 3:
            Overlay16PairEffect_SetAnimation(effect, 7, 1);
            effect->primary.flags.raw = (effect->primary.flags.raw & 0xff0fffff) | 0x400000;
            break;
        }
    } else {
        switch (effect->control.bits.kind) {
        case 1:
            Overlay16PairEffect_SetAnimation(effect, 11, 1);
            effect->primary.flags.raw = (effect->primary.flags.raw & 0xff0fffff) | 0x300000;
            break;
        case 2:
            Overlay16PairEffect_SetAnimation(effect, 0, 1);
            effect->primary.flags.raw = (effect->primary.flags.raw & 0xff0fffff) | 0x300000;
            break;
        case 3:
            Overlay16PairEffect_SetAnimation(effect, 2, 1);
            effect->primary.flags.raw = (effect->primary.flags.raw & 0xff0fffff) | 0x400000;
            break;
        }
    }
    BattleSceneObject_SetAnimationMode01(&effect->primary, 1);
}

void Overlay16PairEffect_StartPursuit(Overlay16PairEffect *effect, BattleSceneObject **target)
{
    if (effect->control.bits.phase != 2)
        Overlay16PairEffect_ApplyKindAnimation(effect, 1);
    BattleMotion_StartPursuit(&effect->primary, 3, 2, *target, 26624);
    effect->control.bits.phase = 4;
}

void Overlay16PairEffect_SetAnimation(Overlay16PairEffect *effect, int animation, int argument)
{
    effect->primary.resource = (BattleSceneResource *)(gBattleContext + 56760);
    effect->secondary.resource = (BattleSceneResource *)(gBattleContext + 56808);
    effect->secondary.animation_variant_offset = 0;
    effect->primary.animation_variant_offset = effect->secondary.animation_variant_offset;
    effect->secondary.render_state = 32767;
    effect->primary.render_state = effect->secondary.render_state;
    BattleSceneObject_SetAnimation(&effect->primary, animation, 0);
    BattleSceneObject_SetAnimation(&effect->secondary, animation, 1);
    BattleSceneObject_SetAnimationMode01(&effect->primary, argument);
    BattleSceneObject_SetAnimationMode01(&effect->secondary, argument);
    func_ov002_02071938(&effect->secondary, &effect->primary, 76);
}

void Overlay16PairEffect_BeginFinish(Overlay16PairEffect *effect)
{
    Overlay16PairEffect_SetAnimation(effect, -1, 1);
    effect->control.bits.phase = 6;
}

int Overlay16PairEffect_FindFree(void)
{
    int i = 0;
    Overlay16PairControl *control = &data_ov002_020c0710->effects[0].control;
    for (i = 0; i < 32; ++i) {
        if (!control->bits.phase)
            return i;
        control = (Overlay16PairControl *)((u8 *)control + sizeof(Overlay16PairEffect));
    }
    return -1;
}

int Overlay16PairEffect_AreAllIdle(Overlay16PairEffect *effect, int count)
{
    int i;
    for (i = 0; i < count; ++i, ++effect)
        if (effect->control.bits.phase)
            return 0;
    return 1;
}

void Overlay16PairEffect_BeginKind(Overlay16PairEffect *effect, u8 kind)
{
    Overlay16PairEffect_Initialize(effect, 0, 0);
    effect->control.bits.kind = kind;
    effect->control.bits.phase = 3;
}

void Overlay16PairController_Initialize(Overlay16PairController *controller, int x, int y)
{
    BattleSceneObject *object;
    controller->primary = BattleSceneObject_GetById(40);
    controller->secondary = BattleSceneObject_GetById(42);
    controller->unknown16 = 0;
    controller->position_z_q12 = 0;
    controller->unknown24 = 0;
    controller->radius = 0;
    controller->phase = 0;
    object = controller->primary;
    BattleSceneObject_AdjustPosition(object, x - object->x, y - object->y, y + 160 - object->z);
    object = BattleSceneObject_GetById(45);
    BattleSceneObject_AdjustPosition(object, x - object->x, y - object->y, y + 308 - object->z);
    controller->position_z_q12 = controller->primary->z << 12;
}

void Overlay16PairController_Hide(Overlay16PairController *controller)
{
    BattleSceneObject_SetAnimation(controller->primary, -1, -1);
    BattleSceneObject_SetAnimation(controller->secondary, -1, -1);
}
