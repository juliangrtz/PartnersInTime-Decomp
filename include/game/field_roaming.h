#ifndef PIT_GAME_FIELD_ROAMING_H
#define PIT_GAME_FIELD_ROAMING_H
#include <game/field_entity.h>
#ifdef __cplusplus
extern "C" {
#endif
void FieldEntity_SelectBodyContact(FieldRuntimeEntity *entity, FieldRuntimeEntity *contact, u8 mask);
/* Group bits map to native flags 0, 2/3, 1, 6 and 4, respectively.
 * Individual flag behavior remains unnamed pending runtime evidence. */
void FieldEntity_SetCollisionFlagGroups(FieldRuntimeEntity *entity, u8 mask, u8 enabled);
/* A value of -1 preserves the corresponding group. */
void FieldEntity_UpdateCollisionFlagGroups(FieldRuntimeEntity *entity, int flag0, int flag23, int flag1,
                                           int flag6, int flag4);
void FieldEntity_LoadBodyBounds(FieldRuntimeEntity *entity);
void FieldRoaming_Update(FieldRuntimeEntity *entity);
void FieldRoaming_Resume(FieldRuntimeEntity *entity);
void FieldRoaming_Pause(FieldRuntimeEntity *entity);
void FieldRoaming_Stop(FieldRuntimeEntity *entity);
void FieldPath_Advance(FieldRuntimeEntity *entity);
void FieldPath_Start(FieldRuntimeEntity *entity);
void FieldPath_Clear(FieldRuntimeEntity *entity);
#ifdef __cplusplus
}
#endif
#endif
