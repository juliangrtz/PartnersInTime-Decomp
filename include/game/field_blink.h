#ifndef PIT_GAME_FIELD_BLINK_H
#define PIT_GAME_FIELD_BLINK_H

#include <game/field_entity.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Signed duration pairs end at a -1 sentinel; mode 3 advances through pairs. */
void FieldBlink_Start(FieldRuntimeEntity *entity, int mode, const s8 *durations, u8 length,
                     FieldEntityVisibilityCallback first_phase, FieldEntityVisibilityCallback second_phase);
void FieldBlink_Update(FieldRuntimeEntity *entity);
/* -1 uses the current mode; -2 stops without invoking a visibility callback. */
void FieldBlink_Stop(FieldRuntimeEntity *entity, int final_mode);
void FieldBlink_StartRenderers(FieldRuntimeEntity *entity, int mode, const s8 *durations, u8 length,
                              FieldEntityVisibilityCallback show, FieldEntityVisibilityCallback hide);
void FieldBlink_ShowRenderers(FieldEntity *entity);
void FieldBlink_HideRenderers(FieldEntity *entity);
void FieldEntity_CopySortKeyToAnchor(FieldRuntimeEntity *entity);
void FieldEntity_CopySortKeysToAnchors(FieldRuntimeEntity *entity);

#ifdef __cplusplus
}
#endif

#endif
