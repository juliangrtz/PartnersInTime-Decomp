#include <game/field_blink.h>
extern void func_020093b4(FieldRenderObject *, int);

void FieldEntity_CopySortKeysToAnchors(FieldRuntimeEntity *entity)
{
    FieldRenderObject *model = entity->render_object;
    if (model) {
        model->render_anchor_z = model->sort_key;
        model = entity->auxiliary_render_object;
        if (model) model->render_anchor_z = model->sort_key;
    }
}

void FieldBlink_StartRenderers(FieldRuntimeEntity *entity, int mode, const s8 *durations, u8 length,
                       FieldEntityVisibilityCallback show, FieldEntityVisibilityCallback hide)
{
    if (!show) show = FieldBlink_ShowRenderers;
    if (!hide) hide = FieldBlink_HideRenderers;
    FieldBlink_Start(entity, mode, durations, length, show, hide);
}

void FieldBlink_ShowRenderers(FieldEntity *base)
{
    FieldRuntimeEntity *entity = (FieldRuntimeEntity *)base;
    func_020093b4(entity->render_object, 1);
    if (entity->auxiliary_render_object && entity->field_state_flag_bits.shadow_enabled &&
        (entity->relative_height > 0 || entity->field_state_flag_bits.unknown_16))
        func_020093b4(entity->auxiliary_render_object, 1);
}

void FieldBlink_HideRenderers(FieldEntity *base)
{
    FieldRuntimeEntity *entity = (FieldRuntimeEntity *)base;
    func_020093b4(entity->render_object, 0);
    if (entity->auxiliary_render_object && entity->field_state_flag_bits.shadow_enabled &&
        (entity->relative_height > 0 || entity->field_state_flag_bits.unknown_16))
        func_020093b4(entity->auxiliary_render_object, 0);
}
