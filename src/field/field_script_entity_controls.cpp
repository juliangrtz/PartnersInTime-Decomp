extern "C" {
#include <nitro/fx.h>
}
#include <game/field_script_manager.h>
extern "C" {
extern void func_020093b4(FieldRenderObject *, s8);

void FieldEntity_SetAnimationWait(FieldRuntimeEntity *entity, s8 enabled)
{
    entity->base_state_flag_bits.animation_wait_enabled = enabled;
    if (entity->render_object)
        func_020093b4(entity->render_object, enabled);
}

void FieldEntity_RestoreContactFlags(FieldRuntimeEntity *entity)
{
    entity->collision_flag_bytes.current = entity->collision_flag_bytes.saved;
}
}
