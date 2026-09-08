extern "C" {
#include <nitro/fx.h>
#include <game/rumble.h>
#include <game/save_data.h>
}
#include <game/field_hud.h>
extern "C" {
extern void func_020093b4(FieldRenderObject *, int);
int FieldEffect_FindSlot(void *field_context, FieldRuntimeEntity *entity)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    int i;
    for (i = 0; i < 8; ++i) {
        int owner = field->effect_owners[i];
        if (owner != 255 && entity->base.index == (owner & ~128))
            return i;
    }
    return -1;
}
}
