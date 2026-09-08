#include <game/field_area.h>

extern const void *data_ov000_020c0e20[];
void func_0202cbd4(void *destination, int value, unsigned size);

FieldAreaContext *FieldArea_Construct(FieldAreaContext *field)
{
    field->vtable = data_ov000_020c0e20;
    FieldScriptManager_Construct(&field->scripts);
    func_0202cbd4((u8 *)field + 4, 0, sizeof(*field) - 4);
    return field;
}

FieldAreaContext *FieldArea_Destroy(FieldAreaContext *field)
{
    field->vtable = data_ov000_020c0e20;
    FieldScriptManager_Destroy(&field->scripts);
    return field;
}

void FieldArea_CaptureSnapshot(const FieldAreaContext *field, FieldAreaSnapshot *snapshot)
{
    snapshot->room_id = field->room_id;
    snapshot->flags.party_active = field->flags.party_active;
    snapshot->flags.mode = field->flags.mode;
    snapshot->flags.unknown_03 = field->flags.unknown_15;
    snapshot->flags.area_value = field->area_value;
    snapshot->flags.hud_visible = field->hud.visible;
    snapshot->camera_x = field->camera_x;
    snapshot->camera_y = field->camera_y;
    snapshot->unknown_0c = field->unknown_24c0;
}
