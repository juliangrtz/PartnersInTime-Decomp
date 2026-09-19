/* Combine masked physical input with the field script's held/pressed input. */
#include <game/field_area.h>
#include <game/input.h>
extern "C" {
extern GameDisplayWork data_0206032c;
int func_ov000_0207ebc4(FieldAreaContext *, u16);
void FieldArea_ReadInput(FieldAreaContext *area, int mask, u16 *held, u16 *pressed, u8 mask_script)
{
    u16 physical_held = data_0206032c.input.held;
    int physical_pressed = data_0206032c.input.pressed;
    int enabled = mask & FieldArea_GetCommonInputMask(area);
    int rotated_held = func_ov000_0207ebc4(area, physical_held & enabled);
    int rotated_pressed = func_ov000_0207ebc4(area, physical_pressed & enabled);
    int result_held, result_pressed;
    if (mask_script) {
        result_held = (u16)(rotated_held | (area->scripted_held & area->scripted_held_mask));
        result_pressed =
            (u16)(rotated_pressed | (area->scripted_pressed & area->scripted_pressed_mask));
    } else {
        result_held = rotated_held | area->scripted_held;
        result_pressed = rotated_pressed | area->scripted_pressed;
    }
    *held = result_held;
    *pressed = result_pressed;
}
}
