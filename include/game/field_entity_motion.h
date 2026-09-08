#ifndef PIT_GAME_FIELD_ENTITY_MOTION_H
#define PIT_GAME_FIELD_ENTITY_MOTION_H

#include <game/field_entity.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The per-frame deltas are accumulated before the entity update applies them. */
void FieldEntity2D_AccumulateMotion(FieldRuntimeEntity *entity, fx32 x, fx32 y);
void FieldEntity3D_AccumulateMotion(FieldRuntimeEntity *entity, fx32 x, fx32 y, fx32 z);
void FieldEntity_UpdateLocomotionState(FieldRuntimeEntity *entity);
void FieldEntity2D_UpdateLocomotionState(FieldRuntimeEntity *entity);
void FieldEntity3D_UpdateLocomotionState(FieldRuntimeEntity *entity);
void FieldEntity2D_MapLocomotionState(FieldRuntimeEntity *entity);
void FieldEntity3D_MapLocomotionState(FieldRuntimeEntity *entity);

/* -1 selects stored defaults. A zero launch velocity starts falling using the
 * default gravity. StartToHeight preserves the original discrete-step solver. */
void FieldVertical_Start(FieldRuntimeEntity *entity, fx32 velocity, fx32 gravity, fx32 terminal_velocity);
void FieldVertical_StartToHeight(FieldRuntimeEntity *entity, fx32 height, fx32 gravity, fx32 terminal_velocity);
void FieldVertical_Update(FieldRuntimeEntity *entity);
void FieldVertical_Stop(FieldRuntimeEntity *entity);

/* BeginFrame snapshots previous state and clears this frame's motion/contact accumulators. */
void FieldEntity2D_BeginFrame(FieldRuntimeEntity *entity);
void FieldEntity3D_BeginFrame(FieldRuntimeEntity *entity);
void FieldEntity3D_SetPosition(FieldRuntimeEntity *entity, fx32 x, fx32 y, fx32 z);
void FieldEntity_FaceTarget(FieldRuntimeEntity *entity, FieldRuntimeEntity *target);
void FieldEntity3D_UpdateSupportClearance(FieldRuntimeEntity *entity);
void FieldEntity3D_SetNavigationSurfaces(FieldRuntimeEntity *entity, FieldNavigationSurface *surfaces,
                                         const void *resource);

#ifdef __cplusplus
}
#endif
#endif
