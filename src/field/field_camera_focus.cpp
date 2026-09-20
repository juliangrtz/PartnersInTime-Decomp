/*
 * Camera focus transitions (overlay 0, 0x02073740-0x02073860).
 * Move toward an entity at twelve pixels per frame, then resume following it.
 * Completion can start an auxiliary room script; -1 means no script.
 */

#include <game/field_area_motion.h>
#include <game/field_entity.h>
#include <game/field_script_manager.h>
extern "C" void FieldArea_BeginCameraFocus(FieldAreaContext *area, int entity_index, s16 script)
{
    FieldRuntimeEntity *entity = area->entities[entity_index];
    area->pending_camera_target = entity_index;
    area->camera_target_index = -1;
    area->flags.unknown_15 = 0;
    area->camera_completion_script = script;
    FieldArea_MoveCameraWithProfile(area, 0, entity->position_x - 0x80000,
                                    entity->position_y - entity->position_z - 0x88000, 0xc000, 0,
                                    0xc000, 0, 1, 1, (FieldCameraMotion *)area->unknown_2b5c);
}
extern "C" void FieldArea_CompleteCameraFocus(FieldAreaContext *area)
{
    area->flags.unknown_15 = 1;
    area->camera_target_index = area->pending_camera_target;
    area->pending_camera_target = -1;
    FieldRuntimeEntity *entity = area->entities[area->camera_target_index];
    area->unknown_2468[0] = entity->position_x;
    area->unknown_2468[1] = entity->position_y - entity->position_z;
    if (area->camera_completion_script != -1) {
        FieldScriptManager_StartAuxiliary(&area->scripts, 0, area->camera_completion_script);
        area->camera_completion_script = -1;
    }
}
