#ifndef PIT_GAME_FIELD_ORBIT_H
#define PIT_GAME_FIELD_ORBIT_H

#include <game/field_entity.h>

#ifdef __cplusplus
extern "C" {
#endif

/* A null controller selects the entity's embedded orbit controller. */
void FieldOrbit_UpdatePosition(FieldRuntimeEntity *entity, FieldOrbitController *orbit);
int FieldOrbit_AdvanceFrame(FieldRuntimeEntity *entity, FieldOrbitController *orbit);
void FieldOrbit_Cancel(FieldRuntimeEntity *entity, FieldOrbitController *orbit, int snap_to_destination);
void FieldOrbit_Pause(FieldRuntimeEntity *entity, FieldOrbitController *orbit);
void FieldOrbit_Resume(FieldRuntimeEntity *entity, FieldOrbitController *orbit);
void FieldOrbit_CalculatePosition(FieldRuntimeEntity *entity, fx32 *x, fx32 *y, FieldOrbitController *orbit);
fx32 FieldOrbit_AdjustRadius(FieldRuntimeEntity *entity, fx32 radius, u16 angle, fx32 scale);

void FieldOrbit3D_UpdatePosition(FieldRuntimeEntity *entity, FieldOrbitController *orbit);
int FieldOrbit3D_AdvanceFrame(FieldRuntimeEntity *entity, FieldOrbitController *orbit);
int FieldOrbit3D_CheckStopMasks(FieldRuntimeEntity *entity, FieldOrbitController *orbit);
void FieldOrbit3D_Cancel(FieldRuntimeEntity *entity, FieldOrbitController *orbit, int snap_to_destination);
void FieldOrbit3D_CalculatePosition(FieldRuntimeEntity *entity, fx32 *x, fx32 *y, fx32 *z,
                                    FieldOrbitController *orbit);

/* relative_center == 1 adds the point offsets to the current position.
 * relative_angle selects a sweep; otherwise angle gives the destination plus
 * complete turns in its high word. Direction selects the winding sign. */
int FieldOrbit_StartAroundPoint(FieldRuntimeEntity *entity, int relative_center, fx32 offset_x, fx32 offset_y,
                                int relative_angle, s32 angle, fx32 speed, fx32 acceleration,
                                fx32 maximum_speed, fx32 deceleration, int direction, fx32 scale,
                                u8 stop_at_destination, FieldOrbitController *orbit);
int FieldOrbit_StartTimedAroundPoint(FieldRuntimeEntity *entity, int relative_center, fx32 offset_x,
                                     fx32 offset_y, int relative_angle, s32 angle, s32 duration,
                                     int direction, fx32 scale, u8 stop_at_destination,
                                     FieldOrbitController *orbit);
int FieldOrbit_StartAroundEntity(FieldRuntimeEntity *entity, FieldRuntimeEntity *center, fx32 offset_x,
                                 fx32 offset_y, int relative_angle, s32 angle, s32 speed, int direction,
                                 fx32 scale, u8 stop_at_destination, FieldOrbitController *orbit);
int FieldOrbit_StartTimedAroundEntity(FieldRuntimeEntity *entity, FieldRuntimeEntity *center, fx32 offset_x,
                                      fx32 offset_y, int relative_angle, s32 angle, s32 duration,
                                      int direction, fx32 scale, u8 stop_at_destination,
                                      FieldOrbitController *orbit);
int FieldOrbit3D_StartTimedAroundPoint(FieldRuntimeEntity *entity, int relative_center, fx32 offset_x,
                                       fx32 offset_y, fx32 offset_z, int relative_angle, s32 angle,
                                       s32 duration, int plane, int direction, fx32 scale, u32 stop_mask_a,
                                       u32 stop_mask_b, u8 stop_at_destination, FieldOrbitController *orbit);

#ifdef __cplusplus
}
#endif

#endif
