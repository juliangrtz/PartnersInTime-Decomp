/* Field script starts and entity lookup (overlay 0, 0x0208221C-0x020823F8).
 * Reset the VM banks and inherit the script context before execution starts.
 */
#include <game/field_area.h>
#include <game/field_script.h>
#include <game/field_entity.h>
void func_0202cbd4(void *, int, u32);
/* Enabled entity scripts may restart while active; other active owners reject
 * the request before changing any state. Parent reads intentionally follow the
 * resets because the parent may be this same state. */
int FieldScript_Begin(FieldScriptState *state, FieldScriptState *parent,
                       int context_type, const u16 *script)
{
    u32 parent_type;
    if (state->flag_bits.enabled &&
        (!state->flag_bits.active || state->flag_bits.owner_type == FIELD_SCRIPT_OWNER_ENTITY)) {
        /* The two local-variable banks and call frames are separate native clears. */
        func_0202cbd4(state->vm_state.variables_04, 0, 32);
        func_0202cbd4(&state->vm_state.variables_04[8], 0, 64);
        func_0202cbd4(state->vm_state.call_stack, 0, 64);
        state->vm_state.loop_stack_base = 0;
        state->vm_state.script = script;
        state->flag_bits.active = 1;
        state->flag_bits.paused = 0;
        state->flag_bits.unknown_03 = 0;
        state->flag_bits.context_type = context_type;
        state->vm_state.delay = 0;
        state->vm_state.stack_depth = 0;
        if (parent) {
            parent_type = parent->flag_bits.owner_type;
            if (parent_type == FIELD_SCRIPT_OWNER_ENTITY)
                parent_type = parent->flag_bits.parent_type;
            state->flag_bits.parent_type = parent_type;
            state->parent_entity_id = parent->flag_bits.owner_type == FIELD_SCRIPT_OWNER_ENTITY
                ? parent->owner_entity->index : -1;
            if (parent->flag_bits.owner_type == FIELD_SCRIPT_OWNER_ENTITY) {
                if (context_type == 1) {
                    ((s8 *)state->owner_data)[0] = ((s8 *)parent->owner_data)[0];
                    ((s8 *)state->owner_data)[1] = ((s8 *)parent->owner_data)[1];
                    ((s8 *)state->owner_data)[2] = ((s8 *)parent->owner_data)[2];
                } else if (context_type == 2) {
                    ((s8 *)state->owner_data)[3] = ((s8 *)parent->owner_data)[3];
                }
                state->flag_bits.owner_subtype = parent->flag_bits.owner_subtype;
            }
        } else {
            state->flag_bits.parent_type = 5;
            state->parent_entity_id = -1;
        }
        return 1;
    }
    return 0;
}

/* Selector -1 denotes the current owner; other selectors address the area table. */
FieldEntity *FieldScript_ResolveEntity(FieldVmRuntime *runtime, FieldScriptState *state, int selector)
{
    FieldEntity *entity = state->owner_entity;
    if (selector != -1)
        entity = (FieldEntity *)((FieldAreaContext *)runtime->field_context)->entities[selector];
    return entity;
}
