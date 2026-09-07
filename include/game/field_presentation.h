#ifndef PIT_GAME_FIELD_PRESENTATION_H
#define PIT_GAME_FIELD_PRESENTATION_H

#include <game/field_entity.h>

#ifdef __cplusplus
extern "C" {
#endif
void FieldEntity_SetFacingDirection(FieldRuntimeEntity *entity, int relative, int direction, int refresh);
void FieldEntity_SetBehaviorMode(FieldEntity *entity, int mode);
void FieldEntity_SetInteractionBounds(FieldEntity *entity, s16 minimum_x, s16 maximum_y,
                                     u16 width, u16 height, u16 vertical_extent);
#ifdef __cplusplus
}
#endif

#endif
