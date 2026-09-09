#ifndef PIT_GAME_FIELD_ROOM_INTERACTION_H
#define PIT_GAME_FIELD_ROOM_INTERACTION_H
#include <nitro.h>

struct FieldAreaContext;
struct FieldRuntimeEntity;
struct FieldVariableEntity;
struct FieldVariablePlacement;

#ifdef __cplusplus
extern "C" {
#endif
int FieldArea_CollectPlacementItem(struct FieldAreaContext *field,
                                   const struct FieldVariablePlacement *record);
const struct FieldVariablePlacement *FieldArea_FindUncollectedPlacement(struct FieldAreaContext *field,
                                                                        struct FieldRuntimeEntity *entity);
int FieldArea_CollectEntityItem(struct FieldAreaContext *field, struct FieldVariableEntity *entity);
void FieldArea_EnsurePartyPresent(struct FieldAreaContext *field, int side, int x, int y, s16 z,
                                  int direction);
#ifdef __cplusplus
}
#endif
#endif
