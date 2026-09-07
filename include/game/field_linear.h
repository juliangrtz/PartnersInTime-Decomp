#ifndef PIT_GAME_FIELD_LINEAR_H
#define PIT_GAME_FIELD_LINEAR_H

#include <game/field_entity.h>

#ifdef __cplusplus
extern "C" {
#endif

/* A null controller selects the embedded controller at +0x1F4. */
void FieldLinear_UpdatePosition(FieldRuntimeEntity *entity, FieldLinearController *movement);
int FieldLinear_CheckCompletion(FieldRuntimeEntity *entity, FieldLinearController *movement);
void FieldLinear_Cancel(FieldRuntimeEntity *entity, FieldLinearController *movement, int snap_to_destination);
void FieldLinear_Pause(FieldRuntimeEntity *entity, FieldLinearController *movement);
void FieldLinear_Resume(FieldRuntimeEntity *entity, FieldLinearController *movement);

void FieldLinear3D_UpdatePosition(FieldRuntimeEntity *entity, FieldLinearController *movement);
int FieldLinear3D_CheckCompletion(FieldRuntimeEntity *entity, FieldLinearController *movement);
int FieldLinear3D_CheckStopMasks(FieldRuntimeEntity *entity, FieldLinearController *movement);
void FieldLinear3D_Cancel(FieldRuntimeEntity *entity, FieldLinearController *movement,
                          int snap_to_destination);

/* relative == 1 interprets point coordinates as offsets from the entity.
 * Timed movement retains the caller's duration; speed zero selects defaults. */
int FieldLinear_Start(FieldRuntimeEntity *entity, int relative, fx32 x, fx32 y, fx32 speed, fx32 acceleration,
                      fx32 maximum_speed, fx32 deceleration, u8 stop, FieldLinearController *movement);
int FieldLinear_StartTimed(FieldRuntimeEntity *entity, int relative, fx32 x, fx32 y, s32 duration, u8 stop,
                           FieldLinearController *movement);
int FieldLinear_StartFollowing(FieldRuntimeEntity *entity, FieldRuntimeEntity *target, fx32 x, fx32 y,
                               fx32 speed, u8 stop, FieldLinearController *movement);
int FieldLinear_StartTimedFollowing(FieldRuntimeEntity *entity, FieldRuntimeEntity *target, fx32 x, fx32 y,
                                    u32 duration, u8 stop, FieldLinearController *movement);

/* Spatial starters suppress the Z axis while tracking ground. Masks are
 * truncated to the controller's six-bit contact/state fields. */
int FieldLinear3D_Start(FieldRuntimeEntity *entity, int relative, fx32 x, fx32 y, fx32 z, fx32 speed, fx32 acceleration,
                      fx32 maximum_speed, fx32 deceleration, u32 stop_mask_a, u32 stop_mask_b, u8 stop, FieldLinearController *movement);
int FieldLinear3D_StartTimed(FieldRuntimeEntity *entity, int relative, fx32 x, fx32 y, fx32 z, s32 duration, u32 stop_mask_a, u32 stop_mask_b, u8 stop,
                           FieldLinearController *movement);
int FieldLinear3D_StartFollowing(FieldRuntimeEntity *entity, FieldRuntimeEntity *target, fx32 x, fx32 y, fx32 z,
                               fx32 speed, u32 stop_mask_a, u32 stop_mask_b, u8 stop, FieldLinearController *movement);
int FieldLinear3D_StartTimedFollowing(FieldRuntimeEntity *entity, FieldRuntimeEntity *target, fx32 x, fx32 y, fx32 z,
                                    u32 duration, u32 stop_mask_a, u32 stop_mask_b, u8 stop, FieldLinearController *movement);

#ifdef __cplusplus
}
#endif
#endif
