/* Snapshot room/camera state before each active field frame and reset per-frame offsets. */
#include <game/field_entity.h>
#include <game/field_area.h>
extern "C" void FieldArea_BeginFrame(FieldAreaContext *area)
{
    area->unknown_23f2 = area->unknown_23f0;
    if (area->unknown_23f0 == 71) {
        area->unknown_243c[0] = area->camera_x;
        area->unknown_243c[1] = area->camera_y;
        area->unknown_244c[2] = area->unknown_244c[0];
        area->unknown_244c[3] = area->unknown_244c[1];
        area->unknown_2454[2] = area->unknown_2454[0];
        area->unknown_2454[3] = area->unknown_2454[1];
        area->unknown_2424[2] = area->unknown_2424[0];
        area->unknown_2424[3] = area->unknown_2424[1];
        area->unknown_2424[0] = 0;
        area->unknown_2424[1] = 0;
        for (int i = 0; i < area->entity_count; ++i) {
            FieldRuntimeEntity *entity = area->entities[i];
            if (entity && !entity->base.visibility_bits.unknown_08)
                entity->base.unknown_08();
        }
    }
}
