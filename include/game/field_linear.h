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

#ifdef __cplusplus
}
#endif
#endif
