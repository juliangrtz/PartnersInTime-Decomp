extern "C" {
#include <game/field_timed_renderer.h>

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

}
