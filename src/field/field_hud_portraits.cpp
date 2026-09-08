extern "C" {
#include <nitro/fx.h>
#include <game/rumble.h>
#include <game/save_data.h>
}
#include <game/field_hud.h>
extern "C" {
extern void func_020093b4(FieldRenderObject *, int);
void FieldHud_SelectPortraits(void *field_context, int single)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    field->renderers[7]->animation.base.set_animation(2 * field->party->active_group, 0, 1);
    if (!single)
        field->renderers[8]->animation.base.set_animation(2 * field->party->active_group + 1, 0, 1);
}
}
