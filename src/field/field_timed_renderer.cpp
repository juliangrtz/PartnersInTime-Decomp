#include <game/field_timed_renderer.h>
#include <game/battle_scene.h>
extern "C" {
#include <game/heap.h>
extern FieldRenderObjectVTable data_ov000_020c1594;
extern FieldRenderObjectVTable data_ov000_020c14d4;

static inline FieldAnimationRenderer *InitRenderer(FieldAnimationRenderer *model)
{
    BattleModelController_Init((BattleModel *)model);
    *(FieldRenderObjectVTable **)model = &data_ov000_020c1594;
    /* Preserve the low six order bits while selecting field layer 14. */
    model->base.sort_key &= 0xF000003F;
    model->base.sort_key_bits.layer = 14;
    model->base.overlap_priority_bytes[0] = 0;
    model->base.overlap_priority_bytes[1] = 0;
    model->base.overlap_priority_bytes[2] = 0;
    model->base.overlap_priority_bytes[3] = 0;
    model->control.loops_remaining = 0;
    model->control.finished = 0;
    model->base.texture_offsets = 0;
    model->base.texture.previous = 0;
    model->base.texture.next = 0;
    return model;
}

FieldAnimationRenderer *FieldAnimationRenderer_Init(FieldAnimationRenderer *model)
{
    return InitRenderer(model);
}

FieldAnimationRenderer *FieldAnimationRenderer_InitBase(FieldAnimationRenderer *model)
{
    return InitRenderer(model);
}

void FieldAnimationRenderer_RestoreController(FieldRenderObject *model, const ModelRenderDescriptor *descriptor,
                       void *controller, s16 animation)
{
    BattleModelController_Restore((BattleModel *)model, descriptor, controller, animation);
    model->overlap_priority_bytes[0] = descriptor->flag_bits.overlap_priority;
    model->overlap_priority_bytes[1] = descriptor->flag_bits.overlap_priority;
    model->overlap_priority_bytes[2] = descriptor->flag_bits.overlap_priority;
    model->overlap_priority_bytes[3] = descriptor->flag_bits.overlap_priority;
}

u8 FieldAnimationRenderer_GetOverlapPriority(const FieldRenderObject *model, const ModelRenderSortKey *key)
{
    return model->overlap_priority_bytes[key->mode];
}

void FieldRenderList_Clear(int screen)
{
    BattleModel *model = gModelRenderList[screen];
    gModelRenderList[screen] = 0;
    gModelRenderListTail[screen] = 0;
    while (model) {
        BattleModel *next = model->render_next;
        GameHeap_DeleteArray((void *)model->texture_offsets);
        if (model)
            model->unknown_14();
        model = next;
    }
}

extern void func_020093b4(FieldRenderObject *model, int enabled);
extern void func_0200c9c8(FieldRenderObject *model);
extern int func_02009224(FieldRenderObject *model, int animation);

void FieldTimedRenderer_UpdateAnimation(FieldAnimationRenderer *model)
{
    if (model->base.state_flag_bits.animation_active &&
        model->base.state_flag_bits.behavior_state == 1 && model->control.finished)
        model->base.state_flags |= 0x200;
    func_0200c9c8(&model->base);
    if (model->base.state_flag_bits.animation_active) {
        if (model->base.state_flag_bits.behavior_state == 1) {
            if (model->base.state_flag_bits.animation_suppressed &&
                model->base.animation_id == func_02009224(&model->base, -1) - 1) {
                if (model->control.loops_remaining > 0) {
                    model->control.loops_remaining =
                        (s16)(model->control.loops_remaining - 1);
                    if (model->control.loops_remaining > 0) {
                        model->base.state_flags &= ~0x200;
                        FieldTimedRenderer_RestartAnimation(model);
                        return;
                    }
                }
                model->control.raw |= 16;
            } else {
                model->control.finished = 0;
            }
        } else {
            model->control.finished = 0;
        }
    }
}

void FieldTimedRenderer_RestartAnimation(FieldAnimationRenderer *model)
{
    int frame;
    if (model->base.animation_speed > 0) frame = 0;
    else {
        const FieldRenderAnimationRange *ranges = model->base.animation_ranges;
        u16 end = ranges[model->base.resource_animation].end;
        u16 first = ranges[model->base.resource_animation].first;
        frame = end - first;
        if (frame) --frame;
    }
    model->base.set_animation((u8)model->base.resource_animation, (s16)frame, 1);
    model->control.finished = 0;
}

void FieldTimedRenderer_SetLoopCount(FieldAnimationRenderer *model, int loop_count)
{
    model->control.loops_remaining = (s16)loop_count;
    model->control.finished = 0;
    model->base.state_flag_bits.behavior_state = 1;
}

void FieldTimedRenderer_ResetAnimationControl(FieldAnimationRenderer *model)
{
    model->control.finished = 0;
    model->control.loops_remaining = 0;
    model->base.state_flag_bits.behavior_state = 0;
    model->base.state_flag_bits.animation_suppressed = 0;
}

void FieldTimedRenderer_Update(FieldTimedRenderer *model)
{
    if (model->animation.base.state_flag_bits.animation_active) {
        if (model->state.bits.expired) func_020093b4(&model->animation.base, 0);
        else if (model->animation.base.state_flag_bits.behavior_state == 1 && model->animation.control.finished)
            func_020093b4(&model->animation.base, 0);
    }
    FieldTimedRenderer_UpdateAnimation(&model->animation);
    if (model->animation.base.state_flag_bits.animation_active && model->state.halves.remaining) {
        --model->state.halves.remaining;
        if (!model->state.halves.remaining) model->state.raw |= 0x8000;
    }
}


FieldTimedRenderer *FieldTimedRenderer_DestroyBase(FieldTimedRenderer *model)
{
    *(FieldRenderObjectVTable **)model = &data_ov000_020c14d4;
    *(FieldRenderObjectVTable **)model = &data_ov000_020c1594;
    BattleModelController_DestroyBase((BattleModel *)model);
    return model;
}

FieldAnimationRenderer *FieldAnimationRenderer_DestroyBase(FieldAnimationRenderer *model)
{
    *(FieldRenderObjectVTable **)model = &data_ov000_020c1594;
    BattleModelController_DestroyBase((BattleModel *)model);
    return model;
}
}
