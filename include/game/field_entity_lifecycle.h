#ifndef PIT_GAME_FIELD_ENTITY_LIFECYCLE_H
#define PIT_GAME_FIELD_ENTITY_LIFECYCLE_H

#include <game/field_entity.h>
#include <game/field_timed_renderer.h>
#include <game/field_spawn.h>

/* Allocation sizes of the planar base and its spatial extension.
 * FieldRuntimeEntity describes their shared command-facing offsets;
 * only the spatial extension owns fields beyond the planar allocation. */
enum { FIELD_ENTITY_2D_SIZE = 0x2B0, FIELD_ENTITY_3D_SIZE = 0x520 };

#ifdef __cplusplus
extern "C" {
#endif

FieldRuntimeEntity *FieldEntity2D_InitPlacement(FieldRuntimeEntity *entity, int entity_index,
    const FieldSpawnRecord *spawn, int argument_3, int resource_set, u8 animation_enabled);
FieldRuntimeEntity *FieldEntity2D_InitPlacementBase(FieldRuntimeEntity *entity, int entity_index,
    const FieldSpawnRecord *spawn, int argument_3, int resource_set, u8 animation_enabled);
void FieldEntity2D_UpdateMotionAndAnimation(FieldRuntimeEntity *entity);
void FieldEntity2D_SetPosition(FieldRuntimeEntity *entity, fx32 x, fx32 y);
FieldRuntimeEntity *FieldEntity2D_Init(FieldRuntimeEntity *entity);
FieldRuntimeEntity *FieldEntity2D_InitBase(FieldRuntimeEntity *entity);
FieldRuntimeEntity *FieldEntity2D_Destroy(FieldRuntimeEntity *entity);
FieldRuntimeEntity *FieldEntity2D_Delete(FieldRuntimeEntity *entity);
FieldRuntimeEntity *FieldEntity2D_DestroyBase(FieldRuntimeEntity *entity);
void FieldEntity_ResetLocomotionParameters(FieldRuntimeEntity *entity);
void FieldEntity_SetLocomotionParameters(FieldRuntimeEntity *entity, fx32 starting_speed, fx32 acceleration,
                       fx32 maximum_speed, fx32 deceleration, fx32 reverse_deceleration, fx32 turn_speed_limit);
void FieldEntity2D_AllocateRenderer(FieldRuntimeEntity *entity);
FieldAnimationRenderer *FieldAnimationRenderer_Delete(FieldAnimationRenderer *renderer);
void FieldEntity2D_ReleaseRenderer(FieldRuntimeEntity *entity);
FieldRuntimeEntity *FieldEntity3D_InitPlacement(FieldRuntimeEntity *entity, int entity_index,
    const FieldSpawnRecord *spawn, int argument_3, int resource_set, u8 animation_enabled);
FieldRuntimeEntity *FieldEntity3D_InitPlacementBase(FieldRuntimeEntity *entity, int entity_index,
    const FieldSpawnRecord *spawn, int argument_3, int resource_set, u8 animation_enabled);
FieldRuntimeEntity *FieldEntity3D_Init(FieldRuntimeEntity *entity);
FieldRuntimeEntity *FieldEntity3D_InitBase(FieldRuntimeEntity *entity);
FieldRuntimeEntity *FieldEntity3D_Destroy(FieldRuntimeEntity *entity);
FieldRuntimeEntity *FieldEntity3D_Delete(FieldRuntimeEntity *entity);
FieldRuntimeEntity *FieldEntity3D_DestroyBase(FieldRuntimeEntity *entity);
void FieldEntity3D_ResetMotionParameters(FieldRuntimeEntity *entity);
void FieldEntity_SetVerticalParameters(FieldRuntimeEntity *entity, fx32 launch_velocity, fx32 gravity, fx32 terminal_velocity);
void FieldEntity3D_AllocateRenderers(FieldRuntimeEntity *entity);
void FieldEntity3D_ReleaseRenderers(FieldRuntimeEntity *entity);

#ifdef __cplusplus
}
#endif

#endif
