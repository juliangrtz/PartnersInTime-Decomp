#ifndef PARTNERS_IN_TIME_GAME_FIELD_TRANSFORM_H
#define PARTNERS_IN_TIME_GAME_FIELD_TRANSFORM_H
#include <game/field_entity.h>
#ifdef __cplusplus
extern "C" {
#endif
/* A null controller selects the entity's embedded storage. */
void FieldEntity_DisableAnimation(FieldEntity *entity);
void FieldEntity_EnableAnimation(FieldEntity *entity);
int FieldScaling_Start(FieldRuntimeEntity *entity, int relative, int x, int y, int speed_x, int speed_y,
                       FieldTransformController *controller);
int FieldScaling_StartTimed(FieldRuntimeEntity *entity, int relative, int x, int y, int duration_x,
                            int duration_y, FieldTransformController *controller);
void FieldScaling_Stop(FieldRuntimeEntity *entity, FieldTransformController *controller, int snap);
void FieldScaling_Pause(FieldRuntimeEntity *entity, FieldTransformController *controller);
void FieldScaling_Resume(FieldRuntimeEntity *entity, FieldTransformController *controller);
int FieldRotation_StartTimed(FieldRuntimeEntity *entity, int relative, u32 angle, int duration, int direction,
                             u8 snap, FieldTransformController *controller);
void FieldRotation_Stop(FieldRuntimeEntity *entity, FieldTransformController *controller, int snap);
void FieldTransform_Advance(FieldRuntimeEntity *entity, FieldTransformController *controller);
int FieldTransform_CheckCompletion(FieldRuntimeEntity *entity, FieldTransformController *controller);
#ifdef __cplusplus
}
#endif
#endif
