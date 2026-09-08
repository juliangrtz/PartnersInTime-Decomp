extern "C" {
#include <nitro/fx.h>
}
#include <game/field_script_context.h>
extern "C" {
extern int func_ov000_02082240(FieldScriptState *, FieldScriptState *, int, const u16 *);

void FieldScriptManager_StartAuxiliary(FieldScriptManager *manager, FieldScriptState *parent, int slot)
{
    if (!manager->states[3].flag_bits.active) {
        FieldScriptContext *field = (FieldScriptContext *)manager->runtimes[0].field_context;
        func_ov000_02082240(&manager->states[3], parent, 0, FieldScript_Lookup(field, 0, slot));
    }
}
}
