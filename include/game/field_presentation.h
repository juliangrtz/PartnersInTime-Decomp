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

/* Bounds resources contain six relative u16 offsets; each section begins with
 * a count byte followed by its records. Binding refreshes the entity bounds. */
void FieldEntity2D_BindBoundsResource(FieldRuntimeEntity *entity, const u16 *resource);
void FieldEntity3D_BindBoundsResource(FieldRuntimeEntity *entity, const u16 *resource);
void FieldEntity_SetResourceAnimation(FieldRuntimeEntity *entity, int resource_animation, int animation,
                                       int reset_control, u8 update_bounds);
void FieldEntity_StopRenderer(FieldRuntimeEntity *entity);
#ifdef __cplusplus
}
#endif

#endif
