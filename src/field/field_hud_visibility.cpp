extern "C" {
#include <nitro/fx.h>
#include <game/rumble.h>
#include <game/save_data.h>
}
#include <game/field_hud.h>
extern "C" {
extern void func_020093b4(FieldRenderObject *, int);
extern void func_ov000_02071c08(void *, int);
void FieldHud_Init(void *field_context, int visible)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    int single = field->party->groups[field->party->active_group].active_members == 1;
    if (!visible)
        field->hud_y = 52;
    else
        field->hud_y = 0;
    field->hud.raw = (field->hud.raw & ~0x4000) | ((visible & 1) << 14);
    FieldHud_SetRenderersVisible(field, visible, single);
    FieldHud_UpdatePositions(field, single);
    FieldHud_SelectPortraits(field, single);
}

void FieldHud_SetRenderersVisible(void *field_context, int visible, int single)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    func_020093b4(&field->renderers[7]->animation.base, visible);
    func_020093b4(&field->renderers[8]->animation.base, single ? 0 : visible);
    func_020093b4(&field->renderers[6]->animation.base, visible);
    func_ov000_02071c08(field, single);
}
}
