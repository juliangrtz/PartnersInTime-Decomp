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

#ifdef __cplusplus
}
#endif

#endif
