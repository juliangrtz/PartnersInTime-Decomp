extern "C" {
#include <game/field_timed_renderer.h>

extern void func_020093b4(FieldRenderObject *model, int enabled);
extern void func_ov000_020bd93c(FieldTimedRenderer *model);

void FieldTimedRenderer_RestartAnimation(FieldTimedRenderer *model)
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
    model->base.set_animation( (u8)model->base.resource_animation, (s16)frame, 1);
    model->control.finished = 0;
}

void FieldTimedRenderer_SetAnimationDelay(FieldTimedRenderer *model, int delay)
{
    model->control.delay = (s16)delay;
    model->control.finished = 0;
    model->base.state_flag_bits.behavior_state = 1;
}

void FieldTimedRenderer_ResetAnimationControl(FieldTimedRenderer *model)
{
    model->control.finished = 0;
    model->control.delay = 0;
    model->base.state_flag_bits.behavior_state = 0;
    model->base.state_flag_bits.animation_suppressed = 0;
}

void FieldTimedRenderer_Update(FieldTimedRenderer *model)
{
    if (model->base.state_flag_bits.animation_active) {
        if (model->state.bits.expired) func_020093b4(&model->base, 0);
        else if (model->base.state_flag_bits.behavior_state == 1 && model->control.finished)
            func_020093b4(&model->base, 0);
    }
    func_ov000_020bd93c(model);
    if (model->base.state_flag_bits.animation_active && model->state.halves.remaining) {
        --model->state.halves.remaining;
        if (!model->state.halves.remaining) model->state.raw |= 0x8000;
    }
}

}
