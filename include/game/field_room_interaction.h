#ifndef PIT_GAME_FIELD_ROOM_INTERACTION_H
#define PIT_GAME_FIELD_ROOM_INTERACTION_H

/*
 * What the interaction button acts on in the current room: collecting a placed
 * item or an entity's item, and the check that the required party member is
 * present.
 */
#include <nitro.h>

struct FieldAreaContext;
struct FieldRuntimeEntity;
struct FieldVariableEntity;
struct FieldVariablePlacement;

#ifdef __cplusplus
extern "C" {
#endif
void FieldArea_PlayItemCollectionSound(struct FieldAreaContext *field, int item, int source);
int FieldArea_CollectPlacementItem(struct FieldAreaContext *field,
                                   const struct FieldVariablePlacement *record);
const struct FieldVariablePlacement *FieldArea_FindUncollectedPlacement(struct FieldAreaContext *field,
                                                                        struct FieldRuntimeEntity *entity);
int FieldArea_CollectEntityItem(struct FieldAreaContext *field, struct FieldVariableEntity *entity);
void FieldArea_ShowEntityItemPickup(struct FieldAreaContext *field,
                                   struct FieldVariableEntity *entity, int added);
void FieldArea_EnsurePartyPresent(struct FieldAreaContext *field, int side, int x, int y, s16 z,
                                  int direction);
#ifdef __cplusplus
}
#endif
#endif
