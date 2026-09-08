extern "C" {
#include <nitro/fx.h>
#include <game/rumble.h>
#include <game/save_data.h>
}
#include <game/field_hud.h>
extern "C" {
extern void func_020093b4(FieldRenderObject *, int);
extern u8 data_0205a00c;
void FieldCamera_StopShake(void *field_context)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    field->shake.active = 0;
    if (!field->shake.vertical)
        field->camera_x = field->shake_origin;
    else
        field->camera_y = field->shake_origin;
    if (field->shake.rumble > 0 && data_0205a00c)
        GameRumble_Stop();
}
}
